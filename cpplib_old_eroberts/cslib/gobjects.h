/*
 * File: gobjects.h
 * ----------------
 * This interface exports a hierarchy of graphical shapes based on
 * the model developed for the ACM Java Graphics.
 * <include src="pictures/TypeHierarchies/GObjectHierarchy.html">
 */

#ifndef _gobjects_h
#define _gobjects_h

#include "cslib.h"
#include "generic.h"
#include "gtypes.h"
#include "vector.h"

#ifndef _gwindow_h

typedef struct GWindowCDT *GWindow;

/*
 * Type: GObject
 * -------------
 * This type represents an abstract type that unifies the set of all
 * graphics objects.  As with an abstract class in an object-oriented
 * language, it is not legal to create a <code>GObject</code> directly.
 * Graphical objects are created instead by calling the constructor for
 * one of the concrete subclasses, as follows:
 *
 *<ul>
 * <li><a href="#Function:newGRect"
 *     ><code>newGRect(x, y, width, height)</code></a>
 * <li><a href="#Function:newG3DRect"
 *     ><code>newG3DRect(x, y, width, height, raised)</code></a>
 * <li><a href="#Function:newGRoundRect"
 *     ><code>newGRoundRect(x, y, width, height, corner)</code></a>
 * <li><a href="#Function:newGOval"
 *     ><code>newGOval(x, y, width, height)</code></a>
 * <li><a href="#Function:newGLine"
 *     ><code>newGLine(x0, y0, x1, y1)</code></a>
 * <li><a href="#Function:newGArc"
 *     ><code>newGArc(x, y, width, height, start, sweep)</code></a>
 * <li><a href="#Function:newGImage"
 *     ><code>newGImage(filename, x, y)</code></a>
 * <li><a href="#Function:newGLabel"
 *     ><code>newGLabel(str, x, y)</code></a>
 * <li><a href="#Function:newGPolygon"
 *     ><code>newGPolygon</code></a>
 * <li><a href="#Function:newGCompound"
 *     ><code>newCompound</code></a>
 *</ul>
 *
 * <p>Even though C has no object-oriented hierarchies, this interface
 * defines the types <code>GArc</code>, <code>GCompound</code>,
 * <code>GImage</code>, <code>GLabel</code>, <code>GLine</code>,
 * <code>GOval</code>, <code>GPolygon</code>, <code>GRect</code>,
 * <code>GRoundRect</code>, and <code>G3DRect</code> as synonyms
 * for <code>GObject</code>.  Doing so helps to clarify what types
 * are expected by each function and simplifies the process of
 * converting code from languages that implement full hierarchies.
 */

typedef struct GObjectCDT *GObject;

#endif

/*
 * Function: freeGObject
 * Usage: freeGObject(gobj);
 * -------------------------
 * Frees the memory associated with the object.  This operation is
 * not necessary for objects that have been installed in a window.
 * Adding a <code>GObject</code> to a <code>GWindow</code> or a
 * <code>GCompound</code> transfers ownership to the container,
 * which assumes responsibility for freeing the object.
 */

void freeGObject(GObject gobj);

/*
 * Function: getX
 * Usage: x = getX(gobj);
 * ----------------------
 * Returns the <i>x</i>-coordinate of the object.
 */

double getXGObject(GObject gobj);

/*
 * Function: getY
 * Usage: y = getY(gobj);
 * ----------------------
 * Returns the <i>y</i>-coordinate of the object.
 */

double getYGObject(GObject gobj);

/*
 * Function: getLocation
 * Usage: pt = getLocation(gobj);
 * ------------------------------
 * Returns the location of this object as a <code>GPoint</code>.
 */

GPoint getLocation(GObject gobj);

/*
 * Function: setLocation
 * Usage: setLocation(gobj, x, y);
 * -------------------------------
 * Sets the location of this object to the specified coordinates.
 */

void setLocation(GObject gobj, double x, double y);

/*
 * Function: move
 * Usage: move(gobj, dx, dy);
 * --------------------------
 * Moves the object on the screen using the displacements
 * <code>dx</code> and <code>dy</code>.
 */

void move(GObject gobj, double dx, double dy);

