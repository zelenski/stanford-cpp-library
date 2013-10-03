/*
 * File: gmath.cpp
 * ---------------
 * This file implements the gmath.h interface.  In all cases, the
 * implementation for each function requires only one line of code,
 * which makes detailed documentation unnecessary.
 */

#include <cmath>
#include "gmath.h"
#include "gtypes.h"

extern const double PI = 3.14159265358979323846;
extern const double E  = 2.71828182845904523536;

double sinDegrees(double angle) {
   return sin(toRadians(angle));
}

double cosDegrees(double angle) {
   return cos(toRadians(angle));
}

double tanDegrees(double angle) {
   return tan(toRadians(angle));
}

double toDegrees(double radians) {
   return radians * 180 / PI;
}

double toRadians(double degrees) {
   return degrees * PI / 180;
}

double vectorDistance(const GPoint & pt) {
   return vectorDistance(pt.getX(), pt.getY());
}

double vectorDistance(double x, double y) {
   return sqrt(x * x + y * y);
}

double vectorAngle(const GPoint & pt) {
   return vectorAngle(pt.getX(), pt.getY());
}

double vectorAngle(double x, double y) {
  return (x == 0 && y == 0) ? 0 : toDegrees(atan2(-y, x));
}
