/*
 * @(#)GPen.java   1.99.1 08/12/08
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
// Documentation fix 26-Apr-07 (ESR)
//   1. Removed inherited method description for setLocation.
//
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.io.*;
import java.util.*;

/* Class: GPen */
/**
 * The <code>GPen</code> class simulates a pen drawing on a canvas.
 * As with the other graphical objects in the
 * <a href="index.html"><code>acm.graphics</code></a>
 * package, you use this class by constructing a new <code>GPen</code>
 * object and adding that object to a <a href="GCanvas.html"><code>GCanvas</code></a>.
 * You can move the pen on the canvas by using the methods
 * <a href="#setLocation(double, double)"<code>setLocation</code></a> and
 * <a href="#move(double, double)"<code>move</code></a> and then
 * draw lines using  <a href="#drawLine(double, double)"<code>drawLine</code></a>.
 * The <code>GPen</code> object remembers the series of lines
 * and can therefore repaint the screen image when necessary.
 */
public class GPen extends GObject {

/* Constructor: GPen() */
/**
 * Creates a new <code>GPen</code> object with an empty path.
 *
 * @usage GPen gpen = new GPen();
 */
	public GPen() {
		penVisible = false;
		path = new PathList();
		animator = new Animator();
		setSpeed(1.0);
		erasePath();
	}

/* Constructor: GPen(x, y) */
/**
 * Creates a new <code>GPen</code> object with an empty path, initially
 * positioned at the point (<code>x</code>, <code>y</code>).
 *
 * @usage GPen gpen = new GPen(x, y);
 * @param x The initial x coordinate of the pen
 * @param y The initial y coordinate of the pen
 */
	public GPen(double x, double y) {
		this();
		setLocation(x, y);
	}

/* Method: erasePath() */
/**
 * Erases the entire path drawn by the pen but does not change its position.
 *
 * @usage gpen.erasePath();
 */
	public void erasePath() {
		path.clear();
		xScale = 1.0;
		yScale = 1.0;
		regionOpen = false;
		regionStarted = false;
		repaint();
	}

/* Method: setLocation(x, y) */
/**
 * Moves the pen to the point (<code>x</code>, <code>y</code>) without drawing a line.
 *
 * @usage gpen.setLocation(x, y);
 * @param x The x-coordinate of the new position
 * @param y The y-coordinate of the new position
 */
	public void setLocation(double x, double y) {
		if (regionStarted) {
			throw new ErrorException("It is illegal to move the pen while you are " +
			                         "defining a filled region.");
		}
		super.setLocation(x, y);
		animator.delay();
	}

/* Method: drawLine(dx, dy) */
/**
 * Draws a line segment with displacements <code>dx</code> and <code>dy</code>.
 * Drawing a line leaves the pen positioned at the end of the line segment, so
 * that the next call to <code>drawLine</code> will continue from where this
 * one finished.
 *
 * @usage drawLine(dx, dy);
 * @param dx The extent of the line in the x direction
 * @param dy The extent of the line in the y direction
 */
	public void drawLine(double dx, double dy) {
		double x = getX();
		double y = getY();
		if (regionStarted) {
			path.add(new DrawLineElement(dx, dy));
		} else {
			path.add(new SetLocationElement(x, y), new DrawLineElement(dx, dy));
			regionStarted = regionOpen;
		}
		super.setLocation(x + dx, y + dy);
		animator.delay();
	}

/* Method: drawPolarLine(r, theta) */
/**
 * Draws a line segment using displacements given in polar coordinates.
 * The parameter <code>r</code> specifies the length of the line segment, and
 * <code>theta</code> specifies the angle at which the line is drawn.  The
 * angle is measured in degrees increasing counterclockwise from the +x axis.
 * Thus, the call
 *
 * <p><pre><code>
 * &nbsp;    drawPolarLine(1.0, 0);
 * </code></pre>
 *
 * <p>draws a line extending rightward one inch from the current point.
 * Similarly
 *
 * <p><pre><code>
 * &nbsp;    drawPolarLine(2.0, 135);
 * </code></pre>
 *
 * <p>draws a two-inch line extending to the northwest from the current point.
 *
 * @usage drawPolarLine(r, theta);
 * @param r The length of the line segment
 * @param theta The angle at which to draw the line, measured in degrees
 *              increasing counterclockwise from the +x axis
 */
	public final void drawPolarLine(double r, double theta) {
		double radians = theta * Math.PI / 180;
		drawLine(r * Math.cos(radians), -r * Math.sin(radians));
	}

/* Method: setColor(color) */
/**
 * Sets the color of the pen.
 *
 * @usage gpen.setColor(color);
 * @param color The new color for the pen
 */
	public void setColor(Color color) {
		if (regionStarted) {
			throw new ErrorException("It is illegal to change the color while you are " +
			                         "defining a filled region.");
		}
		path.add(new SetColorElement(color));
		super.setColor(color);
	}

/* Method: setFillColor(color) */
/**
 * Sets the color used to fill a region.
 *
 * @usage gpen.setFillColor(color);
 * @param color The new color used to fill a region
 */
	public void setFillColor(Color color) {
		if (regionStarted) {
			throw new ErrorException("It is illegal to change the fill color while you are " +
			                         "defining a filled region.");
		}
		fillColor = color;
	}

/* Method: getFillColor() */
/**
 * Returns the color used to display the filled region of this object.  If
 * none has been set, <code>getFillColor</code> returns the color of the
 * object.
 *
 * @usage Color color = gpen.getFillColor();
 * @return The color used to display the filled region of this object
 */
	public Color getFillColor() {
		return (fillColor == null) ? getColor() : fillColor;
	}

/* Method: startFilledRegion() */
/**
 * Starts defining a region that will be filled with the current color.
 * Any line segments drawn between now and the matching call to
 * <code>endFilledRegion</code> will be used to create a polygon that
 * is then filled.
 *
 * @usage gpen.startFilledRegion();
 */
	public void startFilledRegion() {
		if (regionOpen) {
			throw new ErrorException("You are already filling a region.");
		}
		regionOpen = true;
		regionStarted = false;
		path.add(new StartRegionElement(fillColor));
	}

/* Method: endFilledRegion() */
/**
 * Ends the definition of a region and fills it with the current color.
 *
 * @usage gpen.endFilledRegion();
 */
	public void endFilledRegion() {
		if (!regionOpen) {
			throw new ErrorException("You need to call startFilledRegion " +
			                         "before you call endFilledRegion.");
		}
		regionOpen = false;
		regionStarted = false;
		path.add(new EndRegionElement());
		repaint();
	}

/* Method: showPen() */
/**
 * Makes the pen itself visible.  If the pen is visible, the <code>drawPen</code>
 * method will be called at the end of painting the path to show the current location.
 *
 * @usage gpen.showPen();
 */
	public void showPen() {
		penVisible = true;
		repaint();
		animator.delay();
	}

/* Method: hidePen() */
/**
 * Makes the pen itself invisible.
 *
 * @usage gpen.showPen();
 */
	public void hidePen() {
		penVisible = false;
		repaint();
		animator.delay();
	}

/* Method: isPenVisible() */
/**
 * Returns whether the pen is visible.
 *
 * @usage if (isPenVisible(visible)) . . .
 * @return <code>true</code> if the pen is visible, otherwise <code>false</code>
 */
	public boolean isPenVisible() {
		return penVisible;
	}

/* Method: setSpeed(speed) */
/**
 * Sets the speed of the pen, which must be a number between 0 (slowest)
 * and 1 (fastest).  Setting speed to a value less than one makes the pen
 * move slowly, thereby making it easy to see exactly how a figure is being drawn.
 *
 * @usage setSpeed(speed);
 * @param speed The speed of the pen (0 is slowest, 1 is fastest)
 */
	public void setSpeed(double speed) {
		animator.setSpeed(speed);
	}

/* Method: getSpeed() */
/**
 * Returns the current speed of the pen.
 *
 * @usage double speed = getSpeed();
 * @return The current speed of the pen (0 is slowest, 1 is fastest)
 */
	public double getSpeed() {
		return animator.getSpeed();
	}

/* Method: scale(sx, sy) */
/**
 * Sets the scale factors <code>sx</code> and <code>sy</code>.  Scaling a <code>GPen</code>
 * object changes the location of points as well the size of line segments.  This
 * behavior seems counterintuitive for a pen installed directly in a <code>GCanvas</code>,
 * but is quite useful when a <code>GPen</code> is embedded in a compound object.
 *
 * @usage gpen.scale(sx, sy);
 * @param sx The factor used to scale all coordinates in the x direction
 * @param sy The factor used to scale all coordinates in the y direction
 */
	public void scale(double sx, double sy) {
		xScale = sx;
		yScale = sy;
		repaint();
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		PathState state = new PathState();
		state.sx = xScale;
		state.sy = yScale;
		path.mapPaint(g, state);
		if (penVisible) drawPen(g);
	}

/* Method: getBounds() */
/**
 * Returns the bounding box for the entire figure traced by the pen.
 *
 * @usage bounds = getBounds();
 * @return A <code>GRectangle</code> representing the bounding box
 */
	public GRectangle getBounds() {
		PathState state = new PathState();
		state.sx = xScale;
		state.sy = yScale;
		return path.getBounds(state);
	}

/* Method: contains(x, y) */
/**
 * Contains is defined to be false for the <code>GPen</code> object to avoid having the
 * trace intercept mouse clicks.
 *
 * @noshow
 */
	public boolean contains(double x, double y) {
		return false;
	}

/* Method: setPenImage(image) */
/**
 * Sets the image of the pen to be the specified image.  This image is drawn with
 * its center at the pen position.
 *
 * @usage pen.setPenImage(image);
 * @param image The new image to use for the pen
 */
	public void setPenImage(Image image) {
		penImage = MediaTools.loadImage(image);
	}

/* Method: getPenImage() */
/**
 * Returns the image used to draw the pen when <code>setPenVisible</code> has been
 * called.
 */
	public Image getPenImage() {
		if (penImage == null) penImage = PenImage.getImage();
		return penImage;
	}

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

/* Inherited method: getSize() */
/**
 * @inherited GObject#GDimension getSize()
 * Returns the size of the bounding box for this object.
 */

/* Inherited method: getWidth() */
/**
 * @inherited GObject#double getWidth()
 * Returns the width of this object as a double-precision value, which
 * is defined to be the width of the bounding box.
 */

/* Inherited method: getHeight() */
/**
 * @inherited GObject#double getHeight()
 * Returns the height of this object as a double-precision value, which
 * is defined to be the height of the bounding box.
 */

/* Inherited method: move(dx, dy) */
/**
 * @inherited GObject#void move(double dx, double dy)
 * Moves the pen on the screen by the specified displacements without drawing a line.
 */

/* Inherited method: movePolar(r, theta) */
/**
 * @inherited GObject#void movePolar(double r, double theta)
 * Moves the pen using displacements given in polar coordinates without drawing
 * a line.
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

/* Inherited method: getColor() */
/**
 * @inherited GObject#Color getColor()
 * Returns the color used to display this object.
 */

/* Inherited method: setVisible(visible) */
/**
 * @inherited GObject#void setVisible(boolean visible)
 * Sets whether the trace of the pen is visible.
 */

/* Inherited method: isVisible() */
/**
 * @inherited GObject#boolean isVisible()
 * Checks to see whether the trace of the pen is visible.
 */

/* Protected method: drawPen(g) */
/**
 * This method draws a representation of the pen at the current location.
 * Subclasses can override this method to draw fancier pens.
 *
 * @usage drawPen(g);
 * @param g The graphics context in which to draw the pen
 */
	protected void drawPen(Graphics g) {
		Component comp = getComponent();
		if (comp == null) return;
		if (penImage == null) penImage = PenImage.getImage();
		int width = penImage.getWidth(comp);
		int height = penImage.getHeight(comp);
		int x = (int) Math.round(getX());
		int y = (int) Math.round(getY());
		g.drawImage(penImage, x - width / 2, y - height / 2, comp);
	}

/* Protected method: getPenBounds() */
/**
 * This method returns the bounds that the pen occupies.
 *
 * @usage Rectangle r = getPenBounds();
 */
	protected Rectangle getPenBounds() {
		Component comp = getComponent();
		if (comp == null) return new Rectangle();
		if (penImage == null) penImage = PenImage.getImage();
		int width = penImage.getWidth(comp);
		int height = penImage.getHeight(comp);
		int x = (int) Math.round(getX());
		int y = (int) Math.round(getY());
		return new Rectangle(x - width / 2, y - height / 2, width, height);
	}

/* Private instance variables */
	private Animator animator;
	private double xScale, yScale;
	private boolean regionOpen;
	private boolean regionStarted;
	private boolean penVisible;
	private PathList path;
	private Image penImage;
	private Color fillColor;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}