/*
 * Function: getWidth
 * Usage: width = getWidth(gobj);
 * ------------------------------
 * Returns the width of this object, which is defined to be the width of
 * the bounding box.
 */

double getWidthGObject(GObject gobj);

/*
 * Function: getHeight
 * Usage: height = getHeight(gobj);
 * --------------------------------
 * Returns the height of this object, which is defined to be the height
 * of the bounding box.
 */

double getHeightGObject(GObject gobj);

/*
 * Function: getSize
 * Usage: size = getSize(gobj);
 * ----------------------------
 * Returns the size of the object as a <code>GDimension</code>.
 */

GDimension getSize(GObject gobj);

/*
 * Function: getBounds
 * Usage: rect = getBounds(gobj);
 * ------------------------------
 * Returns the bounding box of this object, which is defined to be the
 * smallest rectangle that covers everything drawn by the figure.  The
 * coordinates of this rectangle do not necessarily match the location
 * returned by <code>getLocation</code>.  Given a <code>GLabel</code>
 * object, for example, <code>getLocation</code> returns the coordinates
 * of the point on the baseline at which the string begins; the
 * <code>getBounds</code> method, by contrast, returns a rectangle that
 * covers the entire window area occupied by the string.
 */

GRectangle getBounds(GObject gobj);

/*
 * Function: setColor
 * Usage: setColor(gobj, color);
 * -----------------------------
 * Sets the color used to display this object.  The <code>color</code>
 * string is usually one of the predefined color names:
 *
 *    <code>BLACK</code><br>
 *    <code>BLUE</code><br>
 *    <code>CYAN</code><br>
 *    <code>DARK_GRAY</code><br>
 *    <code>GRAY</code><br>
 *    <code>GREEN</code><br>
 *    <code>LIGHT_GRAY</code><br>
 *    <code>MAGENTA</code><br>
 *    <code>ORANGE</code><br>
 *    <code>PINK</code><br>
 *    <code>RED</code><br>
 *    <code>WHITE</code><br>
 *    <code>YELLOW</code><br>
 *
 * <p>The case of the individual letters in the color name is ignored, as
 * are spaces and underscores, so that the color <code>DARK_GRAY</code>
 * can be written as <code>"Dark Gray"</code>.
 */

void setColorGObject(GObject gobj, string color);

/*
 * Function: getColor
 * Usage: color = getColor(gobj);
 * ------------------------------
 * Returns the color used to display this object.  This color is
 * always returned as a string in the form <code>"#rrggbb"</code>,
 * where <code>rr</code>, <code>gg</code>, and <code>bb</code> are
 * the red, green, and blue components of the color, expressed as
 * two-digit hexadecimal values.
 */

string getColorGObject(GObject gobj);

/*
 * Function: setVisible
 * Usage: setVisible(gobj, flag);
 * ------------------------------
 * Sets whether this object is visible.
 */

void setVisibleGObject(GObject gobj, bool flag);

/*
 * Function: isVisible
 * Usage: if (isVisible(gobj)) . . .
 * ---------------------------------
 * Returns <code>true</code> if this object is visible.
 */

bool isVisibleGObject(GObject gobj);

/*
 * Function: sendForward
 * Usage: sendForward(gobj);
 * -------------------------
 * Moves this object one step toward the front in the <i>z</i> dimension.
 * If it was already at the front of the stack, nothing happens.
 */

void sendForward(GObject gobj);

/*
 * Function: sendToFront
 * Usage: sendToFront(gobj);
 * -------------------------
 * Moves this object to the front of the display in the <i>z</i> dimension.
 * By moving it to the front, this object will appear to be on top of the
 * other graphical objects on the display and may hide any objects that
 * are further back.
 */

void sendToFront(GObject gobj);

/*
 * Function: sendBackward
 * Usage: sendBackward(gobj);
 * --------------------------
 * Moves this object one step toward the back in the <i>z</i> dimension.
 * If it was already at the back of the stack, nothing happens.
 */

void sendBackward(GObject gobj);

