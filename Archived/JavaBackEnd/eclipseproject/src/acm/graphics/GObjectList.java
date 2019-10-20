/*
 * @version 2017/04/27
 * - moved out from GContainer.java into its own file
 */

package acm.graphics;

import java.awt.*;
import java.io.*;
import java.util.*;

/* Package class: GObjectList */
/**
 * This class implements a synchronized list of <code>GObject</code> values
 * that is shared by both <code>GCompound</code> and <code>GCanvas</code>.
 * The list includes two sets of objects: one for the ordered list in the
 * container and another for objects that are enabled to receive mouse
 * events.
 */
class GObjectList implements Serializable {
	/* Private instance variables */
	private GContainer parent;
	private ArrayList<GObject> contents;
	private ArrayList<GObject> enabledList;

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
			contents.get(i).paint(g);
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
}

