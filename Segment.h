#ifndef SEGMENT_H
#define SEGMENT_H
#include "Vec2.h"

class Segment {
  public:
    Vec2 p1;
    Vec2 p2;
    float  distance;

    Segment (float x1, float y1, float x2, float y2);

    bool operator == (const Segment& rhs) const;
};

#endif