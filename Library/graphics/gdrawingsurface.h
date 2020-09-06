/*
 * File: gdrawingsurface.h
 * -----------------------
 *
 * @author Marty Stepp
 * @version 2018/09/10
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gdrawingsurface.h to replace Java version
 * @version 2018/07/11
 * - initial version
 */


#ifndef _gdrawingsurface_h
#define _gdrawingsurface_h

#include <string>
#include <QFont>
#include <QWidget>

#include "grid.h"
#include "gobjects.h"
#include "gtypes.h"

/**
 * GDrawingSurface is an abstract superclass for types that allow drawing shapes
 * and pixels onto themselves as a pixel background layer.
 * This includes graphical canvas objects (GCanvas) as well as windows (GWindow).
 */
class GDrawingSurface {
public:
    /**
     * Erases any pixel data from the drawing surface.
     */
    virtual void clear() = 0;

    /**
     * Repaints the interactor only if its contents have changed.
     */
    virtual void conditionalRepaint();

    /**
     * Repaints the given region of the interactor only if its contents have changed.
     */
    virtual void conditionalRepaintRegion(int x, int y, int width, int height);

    /**
     * Repaints the given region of the interactor only if its contents have changed.
     */
    virtual void conditionalRepaintRegion(const GRectangle& bounds);

    /**
     * Draws the given graphical object onto the background pixel layer of this
     * interactor.
     * @throw ErrorException if the object is null
     */
    virtual void draw(GObject* gobj) = 0;

    /**
     * Draws the given graphical object onto the background pixel layer of this
     * interactor, moving it to the given x/y location first.
     * @throw ErrorException if the object is null
     */
    virtual void draw(GObject* gobj, double x, double y);

    /**
     * Draws the given graphical object onto the background pixel layer of this
     * interactor.
     */
    virtual void draw(GObject& gobj);

    /**
     * Draws the given graphical object onto the background pixel layer of this
     * interactor, moving it to the given x/y location first.
     */
    virtual void draw(GObject& gobj, double x, double y);

    /**
     * Draws this interactor with the given Qt painter object.
     * @private
     */
    virtual void draw(QPainter* painter) = 0;

    /**
     * Draws an unfilled arc with the given attributes onto the background pixel
     * layer of this interactor in the current color.
     * See gobjects.h for explanation of GArc parameters.
     */
    virtual void drawArc(double x, double y, double width, double height, double start, double sweep);

    /**
     * Draws an image loaded from the given file name onto the background pixel
     * layer of this interactor at the given x/y location.
     * See gobjects.h for explanation of GImage parameters.
     * @throw ErrorException if the given file is not found or cannot be loaded
     *        as a valid image file
     */
    virtual void drawImage(const std::string& filename, double x = 0, double y = 0);

    /**
     * Draws a line between the given two points onto the background pixel
     * layer of this interactor at the given x/y location in the current color.
     * See gobjects.h for explanation of GLine parameters.
     */
    virtual void drawLine(const GPoint& p0, const GPoint& p1);

    /**
     * Draws a line between the given two points onto the background pixel
     * layer of this interactor at the given x/y location in the current color.
     * See gobjects.h for explanation of GLine parameters.
     */
    virtual void drawLine(double x0, double y0, double x1, double y1);

    /**
     * Draws an unfilled oval with the given bounding box onto the background pixel
     * layer of this interactor at the given x/y location in the current color.
     * See gobjects.h for explanation of GOval parameters.
     */
    virtual void drawOval(const GRectangle& bounds);

    /**
     * Draws an unfilled oval with the given bounding box onto the background pixel
     * layer of this interactor at the given x/y location in the current color.
     * See gobjects.h for explanation of GOval parameters.
     */
    virtual void drawOval(double x, double y, double width, double height);

    /**
     * Draws a line using polar coordinates onto the background pixel
     * layer of this interactor in the current color.
     * The line begins at the given x/y point and extends from there by the
     * given angle and radius.
     * Returns the end point opposite p0 where the line ends.
     * See gobjects.h for explanation of GLine parameters.
     */
    virtual GPoint drawPolarLine(const GPoint& p0, double r, double theta);

    /**
     * Draws a line using polar coordinates onto the background pixel
     * layer of this interactor in the current color.
     * The line begins at the given x/y point and extends from there by the
     * given angle and radius.
     * Returns the end point where the line ends.
     * See gobjects.h for explanation of GLine parameters.
     */
    virtual GPoint drawPolarLine(double x0, double y0, double r, double theta);

