#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Vec2.h"

class Component {
  public:
    bool has = false;
};

class CPosition: public Component {
  public:
    Vec2 p;
    CPosition () {}
    CPosition (float x, float y): p(x,y) {}
};

class CLine: public Component {
  public:
    Vec2            p1;
    Vec2            p2;
    float           angle = 0;
    sf::VertexArray line;
  
    CLine () {}
    CLine (
      Vec2 point1, 
      Vec2 point2,
      float a = 0
    ): p1(point1), p2(point2), angle(a), line(sf::PrimitiveType::Lines, 2) {
      line[0].position = sf::Vector2f(p1.x, p1.y);
      line[1].position = sf::Vector2f(p2.x, p2.y);
    }
};

class CShape: public Component {
  public:
    sf::RectangleShape rect;

    CShape () {}
    CShape (float w, float h): rect({w, h}) {};
};

#endif