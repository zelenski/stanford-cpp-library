<html>
<head>
<title>../include/gwindow.h</title>
<link rel="stylesheet" type="text/css" href="cppdoc.css" />
</head>
<body>
<pre class=code>
<scan class=comment>/*
 * File: gwindow.h
 * ---------------
 * This file defines the GWindow class which supports drawing graphical
 * objects on the screen.
 */
</scan>
#ifndef _gwindow_h
#define _gwindow_h

#include &lt;string&gt;
#include "gtypes.h"
#include "vector.h"

class GCompound;
class GInteractor;
class GLabel;
class GObject;

<a name="Friend type:GWindowData"><scan class=comment>/*
 * Friend type: GWindowData
 * ------------------------
 * This block contains all data pertaining to the window.  Shallow copying
 * of the GWindow object ensures that all copies refer to the same onscreen
 * window.
 */
</scan>
struct GWindowData {
   double windowWidth;
   double windowHeight;
   std::string windowTitle;
   std::string color;
   bool visible;
   GCompound *top;
};

<a name="Class:GWindow"><scan class=comment>/*
 * Class: GWindow
 * --------------
 * This class represents a graphics window that supports simple graphics. 
 * Each GWindow consists of two layers.  The background layer provides a
 * surface for drawing static pictures that involve no animation. 
 * Graphical objects drawn in the background layer are persistent and do
 * not require the client to update the contents of the window.  The
 * foreground layer contains graphical objects that are redrawn as
 * necessary.
 *
 * The GWindow class includes several methods that draw lines, rectangles,
 * and ovals on the background layer without making use of the facilities
 * of the gobjects.h interface.  For example, the following program draws a
 * diamond, rectangle, and oval at the center of the window.
 *
 *    int main() {
 *       GWindow gw;
 *       cout &lt;&lt; "This program draws a diamond, rectangle, and oval." &lt;&lt; endl;
 *       double width = gw.getWidth();
 *       double height = gw.getHeight();
 *       gw.drawLine(0, height / 2, width / 2, 0);
 *       gw.drawLine(width / 2, 0, width, height / 2);
 *       gw.drawLine(width, height / 2, width / 2, height);
 *       gw.drawLine(width / 2, height, 0, height / 2);
 *       gw.setColor("BLUE");
 *       gw.fillRect(width / 4, height / 4, width / 2, height / 2);
 *       gw.setColor("GRAY");
 *       gw.fillOval(width / 4, height / 4, width / 2, height / 2);
 *       return 0;
 *    }
 *
 * A GWindow object may be freely copied, after which all copies refer to
 * the same window.
 */
