/*
 * File: gwindow.h
 * ---------------
 * This file defines the <code>GWindow</code> class which supports
 * drawing graphical objects on the screen.
 * 
 * @version 2016/11/24
 * - added setCloseOperation
 * @version 2016/11/02
 * - added drawString, setFont methods
 * @version 2016/10/23
 * - added Region, Alignment enums and overloads of region-based methods
 * @version 2016/10/16
 * - added get/setPixel[s], get/setWidth/Height
 * - alphabetized methods
 * @version 2016/10/12
 * - added getRegionWidth, getRegionHeight methods
 * @version 2016/10/08
 * - added toBack/Front
 * @version 2016/10/07
 * - added getCanvasWidth, getCanvasHeight methods
 * @version 2016/08/02
 * - added saveCanvasPixels method
 * @version 2014/11/20
 * - added clearCanvas method
 * @version 2014/11/18
 * - added setResizable method
 * @version 2014/10/13
 * - added gwindowSetExitGraphicsEnabled function for autograders
 * - removed 'using namespace' statement
 */

#ifndef _gwindow_h
#define _gwindow_h

#include <string>
#include "grid.h"
#include "gtypes.h"
#include "point.h"
#include "vector.h"

class GCompound;
class GInteractor;
class GLabel;
class GObject;
class GMouseEvent;

namespace stanfordcpplib {
class Platform;
}

/*
 * Friend type: GWindowData
 * ------------------------
 * This block contains all data pertaining to the window.  Shallow copying
 * of the <code>GWindow</code> object ensures that all copies refer to the
 * same onscreen window.
 */
struct GWindowData {
    double windowWidth;
    double windowHeight;
    int windowX;
    int windowY;
    std::string windowTitle;
    std::string color;
    std::string font;
    int colorInt;
    bool visible;
    bool resizable;
    bool closed;
    bool exitOnClose;
    bool repaintImmediately;
    GCompound* top;
};

/*
 * Class: GWindow
 * --------------
 * This class represents a graphics window that supports simple graphics.
 * Each <code>GWindow</code> consists of two layers.  The background layer
 * provides a surface for drawing static pictures that involve no animation.
 * Graphical objects drawn in the background layer are persistent and do
 * not require the client to update the contents of the window.  The
 * foreground layer contains graphical objects that are redrawn as necessary.
 *
 * <p>The <code>GWindow</code> class includes several methods that draw
 * lines, rectangles, and ovals on the background layer without making
 * use of the facilities of the <code>gobjects.h</code> interface.  For
 * example, the following program draws a diamond, rectangle, and oval
 * at the center of the window.
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws a diamond, rectangle, and oval." << endl;
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
 *</pre>
 *
 * A <code>GWindow</code> object may be freely copied, after which all
 * copies refer to the same window.
 */
