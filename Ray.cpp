#include <cmath>
#include "Vec2.h"
#include "Ray.h"

Ray::Ray (float x, float y): to(x,y), intersection(x, y) {}

Ray Ray::fromAngleOffset(float x, float y, const Ray& r, float offset) {
    float ca = r.to.x - x;
    float co = r.to.y - y;
    float angle = atan2(co, ca);
    float h = co / sin(angle);
    float angle1 = angle + offset;

    return Ray(x + (cos(angle1) * h), y + (sin(angle1) * h));    
}

void Ray::calcAngle(float x, float y) {
    float ca = to.x - x;
    float co = to.y - y;

    angle = atan2(co, ca);
}