/*
 * Function: sendToBack
 * Usage: sendToBack(gobj);
 * ------------------------
 * Moves this object to the back of the display in the <i>z</i> dimension.
 * By moving it to the back, this object will appear to be behind the other
 * graphical objects on the display and may be obscured by other objects
 * in front.
 */

void sendToBack(GObject gobj);

/*
 * Function: contains
 * Usage: if (contains(gobj, x, y)) . . .
 * --------------------------------------
 * Returns <code>true</code> if the specified point is inside the object.
 */

bool containsGObject(GObject gobj, double x, double y);

/*
 * Function: getType
 * Usage: type = getType(gobj);
 * ----------------------------
 * Returns the subtype of the object as a string, as in <code>"GOval"</code>
 * or <code>"GRect"</code>.
 */

string getType(GObject gobj);

/*
 * Function: getParent
 * Usage: parent = getParent(gobj);
 * --------------------------------
 * Returns a pointer to the <code>GCompound</code> that contains this
 * object.  Every <code>GWindow</code> is initialized to contain a single
 * <code>GCompound</code> that is aligned with the window.  Adding
 * objects to the window adds them to that <code>GCompound</code>,
 * which means that every object you add to the window has a parent.
 * Calling <code>getParent</code> on the top-level <code>GCompound</code>
 * returns <code>NULL</code>.
 */

GObject getParent(GObject gobj);

/* Functions that apply to several types */

/*
 * Function: setSize
 * Usage: setSize(gobj, width, height);
 * ------------------------------------
 * Changes the size of this object to the specified width and height.  This
 * method applies to the types <code>GOval</code>, <code>GImage</code>,
 * and <code>GRect</code> (and its subclasses).
 */

void setSize(GObject gobj, double width, double height);

/*
 * Function: setBounds
 * Usage: setBounds(gobj, x, y, width, height);
 * --------------------------------------------
 * Changes the bounds of this object to the specified values.  This
 * method applies to the types <code>GOval</code>, <code>GImage</code>,
 * and <code>GRect</code> (and its subclasses).
 */

void setBounds(GObject gobj, double x, double y, double width, double height);

/*
 * Function: setFilled
 * Usage: setFilled(gobj, flag);
 * -----------------------------
 * Sets the fill status for <code>gobj</code>, where <code>false</code>
 * is outlined and <code>true</code> is filled.
 */

void setFilled(GObject gobj, bool flag);

/*
 * Function: isFilled
 * Usage: if (isFilled(gobj)) . . .
 * --------------------------------
 * Returns <code>true</code> if <code>gobj</code> is filled.
 */

bool isFilled(GObject gobj);

/*
 * Function: setFillColor
 * Usage: setFillColor(gobj, color);
 * ---------------------------------
 * Sets the color used to display the filled region of this rectangle.
 */

void setFillColor(GObject gobj, string color);

/*
 * Function: getFillColor
 * Usage: color = getFillColor(gobj);
 * ----------------------------------
 * Returns the color used to display the filled region of <code>gobj</code>.
 * rectangle.  If none has been set, <code>getFillColor</code> returns
 * the empty string.
 */

string getFillColor(GObject gobj);

/*
 * Type: GRect
 * -----------
 * This type represents a graphical object whose appearance consists of a
 * rectangular box.  For example, the following code adds a filled, red,
 * <nobr>200<font class=sansserif size=-1>x</font>100</nobr> rectangle
 * at the upper left corner of the graphics window:
 *
 *<pre>
 *    main() {
 *       GWindow gw = newGWindow(500, 300);
 *       printf("This program draws a red rectangle at (0, 0).\n")
 *       GRect rect = newGRect(0, 0, 200, 100);
 *       setFilled(rect, true);
 *       setColor(rect, "RED");
 *       add(gw, rect);
 *    }
 *</pre>
 */

typedef GObject GRect;

/*
 * Function: newGRect
 * Usage: rect = newGRect(x, y, width, height);
 * --------------------------------------------
 * Creates a new <code>GRect</code> with the specified bounds.  By
 * default, the rectangle is unfilled.
 */

GRect newGRect(double x, double y, double width, double height);

/*
 * Type: GRoundRect
 * ----------------
 * This type represents a rectangular box with rounded corners.
 */

