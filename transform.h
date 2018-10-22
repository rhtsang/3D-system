#ifndef TRANSFORM_H
#define  TRANSFORM_H

#include "polygon.h"

extern const double pi;

Coordinate findCentroid(Polygon polygon);
void translate(Polygon& polygon, double translate_x, double translate_y, double translate_z);
void scale(Polygon& polygon, double factor);
void rotate(Polygon& polygon, Coordinate axis_start, Coordinate axis_end, double angle);

#endif
