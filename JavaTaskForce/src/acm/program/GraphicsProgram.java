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
import java.lang.reflect.*;
import java.util.*;

/**
 * This class is a standard subclass of <code><a href="Program.html">Program</a></code>
 * whose principal window is used for drawing graphics.
 */
public abstract class GraphicsProgram extends Program {

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
	}

/* Method: run() */
/**
 * Specifies the code to be executed as the program runs.
 * The <code>run</code> method is required for applications that have
 * a thread of control that runs even in the absence of user actions,
 * such as a program that uses console interation or that involves
 * animation.  GUI-based programs that operate by setting up an initial
 * configuration and then wait for user events usually do not specify a
 * <code>run</code> method and supply a new definition for <code>init</code>
 * instead.
 */
	public void run() {
		/* Empty */
	}

/* Method: init() */
/**
 * Specifies the code to be executed as startup time before the
 * <code>run</code> method is called.  Subclasses can override this
 * method to perform any initialization code that would ordinarily
 * be included in an applet <code>init</code> method.  In general,
 * subclasses will override <code>init</code> in GUI-based programs
 * where the program simply sets up an initial state and then waits
 * for events from the user.  The <code>run</code> method is required
 * for applications in which there needs to be some control thread
 * while the program runs, as in a typical animation.
 *
 * @usage program.init();
 */
	public void init() {
		/* Empty */
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

/* Method: add(gobj) */
/**
 * Adds a new graphical object to this container.
 *
 * @usage add(gobj);
 * @param gobj The graphical object to add
 */
	public void add(GObject gobj) {
		gc.add(gobj);
	}

/* Method: add(gobj, x, y) */
/**
 * Adds the graphical object to the canvas and sets its location
 * to the point (<code>x</code>,&nbsp;<code>y</code>).
 *
 * @usage add(gobj, x, y);
 * @param gobj The graphical object to add
 * @param x The new x-coordinate for the object
 * @param y The new y-coordinate for the object
 */
	public final void add(GObject gobj, double x, double y) {
		gobj.setLocation(x, y);
		add(gobj);
	}

/* Method: add(gobj, pt) */
/**
 * Adds the graphical object to the canvas and sets its location to the specified point.
 *
 * @usage add(gobj, pt);
 * @param gobj The graphical object to add
 * @param pt The new coordinates of the point
 */
	public final void add(GObject gobj, GPoint pt) {
		gobj.setLocation(pt);
		add(gobj);
	}

/* Method: add(comp, x, y) */
/**
 * Adds the component to the canvas and sets its location
 * to the point (<code>x</code>,&nbsp;<code>y</code>).
 *
 * @usage add(comp, x, y);
 * @param comp The component to add
 * @param x The new x-coordinate for the object
 * @param y The new y-coordinate for the object
 */
	public final void add(Component comp, double x, double y) {
		comp.setLocation(GMath.round(x), GMath.round(y));
		gc.add(comp);
	}

/* Method: add(comp, pt) */
/**
 * Adds the component to the canvas and sets its location to the specified point.
 *
 * @usage add(comp, pt);
 * @param comp The component to add
 * @param pt A <code>GPoint</code> object giving the coordinates of the point
 */
	public final void add(Component comp, GPoint pt) {
		add(comp, pt.getX(), pt.getY());
	}

/* Method: remove(gobj) */
/**
 * Removes a graphical object from this container.
 *
 * @usage remove(gobj);
 * @param gobj The graphical object to remove
 */
	public void remove(GObject gobj) {
		gc.remove(gobj);
	}

/* Method: removeAll() */
/**
 * Removes all graphical objects from this container.  Note that this
 * definition overrides the <code>Container</code> version of
 * <code>removeAll</code>, which is replaced by
 * <a href="#removeAllComponents()"><code>removeAllComponents</code></a>.
 *
 * @usage removeAll();
 */
	public void removeAll() {
		gc.removeAll();
	}

/* Method: getElementCount() */
/**
 * Returns the number of graphical objects stored in this <code>GCanvas</code>.
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
 * @param index The index of the component to return
 * @return The graphical object at the specified index
 */
	public GObject getElement(int index) {
		return gc.getElement(index);
	}

/* Method: getElementAt(x, y) */
/**
 * Returns the topmost graphical object that contains the point
 * (<code>x</code>, <code>y</code>), or <code>null</code> if no such
 * object exists.
 *
 * @usage GObject gobj = program.getElementAt(x, y);
 * @param x The x-coordinate of the point being tested
 * @param y The y-coordinate of the point being tested
 * @return The graphical object at the specified location, or <code>null</code>
 *         if no such object exists.
 */
	public GObject getElementAt(double x, double y) {
		return gc.getElementAt(x, y);
	}

/* Method: getElementAt(pt) */
/**
 * Returns the topmost graphical object that contains the specified point,
 * or <code>null</code> if no such object exists.
 *
 * @usage GObject gobj = program.getElementAt(pt);
 * @param pt The coordinates being tested
 * @return The graphical object at the specified location, or <code>null</code>
 *         if no such object exists
 */
	public final GObject getElementAt(GPoint pt) {
		return getElementAt(pt.getX(), pt.getY());
	}

/* Method: iterator() */
/**
 * Returns an <code>Iterator</code> that cycles through the elements within
 * this container in the default direction, which is from back to front.
 * You can also run the iterator in the opposite direction by using the
 * <a href="#iterator(int)"><code>iterator</code></a><code>(</code><font
 * size=-1><i>direction</i></font><code>)</code> form of this method.
 *
 * <p>Applets that want to run in browsers, however, should avoid using
 * this method, because <code>Iterator</code> is not supported on 1.1 browsers.
 * For maximum portability, you should rely instead on the
 * <a href="GContainer.html#getElementCount()"><code>getElementCount</code></a>
 * and <a href="GContainer.html#getElement(int)"><code>getElement</code></a> methods,
 * which provide the same functionality in a browser-compatible way.
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
 * Returns an <code>Iterator</code> that cycles through the elements
 * within this container in the specified direction, which must be one
 * of the constants <a href="../graphics/GContainer.html#FRONT_TO_BACK"><code>FRONT_TO_BACK</code></a>
 * or <a href="GContainer.html#BACK_TO_FRONT"><code>BACK_TO_FRONT</code></a>
 * from the <a href="../graphics/GContainer.html"><code>GContainer</code></a> interface.<p>
 *
 * <code>  for (Iterator&lt;GObject&gt; i = iterator(direction); i.hasNext(); )</code>
 *
 * <p>Applets that want to run in browsers, however, should avoid using
 * this method, because <code>Iterator</code> is not supported on 1.1 browsers.
 * For maximum portability, you should rely instead on the
 * <a href="GContainer.html#getElementCount()"><code>getElementCount</code></a>
 * and <a href="GContainer.html#getElement(int)"><code>getElement</code></a> methods,
 * which provide the same functionality in a browser-compatible way.
 *
 * @usage Iterator<GObject> i = iterator(direction);
 * @return An <code>Iterator</code> ranging over the elements of the
 *         container in the specified direction
 */
	public Iterator<GObject> iterator(int direction) {
		return gc.iterator(direction);
	}

/* Method: addMouseListeners() */
/**
 * Adds the program as both a <code>MouseListener</code> and <code>MouseMotionListener</code>
 * to the canvas.
 *
 * @usage addMouseListeners();
 */
	public void addMouseListeners() {
		gc.addMouseListener(this);
		gc.addMouseMotionListener(this);
	}

/* Method: addMouseListeners(listener) */
/**
 * Adds the specified listener as a <code>MouseListener</code> and/or
 * <code>MouseMotionListener</code>, as appropriate, to the canvas.
 *
 * @usage addMouseListeners(listener);
 * @param listener A listener object that is either a <code>MouseListener</code>, a
 *        <code>MouseMotionListener</code>, or both
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
		if (!ok) throw new ErrorException("addMouseListeners: Illegal listener");
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
 * @param listener A <code>KeyListener</code> object
 */
	public void addKeyListeners(KeyListener listener) {
		gc.addKeyListener(listener);
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

/* Method: repaint() */
/**
 * Signals a need to repaint this window.
 * @noshow
 */
	public void repaint() {
		gc.repaint();
		super.repaint();
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

/* Override method: setBackground(bg) */
/**
 * Sets the background color of the <code>GCanvas</code>.
 *
 * @usage setBackground(bg);
 * @param bg The new background color
 * @noshow
 */
	public void setBackground(Color bg) {
		super.setBackground(bg);
		if (gc != null) gc.setBackground(bg);
	}

/* Static method: startGraphicsProgram(gobj, args) */
/**
 * Creates a <code>GraphicsProgram</code> containing the specified <code>GObject</code>
 * and then starts it.  This code is called only by the <code>start</code> method in
 * <code>GObject</code>.
 *
 * @usage startGraphicsProgram(gobj, args);
 * @param gobj The object to be inserted into the <code>GraphicsProgram</code>
 * @param args The array of arguments
 * @noshow
 */
	public static void startGraphicsProgram(GObject gobj, String[] args) {
		GraphicsProgram program = new GObjectProgram();
		program.setStartupObject(gobj);
		program.start(args);
	}

/* Inherited method: print(value) */
/**
 * @inherited Program#void print(String value)
 * Displays the argument value on the console, leaving the cursor at the end of
 * the output.
 */

/* Inherited method: println() */
/**
 * @inherited Program#void println()
 * Advances the console cursor to the beginning of the next line.
 */

/* Inherited method: println(value) */
/**
 * @inherited Program#void println(String value)
 * Displays the argument value on the console and then advances the cursor
 * to the next line.
 */

/* Inherited method: readLine() */
/**
 * @inherited Program#String readLine()
 * Reads and returns a line of input from the console.
 */

/* Inherited method: readLine(prompt) */
/**
 * @inherited Program#String readLine(String prompt)
 * Prompts the user for a line of input.
 */

/* Inherited method: readInt() */
/**
 * @inherited Program#int readInt()
 * Reads and returns an integer value from the user.
 */

/* Inherited method: readInt(prompt) */
/**
 * @inherited Program#int readInt(String prompt)
 * Prompts the user to enter an integer.
 */

/* Inherited method: readDouble() */
/**
 * @inherited Program#double readDouble()
 * Reads and returns a double-precision value from the user.
 */

/* Inherited method: readDouble(prompt) */
/**
 * @inherited Program#double readDouble(String prompt)
 * Prompts the user to enter a double-precision number, which is
 * returned as the value of this method.
 */

/* Inherited method: readBoolean() */
/**
 * @inherited Program#boolean readBoolean()
 * Reads and returns a boolean value (<code>true</code> or <code>false</code>).
 */

/* Inherited method: readBoolean(prompt) */
/**
 * @inherited Program#boolean readBoolean(String prompt)
 * Prompts the user to enter a boolean value.
 */

/* Inherited method: readBoolean(prompt, trueLabel, falseLabel) */
/**
 * @inherited Program#boolean readBoolean(String prompt, String trueLabel, String falseLabel)
 * Prompts the user to enter a boolean value, which is matched against the
 * labels provided.
 */

/* Inherited method: getConsole() */
/**
 * @inherited Program#IOConsole getConsole()
 * Returns the console associated with this program.
 */

/* Inherited method: getDialog() */
/**
 * @inherited Program#IODialog getDialog()
 * Returns the dialog used for user interaction.
 */

/* Inherited method: getReader() */
/**
 * @inherited Program#BufferedReader getReader()
 * Returns a <code>BufferedReader</code> whose input comes from the console.
 */

/* Inherited method: getWriter() */
/**
 * @inherited Program#PrintWriter getWriter()
 * Returns a <code>PrintWriter</code> whose output is directed to the console.
 */

/* Inherited method: setTitle(title) */
/**
 * @inherited Program#void setTitle(String title)
 * Sets the title of this program.
 */

/* Inherited method: getTitle() */
/**
 * @inherited Program#String getTitle()
 * Gets the title of this program.
 */

/* Inherited method: pause(milliseconds) */
/**
 * @inherited Program#void pause(double milliseconds)
 * Delays the calling thread for the specified time, which is expressed in
 * milliseconds.
 */

/* Factory method: createGCanvas() */
/**
 * Creates the <code>GCanvas</code> used by the <code>GraphicsProgram</code>.  Subclasses can
 * override this method to create their own <code>GCanvas</code> types.
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
		gc.repaint();
	}

/* Protected method: isStarted() */
/**
 * Checks to see whether this program has started, usually by checking to see
 * whether some pane exists.  Subclasses can override this method to ensure
 * that their structures are visible before proceeding.
 * @noshow
 */
	protected boolean isStarted() {
		if (gc == null || !super.isStarted()) return false;
		Dimension size = gc.getSize();
		return (size != null) && (size.width != 0) && (size.height != 0);
	}

/* Private instance variables */
	private GCanvas gc;
	private GProgramListener eventListener;
}

/* Package class: GProgramListener */
/**
 * The <code>GProgramListener</code> class implements the waitForClick
 * method and the objectdraw-style listener model.
 */
class GProgramListener implements MouseListener, MouseMotionListener {

/* Constructor: GProgramListener() */
/**
 * Creates the <code>GProgramListener</code>.
 */
	public GProgramListener(GraphicsProgram program) {
		myProgram = program;
		try {
			Class<?> programClass = program.getClass();
			Class[] types = { Class.forName("acm.graphics.GPoint") };
			try {
				mousePressedHook = programClass.getMethod("mousePressed", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseReleasedHook = programClass.getMethod("mouseReleased", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseClickedHook = programClass.getMethod("mouseClicked", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseMovedHook = programClass.getMethod("mouseMoved", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseDraggedHook = programClass.getMethod("mouseDragged", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Method: needsMouseMotionListeners() */
/**
 * Returns true if this listener has to respond to mouse motion events as well.
 */
	public boolean needsMouseMotionListeners() {
		return mouseMovedHook != null || mouseDraggedHook != null;
	}

/* Method: mouseClicked() */
/**
 * Called by the event-handling system when the mouse is clicked in the canvas.
 */
	public void mouseClicked(MouseEvent e) {
		if (mouseClickedHook != null) {
			Object[] args = { new GPoint(e.getX(), e.getY()) };
			try {
				mouseClickedHook.invoke(myProgram, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
		signalClickOccurred();
	}

/* Method: mousePressed() */
/**
 * Called by the event-handling system when the mouse button is pressed.
 */
	public void mousePressed(MouseEvent e) {
		if (mousePressedHook != null) {
			Object[] args = { new GPoint(e.getX(), e.getY()) };
			try {
				mousePressedHook.invoke(myProgram, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: mouseReleased() */
/**
 * Called by the event-handling system when the mouse button is released.
 */
	public void mouseReleased(MouseEvent e) {
		if (mouseReleasedHook != null) {
			Object[] args = { new GPoint(e.getX(), e.getY()) };
			try {
				mouseReleasedHook.invoke(myProgram, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: mouseEntered() */
/**
 * Called by the event-handling system when the mouse enters the component.
 */
	public void mouseEntered(MouseEvent e) {
		/* Empty */
	}

/* Method: mouseExited() */
/**
 * Called by the event-handling system when the mouse leaves the component.
 */
	public void mouseExited(MouseEvent e) {
		/* Empty */
	}

/* Method: mouseMoved() */
/**
 * Called by the event-handling system when the mouse moves.
 */
	public void mouseMoved(MouseEvent e) {
		if (mouseMovedHook != null) {
			Object[] args = { new GPoint(e.getX(), e.getY()) };
			try {
				mouseMovedHook.invoke(myProgram, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: mouseDragged() */
/**
 * Called by the event-handling system when the mouse is dragged with the button down.
 */
	public void mouseDragged(MouseEvent e) {
		if (mouseDraggedHook != null) {
			Object[] args = { new GPoint(e.getX(), e.getY()) };
			try {
				mouseDraggedHook.invoke(myProgram, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: waitForClick() */
/**
 * Waits for a mouse click in the window before proceeding.
 *
 * @usage waitForClick();
 */
	public synchronized void waitForClick() {
		clickFlag = false;
		while (!clickFlag) {
			try {
				wait();
			} catch (InterruptedException ex) {
				/* Empty */
			}
		}
	}

/* Private method: signalClickOccurred() */
/**
 * Notifies any waiting objects that a click has occurred.
 */
	private synchronized void signalClickOccurred() {
		clickFlag = true;
		notifyAll();
	}

/* Private instance variables */
	private GraphicsProgram myProgram;
	private Method mousePressedHook;
	private Method mouseReleasedHook;
	private Method mouseClickedHook;
	private Method mouseMovedHook;
	private Method mouseDraggedHook;
	private boolean clickFlag;

}

/* Package class: GObjectProgram */
/**
 * This class is used to launch a program containing a single <code>GObject</code>
 * instance at its center.
 */
class GObjectProgram extends GraphicsProgram {

/* Hook method: runHook() */
/**
 * Calls the run method in the graphical object.
 */
	protected void runHook() {
		GObject gobj = (GObject) getStartupObject();
		GDimension size = gobj.getSize();
		add(gobj, (getWidth() - size.getWidth()) / 2, (getHeight() - size.getHeight()) / 2);
		try {
			Class<?> gobjClass = gobj.getClass();
			String className = gobjClass.getName();
			className = className.substring(className.lastIndexOf(".") + 1);
			setTitle(className);
			Method run = gobjClass.getMethod("run", new Class[0]);
			if (run == null) throw new ErrorException(className + " has no run method");
			run.invoke(gobj, new Object[0]);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}
}