typedef GRect GRoundRect;

/*
 * Function: newGRoundRect
 * Usage: GRoundRect rect = newGRoundRect(x, y, width, height, corner);
 * --------------------------------------------------------------------
 * Creates a new <code>GRoundRect</code> with the specified dimensions.
 * The <code>corner</code> parameter specifies the diameter of the arc
 * forming the corner.
 */

GRoundRect newGRoundRect(double x, double y, double width, double height,
                                             double corner);

/*
 * Type: G3DRect
 * -------------
 * This type represents a rectangular box that can appear raised or lowered.
 */

typedef GRect G3DRect;

/*
 * Function: newG3DRect
 * Usage: G3DRect rect = newG3DRect(x, y, width, height, raised);
 * --------------------------------------------------------------
 * Creates a new <code>G3DRect</code> with the specified dimensions.
 * The <code>corner</code> parameter specifies whether this rectangle
 * should appear raised.
 */

G3DRect newG3DRect(double x, double y, double width, double height,
                                       bool raised);
/*
 * Function: setRaised
 * Usage: setRaised(rect, raised);
 * -------------------------------
 * Indicates whether this object appears raised.
 */

void setRaised(G3DRect rect, bool raised);

/*
 * Function: isRaised
 * Usage: if (isRaised(rect)) ...
 * ------------------------------
 * Returns <code>true</code> if this object appears raised.
 */

bool isRaised(G3DRect rect);

/*
 * Type: GOval
 * -----------
 * This type represents an oval inscribed in a rectangular box.
 * For example, the following code displays a filled green oval
 * inscribed in the graphics window:
 *
 *<pre>
 *    main() {
 *       GWindow gw = newGWindow(500, 300);
 *       printf("This program draws a green oval filling the window.\n");
 *       GOval oval = newGOval(getWidth(gw), getHeight(gw));
 *       setFilled(oval, true);
 *       setColor(oval, "GREEN");
 *       add(gw, oval);
 *    }
 *</pre>
 */

typedef GObject GOval;

/*
 * Function: newGOval
 * Usage: oval = newGOval(x, y, width, height);
 * --------------------------------------------
 * Creates a new <code>GOval</code> with the specified bounds.  By default,
 * the oval is unfilled.
 */

GObject newGOval(double x, double y, double width, double height);

/*
 * Type: GLine
 * -----------
 * This type represents a line segment.  For example, the following code
 * adds lines that mark the diagonals of the graphics window:
 *
 *<pre>
 *    main() {
 *       GWindow gw = newGWindow(500, 300);
 *       printf("This program draws the diagonals on the window.\n");
 *       add(gw, newGLine(0, 0, getWidth(gw), getHeight(gw)));
 *       add(gw, newGLine(0, getHeight(gw), getWidth(gw), 0));
 *    }
 *</pre>
 */

typedef GObject GLine;

/*
 * Function: newGLine
 * Usage: line = newGLine(x0, y0, x1, y1);
 * ---------------------------------------
 * Creates a new graphical line connecting the points
 * (<code>x0</code>,&nbsp;<code>y0</code>) and
 * (<code>x1</code>,&nbsp;<code>y1</code>).
 */

GObject newGLine(double x0, double y0, double x1, double y1);

/*
 * Function: setStartPoint
 * Usage: setStartPoint(gline, x, y);
 * ----------------------------------
 * Sets the start point for the line to (<code>x</code>,&nbsp;<code>y</code>),
 * leaving the end point unchanged.  This method is therefore different
 * from <code>setLocation</code>, which moves both components of the
 * line segment.
 */

void setStartPoint(GLine line, double x, double y);

/*
 * Function: setEndPoint
 * Usage: setEndPoint(line, x, y);
 * -------------------------------
 * Sets the end point for the line to (<code>x</code>,&nbsp;<code>y</code>),
 * leaving the start point unchanged.
 */

void setEndPoint(GLine line, double x, double y);

/*
 * Function: getStartPoint
 * Usage: pt = getStartPoint(gobj);
 * --------------------------------
 * Returns the point at which a <code>GLine</code> or <code>GArc</code> starts.
 */

