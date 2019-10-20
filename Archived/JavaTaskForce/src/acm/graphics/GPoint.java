/*
 * @(#)GPoint.java   1.99.1 08/12/08
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
// Performance improvement 23-Jan-07 (ESR)
//   1. Changed hashCode implementation for better performance.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import java.awt.*;
import java.io.*;

/* Class: GPoint */
/**
 * This class is a double-precision version of the <code>Point</code> class
 * in <code>java.awt</code>.
 */
public class GPoint implements Serializable {

/* Constructor: GPoint() */
/**
 * Constructs a new <code>GPoint</code> at the origin (0, 0).
 *
 * @usage pt = new GPoint();
 */
	public GPoint() {
		this(0, 0);
	}

/* Constructor: GPoint(x, y) */
/**
 * Constructs a new <code>GPoint</code> with the specified coordinates.
 *
 * @usage pt = new GPoint(x, y);
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 */
	public GPoint(double x, double y) {
		xc = x;
		yc = y;
	}

/* Constructor: GPoint(p) */
/**
 * Constructs a new <code>GPoint</code> from an existing one.
 *
 * @usage pt = new GPoint(p);
 * @param p The original point
 */
	public GPoint(GPoint p) {
		this(p.xc, p.yc);
	}

/* Constructor: GPoint(p) */
/**
 * Constructs a new <code>GPoint</code> from an existing AWT <code>Point</code>.
 *
 * @usage pt = new GPoint(p);
 * @param p An AWT <code>Point</code>
 */
	public GPoint(Point p) {
		this(p.x, p.y);
	}

/* Method: getX() */
/**
 * Returns the x coordinate of this <code>GPoint</code>.
 *
 * @usage x = pt.getX();
 * @return The x coordinate of this <code>GPoint</code>
 */
	public double getX() {
		return xc;
	}

/* Method: getY() */
/**
 * Returns the y coordinate of this <code>GPoint</code>.
 *
 * @usage y = pt.getY();
 * @return The y coordinate of this <code>GPoint</code>
 */
	public double getY() {
		return yc;
	}

/* Method: setLocation(x, y) */
/**
 * Sets the location of the <code>GPoint</code> to the specified <code>x</code>
 * and <code>y</code> values.
 *
 * @usage pt.setLocation(x, y);
 * @param x The new x-coordinate for the point
 * @param y The new y-coordinate for the point
 */
	public void setLocation(double x, double y) {
		xc = x;
		yc = y;
	}

/* Method: setLocation(p) */
/**
 * Sets the location of the <code>GPoint</code> to that of an existing one.
 *
 * @usage pt.setLocation(p);
 * @param p An existing <code>GPoint</code> specifying the new location
 */
	public void setLocation(GPoint p) {
		setLocation(p.xc, p.yc);
	}

/* Method: getLocation() */
/**
 * Returns a new <code>GPoint</code> whose coordinates are the same as this one.
 *
 * @usage p = pt.getLocation();
 * @return A new point with the same coordinates
 */
	public GPoint getLocation() {
		return new GPoint(xc, yc);
	}

/* Method: translate(dx, dy) */
/**
 * Adjusts the coordinates of a point by the specified <code>dx</code> and
 * <code>dy</code> offsets.
 *
 * @usage pt.translate(dx, dy);
 * @param dx The change in the x direction (positive is rightward)
 * @param dy The change in the y direction (positive is downward)
 */
	public void translate(double dx, double dy) {
		xc += dx;
		yc += dy;
	}

/* Method: toPoint() */
/**
 * Converts this <code>GPoint</code> to the nearest integer-based <code>Point</code>.
 *
 * @usage size = dim.toPoint();
 * @return The closest integer-based <code>Point</code>
 */
	public Point toPoint() {
		return new Point((int) Math.round(xc), (int) Math.round(yc));
	}

/* Method: hashCode() */
/**
 * Returns an integer hash code for the point.  The hash code for a
 * <code>GPoint</code> is constructed from the hash codes from the
 * <code>float</code> values of the coordinates, which are the ones used in the
 * <code>equals</code> method.
 *
 * @usage hash = pt.hashCode();
 * @return The hash code for this pt
 * @noshow
 */
	public int hashCode() {
		return new Float((float) xc).hashCode() ^ (37 * new Float((float) yc).hashCode());
	}

/* Method: equals(obj) */
/**
 * Tests whether two <code>GPoint</code> objects are equal.
 * Because floating-point values are inexact, this method checks for
 * equality by comparing the <code>float</code> values (rather than the
 * <code>double</code> values) of the coordinates.
 *
 * @usage if (pt.equals(obj)) . . .
 * @param obj Any object
 * @return <code>true</code> if the <code>obj</code> is a <code>GPoint</code>
 *         equal to this one, and <code>false</code> otherwise
 * @noshow
 */
	public boolean equals(Object obj) {
		if (!(obj instanceof GPoint)) return false;
		GPoint pt = (GPoint) obj;
		return ((float) xc == (float) pt.xc) && ((float) yc == (float) pt.yc);
	}

/* Method: toString() */
/**
 * Converts this <code>GPoint</code> to its string representation.
 *
 * @usage str = rect.toString();
 * @return A string representation of this point
 * @noshow
 */
	public String toString() {
		return "(" + (float) xc + ", " + (float) yc + ")";
	}

/* Private instance variables */
	private double xc;
	private double yc;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
