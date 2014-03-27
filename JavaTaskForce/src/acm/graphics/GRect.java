/*
 * @(#)GRect.java   1.99.1 08/12/08
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
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.
//   2. Revised bounding box calculation to conform to Java standard.

package acm.graphics;

import java.awt.*;

/* Class: GRect */
/**
 * The <code>GRect</code> class is a graphical object whose appearance consists
 * of a rectangular box.
 */
public class GRect extends GObject
  implements GFillable, GResizable, GScalable {

/* Constructor: GRect(width, height) */
/**
 * Constructs a new rectangle with the specified width and height,
 * positioned at the origin.
 *
 * @usage GRect grect = new GRect(width, height);
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 */
	public GRect(double width, double height) {
		this(0, 0, width, height);
	}

/* Constructor: GRect(x, y, width, height) */
/**
 * Constructs a new rectangle with the specified bounds.
 *
 * @usage GRect grect = new GRect(x, y, width, height);
 * @param x The x-coordinate of the upper left corner
 * @param y The y-coordinate of the upper left corner
 * @param width The width of the rectangle in pixels
 * @param height The height of the rectangle in pixels
 */
	public GRect(double x, double y, double width, double height) {
		frameWidth = width;
		frameHeight = height;
		setLocation(x, y);
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		Rectangle r = getAWTBounds();
		if (isFilled()) {
			g.setColor(getFillColor());
			g.fillRect(r.x, r.y, r.width, r.height);
			g.setColor(getColor());
		}
		g.drawRect(r.x, r.y, r.width, r.height);
	}

/* Method: setFilled(fill) */
/**
 * Sets whether this object is filled.
 *
 * @usage gobj.setFilled(fill);
 * @param fill <code>true</code> if the object should be filled, <code>false</code> for an outline
 */
	public void setFilled(boolean fill) {
		isFilled = fill;
		repaint();
	}

/* Method: isFilled() */
/**
 * Returns whether this object is filled.
 *
 * @usage if (gobj.isFilled()) . . .
 * @return The color used to display the object
 */
	public boolean isFilled() {
		return isFilled;
	}

/* Method: setFillColor(color) */
/**
 * Sets the color used to display the filled region of this object.
 *
 * @usage gobj.setFillColor(color);
 * @param color The color used to display the filled region of this object
 */
	public void setFillColor(Color color) {
		fillColor = color;
		repaint();
	}

/* Method: getFillColor() */
/**
 * Returns the color used to display the filled region of this object.  If
 * none has been set, <code>getFillColor</code> returns the color of the
 * object.
 *
 * @usage Color color = gobj.getFillColor();
 * @return The color used to display the filled region of this object
 */
	public Color getFillColor() {
		return (fillColor == null) ? getColor() : fillColor;
	}

/* Method: setSize(width, height) */
/**
 * Changes the size of this object to the specified width and height.
 *
 * @usage gobj.setSize(width, height);
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setSize(double width, double height) {
		frameWidth = width;
		frameHeight = height;
		repaint();
	}

/* Method: setSize(size) */
/**
 * Changes the size of this object to the specified <code>GDimension</code>.
 *
 * @usage gobj.setSize(size);
 * @param size A <code>GDimension</code> object specifying the size
 * @noshow
 */
	public final void setSize(GDimension size) {
		setSize(size.getWidth(), size.getHeight());
	}

/* Method: getSize() */
/**
 * Returns the size of this object as a <code>GDimension</code>.
 *
 * @usage GDimension size = gobj.getSize();
 * @return The size of this object
 */
	public GDimension getSize() {
		return new GDimension(frameWidth, frameHeight);
	}

/* Method: setBounds(x, y, width, height) */
/**
 * Changes the bounds of this object to the specified values.
 *
 * @usage gobj.setBounds(x, y, width, height);
 * @param x The new x-coordinate for the object
 * @param y The new y-coordinate for the object
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setBounds(double x, double y, double width, double height) {
		frameWidth = width;
		frameHeight = height;
		setLocation(x, y);
	}

/* Method: setBounds(bounds) */
/**
 * Changes the bounds of this object to the values from the specified
 * <code>GRectangle</code>.
 *
 * @usage gobj.setBounds(bounds);
 * @param bounds A <code>GRectangle</code> specifying the new bounds
 */
	public final void setBounds(GRectangle bounds) {
		setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	}

/* Method: getBounds() */
/**
 * Returns the bounding box of this object.
 *
 * @usage GRectangle bounds = gobj.getBounds();
 * @return The bounding box for this object
 */
	public GRectangle getBounds() {
		return new GRectangle(getX(), getY(), frameWidth, frameHeight);
	}

/* Method: getWidth() */
/**
 * Returns the width of this object as a double-precision value, which
 * is defined to be the width of the bounding box.
 *
 * @usage double width = gobj.getWidth();
 * @return The width of this object on the screen
 */
	public double getWidth() {
		return frameWidth;
	}

/* Method: getHeight() */
/**
 * Returns the height of this object as a double-precision value, which
 * is defined to be the height of the bounding box.
 *
 * @usage double height = gobj.getHeight();
 * @return The height of this object on the screen
 */
	public double getHeight() {
		return frameHeight;
	}

/* Method: scale(sx, sy) */
/**
 * Scales the object on the screen by the scale factors <code>sx</code> and <code>sy</code>.
 *
 * @usage gobj.scale(sx, sy);
 * @param sx The factor used to scale all coordinates in the x direction
 * @param sy The factor used to scale all coordinates in the y direction
 */
	public void scale(double sx, double sy) {
		frameWidth *= sx;
		frameHeight *= sy;
		repaint();
	}

/* Method: scale(sf) */
/**
 * Scales the object on the screen by the scale factor <code>sf</code>, which applies
 * in both dimensions.
 *
 * @usage gobj.scale(sf);
 * @param sf The factor used to scale all coordinates in both dimensions
 */
	public final void scale(double sf) {
		scale(sf, sf);
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

/* Protected method: getAWTBounds() */
/**
 * Returns an AWT <code>Rectangle</code> that specifies the bounds of this object.
 *
 * @usage Rectangle r = grect.getAWTBounds();
 * @return A <code>Rectangle</code> that specifies the bounds of this object
 */
	protected Rectangle getAWTBounds() {
		return new Rectangle(GMath.round(getX()), GMath.round(getY()), GMath.round(frameWidth), GMath.round(frameHeight));
	}

/* Private instance variables */
	private double frameWidth;
	private double frameHeight;
	private boolean isFilled;
	private Color fillColor;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
