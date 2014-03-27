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

import acm.util.*;
import java.awt.*;
import java.io.*;
import java.util.*;

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
	public GObject getElement(int index);

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
	public GObject getElementAt(double x, double y);

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
	public GObject getElementAt(GPoint pt);
}

/* Package class: GObjectList */
/**
 * This class implements a synchronized list of <code>GObject</code> values
 * that is shared by both <code>GCompound</code> and <code>GCanvas</code>.
 * The list includes two sets of objects: one for the ordered list in the
 * container and another for objects that are enabled to receive mouse
 * events.
 */
class GObjectList implements Serializable {

/* Constructor: new GObjectList(container) */
/**
 * Creates a new <code>GObjectList</code> with no elements.
 *
 * @param container The <code>GCanvas</code> or <code>GCompound</code> that owns this list.
 */
	public GObjectList(GContainer container) {
		parent = container;
		contents = new ArrayList<GObject>();
		if (parent instanceof GCanvas) enabledList = new ArrayList<GObject>();
	}

/* Method: add(gobj) */
/**
 * Adds the specified <code>GObject</code> to the end of the contents list,
 * and includes it in the enabled list if mouse events are enabled.
 */
	public synchronized void add(GObject gobj) {
		if (gobj.getParent() != null) gobj.getParent().remove(gobj);
		gobj.setParent(parent);
		contents.add(gobj);
		if (enabledList != null && gobj.areMouseListenersEnabled()) {
			enabledList.add(gobj);
		}
	}

/* Method: remove(gobj) */
/**
 * Removes the specified object from the list.
 */
	public synchronized void remove(GObject gobj) {
		contents.remove(gobj);
		gobj.setParent(null);
		if (enabledList != null) enabledList.remove(gobj);
	}

/* Method: removeAll() */
/**
 * Removes all objects from the list.
 */
	public synchronized void removeAll() {
		contents.clear();
		if (enabledList != null) enabledList.clear();
	}

/* Method: getElementCount() */
/**
 * Returns the number of elements in the list.
 */
	public int getElementCount() {
		return contents.size();
	}

/* Method: getElement(index) */
/**
 * Returns the graphical object at the specified index, numbering from back
 * to front in the the <i>z</i> dimension.
 */
	public GObject getElement(int index) {
		return contents.get(index);
	}

/* Method: getElementAt(x, y, requireEnabled) */
/**
 * Returns the topmost graphical object that contains the point
 * (<code>x</code>, <code>y</code>), or <code>null</code> if no such
 * object exists.
 */
	public synchronized GObject getElementAt(double x, double y, boolean requireEnabled) {
		ArrayList<GObject> list = (requireEnabled) ? enabledList : contents;
		for (int i = list.size() - 1; i >= 0; i--) {
			GObject gobj = list.get(i);
			if (gobj.contains(x, y)) return gobj;
		}
		return null;
	}

/* Method: sendToFront(gobj) */
/**
 * Implements the <code>sendToFront</code> function from the <code>GContainer</code>
 * interface.
 */
	public synchronized void sendToFront(GObject gobj) {
		int index = contents.indexOf(gobj);
		if (index >= 0) {
			contents.remove(index);
			contents.add(gobj);
		}
	}

/* Method: sendToBack(gobj) */
/**
 * Implements the <code>sendToBack</code> function from the <code>GContainer</code>
 * interface.
 */
	public synchronized void sendToBack(GObject gobj) {
		int index = contents.indexOf(gobj);
		if (index >= 0) {
			contents.remove(index);
			contents.add(0, gobj);
		}
	}

/* Method: sendForward(gobj) */
/**
 * Implements the <code>sendForward</code> function from the <code>GContainer</code>
 * interface.
 */
	public synchronized void sendForward(GObject gobj) {
		int index = contents.indexOf(gobj);
		if (index >= 0) {
			contents.remove(index);
			contents.add(Math.min(contents.size(), index + 1), gobj);
		}
	}

/* Method: sendBackward(gobj) */
/**
 * Implements the <code>sendBackward</code> function from the <code>GContainer</code>
 * interface.
 */
	public synchronized void sendBackward(GObject gobj) {
		int index = contents.indexOf(gobj);
		if (index >= 0) {
			contents.remove(index);
			contents.add(Math.max(0, index - 1), gobj);
		}
	}

/* Method: getBounds() */
/**
 * Returns the bounding rectangle for the objects in the list.
 */
	public synchronized GRectangle getBounds() {
		GRectangle bounds = new GRectangle();
		int nElements = contents.size();
		for (int i = 0; i < nElements; i++) {
			if (i == 0) {
				bounds = new GRectangle(contents.get(i).getBounds());
			} else {
				bounds.add(contents.get(i).getBounds());
			}
		}
		return bounds;
	}

/* Method: contains(x, y) */
/**
 * Checks to see whether a point is "inside" one of the objects on the list.
 */
	public synchronized boolean contains(double x, double y) {
		int nElements = contents.size();
		for (int i = 0; i < nElements; i++) {
			if (contents.get(i).contains(x, y)) return true;
		}
		return false;
	}

/* Method: mapPaint(g) */
/**
 * Paints all the elements of this container using the graphics context <code>g</code>.
 */
	public synchronized void mapPaint(Graphics g) {
		int nElements = contents.size();
		for (int i = 0; i < nElements; i++) {
			contents.get(i).paintObject(g);
		}
	}

/* Method: areMouseListenersEnabled() */
/**
 * Returns <code>true</code> if mouse listeners have ever been assigned to
 * this object or to any of the contained objects.
 */
	public synchronized boolean areMouseListenersEnabled() {
		int nElements = contents.size();
		for (int i = 0; i < nElements; i++) {
			GObject gobj = contents.get(i);
			if (gobj.areMouseListenersEnabled()) return true;
		}
		return false;
	}

/* Method: updateEnabledList() */
/**
 * Reconstructs the enabledList list in the correct order.
 */
	public synchronized void updateEnabledList() {
		enabledList.clear();
		int nElements = contents.size();
		for (int i = 0; i < nElements; i++) {
			GObject gobj = contents.get(i);
			if (gobj.areMouseListenersEnabled()) enabledList.add(gobj);
		}
	}

/* Private instance variables */
	private GContainer parent;
	private ArrayList<GObject> contents;
	private ArrayList<GObject> enabledList;
}

