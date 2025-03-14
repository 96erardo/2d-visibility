#include <cmath>
#include "Vec2.h"
#include "Vertex.h"
#include "Segment.h"

Vertex::Vertex (float x, float y, Segment* s): p(x,y), segment(s) {}

void Vextex::calcAngle(float x, float y) {
    float ca = p.x - x;
    float co = p.y - y;

    angle = atan2(co, ca);
}

void Vertex::calcDistance (float x, float y) {
    distance = std::sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}