    /**
     * Colors the given x/y pixel of the background layer of this interactor
     * using the interactor's current color.
     */
    virtual void drawPixel(double x, double y);

    /**
     * Colors the given x/y pixel of the background layer of this interactor
     * using the given color.
     */
    virtual void drawPixel(double x, double y, int color);

    /**
     * Colors the given x/y pixel of the background layer of this interactor
     * using the given color.
     */
    virtual void drawPixel(double x, double y, const std::string& color);

    /**
     * Draws an unfilled polygon containing the given points onto the background
     * pixel layer of this interactor in the current color.
     * See gobjects.h for explanation of GPolygon parameters.
     */
    virtual void drawPolygon(std::initializer_list<double> coords);

    /**
     * Draws an unfilled polygon containing the given points onto the background
     * pixel layer of this interactor in the current color.
     * See gobjects.h for explanation of GPolygon parameters.
     */
    virtual void drawPolygon(std::initializer_list<GPoint> points);

    /**
     * Draws an unfilled rectangle of the given dimensions onto the background
     * pixel layer of this interactor in the current color.
     * See gobjects.h for explanation of GRect parameters.
     */
    virtual void drawRect(const GRectangle& bounds);

    /**
     * Draws an unfilled rectangle of the given dimensions onto the background
     * pixel layer of this interactor in the current color.
     * See gobjects.h for explanation of GRect parameters.
     */
    virtual void drawRect(double x, double y, double width, double height);

    /**
     * Draws a text string onto the background pixel layer of this interactor
     * at the given x/y location in the current font and color.
     * See gobjects.h for explanation of GText parameters.
     */
    virtual void drawString(const std::string& text, double x, double y);

    /**
     * Draws a filled arc with the given attributes onto the background pixel
     * layer of this interactor in the current color and fill color.
     * See gobjects.h for explanation of GArc parameters.
     */
    virtual void fillArc(double x, double y, double width, double height, double start, double sweep);

    /**
     * Draws a filled oval with the given bounding box onto the background pixel
     * layer of this interactor at the given x/y location in the current color
     * and fill color.
     * See gobjects.h for explanation of GOval parameters.
     */
    virtual void fillOval(const GRectangle& bounds);

    /**
     * Draws a filled oval with the given bounding box onto the background pixel
     * layer of this interactor at the given x/y location in the current color
     * and fill color.
     * See gobjects.h for explanation of GOval parameters.
     */
    virtual void fillOval(double x, double y, double width, double height);

    /**
     * Draws a filled polygon containing the given points onto the background
     * pixel layer of this interactor in the current color and fill color.
     * See gobjects.h for explanation of GPolygon parameters.
     */
    virtual void fillPolygon(std::initializer_list<double> coords);

    /**
     * Draws a filled polygon containing the given points onto the background
     * pixel layer of this interactor in the current color and fill color.
     * See gobjects.h for explanation of GPolygon parameters.
     */
    virtual void fillPolygon(std::initializer_list<GPoint> coords);

    /**
     * Draws a filled rectangle of the given dimensions onto the background
     * pixel layer of this interactor in the current color and fill color.
     * See gobjects.h for explanation of GRect parameters.
     */
    virtual void fillRect(const GRectangle& bounds);

    /**
     * Draws a filled rectangle of the given dimensions onto the background
     * pixel layer of this interactor in the current color and fill color.
     * See gobjects.h for explanation of GRect parameters.
     */
    virtual void fillRect(double x, double y, double width, double height);

    /**
     * Returns the pixel color data at the given x/y location,
     * retaining alpha-channel transparency in the top 8 bits.
     */
    virtual int getARGB(double x, double y) const;

    /**
     * Returns the current background color of the interactor as a string.
     * See gcolor.h for more detail about color strings.
     */
    virtual std::string getBackground() const;

    /**
     * Returns the current background color of the interactor as an RGB integer.
     * See gcolor.h for more detail about colors.
     */
    virtual int getBackgroundInt() const;

    /**
     * Returns the current foreground outline color of the interactor as a string.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to getForeground.
     */
    virtual std::string getColor() const;

