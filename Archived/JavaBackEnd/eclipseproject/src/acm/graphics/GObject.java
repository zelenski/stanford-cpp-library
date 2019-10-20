/*
 * @version 2017/10/14
 * - use GraphicsUtils for antialiasing
 * @version 2017/05/06
 * - added get/setCenterX/Y/Location
 * @version 2016/05/05
 * - re-synched with eroberts source
 * - alphabetized method names
 * @version 2016/05/04
 * - fixed bug with contains() always returning false
 */

// REVISION HISTORY
//
// -- V2.0 --
// Feature extension 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.io.*;
import java.lang.reflect.*;

/**
 * This class is the common superclass of all graphical objects that can
 * be displayed on a <a href="GCanvas.html"><code>GCanvas</code></a>.
 * Because it is an abstract class, you are not allowed to construct an
 * object whose class is <code>GObject</code> directly.  What you do
 * instead is construct one of the concrete subclasses like
 * <a href="GRect.html"><code>GRect</code></a> or
 * <a href="GLine.html"><code>GLine</code></a>.
 * The purpose of this class definition is to define methods that apply
 * to all graphical objects regardless of their specific class.
 * <p>
 * <p>The <code>GObject</code> class implements the <code>Serializable</code>
 * interface by saving all of the internal state of the object.  The parent,
 * however, is saved only if the parent is a <code>GCompound</code>.
 */
public abstract class GObject implements Cloneable, Serializable, GScalable {
	private static boolean antialiasing = true;

	public static boolean isAntiAliasing() {
		return antialiasing;
	}

	public static void setAntiAliasing(boolean value) {
		antialiasing = value;
	}

	private GCompound compoundParent;
	private AffineTransform matrix = null;
	private Color objectColor;
	private double lineWidth = 1.0;
	private double xc;
	private double yc;
	private boolean isVisible = true;
	private boolean mouseListenersEnabled = false;
	private transient MouseListener mouseListener;
	private transient MouseMotionListener mouseMotionListener;
	private transient ActionListener actionListener;
	private transient GContainer transientParent;
	static final long serialVersionUID = 21L;

	/**
	 * Constructs a new <code>GObject</code> and initializes its state.  This
	 * constructor is never called explicitly, but is instead invoked by the
	 * constructors of its subclasses.
	 */
	protected GObject() {
		// empty
	}

	/**
	 * Adds an action listener to this graphical object.
	 *
	 * @param listener Any object that implements the <code>ActionListener</code> interface
	 * @usage gobj.addActionListener(listener);
	 */
	public void addActionListener(ActionListener listener) {
		actionListener = AWTEventMulticaster.add(actionListener, listener);
	}

	/**
	 * Adds a mouse listener to this graphical object.
	 *
	 * @param listener Any object that implements the <code>MouseListener</code> interface
	 * @usage gobj.addMouseListener(listener);
	 */
	public void addMouseListener(MouseListener listener) {
		mouseListener = AWTEventMulticaster.add(mouseListener, listener);
		mouseListenersEnabled = true;
		updateEnabledList();
	}

	/**
	 * Adds a mouse motion listener to this graphical object.
	 *
	 * @param listener Any object that implements the <code>MouseMotionListener</code> interface
	 * @usage gobj.addMouseMotionListener(listener);
	 */
	public void addMouseMotionListener(MouseMotionListener listener) {
		mouseMotionListener = AWTEventMulticaster.add(mouseMotionListener, listener);
		mouseListenersEnabled = true;
		updateEnabledList();
	}

	/**
	 * Returns <code>true</code> if mouse listeners have ever been assigned to
	 * this object.
	 *
	 * @return <code>true</code> if mouse listeners have been enabled in this object
	 * @usage if (gobj.areMouseListenersEnabled()) . . .
	 * @noshow
	 */
	protected boolean areMouseListenersEnabled() {
		return mouseListenersEnabled;
	}

