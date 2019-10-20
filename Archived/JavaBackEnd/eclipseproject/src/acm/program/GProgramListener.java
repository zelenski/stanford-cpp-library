/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - alphabetized methods
 * @version 2017/04/25
 * - initial version; moved out of GraphicsProgram.java into its own file
 */

package acm.program;

import java.awt.event.*;
import java.lang.reflect.Method;
import acm.graphics.*;
import acm.util.*;

/* Package class: GProgramListener */
/**
 * The <code>GProgramListener</code> class implements the waitForClick method
 * and the objectdraw-style listener model.
 */
class GProgramListener implements MouseListener, MouseMotionListener {
	// private fields
	private GraphicsProgram myProgram;
	private Method mousePressedHook;
	private Method mouseReleasedHook;
	private Method mouseClickedHook;
	private Method mouseMovedHook;
	private Method mouseDraggedHook;
	private boolean clickFlag;

	/* Constructor: GProgramListener() */
	/**
	 * Creates the <code>GProgramListener</code>.
	 */
	public GProgramListener(GraphicsProgram program) {
		myProgram = program;
		try {
			Class<?> programClass = program.getClass();
			Class<?>[] types = { acm.graphics.GPoint.class };
			try {
				mousePressedHook = programClass
						.getMethod("mousePressed", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseReleasedHook = programClass.getMethod("mouseReleased",
						types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseClickedHook = programClass
						.getMethod("mouseClicked", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseMovedHook = programClass.getMethod("mouseMoved", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
			try {
				mouseDraggedHook = programClass
						.getMethod("mouseDragged", types);
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

	/* Method: mouseClicked() */
	/**
	 * Called by the event-handling system when the mouse is clicked in the
	 * canvas.
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

	/* Method: mouseDragged() */
	/**
	 * Called by the event-handling system when the mouse is dragged with the
	 * button down.
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

	/* Method: needsMouseMotionListeners() */
	/**
	 * Returns true if this listener has to respond to mouse motion events as
	 * well.
	 */
	public boolean needsMouseMotionListeners() {
		return mouseMovedHook != null || mouseDraggedHook != null;
	}

	/* Private method: signalClickOccurred() */
	/**
	 * Notifies any waiting objects that a click has occurred.
	 */
	private synchronized void signalClickOccurred() {
		clickFlag = true;
		notifyAll();
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
}
