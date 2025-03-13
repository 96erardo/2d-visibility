#include <SFML/System/Angle.hpp>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <array>
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
  float playerX = 100.0f;
  float playerY = 100.0f;
  float x1 = 0;
  float y1 = 0;
  float x2 = 0;
  float y2 = 0;

  while (file >> command) {
    if (command == "Wall") {
      file >> x1 >> y1 >> x2 >> y2;

      auto entity = m_entities.addEntity("Wall");

      entity->addComponent<CLine>(
        Vec2(x1, y1),
        Vec2(x2, y2)
      );

      entity->getComponent<CLine>().line[0].color = sf::Color::White;
      entity->getComponent<CLine>().line[1].color = sf::Color::White;

      if (
        std::find_if(points.begin(), points.end(), [x1, y1](Vec2 p) -> bool { return p == Vec2(x1, y1); } ) == points.end()
      ) {
        points.push_back(Vec2(x1, y1));
      }

      if (
        std::find_if(points.begin(), points.end(), [x2, y2](Vec2 p) -> bool { return p == Vec2(x2, y2); } ) == points.end()
      ) {
        points.push_back(Vec2(x2, y2));
      }
    }
  }

  for (auto point : points) {
    auto ray = m_entities.addEntity("Ray");
    //auto ray1 = m_entities.addEntity("Ray");
    // auto ray2 = m_entities.addEntity("Ray");

    float ca = point.x - playerX;
    float co = point.y - playerY;
    float angle = atan2(co, ca);
    float h = co / sin(angle);
    float angle1 = angle - 0.001;
    float angle2 = angle + 0.001;

    ray->addComponent<CLine>(Vec2(playerX, playerY), Vec2(point.x, point.y), angle);
    ray->getComponent<CLine>().line[0].color = sf::Color::Red;
    ray->getComponent<CLine>().line[1].color = sf::Color::Red;

    // ray1->addComponent<CLine>(
    //   Vec2(playerX, playerY), 
    //   Vec2(playerX + (cos(angle1)* h), playerY + (sin(angle1) * h))
    // );

    // ray1->getComponent<CLine>().line[0].color = sf::Color::Red;
    // ray1->getComponent<CLine>().line[1].color = sf::Color::Red;

    // ray2->addComponent<CLine>(
    //   Vec2(playerX, playerY), 
    //   Vec2(playerX + (cos(angle2) * h), playerY + (sin(angle2) * h))
    // );

    // ray2->getComponent<CLine>().line[0].color = sf::Color::Red;
    // ray2->getComponent<CLine>().line[1].color = sf::Color::Red;
  }

  auto player = m_entities.addEntity("Player");
  
  player->addComponent<CPosition>(playerX, playerY);
  player->addComponent<CShape>(12, 12);
  player->getComponent<CShape>().rect.setOrigin({ 6, 6 });
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
        m_player->getComponent<CPosition>().p = Vec2(mouseMoved->position.x, mouseMoved->position.y);
      
      } else if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
    }

    update();
  }
}

void Game::sMovement () {
  m_player->getComponent<CShape>().rect.setPosition({
    m_player->getComponent<CPosition>().p.x,
    m_player->getComponent<CPosition>().p.y,
  });

  for (auto line : m_entities.getEntities("Ray")) {
    line->getComponent<CLine>().p1 = Vec2(
      m_player->getComponent<CPosition>().p.x,
      m_player->getComponent<CPosition>().p.y
    );

    line->getComponent<CLine>().line[0].position.x = m_player->getComponent<CPosition>().p.x;
    line->getComponent<CLine>().line[0].position.y = m_player->getComponent<CPosition>().p.y;
  }
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
    if (entity->hasComponent<CShape>()) {
      m_window.draw(entity->getComponent<CShape>().rect);
    }

    if (entity->hasComponent<CLine>()) {
      m_window.draw(entity->getComponent<CLine>().line);
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