GPoint getStartPoint(GObject gobj);

/*
 * Function: getEndPoint
 * Usage: pt = getEndPoint(gobj);
 * ------------------------------
 * Returns the point at which a <code>GLine</code> or <code>GArc</code> ends.
 */

GPoint getEndPoint(GObject gobj);

/*
 * Type: GArc
 * ----------
 * This type represents an elliptical arc.  The arc is specified by the
 * following parameters:
 *
 * <p><ul>
 *   <li>The coordinates of the bounding rectangle (<code>x</code>,
 *       <code>y</code>, <code>width</code>, <code>height</code>)
 *   <li>The angle at which the arc starts (<code>start</code>)
 *   <li>The number of degrees that the arc covers (<code>sweep</code>)
 * </ul>
 *
 * <p>All angles in a <code>GArc</code> description are measured in
 * degrees moving counterclockwise from the +<i>x</i> axis.  Negative
 * values for either <code>start</code> or <code>sweep</code> indicate
 * motion in a clockwise direction.
 * <include src="pictures/GObjectDiagrams/GArcGeometry.html">
 */

typedef GObject GArc;

/*
 * Function: newGArc
 * Usage: arc = newGArc(x, y, width, height, start, sweep);
 * --------------------------------------------------------
 * Creates a new <code>GArc</code> consisting of an elliptical arc.
 * <include src="pictures/GObjectDiagrams/GArcExamples.html">
 */

GArc newGArc(double x, double y, double width, double height,
                                 double start, double sweep);

/*
 * Function: setStartAngle
 * Usage: setStartAngle(arc, start);
 * ---------------------------------
 * Sets the starting angle for this <code>GArc</code> object.
 */

void setStartAngle(GArc arc, double start);

/*
 * Function: getStartAngle
 * Usage: angle = getStartAngle(arc);
 * ----------------------------------
 * Returns the starting angle for this <code>GArc</code> object.
 */

double getStartAngle(GArc arc);

/*
 * Function: setSweepAngle
 * Usage: setSweepAngle(arc, start);
 * ---------------------------------
 * Sets the sweep angle for this <code>GArc</code> object.
 */

void setSweepAngle(GArc arc, double start);

/*
 * Function: getSweepAngle
 * Usage: angle = getSweepAngle(arc);
 * ----------------------------------
 * Returns the sweep angle for this <code>GArc</code> object.
 */

double getSweepAngle(GArc arc);

/*
 * Function: setFrameRectangle
 * Usage: setFrameRectangle(arc, x, y, width, height);
 * ---------------------------------------------------
 * Changes the boundaries of the rectangle used to frame the arc.
 */

void setFrameRectangle(GArc garc, double x, double y,
                                  double width, double height);

/*
 * Function: getFrameRectangle
 * Usage: rect = getFrameRectangle(arc);
 * -------------------------------------
 * Returns the boundaries of the rectangle used to frame the arc.
 */

GRectangle getFrameRectangle(GArc arc);

/*
 * Type: GLabel
 * ------------
 * This subtype represents a text string.  For example, the following code
 * adds a <code>GLabel</code> containing <code>"hello, world"</code> to
 * the center of the window:
 *
 *<pre>
 *    main() {
 *       GWindow gw;
 *       GLabel label;
 *       double x, y;
 *
 *       printf("This program draws the 'hello, world' message.\n");
 *       gw = newGWindow(600, 400);
 *       label = newGLabel("hello, world");
 *       setFont(label, "SansSerif-18");
 *       x = (getWidth(gw) - getWidth(label)) / 2;
 *       y = (getHeight(gw) + getFontAscent(label)) / 2;
 *       setLocation(label, x, y);
 *       add(gw, label);
 *    }
 *</pre>
 *
 * Controlling the appearance and positioning of a <code>GLabel</code>
 * depends on understanding the following terms:
 *
 * <p><ul>
 *   <li>The <b><i>baseline</i></b> is the horizontal line on which the
 *       characters rest.
 *   <li>The <b><i>origin</i></b> is the point on the baseline at which
 *       the label begins.
 *   <li>The <b><i>height</i></b> is the distance that separate two
 *       successive lines.
 *   <li>The <b><i>ascent</i></b> is the maximum distance a character
 *       in this font extends above the baseline.
 *   <li>The <b><i>descent</i></b> is the maximum distance a character
 *       in this font extends below the baseline.
 * </ul>
 * <include src="pictures/GObjectDiagrams/GLabelGeometry.html">
 */

