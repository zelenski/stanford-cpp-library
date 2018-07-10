/*
 * File: qgbufferedimage.cpp
 * -------------------------
 * This file implements the qgbufferedimage.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2018/07/07
 * - initial version
 */

#include "qgbufferedimage.h"
#include <cstring>
#include "filelib.h"
#include "gmath.h"
#include "qgcolor.h"
#include "strlib.h"

_Internal_QImageLabel::_Internal_QImageLabel(QGBufferedImage* image, QWidget* parent)
        : QLabel(parent),
          _qgbufferedimage(image) {
    // empty
}

void _Internal_QImageLabel::repaint() {
    if (_qgbufferedimage->_qimage) {
        setPixmap(QPixmap::fromImage(*_qgbufferedimage->_qimage));
    }
}

#define CHAR_TO_HEX(ch) ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10))

const int QGBufferedImage::WIDTH_HEIGHT_MAX = 65535;

int QGBufferedImage::createRgbPixel(int red, int green, int blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("RGB values must be between 0-255");
    }
    return ((red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff)) & 0xffffff;
}

int QGBufferedImage::getAlpha(int argb) {
    // have to & a second time because of sign-extension on >> shift
    return ((argb & 0xff000000) >> 24) & 0x000000ff;
}

int QGBufferedImage::getBlue(int rgb) {
    return rgb & 0x0000ff;
}

int QGBufferedImage::getGreen(int rgb) {
    return (rgb & 0x00ff00) >> 8;
}

int QGBufferedImage::getRed(int rgb) {
    return (rgb & 0xff0000) >> 16;
}

void QGBufferedImage::getRedGreenBlue(int rgb, int& red, int& green, int& blue) {
    red = getRed(rgb);
    green = getGreen(rgb);
    blue = getBlue(rgb);
}

QGBufferedImage::QGBufferedImage(QWidget* parent)
        : _qimage(),
          _backgroundColor(0),
          _filename(""),
          _autoRepaintFlag(true) {
    init(/* width */ 1, /* height */ 1, 0x000000, parent);
}

QGBufferedImage::QGBufferedImage(const std::string& filename,
                                 QWidget* parent)
    : _qimage(),
      _backgroundColor(0),
      _filename(filename),
      _autoRepaintFlag(true) {
    init(/* width */ 1, /* height */ 1, 0x000000, parent);
    load(filename);
}

QGBufferedImage::QGBufferedImage(double width,
                                 double height,
                                 int rgbBackground,
                                 QWidget* parent)
    : _qimage(),
      _backgroundColor(rgbBackground),
      _filename(""),
      _autoRepaintFlag(true) {
    init(width, height, rgbBackground, parent);
}

QGBufferedImage::QGBufferedImage(double width,
                                 double height,
                                 const std::string& rgbBackground,
                                 QWidget* parent)
    : _qimage(),
      _backgroundColor(0),
      _filename(""),
      _autoRepaintFlag(true) {
    init(width, height, QGColor::convertColorToRGB(rgbBackground), parent);
}

QGBufferedImage::~QGBufferedImage() {
    // TODO: delete?
    _qimage = nullptr;
    _iqlabel = nullptr;
}

void QGBufferedImage::checkColor(const std::string& /* member */, int /* rgb */) const {
    // I think this code is wrong; it ignores the possibility of alpha values
    // or of the top bits being set to 255 (all 1) by default by Qt libraries
//    if (rgb < 0x0 || rgb > 0xffffff) {
//        error("QGBufferedImage::" + member
//              + ": color is outside of range 0x000000 through 0xffffff");
//    }
}

void QGBufferedImage::checkIndex(const std::string& member, double x, double y) const {
    if (!inBounds(x, y)) {
        error("QGBufferedImage::" + member
              + ": (x=" + integerToString((int) x)
              + ", y=" + integerToString((int) y)
              + ") is out of valid range of (0, 0) through ("
              + integerToString((int) getWidth() - 1) + ", "
              + integerToString((int) getHeight() - 1) + ")");
    }
}

void QGBufferedImage::checkIndex(const std::string& member, int x, int y) const {
    if (!inBounds(x, y)) {
        error("QGBufferedImage::" + member
              + ": (x=" + integerToString((int) x)
              + ", y=" + integerToString((int) y)
              + ") is out of valid range of (0, 0) through ("
              + integerToString((int) getWidth() - 1) + ", "
              + integerToString((int) getHeight() - 1) + ")");
    }
}

void QGBufferedImage::checkSize(const std::string& member, double width, double height) const {
    if (width < 0 || height < 0) {
        error("QGBufferedImage::" + member + ": width/height cannot be negative");
    }
    if (width > QGBufferedImage::WIDTH_HEIGHT_MAX
            || height > QGBufferedImage::WIDTH_HEIGHT_MAX) {
        error("QGBufferedImage::" + member + ": width/height too large (cannot exceed "
              + integerToString(WIDTH_HEIGHT_MAX));
    }
}

