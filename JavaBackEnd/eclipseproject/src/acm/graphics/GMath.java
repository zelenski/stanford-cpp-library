/*
 * @version 2016/05/19
 * - added round(value, digits)
 */

/*
 * @(#)GMath.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// -- V2.0 --
// Bug fix 15-Aug-07 (ESR, JTFBug 2007-011, reported by William Slough)
//   1. Fixed comments that referred to incorrect trigonometric functions.

package acm.graphics;

/**
 * This class defines a variety of static mathematical methods that are useful
 * for the <code>acm.graphics</code> package.
 */
public class GMath {
	/**
	 * Prevents clients from instantiating this class.
	 */
	private GMath() {
		/* Empty */
	}
	
	/**
	 * Returns true if the given string could be interpreted as a boolean, such as "true" or "false".
	 * @param s a string
	 */
	public static boolean isBoolean(String s) {
		return s != null && (s.equalsIgnoreCase("true") || s.equalsIgnoreCase("false"));
	}

	/**
	 * Returns true if the given string could be interpreted as a char, such as "Q" or "\n".
	 * @param s a string
	 */
	public static boolean isChar(String s) {
		return s != null && s.length() == 1;
	}

	/**
	 * Returns true if the given string could be interpreted as a double, such as "123.45".
	 * @param s a string
	 */
	public static boolean isDouble(String s) {
		try {
			Double.parseDouble(s);
			return true;
		} catch (NumberFormatException nfe) {
			return false;
		}
	}
	
	/**
	 * Returns true if the given string could be interpreted as an integer, such as "12345".
	 * @param s a string
	 */
	public static boolean isInteger(String s) {
		try {
			Integer.parseInt(s);
			return true;
		} catch (NumberFormatException nfe) {
			return false;
		}
	}
	
	/**
	 * Returns true if the given string could be interpreted as an long, such as "12345".
	 * @param s a string
	 */
	public static boolean isLong(String s) {
		try {
			Long.parseLong(s);
			return true;
		} catch (NumberFormatException nfe) {
			return false;
		}
	}
	
	/**
	 * Rounds a <code>double</code> value to the nearest <code>int</code>.
	 *
	 * @usage int n = round(x);
	 * @param x
	 *            A <code>double</code> value
	 * @return The nearest <code>int</code> value
	 */
	public static int round(double x) {
		return (int) Math.round(x);
	}

	/**
	 * Rounds a <code>double</code> value to the given number of decimal places.
	 * For example, round(3.141592, 2) returns 3.14.
	 *
	 * @usage double n = round(x, digits);
	 * @param x
	 *            A <code>double</code> value
	 * @param digitsAfterDecimal
	 *            number of digits after decimal to keep
	 * @return The nearest <code>int</code> value
	 */
	public static double round(double x, int digitsAfterDecimal) {
		int mult = 1;
		for (int i = 0; i < digitsAfterDecimal; i++) {
			mult *= 10;
		}
		return Math.round(x * mult) / mult;
	}

	/**
	 * Returns the trigonometric sine of its argument where <code>angle</code>
	 * is expressed in degrees.
	 *
	 * @usage double s = sinDegrees(angle);
	 * @param angle
	 *            An angle measured in degrees
	 * @return The trigonometric sine of the angle
	 */
	public static double sinDegrees(double angle) {
		return Math.sin(toRadians(angle));
	}

	/**
	 * Returns the trigonometric cosine of its argument where <code>angle</code>
	 * is expressed in degrees.
	 *
	 * @usage double c = cosDegrees(angle);
	 * @param angle
	 *            An angle measured in degrees
	 * @return The trigonometric cosine of the angle
	 */
	public static double cosDegrees(double angle) {
		return Math.cos(toRadians(angle));
	}

	/**
	 * Returns the trigonometric tangent of its argument where
	 * <code>angle</code> is expressed in degrees.
	 *
	 * @usage double t = tanDegrees(angle);
	 * @param angle
	 *            An angle measured in degrees
	 * @return The trigonometric tangent of the angle
	 */
	public static double tanDegrees(double angle) {
		return sinDegrees(angle) / cosDegrees(angle);
	}

