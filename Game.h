#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics/RenderWindow.hpp>
#include "EntityManager.h"
#include "Segment.h"

class Game {
  sf::RenderWindow        m_window;
  std::shared_ptr<Entity> m_player;
  EntityManager           m_entities;
  std::vector<Segment>    m_segments;
  std::vector<Vertex>     m_points;

  void init (const std::string& path);
  void update ();

  void sMovement ();
  void sShape();
  void sCollision ();
  void sRender ();

  public:
    Game (const std::string& path);

    void run ();
};

#endif