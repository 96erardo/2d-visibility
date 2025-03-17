#ifndef VERTEX_H
#define VERTEX_H
#include "Vec2.h"

class Segment;

class Vertex {
  public:
    Vec2   p;
    Vec2   intersection;
    float  angle = 0;
    
    Vertex (float x, float y);

    static Vertex fromAngleOffset (float x, float y, const Vertex& v, float offset);

    void calcAngle (float x, float y);
};

#endif