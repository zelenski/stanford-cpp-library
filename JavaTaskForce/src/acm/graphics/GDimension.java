/*
 * @(#)GDimension.java   1.99.1 08/12/08
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

/* Class: GDimension */
/**
 * This class is a double-precision version of the <code>Dimension</code> class
 * in <code>java.awt</code>.
 */
public class GDimension implements Serializable {

/* Constructor: GDimension() */
/**
 * Constructs a new dimension object with zero values for width and height.
 *
 * @usage dim = new GDimension();
 */
	public GDimension() {
		this(0, 0);
	}

/* Constructor: GDimension(width, height) */
/**
 * Constructs a new dimension object with the specified components.
 *
 * @usage dim = new GDimension(width, height);
 * @param width The width of the dimension object
 * @param height The height of the dimension object
 */
	public GDimension(double width, double height) {
		myWidth = width;
		myHeight = height;
	}

/* Constructor: GDimension(size) */
/**
 * Constructs a new <code>GDimension</code> object from an existing one.
 *
 * @usage dim = new GDimension(size);
 * @param size An existing <code>GDimension</code> object specifying the size
 */
	public GDimension(GDimension size) {
		this(size.myWidth, size.myHeight);
	}

/* Constructor: GDimension(size) */
/**
 * Constructs a new <code>GDimension</code> object from an AWT <code>Dimension</code>.
 *
 * @usage dim = new GDimension(size);
 * @param size An AWT <code>Dimension</code> object specifying the size
 */
	public GDimension(Dimension size) {
		this(size.width, size.height);
	}

/* Method: getWidth() */
/**
 * Returns the width of this <code>GDimension</code>.
 *
 * @usage width = dim.getWidth();
 * @return The width of this <code>GDimension</code>
 */
	public double getWidth() {
		return myWidth;
	}

/* Method: getHeight() */
/**
 * Returns the height of this <code>GDimension</code>.
 *
 * @usage height = dim.getHeight();
 * @return The height of this <code>GDimension</code>
 */
	public double getHeight() {
		return myHeight;
	}

/* Method: setSize(width, height) */
/**
 * Sets the components of the dimension object from the specified parameters.
 *
 * @usage dim.setSize(width, height);
 * @param width The new width of the dimension object
 * @param height The new height of the dimension object
 */
	public void setSize(double width, double height) {
		myWidth = width;
		myHeight = height;
	}

/* Method: setSize(size) */
/**
 * Sets the width and height of one <code>GDimension</code> object equal to that of another.
 *
 * @usage dim.setSize(size);
 * @param size A <code>GDimension</code> object specifying the new size
 */
	public void setSize(GDimension size) {
		setSize(size.myWidth, size.myHeight);
	}

/* Method: getSize() */
/**
 * Returns a new <code>GDimension</code> object equal to this one.
 *
 * @usage size = dim.getSize();
 * @return A new <code>GDimension</code> object with the same size
 */
	public GDimension getSize() {
		return new GDimension(myWidth, myHeight);
	}

/* Method: toDimension() */
/**
 * Converts this <code>GDimension</code> to the nearest integer-based
 * <code>Dimension</code>.
 *
 * @usage size = dim.toDimension();
 * @return The closest integer-based <code>Dimension</code> object
 */
	public Dimension toDimension() {
		return new Dimension((int) Math.round(myWidth), (int) Math.round(myHeight));
	}

/* Method: hashCode() */
/**
 * Returns an integer hash code for the dimension object.  The hash code for a
 * <code>GDimension</code> is constructed from the hash codes from the
 * <code>float</code> values of the width and height, which are the ones used in the
 * <code>equals</code> method.
 *
 * @usage hash = dim.hashCode();
 * @return The hash code for this dimension object
 * @noshow
 */
	public int hashCode() {
		return new Float((float) myWidth).hashCode() ^ (37 * new Float((float) myHeight).hashCode());
	}

/* Method: equals(obj) */
/**
 * Tests whether two <code>GDimension</code> objects are equal.
 * Because floating-point values are inexact, this method checks for
 * equality by comparing the <code>float</code> values (rather than the
 * <code>double</code> values) of the coordinates.
 *
 * @usage if (dim.equals(obj)) . . .
 * @param obj Any object
 * @return <code>true</code> if the <code>obj</code> is a <code>GDimension</code>
 *         equal to this one, and <code>false</code> otherwise
 * @noshow
 */
	public boolean equals(Object obj) {
		if (!(obj instanceof GDimension)) return false;
		GDimension dim = (GDimension) obj;
		return ((float) myWidth == (float) dim.myWidth) && ((float) myHeight == (float) dim.myHeight);
	}

/* Method: toString() */
/**
 * Converts this <code>GDimension</code> to its string representation.
 *
 * @usage str = dim.toString();
 * @return A string representation of this dimension object
 * @noshow
 */
	public String toString() {
		return "(" + (float) myWidth + "x" + (float) myHeight + ")";
	}

/* Private instance variables */
	private double myWidth;
	private double myHeight;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
