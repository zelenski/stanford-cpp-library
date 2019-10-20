/*
 * @version 2018/06/20
 * - added mouse wheel events
 * @version 2017/06/10
 * - added setOpaque/isOpaque
 * @version 2017/04/26
 * - added setShowPixelInfo, setShowPixelGrid
 * - added remove(x, y), removeAll(x, y)
 * @version 2017/04/25
 * - added Save / Save As menu actions for saving graphics output to an image
 * - added Compare Output menu action for checking graphics output against an expected image
 * - alphabetized methods
 * - moved GProgramListener, GObjectProgram classes out into their own files
 * @version 2016/10/12
 * - added clear method as alias for removeAll
 * - added getCanvasSize
 * @version 2016/05/05
 * - added get/setCanvasSize,Width,Height methods for better control over central drawing canvas size
 * - modified setSize to call setCanvasSize because that's what students actually want
 * @version 2016/04/26
 * - modified return type of getElementAt to avoid need for type cast
 * - made GraphicsProgram implement Iterable<GObject>
 * @version 2015/05/09
 * - added methods to control pause/tick functionality
 * @version 2015/04/21
 * - incorporated addMouse/KeyListeners automatically so they don't need to be called
 */

/*
 * @(#)GraphicsProgram.java   1.99.1 08/12/08
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
// Code cleanup 30-Jan-07 (ESR)
//   1. Renamed instance variable "listener" to "eventListener" to avoid
//      warning messages from some compilers.
//   2. Removed unnecessary startHook code.

package acm.program;

import acm.graphics.*;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.util.*;

/**
 * This class is a standard subclass of
 * <code><a href="Program.html">Program</a></code> whose principal window is
 * used for drawing graphics.
 */
