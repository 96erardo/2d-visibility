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
  m_window.setMouseCursorVisible(false);

  std::srand(std::time(0));

  std::ifstream file(path, std::ios::in);
  std::string command;
  std::vector<Vec2> points;
  float playerX = 400.0f;
  float playerY = 300.0f;
  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;

  while (file >> command) {
    if (command == "Box") {
      file >> x >> y >> width >> height;

      auto entity = m_entities.addEntity("Box");

      entity->addComponent<CTransform>(x, y);
      entity->addComponent<CRect>(x, y, width, height);

      m_segments.push_back(Segment(x, y, x + width, y));
      m_segments.push_back(Segment(x + width, y, x + width, y + height));
      m_segments.push_back(Segment(x + width, y + height, x, y + height));
      m_segments.push_back(Segment(x, y + height, x, y));
    }
  }

  auto player = m_entities.addEntity("Player");
  
  player->addComponent<CTransform>(playerX, playerY);
  player->addComponent<CRect>(playerX, playerY, 12, 12);
  player->getComponent<CRect>().rect.setOrigin({ 6, 6 });
  player->getComponent<CShape>().rect.setPosition({ playerX, playerY });
  player->getComponent<CShape>().rect.setFillColor(sf::Color::Red);

  m_player = player;
}

void Game::run () {
  while (m_window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = m_window.pollEvent()) {
      // "close requested" event: we close the window
      if (const auto* mouseMoved =  event->getIf<sf::Event::MouseMoved>()) {
        m_player->getComponent<CTransform>().pos = Vec2(mouseMoved->position.x, mouseMoved->position.y);
      
      } else if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
    }

    update();
  }
}

void Game::sMovement () {
  m_player->getComponent<CShape>().rect.setPosition({
    m_player->getComponent<CTransform>().pos.x,
    m_player->getComponent<CTransform>().pos.y
  });

  Vec2 pos = m_player->getComponent<CPosition>().pos;

  for (auto& segment : m_segments) {
    segment.p1.calcAngle(pos.x, pos.y);
    segment.p1.calcDistance(pos.x, pos.y);
    segment.p2.calcAngle(pos.x, pos.y);
    segment.p2.calcDistance(pos.x, pos.y);

    m_points.push_back(segment.p1);
    m_points.push_back(segment.p2);
  }

  std::sort(m_points.begin(), m_points.end(), [](Vertex a, Vertex b) { return a.angle < b.angle });

  
}

void Game::sCollision () {
  for (auto line : m_entities.getEntities("Ray")) {
    float x1 = line->getComponent<CLine>().p1.x;
    float y1 = line->getComponent<CLine>().p1.y;
    float x2 = line->getComponent<CLine>().p2.x;
    float y2 = line->getComponent<CLine>().p2.y;
    float closest = 300;

    for (auto wall : m_entities.getEntities("Wall")) {
      float x3 = wall->getComponent<CLine>().p1.x;
      float y3 = wall->getComponent<CLine>().p1.y;
      float x4 = wall->getComponent<CLine>().p2.x;
      float y4 = wall->getComponent<CLine>().p2.y;

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

      line->getComponent<CLine>().line[1].position.x = x;
      line->getComponent<CLine>().line[1].position.y = y;
    }
  }
}

void Game::sRender () {
  m_window.clear(sf::Color(51, 54, 68)); 

  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CRect>()) {
      m_window.draw(entity->getComponent<CRect>().rect);
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