#ifndef VERTEX_H
#define VERTEX_H
#include "Vec2.h"
#include "Segment.h"

class Vertex {
  public:
    Vec2     p;
    float    angle = 0;
    float    distance = 0;
    Segment* segment;
    
    Vertex (float x, float y, Segment* segment);
};

#endif