class GWindow {
public:
    enum Alignment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };
    enum Region { REGION_CENTER, REGION_EAST, REGION_NORTH, REGION_SOUTH, REGION_WEST };
    enum CloseOperation { CLOSE_DO_NOTHING = 0, CLOSE_HIDE = 1, CLOSE_DISPOSE = 2, CLOSE_EXIT = 3 };

    static const int DEFAULT_WIDTH = 500;
    static const int DEFAULT_HEIGHT = 300;
    static const int CENTER_MAGIC_VALUE = 999999;

    /*
     * Constructor: GWindow
     * Usage: GWindow gw;
     *        GWindow gw(width, height);
     * ---------------------------------
     * Creates a window, either of the specified size or a default size.
     * You can optionally pass a bool value of false to make the window
     * initially invisible; you must then call setVisible(true) on it
     * for it to appear.
     */
    GWindow();
    GWindow(double width, double height);
    GWindow(double width, double height, bool visible);

    /*
     * Destructor: ~GWindow
     * --------------------
     * Reclaims the internal storage for the window.  Note that the window
     * is not closed by this operation, but persists until it is closed by
     * the client or the user exits the program.
     */
    virtual ~GWindow();

    /*
     * Method: add
     * Usage: gw.add(gobj);
     *        gw.add(gobj, x, y);
     * --------------------------
     * Adds the <code>GObject</code> to the foreground layer of the window.
     * The second form of the call sets the location of the object to
     * (<code>x</code>, <code>y</code>) first.
     *
     * <p>In terms of memory management, adding a <code>GObject</code> pointer to
     * a <code>GWindow</code> transfers control of that object from the client to
     * the window manager.  Deleting a <code>GWindow</code> automatically deletes
     * any <nobr><code>GObject</code><font size=-1>s</font></nobr> it contains.
     */
    void add(GObject* gobj);
    void add(GObject* gobj, double x, double y);

    /*
     * Method: addToRegion
     * Usage: gw.addToRegion(interactor, region);
     * ------------------------------------------
     * Adds the interactor (which can also be a <code>GLabel</code>) to
     * the control strip specified by the <code>region</code> parameter.
     * The <code>region</code> parameter must be one of the enum
     * constants from the Region enum, or one of the region strings
     * <code>"NORTH"</code>, <code>"EAST"</code>, <code>"SOUTH"</code>,
     * or <code>"WEST"</code>.
     */
    void addToRegion(GInteractor* gobj, Region region);
    void addToRegion(GInteractor* gobj, const std::string& region);
    void addToRegion(GLabel* gobj, Region region);
    void addToRegion(GLabel* gobj, const std::string& region);

    /*
     * Sets the (x, y) location of the window to be the center of the screen.
     */
    void center();

    /*
     * Method: clear
     * Usage: gw.clear();
     * ------------------
     * Clears the contents of the window.
     */
    void clear();

    /*
     * Method: clearCanvas
     * Usage: gw.clearCanvas();
     * ------------------
     * Clears the contents of the window's graphical canvas without
     * removing any interactors in the window.
     */
    void clearCanvas();

    /*
     * Method: close
     * Usage: gw.close();
     * ------------------
     * Deletes the window from the screen.
     */
    void close();

    /*
     * Method: compareToImage
     * Usage: gw.compareToImage(filename);
     * -----------------------------------
     * Performs an image 'diff' between the pixels of this window and those
     * of the image found in the given file.
     */
    void compareToImage(const std::string& filename, bool ignoreWindowSize = true) const;

    /*
     * Method: draw
     * Usage: gw.draw(gobj);
     *        gw.draw(gobj, x, y);
     * ---------------------------
     * Draws the <code>GObject</code> on the background layer.  For convenience,
     * the <code>gobj</code> parameter may be passed either as a constant
     * reference or as a pointer.  If the <code>x</code> and <code>y</code>
     * parameters are included, the object is moved to that location before
     * drawing.
     */
    void draw(const GObject& gobj);
    void draw(GObject* gobj);
    void draw(const GObject* gobj);
    void draw(GObject& gobj, double x, double y);
    void draw(GObject* gobj, double x, double y);

    /*
     * Method: drawLine
     * Usage: gw.drawLine(p0, p1);
     *        gw.drawLine(x0, y0, x1, y1);
     * -----------------------------------
     * Draws a line connecting the specified points.
     */
    void drawLine(const GPoint & p0, const GPoint & p1);
    void drawLine(double x0, double y0, double x1, double y1);

    /*
     * Method: drawOval
     * Usage: gw.drawOval(bounds);
     *        gw.drawOval(x, y, width, height);
     * ----------------------------------------
     * Draws the frame of a oval with the specified bounds.
     */
    void drawOval(const GRectangle & bounds);
    void drawOval(double x, double y, double width, double height);

    /*
     * Method: drawPolarLine
     * Usage: GPoint p1 = gw.drawPolarLine(p0, r, theta);
     *        GPoint p1 = gw.drawPolarLine(x0, y0, r, theta);
     * ------------------------------------------------------
     * Draws a line of length <code>r</code> in the direction <code>theta</code>
     * from the initial point.  The angle <code>theta</code> is measured in
     * degrees counterclockwise from the +<i>x</i> axis.  The method returns
     * the end point of the line.
     */
    GPoint drawPolarLine(const GPoint & p0, double r, double theta);
    GPoint drawPolarLine(double x0, double y0, double r, double theta);

    /*
     * Method: drawPixel
     * Usage: gw.drawPixel(x, y);
     * --------------------------
     * Sets the color of the given pixel to be the GWindow's current color.
     * If no color is passed, uses the color last passed to setColor.
     */
    void drawPixel(double x, double y);
    void drawPixel(double x, double y, int color);
    void drawPixel(double x, double y, const std::string& color);

    /*
     * Method: drawRect
     * Usage: gw.drawRect(bounds);
     *        gw.drawRect(x, y, width, height);
     * ----------------------------------------
     * Draws the frame of a rectangle with the specified bounds.
     */
    void drawRect(const GRectangle & bounds);
    void drawRect(double x, double y, double width, double height);

    /*
     * Method: drawString
     * Usage: gw.drawString(text, x, y);
     * ---------------------------------
     * Draws the given text on the window with the baseline of its first
     * character at the given x/y position.
     * An optional font parameter can be passed to draw the string in that font.
     */
    void drawString(const std::string& text, double x, double y);

    /*
     * Method: fillOval
     * Usage: gw.fillOval(bounds);
     *        gw.fillOval(x, y, width, height);
     * ----------------------------------------
     * Fills the frame of a oval with the specified bounds.
     */
    void fillOval(const GRectangle & bounds);
    void fillOval(double x, double y, double width, double height);

    /*
     * Method: fillRect
     * Usage: gw.fillRect(bounds);
     *        gw.fillRect(x, y, width, height);
     * ----------------------------------------
     * Fills the frame of a rectangle with the specified bounds.
     */
    void fillRect(const GRectangle & bounds);
    void fillRect(double x, double y, double width, double height);

    /*
     * Method: getCanvasHeight
     * Usage: double height = gw.getCanvasHeight();
     * --------------------------------------------
     * Returns the height of the graphics window's canvas area in pixels.
     */
    double getCanvasHeight() const;

    /*
     * Method: getCanvasSize
     * Usage: GDimension canvasSize = gw.getCanvasSize();
     * --------------------------------------
     * Returns the width and height of the graphics window's central
     * drawing canvas in pixels as a GDimension.
     */
    GDimension getCanvasSize() const;

    /*
     * Method: getCanvasWidth
     * Usage: double width = gw.getCanvasWidth();
     * ------------------------------------------
     * Returns the width of the graphics window's canvas area in pixels.
     */
    double getCanvasWidth() const;

    /*
     * Method: getColor
     * Usage: string color = gw.getColor();
     * ------------------------------------
     * Returns the current color as a string in the form <code>"#rrggbb"</code>.
     * In this string, the values <code>rr</code>, <code>gg</code>,
     * and <code>bb</code> are two-digit hexadecimal values representing
     * the red, green, and blue components of the color, respectively.
     */
    std::string getColor() const;
    int getColorInt() const;

    /*
     * Method: getGObjectAt
     * Usage: GObject *gobj = getGObjectAt(x, y);
     * ------------------------------------------
     * Returns a pointer to the topmost <code>GObject</code> containing the
     * point (<code>x</code>, <code>y</code>), or <code>nullptr</code> if no such
     * object exists.
     */
    GObject* getGObjectAt(double x, double y) const;

    /*
     * Method: getHeight
     * Usage: double height = gw.getHeight();
     * --------------------------------------
     * Returns the height of the graphics window in pixels.
     */
    double getHeight() const;

    /*
     * Method: getLocation
     * Usage: Point p = gw.getLocation();
     * ----------------------------------
     * Returns the (x, y) location of the graphics window relative to
     * the top/left corner of the screen.
     */
    Point getLocation() const;

    /*
     * Returns the pixel value at the given (x, y) position as an RGB integer.
     */
    int getPixel(double x, double y) const;

    /*
     * Returns the pixel value at the given (x, y) position as an ARGB integer
     * with the alpha transparency component intact.
     */
    int getPixelARGB(double x, double y) const;

    /*
     * Returns the pixel values at all (x, y) positions in the canvas as
     * a grid of RGB integers.
     */
    Grid<int> getPixels() const;

    /*
     * Returns the pixel values at all (x, y) positions in the canvas as
     * a grid of ARGB integers with the alpha transparency component intact.
     */
    Grid<int> getPixelsARGB() const;

    /*
     * Returns the height, size, or width of the given region respectively,
     * specified as "NORTH", "SOUTH", "WEST", "EAST", or "CENTER".
     */
    double getRegionHeight(Region region) const;
    double getRegionHeight(const std::string& region) const;
    GDimension getRegionSize(Region region) const;
    GDimension getRegionSize(const std::string& region) const;
    double getRegionWidth(Region region) const;
    double getRegionWidth(const std::string& region) const;

    /*
     * Method: getSize
     * Usage: GDimension size = gw.getSize();
     * --------------------------------------
     * Returns the width and height of the graphics window in pixels
     * as a GDimension.
     */
    GDimension getSize() const;

    /*
     * Method: getTitle
     * Usage: string title = gw.getTitle();
     * ------------------------------------
     * Returns the title of the graphics window.
     * Equivalent to getWindowTitle.
     */
    std::string getTitle() const;

    /*
     * Method: getWidth
     * Usage: double width = gw.getWidth();
     * ------------------------------------
     * Returns the width of the graphics window in pixels.
     */
    double getWidth() const;

    std::string getWindowData() const;   // not to be called by students

    /*
     * Method: getWindowTitle
     * Usage: string title = gw.getWindowTitle();
     * ------------------------------------------
     * Returns the title of the graphics window.
     */
    std::string getWindowTitle() const;

    /*
     * Method: getX
     * Usage: double x = gw.getX();
     * ----------------------------
     * Returns the x location of the graphics window relative to
     * the top/left corner of the screen.
     */
    double getX() const;

    /*
     * Method: getY
     * Usage: double y = gw.getY();
     * ----------------------------
     * Returns the y location of the graphics window relative to
     * the top/left corner of the screen.
     */
    double getY() const;

    /*
     * Method: inBounds
     * Usage: if (gw.inBounds(x, y)) ...
     * ---------------------------------
     * Returns whether the given (x, y) pixel coordinate is within the bounds
     * of this window.
     * Note that this includes the entire window, not just the canvas area.
     */
    bool inBounds(double x, double y) const;

    /*
     * Method: inCanvasBounds
     * Usage: if (gw.inCanvasBounds(x, y)) ...
     * ---------------------------------
     * Returns whether the given (x, y) pixel coordinate is within the bounds
     * of this window's central canvas area.
     */
    bool inCanvasBounds(double x, double y) const;

    /*
     * Method: isOpen
     * Usage: bool open = gw.isOpen();
     * -------------------------------
     * Returns true if the window is open (has not been closed).
     */
    bool isOpen() const;

    /*
     * Returns whether the GWindow will repaint after every draw operation.
     * Initially true unless changed by a call to setRepaintImmediately.
     */
    bool isRepaintImmediately() const;

    /*
     * Method: isResizable
     * Usage: if (gw.isResizable()) ...
     * --------------------------------
     * Returns whether the window can be resized by the user (default false).
     */
    bool isResizable() const;

    /*
     * Method: isVisible
     * Usage: if (gw.isVisible()) ...
     * ------------------------------
     * Tests whether the window is visible.
     */
    bool isVisible() const;

    void notifyOfClose();           // not to be called by students

    /*
     * Resizes the window to exactly the right size to fit the components
     * inside it.
     */
    void pack();

    /*
     * Method: remove
     * Usage: gw.remove(gobj);
     * -----------------------
     * Removes the object from the window.
     */
    void remove(GObject* gobj);

    /*
     * Method: removeFromRegion
     * Usage: gw.removeFromRegion(interactor, region);
     * -----------------------------------------------
     * Removes the interactor (which can also be a <code>GLabel</code>) from
     * the control strip specified by the <code>region</code> parameter.
     * The <code>region</code> parameter must be one of the strings
     * <code>"NORTH"</code>, <code>"EAST"</code>, <code>"SOUTH"</code>,
     * or <code>"WEST"</code>.
     */
    void removeFromRegion(GInteractor* gobj, Region region);
    void removeFromRegion(GInteractor* gobj, const std::string& region);
    void removeFromRegion(GLabel* gobj, Region region);
    void removeFromRegion(GLabel* gobj, const std::string& region);

    /*
     * Method: repaint
     * Usage: gw.repaint();
     * --------------------
     * Schedule a repaint on this window.
     */
    void repaint();

    /*
     * Method: requestFocus
     * Usage: gw.requestFocus();
     * -------------------------
     * Asks the system to assign the keyboard focus to the window, which
     * brings it to the top and ensures that key events are delivered to
     * the window.  Clicking in the window automatically requests the focus.
     */
    void requestFocus();

    /*
     * Writes the pixels of this window's graphical canvas to the given file.
     */
    void saveCanvasPixels(const std::string& filename);

    /*
     * Method: setCanvasHeight
     * Usage: gw.setCanvasHeight(height);
     * ----------------------------------
     * Sets the height of the graphics window's central drawing canvas in pixels
     * without modifying its width.
     */
    void setCanvasHeight(double height);

    /*
     * Method: setCanvasSize
     * Usage: gw.setCanvasSize(width, height);
     * --------------------------------
     * Sets the size of the graphics window's central drawing canvas in pixels.
     */
    void setCanvasSize(double width, double height);

    /*
     * Method: setCanvasWidth
     * Usage: gw.setCanvasWidth(width);
     * --------------------------------
     * Sets the width of the graphics window's central drawing canvas in pixels
     * without modifying its height.
     */
    void setCanvasWidth(double width);

    /*
     * Sets what this window should do when it is closed.
     * Default is to dispose of its Java back-end resources.
     */
    void setCloseOperation(CloseOperation op);

    /*
     * Method: setColor
     * Usage: gw.setColor(color);
     * --------------------------
     * Sets the color used for drawing.  The <code>color</code> parameter is
     * usually one of the predefined color names:
     *
     *    <code>BLACK</code>,
     *    <code>BLUE</code>,
     *    <code>CYAN</code>,
     *    <code>DARK_GRAY</code>,
     *    <code>GRAY</code>,
     *    <code>GREEN</code>,
     *    <code>LIGHT_GRAY</code>,
     *    <code>MAGENTA</code>,
     *    <code>ORANGE</code>,
     *    <code>PINK</code>,
     *    <code>RED</code>,
     *    <code>WHITE</code>, and
     *    <code>YELLOW</code>.
     *
     * The case of the individual letters in the color name is ignored, as
     * are spaces and underscores, so that the color <code>DARK_GRAY</code>
     * can be written as <code>"Dark Gray"</code>.
     *
     * <p>The color can also be specified as a string in the form
     * <code>"#rrggbb"</code> where <code>rr</code>, <code>gg</code>, and
     * <code>bb</code> are pairs of hexadecimal digits indicating the
     * red, green, and blue components of the color.
     */
    void setColor(int color);
    void setColor(const std::string& color);

    /*
     * Sets this window such that when it is closed, the program will shut down
     * immediately.  Useful if the program has a primary GUI window.
     */
    void setExitOnClose(bool value = true);

    /*
     * Method: setFont
     * Usage: gw.setFont(font);
     * ------------------------
     * Sets the font used by the graphics window in calls such as drawString.
     */
    void setFont(const std::string& font);

    /*
     * Method: setHeight
     * Usage: gw.setHeight(height);
     * ----------------------------
     * Sets the height of the graphics window in pixels without changing its width.
     */
    void setHeight(double height);

    /*
     * Method: setLocation
     * Usage: gw.setLocation(x, y);
     * ----------------------------
     * Sets the (x, y) location of the graphics window relative to the top/left
     * of the screen.
     */
    void setLocation(double x, double y);
    void setLocation(const Point& p);

    /*
     * Method: setLocationSaved
     * Usage: gw.setLocationSaved(true);
     * --------------------------------
     * Causes the (x, y) location of the window to be saved and reloaded
     * between runs of the program.
     */
    void setLocationSaved(bool value);

    /*
     * Sets the pixel value at the given (x, y) position from an RGB integer.
     */
    void setPixel(double x, double y, int rgb);

    /*
     * Sets the pixel value at the given (x, y) position from an ARGB integer
     * with the alpha transparency component intact.
     */
    void setPixelARGB(double x, double y, int argb);

    /*
     * Sets the pixel value at all (x, y) positions in the canvas from
     * the given grid of RGB integers.
     */
    void setPixels(const Grid<int>& pixels);

    /*
     * Sets the pixel value at all (x, y) positions in the canvas from
     * the given grid of ARGB integers.
     */
    void setPixelsARGB(const Grid<int>& pixelsARGB);


    /*
     * Method: setRegionAlignment
     * Usage: gw.setRegionAlignment(region, align);
     * --------------------------------------------
     * Sets the alignment of the specified side region as specified by the
     * string <code>align</code>.  The <code>region</code> parameter must be
     * one of the strings <code>"NORTH"</code>, <code>"EAST"</code>,
     * <code>"SOUTH"</code>, or <code>"WEST"</code> and the <code>align</code>
     * parameter must be <code>"LEFT"</code>, <code>"RIGHT"</code>, or
     * <code>"CENTER"</code>.  By default, side panels use
     * <code>CENTER</code> alignment.
     */
    void setRegionAlignment(Region region, Alignment align);
    void setRegionAlignment(const std::string& region, const std::string& align);

    /*
     * Sets whether the GWindow should repaint after every draw operation.
     * Initially true.
     * If set to false, you must manually repaint when you want the GWindow
     * to update itself.  Useful for optimizing complex/animated GUIs.
     */
    void setRepaintImmediately(bool value);

    /*
     * Method: setResizable
     * Usage: gw.setResizable(true);
     * -----------------------------
     * Sets whether the window can be resized by the user (default false).
     */
    void setResizable(bool resizable);

    /*
     * Method: setSize
     * Usage: gw.setSize(width, height);
     * ---------------------------------
     * Sets the size of the graphics window in pixels.
     */
    void setSize(double width, double height);

    /*
     * Method: setTitle
     * Usage: gw.setTitle(title);
     * --------------------------
     * Sets the title of the graphics window.
     * Equivalent to setWindowTitle.
     * (A synonym for setWindowTitle whose name more closely matches Java's.)
     */
    void setTitle(const std::string& title);

    /*
     * Method: setVisible
     * Usage: gw.setVisible(flag);
     * ---------------------------
     * Determines whether the window is visible on the screen.
     */
    void setVisible(bool flag);

    /*
     * Method: setWidth
     * Usage: gw.setWidth(width);
     * --------------------------
     * Sets the width of the graphics window in pixels without changing its height.
     */
    void setWidth(double width);

    /*
     * Method: setWindowTitle
     * Usage: gw.setWindowTitle(title);
     * --------------------------------
     * Sets the title of the graphics window.
     * Equivalent to setTitle.
     */
    void setWindowTitle(const std::string& title);

    /*
     * Method: setX
     * Usage: gw.setX(x);
     * ------------------
     * Sets the x location of the graphics window relative to the top/left
     * of the screen without modifying its y-coordinate.
     */
    void setX(double x);

    /*
     * Method: setY
     * Usage: gw.setY(y);
     * ------------------
     * Sets the y location of the graphics window relative to the top/left
     * of the screen without modifying its x-coordinate.
     */
    void setY(double y);

    /*
     * Method: toBack
     * Usage: gw.toBack();
     * -----------------------
     * Asks the window to move itself behind other graphical windows on screen.
     */
    void toBack();

    /*
     * Method: toFront
     * Usage: gw.toFront();
     * -----------------------
     * Asks the window to move itself in front of other graphical windows on screen.
     */
    void toFront();

    /*
     * Operator: ==
     * Usage: if (w1 == w2) ...
     * ------------------------
     * Checks whether the two objects refer to the same window.
     */
    bool operator ==(const GWindow& w2);

    /*
     * Operator: !=
     * Usage: if (w1 != w2) ...
     * ------------------------
     * Checks whether the two objects refer to different windows.
     */
    bool operator !=(const GWindow& w2);

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    explicit GWindow(bool visible);
    GWindow(GWindowData* gwd);

