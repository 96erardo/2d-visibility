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
  m_window = sf::RenderWindow(sf::VideoMode({1280, 920}), "2D Visibility");
  m_window.setFramerateLimit(60);
  m_window.setMouseCursorVisible(false);

  std::srand(std::time(0));

  std::ifstream file(path, std::ios::in);
  std::string command;
  std::vector<Vec2> points;
  float x1 = 0;
  float y1 = 0;
  float x2 = 0;
  float y2 = 0;
  float x3 = 0;
  float y3 = 0;
  float x4 = 0;
  float y4 = 0;
  float playerX = 487.0f;
  float playerY = 728.0f;

  m_points.push_back(Ray(0,0));
  m_points.push_back(Ray(1280,0));
  m_points.push_back(Ray(1280,920));
  m_points.push_back(Ray(0,920));
  m_segments.push_back(Segment(0, 0, 1280, 0));
  m_segments.push_back(Segment(1280, 0, 1280, 920));
  m_segments.push_back(Segment(1280, 920, 0, 920));
  m_segments.push_back(Segment(0, 920, 0, 0));

  while (file >> command) {
    if (command == "Box") {
      file >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

      auto entity = m_entities.addEntity("Box");

      entity->addComponent<CShape>(4);
      entity->getComponent<CShape>().shape.setPoint(0, {x1, y1});
      entity->getComponent<CShape>().shape.setPoint(1, {x2, y2});
      entity->getComponent<CShape>().shape.setPoint(2, {x3, y3});
      entity->getComponent<CShape>().shape.setPoint(3, {x4, y4});
      entity->getComponent<CShape>().shape.setFillColor(sf::Color(68,68,68));
      
      m_segments.push_back(Segment(x1, y1, x2, y2));
      m_segments.push_back(Segment(x2, y2, x3, y3));
      m_segments.push_back(Segment(x3, y3, x4, y4));
      m_segments.push_back(Segment(x4, y4, x1, y1));
    
    } if (command == "Triangle") {
      file >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

      auto entity = m_entities.addEntity("Box");

      entity->addComponent<CShape>(3);
      entity->getComponent<CShape>().shape.setPoint(0, {x1, y1});
      entity->getComponent<CShape>().shape.setPoint(1, {x2, y2});
      entity->getComponent<CShape>().shape.setPoint(2, {x3, y3});
      entity->getComponent<CShape>().shape.setFillColor(sf::Color(68,68,68));

      m_segments.push_back(Segment(x1, y1, x2, y2));
      m_segments.push_back(Segment(x2, y2, x3, y3));
      m_segments.push_back(Segment(x3, y3, x1, y1));
    }
  }

  for (auto& s : m_segments) {
    if (
      std::find_if(m_points.begin(), m_points.end(), [s](Ray p) -> bool { return p.to == s.p1; } ) == m_points.end()
    ) {
      Ray v(s.p1.x, s.p1.y);

      m_points.push_back(v);
      m_points.push_back(Ray::fromAngleOffset(playerX, playerY, v, 0.001));
      m_points.push_back(Ray::fromAngleOffset(playerX, playerY, v, -0.001));
    }

    if (
      std::find_if(m_points.begin(), m_points.end(), [s](Ray p) -> bool { return p.to == s.p2; } ) == m_points.end()
    ) {
      Ray v(s.p2.x, s.p2.y);

      m_points.push_back(v);
      m_points.push_back(Ray::fromAngleOffset(playerX, playerY, v, 0.001));
      m_points.push_back(Ray::fromAngleOffset(playerX, playerY, v, -0.001));
    }
  }

  for (auto& point : m_points) {
    auto triangle = m_entities.addEntity("Light");

    triangle->addComponent<CShape>(3);
    triangle->getComponent<CShape>().shape.setFillColor(sf::Color::Transparent);
    triangle->getComponent<CShape>().shape.setFillColor(sf::Color(207, 207, 207));
  }

  auto player = m_entities.addEntity("Player");
  player->addComponent<CTransform>(playerX, playerY);
  player->addComponent<CCircle>(10);
  player->getComponent<CCircle>().circle.setFillColor(sf::Color::Yellow);
 
  m_player = player;
}

void Game::run () {
  while (m_window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = m_window.pollEvent()) {
      // "close requested" event: we close the window
      if (const auto* mouseMoved =  event->getIf<sf::Event::MouseMoved>()) {
        m_player->getComponent<CTransform>().pos = Vec2(mouseMoved->position.x, mouseMoved->position.y);
      
      } else if (const auto* mouseClicked = event->getIf<sf::Event::MouseButtonReleased>()) {
        std::cout << mouseClicked->position.x << " " << mouseClicked->position.y << "\n";

      } else if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
    }

    update();
  }
}

void Game::sMovement () {
  auto& pos = m_player->getComponent<CTransform>().pos;

  m_player->getComponent<CCircle>().circle.setPosition({pos.x,pos.y});

  for (auto& point : m_points) {
    point.calcAngle(pos.x, pos.y);
  }

  std::sort(m_points.begin(), m_points.end(), [](Ray a, Ray b) -> bool { return a.angle < b.angle; });
}

void Game::sCollision () {
  auto& pos = m_player->getComponent<CTransform>().pos;

  for (size_t i = 0; i < m_points.size(); i++) {
    float x1 = pos.x;
    float y1 = pos.y;
    float x2 = m_points.at(i).to.x;
    float y2 = m_points.at(i).to.y;
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

  std::vector<std::shared_ptr<Entity>> triangles = m_entities.getEntities("Light");

  for (size_t i = 0; i < triangles.size(); i++) {
    auto& triangle = triangles.at(i)->getComponent<CShape>().shape;

    triangle.setPoint(0, {
      i == 0 ? m_points.back().intersection.x : m_points.at(i - 1).intersection.x,
      i == 0 ? m_points.back().intersection.y : m_points.at(i - 1).intersection.y
    });

    triangle.setPoint(1, {
      m_points.at(i).intersection.x,
      m_points.at(i).intersection.y
    });

    triangle.setPoint(2, {
      pos.x,
      pos.y
    });
  }
}

void Game::sRender () {
  m_window.clear(sf::Color::Black);

  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CShape>()) {
      m_window.draw(entity->getComponent<CShape>().shape);
    }

    if (entity->hasComponent<CCircle>()) {
      m_window.draw(entity->getComponent<CCircle>().circle);
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