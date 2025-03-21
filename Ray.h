#ifndef RAY_H
#define RAY_H
#include "Vec2.h"

class Segment;

class Ray {
  public:
    Vec2   to;
    Vec2   intersection;
    float  angle = 0;
    
    Ray (float x, float y);

    static Ray fromAngleOffset (float x, float y, const Ray& v, float offset);

    void calcAngle (float x, float y);
};

#endif