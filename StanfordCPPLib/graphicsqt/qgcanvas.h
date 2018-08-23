/*
 * File: qgcanvas.h
 * ----------------
 *
 * @version 2018/06/30
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgcanvas_h
#define _qgcanvas_h

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
#include "qgdrawingsurface.h"
#include "qgevent.h"
#include "qginteractor.h"
#include "qgobjects.h"

// default color used to highlight pixels that do not match between two images
#define QGCANVAS_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

// forward declaration
class QGCanvas;

// Internal class; not to be used by clients.
class _Internal_QCanvas : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCanvas(QGCanvas* qgcanvas, QWidget* parent = nullptr);

    virtual void enterEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
    QGCanvas* _qgcanvas;

    friend class QGCanvas;
};

/*
 * ...
 */
class QGCanvas : public virtual QGInteractor, public virtual QGDrawingSurface {
public:
    /*
     * Largest value that an image's width and/or height can have.
     * Error will be thrown if you try to make/resize an image larger than this.
     */
    static const int WIDTH_HEIGHT_MAX;

    /*
     * Creates a single RGB integer from the given R-G-B components from 0-255.
     */
    static int createRgbPixel(int red, int green, int blue);

    /*
     * Extracts the alpha component from 0-255 of the given ARGB integer.
     * The alpha component comes from bits 24-31 (most significant) of the integer.
     */
    static int getAlpha(int argb);

    /*
     * Extracts the blue component from 0-255 of the given RGB integer.
     * The blue component comes from bits 0-7 (least significant) of the integer.
     */
    static int getBlue(int rgb);

    /*
     * Extracts the green component from 0-255 of the given RGB integer.
     * The green component comes from bits 8-15 of the integer.
     */
    static int getGreen(int rgb);

    /*
     * Extracts the red component from 0-255 of the given RGB integer.
     * The red component comes from bits 16-23 of the integer.
     */
    static int getRed(int rgb);

    /*
     * Extracts the red, green, and blue components from 0-255
     * of the given RGB integer, filling by reference.
     */
    static void getRedGreenBlue(int rgb, int& red, int& green, int& blue);

    /*
     * Constructs a canvas with the specified size and optional background color.
     * You can also pass a filename instead to read an image from that file
     * and draw it onto the canvas's pixel background.
     * If no size is passed, the default size of 0x0 pixels is used.
     * If no background color is passed, the default of black (0x0) is used.
     * Throws an error if the given width/height ranges are negative.
     * Throws an error if the given rgb value is invalid or out of range.
     */
    QGCanvas(QWidget* _parent = nullptr);
    QGCanvas(const std::string& filename, QWidget* parent = nullptr);
    QGCanvas(double width, double height, int rgbBackground = 0x000000, QWidget* parent = nullptr);
    QGCanvas(double width, double height, const std::string& rgbBackground, QWidget* parent = nullptr);
    virtual ~QGCanvas();

    // QGCompound methods
    virtual void add(QGObject* gobj);
    virtual void add(QGObject* gobj, double x, double y);
    virtual void add(QGObject& gobj);
    virtual void add(QGObject& gobj, double x, double y);
    virtual void clear() Q_DECL_OVERRIDE;
    virtual void clearObjects();
    virtual void clearPixels();
    virtual bool contains(double x, double y) const;

    /*
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * You can optionally pass an (x,y) range of pixels to compare; if none is
     * passed, the entire images are compared.
     */
    virtual int countDiffPixels(const QGCanvas& image) const;
    virtual int countDiffPixels(const QGCanvas& image, int xmin, int ymin, int xmax, int ymax) const;
    virtual int countDiffPixels(const QGCanvas* image) const;
    virtual int countDiffPixels(const QGCanvas* image, int xmin, int ymin, int xmax, int ymax) const;

    /*
     * Generates a new canvas whose content is equal to that of this canvas but with
     * any pixels that don't match those in parameter 'image' colored in the given
     * color (default purple) to highlight differences between the two.
     */
    virtual QGCanvas* diff(const QGCanvas& image, int diffPixelColor = QGCANVAS_DEFAULT_DIFF_PIXEL_COLOR) const;
    virtual QGCanvas* diff(const QGCanvas* image, int diffPixelColor = QGCANVAS_DEFAULT_DIFF_PIXEL_COLOR) const;

    virtual void draw(QGObject* gobj) Q_DECL_OVERRIDE;
    virtual void draw(QPainter* painter) Q_DECL_OVERRIDE;

    /*
     * Returns true if the two given canvases contain exactly the same pixel data.
     */
    virtual bool equals(const QGCanvas& other) const;