/* Package class: PathList */
/**
 * The <code>PathList<code> class represents a list of path elements.
 */
class PathList implements Serializable {

/* Constructor: new PathList() */
/**
 * Creates a new <code>PathList</code> with no elements.
 */
	public PathList() {
		path = new ArrayList<PathElement>();
	}

/* Method: add(element) */
/**
 * Adds the specified path element to the end of the contents list.
 */
	public synchronized void add(PathElement element) {
		path.add(element);
	}

/* Method: add(e1, e2) */
/**
 * Adds two path elements under a single synchronization lock.
 */
	public synchronized void add(PathElement e1, PathElement e2) {
		path.add(e1);
		path.add(e2);
	}

/* Method: remove(element) */
/**
 * Removes the specified element from the list.
 */
	public synchronized void remove(PathElement element) {
		path.remove(element);
	}

/* Method: clear() */
/**
 * Removes all path elements from the list.
 */
	public synchronized void clear() {
		path.clear();
	}

/* Method: getElementCount() */
/**
 * Returns the number of path elements in the list.
 */
	public int getElementCount() {
		return path.size();
	}

/* Method: getElement(index) */
/**
 * Returns the path element at the specified index.
 */
	public PathElement getElement(int index) {
		return path.get(index);
	}

/* Method: getBounds() */
/**
 * Returns the bounding rectangle for the objects in the list.
 */
	public synchronized GRectangle getBounds(PathState state) {
		GRectangle bounds = new GRectangle(-1, -1, -1, -1);
		int nElements = path.size();
		for (int i = 0; i < nElements; i++) {
			PathElement element = path.get(i);
			element.updateBounds(bounds, state);
		}
		return bounds;
	}

/* Method: mapPaint(g, state) */
/**
 * Paints all the elements of the path using the graphics context <code>g</code>.
 */
	public synchronized void mapPaint(Graphics g, PathState state) {
		int nElements = path.size();
		for (int i = 0; i < nElements; i++) {
			PathElement element = path.get(i);
			element.paint(g, state);
		}
		FINAL_PATH_ELEMENT.paint(g, state);
	}

/* Private constants */
	private static final PathElement FINAL_PATH_ELEMENT = new FinalPathElement();

/* Private instance variables */
	private ArrayList<PathElement> path;
};

