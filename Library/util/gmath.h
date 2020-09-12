/*
 * File: gmath.h
 * -------------
 * This file exports several functions for working with graphical
 * geometry along with the mathematical constants <code>PI</code>
 * and <code>E</code>.
 *
 * @version 2018/11/22
 * - added headless mode support
 * - alphabetized methods
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/12/12
 * - added floatingPointEqual(a, b, tolerance)
 * @version 2016/10/14
 * - added floatingPointEqual method for comparing floats and doubles
 */


#ifndef _gmath_h
#define _gmath_h

#include <cmath>
#include <limits>

#ifndef SPL_HEADLESS_MODE
#include "gtypes.h"
#endif // SPL_HEADLESS_MODE


/**
 * The mathematical constant pi, which is the ratio of the circumference
 * of a circle to its diameter.
 */
extern const double PI;

/**
 * Constant: E
 * -----------
 * The mathematical constant e, which is the base of natural logarithms.
 */
extern const double E;

/**
 * Returns the trigonometric cosine of <code>angle</code>, which is
 * expressed in degrees.
 */
double cosDegrees(double angle);


/**
 * Returns the number of digits in the given integer in the given base.
 * Defaults to base-10, decimal.
 * @example countDigits(3456) returns 4.
 * @example countDigits(0)    returns 1.
 * @example countDigits(-778) returns 3.
 * @throw ErrorException if base is 0 or negative.
 */
int countDigits(int n, int base = 10);

/**
 * Returns true if the two given floating-point numbers are "equal" to each other,
 * within a given tolerance.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(T f1, T f2, T tolerance) {
    return (std::fabs(f1 - f2) <= tolerance);
}

/**
 * Returns true if the two given floating-point numbers are "equal" to each other.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(T f1, T f2) {
    return floatingPointEqual(f1, f2, /* tolerance */ (T) std::numeric_limits<T>::epsilon() * std::fmax(fabs(f1), fabs(f2)));
}

/**
 * Returns true if the given floating-point number is "equal" to the given integer.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(T f1, int f2) {
    return floatingPointEqual(f1, (T) f2);
}

/**
 * Returns true if the given floating-point number is "equal" to the given integer.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(int f1, T f2) {
    return floatingPointEqual((T) f1, f2);
}

/**
 * Returns true if the given floating-point number is "equal" to the given integer.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(T f1, long int f2) {
    return floatingPointEqual(f1, (T) f2);
}

/**
 * Returns true if the given floating-point number is "equal" to the given integer.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(long int f1, T f2) {
    return floatingPointEqual((T) f1, f2);
}

/**
 * Returns the trigonometric sine of <code>angle</code>, which is
 * expressed in degrees.
 */
double sinDegrees(double angle);

/**
 * Returns the trigonometric tangent of <code>angle</code>, which is
 * expressed in degrees.
 */
double tanDegrees(double angle);

/**
 * Converts an angle from radians to degrees.
 */
double toDegrees(double radians);

/**
 * Converts an angle from degrees to radians.
 */
double toRadians(double degrees);

/**
 * Returns the angle in degrees from the origin to the specified point.
 * This function takes account of the fact that the graphics coordinate
 * system is flipped in the <i>y</i> direction from the traditional
 * Cartesian plane.
 */
double vectorAngle(double x, double y);

/**
 * Returns the angle in degrees from the origin to the specified point.
 * This function takes account of the fact that the graphics coordinate
 * system is flipped in the <i>y</i> direction from the traditional
 * Cartesian plane.
 */
double vectorAngle(const GPoint& pt);

/**
 * Computes the distance between the origin and the specified point.
 */
double vectorDistance(double x, double y);

/**
 * Computes the distance between the origin and the specified point.
 */
double vectorDistance(const GPoint& pt);

#endif // _gmath_h