    /**
     * Returns the current foreground outline color of the interactor as an RGB integer.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about colors.
     * Equivalent to getForegroundInt.
     */
    virtual int getColorInt() const;

    /**
     * Returns the current fill color of the interactor as a string.
     * This color will appear in shapes drawn using the fillXxx methods.
     * See gcolor.h for more detail about color strings.
     */
    virtual std::string getFillColor() const;

    /**
     * Returns the current fill color of the interactor as an RGB integer.
     * This color will appear in shapes drawn using the fillXxx methods.
     * See gcolor.h for more detail about color strings.
     */
    virtual int getFillColorInt() const;

    /**
     * Returns the current text font of the interactor as a font string.
     * This font will be used when drawing text strings using drawString.
     * See gfont.h for more detail about font strings.
     */
    virtual std::string getFont() const;

    /**
     * Returns the current foreground outline color of the interactor as a string.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to getColor.
     */
    virtual std::string getForeground() const;

    /**
     * Returns the current foreground outline color of the interactor as an RGB integer.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about colors.
     * Equivalent to getColor.
     */
    virtual int getForegroundInt() const;

    /**
     * Returns the current line style which will be used to draw outlines of
     * shapes and lines.
     * The default line style is a solid line (GObject::LINE_SOLID).
     */
    virtual GObject::LineStyle getLineStyle() const;

    /**
     * Returns the thickness used when drawing outlines of shapes and lines.
     * The default thickness is 1.
     */
    virtual double getLineWidth() const;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the interactor as an integer such as 0xff00cc.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * Equivalent to getRGB.
     *
     * @throw ErrorException if the given x/y values are out of bounds.
     */
    virtual int getPixel(double x, double y) const = 0;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the interactor as an integer such as 0xffff00cc.
     * This differs from getPixel in that it explicitly retains and returns
     * the alpha channel of the pixel in the top 8 bits, allowing for
     * transparency effects.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * @throw ErrorException if the given x/y values are out of bounds.
     */
    virtual int getPixelARGB(double x, double y) const = 0;

    /**
     * Returns all pixels of the surface as a Grid,
     * where rows represent y values and columns represent x values.
     */
    virtual Grid<int> getPixels() const = 0;

    /**
     * Returns all pixels of the background layer of the surface as a Grid,
     * where rows represent y values and columns represent x values.
     * This differs from getPixels in that it explicitly retains and returns
     * the alpha channel of each pixel in the top 8 bits, allowing for
     * transparency effects.
     */
    virtual Grid<int> getPixelsARGB() const = 0;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the image
     * as a string such as "#ff00cc".
     * The string that is returned comes from the <code>GWindow</code> function
     * <code>convertRGBToColor</code>; see documentation of that function.
     * Throws an error if the given x/y values are out of bounds.
     */
    virtual std::string getPixelString(double x, double y) const;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the interactor as an integer such as 0xff00cc.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * Equivalent to getPixel.
     *
     * @throw ErrorException if the given x/y values are out of bounds.
     */
    virtual int getRGB(double x, double y) const;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the interactor as a color string such as "#ff00cc".
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * @throw ErrorException if the given x/y values are out of bounds.
     */
    virtual std::string getRGBString(double x, double y) const;

    /**
     * Returns true if the interactor should repaint itself automatically whenever
     * any change is made to its graphical data.
     * But if you call setAutoRepaint(false), you must manually repaint the interactor
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     * Equivalent to isRepaintImmediately.
     */
    virtual bool isAutoRepaint() const;

    /**
     * Returns true if the interactor should repaint itself automatically whenever
     * any change is made to its graphical data.
     * But if you call setAutoRepaint(false), you must manually repaint the interactor
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     * Equivalent to isAutoRepaint.
     */
    virtual bool isRepaintImmediately() const;

    /**
     * Instructs the interactor to redraw itself on the screen.
     * By default the interactor will automatically repaint itself whenever you
     * make any change to its graphical data.
     * But if you call setAutoRepaint(false), you must manually repaint the interactor
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     */
    virtual void repaint() = 0;

    /**
     * Instructs the interactor to repaint the given region of pixel data.
     * This can be preferable to repaint() for performance purposes if you have
     * made a small change that affects only the given rectangular region of
     * the interactor.
     */
    virtual void repaintRegion(int x, int y, int width, int height) = 0;

