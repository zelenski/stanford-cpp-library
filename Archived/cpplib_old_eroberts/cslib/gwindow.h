/*
 * File: gwindow.h
 * ---------------
 * This interface defines an abstract type representing a graphics window.
 */

#ifndef _gwindow_h
#define _gwindow_h

#include "cslib.h"
#include "gtypes.h"

#ifndef _gobjects_h

typedef void *GObject;

/*
 * Type: GWindow
 * -------------
 * This type represents a graphics window that supports simple graphics.
 * Each <code>GWindow</code> consists of two layers.  The background layer
 * provides a surface for drawing static pictures that involve no animation.
 * Graphical objects drawn in the background layer are persistent and do
 * not require the client to update the contents of the window.  The
 * foreground layer contains graphical objects that are redrawn as necessary.
 *
 * <p>The <code>GWindow</code> type includes several functions that draw
 * lines, rectangles, and ovals on the background layer without making
 * use of the facilities of the <code>gobjects.h</code> interface.  For
 * example, the following program draws a diamond, rectangle, and oval
 * at the center of the window.
 *
 *<pre>
 *    main() {
 *       double width, height;
 *       GWindow gw;
 *
 *       gw = newGWindow(500, 300);
 *       width = getWidth(gw);
 *       height = getHeight(gw);
 *       drawLine(gw, 0, height / 2, width / 2, 0);
 *       drawLine(gw, width / 2, 0, width, height / 2);
 *       drawLine(gw, width, height / 2, width / 2, height);
 *       drawLine(gw, width / 2, height, 0, height / 2);
 *       setColor(gw, "BLUE");
 *       fillRect(gw, width / 4, height / 4, width / 2, height / 2);
 *       setColor(gw, "GRAY");
 *       fillOval(gw, width / 4, height / 4, width / 2, height / 2);
 *    }
 *</pre>
 */

typedef struct GWindowCDT *GWindow;

#endif

/*
 * Function: newGWindow
 * Usage: gw = newGWindow(width, height);
 * --------------------------------------
 * Creates and displays a graphics window with the specified dimensions.
 */

GWindow newGWindow(double width, double height);

/*
 * Function: closeGWindow
 * Usage: closeGWindow(gw);
 * ------------------------
 * Deletes the window from the screen.
 */

void closeGWindow(GWindow gw);

/*
 * Function: requestFocus
 * Usage: requestFocus(gw);
 * ------------------------
 * Asks the system to assign the keyboard focus to the window, which
 * brings it to the top and ensures that key events are delivered to
 * the window.  Clicking in the window automatically requests the focus.
 */

void requestFocus(GWindow gw);

/*
 * Function: clear
 * Usage: clear(gw);
 * -----------------
 * Clears the contents of the window.
 */

void clearGWindow(GWindow gw);

/*
 * Function: setVisible
 * Usage: setVisible(gw, flag);
 * ----------------------------
 * Determines whether the window is visible on the screen.
 */

void setVisibleGWindow(GWindow gw, bool flag);

/*
 * Function: isVisible
 * Usage: if (isVisible(gw)) . . .
 * -------------------------------
 * Tests whether the window is visible.
 */

bool isVisibleGWindow(GWindow gw);

/*
 * Function: drawLine
 * Usage: drawLine(gw, x0, y0, x1, y1);
 * ------------------------------------
 * Draws a line connecting the specified points.
 */

void drawLine(GWindow gw, double x0, double y0, double x1, double y1);

/*
 * Function: drawPolarLine
 * Usage: pt = drawPolarLine(gw, x, y, r, theta);
 * ----------------------------------------------
 * Draws a line of length <code>r</code> in the direction <code>theta</code>
 * from the initial point.  The angle <code>theta</code> is measured in
 * degrees counterclockwise from the +<i>x</i> axis.  The method returns
 * the end point of the line.
 */

GPoint drawPolarLine(GWindow gw, double x, double y, double r, double theta);

/*
 * Function: drawOval
 * Usage: drawOval(gw, x, y, width, height);
 * -----------------------------------------
 * Draws the frame of a oval with the specified bounds.
 */

void drawOval(GWindow gw, double x, double y, double width, double height);

/*
 * Function: fillOval
 * Usage: fillOval(gw, x, y, width, height);
 * -----------------------------------------
 * Fills the frame of a oval with the specified bounds.
 */

void fillOval(GWindow gw, double x, double y, double width, double height);

/*
 * Function: drawRect
 * Usage: drawRect(gw, x, y, width, height);
 * -----------------------------------------
 * Draws the frame of a rectangle with the specified bounds.
 */

void drawRect(GWindow gw, double x, double y, double width, double height);

/*
 * Function: fillRect
 * Usage: fillRect(gw, x, y, width, height);
 * -----------------------------------------
 * Fills the frame of a rectangle with the specified bounds.
 */

void fillRect(GWindow gw, double x, double y, double width, double height);

