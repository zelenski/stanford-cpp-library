/*
 * File: gmath.h
 * -------------
 * This file exports several functions for working with graphical
 * geometry along with the mathematical constants <code>PI</code>
 * and <code>E</code>.
 *
 * @version 2016/10/14
 * - added floatingPointEqual method for comparing floats and doubles
 */

#ifndef _gmath_h
#define _gmath_h

#include <cmath>
#include <limits>
#include "gtypes.h"

/*
 * Constant: PI
 * ------------
 * The mathematical constant pi, which is the ratio of the circumference
 * of a circle to its diameter.
 */

extern const double PI;

/*
 * Constant: E
 * -----------
 * The mathematical constant e, which is the base of natural logarithms.
 */

extern const double E;

/*
 * Function: sinDegrees
 * Usage: double sine = sinDegrees(angle);
 * ---------------------------------------
 * Returns the trigonometric sine of <code>angle</code>, which is
 * expressed in degrees.
 */

double sinDegrees(double angle);

/*
 * Function: cosDegrees
 * Usage: double cosine = cosDegrees(angle);
 * -----------------------------------------
 * Returns the trigonometric cosine of <code>angle</code>, which is
 * expressed in degrees.
 */

double cosDegrees(double angle);

/*
 * Function: tanDegrees
 * Usage: double tangent = tanDegrees(angle);
 * ------------------------------------------
 * Returns the trigonometric tangent of <code>angle</code>, which is
 * expressed in degrees.
 */

double tanDegrees(double angle);

/*
 * Function: toDegrees
 * Usage: double degrees = toDegrees(radians);
 * -------------------------------------------
 * Converts an angle from radians to degrees.
 */

double toDegrees(double radians);

/*
 * Function: toRadians
 * Usage: double radians = toRadians(degrees);
 * -------------------------------------------
 * Converts an angle from degrees to radians.
 */

double toRadians(double degrees);

/*
 * Function: vectorDistance
 * Usage: double r = vectorDistance(pt);
 *        double r = vectorDistance(x, y);
 * ---------------------------------------
 * Computes the distance between the origin and the specified point.
 */

double vectorDistance(const GPoint & pt);
double vectorDistance(double x, double y);

/*
 * Function: vectorAngle
 * Usage: double angle = vectorAngle(pt);
 *        double angle = vectorAngle(x, y);
 * ----------------------------------------
 * Returns the angle in degrees from the origin to the specified point.
 * This function takes account of the fact that the graphics coordinate
 * system is flipped in the <i>y</i> direction from the traditional
 * Cartesian plane.
 */

double vectorAngle(const GPoint & pt);
double vectorAngle(double x, double y);

/*
 * Returns the number of digits in the given integer in the given base.
 * Defaults to base-10, decimal.
 * For example:
 *   countDigits(3456) returns 4.
 *   countDigits(0)    returns 1.
 *   countDigits(-778) returns 3.
 * Throws an error if base is 0 or negative.
 */
int countDigits(int n, int base = 10);

/*
 * Returns true if the two given floating-point numbers are "equal" to each other.
 * Floating-point equality is tricky because of round-off errors, which can cause
 * the numbers to be nearly the same but not identical.
 *
 * See also:
 * http://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
 */
template<typename T>
bool floatingPointEqual(T f1, T f2) {
  return (std::fabs(f1 - f2) <= std::numeric_limits<T>::epsilon() * std::fmax(fabs(f1), fabs(f2)));
}

template<typename T>
bool floatingPointEqual(T f1, int f2) {
  return floatingPointEqual(f1, (T) f2);
}

template<typename T>
bool floatingPointEqual(int f1, T f2) {
  return floatingPointEqual((T) f1, f2);
}

template<typename T>
bool floatingPointEqual(T f1, long int f2) {
  return floatingPointEqual(f1, (T) f2);
}

template<typename T>
bool floatingPointEqual(long int f1, T f2) {
  return floatingPointEqual((T) f1, f2);
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gmath_h
