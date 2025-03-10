#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
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
    Vec2  p1;
    Vec2  p2;
  
    CLine () {}
    CLine (
      Vec2 point1, 
      Vec2 point2
    ): p1(point1), p2(point2) {}
};

class CShape: public Component {
  public:
    sf::RectangleShape rect;

    CShape () {}
    CShape (float w, float h): rect({w, h}) {};
};

#endif