private:
    /* Instance variables */
    GWindowData* gwd;

    /* Private methods */
    void initGWindow(double width, double height, bool visible);
    static std::string alignmentToString(Alignment alignment);
    static std::string regionToString(Region region);

    friend class stanfordcpplib::Platform;
    friend class GKeyEvent;
    friend class GMouseEvent;
    friend class GWindowEvent;
};

/*
 * Function: convertColorToRGB
 * Usage: int rgb = convertColorToRGB(colorName);
 * ----------------------------------------------
 * Converts a color name into an integer that encodes the
 * red, green, and blue components of the color.
 */
int convertColorToRGB(const std::string& colorName);

/*
 * Function: convertRGBToColor
 * Usage: int colorName = convertRGBToColor(rgb);
 * ----------------------------------------------
 * Converts an <code>rgb</code> value into a color name in the
 * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
 * <code>gg</code>, and <code>bb</code> values are two-digit
 * hexadecimal numbers indicating the intensity of that component.
 */
std::string convertRGBToColor(int rgb);

/*
 * Function: exitGraphics
 * Usage: exitGraphics();
 * ----------------------
 * Closes all graphics windows and exits from the application without
 * waiting for any additional user interaction.
 */
void exitGraphics();

/*
 * Function: getScreenHeight
 * Usage: height = getScreenHeight();
 * ----------------------------------
 * Returns the height of the entire display screen.
 */
