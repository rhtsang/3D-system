#ifndef TRANSFORM_H
#define  TRANSFORM_H

#include "polygon.h"

Coordinate findCentroid(Polygon polygon);
void translate(Polygon& polygon, float translate_x, float translate_y, float translate_z);
void scale(Polygon& polygon, float factor);
void rotate(Polygon& polygon, Coordinate axis_start, Coordinate axis_end, float angle);

#endif
