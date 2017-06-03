/*
 * @version 2016/06/01
 * - added new constructors
 * - added setEndPointPolar
 * - alphabetized methods
 * @version 2016/05/22
 * - bug fix for incorrect positioning in paint2d
 */

/*
 * @(#)GLine.java   1.1 1 08/08/01
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
// Code cleanup 30-Sep-06 (ESR)
//   1. Removed vestigial inherited methods setFilled and isFilled.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import java.awt.*;

/* Class: GLine */
/**
 * The <code>GLine</code> class is a graphical object whose appearance consists
 * of a line segment.
 */
public class GLine extends GObject implements GScalable {
	/* Serial version UID */
	/**
	 * The serialization code for this class. This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */
	static final long serialVersionUID = 1L;

	/* Field: LINE_TOLERANCE */
	/**
	 * This constant defines how close (measured in pixel units) a point has to
	 * be to a line before that point is considered to be "contained" within the
	 * line.
	 */
	public static final double LINE_TOLERANCE = 1.5;

	/* Private instance variables */
	private double dx;
	private double dy;

	/* Constructor: GLine() */
	/**
	 * Constructs a line segment of length 0 at position (0, 0).
	 *
	 * @usage GLine gline = new GLine();
	 */
	public GLine() {
		this(0, 0, 0, 0);
	}

	/* Constructor: GLine(x0, y0) */
	/**
	 * Constructs a line segment of length 0 at position (x0, y0).
	 *
	 * @usage GLine gline = new GLine(x0, y0);
	 */
	public GLine(double x0, double y0) {
		this(x0, y0, x0, y0);
	}

	/* Constructor: GLine(x0, y0, x1, y1) */
	/**
	 * Constructs a line segment from its endpoints. The point (<code>x0</code>
	 * ,&nbsp;<code>y0</code>) defines the start of the line and the point (
	 * <code>x1</code>,&nbsp;<code>y1</code>) defines the end.
	 *
	 * @usage GLine gline = new GLine(x0, y0, x1, y1);
	 * @param x0
	 *            The x-coordinate of the start of the line
	 * @param y0
	 *            The y-coordinate of the start of the line
	 * @param x1
	 *            The x-coordinate of the end of the line
	 * @param y1
	 *            The y-coordinate of the end of the line
	 */
	public GLine(double x0, double y0, double x1, double y1) {
		setLocation(x0, y0);
		dx = x1 - x0;
		dy = y1 - y0;
	}

	/* Method: contains(x, y) */
	/**
	 * Checks to see whether a point is inside the object. For the
	 * <code>GLine</code> class, containment is defined to mean that the point
	 * is within <a href="#LINE_TOLERANCE"><code>LINE_TOLERANCE</code></a>
	 * pixels of the line.
	 *
	 * @usage if (gline.contains(x, y)) . . .
	 * @param x
	 *            The x-coordinate of the point being tested
	 * @param y
	 *            The y-coordinate of the point being tested
	 * @return <code>true</code> if the point (<code>x</code>,&nbsp;
	 *         <code>y</code>) is inside
	 */
	public boolean contains(double x, double y) {
		double x0 = getX();
		double y0 = getY();
		double x1 = x0 + dx;
		double y1 = y0 + dy;
		return intersects(x, y, x0, y0, x1, y1, getLineWidth());
	}

	/* Private method: distanceSquared(x0, y0, x1, y1) */
	/**
	 * Returns the square of the distance between (<code>x0</code>,&nbsp;
	 * <code>y0</code>) and (<code>x1</code>,&nbsp;<code>y1</code>).
	 */
	public static double distanceSquared(double x0, double y0, double x1, double y1) {
		return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
	}

	/* Method: getBounds() */
	/**
	 * Returns the bounding box for this object.
	 *
	 * @usage GRectangle bounds = gline.getBounds();
	 * @return The bounding box for this object
	 */
	public GRectangle getBounds() {
		double x = Math.min(getX(), getX() + dx);
		double y = Math.min(getY(), getY() + dy);
		return new GRectangle(x, y, Math.abs(dx), Math.abs(dy));
	}

	/* Method: getEndPoint() */
	/**
	 * Returns the end point of the line as a <code>GPoint</code> object.
	 *
	 * @usage GPoint pt = gline.getEndPoint();
	 * @return The coordinates of the end point of the line
	 */
	public GPoint getEndPoint() {
		return new GPoint(getX() + dx, getY() + dy);
	}