/* Package class: PathState */
/**
 * The <code>PathState<code> class maintains the information necessary to
 * render the path.  It is a structure in which the elements are read directly
 * by the PathElement subclasses.
 */
class PathState implements Serializable {
	double cx, cy;		/* The current pen position, before scaling */
	double sx, sy;		/* The current scale factors, applied before rendering */
	Polygon region;		/* The current region, or null if no region is in effect */
	Color fillColor;	/* The color used to fill the region */
};

/* Package abstract class: PathElement */
/**
 * The <code>PathElement<code> class is used to elements of the path being constructed.
 * <code>PathElement</code> is the abstract class; the actual code to render and
 * compute the bounds of the path element is provided by the concrete classes.
 */
abstract class PathElement implements Serializable {

/* Constructor: PathElement() */
/**
 * The default constructor for this class.
 */
	public PathElement() {
		/* Empty */
	}

/* Method: paint(g, state); */
/**
 * Performs whatever functions are necessary when this path element is
 * encountered during a painting traversal.
 */
	public void paint(Graphics g, PathState state) {
		if (g == g) /* Avoid unused parameter warning */;
		if (state == state) /* Avoid unused parameter warning */;
	}

/* Method: updateBounds(bounds, state); */
/**
 * Updates the bounds of the rectangle as it would if this operation were
 * performed.
 */
	public void updateBounds(GRectangle bounds, PathState state) {
		if (bounds == bounds) /* Avoid unused parameter warning */;
		if (state == state) /* Avoid unused parameter warning */;
	}
}

