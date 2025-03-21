#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Vec2.h"

class Component {
  public:
    bool has = false;
};

class CTransform: public Component {
  public:
    Vec2  pos;
    float angle;

    CTransform () {};
    CTransform(float x, float y, float a = 0): pos(x,y), angle(a) {}
};

class CShape: public Component {
  public:
    sf::ConvexShape shape;

    CShape() {};
    CShape (int count): shape(count) {}
};

class CCircle: public Component {
  public:
    sf::CircleShape circle;

    CCircle () {};
    CCircle (float r): circle(r) {
      circle.setOrigin({r,r});
    };
};

#endif