/*
 * File: gmath.h
 * -------------
 * This file exports several functions for working with graphical
 * geometry along with the mathematical constants <code>PI</code>
 * and <code>E</code>.
 */

#ifndef _gmath_h
#define _gmath_h

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

#endif