/* Package class: SetLocationElement */
/**
 * The <code>SetLocationElement<code> class is used to represent a change in
 * position.
 */
class SetLocationElement extends PathElement {

	public SetLocationElement(double x, double y) {
		cx = x;
		cy = y;
	}

	public void paint(Graphics g, PathState state) {
		state.cx = cx;
		state.cy = cy;
		if (state.region != null) {
			state.region.addPoint(GMath.round(state.sx * cx), GMath.round(state.sy * cy));
		}
	}

	public void updateBounds(GRectangle bounds, PathState state) {
		state.cx = cx;
		state.cy = cy;
	}

	private double cx, cy;
}

/* Package class: DrawLineElement */
/**
 * The <code>DrawLineElement<code> class is used to represent a
 * line.
 */
class DrawLineElement extends PathElement {

	public DrawLineElement(double dx, double dy) {
		deltaX = dx;
		deltaY = dy;
	}

	public void paint(Graphics g, PathState state) {
		int x0 = GMath.round(state.sx * state.cx);
		int y0 = GMath.round(state.sy * state.cy);
		state.cx += deltaX;
		state.cy += deltaY;
		int x1 = GMath.round(state.sx * state.cx);
		int y1 = GMath.round(state.sy * state.cy);
		if (state.region == null) {
			g.drawLine(x0, y0, x1, y1);
		} else {
			state.region.addPoint(x1, y1);
		}
	}

