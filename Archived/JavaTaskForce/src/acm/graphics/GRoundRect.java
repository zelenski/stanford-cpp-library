/*
 * @(#)GRoundRect.java   1.99.1 08/12/08
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

// -- V2.0 --
// Bug fix 26-Apr-07 (ESR, JTFBug 2007-005, reported by Leland Beck)
//   1. Fixed problems with cross-file references to ArcRenderer.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import java.awt.*;

/* Class: GRoundRect */
/**
 * The <code>GRoundRect</code> class is a graphical object whose appearance consists
 * of a rounded rectangle.
 */
public class GRoundRect extends GRect {

/* Constant: DEFAULT_ARC */
/**
 * The default arc diameter used for the corners of a rounded rectangle.
 */
	public static final double DEFAULT_ARC = 10;

/* Constructor: GRoundRect(width, height) */
/**
 * Constructs a new rounded rectangle with the specified width and height,
 * positioned at the origin.
 *
 * @usage GRoundRect rr = new GRoundRect(width, height);
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 */
	public GRoundRect(double width, double height) {
		this(0, 0, width, height, DEFAULT_ARC);
	}

/* Constructor: GRoundRect(x, y, width, height) */
/**
 * Constructs a new rounded rectangle with the specified bounds.
 *
 * @usage GRoundRect rr = new GRoundRect(x, y, width, height);
 * @param x The x-coordinate of the upper left corner
 * @param y The y-coordinate of the upper left corner
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 */
	public GRoundRect(double x, double y, double width, double height) {
		this(x, y, width, height, DEFAULT_ARC);
	}

/* Constructor: GRoundRect(x, y, width, height, arcSize) */
/**
 * Constructs a new rounded rectangle with the specified bounds and a single
 * parameter describing both the  describing the curvature at the corners.
 *
 * @usage GRoundRect rr = new GRoundRect(x, y, width, height, arcSize);
 * @param x The x-coordinate of the upper left corner
 * @param y The y-coordinate of the upper left corner
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 * @param arcSize The diameter of the circle in place at each corner
 */
	public GRoundRect(double x, double y, double width, double height,
	                  double arcSize) {
		this(x, y, width, height, arcSize, arcSize);
	}

/* Constructor: GRoundRect(x, y, width, height, arcWidth, arcHeight) */
/**
 * Constructs a new rounded rectangle with the specified bounds and
 * arc parameters describing the curvature at the corners.
 *
 * @usage GRoundRect rr = new GRoundRect(x, y, width, height, arcWidth, arcHeight);
 * @param x The x-coordinate of the upper left corner
 * @param y The y-coordinate of the upper left corner
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 * @param arcWidth The width of the oval in place at each corner
 * @param arcHeight The height of the oval in place at each corner
 */
	public GRoundRect(double x, double y, double width, double height,
	                  double arcWidth, double arcHeight) {
		super(x, y, width, height);
		aWidth = arcWidth;
		aHeight = arcHeight;
		renderer = new GArc(this);
	}

/* Method: getArcWidth() */
/**
 * Returns the <i>x</i> component of the corner radius.
 *
 * @usage double arcWidth = rr.getArcWidth();
 * @return The <i>x</i> component of the corner radius, in pixels.
 */
	public double getArcWidth() {
		return aWidth;
	}

/* Method: getArcHeight() */
/**
 * Returns the <i>y</i> component of the corner radius.
 *
 * @usage double arcHeight = rr.getArcHeight();
 * @return The <i>y</i> component of the corner radius, in pixels.
 */
	public double getArcHeight() {
		return aHeight;
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		Component comp = getComponent();
		if ((comp instanceof GCanvas) && !((GCanvas) comp).getNativeArcFlag()) {
			renderer.paint(g);
		} else {
			Rectangle r = getAWTBounds();
			int iArcWidth = GMath.round(aWidth);
			int iArcHeight = GMath.round(aHeight);
			if (isFilled()) {
				g.setColor(getFillColor());
				g.fillRoundRect(r.x, r.y, r.width, r.height, iArcWidth, iArcHeight);
				g.setColor(getColor());
			}
			g.drawRoundRect(r.x, r.y, r.width, r.height, iArcWidth, iArcHeight);
		}
	}

/* Inherited method: setLocation(x, y) */
/**
 * @inherited GObject#void setLocation(double x, double y)
 * Sets the location of this object to the point (<code>x</code>, <code>y</code>).
 */

/* Inherited method: setLocation(pt) */
/**
 * @inherited GObject#void setLocation(GPoint pt)
 * Sets the location of this object to the specified point.
 */

/* Inherited method: getLocation() */
/**
 * @inherited GObject#GPoint getLocation()
 * Returns the location of this object as a <code>GPoint</code>.
 */

/* Inherited method: getX() */
/**
 * @inherited GObject#double getX()
 * Returns the x-coordinate of the object.
 */

/* Inherited method: getY() */
/**
 * @inherited GObject#double getY()
 * Returns the y-coordinate of the object.
 */

/* Inherited method: move(dx, dy) */
/**
 * @inherited GObject#void move(double dx, double dy)
 * Moves the object on the screen using the displacements <code>dx</code> and <code>dy</code>.
 */

/* Inherited method: movePolar(r, theta) */
/**
 * @inherited GObject#void movePolar(double r, double theta)
 * Moves the object using displacements given in polar coordinates.
 */

/* Inherited method: scale(sx, sy) */
/**
 * @inherited GObject#void scale(double sx, double sy)
 * Scales the object on the screen by the scale factors <code>sx</code> and <code>sy</code>.
 */

/* Inherited method: scale(sf) */
/**
 * @inherited GObject#void scale(double sf)
 * Scales the object on the screen by the scale factor <code>sf</code>, which applies
 * in both dimensions.
 */

/* Inherited method: setSize(width, height) */
/**
 * @inherited GObject#void setSize(double width, double height)
 * Changes the size of this object to the specified width and height.
 */

/* Inherited method: getSize() */
/**
 * @inherited GObject#GDimension getSize()
 * Returns the size of the bounding box for this object.
 */

/* Inherited method: getWidth() */
/**
 * @inherited GObject#double getWidth()
 * Returns the width of this object, which is defined to be
 * the width of the bounding box.
 */

/* Inherited method: getHeight() */
/**
 * @inherited GObject#double getHeight()
 * Returns the height of this object, which is defined to be
 * the height of the bounding box.
 */

/* Inherited method: setBounds(x, y, width, height) */
/**
 * @inherited GRect#void setBounds(double x, double y, double width, double height)
 * Changes the bounds of this object to the specified parameters.
 */

/* Inherited method: setBounds(bounds) */
/**
 * @inherited GRect#void setBounds(GRectangle bounds)
 * Changes the bounds of this object as specified.
 */

/* Inherited method: getBounds() */
/**
 * @inherited GObject#GRectangle getBounds()
 * Returns the bounding box for this object.
 */

/* Inherited method: contains(x, y) */
/**
 * @inherited GObject#boolean contains(double x, double y)
 * Checks to see whether a point is inside the object.
 */

/* Inherited method: contains(pt) */
/**
 * @inherited GObject#boolean contains(GPoint pt)
 * Checks to see whether a point is inside the object.
 */

/* Inherited method: sendToFront() */
/**
 * @inherited GObject#void sendToFront()
 * Moves this object to the front of the display in the <i>z</i> dimension.
 */

/* Inherited method: sendToBack() */
/**
 * @inherited GObject#void sendToBack()
 * Moves this object to the back of the display in the <i>z</i> dimension.
 */

/* Inherited method: sendForward() */
/**
 * @inherited GObject#void sendForward()
 * Moves this object one step toward the front in the <i>z</i> dimension.
 */

/* Inherited method: sendBackward() */
/**
 * @inherited GObject#void sendBackward()
 * Moves this object one step toward the back in the <i>z</i> dimension.
 */

/* Inherited method: setColor(color) */
/**
 * @inherited GObject#void setColor(Color color)
 * Sets the color used to display this object.
 */

/* Inherited method: getColor() */
/**
 * @inherited GObject#Color getColor()
 * Returns the color used to display this object.
 */

/* Inherited method: setFillColor(color) */
/**
 * @inherited GObject#void setFillColor(Color color)
 * Sets the color used to display the filled region of this object.
 */

/* Inherited method: getFillColor() */
/**
 * @inherited GObject#Color getFillColor()
 * Returns the color used to display the filled region of this object.
 */

/* Inherited method: setFilled(fill) */
/**
 * @inherited GObject#void setFilled(boolean fill)
 * Sets whether this object is filled.
 */

/* Inherited method: isFilled() */
/**
 * @inherited GObject#boolean isFilled()
 * Returns whether this object is filled.
 */

/* Inherited method: setVisible(visible) */
/**
 * @inherited GObject#void setVisible(boolean visible)
 * Sets whether this object is visible.
 */

/* Inherited method: isVisible() */
/**
 * @inherited GObject#boolean isVisible()
 * Checks to see whether this object is visible.
 */

/* Inherited method: addMouseListener(listener) */
/**
 * @inherited GObject#void addMouseListener(MouseListener listener)
 * Adds a mouse listener to this graphical object.
 */

/* Inherited method: removeMouseListener(listener) */
/**
 * @inherited GObject#void removeMouseListener(MouseListener listener)
 * Removes a mouse listener from this graphical object.
 */

/* Inherited method: addMouseMotionListener(listener) */
/**
 * @inherited GObject#void addMouseMotionListener(MouseMotionListener listener)
 * Adds a mouse motion listener to this graphical object.
 */

/* Inherited method: removeMouseMotionListener(listener) */
/**
 * @inherited GObject#void removeMouseMotionListener(MouseMotionListener listener)
 * Removes a mouse motion listener from this graphical object.
 */
/* Private instance variables */
	private double aWidth, aHeight;
	private GArc renderer;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
