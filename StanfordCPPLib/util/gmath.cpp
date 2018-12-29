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

#define INTERNAL_INCLUDE 1
#include "gmath.h"
#include <cmath>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#undef INTERNAL_INCLUDE

extern const double PI = 3.14159265358979323846;
extern const double E  = 2.71828182845904523536;

double cosDegrees(double angle) {
    return cos(toRadians(angle));
}

int countDigits(int n, int base) {
    if (base <= 0) {
        error("countDigits: base must be 1 or greater");
    }
    if (n == 0) {
        return 0;
    } else if (n < 0) {
        n = -n;
    }

    int digits = 0;
    for (int temp = n; temp > 0 && digits < 65; temp /= base) {
        digits++;
    }
    return digits;
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

#ifndef SPL_HEADLESS_MODE
double vectorAngle(const GPoint& pt) {
    return vectorAngle(pt.getX(), pt.getY());
}
#endif // SPL_HEADLESS_MODE

double vectorAngle(const Point& pt) {
    return vectorAngle(pt.getX(), pt.getY());
}

double vectorDistance(double x, double y) {
    return sqrt(x * x + y * y);
}

#ifndef SPL_HEADLESS_MODE
double vectorDistance(const GPoint& pt) {
    return vectorDistance(pt.getX(), pt.getY());
}
#endif // SPL_HEADLESS_MODE

double vectorDistance(const Point& pt) {
    return vectorDistance(pt.getX(), pt.getY());
}
