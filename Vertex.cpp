#include <cmath>
#include "Vec2.h"
#include "Vertex.h"

Vertex::Vertex (float x, float y): p(x,y), intersection(x, y) {}

Vertex Vertex::fromAngleOffset(float x, float y, const Vertex& v, float offset) {
    float ca = v.p.x - x;
    float co = v.p.y - y;
    float angle = atan2(co, ca);
    float h = co / sin(angle);
    float angle1 = angle + offset;

    return Vertex(x + (cos(angle1) * h), y + (sin(angle1) * h));    
}

void Vertex::calcAngle(float x, float y) {
    float ca = p.x - x;
    float co = p.y - y;

    angle = atan2(co, ca);
}