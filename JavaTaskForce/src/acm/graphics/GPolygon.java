/*
 * @(#)GPolygon.java   1.99.1 08/12/08
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
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//
// Bug fix 6-Jul-07 (ESR, JTFBug 2007-010, reported by Steve Wolfman)
//   1. Fixed bug in recenter code.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.io.*;
import java.util.*;

/* Class: GPolygon */
/**
 * The <code>GPolygon</code> class is a graphical object whose appearance consists
 * of a polygon.
 */
public class GPolygon extends GObject implements GFillable, GScalable {

/* Constructor: GPolygon() */
/**
 * Constructs a new empty polygon at the origin.
 *
 * @usage GPolygon gpoly = new GPolygon();
 */
	public GPolygon() {
		vertices = new VertexList();
		clear();
	}

/* Constructor: GPolygon(x, y) */
/**
 * Constructs a new empty polygon at (<code>x</code>, <code>y</code>).
 *
 * @usage GPolygon gpoly = new GPolygon(x, y);
 * @param x The x-coordinate of the origin of the polygon
 * @param y The y-coordinate of the origin of the polygon
 */
	public GPolygon(double x, double y) {
		this();
		setLocation(x, y);
	}

/* Constructor: GPolygon(points) */
/**
 * Constructs a new polygon from the specified array of <code>GPoint</code>
 * objects.  The polygon is automatically marked as complete.
 *
 * @usage GPolygon gpoly = new GPolygon(points);
 * @param points An array of <code>GPoint</code> objects specifying the vertices
 */
	public GPolygon(GPoint[] points) {
		this();
		vertices.add(points);
		markAsComplete();
	}

/* Method: addVertex(x, y) */
/**
 * Adds a vertex at (<code>x</code>, <code>y</code>) relative to the polygon origin.
 *
 * @usage gpoly.addVertex(x, y);
 * @param x The x-coordinate of the vertex relative to the polygon origin
 * @param y The y-coordinate of the vertex relative to the polygon origin
 */
	public void addVertex(double x, double y) {
		if (complete) {
			throw new ErrorException("You can't add vertices to a GPolygon that has been "
			                         + "marked as complete.");
		}
		vertices.addVertex(x, y);
	}

/* Method: addEdge(dx, dy) */
/**
 * Adds an edge to the polygon whose components are given by the displacements
 * <code>dx</code> and <code>dy</code> from the last vertex.
 *
 * @usage gpoly.addEdge(dx, dy);
 * @param dx The x displacement through which the edge moves
 * @param dy The y displacement through which the edge moves
 */
	public void addEdge(double dx, double dy) {
		if (complete) {
			throw new ErrorException("You can't add edges to a GPolygon that has been "
			                         + "marked as complete.");
		}
		vertices.addEdge(dx, dy);
	}

/* Method: addPolarEdge(r, theta) */
/**
 * Adds an edge to the polygon specified in polar coordinates.  The length of the
 * edge is given by <code>r</code>, and the edge extends in direction <code>theta</code>,
 * measured in degrees counterclockwise from the +x axis.
 *
 * @usage gpoly.addPolarEdge(r, theta);
 * @param r The length of the edge
 * @param theta The angle at which the edge extends measured in degrees
 */
	public final void addPolarEdge(double r, double theta) {
		if (complete) {
			throw new ErrorException("You can't add edges to a GPolygon that has been "
			                         + "marked as complete.");
		}
		vertices.addEdge(r * GMath.cosDegrees(theta), -r * GMath.sinDegrees(theta));
	}

/* Method: addArc(arcWidth, arcHeight, start, sweep) */
/**
 * Adds a series of edges to the polygon that simulates the arc specified by
 * the parameters.  The <i>x</i> and <i>y</i> parameters for the arc bounding
 * box are computed implicitly by figuring out what values would place the
 * current vertex at the starting position.
 *
 * @usage gpoly.addArc(arcWidth, arcHeight, start, sweep);
 * @param arcWidth The width of the oval from which the arc is taken
 * @param arcHeight The height of the oval from which the arc is taken
 * @param start The angle at which the arc begins
 * @param sweep The extent of the arc
 */
	public void addArc(double arcWidth, double arcHeight, double start, double sweep) {
		if (complete) {
			throw new ErrorException("You can't add edges to a GPolygon that has been "
			                         + "marked as complete.");
		}
		vertices.addArc(arcWidth, arcHeight, start, sweep);
	}

/* Method: getCurrentPoint() */
/**
 * Returns the coordinates of the last vertex added to the polygon, or <code>null</code>
 * if the polygon is empty.
 *
 * @usage GPoint vertex = gpoly.getCurrentPoint();
 * @return The last vertex added to the polygon, or <code>null</code> if empty
 */
	public GPoint getCurrentPoint() {
		return vertices.getCurrentPoint();
	}

/* Method: scale(sx, sy) */
/**
 * Scales the polygon by the scale factors <code>sx</code> and <code>sy</code>.
 *
 * @usage gpoly.scale(sx, sy);
 * @param sx The factor used to scale all coordinates in the x direction
 * @param sy The factor used to scale all coordinates in the y direction
 */
	public void scale(double sx, double sy) {
		xScale *= sx;
		yScale *= sy;
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

/* Method: rotate(theta) */
/**
 * Rotates the polygon around its origin by the angle theta, measured in degrees.
 *
 * @usage gpoly.rotate(theta);
 * @param theta The angle of rotation in degrees counterclockwise
 */
	public void rotate(double theta) {
		rotation += theta;
		repaint();
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

/* Method: getBounds() */
/**
 * Returns the bounding box of this object, which is defined to be the
 * smallest rectangle that covers everything drawn by the figure.
 *
 * @usage GRectangle bounds = gpoly.getBounds();
 * @return The bounding box for this object
 */
	public GRectangle getBounds() {
		return vertices.getBounds(getX(), getY(), xScale, yScale, rotation);
	}

/* Method: contains(x, y) */
/**
 * Checks to see whether a point is inside the object.
 *
 * @usage if (gpoly.contains(x, y)) . . .
 * @param x The x-coordinate of the point being tested
 * @param y The y-coordinate of the point being tested
 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>) is inside
 *         the object, and <code>false</code> otherwise
 */
	public boolean contains(double x, double y) {
		return vertices.contains((x - getX()) / xScale, (y - getY()) / yScale);
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		if (vertices.size() == 0) return;
		Polygon p = getPolygon();
		if (isFilled()) {
			g.setColor(getFillColor());
			g.fillPolygon(p.xpoints, p.ypoints, p.npoints);
			g.setColor(getColor());
		}
		g.drawPolygon(p.xpoints, p.ypoints, p.npoints);
	}

/* Method: recenter() */
/**
 * Recalculates the vertices of the polygon so that they are positioned
 * relative to the geometric center of the object.  This method allows
 * clients to take a polygon drawn using mouse clicks on the screen and
 * then to reformulate it so that it can be displayed relative to its center.
 *
 * @usage gpoly.recenter();
 */
	public void recenter() {
		vertices.recenter();
		cacheValid = false;
	}

/* Method: clone() */
/**
 * Overrides <code>clone</code> in <code>Object</code> to make sure
 * that the vertex list is copied rather than shared.
 * @noshow
 */
	public Object clone() {
		try {
			GPolygon clone = (GPolygon) super.clone();
			clone.vertices = new VertexList(clone.vertices);
			return clone;
		} catch (Exception CloneNotSupportedException) {
			throw new ErrorException("Impossible exception");
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

/* Protected method: repaint() */
/**
 * Overrides <code>repaint</code> in <code>GObject</code> to invalidate the
 * cached polygon.
 * @noshow
 */
	protected void repaint() {
		cacheValid = false;
		super.repaint();
	}

/* Protected method: getPolygon() */
/**
 * Returns an AWT <code>Polygon</code> whose points are as close as possible
 * to the ones in this <code>GPolygon</code>.
 *
 * @usage Polygon p = gpoly.getPolygon();
 * @return An AWT polygon corresponding to this object
 */
	protected Polygon getPolygon() {
		if (cacheValid) return poly;
		poly = vertices.createPolygon(getX(), getY(), xScale, yScale, rotation);
		cacheValid = true;
		return poly;

	}

/* Protected method: markAsComplete() */
/**
 * Calling this method makes it illegal to add or remove vertices from the
 * polygon.  Subclasses can invoke this method to protect the integrity of
 * the structure from changes by the client.
 */
	protected void markAsComplete() {
		complete = true;
	}

/* Protected method: clear() */
/**
 * Calling this method deletes all vertices from the polygon and resets the
 * scale and rotation factors to the their default values.  Subclasses can
 * use this method to reconstruct a polygon.
 */
	protected void clear() {
		if (complete) {
			throw new ErrorException("You can't clear a GPolygon that has been "
			                         + "marked as complete.");
		}
		vertices.clear();
		rotation = 0;
		xScale = 1.0;
		yScale = 1.0;
		cacheValid = false;
	}

/* Private instance variables */
	private double xScale;
	private double yScale;
	private double rotation;
	private VertexList vertices;
	private boolean cacheValid;
	private boolean complete;
	private Polygon poly;
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

/* Package class: VertexList */
/**
 * The <code>VertexList<code> class represents a list of vertices.
 */
class VertexList implements Serializable {

/* Constructor: new VertexList() */
/**
 * Creates a new <code>VertexList</code> with no elements.
 */
	public VertexList() {
		vertices = new ArrayList<GPoint>();
		cx = 0;
		cy = 0;
	}

/* Constructor: new VertexList(oldList) */
/**
 * Creates a new <code>VertexList</code> that is a clone of the old one.
 */
	public VertexList(VertexList oldList) {
		this();
		for (int i = 0; i < oldList.vertices.size(); i++) {
			vertices.add(oldList.vertices.get(i));
		}
	}

/* Method: addVertex(x, y) */
/**
 * Adds the specified vertex to the end of the list.
 */
	public synchronized void addVertex(double x, double y) {
		cx = x;
		cy = y;
		vertices.add(new GPoint(cx, cy));
	}

/* Method: addEdge(dx, dy) */
/**
 * Adds the specified edge to the end of the list.
 */
	public synchronized void addEdge(double dx, double dy) {
		cx += dx;
		cy += dy;
		vertices.add(new GPoint(cx, cy));
	}

/* Method: addArc(arcWidth, arcHeight, start, sweep) */
/**
 * Adds a series of edges to the polygon that simulates the arc specified by
 * the parameters.  The <i>x</i> and <i>y</i> parameters for the arc bounding
 * box are computed implicitly by figuring out what values would place the
 * current vertex at the starting position.
 */
	public void addArc(double arcWidth, double arcHeight, double start, double sweep) {
		double aspectRatio = arcHeight / arcWidth;
		double rx = arcWidth / 2.0;
		double ry = arcHeight / 2.0;
		double x0 = cx - rx * GMath.cosDegrees(start);
		double y0 = cy + ry * GMath.sinDegrees(start);
		if (sweep > 359.99) sweep = 360;
		if (sweep < -359.99) sweep = -360;
		double dt = Math.atan2(1, Math.max(arcWidth, arcHeight));
		int nSteps = (int) (GMath.toRadians(Math.abs(sweep)) / dt);
		dt = GMath.toRadians(sweep) / nSteps;
		double theta = GMath.toRadians(start);
		for (int i = 0; i < nSteps; i++) {
			theta += dt;
			double px = x0 + rx * Math.cos(theta);
			double py = y0 - rx * Math.sin(theta) * aspectRatio;
			addVertex(px, py);
		}
	}

/* Method: add(array) */
/**
 * Adds copies of the points to the end of the vertex list.
 */
	public synchronized void add(GPoint[] array) {
		for (int i = 0; i < array.length; i++) {
			vertices.add(new GPoint(array[i].getX(), array[i].getY()));
		}
	}

/* Method: remove(vertex) */
/**
 * Removes the specified vertex from the list.
 */
	public synchronized void remove(GPoint vertex) {
		vertices.remove(vertex);
	}

/* Method: clear() */
/**
 * Removes all vertices from the list.
 */
	public synchronized void clear() {
		vertices.clear();
	}

/* Method: size() */
/**
 * Returns the number of vertices in the list.
 */
	public int size() {
		return vertices.size();
	}

/* Method: getCurrentPoint() */
/**
 * Returns the coordinates of the last vertex added to the polygon, or <code>null</code>
 * if the polygon is empty.
 */
	public GPoint getCurrentPoint() {
		return (vertices.size() == 0) ? null : new GPoint(cx, cy);
	}

/* Method: getBounds(x0, y0, xScale, yScale, rotation) */
/**
 * Returns the bounding box for the polygon.
 */
	public synchronized GRectangle getBounds(double x0, double y0, double xScale, double yScale, double rotation) {
		int nPoints = vertices.size();
		if (nPoints == 0) return new GRectangle();
		double xMin = 0;
		double xMax = 0;
		double yMin = 0;
		double yMax = 0;
		double sinTheta = GMath.sinDegrees(rotation);
		double cosTheta = GMath.cosDegrees(rotation);
		boolean first = true;
		for (int i = 0; i < vertices.size(); i++) {
			GPoint vertex = vertices.get(i);
			double x = x0 + xScale * (cosTheta * vertex.getX() + sinTheta * vertex.getY());
			double y = y0 + yScale * (cosTheta * vertex.getY() - sinTheta * vertex.getX());
			if (first) {
				xMin = x;
				xMax = x;
				yMin = y;
				yMax = y;
				first = false;
			} else {
				xMin = Math.min(xMin, x);
				xMax = Math.max(xMax, x);
				yMin = Math.min(yMin, y);
				yMax = Math.max(yMax, y);
			}
		}
		return new GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
	}

/* Method: contains(x, y) */
/**
 * Returns <code>true</code> if the polygon described by this
 * <code>VertexList</code> contains the specified point.
 */
	public synchronized boolean contains(double x, double y) {
		int nPoints = vertices.size();
		boolean isContained = false;
		for (int i = 0; i < nPoints; i++) {
			GPoint v1 = vertices.get(i);
			GPoint v2 = vertices.get((i + 1) % nPoints);
			if (((v1.getY() < y) && (v2.getY() >= y)) || ((v2.getY() < y) && (v1.getY() >= y))) {
				if (v1.getX() + (y - v1.getY()) / (v2.getY() - v1.getY()) * (v2.getX() - v1.getX()) < x) {
					isContained = !isContained;
				}
			}
		}
		return isContained;
	}

/* Method: createPolygon(x0, y0, xScale, yScale, rotation) */
/**
 * Creates a <code>java.awt</code> <code>Polygon</code>.
 */
	public synchronized Polygon createPolygon(double x0, double y0, double xScale, double yScale, double rotation) {
		double sinTheta = GMath.sinDegrees(rotation);
		double cosTheta = GMath.cosDegrees(rotation);
		Polygon poly = new Polygon();
		for (int i = 0; i < vertices.size(); i++) {
			GPoint vertex = vertices.get(i);
			double x = x0 + xScale * (cosTheta * vertex.getX() + sinTheta * vertex.getY());
			double y = y0 + yScale * (cosTheta * vertex.getY() - sinTheta * vertex.getX());
			poly.addPoint(GMath.round(x), GMath.round(y));
		}
		return poly;
	}

/* Method: recenter() */
/**
 * Recalculates the vertices of the polygon so that they are positioned
 * relative to the geometric center of the object.  This method allows
 * clients to take a polygon drawn using mouse clicks on the screen and
 * then to reformulate it so that it can be displayed relative to its center.
 */
	public void recenter() {
		double xMin = 0;
		double xMax = 0;
		double yMin = 0;
		double yMax = 0;
		boolean first = true;
		for (int i = 0; i < vertices.size(); i++) {
			GPoint vertex = vertices.get(i);
			if (first) {
				xMin = vertex.getX();
				xMax = vertex.getX();
				yMin = vertex.getY();
				yMax = vertex.getY();
				first = false;
			} else {
				xMin = Math.min(xMin, vertex.getX());
				xMax = Math.max(xMax, vertex.getX());
				yMin = Math.min(yMin, vertex.getY());
				yMax = Math.max(yMax, vertex.getY());
			}
		}
		double xc = (xMin + xMax) / 2;
		double yc = (yMin + yMax) / 2;
		for (int i = 0; i < vertices.size(); i++ ) {
			GPoint vertex = vertices.get(i);
			vertex.translate(-xc, -yc);
		}
	}

/* Private instance variables */
	private ArrayList<GPoint> vertices;
	private double cx;
	private double cy;

};