</scan>
class GWindow {

public:

<a name="Constructor:GWindow"><scan class=comment>/*
 * Constructor: GWindow
 * Usage: GWindow gw;
 *        GWindow gw(width, height);
 * ---------------------------------
 * Creates a window, either of the specified size or a default size.
 */
</scan>
   GWindow();
   GWindow(double width, double height);

<a name="Destructor:~GWindow"><scan class=comment>/*
 * Destructor: ~GWindow
 * --------------------
 * Reclaims the internal storage for the window.  Note that the window is
 * not closed by this operation, but persists until it is closed by the
 * client or the user exits the program.
 */
</scan>
   virtual ~GWindow();

<a name="Method:close"><scan class=comment>/*
 * Method: close
 * Usage: gw.close();
 * ------------------
 * Deletes the window from the screen.
 */
</scan>
   void close();

<a name="Method:requestFocus"><scan class=comment>/*
 * Method: requestFocus
 * Usage: gw.requestFocus();
 * -------------------------
 * Asks the system to assign the keyboard focus to the window, which brings
 * it to the top and ensures that key events are delivered to the window. 
 * Clicking in the window automatically requests the focus.
 */
</scan>
   void requestFocus();

<a name="Method:clear"><scan class=comment>/*
 * Method: clear
 * Usage: gw.clear();
 * ------------------
 * Clears the contents of the window.
 */
</scan>
   void clear();

<a name="Method:setVisible"><scan class=comment>/*
 * Method: setVisible
 * Usage: gw.setVisible(flag);
 * ---------------------------
 * Determines whether the window is visible on the screen.
 */
</scan>
   void setVisible(bool flag);

<a name="Method:isVisible"><scan class=comment>/*
 * Method: isVisible
 * Usage: if (gw.isVisible()) ...
 * ------------------------------
 * Tests whether the window is visible.
 */
</scan>
   bool isVisible();

<a name="Method:drawLine"><scan class=comment>/*
 * Method: drawLine
 * Usage: gw.drawLine(p0, p1);
 *        gw.drawLine(x0, y0, x1, y1);
 * -----------------------------------
 * Draws a line connecting the specified points.
 */
</scan>
   void drawLine(const GPoint &amp; p0, const GPoint &amp; p1);
   void drawLine(double x0, double y0, double x1, double y1);

<a name="Method:drawPolarLine"><scan class=comment>/*
 * Method: drawPolarLine
 * Usage: GPoint p1 = gw.drawPolarLine(p0, r, theta);
 *        GPoint p1 = gw.drawPolarLine(x0, y0, r, theta);
 * ------------------------------------------------------
 * Draws a line of length r in the direction theta from the initial point. 
 * The angle theta is measured in degrees counterclockwise from the +x
 * axis.  The method returns the end point of the line.
 */
</scan>
   GPoint drawPolarLine(const GPoint &amp; p0, double r, double theta);
   GPoint drawPolarLine(double x0, double y0, double r, double theta);

<a name="Method:drawOval"><scan class=comment>/*
 * Method: drawOval
 * Usage: gw.drawOval(bounds);
 *        gw.drawOval(x, y, width, height);
 * ----------------------------------------
 * Draws the frame of a oval with the specified bounds.
 */
</scan>
   void drawOval(const GRectangle &amp; bounds);
   void drawOval(double x, double y, double width, double height);

<a name="Method:fillOval"><scan class=comment>/*
 * Method: fillOval
 * Usage: gw.fillOval(bounds);
 *        gw.fillOval(x, y, width, height);
 * ----------------------------------------
 * Fills the frame of a oval with the specified bounds.
 */
</scan>
   void fillOval(const GRectangle &amp; bounds);
   void fillOval(double x, double y, double width, double height);

<a name="Method:drawRect"><scan class=comment>/*
 * Method: drawRect
 * Usage: gw.drawRect(bounds);
 *        gw.drawRect(x, y, width, height);
 * ----------------------------------------
 * Draws the frame of a rectangle with the specified bounds.
 */
</scan>
   void drawRect(const GRectangle &amp; bounds);
   void drawRect(double x, double y, double width, double height);

<a name="Method:fillRect"><scan class=comment>/*
 * Method: fillRect
 * Usage: gw.fillRect(bounds);
 *        gw.fillRect(x, y, width, height);
 * ----------------------------------------
 * Fills the frame of a rectangle with the specified bounds.
 */
</scan>
   void fillRect(const GRectangle &amp; bounds);
   void fillRect(double x, double y, double width, double height);

<a name="Method:setColor"><scan class=comment>/*
 * Method: setColor
 * Usage: gw.setColor(color);
 * --------------------------
 * Sets the color used for drawing.  The color parameter is usually one of
 * the predefined color names:
 *
 *    BLACK,
 *    BLUE,
 *    CYAN,
 *    DARK_GRAY,
 *    GRAY,
 *    GREEN,
 *    LIGHT_GRAY,
 *    MAGENTA,
 *    ORANGE,
 *    PINK,
 *    RED,
 *    WHITE, and
 *    YELLOW.
 *
 * The case of the individual letters in the color name is ignored, as are
 * spaces and underscores, so that the color DARK_GRAY can be written as
 * "Dark Gray".
 *
 * The color can also be specified as a string in the form "#rrggbb" where
 * rr, gg, and bb are pairs of hexadecimal digits indicating the red,
 * green, and blue components of the color.
 */
</scan>
   void setColor(std::string color);
   void setColor(int color);

<a name="Method:getColor"><scan class=comment>/*
 * Method: getColor
 * Usage: string color = gw.getColor();
 * ------------------------------------
 * Returns the current color as a string in the form "#rrggbb".  In this
 * string, the values rr, gg, and bb are two-digit hexadecimal values
 * representing the red, green, and blue components of the color,
 * respectively.
 */
</scan>
   std::string getColor();

<a name="Method:getWidth"><scan class=comment>/*
 * Method: getWidth
 * Usage: double width = gw.getWidth();
 * ------------------------------------
 * Returns the width of the graphics window in pixels.
 */
</scan>
   double getWidth();

<a name="Method:getHeight"><scan class=comment>/*
 * Method: getHeight
 * Usage: double height = gw.getHeight();
 * --------------------------------------
 * Returns the height of the graphics window in pixels.
 */
</scan>
   double getHeight();

<a name="Method:repaint"><scan class=comment>/*
 * Method: repaint
 * Usage: gw.repaint();
 * --------------------
 * Schedule a repaint on this window.
 */
</scan>
   void repaint();

<a name="Method:setWindowTitle"><scan class=comment>/*
 * Method: setWindowTitle
 * Usage: gw.setWindowTitle(title);
 * --------------------------------
 * Sets the title of the graphics window.
 */
</scan>
   void setWindowTitle(std::string title);

<a name="Method:getWindowTitle"><scan class=comment>/*
 * Method: getWindowTitle
 * Usage: string title = gw.getWindowTitle();
 * ------------------------------------------
 * Returns the title of the graphics window.
 */
</scan>
   std::string getWindowTitle();

<a name="Method:draw"><scan class=comment>/*
 * Method: draw
 * Usage: gw.draw(gobj);
 *        gw.draw(gobj, x, y);
 * ---------------------------
 * Draws the GObject on the background layer.  For convenience, the gobj
 * parameter may be passed either as a constant reference or as a pointer. 
 * If the x and y parameters are included, the object is moved to that
 * location before drawing.
 */
</scan>
   void draw(const GObject &amp; gobj);
   void draw(GObject *gobj);
   void draw(const GObject *gobj);
   void draw(GObject &amp; gobj, double x, double y);
   void draw(GObject *gobj, double x, double y);

<a name="Method:add"><scan class=comment>/*
 * Method: add
 * Usage: gw.add(gobj);
 *        gw.add(gobj, x, y);
 * --------------------------
 * Adds the GObject to the foreground layer of the window.  The second form
 * of the call sets the location of the object to (x, y) first.
 *
 * In terms of memory management, adding a GObject pointer to a GWindow
 * transfers control of that object from the client to the window manager. 
 * Deleting a GWindow automatically deletes any GObjects it contains.
 */
</scan>
   void add(GObject *gobj);
   void add(GObject *gobj, double x, double y);

<a name="Method:remove"><scan class=comment>/*
 * Method: remove
 * Usage: gw.remove(gobj);
 * -----------------------
 * Removes the object from the window.
 */
</scan>
   void remove(GObject *gobj);

<a name="Method:addToRegion"><scan class=comment>/*
 * Method: addToRegion
 * Usage: gw.addToRegion(interactor, region);
 * ------------------------------------------
 * Adds the interactor (which can also be a GLabel) to the control strip
 * specified by the region parameter.  The region parameter must be one of
 * the strings "NORTH", "EAST", "SOUTH", or "WEST".
 */
</scan>
   void addToRegion(GInteractor *gobj, std::string region);
   void addToRegion(GLabel *gobj, std::string region);

<a name="Method:removeFromRegion"><scan class=comment>/*
 * Method: removeFromRegion
 * Usage: gw.removeFromRegion(interactor, region);
 * -----------------------------------------------
 * Adds the interactor (which can also be a GLabel) to the control strip
 * specified by the region parameter.  The region parameter must be one of
 * the strings "NORTH", "EAST", "SOUTH", or "WEST".
 */
</scan>
   void removeFromRegion(GInteractor *gobj, std::string region);
   void removeFromRegion(GLabel *gobj, std::string region);

<a name="Method:getGObjectAt"><scan class=comment>/*
 * Method: getGObjectAt
 * Usage: GObject *gobj = getGObjectAt(x, y);
 * ------------------------------------------
 * Returns a pointer to the topmost GObject containing the point (x, y), or
 * NULL if no such object exists.
 */
</scan>
   GObject *getGObjectAt(double x, double y);

<a name="Method:setRegionAlignment"><scan class=comment>/*
 * Method: setRegionAlignment
 * Usage: gw.setRegionAlignment(region, align);
 * --------------------------------------------
 * Sets the alignment of the specified side region as specified by the
 * string align.  The region parameter must be one of the strings "NORTH",
 * "EAST", "SOUTH", or "WEST" and the align parameter must be "LEFT",
 * "RIGHT", or "CENTER".  By default, side panels use CENTER alignment.
 */
</scan>
   void setRegionAlignment(std::string region, std::string align);

<a name="Operator:=="><scan class=comment>/*
 * Operator: ==
 * Usage: if (w1 == w2) ...
 * ------------------------
 * Checks whether the two objects refer to the same window.
 */
</scan>
   bool operator==(GWindow w2);

<a name="Operator:!="><scan class=comment>/*
 * Operator: !=
 * Usage: if (w1 != w2) ...
 * ------------------------
 * Checks whether the two objects refer to different windows.
 */
</scan>
   bool operator!=(GWindow w2);

<scan class=comment>/* Private section */</scan>

<scan class=comment>/**********************************************************************/</scan>
<scan class=comment>/* Note: Everything below this point in the file is logically part    */</scan>
<scan class=comment>/* of the implementation and should not be of interest to clients.    */</scan>
<scan class=comment>/**********************************************************************/</scan>

