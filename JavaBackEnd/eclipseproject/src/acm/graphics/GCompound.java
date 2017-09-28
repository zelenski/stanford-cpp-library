/*
 * @version 2017/09/27
 * - fixed bug with double-offsetting coordinates in inner GObjects
 * @version 2016/05/05
 * - re-synched with eroberts source; sort methods by name
 */

/*
 * @(#)GCompound.java   1.99.1 08/12/08
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
//   1. Removed cross-file reference to GMouseEvent.
//
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//   2. Substituted GObjectList for ArrayList.
//   3. Removed warnings about use of Iterator.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

/**
 * This class defines a graphical object that consists of a collection
 * of other graphical objects.  Once assembled, the internal objects
 * can be manipulated as a unit.
 */
public class GCompound extends GObject implements GContainer, GScalable, Iterable<GObject> {
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */
	static final long serialVersionUID = 1L;

	/* Private instance variables */
	private boolean complete;
	private GObjectList contents;
	private GObject lastObject;
	private GObject dragObject;

	/**
	 * Creates a new <code>GCompound</code> object with no internal components.
	 *
	 * @usage GCompound gcomp = new GCompound();
	 */
	public GCompound() {
		contents = new GObjectList(this);
		complete = false;
	}

	/**
	 * Adds a new graphical object to this <code>GCompound</code>.
	 *
	 * @param gobj The graphical object to add
	 * @usage gcomp.add(gobj);
	 */
	public void add(GObject gobj) {
		if (complete) {
			throw new ErrorException("You can't add objects to a GCompound that has been "
					+ "marked as complete.");
		}
		contents.add(gobj);
		repaint();
	}

	/**
	 * Adds the graphical object to this canvas and sets its location
	 * to the point (<code>x</code>,&nbsp;<code>y</code>).
	 *
	 * @param gobj The graphical object to add
	 * @param x    The new x-coordinate for the object
	 * @param y    The new y-coordinate for the object
	 * @usage gc.add(gobj, x, y);
	 */
	public final void add(GObject gobj, double x, double y) {
		gobj.setLocation(x, y);
		add(gobj);
	}

	/**
	 * Adds the graphical object to this canvas and sets its location to the specified point.
	 *
	 * @param gobj The graphical object to add
	 * @param pt   A <code>GPoint</code> object giving the coordinates of the point
	 * @usage gc.add(gobj, pt);
	 */
	public final void add(GObject gobj, GPoint pt) {
		add(gobj, pt.getX(), pt.getY());
	}

	/**
	 * Returns <code>true</code> if mouse listeners have ever been assigned to
	 * this object or to any of the contained objects.
	 *
	 * @return <code>true</code> if mouse listeners have been enabled in this object
	 * @usage if (gcomp.areMouseListenersEnabled()) . . .
	 * @noshow
	 */
	protected boolean areMouseListenersEnabled() {
		if (super.areMouseListenersEnabled()) return true;
		return contents.areMouseListenersEnabled();
	}

	/**
	 * Checks to see whether a point is "inside" the compound, which means that it is
	 * inside one of the components.
	 *
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>) is inside
	 * the compound, and <code>false</code> otherwise
	 * @usage if (gcomp.contains(x, y)) . . .
	 */
	public boolean contains(double x, double y) {
		return contents.contains(x - getX(), y - getY());
	}