	/* Method: getStartPoint() */
	/**
	 * Returns the coordinates of the initial point in the line. This method is
	 * identical to <a href="#getLocation()"><code>getLocation</code></a> and
	 * exists only to provide symmetry with <a href=
	 * "#setStartPoint(double, double)"><code>setStartPoint</code></a>.
	 *
	 * @usage GPoint pt = gline.getStartPoint();
	 * @return The coordinates of the origin of the line
	 */
	public GPoint getStartPoint() {
		return getLocation();
	}
	
	/**
	 * Returns whether the given x/y point intersects the line from x0,y0 to x1,y1 of the given width.
	 */
	public static boolean intersects(double x, double y, double x0, double y0, double x1, double y1, double lineWidth) {
		double tSquared = LINE_TOLERANCE * LINE_TOLERANCE + lineWidth - 1;
		if (GLine.distanceSquared(x, y, x0, y0) < tSquared)
			return true;
		if (GLine.distanceSquared(x, y, x1, y1) < tSquared)
			return true;
		if (x < Math.min(x0, x1) - LINE_TOLERANCE + lineWidth - 1)
			return false;
		if (x > Math.max(x0, x1) + LINE_TOLERANCE + lineWidth - 1)
			return false;
		if (y < Math.min(y0, y1) - LINE_TOLERANCE + lineWidth - 1)
			return false;
		if (y > Math.max(y0, y1) + LINE_TOLERANCE + lineWidth - 1)
			return false;
		if ((float) x0 - (float) x1 == 0 && (float) y0 - (float) y1 == 0)
			return false;
		double u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0)) / distanceSquared(x0, y0, x1, y1);
		return distanceSquared(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tSquared;
	}

	/* Method: paint2d(g) */
	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.
	 * This method is not called directly by clients.
	 * 
	 * @noshow
	 */
	public void paint2d(Graphics2D g) {
		// Line2D.Double line = new Line2D.Double(0.0D, 0.0D, GMath.round(dx), GMath.round(dy));
		// g.draw(line);
		g.drawLine(0, 0, (int) dx, (int) dy);
	}

	/* Protected method: paramString() */
	/**
	 * Returns a string indicating the parameters of this object.
	 * 
	 * @noshow
	 */
	public String paramString() {
		String tail = super.paramString();
		tail = tail.substring(tail.indexOf(')') + 1);
		GPoint pt = getStartPoint();
		String param = "start=(" + pt.getX() + ", " + pt.getY() + ")";
		pt = getEndPoint();
		param += ", end=(" + pt.getX() + ", " + pt.getY() + ")";
		param += ", lineWidth=" + getLineWidth();
		return param + tail;
	}

	/* Method: scale(sx, sy) */
	/**
	 * Scales the line on the screen by the scale factors <code>sx</code> and
	 * <code>sy</code>. This method changes only the end point of the line,
	 * leaving the start of the line fixed.
	 *
	 * @usage gline.scale(sx, sy);
	 * @param sx
	 *            The factor used to scale all coordinates in the x direction
	 * @param sy
	 *            The factor used to scale all coordinates in the y direction
	 */
	public void scale(double sx, double sy) {
		dx *= sx;
		dy *= sy;
		repaint();
	}

	/* Method: setEndPoint(x, y) */
	/**
	 * Sets the end point of the line to the point (<code>x</code>,&nbsp;
	 * <code>y</code>). The origin of the line remains unchanged.
	 *
	 * @usage gline.setEndPoint(x, y);
	 * @param x
	 *            The new x-coordinate of the end point
	 * @param y
	 *            The new y-coordinate of the end point
	 */
	public void setEndPoint(double x, double y) {
		dx = x - getX();
		dy = y - getY();
		repaint();
	}

	/* Method: setEndPointPolar(r, theta) */
	/**
	 * Sets the end point of the line using polar coordinates.
	 * The origin of the line remains unchanged.
	 *
	 * @usage gline.setEndPointPolar(r, theta);
	 * @param r The polar radius length from start point
	 * @param theta The polar angle in degrees
	 */
	public void setEndPointPolar(double r, double theta) {
		dx = r * Math.cos(theta * Math.PI / 180);
		dy = -r * Math.sin(theta * Math.PI / 180);
		repaint();
	}

	/* Method: setStartPoint(x, y) */
	/**
	 * Sets the initial point in the line to (<code>x</code>,&nbsp;
	 * <code>y</code>), leaving the end point unchanged. This method is
	 * therefore different from <a href="#setLocation(double, double)">
	 * <code>setLocation</code></a>, which moves both components of the line
	 * segment.
	 *
	 * @usage gline.setStartPoint(x, y);
	 * @param x
	 *            The new x-coordinate of the origin
	 * @param y
	 *            The new y-coordinate of the origin
	 */
	public void setStartPoint(double x, double y) {
		dx += getX() - x;
		dy += getY() - y;
		setLocation(x, y);
	}
}
