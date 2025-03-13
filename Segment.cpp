#include "Segment.h"

Segment::Segment (float x1, float y1, float x2, float y2): p1(x1, y1, this), p2(x2, y2, this) {}