#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics/RenderWindow.hpp>
#include "EntityManager.h"
#include "Segment.h"
#include "Vertex.h"

struct Event {
  float    angle;
  int      order;
  Segment& segment;
};

class Game {
  sf::RenderWindow        m_window;
  Vec2                    m_lightSource;
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