/* Package class: GIterator */
/**
 * Implements an iterator class for any object that implements
 * <code>GContainer</code> (i.e., <a href="GCanvas.html"><code>GCanvas</code></a>
 * and <a href="GCompound.html"><code>GCompound</code></a>).  The usual method
 * for using this class is to write something like</p>
 *
 * <p><pre><code>
 * &nbsp;    for (Iterator&lt;GObject&gt; i = gc.iterator(direction); i.hasNext(); )
 * </code></pre>
 *
 * where <code>gc</code> is the graphic container.  The enumeration supports
 * traversal in two directions.  By default, it starts with the front
 * element and works toward the back (as would be appropriate, for
 * example, when trying to find the topmost component for a mouse click).
 * You can, however, also process the elements of the container from back
 * to front (as would be useful when drawing elements of the container,
 * when the front objects should be drawn last).  To specify the direction
 * of the traversal, specify either <code>GContainer.FRONT_TO_BACK</code> or
 * <code>GContainer.BACK_TO_FRONT</code> in the <code>iterator</code> call.
 */
class GIterator implements Iterator<GObject> {

/* Constructor: GIterator(container, direction) */
/**
 * Creates a new <code>GIterator</code> that runs through the
 * container in the specified direction (<code>GContainer.FRONT_TO_BACK</code>
 * or <code>GContainer.BACK_TO_FRONT</code>).
 *
 * @usage Iterator<GObject> i = new GIterator(container, direction);
 * @param container The <code>GContainer</code> whose elements the iterator should return
 * @param direction The direction in which to process the elements
 */
	public GIterator(GContainer container, int direction) {
		switch (direction) {
		  case GContainer.FRONT_TO_BACK: case GContainer.BACK_TO_FRONT:
			dir = direction;
			break;
		  default:
			throw new ErrorException("Illegal direction for iterator");
		}
		cont = container;
		index = 0;
		nElements = container.getElementCount();
	}

/* Method: hasNext() */
/**
 * Returns <code>true</code> if the iterator has more elements.  Implements
 * the <code>hasNext</code> method for the <code>Iterator</code> interface.
 *
 * @usage while (i.hasNext()) . . .
 * @return <code>true</code> if the iterator has more elements, <code>false</code> otherwise
 */
	public boolean hasNext() {
		return index < nElements;
	}

/* Method: next() */
/**
 * Returns the next element from the iterator.  Implements the <code>next</code>
 * method for the <code>Iterator</code> interface.
 *
 * @usage Object element = i.next();
 * @return The next element from the iterator
 */
	public GObject next() {
		if (dir == GContainer.FRONT_TO_BACK) {
			return cont.getElement(nElements - index++ - 1);
		} else {
			return cont.getElement(index++);
		}
	}

/* Method: nextElement() */
/**
 * Returns the next element from the iterator as a <code>GObject</code>.  This
 * method is callable only if the iterator is declared as a <code>GIterator</code>.
 *
 * @usage GObject element = i.nextElement();
 * @return The next element from the iterator as a <code>GObject</code>
 */
	public GObject nextElement() {
		return next();
	}

/* Method: remove() */
/**
 * Removes the current element from its container.  Implements the <code>remove</code>
 * method for the <code>Iterator</code> interface.
 *
 * @usage i.remove();
 */
	public void remove() {
		if (dir == GContainer.FRONT_TO_BACK) {
			cont.remove(cont.getElement(nElements - --index - 1));
		} else {
			cont.remove(cont.getElement(--index));
		}
		nElements--;
	}

/* Private instance variables */
	private GContainer cont;
	private int dir;
	private int index;
	private int nElements;
}
