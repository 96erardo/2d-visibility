#include <SFML/System/Angle.hpp>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <array>
#include "Segment.h"
#include "Game.h"
#include "Vec2.h"

Game::Game (const std::string& path) {
  init(path);
}

void Game::init (const std::string& path) {
  m_window = sf::RenderWindow(sf::VideoMode({800, 600}), "Line-Line Collision");
  m_window.setFramerateLimit(60);
  m_window.setMouseCursorVisible(false);

  std::srand(std::time(0));

  std::ifstream file(path, std::ios::in);
  std::string command;
  std::vector<Vec2> points;
  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;

  m_lightSource = Vec2(400.0f, 300.0f);

  m_points.push_back(Vertex(0,0));
  m_points.push_back(Vertex(800,0));
  m_points.push_back(Vertex(800,600));
  m_points.push_back(Vertex(0,600));

  while (file >> command) {
    if (command == "Box") {
      file >> x >> y >> width >> height;

      auto entity = m_entities.addEntity("Box");

      entity->addComponent<CTransform>(x, y);
      entity->addComponent<CRect>(x + width / 2, y + height / 2, width, height);
      entity->getComponent<CRect>().rect.setFillColor(sf::Color(68,68,68));

      m_segments.push_back(Segment(x, y, x + width, y));
      m_segments.push_back(Segment(x + width, y, x + width, y + height));
      m_segments.push_back(Segment(x + width, y + height, x, y + height));
      m_segments.push_back(Segment(x, y + height, x, y));
    }
  }

  for (auto& s : m_segments) {
    if (
      std::find_if(m_points.begin(), m_points.end(), [s](Vertex p) -> bool { return p.p == s.p1; } ) == m_points.end()
    ) {
      Vertex v(s.p1.x, s.p1.y);

      m_points.push_back(v);
      m_points.push_back(Vertex::fromAngleOffset(m_lightSource.x, m_lightSource.y, v, 0.001));
      m_points.push_back(Vertex::fromAngleOffset(m_lightSource.x, m_lightSource.y, v, -0.001));
    }

    if (
      std::find_if(m_points.begin(), m_points.end(), [s](Vertex p) -> bool { return p.p == s.p2; } ) == m_points.end()
    ) {
      Vertex v(s.p2.x, s.p2.y);

      m_points.push_back(v);
      m_points.push_back(Vertex::fromAngleOffset(m_lightSource.x, m_lightSource.y, v, 0.001));
      m_points.push_back(Vertex::fromAngleOffset(m_lightSource.x, m_lightSource.y, v, -0.001));
    }
  }

  for (auto& point : m_points) {
    auto triangle = m_entities.addEntity("Triangle");

    triangle->addComponent<CTriangle>();
  }
}

void Game::run () {
  while (m_window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = m_window.pollEvent()) {
      // "close requested" event: we close the window
      if (const auto* mouseMoved =  event->getIf<sf::Event::MouseMoved>()) {
        m_lightSource = Vec2(mouseMoved->position.x, mouseMoved->position.y);
      
      } else if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
    }

    update();
  }
}

void Game::sMovement () {
  for (auto& point : m_points) {
    point.calcAngle(m_lightSource.x, m_lightSource.y);
  }

  std::sort(m_points.begin(), m_points.end(), [](Vertex a, Vertex b) -> bool { return a.angle < b.angle; });
}

void Game::sCollision () {
  for (size_t i = 0; i < m_points.size(); i++) {
    float x1 = m_lightSource.x;
    float y1 = m_lightSource.y;
    float x2 = m_points.at(i).p.x;
    float y2 = m_points.at(i).p.y;
    float closest = 300;

    for (auto& segment : m_segments) {
      float x3 = segment.p1.x;
      float y3 = segment.p1.y;
      float x4 = segment.p2.x;
      float y4 = segment.p2.y;

      float a = x2 - x1;
      float b = -(x4 - x3);
      float c = y2 - y1;
      float d = -(y4 - y3);
      float e = x3 - x1;
      float f = y3 - y1;

      float det = (a * d) - (b * c);

      if (det == 0) {
        continue;
      }

      float detX = (e * d) - (b * f);
      float detY = (a * f) - (e * c);
      
      float t = detX / det;
      float s = detY / det;

      if (
        (t >= 0) &&
        (s >= 0 && s <= 1)
      ) {
        closest = std::min(closest, t);
      }
    }

    if (closest >= 0) {
      float x = x1 + (closest * (x2 - x1));
      float y = y1 + (closest * (y2 - y1));

      m_points.at(i).intersection = Vec2(x, y);
    }
  }

  std::vector<std::shared_ptr<Entity>> triangles = m_entities.getEntities("Triangle");

  for (size_t i = 0; i < triangles.size(); i++) {
    auto& triangle = triangles.at(i)->getComponent<CTriangle>().triangle;

    triangle[0].position.x = i == 0 ? m_points.back().intersection.x : m_points.at(i - 1).intersection.x;
    triangle[0].position.y = i == 0 ? m_points.back().intersection.y : m_points.at(i - 1).intersection.y;
    triangle[1].position.x = m_lightSource.x;
    triangle[1].position.y = m_lightSource.y;
    triangle[2].position.x = m_points.at(i).intersection.x;
    triangle[2].position.y = m_points.at(i).intersection.y;
  }
}

void Game::sRender () {
  m_window.clear(sf::Color(154, 154, 154)); 

  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CRect>()) {
      m_window.draw(entity->getComponent<CRect>().rect);
    }

    if (entity->hasComponent<CTriangle>()) {
      m_window.draw(entity->getComponent<CTriangle>().triangle);
    }
  }

  m_window.display();
}

void Game::update () {
  m_entities.update();

  sMovement();
  sCollision();
  sRender();
}