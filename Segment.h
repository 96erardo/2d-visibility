#ifndef SEGMENT_H
#define SEGMENT_H
#include "Vertex.h"

class Segment {
  public:
    Vertex p1;
    Vertex p2;

    Segment (float x1, float y1, float x2, float y2);
};

#endif