   explicit GWindow(bool visible);
   GWindow(double width, double height, bool visible);
   GWindow(GWindowData *gwd);

private:

<scan class=comment>/* Instance variables */</scan>

   GWindowData *gwd;

<scan class=comment>/* Private methods */</scan>

   void initGWindow(double width, double height, bool visible);

   friend class Platform;
   friend class GKeyEvent;
   friend class GMouseEvent;
   friend class GWindowEvent;

};

<a name="Function:repaint"><scan class=comment>/*
 * Function: repaint
 * Usage: repaint();
 * -----------------
 * Issues a request to update all graphics windows.  This function is
 * called automatically when the program pauses, waits for an event, waits
 * for user input on the console, or terminates.  As a result, most clients
 * never need to call repaint explicitly.
 */
</scan>
void repaint();

<a name="Function:pause"><scan class=comment>/*
 * Function: pause
 * Usage: pause(milliseconds);
 * ---------------------------
 * Pauses for the indicated number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 */
</scan>
void pause(double milliseconds);

<a name="Function:getScreenWidth"><scan class=comment>/*
 * Function: getScreenWidth
 * Usage: width = getScreenWidth();
 * --------------------------------
 * Returns the width of the entire display screen.
 */
</scan>
double getScreenWidth();

<a name="Function:getScreenHeight"><scan class=comment>/*
 * Function: getScreenHeight
 * Usage: height = getScreenHeight();
 * ----------------------------------
 * Returns the height of the entire display screen.
 */