    /*
     * Sets the color of every pixel in the canvas to the given color value.
     * Throws an error if the given rgb value is not a valid color.
     */
    virtual void fill(int rgb);
    virtual void fill(const std::string& rgb);

    /*
     * Sets the color of every pixel in the given rectangular range of the canvas
     * pixel data to the given color value.
     * Specifically, the pixels in the rectangular range (x, y) through
     * (x + width - 1, y + height - 1) become filled with the given color.
     * Throws an error if the given x/y/width/height range goes outside the bounds
     * of the image.
     * Throws an error if the given rgb value is not a valid color.
     */
    virtual void fillRegion(double x, double y, double width, double height, int rgb);
    virtual void fillRegion(double x, double y, double width, double height,
                            const std::string& rgb);

    virtual void flatten();

    /*
     * Replaces the entire contents of this canvas with the contents of the
     * given grid of RGB pixel values.
     * If this image is not the same size as the grid, the image is resized.
     * Any existing contents of the image are lost.
     */
    virtual void fromGrid(const Grid<int>& grid);

    virtual std::string getBackground() const Q_DECL_OVERRIDE;
    virtual int getBackgroundInt() const Q_DECL_OVERRIDE;
    virtual QGObject* getElement(int index) const;
    virtual QGObject* getElementAt(double x, double y) const;
    virtual int getElementCount() const;


    /*
     * Returns the name of the image file from which this canvas was loaded or
     * to which it was saved most recently.
     * If this canvas was not associated with any file, returns "".
     */
    virtual std::string getFilename() const;

    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /*
     * Returns the color of the pixel at the given x/y coordinates of the image
     * as an integer such as 0xff00cc.
     * Throws an error if the given x/y values are out of bounds.
     */
    virtual int getPixel(double x, double y) const Q_DECL_OVERRIDE;
    virtual int getPixelARGB(double x, double y) const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixels() const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixelsARGB() const Q_DECL_OVERRIDE;

    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isAutoRepaint() const Q_DECL_OVERRIDE;

    /*
     * Reads the canvas's pixel contents from the given image file.
     * Throws an error if the given file is not a valid image file.
     */
    virtual void load(const std::string& filename);

    virtual void remove(QGObject* gobj);
    virtual void remove(QGObject& gobj);
    virtual void removeAll();
    virtual void removeClickListener();
    virtual void removeKeyListener();
    virtual void removeMouseListener();
    virtual void repaint() Q_DECL_OVERRIDE;
    virtual void repaintRegion(int x, int y, int width, int height) Q_DECL_OVERRIDE;

    /*
     * Changes this image's bounds to be the given size.
     * This does not scale the image but rather just changes the max x/y that
     * can be painted onto this image.
     * If the 'retain' parameter is not passed or is set to true,
     * any existing pixel values will be kept during the resize.
     * If 'retain' is false, the contents will be wiped and set to the default.
     * Any existing pixel contents are discarded and revert to defaults.
     * Throws an error if the given width/height ranges are negative.
     */
    virtual void resize(double width, double height, bool retain = true);

    /*
     * Saves the canvas's contents to the given image file.
     * Throws an error if the given file is not writeable.
     */
    virtual void save(const std::string& filename);

    virtual void setAutoRepaint(bool autoRepaint) Q_DECL_OVERRIDE;
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setClickListener(QGEventListener func);
    virtual void setClickListener(QGEventListenerVoid func);
    virtual void setColor(int color) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setKeyListener(QGEventListener func);
    virtual void setKeyListener(QGEventListenerVoid func);
    virtual void setMouseListener(QGEventListener func);
    virtual void setMouseListener(QGEventListenerVoid func);
    virtual void setPixel(double x, double y, int rgb) Q_DECL_OVERRIDE;
    virtual void setPixel(double x, double y, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int argb) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixels(const Grid<int>& pixels) Q_DECL_OVERRIDE;
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) Q_DECL_OVERRIDE;

    /*
     * Converts this canvas's pixel data into a grid of RGB pixels.
     * The grid's first index is a row or y-index, and its second index
     * is the column or x-index.
     * So for example, grid[y][x] returns the RGB int value at that pixel.
     * The grid can either be returned or filled by reference.
     */
    virtual Grid<int> toGrid() const;
    virtual void toGrid(Grid<int>& grid) const;

private:
    _Internal_QCanvas* _iqcanvas;
    QGCompound _qgcompound;
    QImage* _backgroundImage;
    std::string _filename;   // file canvas was loaded from; "" if not loaded from a file

    friend class _Internal_QCanvas;

    void ensureBackgroundImage();
    void ensureBackgroundImageConstHack() const;
    void init(double width, double height, int rgbBackground, QWidget* parent);
    void notifyOfResize(double width, double height);
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
#endif // SPL_QT_GUI
