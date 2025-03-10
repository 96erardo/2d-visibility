#include <SFML/System/Angle.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
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
    }
  }

  auto player = m_entities.addEntity("Player");
  float x = 400.0f;
  float y = 300.0f;
  
  player->addComponent<CPosition>(x, y);
  player->addComponent<CShape>(12, 12);
  player->getComponent<CShape>().rect.setOrigin({ 6, 6 });
  player->getComponent<CShape>().rect.setPosition({ x, y });
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
}

void Game::sRender () {
  m_window.clear(sf::Color(51, 54, 68)); 

  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CShape>()) {
      m_window.draw(entity->getComponent<CShape>().rect);
    }
  }

  m_window.display();
}

void Game::update () {
  m_entities.update();

  sMovement();
  sRender();
}