</scan>
double getScreenHeight();

<a name="Function:convertColorToRGB"><scan class=comment>/*
 * Function: convertColorToRGB
 * Usage: int rgb = convertColorToRGB(colorName);
 * ----------------------------------------------
 * Converts a color name into an integer that encodes the red, green, and
 * blue components of the color.
 */
</scan>
int convertColorToRGB(std::string colorName);

<a name="Function:convertRGBToColor"><scan class=comment>/*
 * Function: convertRGBToColor
 * Usage: int colorName = convertRGBToColor(rgb);
 * ----------------------------------------------
 * Converts an rgb value into a color name in the form "#rrggbb".  Each of
 * the rr, gg, and bb values are two-digit hexadecimal numbers indicating
 * the intensity of that component.
 */
</scan>
std::string convertRGBToColor(int rgb);

<a name="Function:waitForClick"><scan class=comment>/*
 * Function: waitForClick
 * Usage: waitForClick();
 * ----------------------
 * Waits for a mouse click to occur anywhere in any window.
 */
</scan>
void waitForClick();

<a name="Function:exitGraphics"><scan class=comment>/*
 * Function: exitGraphics
 * Usage: exitGraphics();
 * ----------------------
 * Closes all graphics windows and exits from the application without
 * waiting for any additional user interaction.
 */
</scan>
void exitGraphics();

#include "console.h"

#endif
</pre>
</body>
</html>
