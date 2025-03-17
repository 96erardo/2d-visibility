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

class CTransform: public Component {
  public:
    Vec2  pos;
    float angle;

    CTransform () {};
    CTransform(float x, float y, float a = 0): pos(x,y), angle(a) {}
};

class CRect: public Component {
  public:
    sf::RectangleShape rect;

    CRect() {};
    CRect (float x, float y, float w, float h): rect({w,h}) {
      rect.setPosition({x,y});
      rect.setOrigin({w/2, h/2});
    };
};

class CLine: public Component {
  public:
    sf::VertexArray line;

    CLine (): line(sf::PrimitiveType::Lines, 2) {};
    CLine (float x1, float y1, float x2, float y2): line(sf::PrimitiveType::Lines, 2) {
      line[0].position = sf::Vector2f(x1, y1);
      line[1].position = sf::Vector2f(x2, y2);
    };
};

class CTriangle: public Component {
  public:
    sf::VertexArray triangle;

    CTriangle (): triangle(sf::PrimitiveType::Triangles, 3) {
      triangle[0].color = sf::Color::White;
      triangle[1].color = sf::Color::White;
      triangle[2].color = sf::Color::White;
    };
};

#endif