	/**
	 * Dispatches a mouse event to the topmost child that covers the location
	 * in the event <code>e</code>.
	 *
	 * @noshow
	 */
	protected void fireMouseListeners(MouseEvent e) {
		if (super.areMouseListenersEnabled()) {
			super.fireMouseListeners(e);
			return;
		}
		GPoint pt = new GPoint(e.getX() - getX(), e.getY() - getY());
		GObject gobj = getElementAt(pt);
		MouseEvent newEvent = null;
		if (gobj != lastObject) {
			if (lastObject != null) {
				newEvent = GCanvas.createMouseEvent(lastObject, MouseEvent.MOUSE_EXITED, e);
				lastObject.fireMouseListeners(newEvent);
			}
			if (gobj != null) {
				newEvent = GCanvas.createMouseEvent(gobj, MouseEvent.MOUSE_ENTERED, e);
				gobj.fireMouseListeners(newEvent);
			}
		}
		lastObject = gobj;
		if (dragObject != null) {
			gobj = dragObject;
		}
		if (gobj != null) {
			int id = e.getID();
			if (id != MouseEvent.MOUSE_EXITED && id != MouseEvent.MOUSE_ENTERED) {
				if (id == MouseEvent.MOUSE_PRESSED) {
					dragObject = gobj;
				} else if (id == MouseEvent.MOUSE_RELEASED) {
					dragObject = null;
				}
				newEvent = GCanvas.createMouseEvent(gobj, id, e);
				gobj.fireMouseListeners(newEvent);
			}
		}
		if (newEvent != null && newEvent.isConsumed()) {
			e.consume();
		}
	}

	/**
	 * Returns the bounding rectangle for this compound object, which consists of
	 * the union of the bounding rectangles for each of the components.
	 *
	 * @return A <code>GRectangle</code> that bounds the components of this object
	 * @usage GRectangle bounds = gcomp.getBounds();
	 */
	public GRectangle getBounds() {
		GRectangle bounds = contents.getBounds();
		bounds.translate(getX(), getY());
		return bounds;
	}

	/**
	 * Converts the location of the specified point in this compound to
	 * the corresponding point in the enclosing canvas.
	 *
	 * @param localPoint The coordinates in the space of the compound
	 * @return The coordinates in the space of the enclosing <code>GCanvas</code>
	 * @usage canvasPoint = gcomp.getCanvasPoint(localPoint);
	 */
	public final GPoint getCanvasPoint(GPoint localPoint) {
		return getCanvasPoint(localPoint.getX(), localPoint.getY());
	}

	/**
	 * Converts the location of the specified point in this compound to
	 * the corresponding point in the enclosing canvas.
	 *
	 * @param x The x coordinate in the space of the compound
	 * @param y The y coordinate in the space of the compound
	 * @return The coordinates in the space of the enclosing <code>GCanvas</code>
	 * @usage canvasPoint = gcomp.getCanvasPoint(x, y);
	 */
	public GPoint getCanvasPoint(double x, double y) {
		for (GContainer c = this; c instanceof GCompound; ) {
			GCompound comp = (GCompound) c;
			x += comp.getX();
			y += comp.getY();
			c = comp.getParent();
		}
		return new GPoint(x, y);
	}

	/**
	 * Returns the graphical object at the specified index, numbering from back
	 * to front in the the <i>z</i> dimension.
	 *
	 * @param index The index of the component to return
	 * @return The graphical object at the specified index
	 * @usage GObject gobj = gcomp.getElement(index);
	 */
	@SuppressWarnings("unchecked")
	public <T extends GObject> T getElement(int index) {
		return (T) contents.getElement(index);
	}

	/**
	 * Returns the topmost graphical object that contains the point
	 * (<code>x</code>, <code>y</code>), or <code>null</code> if no such
	 * object exists.  Note that these coordinates are relative to the
	 * location of the compound object and not to the canvas in which
	 * it is displayed.
	 *
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return The graphical object at the specified location, or <code>null</code>
	 * if no such object exists
	 * @usage GObject gobj = gcomp.getElementAt(x, y);
	 */
	@SuppressWarnings("unchecked")
	public <T extends GObject> T getElementAt(double x, double y) {
		return (T) contents.getElementAt(x, y, /* requireEnabled */ false);
	}