typedef GObject GLabel;

/*
 * Function: newGLabel
 * Usage: label = newGLabel(str);
 * ------------------------------
 * Creates a <code>GLabel</code> object containing the specified string,
 * positioned with an origin of (0, 0).
 */

GLabel newGLabel(string str);

/*
 * Function: setFont
 * Usage: setFont(label, font);
 * ----------------------------
 * Changes the font used to display the <code>GLabel</code> as specified by
 * the string <code>font</code>, which has the following format:
 *
 *<pre>
 *    family-style-size
 *</pre>
 *
 * where both <code>style</code> and <code>size</code> are optional.
 * If any of these elements are missing or specified as an asterisk,
 * the existing value is retained.
 */

void setFont(GLabel label, string font);

/*
 * Function: getFont
 * Usage: font = getFont(label);
 * -----------------------------
 * Returns the current font for the <code>GLabel</code>.
 */

string getFont(GLabel label);

/*
 * Function: setLabel
 * Usage: setLabel(label, str);
 * ----------------------------
 * Changes the string stored within the <code>GLabel</code> object, so that
 * a new text string appears on the display.
 */

void setLabel(GLabel label, string str);

/*
 * Function: getLabel
 * Usage: str = getLabel(label);
 * -----------------------------
 * Returns the string displayed by this object.
 */

string getLabel(GLabel label);

/*
 * Function: getFontAscent
 * Usage: ascent = getFontAscent(label);
 * -------------------------------------
 * Returns the maximum distance strings in this font extend above
 * the baseline.
 */

double getFontAscent(GLabel label);

/*
 * Function: getFontDescent
 * Usage: descent = getFontDescent(label);
 * ---------------------------------------
 * Returns the maximum distance strings in this font descend below
 * the baseline.
 */

double getFontDescent(GLabel label);

/*
 * Type: GImage
 * ------------
 * This subtype represents an image from a file.  For example, the
 * following code adds a <code>GImage</code> containing the Stanford
 * tree at the center of the window, assuming that the image file
 * <code>StanfordTree.png</code> exists, either in the current
 * directory or an <code>images</code> subdirectory:
 *
 *<pre>
 *    main() {
 *       printf("This program draws the Stanford tree.\n");
 *       GWindow gw = newGWindow(600, 400);
 *       GImage tree = newGImage("StanfordTree.png");
 *       double x = (getWidth(gw) - getWidth(tree)) / 2;
 *       double y = (getHeight(gw) - getHeight(tree)) / 2;
 *       add(gw, tree, x, y);
 *    }
 *</pre>
 */

typedef GObject GImage;

/*
 * Function: newGImage
 * Usage: GImage image = newGImage(filename);
 * ------------------------------------------
 * Constructs a new image by loading the image from the specified
 * file, which is either in the current directory or a subdirectory named
 * <code>images</code>.  The upper left corner of the image is positioned
 * at the origin.
 */

GImage newGImage(string filename);

/*
 * Type: GPolygon
 * --------------
 * This subtype represents a polygon bounded by line segments.
 * The <code>newGPolygon</code> function creates an empty polygon.
 * To complete the figure, you need to add vertices to the polygon
 * using the functions
 * <a href="#Function:addVertex"><code>addVertex</code></a>,
 * <a href="#Function:addEdge"><code>addEdge</code></a>,
 * and <a href="#Function:addPolarEdge"><code>addPolarEdge</code></a>.
 * As an example, the following code adds a filled red octagon to
 * the center of the window:
 *
 *<pre>
 *    main() {
 *       GWindow gw;
 *       GPolygon stopSign;
 *       double edge;
 *       int i;
 *
 *       printf("This program draws a red octagon.\n");
 *       gw = newGWindow(600, 400);
 *       edge = 75;
 *       stopSign = newGPolygon();
 *       addVertex(stopSign, -edge / 2, edge / 2 + edge / sqrt(2.0));
 *       for (i = 0; i < 8; i++) {
 *          addPolarEdge(stopSign, edge, 45 * i);
 *       }
 *       setFilled(stopSign, true);
 *       setColor(stopSign, "RED");
 *       add(gw, stopSign, getWidth(gw) / 2, getHeight(gw) / 2);
 *    }
 *</pre>
 * <include src="pictures/GObjectDiagrams/StopSign.html">
 */