	public void updateBounds(GRectangle bounds, PathState state) {
		if (bounds.getWidth() < 0) {
			bounds.setBounds(state.sx * state.cx, state.sy * state.cy, 0, 0);
		} else {
			bounds.add(state.sx * state.cx, state.sy * state.cy);
		}
		state.cx += deltaX;
		state.cy += deltaY;
		bounds.add(state.sx * state.cx, state.sy * state.cy);
	}

	private double deltaX, deltaY;
}

/* Package class: SetColorElement */
/**
 * The <code>SetColorElement<code> class is used to represent a
 * change of color.
 */
class SetColorElement extends PathElement {
	public SetColorElement(Color color) {
		myColor = color;
	}

	public void paint(Graphics g, PathState state) {
		g.setColor(myColor);
	}

	private Color myColor;
}

/* Package class: StartRegionElement */
/**
 * The <code>StartRegionElement<code> class marks the beginning of a region.
 */
class StartRegionElement extends PathElement {
	public StartRegionElement(Color color) {
		myColor = color;
	}

	public void paint(Graphics g, PathState state) {
		state.region = new Polygon();
		state.fillColor = myColor;
	}

	private Color myColor;
}

/* Package class: EndRegionElement */
/**
 * The <code>EndRegionElement<code> class marks the end of a region.
 */
class EndRegionElement extends PathElement {
	public EndRegionElement() {
		/* Empty */
	}

	public void paint(Graphics g, PathState state) {
		Color oldColor = g.getColor();
		g.setColor(state.fillColor);
		g.fillPolygon(state.region.xpoints, state.region.ypoints, state.region.npoints);
		g.setColor(oldColor);
		g.drawPolygon(state.region.xpoints, state.region.ypoints, state.region.npoints);
		state.region = null;
	}
}

/* Package class: FinalPathElement */
/**
 * The <code>FinalPathElement<code> class is invoked at the end of the path
 * and has the effect of drawing an incomplete region.
 */
class FinalPathElement extends PathElement {
	public FinalPathElement() {
		/* Empty */
	}

	public void paint(Graphics g, PathState state) {
		if (state.region != null) {
			g.drawPolyline(state.region.xpoints, state.region.ypoints, state.region.npoints);
		}
	}
}

/* Package class: PenImage */
/**
 * This class encapsulates the image used to display the pen.
 */
class PenImage {

	public static Image getImage() {
		return MediaTools.createImage(HEX_DATA);
	}