	/**
	 * Returns a deep copy of this object.
	 *
	 * @return deep copy
	 */
	public Object clone() {
		try {
			GObject copy = (GObject) super.clone();
			copy.compoundParent = null;
			copy.transientParent = null;
			copy.mouseListener = null;
			copy.mouseMotionListener = null;
			copy.actionListener = null;
			if (copy.matrix != null) {
				copy.matrix = ((AffineTransform) matrix.clone());
			}
			return copy;
		} catch (Exception localException) {
			throw new ErrorException("Impossible exception");
		}
	}

	/**
	 * Translates a color to a string representation.
	 *
	 * @noshow
	 */
	public static String colorName(Color color) {
		if (color.equals(Color.BLACK)) {
			return "BLACK";
		}
		if (color.equals(Color.BLUE)) {
			return "BLUE";
		}
		if (color.equals(Color.CYAN)) {
			return "CYAN";
		}
		if (color.equals(Color.DARK_GRAY)) {
			return "DARK_GRAY";
		}
		if (color.equals(Color.GRAY)) {
			return "GRAY";
		}
		if (color.equals(Color.GREEN)) {
			return "GREEN";
		}
		if (color.equals(Color.LIGHT_GRAY)) {
			return "LIGHT_GRAY";
		}
		if (color.equals(Color.MAGENTA)) {
			return "MAGENTA";
		}
		if (color.equals(Color.ORANGE)) {
			return "ORANGE";
		}
		if (color.equals(Color.PINK)) {
			return "PINK";
		}
		if (color.equals(Color.RED)) {
			return "RED";
		}
		if (color.equals(Color.WHITE)) {
			return "WHITE";
		}
		if (color.equals(Color.YELLOW)) {
			return "YELLOW";
		}
		return colorName(color.getRGB());
	}
	
	/**
	 * Translates a color RGB int to a string representation such as "0xFF00FF" or "YELLOW".
	 *
	 * @noshow
	 */
	public static String colorName(int colorRGB) {
		if (colorRGB == Color.BLACK.getRGB()) {
			return "BLACK";
		}
		if (colorRGB == Color.BLUE.getRGB()) {
			return "BLUE";
		}
		if (colorRGB == Color.CYAN.getRGB()) {
			return "CYAN";
		}
		if (colorRGB == Color.DARK_GRAY.getRGB()) {
			return "DARK_GRAY";
		}
		if (colorRGB == Color.GRAY.getRGB()) {
			return "GRAY";
		}
		if (colorRGB == Color.GREEN.getRGB()) {
			return "GREEN";
		}
		if (colorRGB == Color.LIGHT_GRAY.getRGB()) {
			return "LIGHT_GRAY";
		}
		if (colorRGB == Color.MAGENTA.getRGB()) {
			return "MAGENTA";
		}
		if (colorRGB == Color.ORANGE.getRGB()) {
			return "ORANGE";
		}
		if (colorRGB == Color.PINK.getRGB()) {
			return "PINK";
		}
		if (colorRGB == Color.RED.getRGB()) {
			return "RED";
		}
		if (colorRGB == Color.WHITE.getRGB()) {
			return "WHITE";
		}
		if (colorRGB == Color.YELLOW.getRGB()) {
			return "YELLOW";
		}
		return "0x" + String.format("%06x", colorRGB & 0xFFFFFF).toUpperCase();
	}
	
	/**
	 * Translates a color to a string representation such as "#ff00ff" or "dark gray".
	 *
	 * @noshow
	 */
	public static String colorNameFriendly(Color color) {
		return colorNameFriendly(color.getRGB());
	}

	/**
	 * Translates a color RGB int to a string representation such as "#ff00ff" or "dark gray".
	 *
	 * @noshow
	 */
	public static String colorNameFriendly(int colorRGB) {
		return colorName(colorRGB)
				.toLowerCase()
				.replace("0x", "#")
				.replace("_", " ");
	}

	/**
	 * Checks to see whether a point is inside the object.  By default, this
	 * method simply checks to see if the point is inside the bounding box.
	 * Many subclasses will need to override this to check whether the point
	 * is contained in the shape.
	 *
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>) is inside
	 * the object, and <code>false</code> otherwise
	 * @usage if (gobj.contains(x, y)) . . .
	 */
	public boolean contains(double x, double y) {
		GRectangle rect = getBounds();
		return rect != null && rect.contains(GMath.round(x), GMath.round(y));
	}

