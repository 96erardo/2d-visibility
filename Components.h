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

class CTransform {
  public:
    Vec2  pos;
    float angle;

    CTransform () {};
    CTransform(float x, float y, float a = 0): pos(x,y), angle(a) {}
};

class CRect {
  public:
    sf::RectangleShape rect;

    CRect() {};
    CRect (float x, float y, float w, float h): rect({w,h}) {
      rect.setPosition({x,y});
    };
};

#endif