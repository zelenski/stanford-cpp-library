/*
 * File: qgbufferedimage.h
 * -----------------------
 * This file exports the QGBufferedImage class for per-pixel graphics.
 * See qgbufferedimage.cpp for implementation of each member.
 *
 * @author Marty Stepp
 * @version 2018/07/07
 * - initial version based on GBufferedImage source
 */

#ifndef _qgbufferedimage_h
#define _qgbufferedimage_h

#include <QLabel>
#include "grid.h"
#include "gtypes.h"
#include "qginteractor.h"
#include "qgobjects.h"

// default color used to highlight pixels that do not match between two images
#define QGBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

// forward declaration
class QGBufferedImage;

// Internal class; not to be used by clients.
class _Internal_QImageLabel : public QLabel, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QImageLabel(QGBufferedImage* image, QWidget* parent = nullptr);
    void repaint();

private:
    QGBufferedImage* _qgbufferedimage;

    friend class QGBufferedImage;
};

/*
 * This class implements a 2D region of colored pixels that can be read/set
 * individually, not unlike the <code>BufferedImage</code> class in Java.
 * The color of each pixel in a <code>QGBufferedImage</code> is represented
 * as a 32-bit integer as described by the <code>GWindow</code> function
 * <code>convertRGBToColor</code></a>, where:
 *
 * - bits 0-7 are unused;
 * - bits 8-15 represent the redness of the color, from 0 (none) through 255 (full);
 * - bits 16-23 represent the greenness of the color, from 0 (none) through 255 (full);
 * - and bits 24-31 represent the blueness of the color, from 0 (none) through 255 (full).
 *
 * For example, the integer <code>0xff00cc</code> is a purple color, while
 * <code>0x770000</code> is a dark red.
 * A <code>QGBufferedImage</code> will also let you represent colors as hex strings
 * such as <code>"#ff00cc"</code> instead of as integers.
 * String colors are always converted to integer colors internally by calling the
 * <code>GWindow</code> function <code>convertColorToRGB</code>.
 * Therefore some common color names like "black" can be passed successfully
 * in lieu of the usual hex format; see that function for more details.
 *
 * Though the x, y, width, and height parameters to the constructor and several
 * methods are received as type <code>double</code> for maximum compatibility
 * with other classes in this library, they should be thought of as
 * integer coordinates.  Any real numbers passed will be internally truncated
 * into integers by discarding any fractional component (by type-casting from
 * <code>double</code> to <code>int</code>).
 */
class QGBufferedImage : public QGInteractor {
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
     * Constructs an image with the specified size and optional background color.
     * You can also pass a filename instead to read the image from that file.
     * If no size is passed, the default size of 0x0 pixels is used.
     * If no background color is passed, the default of black (0x0) is used.
     * Throws an error if the given width/height ranges are negative.
     * Throws an error if the given rgb value is invalid or out of range.
     */
    QGBufferedImage(QWidget* parent = nullptr);
    QGBufferedImage(const std::string& filename, QWidget* parent = nullptr);
    QGBufferedImage(double width, double height, int rgbBackground = 0x000000, QWidget* parent = nullptr);
    QGBufferedImage(double width, double height, const std::string& rgbBackground, QWidget* parent = nullptr);
    virtual ~QGBufferedImage();

    /*
     * Sets all pixels to be the original background RGB passed to the constructor.
     */
    virtual void clear();

    /*
     * Returns the total number of pixels that are not the same color
     * between this image and the given other image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     * You can optionally pass an (x,y) range of pixels to compare; if none is
     * passed, the entire images are compared.
     */
    virtual int countDiffPixels(const QGBufferedImage& image) const;
    virtual int countDiffPixels(const QGBufferedImage& image, int xmin, int ymin, int xmax, int ymax) const;

    /*
     * Generates a new image whose content is equal to that of this image but with
     * any pixels that don't match those in parameter 'image' colored in the given
     * color (default purple) to highlight differences between the two.
     */
    virtual QGBufferedImage* diff(const QGBufferedImage& image, int diffPixelColor = QGBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR) const;

    /*
     * Returns true if the two given images contain exactly the same pixel data.
     */
    virtual bool equals(const QGBufferedImage& other) const;

    /*
     * Sets the color of every pixel in the image to the given color value.
     * Throws an error if the given rgb value is not a valid color.
     */
    virtual void fill(int rgb);
    virtual void fill(const std::string& rgb);

