/*
 * File: gmath.h
 * -------------
 * This file exports several functions for working with graphical
 * geometry along with the mathematical constants <code>PI</code>
 * and <code>E</code>.
 */


#ifndef _gmath_h
#define _gmath_h

#include <cmath>
#include <limits>
#include "gtypes.h"


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
 * Returns true if the two given floating-point numbers are "equal".
 * Floating-point equality is tricky because of roundoff errors, which can cause
 * the numbers to be nearly the same but not identical.
 * This comparison tests whether difference of the two numbers is within machine
 * epsilon.
 */
bool floatingPointEqual(double f1, double f2);
bool floatingPointEqual(float f1, float f2);

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