    /**
     * Instructs the interactor to repaint the given region of pixel data.
     * This can be preferable to repaint() for performance purposes if you have
     * made a small change that affects only the given rectangular region of
     * the interactor.
     */
    virtual void repaintRegion(const GRectangle& bounds);

    /**
     * Sets whether the interactor should repaint itself automatically whenever
     * any change is made to its graphical data.
     * By default this is true.
     * But if you call setAutoRepaint(false), you must manually repaint the interactor
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     * Equivalent to setRepaintImmediately.
     */
    virtual void setAutoRepaint(bool autoRepaint);

    /**
     * Sets the current background color of the interactor as an RGB integer.
     * See gcolor.h for more detail about colors.
     */
    virtual void setBackground(int color);

    /**
     * Sets the current background color of the interactor as a string.
     * See gcolor.h for more detail about color strings.
     */
    virtual void setBackground(const std::string& color);

    /**
     * Sets the current foreground outline color of the interactor as as RGB integer.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to setForeground.
     */
    virtual void setColor(int color);

    /**
     * Sets the current foreground outline color of the interactor as a string.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to setForeground.
     */
    virtual void setColor(const std::string& color);

    /**
     * Sets the current fill color of the interactor as an RGB integer.
     * This color will appear in shapes drawn using the fillXxx methods.
     * See gcolor.h for more detail about color strings.
     */
    virtual void setFillColor(int color);

    /**
     * Returns the current fill color of the interactor as a string.
     * This color will appear in shapes drawn using the fillXxx methods.
     * See gcolor.h for more detail about color strings.
     */
    virtual void setFillColor(const std::string& color);

    /**
     * Returns the current text font of the interactor using a Qt font object.
     * This font will be used when drawing text strings using drawString.
     * @private
     */
    virtual void setFont(const QFont& font);

    /**
     * Sets the current text font of the interactor as a font string.
     * This font will be used when drawing text strings using drawString.
     * See gfont.h for more detail about font strings.
     */
    virtual void setFont(const std::string& font);

    /**
     * Sets the current foreground outline color of the interactor as an RGB integer.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to setColor.
     */
    virtual void setForeground(int color);

    /**
     * Sets the current foreground outline color of the interactor as a string.
     * This color will be used to draw the outlines of shapes drawn using the
     * drawXxx and fillXxx methods, as well as being the default color used when
     * calling setPixel or setRGB.
     * See gcolor.h for more detail about color strings.
     * Equivalent to setColor.
     */
    virtual void setForeground(const std::string& color);

    /**
     * Sets the thickness used when drawing outlines of shapes and lines.
     * The default thickness is 1.
     */
    virtual void setLineWidth(double lineWidth);

    /**
     * Sets the current line style which will be used to draw outlines of
     * shapes and lines.
     * The default line style is a solid line (GObject::LINE_SOLID).
     */
    virtual void setLineStyle(GObject::LineStyle lineStyle);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given RGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setRGB.
     *
     * @throw ErrorException if x/y is out of range or rgb is an invalid color
     */
    virtual void setPixel(double x, double y, int rgb) = 0;

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given RGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setRGB.
     *
     * @throw ErrorException if x/y is out of range or r,g,b are not between 0-255
     */
    virtual void setPixel(double x, double y, int r, int g, int b);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given color.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setRGB.
     *
     * @throw ErrorException if x/y is out of range
     */
    virtual void setPixel(double x, double y, const std::string& color);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given ARGB value.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixelsARGB to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixelsARGB to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or argb is an invalid color
     */
    virtual void setPixelARGB(double x, double y, int argb) = 0;

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given ARGB value.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixelsARGB to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixelsARGB to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or a,r,g,b are not between 0-255
     */
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b);

    /**
     * Sets the color of the all pixels in the background layer of the
     * interactor to the given RGB values, using rows as y-values and columns as
     * x-values.  Any existing background layer pixels will be replaced.
     * If the given grid is not the same size as this interactor, the interactor
     * will be resized to match the grid.
     */
    virtual void setPixels(const Grid<int>& pixels) = 0;

    /**
     * Sets the color of the all pixels in the background layer of the
     * interactor to the given ARGB values, using rows as y-values and columns as
     * x-values.  Any existing background layer pixels will be replaced.
     * If the given grid is not the same size as this interactor, the interactor
     * will be resized to match the grid.
     */
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) = 0;

    /**
     * Sets whether the interactor should repaint itself automatically whenever
     * any change is made to its graphical data.
     * By default this is true.
     * But if you call setAutoRepaint(false), you must manually repaint the interactor
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     * Equivalent to setAutoRepaint.
     */
    virtual void setRepaintImmediately(bool autoRepaint);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given RGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setPixel.
     *
     * @throw ErrorException if x/y is out of range or rgb is an invalid color
     */
    virtual void setRGB(double x, double y, int rgb);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given RGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setPixel.
     *
     * @throw ErrorException if x/y is out of range or r,g,b are not between 0-255
     */
    virtual void setRGB(double x, double y, int r, int g, int b);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * interactor to the given color.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     * Equivalent to setPixel.
     *
     * @throw ErrorException if x/y is out of range
     */
    virtual void setRGB(double x, double y, const std::string& color);

