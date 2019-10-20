/*
 * File: gmath.cpp
 * ---------------
 * This file implements the gmath.h interface.  In all cases, the
 * implementation for each function requires only one line of code,
 * which makes detailed documentation unnecessary.
 */

#include <math.h>
#include "gmath.h"

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

double vectorDistance(double x, double y) {
   return sqrt(x * x + y * y);
}

double vectorAngle(double x, double y) {
  return (x == 0 && y == 0) ? 0 : toDegrees(atan2(-y, x));
}