public abstract class GraphicsProgram extends Program
		implements GraphicsProgramInterface, Iterable<GObject> {
	// private fields
	private GCanvas gc;
	private GProgramListener eventListener;
	private boolean listenersAdded = false;

	/* Constructor: GraphicsProgram() */
	/**
	 * Creates a new graphics program.
	 * 
	 * @usage GraphicsProgram program = new GraphicsProgram();
	 */
	protected GraphicsProgram() {
		eventListener = new GProgramListener(this);
		gc = createGCanvas();
		gc.addMouseListener(eventListener);
		if (eventListener.needsMouseMotionListeners()) {
			gc.addMouseMotionListener(eventListener);
		}
		add(gc, CENTER);
		validate();

		// added by Marty Stepp 2015/04/21 - no reason not to hear mouse/key events
		addMouseListeners();
		addKeyListeners();
	}

	/* Method: add(gobj) */
	/**
	 * Adds a new graphical object to this container.
	 * 
	 * @usage add(gobj);
	 * @param gobj
	 *            The graphical object to add
	 */
	public void add(GObject gobj) {
		gc.add(gobj);
	}

	/* Method: add(gobj, x, y) */
	/**
	 * Adds the graphical object to the canvas and sets its location to the
	 * point (<code>x</code>,&nbsp;<code>y</code>).
	 * 
	 * @usage add(gobj, x, y);
	 * @param gobj
	 *            The graphical object to add
	 * @param x
	 *            The new x-coordinate for the object
	 * @param y
	 *            The new y-coordinate for the object
	 */
	public final void add(GObject gobj, double x, double y) {
		gobj.setLocation(x, y);
		add(gobj);
	}

	/* Method: add(gobj, pt) */
	/**
	 * Adds the graphical object to the canvas and sets its location to the
	 * specified point.
	 * 	
	 * @usage add(gobj, pt);
	 * @param gobj
	 *            The graphical object to add
	 * @param pt
	 *            The new coordinates of the point
	 */
	public final void add(GObject gobj, GPoint pt) {
		gobj.setLocation(pt);
		add(gobj);
	}

	/* Method: add(comp, x, y) */
	/**
	 * Adds the component to the canvas and sets its location to the point (
	 * <code>x</code>,&nbsp;<code>y</code>).
	 * 
	 * @usage add(comp, x, y);
	 * @param comp
	 *            The component to add
	 * @param x
	 *            The new x-coordinate for the object
	 * @param y
	 *            The new y-coordinate for the object
	 */
	public final void add(Component comp, double x, double y) {
		comp.setLocation(GMath.round(x), GMath.round(y));
		gc.add(comp);
	}

	/* Method: add(comp, pt) */
	/**
	 * Adds the component to the canvas and sets its location to the specified
	 * point.
	 * 
	 * @usage add(comp, pt);
	 * @param comp
	 *            The component to add
	 * @param pt
	 *            A <code>GPoint</code> object giving the coordinates of the
	 *            point
	 */
	public final void add(Component comp, GPoint pt) {
		add(comp, pt.getX(), pt.getY());
	}

	/* Method: addKeyListeners() */
	/**
	 * Adds the program as a <code>KeyListener</code> to the canvas.
	 * 
	 * @usage addKeyListeners();
	 */
	public void addKeyListeners() {
		gc.addKeyListener(this);
	}

	/* Method: addKeyListeners(listener) */
	/**
	 * Adds the specified listener as a <code>KeyListener</code> to the canvas.
	 * 
	 * @usage addKeyListeners(listener);
	 * @param listener
	 *            A <code>KeyListener</code> object
	 */
	public void addKeyListeners(KeyListener listener) {
		gc.addKeyListener(listener);
	}

	/* Method: addMouseListeners() */
	/**
	 * Adds the program as both a <code>MouseListener</code> and
	 * <code>MouseMotionListener</code> to the canvas.
	 * 
	 * @usage addMouseListeners();
	 */
	public void addMouseListeners() {
		if (!listenersAdded) {
			// avoid attaching same listeners twice
			gc.addMouseListener(this);
			gc.addMouseMotionListener(this);
			gc.addMouseWheelListener(this);
			listenersAdded = true;
		}
	}

	/* Method: addMouseListeners(listener) */
	/**
	 * Adds the specified listener as a <code>MouseListener</code> and/or
	 * <code>MouseMotionListener</code>, as appropriate, to the canvas.
	 * 
	 * @usage addMouseListeners(listener);
	 * @param listener
	 *            A listener object that is either a <code>MouseListener</code>,
	 *            a <code>MouseMotionListener</code>, or both
	 */
	public void addMouseListeners(EventListener listener) {
		boolean ok = false;
		if (listener instanceof MouseListener) {
			gc.addMouseListener((MouseListener) listener);
			ok = true;
		}
		if (listener instanceof MouseMotionListener) {
			gc.addMouseMotionListener((MouseMotionListener) listener);
			ok = true;
		}
		if (!ok)
			throw new ErrorException("addMouseListeners: Illegal listener");
	}

	/* Method: clear() */
	/**
	 * Removes all graphical objects from this container.
	 * Equivalent to removeAll but also clears pixel data.
	 * 
	 * @usage clear();
	 */
	public void clear() {
		gc.clear();
	}

	/* Method: clearCanvas() */
	/**
	 * Removes all graphical objects from this container.
	 * Equivalent to removeAll.
	 * 
	 * @usage clearCanvas();
	 */
	public void clearCanvas() {
		removeAll();
	}

	/* Factory method: createGCanvas() */
	/**
	 * Creates the <code>GCanvas</code> used by the <code>GraphicsProgram</code>
	 * . Subclasses can override this method to create their own
	 * <code>GCanvas</code> types.
	 * 
	 * @usage GCanvas gc = program.createGCanvas();
	 * @return The <code>GCanvas</code> to be inserted into the program
	 * @noshow
	 */
	protected GCanvas createGCanvas() {
		return new GCanvas();
	}

	/* Protected method: endHook() */
	/**
	 * Ensures that the window is repainted at the end of the program.
	 */
	protected void endHook() {
		super.endHook();
		gc.repaint();
	}

	/**
	 * Returns the height of the central canvas area.
	 * @return the height of the central canvas area
	 */
	public double getCanvasHeight() {
		if (gc != null) {
			return gc.getHeight();
		} else {
			return getHeight();
		}
	}

	/**
	 * Returns the size of the central canvas area.
	 * @return the size of the central canvas area
	 */
	public Dimension getCanvasSize() {
		return new Dimension((int) getCanvasWidth(), (int) getCanvasHeight());
	}

	/**
	 * Returns the width of the central canvas area.
	 * @return the width of the central canvas area
	 */
	public double getCanvasWidth() {
		if (gc != null) {
			return gc.getWidth();
		} else {
			return getWidth();
		}
	}

	/* Method: getElementCount() */
	/**
	 * Returns the number of graphical objects stored in this
	 * <code>GCanvas</code>.
	 * 
	 * @usage int n = getElementCount();
	 * @return The number of graphical objects in this <code>GCanvas</code>
	 */
	public int getElementCount() {
		return gc.getElementCount();
	}

	/* Method: getElement(index) */
	/**
	 * Returns the graphical object at the specified index, numbering from back
	 * to front in the the <i>z</i> dimension.
	 * 
	 * @usage GObject gobj = getElement(index);
	 * @param index
	 *            The index of the component to return
	 * @return The graphical object at the specified index
	 */
	@SuppressWarnings("unchecked")
	public <T extends GObject> T getElement(int index) {
		return (T) gc.getElement(index);
	}

	/* Method: getElementAt(x, y) */
	/**
	 * Returns the topmost graphical object that contains the point (
	 * <code>x</code>, <code>y</code>), or <code>null</code> if no such object
	 * exists.
	 * 
	 * @usage GObject gobj = program.getElementAt(x, y);
	 * @param x
	 *            The x-coordinate of the point being tested
	 * @param y
	 *            The y-coordinate of the point being tested
	 * @return The graphical object at the specified location, or
	 *         <code>null</code> if no such object exists.
	 */
	@SuppressWarnings("unchecked")
	public <T extends GObject> T getElementAt(double x, double y) {
		GObject result = gc.getElementAt(x, y);
		if (result != null && invisibleObjects.contains(result)) {
			return null;
		} else {
			return (T) result;
		}
	}

	/**
	 * This version of getElementAt accepts a variable number of coordinate
	 * pairs (in x1, y1, x2, y2, x3, y3, ... order) and will return the
	 * topmost graphical objectfound at any of these pairs.
	 * The pairs are checked in the order they are passed.
	 * If no graphical object is found at any of these coordinate pairs,
	 * null is returned. 
	 */
	public <T extends GObject> T getElementAt(double... coords) {
		return gc.getElementAt(coords);
	}

	/* Method: getElementAt(pt) */
	/**
	 * Returns the topmost graphical object that contains the specified point,
	 * or <code>null</code> if no such object exists.
	 * 
	 * @usage GObject gobj = program.getElementAt(pt);
	 * @param pt
	 *            The coordinates being tested
	 * @return The graphical object at the specified location, or
	 *         <code>null</code> if no such object exists
	 */
	public final <T extends GObject> T getElementAt(GPoint pt) {
		return getElementAt(pt.getX(), pt.getY());
	}

	/* Method: getGCanvas() */
	/**
	 * Returns the <code>GCanvas</code> object used by this program.
	 * 
	 * @usage GCanvas gc = getGCanvas();
	 * @return The <code>GCanvas</code> object used by the program
	 */
	public GCanvas getGCanvas() {
		return gc;
	}

	/**
	 * Returns true if a graphical object exists that touches the given
	 * (x, y) pixel position, or false if no such object exists.
	 */
	public boolean hasElementAt(double x, double y) {
		return gc.hasElementAt(x, y);
	}

	/**
	 * Returns true if a graphical object exists that touches any of the given
	 * (x, y) pixel positions, or false if no such object exists.
	 * This method accepts a variable number of coordinate
	 * pairs (in x1, y1, x2, y2, x3, y3, ... order).
	 * The pairs are checked in the order they are passed.
	 */
	public boolean hasElementAt(double... coords) {
		return gc.hasElementAt(coords);
	}

	/**
	 * Returns true if a graphical object exists that touches the given
	 * (x, y) pixel position, or false if no such object exists.
	 */
	public boolean hasElementAt(GPoint point) {
		return gc.hasElementAt(point);
	}

	/* Method: init() */
	/**
	 * Specifies the code to be executed as startup time before the
	 * <code>run</code> method is called. Subclasses can override this method to
	 * perform any initialization code that would ordinarily be included in an
	 * applet <code>init</code> method. In general, subclasses will override
	 * <code>init</code> in GUI-based programs where the program simply sets up
	 * an initial state and then waits for events from the user. The
	 * <code>run</code> method is required for applications in which there needs
	 * to be some control thread while the program runs, as in a typical
	 * animation.
	 * 
	 * @usage program.init();
	 */
	public void init() {
		/* Empty */
	}

	/**
	 * Whether this program's canvas uses anti-aliasing, which is
	 * smoothing and blending of neighboring pixels.
	 * Default true.
	 */
	public boolean isAntiAliasing() {
		return gc.isAntiAliasing();
	}
	
	/**
	 * Whether this program has an opaque background.
	 * Default false.
	 */
	public boolean isOpaque() {
		return gc.isOpaque();
	}
	
	/* Protected method: isStarted() */
	/**
	 * Checks to see whether this program has started, usually by checking to
	 * see whether some pane exists. Subclasses can override this method to
	 * ensure that their structures are visible before proceeding.
	 * 
	 * @noshow
	 */
	protected boolean isStarted() {
		if (gc == null || !super.isStarted())
			return false;
		Dimension size = gc.getSize();
		return (size != null) && (size.width != 0) && (size.height != 0);
	}

	/* Method: iterator() */
	/**
	 * Returns an <code>Iterator</code> that cycles through the elements within
	 * this container in the default direction, which is from back to front. You
	 * can also run the iterator in the opposite direction by using the <a
	 * href="#iterator(int)"><code>iterator</code></a><code>(</code><font
	 * size=-1><i>direction</i></font><code>)</code> form of this method.
	 * 
	 * <p>
	 * Applets that want to run in browsers, however, should avoid using this
	 * method, because <code>Iterator</code> is not supported on 1.1 browsers.
	 * For maximum portability, you should rely instead on the <a
	 * href="GContainer.html#getElementCount()"><code>getElementCount</code></a>
	 * and <a href="GContainer.html#getElement(int)"><code>getElement</code></a>
	 * methods, which provide the same functionality in a browser-compatible
	 * way.
	 * 
	 * @usage Iterator<GObject> i = iterator();
	 * @return An <code>Iterator</code> ranging over the elements of the
	 *         container from back to front
	 */
	public Iterator<GObject> iterator() {
		return gc.iterator();
	}

	/* Method: iterator(direction) */
	/**
	 * Returns an <code>Iterator</code> that cycles through the elements within
	 * this container in the specified direction, which must be one of the
	 * constants <a href="../graphics/GContainer.html#FRONT_TO_BACK">
	 * <code>FRONT_TO_BACK</code></a> or <a
	 * href="GContainer.html#BACK_TO_FRONT"><code>BACK_TO_FRONT</code></a> from
	 * the <a href="../graphics/GContainer.html"><code>GContainer</code></a>
	 * interface.
	 * <p>
	 * 
	 * <code>  for (Iterator&lt;GObject&gt; i = iterator(direction); i.hasNext(); )</code>
	 * 
	 * <p>
	 * Applets that want to run in browsers, however, should avoid using this
	 * method, because <code>Iterator</code> is not supported on 1.1 browsers.
	 * For maximum portability, you should rely instead on the <a
	 * href="GContainer.html#getElementCount()"><code>getElementCount</code></a>
	 * and <a href="GContainer.html#getElement(int)"><code>getElement</code></a>
	 * methods, which provide the same functionality in a browser-compatible
	 * way.
	 * 
	 * @usage Iterator<GObject> i = iterator(direction);
	 * @return An <code>Iterator</code> ranging over the elements of the
	 *         container in the specified direction
	 */
	public Iterator<GObject> iterator(int direction) {
		return gc.iterator(direction);
	}
	
	/**
	 * Removes the top-most graphical object at the given (x, y) position from this container.
	 * If no graphical object is located at that position, does nothing.
	 * 
	 * @usage remove(x, y);
	 */
	public void remove(double x, double y) {
		gc.remove(x, y);
	}

	/**
	 * Removes the top-most graphical object at the given (x, y) position from this container.
	 * If no graphical object is located at that position, does nothing.
	 * 
	 * @usage remove(pt);
	 */
	public void remove(GPoint pt) {
		gc.remove(pt);
	}

	/* Method: remove(gobj) */
	/**
	 * Removes a graphical object from this container.
	 * 
	 * @usage remove(gobj);
	 * @param gobj
	 *            The graphical object to remove
	 */
	public void remove(GObject gobj) {
		gc.remove(gobj);
	}

	/* Method: removeAll() */
	/**
	 * Removes all graphical objects from this container. Note that this
	 * definition overrides the <code>Container</code> version of
	 * <code>removeAll</code>, which is replaced by <a
	 * href="#removeAllComponents()"><code>removeAllComponents</code></a>.
	 * 
	 * @usage removeAll();
	 */
	public void removeAll() {
		gc.removeAll();
	}
	
	/**
	 * Removes all graphical objects at the given (x, y) position from this container.
	 * If no graphical objects are located at that position, does nothing.
	 * 
	 * @usage removeAll(x, y);
	 */
	public void removeAll(double x, double y) {
		gc.removeAll(x, y);
	}

	/**
	 * Removes all graphical objects at the given (x, y) positions from this container.
	 * If no graphical objects are located at that position, does nothing.
	 * 
	 * @usage removeAll(x1, y1, x2, y2, ...);
	 */
	public void removeAll(double... coords) {
		gc.removeAll(coords);
	}

	/**
	 * Removes all graphical objects at the given (x, y) position from this container.
	 * If no graphical objects are located at that position, does nothing.
	 * 
	 * @usage removeAll(pt);
	 */
	public void removeAll(GPoint pt) {
		gc.removeAll(pt);
	}

	/* Method: removeAllComponents() */
	/**
	 * Removes all components from this container.
	 * 
	 * @usage removeAllComponents();
	 * @noshow
	 */
	public void removeAllComponents() {
		super.removeAll();
	}

	/* Method: repaint() */
	/**
	 * Signals a need to repaint this window.
	 * 
	 * @noshow
	 */
	public void repaint() {
		gc.repaint();
		super.repaint();
	}

	/* Method: run() */
	/**
	 * Specifies the code to be executed as the program runs. The
	 * <code>run</code> method is required for applications that have a thread
	 * of control that runs even in the absence of user actions, such as a
	 * program that uses console interation or that involves animation.
	 * GUI-based programs that operate by setting up an initial configuration
	 * and then wait for user events usually do not specify a <code>run</code>
	 * method and supply a new definition for <code>init</code> instead.
	 */
	public void run() {
		// empty
	}
	
	/**
	 * Sets whether this program's canvas uses anti-aliasing, which is
	 * smoothing and blending of neighboring pixels.
	 * Default true.
	 */
	public void setAntiAliasing(boolean antialias) {
		gc.setAntiAliasing(antialias);
	}

	/* Override method: setBackground(bg) */
	/**
	 * Sets the background color of the <code>GCanvas</code>.
	 * 
	 * @usage setBackground(bg);
	 * @param bg
	 *            The new background color
	 * @noshow
	 */
	public void setBackground(Color bg) {
		super.setBackground(bg);
		if (gc != null) {
			gc.setBackground(bg);
		}
	}

	/**
	 * Sets this program to be exactly the right size so that its graphical canvas will be
	 * the given height in pixels.
	 * This is often preferred over setHeight() because you'll know exactly how many pixels
	 * are available to you for drawing.
	 * @param height the desired height of the drawing canvas, in pixels
	 */
	public void setCanvasHeight(double height) {
		setCanvasSize(getCanvasWidth(), height);
	}

	/**
	 * Sets this program to be exactly the right size so that its graphical canvas will be
	 * the given size in pixels.
	 * This is often preferred over setSize() because you'll know exactly how many pixels
	 * are available to you for drawing.
	 * @param width the desired width of the drawing canvas, in pixels
	 * @param height the desired height of the drawing canvas, in pixels
	 */
	public void setCanvasSize(double width, double height) {
		if (gc != null) {
			Dimension dim = new Dimension((int) width, (int) height);
			gc.setSize(dim);
			gc.setPreferredSize(dim);
			Window window = getWindow();
			if (window != null) {
				window.pack();
			}
		}
	}

	/**
	 * Sets this program to be exactly the right size so that its graphical canvas will be
	 * the given width in pixels.
	 * This is often preferred over setWidth() because you'll know exactly how many pixels
	 * are available to you for drawing.
	 * @param width the desired width of the drawing canvas, in pixels
	 */
	public void setCanvasWidth(double width) {
		setCanvasSize(width, getCanvasHeight());
	}

	/**
	 * Sets whether this program has an opaque background.
	 * Default false.
	 */
	public void setOpaque(boolean opaque) {
		gc.setOpaque(opaque);
	}
	
	/**
	 * Sets this program to be exactly the right size so that its graphical canvas will be
	 * the given size in pixels.
	 * @param width the desired width of the drawing canvas, in pixels
	 * @param height the desired height of the drawing canvas, in pixels
	 */
	@Override
	public void setSize(double width, double height) {
		String caller = getMyCaller();
		if (caller.startsWith("java.") || caller.startsWith("javax.")) {
			super.setSize(width, height);
		} else {
			super.setSize(width, height);
			setCanvasSize(width, height);
		}
	}
	
	/**
	 * Sets whether to display information on this canvas about the current pixel where
	 * the mouse pointer is resting in this program's graphical canvas.
	 * Displays the current pixel's (x, y) coordinate and color.
	 */
	public void setShowPixelInfo(boolean show) {
		gc.setShowPixelInfo(show);
	}

	/**
	 * Sets this program to be exactly the right size so that its graphical canvas will be
	 * the given size in pixels.
	 * @param width the desired width of the drawing canvas, in pixels
	 * @param height the desired height of the drawing canvas, in pixels
	 */
	@Override
	public void setSize(int width, int height) {
		String caller = getMyCaller();
		if (caller.startsWith("java.") || caller.startsWith("javax.")) {
			super.setSize(width, height);
		} else {
			super.setSize(width, height);
			setCanvasSize(width, height);
		}
	}

	/**
	 * Sets this program's window to be the given size in pixels.
	 * @param width the desired width of the window, in pixels
	 * @param height the desired height of the window, in pixels
	 */
	public void setWindowSize(double width, double height) {
		super.setSize(width, height);
	}

	/* Static method: startGraphicsProgram(gobj, args) */
	/**
	 * Creates a <code>GraphicsProgram</code> containing the specified
	 * <code>GObject</code> and then starts it. This code is called only by the
	 * <code>start</code> method in <code>GObject</code>.
	 * 
	 * @usage startGraphicsProgram(gobj, args);
	 * @param gobj
	 *            The object to be inserted into the
	 *            <code>GraphicsProgram</code>
	 * @param args
	 *            The array of arguments
	 * @noshow
	 */
	public static void startGraphicsProgram(GObject gobj, String[] args) throws Throwable {
		GraphicsProgram program = new GObjectProgram();
		program.setStartupObject(gobj);
		program.start(args);
	}

	/**
	 * Returns an image representation of the current contents of the screen.
	 */
	public BufferedImage toImage() {
		return gc.toImage();
	}
	
	/* Method: waitForClick() */
	/**
	 * Waits for a mouse click in the window before proceeding.
	 * 
	 * @usage waitForClick();
	 */
	public void waitForClick() {
		eventListener.waitForClick();
	}
}
