#include <math.h>
#include <algorithm>
#include "Segment.h"

Segment::Segment (float x1, float y1, float x2, float y2): p1(x1, y1), p2(x2, y2) {}

bool Segment::operator== (const Segment& s) const {
  return s.p1 == p1 && s.p2 == p2;
}