typedef GObject GPolygon;

/*
 * Function: newGPolygon
 * Usage: poly = newGPolygon();
 * ----------------------------
 * Constructs a new empty polygon.
 */

GPolygon newGPolygon(void);

/*
 * Function: addVertex
 * Usage: addVertex(poly, x, y);
 * -----------------------------
 * Adds a vertex at (<code>x</code>, <code>y</code>) relative to the polygon
 * origin.
 */

void addVertex(GPolygon poly, double x, double y);

/*
 * Function: addEdge
 * Usage: addEdge(poly, dx, dy);
 * -----------------------------
 * Adds an edge to the polygon whose components are given by the displacements
 * <code>dx</code> and <code>dy</code> from the last vertex.
 */

void addEdge(GPolygon poly, double dx, double dy);

/*
 * Function: addPolarEdge
 * Usage: addPolarEdge(poly, r, theta);
 * ------------------------------------
 * Adds an edge to the polygon specified in polar coordinates.  The length
 * of the edge is given by <code>r</code>, and the edge extends in
 * direction <code>theta</code>, measured in degrees counterclockwise
 * from the +x axis.
 */

void addPolarEdge(GPolygon poly, double r, double theta);

/*
 * Function: getVertices
 * Usage: vec = getVertices(poly);
 * -------------------------------
 * Returns a vector whose elements are pointers to the GPoint values that
 * represent the vertices.  This vector is shared with the internal data
 * structure of the GPolygon and must not be freed by the client.
 */

Vector getVertices(GPolygon poly);

/*
 * Type: GCompound
 * ---------------
 * This subtype consists of a collection of other graphical objects.
 * Once assembled, the internal objects can be manipulated as a unit.
 * The <code>GCompound</code> keeps track of its own position, and all
 * items within it are drawn relative to that location.
 */

typedef GObject GCompound;

/*
 * Function: newGCompound
 * Usage: comp = newGCompound();
 * -----------------------------
 * Creates a new graphical compound with no internal components.
 */

GObject newGCompound(void);

/*
 * Function: add
 * Usage: add(compound, gobj);
 * ---------------------------
 * Adds the object to the compound.
 */

void addGCompound(GCompound compound, GObject gobj);

/*
 * Function: add
 * Usage: add(compound, gobj);
 * ---------------------------
 * Removes the object from the compound.
 */

void removeGCompound(GCompound compound, GObject gobj);

/*
 * Friend function: getGObjectCompound
 * Usage: gobj = getGObjectCompound(GCompound compound, double x, double y);
 * -------------------------------------------------------------------------
 * Returns the topmost object covering x and y, or <code>NULL</code> if
 * no such object exists.
 */

GObject getGObjectCompound(GCompound compound, double x, double y);

/*
 * Friend type: ObjectTypeBits
 * ---------------------------
 * This enumeration identifies a GObject type in a way that simplifies
 * checking whether a particular function is legal on that object.
 */

typedef enum {
   GARC = 1<<0,
   GCOMPOUND = 1<<1,
   GIMAGE = 1<<2,
   GLABEL = 1<<3,
   GLINE = 1<<4,
   GOVAL = 1<<5,
   GPOLYGON = 1<<6,
   GRECT = 1<<7,
   G3DRECT = 1<<8,
   GROUNDRECT = 1<<9,
   GINTERACTOR = 0x1F<<10,
   GBUTTON = 1<<10,
   GCHECKBOX = 1<<11,
   GCHOOSER = 1<<12,
   GSLIDER = 1<<13,
   GTEXTFIELD = 1<<14
} GObjectTypeBits;

#endif