    /*
     * Sets the color of every pixel in the given rectangular range of the image
     * to the given color value.
     * Specifically, the pixels in the rectangular range (x, y) through
     * (x + width - 1, y + height - 1) become filled with the given color.
     * Throws an error if the given x/y/width/height range goes outside the bounds
     * of the image.
     * Throws an error if the given rgb value is not a valid color.
     */
    virtual void fillRegion(double x, double y, double width, double height, int rgb);
    virtual void fillRegion(double x, double y, double width, double height,
                            const std::string& rgb);

    /*
     * Replaces the entire contents of this image with the contents of the
     * given grid of RGB pixel values.
     * If this image is not the same size as the grid, the image is resized.
     * Any existing contents of the image are lost.
     */
    virtual void fromGrid(const Grid<int>& grid);

    /*
     * Returns the name of the file from which this image was loaded or
     * to which it was saved most recently.
     * If this image was not associated with any file, returns "".
     */
    virtual std::string getFilename() const;

    /*
     * Returns the color of the pixel at the given x/y coordinates of the image
     * as an integer such as 0xff00cc.
     * Throws an error if the given x/y values are out of bounds.
     */
    virtual int getPixel(double x, double y) const;
    virtual int getPixel(int x, int y) const;
    virtual int getRGB(double x, double y) const;
    virtual int getRGB(int x, int y) const;

    /*
     * Returns the color of the pixel at the given x/y coordinates of the image
     * as a string such as "#ff00cc".
     * The string that is returned comes from the <code>GWindow</code> function
     * <code>convertRGBToColor</code>; see documentation of that function.
     * Throws an error if the given x/y values are out of bounds.
     */
    virtual std::string getRGBString(double x, double y) const;

    virtual std::string getType() const;

    virtual QWidget* getWidget() const;

    virtual bool isAutoRepaint() const;

    /*
     * Reads the image's contents from the given image file.
     * Throws an error if the given file is not a valid image file.
     */
    virtual void load(const std::string& filename);

    void repaint();

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
     * Saves the image's contents to the given image file.
     * Throws an error if the given file is not writeable.
     */
    virtual void save(const std::string& filename);

    virtual void setAutoRepaint(bool autoRepaint);

    /*
     * Sets the color of the pixel at the given x/y coordinates of the image
     * to the given value.
     * Implementation/performance note: Each call to this method produces a
     * call to the Java graphical back-end.  Calling this method many times
     * in a tight loop can lead to poor performance.  If you need to fill a
     * large rectangular region, consider calling fill or fillRegion instead.
     * Throws an error if the given x/y values are out of bounds.
     * Throws an error if the given rgb value is not a valid color.
     */
    virtual void setPixel(double x, double y, int rgb);
    virtual void setPixel(double x, double y, const std::string& rgb);
    virtual void setRGB(double x, double y, int rgb);
    virtual void setRGB(double x, double y, const std::string& rgb);
    virtual void setRGB(int x, int y, int rgb);
    virtual void setRGB(int x, int y, const std::string& rgb);

    virtual void setSize(double width, double height);

    /*
     * Converts this image into a grid of RGB pixels.
     * The grid's first index is a row or y-index, and its second index
     * is the column or x-index.
     * So for example, grid[y][x] returns the RGB int value at that pixel.
     * The grid can either be returned or filled by reference.
     */
    virtual Grid<int> toGrid() const;
    virtual void toGrid(Grid<int>& grid) const;

    virtual std::string toString() const;

private:
    QImage* _qimage;
    _Internal_QImageLabel* _iqlabel;
    int _backgroundColor;
    std::string _filename;  // file image was loaded from; "" if not loaded from a file
    bool _autoRepaintFlag;

    /*
     * Throws an error if the given rgb value is not a valid color.
     */
    void checkColor(const std::string& member, int rgb) const;

    /*
     * Throws an error if the given x/y values are out of bounds.
     */
    void checkIndex(const std::string& member, double x, double y) const;
    void checkIndex(const std::string& member, int x, int y) const;

    /*
     * Throws an error if the given width/height values are out of bounds.
     */
    void checkSize(const std::string& member, double width, double height) const;

    void conditionalRepaint();

    /*
     * Initializes private member variables; called by all constructors.
     */
    void init(double width, double height, int rgb, QWidget* parent);

    friend class _Internal_QImageLabel;

    // allow operators to see private data inside image
    friend bool operator ==(const QGBufferedImage& img1, const QGBufferedImage& img2);
    friend bool operator !=(const QGBufferedImage& img1, const QGBufferedImage& img2);
};

bool operator ==(const QGBufferedImage& img1, const QGBufferedImage& img2);
bool operator !=(const QGBufferedImage& img1, const QGBufferedImage& img2);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgbufferedimage_h
