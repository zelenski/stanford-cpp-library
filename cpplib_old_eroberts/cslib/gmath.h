/*
 * File: gmath.h
 * -------------
 * This interface exports several functions for working with graphical
 * geometry along with the constants <code>PI</code> and <code>E</code>.
 */

#ifndef _gmath_h
#define _gmath_h

/*
 * Constant: PI
 * ------------
 * The mathematical constant pi, which is the ratio of the circumference
 * of a circle to its diameter.
 */

#define PI 3.14159265358979323846

/*
 * Constant: E
 * -----------
 * The mathematical constant e, which is the base of natural logarithms.
 */

#define E 2.71828182845904523536

/*
 * Function: sinDegrees
 * Usage: sine = sinDegrees(angle);
 * --------------------------------
 * Returns the trigonometric sine of <code>angle</code>, which is
 * expressed in degrees.
 */

double sinDegrees(double angle);

/*
 * Function: cosDegrees
 * Usage: cosine = cosDegrees(angle);
 * ----------------------------------
 * Returns the trigonometric cosine of <code>angle</code>, which is
 * expressed in degrees.
 */

double cosDegrees(double angle);

/*
 * Function: tanDegrees
 * Usage: tangent = tanDegrees(angle);
 * -----------------------------------
 * Returns the trigonometric tangent of <code>angle</code>, which is
 * expressed in degrees.
 */

double tanDegrees(double angle);

/*
 * Function: toDegrees
 * Usage: degrees = toDegrees(radians);
 * ------------------------------------
 * Converts an angle from radians to degrees.
 */

double toDegrees(double radians);

/*
 * Function: toRadians
 * Usage: radians = toRadians(degrees);
 * ------------------------------------
 * Converts an angle from degrees to radians.
 */

double toRadians(double degrees);

/*
 * Function: vectorDistance
 * Usage: r = vectorDistance(x, y);
 * --------------------------------
 * Computes the distance between the origin and the specified point.
 */

double vectorDistance(double x, double y);

/*
 * Function: vectorAngle
 * Usage: angle = vectorAngle(x, y);
 * ---------------------------------
 * Returns the angle in degrees from the origin to the specified point.
 * This function takes account of the fact that the graphics coordinate
 * system is flipped in the <i>y</i> direction from the traditional
 * Cartesian plane.
 */

double vectorAngle(double x, double y);

#endif
