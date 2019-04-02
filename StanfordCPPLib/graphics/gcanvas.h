/*
 * File: gcanvas.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2019/03/07
 * - added support for loading canvas directly from istream (htiek)
 * @version 2018/09/10
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcanvas.h to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gcanvas_h
#define _gcanvas_h

#include <string>
#include <QWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QWheelEvent>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "gdrawingsurface.h"
#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "gobjects.h"
#undef INTERNAL_INCLUDE

// default color used to highlight pixels that do not match between two images
#define GCANVAS_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

class _Internal_QCanvas;

/**
 * A GCanvas is a graphical drawing surface on which you can draw shapes, lines,
 * and colors, as well as setting the RGB color values of individual pixels.
 *
 * The graphical canvas consists of two layers:
 *
 * 1) The background layer provides a surface for drawing static pictures that
 * involve no animation, or for 2D pixel-based drawing algorithms.
 * The class includes several drawXxx and fillXxx methods that draw
 * lines, rectangles, and ovals on the background layer.
 *
 * The setPixel and setPixels methods manipulate the color of pixels in the
 * background layer.  You can get all of the pixels as a Grid using getPixels,
 * modify the grid, then pass it back in using setPixels, to perform 2D
 * pixel-based manipulations on the canvas.
 *
 * 2) The foreground layer provides an abstraction for adding stateful shapes and
 * graphical objects onto the canvas.  The add() methods that accept GObject
 * parameters place these objects onto the foreground layer.  The advantage of
 * the foreground layer is that you can manipulate the object over time, such as
 * moving it, changing its color, size, or other properties, and see these
 * changes immediately on the screen.  This makes the foreground layer most
 * appropriate for animations or moving sprites.
 *
 * A GCanvas is implicitly added to the center of every GWindow when the client
 * calls the window's add(), drawXxx/fillXxx, or other methods.  In most cases
 * the window just forwards these method calls to its internal GCanvas, which
 * performs the bulk of the work.
 *
 * See gobjects.h for more detail about drawing shapes and objects.
 */
class GCanvas : public virtual GInteractor, public virtual GDrawingSurface {
public:
    /**
     * Largest value that an image's width and/or height can have.
     * Error will be thrown if you try to make/resize an image larger than this.
     */
    static const int WIDTH_HEIGHT_MAX;

    /**
     * Creates a single RGB integer from the given R-G-B components from 0-255.
     * @throw ErrorException if red, green, or blue is not between 0-255 inclusive
     */
    static int createRgbPixel(int red, int green, int blue);

    /**
     * Extracts the alpha component from 0-255 of the given ARGB integer.
     * The alpha component comes from bits 24-31 (most significant) of the integer.
     */
    static int getAlpha(int argb);

    /**
     * Extracts the blue component from 0-255 of the given RGB integer.
     * The blue component comes from bits 0-7 (least significant) of the integer.
     */
    static int getBlue(int rgb);

    /**
     * Extracts the green component from 0-255 of the given RGB integer.
     * The green component comes from bits 8-15 of the integer.
     */
    static int getGreen(int rgb);

    /**
     * Extracts the red component from 0-255 of the given RGB integer.
     * The red component comes from bits 16-23 of the integer.
     */
    static int getRed(int rgb);

    /**
     * Extracts the red, green, and blue components from 0-255
     * of the given RGB integer, filling by reference.
     */
    static void getRedGreenBlue(int rgb, int& red, int& green, int& blue);

    /**
     * Creates an empty canvas with a default size of 0x0 pixels
     * and a default background and foreground color of black.
     */
    GCanvas(QWidget* parent = nullptr);

    /**
     * Creates a canvas that loads its background layer pixel data from
     * the given image file name.
     * @throw ErrorException if the given file does not exist or cannot be read
     *        as a valid image file
     */
    GCanvas(const std::string& filename, QWidget* parent = nullptr);

    /**
     * Creates a canvas that loads its background layer pixel data from
     * the given input stream
     * @throw ErrorException if the given stream cannot be read as a valid image file
     */
    GCanvas(std::istream& filename, QWidget* parent = nullptr);

