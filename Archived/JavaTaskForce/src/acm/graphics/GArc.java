/*
 * @(#)GArc.java   1.99.1 08/12/08
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
// Bug fix 26-Apr-07 (ESR, JTFBug 2007-005, reported by Leland Beck)
//   1. Fixed problems with cross-file references to ArcRenderer.
//
// Bug fix 28-May-07 (ESR, JTFBug 2007-009)
//   1. Fixed synchronization problem in ArcRenderer.
//
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.
//   2. Revised bounding box calculation to conform to Java standard.

package acm.graphics;

import java.awt.*;
import java.io.*;

/* Class: GArc */
/**
 * The <code>GArc</code> class is a graphical object whose appearance consists
 * of an arc.  If unfilled, the arc is simply a portion of the circumference of
 * an ellipse; if filled, the arc is a pie-shaped wedge connected to the center
 * of the figure.
 */
public class GArc extends GObject implements GFillable, GScalable {

/* Field: ARC_TOLERANCE */
/**
 * This constant defines how close (measured in pixel units) a point has
 * to be to an arc before that point is considered to be "contained" within
 * the arc.
 */
	public static final double ARC_TOLERANCE = 2.5;
	// BUG?: THIS SEEMS TO REQUIRE HITTING THE ARC EXACTLY EVEN WITH 2.5

/* Constructor: GArc(width, height, start, sweep) */
/**
 * Creates a new <code>GArc</code> object consisting of an elliptical arc
 * located at the point (0,&nbsp;0).  For complete descriptions of the
 * other parameters, see the entry for the
 * <a href="#GArc(double, double, double, double, double, double)"><code>GArc</code></a>
 * constructor that includes explicit <code>x</code> and <code>y</code>
 * parameters.
 *
 * @usage GArc garc = new GArc(width, height, start, sweep);
 * @param width The width of the rectangle in which the arc is inscribed
 * @param height The height of the rectangle in which the arc is inscribed
 * @param start The angle at which the arc begins measured in degrees counterclockwise
 *              from the +x axis
 * @param sweep The extent of the arc, measured in degrees counterclockwise
 */
	public GArc(double width, double height, double start, double sweep) {
		this(0, 0, width, height, start, sweep);
	}

/* Constructor: GArc(x, y, width, height, start, sweep) */
/**
 * Creates a new <code>GArc</code> object consisting of an elliptical arc
 * inscribed in a rectangle located at the point (<code>x</code>,&nbsp;<code>y</code>)
 * with the specified width and height.  The <code>start</code> parameter indicates
 * the angle at which the arc begins and is measured in degrees counterclockwise
 * from the +x axis.  Thus, a <code>start</code> angle of 0 indicates an arc
 * that begins along the line running eastward from the center (the 3:00
 * o&#146;clock position), a <code>start</code> angle of 135
 * begins along the line running northwest, and a <code>start</code>
 * angle of -90 begins along the line running south (the 6:00
 * o&#146;clock position).  The <code>sweep</code> parameter indicates
 * the extent of the arc and is also measured in degrees counterclockwise.
 * A <code>sweep</code> angle of 90 defines a quarter circle extending
 * counterclockwise from the <code>start</code> angle, and a
 * <code>sweep</code> angle of -180 defines a semicircle extending
 * clockwise.
 *
 * @usage GArc garc = new GArc(x, y, width, height, start, sweep);
 * @param x The x-coordinate for the rectangle in which the arc is inscribed
 * @param y The y-coordinate for the rectangle in which the arc is inscribed
 * @param width The width of the rectangle in which the arc is inscribed
 * @param height The height of the rectangle in which the arc is inscribed
 * @param start The angle at which the arc begins measured in degrees counterclockwise
 *              from the +x axis
 * @param sweep The extent of the arc, measured in degrees counterclockwise
 */
	public GArc(double x, double y, double width, double height, double start, double sweep) {
		frameWidth = width;
		frameHeight = height;
		arcStart = start;
		arcSweep = sweep;
		setLocation(x, y);
		renderer = new ArcRenderer(this);
	}

/* Method: setStartAngle(start) */
/**
 * Sets the starting angle for this <code>GArc</code> object.
 *
 * @usage garc.setStartAngle(start);
 * @param start The new starting angle
 */
	public void setStartAngle(double start) {
		arcStart = start;
		repaint();
	}

/* Method: getStartAngle() */
/**
 * Returns the starting angle for this <code>GArc</code> object.
 *
 * @usage double start = garc.getStartAngle();
 * @return The starting angle for this arc
 */
	public double getStartAngle() {
		return arcStart;
	}

/* Method: setSweepAngle(sweep) */
/**
 * Sets the sweep angle for this <code>GArc</code> object.
 *
 * @usage garc.setSweepAngle(sweep);
 * @param sweep The new sweep angle
 */
	public void setSweepAngle(double sweep) {
		arcSweep = sweep;
		repaint();
	}

/* Method: getSweepAngle() */
/**
 * Returns the sweep angle for this <code>GArc</code> object.
 *
 * @usage double sweep = garc.getSweepAngle();
 * @return The sweep angle for this arc
 */
	public double getSweepAngle() {
		return arcSweep;
	}

/* Method: getStartPoint() */
/**
 * Returns the point at which the arc starts.
 *
 * @usage GPoint pt = garc.getStartPoint();
 * @return The point at which the arc starts
 */
	public GPoint getStartPoint() {
		return getArcPoint(arcStart);
	}

/* Method: getEndPoint() */
/**
 * Returns the point at which the arc ends.
 *
 * @usage GPoint pt = garc.getEndPoint();
 * @return The point at which the arc ends
 */
	public GPoint getEndPoint() {
		return getArcPoint(arcStart + arcSweep);
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		Component comp = getComponent();
		if (isRenderer || ((comp instanceof GCanvas) && !((GCanvas) comp).getNativeArcFlag())) {
			renderer.draw(g);
		} else {
			Rectangle r = getAWTBounds();
			int cx = GMath.round(getX() + frameWidth / 2);
			int cy = GMath.round(getY() + frameHeight / 2);
			int iStart = GMath.round(arcStart);
			int iSweep = GMath.round(arcSweep);
			if (isFilled()) {
				g.setColor(getFillColor());
				g.fillArc(r.x, r.y, r.width, r.height, iStart, iSweep);
				g.setColor(getColor());
				g.drawArc(r.x, r.y, r.width, r.height, iStart, iSweep);
				Point start = getArcPoint(iStart).toPoint();
				g.drawLine(cx, cy, start.x, start.y);
				Point end = getArcPoint(iStart + iSweep).toPoint();
				g.drawLine(cx, cy, end.x, end.y);
			} else {
				g.drawArc(r.x, r.y, r.width, r.height, iStart, iSweep);
			}
		}
	}

/* Method: getBounds() */
/**
 * Returns the bounding box of the arc.  Note that this method returns the
 * bounds of the visible portion of the arc and will therefore not contain
 * the same values as specified in
 * <a href="#setFrameRectangle(double, double, double, double)"><code>setFrameRectangle</code></a>.
 * To obtain the bounds used to describe the Java arc, use
 * <a href="#getFrameRectangle()"><code>getFrameRectangle</code></a>.
 *
 * @usage GRectangle bounds = garc.getBounds();
 * @return The bounding box of this object
 */
	public GRectangle getBounds() {
		double rx = frameWidth / 2;
		double ry = frameHeight / 2;
		double cx = getX() + rx;
		double cy = getY() + ry;
		double p1x = cx + GMath.cosDegrees(arcStart) * rx;
		double p1y = cy - GMath.sinDegrees(arcStart) * ry;
		double p2x = cx + GMath.cosDegrees(arcStart + arcSweep) * rx;
		double p2y = cy - GMath.sinDegrees(arcStart + arcSweep) * ry;
		double xMin = Math.min(p1x, p2x);
		double xMax = Math.max(p1x, p2x);
		double yMin = Math.min(p1y, p2y);
		double yMax = Math.max(p1y, p2y);
		if (containsAngle(0)) xMax = cx + rx;
		if (containsAngle(90)) yMin = cy - ry;
		if (containsAngle(180)) xMin = cx - rx;
		if (containsAngle(270)) yMax = cy + ry;
		if (isFilled()) {
			xMin = Math.min(xMin, cx);
			yMin = Math.min(yMin, cy);
			xMax = Math.max(xMax, cx);
			yMax = Math.max(yMax, cy);
		}
		return new GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
	}

/* Method: contains(x, y) */
/**
 * Checks to see whether a point is inside the object.  For the <code>GArc</code>
 * class, containment depends on whether the arc is filled.  Filled arcs are a
 * wedge in which containment can be defined in a natural way; unfilled arcs are
 * essentially lines, which means that containment is defined to mean that the
 * point is within <a href="#ARC_TOLERANCE"><code>ARC_TOLERANCE</code></a> pixels
 * of the arc.
 *
 * @usage if (garc.contains(x, y)) . . .
 * @param x The x-coordinate of the point being tested
 * @param y The y-coordinate of the point being tested
 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>) is inside
 */
	public boolean contains(double x, double y) {
		double rx = frameWidth / 2;
		double ry = frameHeight / 2;
		if (rx == 0 || ry == 0) return false;
		double dx = x - (getX() + rx);
		double dy = y - (getY() + ry);
		double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
		if (isFilled()) {
			if (r > 1.0) return false;
		} else {
			double t = ARC_TOLERANCE / ((rx + ry) / 2);
			if (Math.abs(1.0 - r) > t) return false;
		}
		return containsAngle(GMath.toDegrees(Math.atan2(-dy, dx)));
	}

/* Method: setFrameRectangle(x, y, width, height) */
/**
 * Changes the arc bounds to the specified values.
 *
 * @usage garc.setBounds(x, y, width, height);
 * @param x The x-coordinate for the rectangle in which the arc is inscribed
 * @param y The y-coordinate for the rectangle in which the arc is inscribed
 * @param width The width of the rectangle in which the arc is inscribed
 * @param height The height of the rectangle in which the arc is inscribed
 */
	public void setFrameRectangle(double x, double y, double width, double height) {
		frameWidth = width;
		frameHeight = height;
		setLocation(x, y);
	}

/* Method: setFrameRectangle(bounds) */
/**
 * Changes the arc bounds to the values from the specified <code>GRectangle</code>.
 *
 * @usage garc.setFrameRectangle(bounds);
 * @param bounds A <code>GRectangle</code> specifying the new arc bounds
 */
	public final void setFrameRectangle(GRectangle bounds) {
		setFrameRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	}

/* Method: getFrameRectangle() */
/**
 * Returns the bounds of the <code>GRectangle</code> in which this arc is inscribed.
 * Note that this is usually different from the bounding box returned by
 * <a href="#getBounds()"><code>getBounds</code></a>, which returns the bounding
 * box in which the displayed portion of the arc is contained.
 *
 * @usage GRectangle bounds = garc.getFrameRectangle();
 * @return The <code>GRectangle</code> in which this arc is inscribed
 */
	public GRectangle getFrameRectangle() {
		return new GRectangle(getX(), getY(), frameWidth, frameHeight);
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

/* Method: setFilled(fill) */
/**
 * Sets whether this object is filled.
 *
 * @usage garc.setFilled(fill);
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
 * @usage if (garc.isFilled()) . . .
 * @return The color used to display the object
 */
	public boolean isFilled() {
		return isFilled;
	}

/* Method: setFillColor(color) */
/**
 * Sets the color used to display the filled region of this object.
 *
 * @usage garc.setFillColor(color);
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
 * @usage Color color = garc.getFillColor();
 * @return The color used to display the filled region of this object
 */
	public Color getFillColor() {
		return (fillColor == null) ? getColor() : fillColor;
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
 * @usage Rectangle r = garc.getAWTBounds();
 * @return A <code>Rectangle</code> that specifies the bounds of this object
 */
	protected Rectangle getAWTBounds() {
		return new Rectangle(GMath.round(getX()), GMath.round(getY()), GMath.round(frameWidth), GMath.round(frameHeight));
	}

/* Protected method: paramString() */
/**
 * Returns a string indicating the parameters of this object.
 * @noshow
 */
	public String paramString() {
		String tail = super.paramString();
		tail = tail.substring(tail.indexOf(')') + 1);
		GRectangle r = getFrameRectangle();
		String param = "frame=(" + r.getX() + ", " + r.getY() + ", "
		               + r.getWidth() + ", " + r.getHeight() + ")";
		param += ", start=" + arcStart + ", sweep=" + arcSweep;
		return param + tail;
	}

/** Private method: getArcPoint(angle) */
/**
 * Returns the point on the ellipse for the arc at the specified angle.
 */
	private GPoint getArcPoint(double angle) {
		double rx = frameWidth / 2;
		double ry = frameHeight / 2;
		double cx = getX() + rx;
		double cy = getY() + ry;
		return new GPoint(cx + rx * GMath.cosDegrees(angle), cy - ry * GMath.sinDegrees(angle));
	}

/* Private method: containsAngle(theta) */
/**
 * Returns <code>true</code> if the arc contains the specified angle.
 */
	private boolean containsAngle(double theta) {
		double start = Math.min(getStartAngle(), getStartAngle() + getSweepAngle());
		double sweep = Math.abs(getSweepAngle());
		if (sweep >= 360) return true;
		theta = (theta < 0) ? 360 - (-theta % 360) : theta % 360;
		start = (start < 0) ? 360 - (-start % 360) : start % 360;
		if (start + sweep > 360) {
			return theta >= start || theta <= start + sweep - 360;
		} else {
			return theta >= start && theta <= start + sweep;
		}
	}

/* Package-private constructor: GArc(gobj) */
/**
 * This constructor creates a <code>GArc</code> that exists only
 * to render the arcs on behalf some other object.
 */
	GArc(GObject gobj) {
		isRenderer = true;
		renderer = new ArcRenderer(gobj);
	}

/* Private instance variables */
	private double frameWidth;
	private double frameHeight;
	private double arcStart;
	private double arcSweep;
	private Color fillColor;
	private boolean isFilled;
	private boolean isRenderer;
	private ArcRenderer renderer;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}

/* Package class: ArcRenderer */
/**
 * This class supports the implementation of rendered arcs, which are
 * used by default for arcs and ovals.  The class maintains an internal
 * polygon that stores the approximate points.  The methods that draw
 * the object automatically update the cached version if any of the
 * parameters have changed.
 */
class ArcRenderer implements Serializable {

/* Constructor: ArcRenderer(gobj) */
/**
 * Creates a new renderer object to draw the parent object.
 */
	public ArcRenderer(GObject gobj) {
		target = gobj;
		poly = new Polygon();
		cacheValid = false;
	}

/* Method: draw(g) */
/**
 * Draws the object given the specified graphics context, filling
 * it if appropriate.
 */
	public synchronized void draw(Graphics g) {
		validateCache();
		if (((GFillable) target).isFilled()) {
			g.setColor(((GFillable) target).getFillColor());
			g.fillPolygon(poly.xpoints, poly.ypoints, poly.npoints);
			g.setColor(target.getColor());
			g.drawPolygon(poly.xpoints, poly.ypoints, poly.npoints);
		} else if (target instanceof GArc) {
			g.drawPolyline(poly.xpoints, poly.ypoints, poly.npoints - 1);
		} else {
			g.drawPolygon(poly.xpoints, poly.ypoints, poly.npoints);
		}
	}

/* Private method: validateCache() */
/**
 * Makes sure that the internal polygon is up to date.
 */
	private void validateCache() {
		GRectangle bounds = null;
		double start = 0;
		double sweep = 360;
		if (target instanceof GOval || target instanceof GRoundRect) {
			bounds = target.getBounds();
		} else {
			GArc arc = (GArc) target;
			bounds = arc.getFrameRectangle();
			start = arc.getStartAngle();
			sweep = arc.getSweepAngle();
			if (start != oldStart || sweep != oldSweep) cacheValid = false;
		}
		if (bounds == null) return;
		double x = bounds.getX();
		double y = bounds.getY();
		double width = bounds.getWidth();
		double height = bounds.getHeight();
		if (width <= 0 || height <= 0) return;
		if (!cacheValid || width != oldBounds.getWidth() || height != oldBounds.getHeight()) {
			poly.npoints = 0;
			if (target instanceof GRoundRect) {
				GRoundRect rr = (GRoundRect) target;
				double arcWidth = GMath.round(rr.getArcWidth());
				double arcHeight = GMath.round(rr.getArcHeight());
				poly.addPoint(GMath.round(x + arcWidth / 2 + EPSILON), GMath.round(y + EPSILON));
				addArc(x, y, arcWidth, arcHeight, 90, 90);
				addArc(x, y + height - arcHeight, arcWidth, arcHeight, 180, 90);
				addArc(x + width - arcWidth, y + height - arcHeight, arcWidth, arcHeight, 270, 90);
				addArc(x + width - arcWidth, y, arcWidth, arcHeight, 0, 90);
			} else {
				addArc(x, y, width, height, start, sweep);
				if (target instanceof GArc) {
					poly.addPoint(GMath.round(x + width / 2 + EPSILON), GMath.round(y + height / 2 + EPSILON));
					oldStart = start;
					oldSweep = sweep;
				}
			}
			x0 = GMath.round(poly.xpoints[0] - x);
			y0 = GMath.round(poly.ypoints[0] - y);
			oldBounds = bounds;
		}
		if (x != oldBounds.getX() || y != oldBounds.getY()) {
			movePoly(GMath.round(x + EPSILON), GMath.round(y + EPSILON));
			oldBounds = bounds;
		}
		cacheValid = true;
	}

/* Private method: addArc(x, y, width, height, start, sweep) */
/**
 * Adds the points for an elliptical arc as specified by the parameters, which
 * have the same interpretation as in the <code>GArc</code> class.
 */
	private synchronized void addArc(double x, double y, double width, double height, double start, double sweep) {
		double aspectRatio = height / width;
		double rx = width / 2.0;
		double ry = height / 2.0;
		double xc = x + rx;
		double yc = y + ry;
		if (sweep < 0) {
			start += sweep;
			sweep = -sweep;
		}
		if (sweep > 359.99) sweep = 360;
		double dt = Math.atan2(1, Math.max(width, height));
		int nSteps = (int) (GMath.toRadians(sweep) / dt);
		dt = GMath.toRadians(sweep) / nSteps;
		double theta = GMath.toRadians(start);
		for (int i = 0; i <= nSteps; i++) {
			double px = xc + rx * Math.cos(theta);
			double py = yc - rx * Math.sin(theta) * aspectRatio;
			poly.addPoint(GMath.round(px + EPSILON), GMath.round(py + EPSILON));
			theta += dt;
		}
	}

/* Private method: movePoly(x, y) */
/**
 * Shifts the polygon so that its origin is now at (x, y).
 */
	private synchronized void movePoly(int x, int y) {
		int dx = x - (poly.xpoints[0] - x0);
		int dy = y - (poly.ypoints[0] - y0);
		for (int i = 0; i < poly.npoints; i++) {
			poly.xpoints[i] += dx;
			poly.ypoints[i] += dy;
		}
	}

/* Private constants */
	private static final double EPSILON = 0.00001;

/* Private instance variables */
	private GObject target;
	private Polygon poly;
	private int x0;
	private int y0;
	private GRectangle oldBounds;
	private double oldStart;
	private double oldSweep;
	private boolean cacheValid;
}