void QGBufferedImage::clear() {
    fill(_backgroundColor);
}

void QGBufferedImage::conditionalRepaint() {
    if (_autoRepaintFlag) {
        repaint();
    }
}

int QGBufferedImage::countDiffPixels(const QGBufferedImage& image) const {
    int w1 = (int) getWidth();
    int h1 = (int) getHeight();
    int w2 = (int) image.getWidth();
    int h2 = (int) image.getHeight();

    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);

    int overlap = std::min(w1, w2) * std::min(h1, h2);
    int diffPxCount = (w1 * h1 - overlap) + (w2 * h2 - overlap);

    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = getRGB(x, y);
            int px2 = image.getRGB(x, y);
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    return diffPxCount;
}

int QGBufferedImage::countDiffPixels(const QGBufferedImage& image, int xmin, int ymin, int xmax, int ymax) const {
    int w1 = (int) getWidth();
    int h1 = (int) getHeight();
    int w2 = (int) image.getWidth();
    int h2 = (int) image.getHeight();
    int diffPxCount = 0;

    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            int px1 = x < w1 && y < h1 ? getRGB(x, y) : -1;
            int px2 = x < w2 && y < h2 ? image.getRGB(x, y) : -1;
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    return diffPxCount;
}

int QGBufferedImage::countDiffPixels(const QGBufferedImage* image) const {
    return countDiffPixels(*image);
}

int QGBufferedImage::countDiffPixels(const QGBufferedImage* image, int xmin, int ymin, int xmax, int ymax) const {
    return countDiffPixels(*image, xmin, ymin, xmax, ymax);
}

QGBufferedImage* QGBufferedImage::diff(const QGBufferedImage& image, int diffPixelColor) const {
    int w1 = (int) getWidth();
    int h1 = (int) getHeight();
    int w2 = (int) image.getWidth();
    int h2 = (int) image.getHeight();
    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);
    int wmax = std::max(w1, w2);
    int hmax = std::max(h1, h2);

    Grid<int> resultGrid;
    resultGrid.resize(hmax, wmax);
    resultGrid.fill(diffPixelColor);
    for (int r = 0; r < h1; r++) {
        for (int c = 0; c < w1; c++) {
            resultGrid[r][c] = _backgroundColor;
        }
    }
    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = getRGB(x, y);
            int px2 = image.getRGB(x, y);
            if (px1 != px2) {
                resultGrid[y][x] = diffPixelColor;
            }
        }
    }
    QGBufferedImage* result = new QGBufferedImage(wmax, hmax);
    result->fromGrid(resultGrid);
    return result;
}

QGBufferedImage* QGBufferedImage::diff(const QGBufferedImage* image, int diffPixelColor) const {
    return diff(*image, diffPixelColor);
}

bool QGBufferedImage::equals(const QGBufferedImage& other) const {
    if (getSize() != other.getSize()) {
        return false;
    }
    // TODO: compare pixels
    return true;
}

void QGBufferedImage::fill(int rgb) {
    checkColor("fill", rgb);
    // TODO: for each row/col, setPixel
}