	/**
	 * Checks to see whether a point is inside the object.
	 *
	 * @param pt The point being tested
	 * @return <code>true</code> if the point is inside the object, and <code>false</code> otherwise
	 * @usage if (gobj.contains(pt)) . . .
	 */
	public final boolean contains(GPoint pt) {
		return contains(pt.getX(), pt.getY());
	}

	/**
	 * Creates a new Graphics2D that includes any transformations done to this object,
	 * such as scaling and rotation.
	 *
	 * @param g initial graphics context
	 * @return tranformed graphics object
	 */
	protected Graphics2D createTransformedGraphics(Graphics g) {
		Graphics2D g2 = (Graphics2D) g.create();
		GraphicsUtils.setAntialiasing(g2, GObject.isAntiAliasing());
		Color objectColor = getObjectColor();
		if (objectColor != null) {
			g2.setColor(objectColor);
		}
		double myX = getX();
		double myY = getY();
		if (myX != 0.0 || myY != 0.0) {
			g2.translate(getX(), getY());
		}
		if (this.lineWidth != 1.0) {
			g2.setStroke(new BasicStroke((float) this.lineWidth));
		}
		if (this.matrix != null) {
			g2.transform(this.matrix);
		}
		return g2;
	}

	/**
	 * Triggers an action event for this graphical object.
	 *
	 * @param e The <code>ActionEvent</code> to fire
	 * @usage gobj.fireActionEvent(e);
	 */
	public void fireActionEvent(ActionEvent e) {
		if (actionListener != null) {
			actionListener.actionPerformed(e);
		}
	}