	private static final String[] HEX_DATA = {
		"4749463839614F006500F70000FFFFFF980098339999989800111111222222000054CBFFCB003298",
		"0033660033CC0033FE00323266330066660000659800989800CC9900FE99329800659800CC0099FE",
		"0098659898999999CC9900FE98009800329800659900CC9800FE3399CB3399FF9999339898659832",
		"0098650099339998659833CB9833FF9999CC0099FE00336699656698CC9898FF9999323200336600",
		"32003233006632009833339965009866339900663300983200666600986500CC3300FE3200CC6600",
		"FE65CCCC98CCFF99FFCC99FFFF993300CC3200FE6600CC6500FECC0033CC0066FE0032FE00653399",
		"33339966669933669865CC00CCCB00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966",
		"FF9933FF9865333333326532323265326565660033653232660066653265CC3300CC6600FE3200FE",
		"65000066CC0099CC0066FE0098FE00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB33",
		"98CC6699FF3399FF659866CC9965FF9898CC9899FF99CCCC00CCFE00FECB00FEFE00993333996633",
		"9933669865659833CB9966CC9933FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698",
		"FF65CC98CCCCCCCCCC99FFCBCBFFFF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533",
		"CB6666CC6633FF6565FFCB3333CB6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF33",
		"33CB6633FF6666CB3366FF3366CC6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF",
		"65FF66CCCC65FFCC65CCFF65FFFF98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC",
		"33FFFF33FFCC65FFFF65444444656532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000",
		"000098000066777777888888AAAAAABBBBBB5555556666660000100000224400005400000000CC00",
		"00DC0000EE0000FE00003200004400880000980000AA0000BA0000CC0000DC0000EE0000FE00CC00",
		"00DC0000EE0000FE0000004400005400006600007600220000320000AA0000BA0000002200003200",
		"7600008800000000AA0000BA00007600008800000021F90401000000002C000000004F006500C7FF",
		"FFFF980098339999989800111111222222000054CBFFCB0032980033660033CC0033FE0032326633",
		"0066660000659800989800CC9900FE99329800659800CC0099FE0098659898999999CC9900FE9800",
		"9800329800659900CC9800FE3399CB3399FF99993398986598320098650099339998659833CB9833",
		"FF9999CC0099FE00336699656698CC9898FF99993232003366003200323300663200983333996500",
		"9866339900663300983200666600986500CC3300FE3200CC6600FE65CCCC98CCFF99FFCC99FFFF99",
		"3300CC3200FE6600CC6500FECC0033CC0066FE0032FE0065339933339966669933669865CC00CCCB",
		"00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966FF9933FF98653333333265323232",
		"65326565660033653232660066653265CC3300CC6600FE3200FE65000066CC0099CC0066FE0098FE",
		"00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB3398CC6699FF3399FF659866CC9965",
		"FF9898CC9899FF99CCCC00CCFE00FECB00FEFE009933339966339933669865659833CB9966CC9933",
		"FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698FF65CC98CCCCCCCCCC99FFCBCBFF",
		"FF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533CB6666CC6633FF6565FFCB3333CB",
		"6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF3333CB6633FF6666CB3366FF3366CC",
		"6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF65FF66CCCC65FFCC65CCFF65FFFF",
		"98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC33FFFF33FFCC65FFFF6544444465",
		"6532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000000098000066777777888888AAAA",
		"AABBBBBB5555556666660000100000224400005400000000CC0000DC0000EE0000FE000032000044",
		"00880000980000AA0000BA0000CC0000DC0000EE0000FE00CC0000DC0000EE0000FE000000440000",
		"5400006600007600220000320000AA0000BA00000022000032007600008800000000AA0000BA0000",
		"7600008800000008FF0001081C48B0A0C18308132A5CA81012248610234A640809DBBF2DD830E431",
		"D1451B868920432684C4022306483F5AB0E882ADE5968F226386C40026E38F479030E8C45072CB0F",
		"9940252EEB226265476D0CFE29C5F630A8538818B6B4C00869D9D3AB22B7FC636115ABD7AF60C38A",
		"1D4BB6ACD9B368D3AA5DCB166B450334E23280D916E48F2D1E7F406AA11792096D2CEA4A848491C5",
		"8FBE3FF23CFA81C10B5DC10A317089CAC221DF9C5E5E428EF8A30B06315D0C30D0267573C8652CA2",
		"3E36CDBAB5EBD7B063CB9E4DBBB6EDC1D80C6CD9D2F4B6418E09BAD0B04143B3EF81183A62435CA5",
		"4B8CC0C73B6364BCF3824E9FBE519BD49B27A76516CB6F57FFC4963AE70A315EC0E0DD52597C4B9D",
		"7A5B3844597235ED962CE2D2E8D245065E6C941C0700249E61D3C50CA46504C901020A74D7671D79",
		"D15B830E7A661C850661808D18D860E8E187208628E288249668E28928A6A8E28A6C2D13156F2326",
		"275C0C4C819887180CE8C7D28402A2C6400C5BD8E0455C5D74D5E00F167551194A5EF0C7608384BD",
		"601824BBC8971C76024615C305F275F74877D878F1D37184B177521E3AB160020BE859729C760A22",
		"F6C32E4876C1236D5191E7D04D7BE6D4451E6416261F75D571E8E66DA8BDA7174F8CB2B09591B60D",
		"95D1231718888D132C8077E16D26607381097ACD199F81F6D1462084CFB17041A65B743126996068",
		"5728DC51EBDD699B8618C4B5C58F5B00D95E83B8CEFADF931882975C71A086F81E035B98606B8382",
		"8057638C196D11E088E3C148A2815892F8CFB42C862BEEB8E4966BEEB9E8A6ABEEBAECB6EBEEBBF0",
		"C62BEFBCF4CA1B100021FF0B4D414347436F6E2004031039000000015772697474656E2062792047",
		"4946436F6E76657274657220322E342E33206F66204D6F6E6461792C204D61792032352C20313939",
		"38003B"
	};

}
