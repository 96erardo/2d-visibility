#include "Vec2.h"
#include "Vertex.h"
#include "Segment.h"

Vertex::Vertex (float x, float y, Segment* s): p(x,y), segment(s) {}