/*
 * Function: setColor
 * Usage: setColor(gw, color);
 * ---------------------------
 * Sets the color used for drawing.  The <code>color</code> parameter is
 * usually one of the predefined color names from Java: <code>BLACK</code>,
 * <code>BLUE</code>, <code>CYAN</code>, <code>DARK_GRAY</code>,
 * <code>GRAY</code>, <code>GREEN</code>, <code>LIGHT_GRAY</code>,
 * <code>MAGENTA</code>, <code>ORANGE</code>, <code>PINK</code>,
 * <code>RED</code>, <code>WHITE</code>, or <code>YELLOW</code>.
 * The case of the individual letters in the color name is ignored,
 * as are spaces and underscores, so that the Java color
 * <code>DARK_GRAY</code> could be written as <code>"Dark&nbsp;Gray"</code>.
 */

void setColorGWindow(GWindow gw, string color);

/*
 * Function: getColor
 * Usage: color = getColor(gw);
 * ----------------------------
 * Returns the current color as a string in the form <code>"#rrggbb"</code>.
 * In this string, the values <code>rr</code>, <code>gg</code>,
 * and <code>bb</code> are two-digit hexadecimal values representing
 * the red, green, and blue components of the color, respectively.
 */

string getColorGWindow(GWindow gw);

/*
 * Function: getWidth
 * Usage: width = getWidth(gw);
 * ----------------------------
 * Returns the width of the graphics window in pixels.
 */

double getWidthGWindow(GWindow gw);

/*
 * Function: getHeight
 * Usage: height = getHeight(gw);
 * ------------------------------
 * Returns the height of the graphics window in pixels.
 */

double getHeightGWindow(GWindow gw);

/*
 * Function: repaint
 * Usage: repaint(gw);
 * -------------------
 * Schedule a repaint on the graphics window.
 */

void repaint(GWindow gw);

/*
 * Function: setWindowTitle
 * Usage: setWindowTitle(gw, title);
 * ---------------------------------
 * Sets the title of the graphics window.
 */

void setWindowTitle(GWindow gw, string title);

/*
 * Function: getWindowTitle
 * Usage: title = getWindowTitle(gw);
 * ----------------------------------
 * Returns the title of the graphics window.
 */

string getWindowTitle(GWindow gw);

/*
 * Function: draw
 * Usage: draw(gw, gobj);
 * ----------------------
 * Draws the <code>GObject</code> on the background layer.
 */

void draw(GWindow gw, GObject gobj);

/*
 * Function: drawAt
 * Usage: drawAt(gw, gobj, x, y);
 * ------------------------------
 * Moves the <code>GObject</code> to (<code>x</code>, <code>y</code>)
 * and then draws it on the window.
 */

void drawAt(GWindow gw, GObject gobj, double x, double y);

/*
 * Function: add
 * Usage: add(gw, gobj);
 * ---------------------
 * Adds the <code>GObject</code> to the foreground layer of the window.
 * Adding a <code>GObject</code> to a <code>GWindow</code> transfers
 * control of the memory for that object from the client to the graphics
 * package.  Freeing a <code>GWindow</code> automatically frees
 * any <nobr><code>GObject</code><font size=-1>s</font></nobr> it contains.
 */

void addGWindow(GWindow gw, GObject gobj);

/*
 * Function: addAt
 * Usage: addAt(gw, gobj, x, y);
 * -----------------------------
 * Adds the <code>GObject</code> to the foreground layer of the window
 * after moving it to the point (<code>x</code>, <code>y</code>).
 */

void addAt(GWindow gw, GObject gobj, double x, double y);

/*
 * Function: addToRegion
 * Usage: addToRegion(gw, gobj, region);
 * -------------------------------------
 * Adds the <code>GObject</code> (which must be an interactor or a label)
 * to the control strip specified by <code>region</code>.
 * The <code>region</code> parameter must be one of the strings
 * <code>"NORTH"</code>, <code>"EAST"</code>, <code>"SOUTH"</code>,
 * or <code>"WEST"</code>.
 */

void addToRegion(GWindow gw, GObject gobj, string region);

/*
 * Function: remove
 * Usage: remove(gw, gobj);
 * ------------------------
 * Removes the object from its container or region.
 */

void removeGWindow(GWindow gw, GObject gobj);

/*
 * Function: getGObjectAt
 * Usage: gobj = getGObjectAt(gw, x, y);
 * -------------------------------------
 * Returns a pointer to the topmost <code>GObject</code> containing the
 * point (<code>x</code>, <code>y</code>), or <code>NULL</code> if no such
 * object exists.
 */

GObject getGObjectAt(GWindow gw, double x, double y);

/*
 * Function: setRegionAlignment
 * Usage: setRegionAlignment(gw, region, align);
 * ---------------------------------------------
 * Sets the alignment of the specified side region as specified by the
 * string <code>align</code>.  The <code>region</code> parameter must be
 * one of the strings <code>"NORTH"</code>, <code>"EAST"</code>,
 * <code>"SOUTH"</code>, or <code>"WEST"</code> and the <code>align</code>
 * parameter must be <code>"LEFT"</code>, <code>"RIGHT"</code>, or
 * <code>"CENTER"</code>.  By default, side panels use
 * <code>CENTER</code> alignment.
 */

void setRegionAlignment(GWindow gw, string region, string align);

/*
 * Function: pause
 * Usage: pause(milliseconds);
 * ---------------------------
 * Pauses for the indicated number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 */

void pause(double milliseconds);

#endif
