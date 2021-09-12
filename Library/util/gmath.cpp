/*
 * File: gmath.cpp
 * ---------------
 * This file implements the gmath.h interface.  In all cases, the
 * implementation for each function requires only one line of code,
 * which makes detailed documentation unnecessary.
 *
 * @version 2018/11/22
 * - added headless mode support
 * - alphabetized methods
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 */

#include "gmath.h"
#include <cmath>
#include "error.h"
#include "gtypes.h"

extern const double PI = 3.14159265358979323846;
extern const double E  = 2.71828182845904523536;

// See also:
// http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
template<typename T>
bool withinEpsilon(T f1, T f2) {
    T larger = std::fmax(std::fabs(f1), std::fabs(f2));
    T tolerance = std::numeric_limits<T>::epsilon() * larger;
    return std::fabs(f1 - f2) <= tolerance;
}

bool floatingPointEqual(double f1, double f2) {
    return withinEpsilon(f1, f2);
}

bool floatingPointEqual(float f1, float f2) {
    return withinEpsilon(f1, f2);
}

double cosDegrees(double angle) {
    return cos(toRadians(angle));
}

double sinDegrees(double angle) {
    return sin(toRadians(angle));
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

double vectorAngle(double x, double y) {
    return floatingPointEqual(x, 0) && floatingPointEqual(y, 0)
            ? 0 : toDegrees(atan2(-y, x));
}

double vectorAngle(const GPoint& pt) {
    return vectorAngle(pt.x, pt.y);
}

double vectorDistance(double x, double y) {
    return sqrt(x * x + y * y);
}

double vectorDistance(const GPoint& pt) {
    return vectorDistance(pt.x, pt.y);
}