void QGBufferedImage::fill(const std::string& rgb) {
    fill(QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::fillRegion(double x, double y, double width, double height, int rgb) {
    checkIndex("fillRegion", x, y);
    checkIndex("fillRegion", x + width - 1, y + height - 1);
    checkColor("fillRegion", rgb);
    for (int r = (int) y; r < y + height; r++) {
        for (int c = (int) x; c < x + width; c++) {
            setRGB(/* x */ c, /* y */ r, rgb);
        }
    }
    // TODO: repaint?
}

void QGBufferedImage::fillRegion(double x, double y, double width, double height, const std::string& rgb) {
    fillRegion(x, y, width, height, QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::fromGrid(const Grid<int>& grid) {
    checkSize("fromGrid", grid.width(), grid.height());
    setSize(grid.width(), grid.height());

    // TODO: for each row/col, setRGB
    for (int row = 0, width = grid.width(), height = grid.height(); row < height; row++) {
        for (int col = 0; col < width; col++) {
            setPixel(col, row, grid[row][col]);
        }
    }

    conditionalRepaint();
}

std::string QGBufferedImage::getFilename() const {
    return _filename;
}

int QGBufferedImage::getPixel(double x, double y) const {
    checkIndex("getPixel", x, y);
    return _qimage->pixel((int) x, (int) y);
}

int QGBufferedImage::getPixel(int x, int y) const {
    checkIndex("getPixel", x, y);
    return _qimage->pixel(x, y);
}

int QGBufferedImage::getRGB(double x, double y) const {
    checkIndex("getRGB", x, y);
    return _qimage->pixel((int) x, (int) y);
}

int QGBufferedImage::getRGB(int x, int y) const {
    checkIndex("getRGB", x, y);
    return _qimage->pixel(x, y);
}

std::string QGBufferedImage::getRGBString(double x, double y) const {
    return QGColor::convertRGBToColor(getRGB(x, y));
}

std::string QGBufferedImage::getType() const {
    return "QGBufferedImage";
}

QWidget* QGBufferedImage::getWidget() const {
    return _iqlabel;
}

void QGBufferedImage::init(double width, double height, int rgb, QWidget* parent) {
    checkSize("constructor", width, height);
    checkColor("constructor", rgb);
    _iqlabel = new _Internal_QImageLabel(this, getInternalParent(parent));
    setSize(width, height);

    _backgroundColor = rgb;
    if (width > 0 && height > 0) {
        if (rgb != 0) {
            fill(rgb);
        }
    }
}

bool QGBufferedImage::isAutoRepaint() const {
    return _autoRepaintFlag;
}

void QGBufferedImage::load(const std::string& filename) {
    ensureThreadSafety("QGBufferedImage::load");   // TODO: call runOnQtGuiThread?

    // for efficiency, let's at least check whether the file exists
    // and throw error immediately rather than contacting the back-end
    if (!fileExists(filename)) {
        error("QGBufferedImage::load: file not found: " + filename);
    }
    if (!_qimage->load(QString::fromStdString(filename))) {
        error("QGBufferedImage::load: failed to load from " + filename);
    }
    _filename = filename;
    QGInteractor::setSize(_qimage->width(), _qimage->height());
    // setSize(_qimage->width(), _qimage->height());
    conditionalRepaint();
}

void QGBufferedImage::resize(double width, double height, bool /* retain */) {
    checkSize("resize", width, height);

    // TODO
    setSize(width, height);

//    bool wasZero = (floatingPointEqual(_width, 0)
//                    && floatingPointEqual(_height, 0));
//    _width = width;
//    _height = height;
//    if (wasZero) {
//        if (width > 0 && height > 0) {
//            init(_width, _height, _backgroundColor);
//        } // else still zero
//    } else {
//        // was non-zero
//        _pixels.resize((int) _height, (int) _width, retain);
//        // stanfordcpplib::getPlatform()->qgbufferedimage_resize(this, width, height, retain);
//        if (!retain && _backgroundColor != 0x0) {
//            _pixels.fill(_backgroundColor);
//        }
//    }

    conditionalRepaint();
}

void QGBufferedImage::repaint() {
    _iqlabel->repaint();
}

void QGBufferedImage::save(const std::string& filename) {
    ensureThreadSafety("QGBufferedImage::save");   // TODO: call runOnQtGuiThread?
    if (!_qimage->save(QString::fromStdString(filename))) {
        error("QGBufferedImage::save: failed to save to " + filename);
    }
    _filename = filename;
}

void QGBufferedImage::setAutoRepaint(bool autoRepaint) {
    _autoRepaintFlag = autoRepaint;
}

void QGBufferedImage::setPixel(double x, double y, int rgb) {
    checkIndex("setPixel", x, y);
    checkColor("setPixel", rgb);
    _qimage->setPixel((int) x, (int) y, rgb);
    conditionalRepaint();
}

void QGBufferedImage::setPixel(double x, double y, const std::string& rgb) {
    setPixel(x, y, QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::setPixel(int x, int y, int rgb) {
    checkIndex("setPixel", x, y);
    checkColor("setPixel", rgb);
    _qimage->setPixel(x, y, rgb);
    conditionalRepaint();
}

void QGBufferedImage::setPixel(int x, int y, const std::string& rgb) {
    setPixel(x, y, QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::setRGB(double x, double y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    _qimage->setPixel((int) x, (int) y, rgb);
    conditionalRepaint();
}

void QGBufferedImage::setRGB(double x, double y, const std::string& rgb) {
    setRGB(x, y, QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::setRGB(int x, int y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    _qimage->setPixel(x, y, rgb);
    conditionalRepaint();
}

void QGBufferedImage::setRGB(int x, int y, const std::string& rgb) {
    setRGB(x, y, QGColor::convertColorToRGB(rgb));
}

void QGBufferedImage::setSize(double width, double height) {
    if (_qimage) {
        // TODO: delete _qimage;
    }
    _qimage = new QImage((int) width, (int) height, QImage::Format_RGB32);
    QGInteractor::setSize(width, height);
    conditionalRepaint();
}

Grid<int> QGBufferedImage::toGrid() const {
    Grid<int> grid;
    toGrid(grid);
    return grid;
}

void QGBufferedImage::toGrid(Grid<int>& grid) const {
    grid.resize(getHeight(), getWidth());
    for (int row = 0, width = (int) getWidth(), height = (int) getHeight(); row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = _qimage->pixel(col, row);
        }
    }
}

std::string QGBufferedImage::toString() const {
    return "QGBufferedImage()";
}

bool operator ==(const QGBufferedImage& img1, const QGBufferedImage& img2) {
    return img1.equals(img2);
}

bool operator !=(const QGBufferedImage& img1, const QGBufferedImage& img2) {
    return !img1.equals(img2);
}