	/**
	 * Returns the topmost graphical object that contains the specified point,
	 * or <code>null</code> if no such object exists.
	 *
	 * @param pt The coordinates being tested
	 * @return The graphical object at the specified location, or <code>null</code>
	 * if no such object exists
	 * @usage GObject gobj = gc.getElementAt(pt);
	 */
	@SuppressWarnings("unchecked")
	public final <T extends GObject> T getElementAt(GPoint pt) {
		return (T) getElementAt(pt.getX(), pt.getY());
	}

	/**
	 * Returns the number of graphical objects stored in this container.
	 *
	 * @return The number of graphical objects in this container
	 * @usage int n = gcomp.getElementCount();
	 */
	public int getElementCount() {
		return contents.getElementCount();
	}

	/**
	 * Converts the specified point on the enclosing canvas to the
	 * corresponding point in the space of this compound.
	 *
	 * @param x The x coordinate in the space of the space of the enclosing <code>GCanvas</code>
	 * @param y The y coordinate in the space of the space of the enclosing <code>GCanvas</code>
	 * @return The coordinates in the space of the compound
	 * @usage localPoint = gcomp.getCanvasPoint(x, y);
	 */
	public GPoint getLocalPoint(double x, double y) {
		for (GContainer c = this; c instanceof GCompound; ) {
			GCompound comp = (GCompound) c;
			x -= comp.getX();
			y -= comp.getY();
			c = comp.getParent();
		}
		return new GPoint(x, y);
	}

	/**
	 * Converts the location of the specified point on the enclosing canvas
	 * to the corresponding point in the space of this compound.
	 *
	 * @param canvasPoint The coordinates in the space of the enclosing <code>GCanvas</code>
	 * @return The coordinates in the space of the compound
	 * @usage localPoint = gcomp.getLocalPoint(canvasPoint);
	 */
	public final GPoint getLocalPoint(GPoint canvasPoint) {
		return getLocalPoint(canvasPoint.getX(), canvasPoint.getY());
	}

	/**
	 * Returns an <code>Iterator</code> that cycles through the elements within
	 * this container in the default direction, which is from back to front.
	 * You can also run the iterator in the opposite direction by using the
	 * <a href="#iterator(int)"><code>iterator</code></a><code>(</code><font
	 * size=-1><i>direction</i></font><code>)</code> form of this method.
	 *
	 * @return An <code>Iterator</code> ranging over the elements of the
	 * container from back to front
	 * @usage Iterator<GObject> i = gc.iterator();
	 */
	public Iterator<GObject> iterator() {
		return new GIterator(this, GContainer.BACK_TO_FRONT);
	}

	/**
	 * Returns an <code>Iterator</code> that cycles through the elements
	 * within this container in the specified direction, which must be one
	 * of the constants <a href="GContainer.html#FRONT_TO_BACK"><code>GContainer.FRONT_TO_BACK</code></a>
	 * or <a href="GContainer.html#BACK_TO_FRONT"><code>GContainer.BACK_TO_FRONT</code></a>.<p>
	 * <p>
	 * <p><pre><code>
	 * &nbsp;    for (Iterator&lt;GObject&gt; i = gc.iterator(direction); i.hasNext(); )
	 * </code></pre>
	 *
	 * @return An <code>Iterator</code> ranging over the elements of the
	 * container in the specified direction
	 * @usage Iterator<GObject> i = gc.iterator(direction);
	 */
	public Iterator<GObject> iterator(int direction) {
		return new GIterator(this, direction);
	}

	/**
	 * Calling this method makes it illegal to add or remove elements from the
	 * compound object.  Subclasses can invoke this method to protect the
	 * integrity of the structure from changes by the client.
	 *
	 * @usage gcomp.markAsComplete();
	 */
	public void markAsComplete() {
		complete = true;
	}