	/**
	 * Triggers an action event for this graphical object with the specified
	 * action command.
	 *
	 * @param actionCommand The action command to include in the event
	 * @usage gobj.fireActionEvent(actionCommand);
	 */
	public void fireActionEvent(String actionCommand) {
		fireActionEvent(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, actionCommand));
	}

	/**
	 * Sends the event to the appropriate listener.
	 *
	 * @param e The <code>MouseEvent</code> that triggered this response
	 * @usage gobj.fireMouseListeners(e);
	 * @noshow
	 */
	protected void fireMouseListeners(MouseEvent e) {
		switch (e.getID()) {
			case MouseEvent.MOUSE_PRESSED:
				if (mouseListener != null) {
					mouseListener.mousePressed(e);
				}
				break;
			case MouseEvent.MOUSE_RELEASED:
				if (mouseListener != null) {
					mouseListener.mouseReleased(e);
				}
				break;
			case MouseEvent.MOUSE_CLICKED:
				if (mouseListener != null) {
					mouseListener.mouseClicked(e);
				}
				break;
			case MouseEvent.MOUSE_EXITED:
				if (mouseListener != null) {
					mouseListener.mouseExited(e);
				}
				break;
			case MouseEvent.MOUSE_ENTERED:
				if (mouseListener != null) {
					mouseListener.mouseEntered(e);
				}
				break;
			case MouseEvent.MOUSE_MOVED:
				if (mouseMotionListener != null) {
					mouseMotionListener.mouseMoved(e);
				}
				break;
			case MouseEvent.MOUSE_DRAGGED:
				if (mouseMotionListener != null) {
					mouseMotionListener.mouseDragged(e);
				}
				break;
		}
	}

	/**
	 * Returns the bottom y-coordinate of the object.
	 *
	 * @return The bottom y-coordinate of the object
	 * @usage double y = gobj.getBottomY();
	 */
	public double getBottomY() {
		return getY() + getHeight();
	}

	/**
	 * Returns the bounding box of this object, which is defined to be the
	 * smallest rectangle that covers everything drawn by the figure.  The
	 * coordinates of this rectangle do not necessarily match the location
	 * returned by <a href="#getLocation()"><code>getLocation</code></a>.
	 * Given a <a href="GLabel.html"><code>GLabel</code></a> object, for
	 * example, <a href="#getLocation()"><code>getLocation</code></a>
	 * returns the coordinates of the point on the baseline at which the
	 * string begins; <code>getBounds</code>, by contrast, returns a
	 * rectangle that covers the entire window area occupied by the string.
	 *
	 * @return The bounding box for this object
	 * @usage GRectangle bounds = gobj.getBounds();
	 */
	public abstract GRectangle getBounds();

	/**
	 * Returns the central x/y-coordinates of this object.
	 *
	 * @usage GPoint pt = gobj.getCenterLocation();
	 */
	public final GPoint getCenterLocation() {
		return new GPoint(getCenterX(), getCenterY());
	}

	/**
	 * Returns the central x-coordinate of this object.
	 *
	 * @usage double centerX = gobj.getCenterX();
	 */
	public double getCenterX() {
		return getX() + getWidth() / 2;
	}

	/**
	 * Returns the central y-coordinate of this object.
	 *
	 * @usage double centerY = gobj.getCenterY();
	 */
	public double getCenterY() {
		return getY() + getHeight() / 2;
	}

	/**
	 * Returns the color used to display this object.
	 *
	 * @return The color used to display this object
	 * @usage Color color = gobj.getColor();
	 */
	public Color getColor() {
		GObject obj = this;
		while (obj.objectColor == null) {
			GContainer parent = obj.getParent();
			if (parent instanceof GObject) {
				obj = (GObject) parent;
			} else if (parent instanceof Component) {
				return ((Component) parent).getForeground();
			} else {
				return Color.BLACK;
			}
		}
		return obj.objectColor;
	}

	/**
	 * Returns the component in which this object is installed, or <code>null</code>
	 * if none exists.
	 *
	 * @return The component in which this object is installed, or <code>null</code> if none exists
	 * @usage Component comp = gobj.getComponent();
	 * @noshow
	 */
	protected Component getComponent() {
		GContainer parent = getParent();
		while (parent instanceof GObject) {
			parent = ((GObject) parent).getParent();
		}
		return (parent instanceof Component) ? (Component) parent : null;
	}

	/**
	 * Returns the height of this object, which is defined to be
	 * the height of the bounding box.
	 *
	 * @return The height of this object on the screen
	 * @usage double height = gobj.getHeight();
	 */
	public double getHeight() {
		return getBounds().getHeight();
	}

	/**
	 * Returns the thickness of lines drawn on this shape.
	 * This will be 1.0 unless changed by setLineWidth.
	 */
	public double getLineWidth() {
		return this.lineWidth;
	}

	/**
	 * Returns the location of this object as a <code>GPoint</code>.
	 *
	 * @return The location of this object as a <code>GPoint</code>
	 * @usage GPoint pt = gobj.getLocation();
	 */
	public GPoint getLocation() {
		return new GPoint(xc, yc);
	}

	/**
	 * Returns the transformation matrix for this object, including any scaling and rotation done to it.
	 *
	 * @return
	 */
	protected AffineTransform getMatrix() {
		return matrix;
	}

	/**
	 * This method returns the color set for this specific object, which may
	 * be <code>null</code>.  It differs from <code>getColor</code> in that
	 * it does not walk up the containment chain.
	 *
	 * @noshow
	 */
	protected Color getObjectColor() {
		return objectColor;
	}

	/**
	 * Returns the parent of this object, which is the canvas or compound object in
	 * which it is enclosed.
	 *
	 * @return The parent of this object
	 * @usage GContainer parent = gobj.getParent();
	 */
	public GContainer getParent() {
		return (compoundParent != null) ? compoundParent : transientParent;
	}

	/**
	 * Returns the rightmost x-coordinate of the object.
	 *
	 * @return The rightmost x-coordinate of the object
	 * @usage double x = gobj.getRightX();
	 */
	public double getRightX() {
		return getX() + getWidth();
	}

	/**
	 * Returns the size of the bounding box for this object.
	 *
	 * @return The size of this object
	 * @usage GDimension size = gobj.getSize();
	 */
	public GDimension getSize() {
		GRectangle bounds = getBounds();
		return new GDimension(bounds.getWidth(), bounds.getHeight());
	}

	/**
	 * Returns the width of this object, which is defined to be
	 * the width of the bounding box.
	 *
	 * @return The width of this object on the screen
	 * @usage double width = gobj.getWidth();
	 */
	public double getWidth() {
		return getBounds().getWidth();
	}

	/**
	 * Returns the leftmost x-coordinate of the object.
	 *
	 * @return The x-coordinate of the object
	 * @usage double x = gobj.getX();
	 */
	public double getX() {
		return xc;
	}

	/**
	 * Returns the topmost y-coordinate of the object.
	 *
	 * @return The y-coordinate of the object
	 * @usage double y = gobj.getY();
	 */
	public double getY() {
		return yc;
	}

	/**
	 * Returns true if this object touches the given other object.
	 *
	 * @param obj the other object to compare against
	 * @return true if the objects intersect, else false
	 */
	public boolean intersects(GObject obj) {
		return getBounds().intersects(obj.getBounds());
	}

	/**
	 * Checks to see whether this object is visible.
	 *
	 * @return <code>true</code> if the object is visible, otherwise <code>false</code>
	 * @usage if (gobj.isVisible()) . . .
	 */
	public boolean isVisible() {
		return isVisible;
	}

	/**
	 * Moves the object on the screen using the displacements <code>dx</code> and <code>dy</code>.
	 *
	 * @param dx The distance to move the object in the x direction (positive is rightward)
	 * @param dy The distance to move the object in the y direction (positive is downward)
	 * @usage gobj.move(dx, dy);
	 */
	public void move(double dx, double dy) {
		setLocation(xc + dx, yc + dy);
	}

	/**
	 * Moves the object using displacements given in polar coordinates.  The
	 * parameter <code>r</code> specifies the distance to move and <code>theta</code>
	 * specifies the angle in which the motion occurs.  The angle is measured in
	 * degrees increasing counterclockwise from the +x axis.
	 *
	 * @param r     The distance to move
	 * @param theta The angle in which to move, measured in degrees
	 *              increasing counterclockwise from the +x axis
	 * @usage gobj.movePolar(r, theta);
	 */
	public final void movePolar(double r, double theta) {
		double radians = theta * Math.PI / 180;
		move(r * Math.cos(radians), -r * Math.sin(radians));
	}

	/**
	 * All subclasses of <code>GObject</code> must define a <code>paint</code>
	 * method which allows the object to draw itself on the <code>Graphics</code>
	 * context passed in as the parameter <code>g</code>.
	 *
	 * @param g The graphics context into which the painting is done
	 * @usage gobj.paint(g);
	 */
	public final void paint(Graphics g) {
		if (this.isVisible) {
			Graphics2D g2 = createTransformedGraphics(g);
			paint2d(g2);
			if (g2 != g) {
				g2.dispose();
			}
		}
	}

	/**
	 * All subclasses of <code>GObject</code> must define a <code>paint2d</code>
	 * method which allows the object to draw itself on the <code>Graphics</code>
	 * context passed in as the parameter <code>g</code>.
	 *
	 * @param g The graphics context into which the painting is done
	 * @usage gobj.paint(g);
	 */
	protected abstract void paint2d(Graphics2D g);

	/**
	 * Returns a string indicating the parameters of this object.
	 *
	 * @noshow
	 */
	protected String paramString() {
		String param = "";
		if (this instanceof GResizable) {
			GRectangle r = getBounds();
			param += "bounds=(" + r.getX() + ", " + r.getY() + ", "
					+ r.getWidth() + ", " + r.getHeight() + ")";
		} else {
			GPoint pt = getLocation();
			param += "location=(" + pt.getX() + ", " + pt.getY() + ")";
		}
		if (objectColor != null) {
			param += ", color=" + colorName(objectColor);
		}
		if (this instanceof GFillable) {
			param += ", filled=" + ((GFillable) this).isFilled();
			Color fillColor = ((GFillable) this).getFillColor();
			if (fillColor != null && fillColor != objectColor) {
				param += ", fillColor=" + colorName(fillColor);
			}
		}
		return param;
	}

	/**
	 * Delays the calling thread for the specified time, which is expressed in
	 * milliseconds.  Unlike <code>Thread.sleep</code>, this method never throws an
	 * exception.
	 *
	 * @param milliseconds The sleep time in milliseconds
	 * @usage gobj.pause(milliseconds);
	 */
	public void pause(double milliseconds) {
		JTFTools.pause(milliseconds);
	}

	/**
	 * Removes an action listener from this graphical object.
	 *
	 * @param listener The listener object to remove
	 * @usage gobj.removeActionListener(listener);
	 */
	public void removeActionListener(ActionListener listener) {
		actionListener = AWTEventMulticaster.remove(actionListener, listener);
	}

	/**
	 * Removes a mouse listener from this graphical object.
	 *
	 * @param listener The listener object to remove
	 * @usage gobj.removeMouseListener(listener);
	 */
	public void removeMouseListener(MouseListener listener) {
		mouseListener = AWTEventMulticaster.remove(mouseListener, listener);
	}

	/**
	 * Removes a mouse motion listener from this graphical object.
	 *
	 * @param listener The listener object to remove
	 * @usage gobj.removeMouseMotionListener(listener);
	 */
	public void removeMouseMotionListener(MouseMotionListener listener) {
		mouseMotionListener = AWTEventMulticaster.remove(mouseMotionListener, listener);
	}

	/**
	 * Signals that the object needs to be repainted.
	 *
	 * @noshow
	 */
	protected void repaint() {
		GContainer parent = getParent();
		while (parent instanceof GObject) {
			parent = ((GObject) parent).getParent();
		}
		if (parent instanceof GCanvas) {
			((GCanvas) parent).conditionalRepaint();
		}
	}

	/**
	 * Rotates this object by the given degrees counter-clockwise.
	 *
	 * @param degrees number of degrees to rotate
	 */
	public void rotate(double degrees) {
		if (this.matrix == null) {
			this.matrix = new AffineTransform();
		}
		this.matrix.rotate(-GMath.toRadians(degrees));
		repaint();
	}

	/**
	 * Scales the size of this object by the given factors in the x and y dimensions.
	 *
	 * @param sx factor to scale in x dimension (1.0 = same size)
	 * @param sy factor to scale in y dimension (1.0 = same size)
	 */
	public void scale(double sx, double sy) {
		if (this.matrix == null) {
			this.matrix = new AffineTransform();
		}
		this.matrix.scale(sx, sy);
		repaint();
	}

	/**
	 * Scales the size of this object by the given factor in the x and y dimensions.
	 *
	 * @param sf factor to scale in x and y dimensions (1.0 = same size)
	 */
	public final void scale(double sf) {
		scale(sf, sf);
	}

	/**
	 * Moves this object one step toward the back in the <i>z</i> dimension.
	 * If it was already at the back of the stack, nothing happens.
	 *
	 * @usage gobj.sendBackward();
	 */
	public void sendBackward() {
		if (compoundParent != null) {
			compoundParent.sendBackward(this);
		} else if (transientParent instanceof GCanvas) {
			((GCanvas) transientParent).sendBackward(this);
		} else if (transientParent != null) {
			try {
				Class<?> parentClass = transientParent.getClass();
				Class<?>[] types = {acm.graphics.GObject.class};
				Object[] args = {this};
				Method fn = parentClass.getMethod("sendBackward", types);
				if (fn != null) {
					fn.invoke(transientParent, args);
				}
			} catch (Exception ex) {
				/* Empty */
			}
		}
		if (mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	/**
	 * Moves this object one step toward the front in the <i>z</i> dimension.
	 * If it was already at the front of the stack, nothing happens.
	 *
	 * @usage gobj.sendForward();
	 */
	public void sendForward() {
		if (compoundParent != null) {
			compoundParent.sendForward(this);
		} else if (transientParent instanceof GCanvas) {
			((GCanvas) transientParent).sendForward(this);
		} else if (transientParent != null) {
			try {
				Class<?> parentClass = transientParent.getClass();
				Class<?>[] types = {acm.graphics.GObject.class};
				Object[] args = {this};
				Method fn = parentClass.getMethod("sendForward", types);
				if (fn != null) {
					fn.invoke(transientParent, args);
				}
			} catch (Exception ex) {
				/* Empty */
			}
		}
		if (mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	/**
	 * Moves this object to the back of the display in the <i>z</i> dimension.  By
	 * moving it to the back, this object will appear to be behind the other graphical
	 * objects on the display and may be obscured by other objects in front.
	 *
	 * @usage gobj.sendToBack();
	 */
	public void sendToBack() {
		if (compoundParent != null) {
			compoundParent.sendToBack(this);
		} else if (transientParent instanceof GCanvas) {
			((GCanvas) transientParent).sendToBack(this);
		} else if (transientParent != null) {
			try {
				Class<?> parentClass = transientParent.getClass();
				Class<?>[] types = {acm.graphics.GObject.class};
				Object[] args = {this};
				Method fn = parentClass.getMethod("sendToBack", types);
				if (fn != null) {
					fn.invoke(transientParent, args);
				}
			} catch (Exception ex) {
				/* Empty */
			}
		}
		if (mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	/**
	 * Moves this object to the front of the display in the <i>z</i> dimension.  By
	 * moving it to the front, this object will appear to be on top of the other graphical
	 * objects on the display and may hide any objects that are further back.
	 *
	 * @usage gobj.sendToFront();
	 */
	public void sendToFront() {
		if (compoundParent != null) {
			compoundParent.sendToFront(this);
		} else if (transientParent instanceof GCanvas) {
			((GCanvas) transientParent).sendToFront(this);
		} else if (transientParent != null) {
			try {
				Class<?> parentClass = transientParent.getClass();
				Class<?>[] types = {acm.graphics.GObject.class};
				Object[] args = {this};
				Method fn = parentClass.getMethod("sendToFront", types);
				if (fn != null) {
					fn.invoke(transientParent, args);
				}
			} catch (Exception ex) {
				/* Empty */
			}
		}
		if (mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	/**
	 * Sets the bottom y-coordinate of this object to the given y value.
	 *
	 * @param bottomY The new bottom y-coordinate for the object
	 * @usage gobj.setBottomY(y);
	 */
	public void setBottomY(double bottomY) {
		setLocation(getX(), bottomY - getHeight());
	}

	/**
	 * Sets the central x/y-coordinates of this object to the given x/y values.
	 *
	 * @param centerX The new central x-coordinate for the object
	 * @param centerY The new central y-coordinate for the object
	 * @usage gobj.setCenterLocation(x, y);
	 */
	public void setCenterLocation(double centerX, double centerY) {
		setLocation(centerX - getWidth() / 2, centerY - getHeight() / 2);
	}

	/**
	 * Sets the central x/y-coordinates of this object to the given x/y values.
	 *
	 * @param pt The new central x/y-coordinates for the object
	 * @usage gobj.setCenterLocation(pt);
	 */
	public final void setCenterLocation(GPoint pt) {
		setCenterLocation(pt.getX(), pt.getY());
	}

	/**
	 * Sets the central x-coordinate of this object to the given x value.
	 *
	 * @param centerX The new central x-coordinate for the object
	 * @usage gobj.setCenterX(x);
	 */
	public void setCenterX(double centerX) {
		setLocation(centerX - getWidth() / 2, getY());
	}

	/**
	 * Sets the central y-coordinate of this object to the given y value.
	 *
	 * @param centerY The new central y-coordinate for the object
	 * @usage gobj.setCenterY(y);
	 */
	public void setCenterY(double centerY) {
		setLocation(getX(), centerY - getHeight() / 2);
	}

	/**
	 * Sets the color used to display this object.
	 *
	 * @param color The color used to display this object
	 * @usage gobj.setColor(color);
	 */
	public void setColor(Color color) {
		objectColor = color;
		repaint();
	}

	/**
	 * Sets the thickness of lines drawn on this shape to the given number of pixels.
	 *
	 * @param lineWidth line width in pixels
	 */
	public void setLineWidth(double lineWidth) {
		this.lineWidth = lineWidth;
		repaint();
	}

	/**
	 * Sets the location of this object to the point (<code>x</code>, <code>y</code>).
	 *
	 * @param x The new x-coordinate for the object
	 * @param y The new y-coordinate for the object
	 * @usage gobj.setLocation(x, y);
	 */
	public void setLocation(double x, double y) {
		xc = x;
		yc = y;
		repaint();
	}

	/**
	 * Sets the location of this object to the specified point.
	 *
	 * @param pt The new location for this object
	 * @usage gobj.setLocation(pt);
	 * @noshow
	 */
	public final void setLocation(GPoint pt) {
		setLocation(pt.getX(), pt.getY());
	}

	/**
	 * Sets the parent of this object, which should be called only by the
	 * <code>GContainer</code> in which this is installed.  The
	 * serialization behavior of the parent data depends on the parent
	 * type.  Because a <code>GCompound</code> is serializable, it
	 * needs to be maintained in a nontransient variable; other parent
	 * classes are transient, so that these parents are not recorded
	 * in the serial form.
	 *
	 * @noshow
	 */
	public void setParent(GContainer parent) {
		if (parent instanceof GCompound) {
			compoundParent = (GCompound) parent;
			transientParent = null;   // JL
		} else {
			transientParent = parent;
			compoundParent = null;   // JL
		}
	}

	/**
	 * Sets the rightmost x-coordinate of this object to the given x value.
	 *
	 * @param rightX The new rightmost x-coordinate for the object
	 * @usage gobj.setRightX(x);
	 */
	public final void setRightX(double rightX) {
		setLocation(rightX - getWidth(), getY());
	}

	/**
	 * Sets whether this object is visible.
	 *
	 * @param visible <code>true</code> to make the object visible, <code>false</code> to hide it
	 * @usage gobj.setVisible(visible);
	 */
	public void setVisible(boolean visible) {
		isVisible = visible;
		repaint();
	}

	/**
	 * Sets the leftmost x-coordinate of this object to the given x value.
	 *
	 * @param x The new x-coordinate for the object
	 * @usage gobj.setX(x);
	 */
	public final void setX(double x) {
		setLocation(x, getY());
	}

	/**
	 * Sets the topmost y-coordinate of this object to the given y value.
	 *
	 * @param y The new y-coordinate for the object
	 * @usage gobj.setY(y);
	 */
	public final void setY(double y) {
		setLocation(getX(), y);
	}

	/**
	 * Starts a <code>GraphicsProgram</code> containing this object.
	 *
	 * @usage gobj.start();
	 * @noshow
	 */
	protected void start() {
		start(null);
	}

	/**
	 * Starts a <code>GraphicsProgram</code> containing this object, passing
	 * it the specified arguments.
	 *
	 * @param args The array of arguments
	 * @usage gobj.start();
	 * @noshow
	 */
	protected void start(String[] args) {
		try {
			Class<?> programClass = acm.program.GraphicsProgram.class;
			Class<?> gObjectClass = acm.graphics.GObject.class;
			Class<?>[] types = {gObjectClass, args.getClass()};
			Object[] params = {this, args};
			Method startGraphicsProgram = programClass.getMethod("startGraphicsProgram", types);
			startGraphicsProgram.invoke(null, params);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

	/**
	 * Overrides the <code>toString</code> method in <code>Object</code> to produce
	 * more readable output.
	 *
	 * @noshow
	 */
	public String toString() {
		String name = getClass().getName();
		if (name.startsWith("acm.graphics.")) {
			name = name.substring("acm.graphics.".length());
		}
		return name + "[" + paramString() + "]";
	}

	/**
	 * Tells the parent to update its list of enabled objects.
	 *
	 * @noshow
	 */
	protected void updateEnabledList() {
		Component comp = getComponent();
		if (comp instanceof GCanvas) {
			((GCanvas) comp).updateEnabledList();
		}
	}
}