	/**
	 * Converts an angle from radians to degrees. This method is defined in the
	 * <code>Math</code> class, but was added only in JDK1.2, which is not
	 * supported in all browsers.
	 *
	 * @usage double degrees = toDegrees(radians);
	 * @param radians
	 *            An angle measured in radians
	 * @return The equivalent angle in degrees
	 */
	public static double toDegrees(double radians) {
		return radians * 180 / Math.PI;
	}

	/**
	 * Converts an angle from degrees to radians. This method is defined in the
	 * <code>Math</code> class, but was added only in JDK1.2, which is not
	 * supported in all browsers.
	 *
	 * @usage double radians = toRadians(degrees);
	 * @param degrees
	 *            An angle measured in degrees
	 * @return The equivalent angle in radians
	 */
	public static double toRadians(double degrees) {
		return degrees * Math.PI / 180;
	}

	/**
	 * Computes the distance between the origin and the point (<code>x</code>
	 * ,&nbsp;<code>y</code>).
	 *
	 * @usage double d = distance(x, y);
	 * @param x
	 *            The x-coordinate of the point
	 * @param y
	 *            The y-coordinate of the point
	 * @return The distance from the origin to the point (<code>x</code>,&nbsp;
	 *         <code>y</code>)
	 */
	public static double distance(double x, double y) {
		return Math.sqrt(x * x + y * y);
	}

	/**
	 * Computes the distance between the points (<code>x0</code>,&nbsp;
	 * <code>y0</code>) and (<code>x1</code>,&nbsp;<code>y1</code>).
	 *
	 * @usage double d = distance(x0, y0, x1, y1);
	 * @param x0
	 *            The x-coordinate of one point
	 * @param y0
	 *            The y-coordinate of that point
	 * @param x1
	 *            The x-coordinate of the other point
	 * @param y1
	 *            The y-coordinate of that point
	 * @return The distance between the points (<code>x0</code>,&nbsp;
	 *         <code>y0</code>) and (<code>x1</code>,&nbsp;<code>y1</code>)
	 */
	public static double distance(double x0, double y0, double x1, double y1) {
		return distance(x1 - x0, y1 - y0);
	}

	/**
	 * Returns the angle in degrees from the origin to the point (<code>x</code>
	 * ,&nbsp;<code>y</code>). This method is easier to use than
	 * <code>atan2</code> because it specifies the displacements in the usual
	 * x/y order and because it takes care of the fact that the Java coordinate
	 * system is flipped. The point (0, 0) is arbitrarily defined to be at angle
	 * 0.
	 *
	 * @usage double theta = angle(x, y);
	 * @param x
	 *            The x-coordinate of the point
	 * @param y
	 *            The y-coordinate of the point
	 * @return The angle from the origin to the point (<code>x</code>,&nbsp;
	 *         <code>y</code>) measured in degrees counterclockwise from the +x
	 *         axis
	 */
	public static double angle(double x, double y) {
		if (x == 0 && y == 0)
			return 0;
		return toDegrees(Math.atan2(-y, x));
	}

	/**
	 * Computes the angle in degrees formed by a line segment from the point (
	 * <code>x0</code>,&nbsp;<code>y0</code>) and (<code>x1</code>,&nbsp;
	 * <code>y1</code>).
	 *
	 * @usage double theta = angle(x0, y0, x1, y1);
	 * @param x0
	 *            The x-coordinate of one point
	 * @param y0
	 *            The y-coordinate of that point
	 * @param x1
	 *            The x-coordinate of the other point
	 * @param y1
	 *            The y-coordinate of that point
	 * @return The angle formed by the line segment from (<code>x0</code>,&nbsp;
	 *         <code>y0</code>) to (<code>x1</code>,&nbsp;<code>y1</code>)
	 */
	public static double angle(double x0, double y0, double x1, double y1) {
		return angle(x1 - x0, y1 - y0);
	}
}