	/**
	 * Implements the <code>paint</code> operation for this graphical object.  This method
	 * is not called directly by clients.
	 *
	 * @noshow
	 */
//	public void paint(Graphics g) {
//		g = g.create();
//		g.translate(GMath.round(getX()), GMath.round(getY()));
//		contents.mapPaint(g);
//	}

	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.  This method
	 * is not called directly by clients.
	 *
	 * @noshow
	 */
	protected void paint2d(Graphics2D g) {
		// BUGBUG: GObject.paint() calls createTransformedGraphics(),
		// which translates g by x/y already.
		// So we don't need to also translate g2 here.
		
//		Graphics g2 = g.create();
//		g2.translate(GMath.round(getX()), GMath.round(getY()));
//		contents.mapPaint(g2);
		
		contents.mapPaint(g);
	}

	/**
	 * Removes a graphical object from this <code>GCompound</code>.
	 *
	 * @param gobj The graphical object to remove
	 * @usage gcomp.remove(gobj);
	 */
	public void remove(GObject gobj) {
		if (complete) {
			throw new ErrorException("You can't remove objects from a GCompound that has been "
					+ "marked as complete.");
		}
		contents.remove(gobj);
		repaint();
	}

	/**
	 * Removes all graphical objects from this <code>GCompound</code>.
	 *
	 * @usage gcomp.removeAll();
	 */
	public void removeAll() {
		if (complete) {
			throw new ErrorException("You can't remove objects from a GCompound that has been "
					+ "marked as complete.");
		}
		contents.removeAll();
		repaint();
	}

	/**
	 * Scales every object contained in this compound by the scale factors
	 * <code>sx</code> and <code>sy</code>.  Automatic repaint is turned off
	 * during the scaling operation so that at most one repaint is performed.
	 *
	 * @param sx The factor used to scale all coordinates in the x direction
	 * @param sy The factor used to scale all coordinates in the y direction
	 * @usage gcomp.scale(sx, sy);
	 */
	public void scale(double sx, double sy) {
		Component comp = getComponent();
		boolean oldAutoRepaint = false;
		if (comp instanceof GCanvas) {
			oldAutoRepaint = ((GCanvas) comp).getAutoRepaintFlag();
			((GCanvas) comp).setAutoRepaintFlag(false);
		}
		for (int i = getElementCount() - 1; i >= 0; i--) {
			GObject gobj = getElement(i);
			gobj.setLocation(sx * gobj.getX(), sy * gobj.getY());
			if (gobj instanceof GScalable) {
				((GScalable) gobj).scale(sx, sy);
			}
		}
		if (comp instanceof GCanvas) {
			((GCanvas) comp).setAutoRepaintFlag(oldAutoRepaint);
		}
		repaint();
	}

	/**
	 * Implements the <code>sendBackward</code> function from the <code>GContainer</code>
	 * interface.  Clients should not be calling this method, but the semantics of
	 * interfaces forces it to be exported.
	 *
	 * @noshow
	 */
	protected void sendBackward(GObject gobj) {
		contents.sendBackward(gobj);
		repaint();
	}

	/**
	 * Implements the <code>sendForward</code> function from the <code>GContainer</code>
	 * interface.  Clients should not be calling this method, but the semantics of
	 * interfaces forces it to be exported.
	 *
	 * @noshow
	 */
	protected void sendForward(GObject gobj) {
		contents.sendForward(gobj);
		repaint();
	}

	/**
	 * Implements the <code>sendToBack</code> function from the <code>GContainer</code>
	 * interface.  Clients should not be calling this method, but the semantics of
	 * interfaces forces it to be exported.
	 *
	 * @noshow
	 */
	protected void sendToBack(GObject gobj) {
		contents.sendToBack(gobj);
		repaint();
	}

	/**
	 * Implements the <code>sendToFront</code> function from the <code>GContainer</code>
	 * interface.  Clients should not be calling this method, but the semantics of
	 * interfaces forces it to be exported.
	 *
	 * @noshow
	 */
	protected void sendToFront(GObject gobj) {
		contents.sendToFront(gobj);
		repaint();
	}
}