protected:
    GDrawingSurface();
    virtual ~GDrawingSurface();

    GDrawingSurface* _forwardTarget;
    std::string _backgroundColor;
    std::string _color;
    std::string _fillColor;
    std::string _font;
    int _backgroundColorInt;
    int _colorInt;
    int _fillColorInt;
    GObject::LineStyle _lineStyle;
    double _lineWidth;
    bool _autoRepaint;

    /**
     * Throws an error if the given x/y values are out of bounds.
     */
    void checkBounds(const std::string& member, double x, double y, double width, double height) const;

    /**
     * Throws an error if the given rgb value is not a valid color.
     */
    void checkColor(const std::string& member, int rgb) const;

    /**
     * Throws an error if the given width/height values are out of bounds.
     */
    void checkSize(const std::string& member, double width, double height) const;

    /**
     * Initializes a new graphical object to be drawn.
     * Used as a convenience method to set the color, fill color, outline style,
     * font, and other settings of graphical objects based on the settings of
     * the drawing surface.
     */
    virtual void initializeGObject(GObject& obj, bool filled = false);

    /**
     * Initializes a new graphical object to be drawn.
     * Used as a convenience method to set the color, fill color, outline style,
     * font, and other settings of graphical objects based on the settings of
     * the drawing surface.
     */
    virtual void initializeGObject(GObject* obj, bool filled = false);

    /**
     * Sets a forward target to which drawing calls should be sent.
     * Used by GForwardDrawingSurface.
     * @private
     */
    virtual void setDrawingForwardTarget(GDrawingSurface* forwardTarget);
};

/**
 * A drawing surface that just forwards any calls it receives to another
 * drawing surface.
 * In our library this class is primarily used so that a GWindow can pass on
 * drawing calls like drawRect and fillOval on to its internal GCanvas instance.
 * @private
 */
class GForwardDrawingSurface : public virtual GDrawingSurface {
public:
    void clear() override;
    void draw(GObject* gobj) override;
    void draw(GObject* gobj, double x, double y) override;
    void draw(GObject& gobj) override;
    void draw(GObject& gobj, double x, double y) override;
    void draw(QPainter* painter) override;
    int getPixel(double x, double y) const override;
    int getPixelARGB(double x, double y) const override;
    Grid<int> getPixels() const override;
    Grid<int> getPixelsARGB() const override;
    bool isAutoRepaint() const override;
    void repaint() override;
    void repaintRegion(int x, int y, int width, int height) override;
    void setAutoRepaint(bool autoRepaint) override;
    void setBackground(int color) override;
    void setBackground(const std::string& color) override;
    void setColor(int color) override;
    void setColor(const std::string& color) override;
    void setFillColor(int color) override;
    void setFillColor(const std::string& color) override;
    void setFont(const QFont& font) override;
    void setFont(const std::string& font) override;
    void setLineWidth(double lineWidth) override;
    void setPixel(double x, double y, int rgb) override;
    void setPixel(double x, double y, int r, int g, int b) override;
    void setPixelARGB(double x, double y, int argb) override;
    void setPixelARGB(double x, double y, int a, int r, int g, int b) override;
    void setPixels(const Grid<int>& pixels) override;
    void setPixelsARGB(const Grid<int>& pixelsARGB) override;
    void setRepaintImmediately(bool repaintImmediately) override;

protected:
    virtual void ensureForwardTarget() = 0;
    virtual void ensureForwardTargetConstHack() const;
};

#endif // _gcanvas_h