    /**
     * Creates an empty canvas of the specified size and optional background color.
     * If no background color is passed, a default transparent background is used.
     * @throw ErrorException if the given width/height ranges are negative
     * @throw ErrorException if the given rgb value is invalid or out of range
     */
    GCanvas(double width, double height, int rgbBackground, QWidget* parent = nullptr);

    /**
     * Creates an empty canvas of the specified size and background color.
     * If no background color is passed, a default transparent background is used.
     * @throw ErrorException if the given width/height ranges are negative
     * @throw ErrorException if the given rgb value is invalid or out of range
     */
    GCanvas(double width, double height, const std::string& rgbBackground = "#00000000", QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the canvas.
     */
    virtual ~GCanvas() Q_DECL_OVERRIDE;

    /**
     * Adds the given interactor to canvas.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GObject* gobj);

    /**
     * Adds the given interactor to the canvas
     * and moves it to the given x/y location.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GObject* gobj, double x, double y);

    /**
     * Adds the given interactor to canvas.
     */
    virtual void add(GObject& gobj);

    /**
     * Adds the given interactor to the canvas
     * and moves it to the given x/y location.
     */
    virtual void add(GObject& gobj, double x, double y);

    /**
     * Removes all graphical objects from the canvas foreground layer
     * and wipes the background layer to show the current background color.
     */
    virtual void clear() Q_DECL_OVERRIDE;

    /**
     * Removes all graphical objects from the foreground layer of the canvas.
     * This means that any shapes added using the add() methods, such as GRect,
     * GOval, etc. will be removed, while any shapes drawn directly onto the
     * canvas's background pixel layer by calling the drawXxx() methods will be
     * retained.  To clear the background layer as well, call clearPixels
     * or clear instead.
     */
    virtual void clearObjects();

    /**
     * Resets the background layer of pixels in the canvas to the current
     * background color.
     * This means that any shapes added using the add() methods, such as GRect,
     * GOval, etc. will remain, while any shapes drawn directly onto the
     * canvas's background pixel layer by calling the drawXxx() methods will be
     * wiped out.  To clear the shapes added to the foreground layer as well,
     * call clearObjects or clear instead.
     */
    virtual void clearPixels();

    /**
     * Returns true if any of the graphical objects in the foreground layer of
     * the canvas touch the given x/y pixel.
     */
    virtual bool contains(double x, double y) const;

    /**
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * In this version of the method, the entire images are compared.
     */
    virtual int countDiffPixels(const GCanvas& image) const;

    /**
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * In this version of the method, you pass an (x,y) range of pixels to compare.
     */
    virtual int countDiffPixels(const GCanvas& image, int xmin, int ymin, int xmax, int ymax) const;

    /**
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * In this version of the method, the entire images are compared.
     * @throw ErrorException if the image passed is null
     */
    virtual int countDiffPixels(const GCanvas* image) const;

    /**
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * In this version of the method, you pass an (x,y) range of pixels to compare.
     * @throw ErrorException if the image passed is null
     */
    virtual int countDiffPixels(const GCanvas* image, int xmin, int ymin, int xmax, int ymax) const;

    /**
     * Generates a new canvas whose content is equal to that of this canvas but with
     * any pixels that don't match those in parameter 'image' colored in the given
     * color (default purple) to highlight differences between the two.
     */
    virtual GCanvas* diff(const GCanvas& image, int diffPixelColor = GCANVAS_DEFAULT_DIFF_PIXEL_COLOR) const;

    /**
     * Generates a new canvas whose content is equal to that of this canvas but with
     * any pixels that don't match those in parameter 'image' colored in the given
     * color (default purple) to highlight differences between the two.
     * @throw ErrorException if the image passed is null
     */
    virtual GCanvas* diff(const GCanvas* image, int diffPixelColor = GCANVAS_DEFAULT_DIFF_PIXEL_COLOR) const;

    /**
     * Draws the given graphical object onto the background layer of the canvas.
     * Note that since it is drawn on the background layer, future changes to
     * the graphical object, such as setting its location, color, size, etc.,
     * will not be reflected immediately on this canvas.
     * If you do want such changes to be reflected, instead add the shape to
     * the foreground layer using add().
     * @throw ErrorException if the object passed is null
     */
    virtual void draw(GObject* gobj) Q_DECL_OVERRIDE;

    /**
     * @inherit
     * @private
     */
    virtual void draw(QPainter* painter) Q_DECL_OVERRIDE;

    /**
     * Returns true if the two given canvases contain exactly the same pixel data.
     */
    virtual bool equals(const GCanvas& other) const;

    /**
     * Sets the color of every pixel in the canvas to the given color value.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given rgb value is not a valid color
     */
    virtual void fill(int rgb);

    /**
     * Sets the color of every pixel in the canvas to the given color value.
     * See gcolor.h for more detail about colors.
     */
    virtual void fill(const std::string& rgb);

    /**
     * Sets the color of every pixel in the given rectangular range of the canvas
     * pixel data to the given color value.
     * Specifically, the pixels in the rectangular range (x, y) through
     * (x + width - 1, y + height - 1) become filled with the given color.
     * @throw ErrorException if the given x/y/width/height range goes outside
     *        the bounds of the image,
     *        or if the given rgb value is not a valid color
     */
    virtual void fillRegion(double x, double y, double width, double height, int rgb);

    /**
     * Sets the color of every pixel in the given rectangular range of the canvas
     * pixel data to the given color value.
     * Specifically, the pixels in the rectangular range (x, y) through
     * (x + width - 1, y + height - 1) become filled with the given color.
     * @throw ErrorException if the given x/y/width/height range goes outside
     *        the bounds of the image
     */
    virtual void fillRegion(double x, double y, double width, double height,
                            const std::string& rgb);

    /**
     * Moves all graphical objects from the foreground layer to the background
     * layer.  This means that future changes to those objects will not be
     * seen on the canvas.
     */
    virtual void flatten();

    /**
     * Replaces the entire contents of the background layer of the canvas with
     * the contents of the given grid of RGB pixel values.
     * If this image is not the same size as the grid, the image is resized.
     * Any previous background layer contents are lost.
     * Equivalent to getPixels.
     */
    virtual void fromGrid(const Grid<int>& grid);

    /* @inherit */
    virtual std::string getBackground() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual int getBackgroundInt() const Q_DECL_OVERRIDE;

    /**
     * Returns a pointer to the graphical object in the foreground layer of
     * the canvas at the specified index, numbering from back to front in the
     * <i>z</i> dimension.
     * @throw ErrorException if the index is out of range
     */
    virtual GObject* getElement(int index) const;

    /**
     * Returns a pointer to the first graphical object in the foreground layer of
     * the canvas that contains the given (x, y) point, or a null pointer if no
     * object in this canvas touches it.
     */
    virtual GObject* getElementAt(double x, double y) const;

    /**
     * Returns the number of graphical objects stored in the foreground layer
     * of the canvas.
     */
    virtual int getElementCount() const;

    /**
     * Returns the name of the image file from which this canvas was loaded or
     * to which it was saved most recently.
     * If this canvas was not associated with any file, returns "".
     */
    virtual std::string getFilename() const;

    /* @inherit */
    virtual std::string getFont() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the canvas as an integer such as 0xff00cc.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * @throw ErrorException if the given x/y values are out of bounds.
     */
    virtual int getPixel(double x, double y) const Q_DECL_OVERRIDE;

    /**
     * Returns the color of the pixel at the given x/y coordinates of the
     * background layer of the canvas as an integer such as 0xffff00cc.
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
    virtual int getPixelARGB(double x, double y) const Q_DECL_OVERRIDE;

    /**
     * Returns all pixels of the background layer of the canvas as a Grid,
     * where rows represent y values and columns represent x values.
     * So for example, grid[y][x] returns the RGB int value at that pixel.
     * Equivalent to toGrid.
     */
    virtual Grid<int> getPixels() const Q_DECL_OVERRIDE;

    /**
     * Returns all pixels of the background layer of the canvas as a Grid,
     * where rows represent y values and columns represent x values.
     * This differs from getPixels in that it explicitly retains and returns
     * the alpha channel of each pixel in the top 8 bits, allowing for
     * transparency effects.
     */
    virtual Grid<int> getPixelsARGB() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual bool isAutoRepaint() const Q_DECL_OVERRIDE;

    /**
     * Reads the canvas's pixel contents from the given image file.
     * @throw ErrorException if the given file does not exist or cannot be read
     *        as a valid image file
     */
    virtual void load(const std::string& filename);

    /**
     * Reads the canvas's pixel contents from the given input stream.
     * @throw ErrorException if the given file does not exist or cannot be read
     *        as a valid image file
     */

    /**
     * Removes the given graphical object from the foreground layer of the canvas,
     * if it was present.
     * @throw ErrorException if the graphical object is null
     */
    virtual void remove(GObject* gobj);

    /**
     * Removes the given graphical object from the foreground layer of the canvas,
     * if it was present.
     */
    virtual void remove(GObject& gobj);

    /**
     * Removes all graphical objects from the foreground layer of the canvas.
     */
    virtual void removeAll();

    /**
     * Removes the click listener from the canvas so that it will no longer
     * call it when events occur.
     */
    virtual void removeClickListener();

    /**
     * Removes the double-click listener from the canvas so that it will no longer
     * call it when events occur.
     */
    virtual void removeDoubleClickListener();

    /**
     * Removes the key listener from the canvas so that it will no longer
     * call it when events occur.
     */
    virtual void removeKeyListener();

    /**
     * Removes the mouse listener from the canvas so that it will no longer
     * call it when events occur.
     */
    virtual void removeMouseListener();

    /**
     * Instructs the canvas to redraw its layers.
     * By default the canvas will automatically repaint itself whenever you make
     * a change to either the background or foreground layer.
     * But if you call setAutoRepaint(false), you must manually repaint the canvas
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     */
    virtual void repaint() Q_DECL_OVERRIDE;

    /**
     * Instructs the canvas to redraw the given region of pixels within both
     * of its layers.
     */
    virtual void repaintRegion(int x, int y, int width, int height) Q_DECL_OVERRIDE;

    /**
     * Changes this image's bounds to be the given size.
     * This does not scale the image but rather just changes the max x/y that
     * can be painted onto this image.
     * If the 'retain' parameter is not passed or is set to true,
     * any existing pixel values will be kept during the resize.
     * If 'retain' is false, the contents will be wiped and set to the default.
     * Any existing pixel contents are discarded and revert to defaults.
     * @throw ErrorException if the given width/height ranges are negative
     */
    virtual void resize(double width, double height, bool retain = true);

    /**
     * Saves the canvas's contents to the given image file.
     * @throw ErrorException if the given file is not writeable.
     */
    virtual void save(const std::string& filename);

    /**
     * Sets whether the canvas will automatically repaint itself whenever you make
     * a change to either the background or foreground layer.
     * By default this is true.
     * But if you call setAutoRepaint(false), you must manually repaint the canvas
     * to see any changes. This can be desirable if you plan to make a large batch
     * of changes and want to repaint only after all of them are done.
     */
    virtual void setAutoRepaint(bool autoRepaint) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setBackground(int color) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is clicked on the canvas.
     * Any existing click listener will be replaced.
     */
    virtual void setClickListener(GEventListener func);

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is clicked on the canvas.
     * Any existing click listener will be replaced.
     */
    virtual void setClickListener(GEventListenerVoid func);

    /* @inherit */
    virtual void setColor(int color) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is double-clicked on the canvas.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListener func);

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is double-clicked on the canvas.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListenerVoid func);

    /* @inherit */
    virtual void setFont(const QFont& font) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setForeground(int rgb) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets a key listener on this canvas so that it will be called
     * when any key is pressed or released on the canvas.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListener func);

    /**
     * Sets a key listener on this canvas so that it will be called
     * when any key is pressed or released on the canvas.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListenerVoid func);

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is moved or clicked on the canvas.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListener func);

    /**
     * Sets a mouse listener on this canvas so that it will be called
     * when the mouse is moved or clicked on the canvas.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListenerVoid func);

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * canvas to the given RGB value.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or rgb is an invalid color
     */
    virtual void setPixel(double x, double y, int rgb) Q_DECL_OVERRIDE;

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * canvas to the given RGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixels to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixels to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or r,g,b are not between 0-255
     */
    virtual void setPixel(double x, double y, int r, int g, int b) Q_DECL_OVERRIDE;

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * canvas to the given ARGB value.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixelsARGB to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixelsARGB to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or argb is an invalid color
     */
    virtual void setPixelARGB(double x, double y, int argb) Q_DECL_OVERRIDE;

    /**
     * Sets the color of the given x/y pixel in the background layer of the
     * canvas to the given ARGB values.
     *
     * Note that if you are planning to set many pixels in the background and
     * want maximum performance, you should instead call getPixelsARGB to extract
     * all pixels into a Grid, then manipulate all desired pixels in that Grid,
     * then call setPixelsARGB to submit all of your changes.
     *
     * @throw ErrorException if x/y is out of range or a,r,g,b are not between 0-255
     */
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b) Q_DECL_OVERRIDE;

    /**
     * Sets the color of the all pixels in the background layer of the
     * canvas to the given RGB values, using rows as y-values and columns as
     * x-values.  Any existing background layer pixels will be replaced.
     * If the given grid is not the same size as this canvas, the canvas will
     * be resized to match the grid.
     * Equivalent to fromGrid.
     */
    virtual void setPixels(const Grid<int>& pixels) Q_DECL_OVERRIDE;

    /**
     * Sets the color of the all pixels in the background layer of the
     * canvas to the given ARGB values, using rows as y-values and columns as
     * x-values.  Any existing background layer pixels will be replaced.
     * If the given grid is not the same size as this canvas, the canvas will
     * be resized to match the grid.
     */
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) Q_DECL_OVERRIDE;

    /**
     * Converts the pixels of the canvas into a GImage object.
     */
    virtual GImage* toGImage() const;

    /**
     * Converts this canvas's pixel data into a grid of RGB pixels.
     * The grid's first index is a row or y-index, and its second index
     * is the column or x-index.
     * So for example, grid[y][x] returns the RGB int value at that pixel.
     * In this version of the method, the grid is returned.
     * Equivalent to getPixels.
     */
    virtual Grid<int> toGrid() const;

    /**
     * Converts this canvas's pixel data into a grid of RGB pixels.
     * The grid's first index is a row or y-index, and its second index
     * is the column or x-index.
     * So for example, grid[y][x] returns the RGB int value at that pixel.
     * In this version of the method, the grid is filled by reference.
     * Equivalent to getPixels.
     */
    virtual void toGrid(Grid<int>& grid) const;

private:
    Q_DISABLE_COPY(GCanvas)

    _Internal_QCanvas* _iqcanvas;
    GCompound _gcompound;
    QImage* _backgroundImage;
    std::string _filename;   // file canvas was loaded from; "" if not loaded from a file

    friend class _Internal_QCanvas;

    void ensureBackgroundImage();

    void ensureBackgroundImageConstHack() const;

    void init(double width, double height, int rgbBackground, QWidget* parent);

    /**
     * Reads the canvas's pixel contents from the given stream.
     * @return true if loaded successfully and false if the load failed
     */
    virtual bool loadFromStream(std::istream& input);

    void notifyOfResize(double width, double height);
};


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QCanvas : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCanvas(GCanvas* gcanvas, QWidget* parent = nullptr);

    virtual void enterEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual void setCanvasSize(double width, double height);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
    GCanvas* _gcanvas;

    friend class GCanvas;
};

#endif // _gcanvas_h