double getScreenHeight();

/*
 * Function: getScreenWidth
 * Usage: width = getScreenWidth();
 * --------------------------------
 * Returns the width of the entire display screen.
 */
double getScreenWidth();

/*
 * Function: getScreenSize
 * Usage: size = getScreenSize();
 * ----------------------------------
 * Returns the width/height of the entire display screen.
 */
GDimension getScreenSize();

// private functions for auditing calls to pause();
// used to facilitate creation of autograder programs
double gwindowGetLastPauseMS();
int gwindowGetNumPauses();
void gwindowResetLastPauseMS();
void gwindowResetNumPauses();
void gwindowSetExitGraphicsEnabled(bool value = true);
void gwindowSetPauseEnabled(bool value = true);

/*
 * Function: pause
 * Usage: pause(milliseconds);
 * ---------------------------
 * Pauses for the indicated number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 */
void pause(double milliseconds);

/*
 * Function: repaint
 * Usage: repaint();
 * -----------------
 * Issues a request to update all graphics windows.  This function
 * is called automatically when the program pauses, waits for an
 * event, waits for user input on the console, or terminates.  As
 * a result, most clients never need to call repaint explicitly.
 */
void repaint();

/*
 * Function: waitForClick
 * Usage: waitForClick();
 * ----------------------
 * Waits for a mouse click to occur anywhere in any window.
 */
GMouseEvent waitForClick();

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gwindow_h
