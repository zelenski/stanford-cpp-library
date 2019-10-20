/*
 * @(#)GContainer.java   1.99.1 08/12/08
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
//   1. Factored out common implementation of GObjectList.
//   2. Moved GIterator code to this module.

package acm.graphics;

/* Interface: GContainer */
/**
 * Defines the functionality of an object that can serve as the parent
 * of a <a href="GObject.html"><code>GObject</code></a>.
 */
public interface GContainer {

	/* Constant: BACK_TO_FRONT */
	/** Specifies that iterators should run from back to front */
	public static final int BACK_TO_FRONT = 0;

	/* Constant: FRONT_TO_BACK */
	/** Specifies that iterators should run from front to back */
	public static final int FRONT_TO_BACK = 1;

	/* Method: add(gobj) */
	/**
	 * Adds a new graphical object to this container.
	 *
	 * @usage gc.add(gobj);
	 * @param gobj The graphical object to add
	 */
	public void add(GObject gobj);

	/* Method: add(gobj, x, y) */
	/**
	 * Adds the graphical object to this canvas and sets its location
	 * to the point (<code>x</code>,&nbsp;<code>y</code>).
	 *
	 * @usage gc.add(gobj, x, y);
	 * @param gobj The graphical object to add
	 * @param x The new x-coordinate for the object
	 * @param y The new y-coordinate for the object
	 */
	public void add(GObject gobj, double x, double y);

	/* Method: add(gobj, pt) */
	/**
	 * Adds the graphical object to this canvas and sets its location to the specified point.
	 *
	 * @usage gc.add(gobj, pt);
	 * @param gobj The graphical object to add
	 * @param pt A <code>GPoint</code> object giving the coordinates of the point
	 */
	public void add(GObject gobj, GPoint pt);

	/* Method: remove(gobj) */
	/**
	 * Removes a graphical object from this container.
	 *
	 * @usage gc.remove(gobj);
	 * @param gobj The graphical object to remove
	 */
	public void remove(GObject gobj);

	/* Method: removeAll() */
	/**
	 * Removes all graphical objects from this container.
	 *
	 * @usage gc.removeAll();
	 */
	public void removeAll();

	/* Method: getElementCount() */
	/**
	 * Returns the number of graphical objects stored in this <code>GCanvas</code>.
	 *
	 * @usage int n = gc.getElementCount();
	 * @return The number of graphical objects in this <code>GCanvas</code>
	 */
	public int getElementCount();

	/* Method: getElement(index) */
	/**
	 * Returns the graphical object at the specified index, numbering from back
	 * to front in the the <i>z</i> dimension.
	 *
	 * @usage GObject gobj = gc.getElement(index);
	 * @param index The index of the component to return
	 * @return The graphical object at the specified index
	 */
	public <T extends GObject> T getElement(int index);

	/* Method: getElementAt(x, y) */
	/**
	 * Returns the topmost graphical object that contains the point
	 * (<code>x</code>, <code>y</code>), or <code>null</code> if no such
	 * object exists.
	 *
	 * @usage GObject gobj = gc.getElementAt(x, y);
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return The graphical object at the specified location, or <code>null</code>
	 *         if no such object exists.
	 */
	public <T extends GObject> T getElementAt(double x, double y);

	/* Method: getElementAt(pt) */
	/**
	 * Returns the topmost graphical object that contains the specified point,
	 * or <code>null</code> if no such object exists.
	 *
	 * @usage GObject gobj = gc.getElementAt(pt);
	 * @param pt The coordinates being tested
	 * @return The graphical object at the specified location, or <code>null</code>
	 *         if no such object exists
	 */
	public <T extends GObject> T getElementAt(GPoint pt);
}
