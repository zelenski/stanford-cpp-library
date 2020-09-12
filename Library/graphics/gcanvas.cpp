/*
 * File: gcanvas.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2019/05/01
 * - added createArgbPixel
 * - bug fixes related to save / setPixels with alpha transparency
 * @version 2019/04/23
 * - bug fix for loading canvas from file on Windows related to istream change
 * - moved most event listener code to GInteractor superclass
 * @version 2019/03/07
 * - added support for loading canvas directly from istream (htiek)
 * @version 2019/02/06
 * - fixed mouse wheel listeners to work even if no actual scroll area exists
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/09/20
 * - added read/write lock for canvas contents to avoid race conditions
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcanvas.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#include "gcanvas.h"
#include "gcolor.h"
#include "gthread.h"
#include "gwindow.h"
#include "error.h"
#include "filelib.h"
#include "require.h"
#include "strlib.h"

const int GCanvas::WIDTH_HEIGHT_MAX = 65535;

int GCanvas::createArgbPixel(int alpha, int red, int green, int blue) {
    if (alpha < 0 || alpha > 255 || red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("ARGB values must be between 0-255");
    }
    return (alpha << 24 & 0xff000000) | (red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff);
}

int GCanvas::createRgbPixel(int red, int green, int blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("RGB values must be between 0-255");
    }
    return ((red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff)) & 0xffffff;
}

int GCanvas::getAlpha(int argb) {
    // have to & a second time because of sign-extension on >> shift
    return ((static_cast<unsigned int>(argb) & 0xff000000) >> 24) & 0x000000ff;
}

int GCanvas::getBlue(int rgb) {
    return rgb & 0x0000ff;
}

int GCanvas::getGreen(int rgb) {
    return (rgb & 0x00ff00) >> 8;
}

int GCanvas::getRed(int rgb) {
    return (rgb & 0xff0000) >> 16;
}

void GCanvas::getRedGreenBlue(int rgb, int& red, int& green, int& blue) {
    red = getRed(rgb);
    green = getGreen(rgb);
    blue = getBlue(rgb);
}


GCanvas::GCanvas(QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(/* width */ -1, /* height */ -1, /* background */ 0xffffff, parent);
}

GCanvas::GCanvas(const std::string& filename, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename(filename) {
    init(/* width */ -1, /* height */ -1, /* background */ 0xffffff, parent);
    load(filename);
}

GCanvas::GCanvas(std::istream& source, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("std::istream data") {
    init(/* width */ -1, /* height */ -1, /* background */ 0xffffff, parent);
    if (!loadFromStream(source)) {
        error("GCanvas::constructor: could not load image from input stream");
    }
}

GCanvas::GCanvas(double width, double height, int rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(width, height, rgbBackground, parent);
}

GCanvas::GCanvas(double width, double height, const std::string& rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    _backgroundColor = rgbBackground;
    init(width, height,
         GColor::hasAlpha(rgbBackground)
                ? GColor::convertColorToARGB(rgbBackground)
                : GColor::convertColorToRGB(rgbBackground),
         parent);
}

void GCanvas::init(double width, double height, int rgbBackground, QWidget* parent) {
    checkSize("GCanvas::constructor", width, height);
    checkColor("GCanvas::constructor", rgbBackground);

    GThread::runOnQtGuiThread([this, rgbBackground, parent]() {
        _iqcanvas = new _Internal_QCanvas(this, getInternalParent(parent));
        _gcompound.setWidget(_iqcanvas);
        int alpha = getAlpha(rgbBackground);
        if (GColor::hasAlpha(_backgroundColor)) {
            // empty
        } else if (alpha > 0 && alpha < 255) {
            _backgroundColor = GColor::convertARGBToColor(rgbBackground);
        } else {
            _backgroundColor = GColor::convertRGBToColor(rgbBackground);
        }
        _backgroundColorInt = rgbBackground;
    });

    if (width > 0 && height > 0) {
        setSize(width, height);
        if (rgbBackground != 0) {
            fill(rgbBackground);
        }
    }

    setVisible(false);   // all widgets are not shown until added to a window
}

GCanvas::~GCanvas() {
    // TODO: delete _iqcanvas;
    _iqcanvas->detach();
    _iqcanvas = nullptr;
}

void GCanvas::add(GObject* gobj) {
    GThread::runOnQtGuiThread([this, gobj]() {
        lockForWrite();
        _gcompound.add(gobj);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject* gobj, double x, double y) {
    GThread::runOnQtGuiThread([this, gobj, x, y]() {
        lockForWrite();
        _gcompound.add(gobj, x, y);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject& gobj) {
    GThread::runOnQtGuiThread([this, &gobj]() {
        lockForWrite();
        _gcompound.add(gobj);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject& gobj, double x, double y) {
    GThread::runOnQtGuiThread([this, &gobj, x, y]() {
        lockForWrite();
        _gcompound.add(gobj, x, y);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::clear() {
    clearObjects();
    clearPixels();   // calls conditionalRepaint
}

void GCanvas::clearObjects() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _gcompound.clear();   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::clearPixels() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        if (_backgroundImage) {
            // delete _backgroundImage;
            // _backgroundImage = nullptr;
            // keep background image buffer but fill with background color instead
            GThread::runOnQtGuiThread([this]() {
                if (GColor::hasAlpha(_backgroundColor)) {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            });
        }
        unlock();
    });
    conditionalRepaint();
}

bool GCanvas::contains(double x, double y) const {
    lockForReadConst();
    bool result = _gcompound.contains(x, y);
    unlockConst();
    return result;
}

int GCanvas::countDiffPixels(const GCanvas& image) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());

    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);

    int overlap = std::min(w1, w2) * std::min(h1, h2);
    int diffPxCount = (w1 * h1 - overlap) + (w2 * h2 - overlap);

    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            int px2 = image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    unlockConst();
    return diffPxCount;
}

int GCanvas::countDiffPixels(const GCanvas& image, int xmin, int ymin, int xmax, int ymax) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());
    int diffPxCount = 0;

    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            int px1 = x < w1 && y < h1 ? (_backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff) : -1;
            int px2 = x < w2 && y < h2 ? (image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff) : -1;
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    unlockConst();
    return diffPxCount;
}

int GCanvas::countDiffPixels(const GCanvas* image) const {
    require::nonNull(image, "GCanvas::countDiffPixels");
    return countDiffPixels(*image);
}

int GCanvas::countDiffPixels(const GCanvas* image, int xmin, int ymin, int xmax, int ymax) const {
    require::nonNull(image, "GCanvas::countDiffPixels");
    return countDiffPixels(*image, xmin, ymin, xmax, ymax);
}

GCanvas* GCanvas::diff(const GCanvas& image, int diffPixelColor) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());
    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);
    int wmax = std::max(w1, w2);
    int hmax = std::max(h1, h2);

    Grid<int> resultGrid;
    resultGrid.resize(hmax, wmax);
    resultGrid.fill(diffPixelColor);
    for (int r = 0; r < h1; r++) {
        for (int c = 0; c < w1; c++) {
            resultGrid[r][c] = _backgroundColorInt;
        }
    }
    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            int px2 = image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            if (px1 != px2) {
                resultGrid[y][x] = diffPixelColor;
            }
        }
    }
    GCanvas* result = new GCanvas(wmax, hmax);
    result->fromGrid(resultGrid);
    unlockConst();
    return result;
}

GCanvas* GCanvas::diff(const GCanvas* image, int diffPixelColor) const {
    require::nonNull(image, "GCanvas::diff");
    return diff(*image, diffPixelColor);
}

void GCanvas::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    // lockForRead();
    if (_backgroundImage) {
        painter->drawImage(/* x */ 0, /* y */ 0, *_backgroundImage);
    }
    _gcompound.draw(painter);   // calls conditionalRepaint
    // unlock();
}

void GCanvas::draw(GObject* gobj) {
    require::nonNull(gobj, "GCanvas::draw");
    ensureBackgroundImage();
    if (_backgroundImage && _backgroundImage->paintEngine()) {
        GThread::runOnQtGuiThread([this, gobj]() {
            lockForWrite();
            if (gobj->isVisible()) {
                QPainter painter(_backgroundImage);
                painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
                painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
                gobj->draw(&painter);
                painter.end();
            }
            unlock();
        });
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCanvas::ensureBackgroundImage() {
    if (!_backgroundImage) {
        GThread::runOnQtGuiThread([this]() {
            lockForWrite();
            _backgroundImage = new QImage(
                        static_cast<int>(getWidth()),
                        static_cast<int>(getHeight()),
                        QImage::Format_ARGB32);
            if (!_backgroundColor.empty()) {
                if (GColor::hasAlpha(_backgroundColor)) {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            }
            unlock();
        });
    }
}

void GCanvas::ensureBackgroundImageConstHack() const {
    if (!_backgroundImage) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        GCanvas* hack = const_cast<GCanvas*>(this);
        hack->ensureBackgroundImage();
    }
}

bool GCanvas::equals(const GCanvas& other) const {
    if (getSize() != other.getSize()) {
        return false;
    }
    // TODO: compare pixels
    return true;
}

void GCanvas::fill(int rgb) {
    checkColor("GCanvas::fill", rgb);
    fillRegion(/* x */ 0,
               /* y */ 0,
               /* width */ getWidth(),
               /* height */ getHeight(),
               rgb);
}

void GCanvas::fill(const std::string& rgb) {
    fill(GColor::convertColorToRGB(rgb));
}

void GCanvas::fillRegion(double x, double y, double width, double height, int rgb) {
    checkBounds("GCanvas::fillRegion", x, y, getWidth(), getHeight());
    checkBounds("GCanvas::fillRegion", x + width - 1, y + height - 1, getWidth(), getHeight());
    checkColor("GCanvas::fillRegion", rgb);
    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);
    GThread::runOnQtGuiThread([this, x, y, width, height, rgb]() {
        ensureBackgroundImage();
        lockForWrite();
        int argb = rgb | 0xff000000;
        for (int yy = static_cast<int>(y); yy < y + height; yy++) {
            for (int xx = static_cast<int>(x); xx < x + width; xx++) {
                _backgroundImage->setPixel(xx, yy, static_cast<unsigned int>(argb));
            }
        }
        unlock();
    });
    setAutoRepaint(wasAutoRepaint);
    conditionalRepaint();
}

void GCanvas::fillRegion(double x, double y, double width, double height, const std::string& rgb) {
    fillRegion(x, y, width, height, GColor::convertColorToRGB(rgb));
}

void GCanvas::flatten() {
    GThread::runOnQtGuiThread([this]() {
        ensureBackgroundImage();
        lockForWrite();
        QPainter painter(_backgroundImage);
        painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
        painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
        _gcompound.draw(&painter);
        painter.end();
        _gcompound.clear();   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::fromGrid(const Grid<int>& grid) {
    checkSize("GCanvas::fromGrid", grid.numCols(), grid.numRows());
    setSize(grid.numCols(), grid.numRows());

    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);

    GThread::runOnQtGuiThread([this, &grid]() {
        ensureBackgroundImage();
        lockForWrite();
        for (int row = 0, width = grid.numCols(), height = grid.numRows(); row < height; row++) {
            for (int col = 0; col < width; col++) {
                int argb = GColor::fixAlpha(grid[row][col]);
                _backgroundImage->setPixel(col, row, static_cast<unsigned int>(argb));
            }
        }
        unlock();
    });

    if (wasAutoRepaint) {
        setAutoRepaint(wasAutoRepaint);
        conditionalRepaint();
    }
}

std::string GCanvas::getBackground() const {
    return GDrawingSurface::getBackground();
}

int GCanvas::getBackgroundInt() const {
    return GDrawingSurface::getBackgroundInt();
}

GObject* GCanvas::getElement(int index) const {
    lockForReadConst();
    GObject* result = _gcompound.getElement(index);
    unlockConst();
    return result;
}

GObject* GCanvas::getElementAt(double x, double y) const {
    lockForReadConst();
    GObject* result = _gcompound.getElementAt(x, y);
    unlockConst();
    return result;
}

int GCanvas::getElementCount() const {
    lockForReadConst();
    int result = _gcompound.getElementCount();
    unlockConst();
    return result;
}

std::string GCanvas::getFilename() const {
    return _filename;
}

std::string GCanvas::getFont() const {
    return GDrawingSurface::getFont();
}

_Internal_QWidget* GCanvas::getInternalWidget() const {
    return _iqcanvas;
}

int GCanvas::getPixel(double x, double y) const {
    checkBounds("GCanvas::getPixel", x, y, getWidth(), getHeight());
    ensureBackgroundImageConstHack();
    lockForReadConst();
    int pixel = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
    unlockConst();
    return pixel;
}

int GCanvas::getPixelARGB(double x, double y) const {
    checkBounds("GCanvas::getPixelARGB", x, y, getWidth(), getHeight());
    ensureBackgroundImageConstHack();
    lockForReadConst();
    int pixel = static_cast<int>(_backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)));
    unlockConst();
    return pixel;
}

Grid<int> GCanvas::getPixels() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    int w = static_cast<int>(getWidth());
    int h = static_cast<int>(getHeight());
    Grid<int> grid(h, w);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            grid[y][x] = _backgroundImage->pixel(x, y) & 0x00ffffff;
        }
    }
    unlockConst();
    return grid;
}

Grid<int> GCanvas::getPixelsARGB() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    Grid<int> grid(static_cast<int>(getHeight()), static_cast<int>(getWidth()));
    for (int y = 0; y < static_cast<int>(getHeight()); y++) {
        for (int x = 0; x < static_cast<int>(getWidth()); x++) {
            grid[y][x] = static_cast<int>(_backgroundImage->pixel(x, y));
        }
    }
    unlockConst();
    return grid;
}

std::string GCanvas::getType() const {
    return "GCanvas";
}

QWidget* GCanvas::getWidget() const {
    return static_cast<QWidget*>(_iqcanvas);
}

bool GCanvas::isAutoRepaint() const {
    return _gcompound.isAutoRepaint();
}

void GCanvas::load(const std::string& filename) {
    // for efficiency, let's at least check whether the file exists
    // and throw error immediately rather than contacting the back-end
    if (!fileExists(filename)) {
        error("GCanvas::load: file not found: " + filename);
    }

    bool hasError = false;
    GThread::runOnQtGuiThread([this, filename, &hasError]() {
        ensureBackgroundImage();
        lockForWrite();
        if (!_backgroundImage->load(QString::fromStdString(filename))) {
            hasError = true;
            unlock();
            return;
        }

        _filename = filename;
        GInteractor::setSize(_backgroundImage->width(), _backgroundImage->height());
        // setSize(_qimage->width(), _qimage->height());
        unlock();
        conditionalRepaint();
    });

    if (hasError) {
        error("GCanvas::load: failed to load from " + filename);
    }
}

bool GCanvas::loadFromStream(std::istream& input) {
    // buffer bytes into a std::string
    std::ostringstream byteStream;
    byteStream << input.rdbuf();
    std::string bytes = byteStream.str();

    bool hasError = false;
    GThread::runOnQtGuiThread([&, this]() {
        ensureBackgroundImage();
        lockForWrite();
        if (!_backgroundImage->loadFromData(reinterpret_cast<const uchar *>(bytes.data()), bytes.length())) {
            hasError = true;
            unlock();
            return;
        }

        GInteractor::setSize(_backgroundImage->width(), _backgroundImage->height());
        // setSize(_qimage->width(), _qimage->height());
        unlock();
        conditionalRepaint();
    });

    return !hasError;
}

void GCanvas::notifyOfResize(double width, double height) {
    if (_backgroundImage) {
        GThread::runOnQtGuiThread([this, width, height]() {
            // make new image buffer of the new size
            lockForWrite();
            QImage* newImage = new QImage(
                        static_cast<int>(width),
                        static_cast<int>(height), QImage::Format_ARGB32);
            if (!_backgroundColor.empty()) {
                if (GColor::hasAlpha(_backgroundColor)) {
                    newImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    newImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            }

            // draw any previous contents onto it
            if (newImage->paintEngine()) {
                QPainter painter(newImage);
                painter.drawImage(0, 0, *_backgroundImage);
            }

            // TODO: delete _backgroundImage;
            _backgroundImage = newImage;
            unlock();
            conditionalRepaint();
        });
    }
}

void GCanvas::remove(GObject* gobj) {
    GThread::runOnQtGuiThread([this, gobj]() {
        lockForWrite();
        _gcompound.remove(gobj);
        unlock();
    });
}

void GCanvas::remove(GObject& gobj) {
    GThread::runOnQtGuiThread([this, &gobj]() {
        lockForWrite();
        _gcompound.remove(gobj);
        unlock();
    });
}

void GCanvas::removeAll() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _gcompound.removeAll();
        unlock();
    });
}

void GCanvas::repaint() {
    GThread::runOnQtGuiThreadAsync([this]() {
        lockForRead();
        getWidget()->repaint();
        unlock();
        // _gcompound.repaint();   // runs on Qt GUI thread
    });
}

void GCanvas::repaintRegion(int x, int y, int width, int height) {
    GThread::runOnQtGuiThreadAsync([this, x, y, width, height]() {
        lockForRead();
        getWidget()->repaint(x, y, width, height);
        unlock();
    });
}

void GCanvas::resize(double width, double height, bool /* retain */) {
    checkSize("GCanvas::resize", width, height);

    // TODO
    setSize(width, height);

    conditionalRepaint();
}

void GCanvas::save(const std::string& filename) {
    GThread::runOnQtGuiThread([this, filename]() {
        ensureBackgroundImage();
        lockForRead();
        if (!_gcompound.isEmpty()) {
            // flatten image in a copy object, then save
            QImage imageCopy = this->_backgroundImage->copy(
                        0, 0,
                        static_cast<int>(getWidth()), static_cast<int>(getHeight()));
            QPainter painter(&imageCopy);
            painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
            painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
            _gcompound.draw(&painter);
            painter.end();
            bool result = imageCopy.save(QString::fromStdString(filename));
            unlock();
            if (!result) {
                error("GCanvas::save: failed to save to " + filename);
            }
        } else {
            // save it myself
            bool result = _backgroundImage->save(QString::fromStdString(filename));
            unlock();
            if (!result) {
                error("GCanvas::save: failed to save to " + filename);
            }
        }
    });
    _filename = filename;
}

void GCanvas::setAutoRepaint(bool autoRepaint) {
    GThread::runOnQtGuiThread([this, autoRepaint]() {
        lockForWrite();
        _gcompound.setAutoRepaint(autoRepaint);
        unlock();
    });
}

void GCanvas::setBackground(int color) {
    GDrawingSurface::setBackground(color);
    GInteractor::setBackground(color);
    if (_backgroundImage) {
        // Slightly confusing semantics here;
        // if you draw some shapes on a canvas (with draw() variants) and then set its background,
        // the shapes will get wiped out.
        // The lesson is, set the background first before drawing stuff.
        // Or add your shapes using add() rather than draw() so they sit atop the background.
        GThread::runOnQtGuiThread([this, color]() {
            lockForWrite();
            _backgroundImage->fill(static_cast<unsigned int>(color));
            unlock();
        });
        conditionalRepaint();
    }
}

void GCanvas::setBackground(const std::string& color) {
    setBackground(GColor::convertColorToRGB(color));
}

void GCanvas::setColor(int color) {
    GDrawingSurface::setColor(color);
}

void GCanvas::setColor(const std::string& color) {
    setColor(GColor::convertColorToRGB(color));
}

void GCanvas::setFont(const QFont& font) {
    GDrawingSurface::setFont(font);
}

void GCanvas::setFont(const std::string& font) {
    GDrawingSurface::setFont(font);
}

void GCanvas::setForeground(int color) {
    GDrawingSurface::setForeground(color);
}

void GCanvas::setForeground(const std::string& color) {
    setForeground(GColor::convertColorToRGB(color));
}

void GCanvas::setKeyListener(GEventListener func) {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
        unlock();
    });
    GInteractor::setKeyListener(func);   // call super
}

void GCanvas::setKeyListener(GEventListenerVoid func) {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
        unlock();
    });
    GInteractor::setKeyListener(func);   // call super
}

void GCanvas::setPixel(double x, double y, int rgb) {
    require::inRange2D(x, y, getWidth(), getHeight(), "GCanvas::setPixel", "x", "y");
    checkColor("GCanvas::setPixel", rgb);
    GThread::runOnQtGuiThread([this, x, y, rgb]() {
        ensureBackgroundImage();
        lockForWrite();
        int argb = GColor::fixAlpha(rgb);
        _backgroundImage->setPixel(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<unsigned int>(argb));
        unlock();
        conditionalRepaintRegion(
                static_cast<int>(x),
                static_cast<int>(y),
                /* width */ 1,
                /* height */ 1);
    });
}

void GCanvas::setPixel(double x, double y, int r, int g, int b) {
    int rgb = GColor::convertRGBToRGB(r, g, b);
    int argb = rgb | 0xff000000;
    setPixel(x, y, argb);
}

void GCanvas::setPixelARGB(double x, double y, int argb) {
    require::inRange2D(x, y, getWidth(), getHeight(), "GCanvas::setPixelARGB", "x", "y");
    checkColor("GCanvas::setPixel", argb);
    GThread::runOnQtGuiThread([this, x, y, argb]() {
        ensureBackgroundImage();
        lockForWrite();
        _backgroundImage->setPixel((int) x, (int) y, argb);
        unlock();
        conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
    });
}

void GCanvas::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, GColor::convertARGBToARGB(a, r, g, b));
}

void GCanvas::setPixels(const Grid<int>& pixels) {
    // TODO: is this redundant with fromGrid?
    ensureBackgroundImage();
    if (pixels.numCols() != (int) getWidth() || pixels.numRows() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }
    GThread::runOnQtGuiThread([this, &pixels]() {
        lockForWrite();
        for (int y = 0, w = pixels.numCols(), h = pixels.numRows(); y < h; y++) {
            for (int x = 0; x < w; x++) {
                int argb = pixels[y][x] | 0xff000000;
                _backgroundImage->setPixel(x, y, static_cast<unsigned int>(argb));
            }
        }
        unlock();
        conditionalRepaint();
    });
}

void GCanvas::setPixelsARGB(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.numCols() != (int) getWidth() || pixels.numRows() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }

    GThread::runOnQtGuiThread([this, &pixels]() {
        lockForWrite();
        for (int y = 0; y < pixels.numRows(); y++) {
            for (int x = 0; x < pixels.numCols(); x++) {
                _backgroundImage->setPixel(x, y, pixels[y][x]);
            }
        }
        unlock();
        conditionalRepaint();
    });
}

GImage* GCanvas::toGImage() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    GImage* image = new GImage(_backgroundImage);
    unlockConst();
    return image;

//    GCanvas* that = const_cast<GCanvas*>(this);
//    QImage* backgroundImage = _backgroundImage;
//    GImage* gimage = nullptr;
//    GThread::runOnQtGuiThread([that, backgroundImage, &gimage]() {
//        QImage* copy = new QImage((int) that->getWidth(), (int) that->getHeight(), QImage::Format_ARGB32);
//        *copy = backgroundImage->copy();
//        QPainter painter(copy);
//        painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
//        painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
//        that->_gcompound.draw(&painter);
//        painter.end();

//        gimage = new GImage(copy);
//    });
//    return gimage;
}

Grid<int> GCanvas::toGrid() const {
    Grid<int> grid;
    toGrid(grid);
    return grid;
}

void GCanvas::toGrid(Grid<int>& grid) const {
    grid.resize(getHeight(), getWidth());
    lockForReadConst();
    for (int row = 0, width = (int) getWidth(), height = (int) getHeight(); row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = _backgroundImage->pixel(col, row);
        }
    }
    unlockConst();
}


_Internal_QCanvas::_Internal_QCanvas(GCanvas* gcanvas, QWidget* parent)
        : QWidget(parent),
          _gcanvas(gcanvas) {
    require::nonNull(gcanvas, "_Internal_QCanvas::constructor");
    setObjectName(QString::fromStdString("_Internal_QCanvas_" + std::to_string(gcanvas->getID())));

    // set default white background color
//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, Qt::white);
//    setAutoFillBackground(true);
//    setPalette(pal);
    setMouseTracking(true);   // causes mouse move events to occur
}

void _Internal_QCanvas::detach() {
    _gcanvas = nullptr;
}

void _Internal_QCanvas::enterEvent(QEvent* event) {
    QWidget::enterEvent(event);   // call super
    if (!_gcanvas || !_gcanvas->isAcceptingEvent("mouseenter")) {
        return;
    }
    _gcanvas->fireGEvent(event, MOUSE_ENTERED, "mouseenter");
}

void _Internal_QCanvas::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);   // call super
    if (!_gcanvas || !_gcanvas->isAcceptingEvent("keypress")) {
        return;
    }
    _gcanvas->fireGEvent(event, KEY_PRESSED, "keypress");
}

void _Internal_QCanvas::keyReleaseEvent(QKeyEvent* event) {
    QWidget::keyReleaseEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }

    if (_gcanvas->isAcceptingEvent("keyrelease")) {
        _gcanvas->fireGEvent(event, KEY_RELEASED, "keyrelease");
    }
    if (_gcanvas->isAcceptingEvent("keytype")) {
        _gcanvas->fireGEvent(event, KEY_TYPED, "keytype");
    }
}

void _Internal_QCanvas::leaveEvent(QEvent* event) {
    QWidget::leaveEvent(event);   // call super
    if (!_gcanvas || !_gcanvas->isAcceptingEvent("mouseexit")) {
        return;
    }
    _gcanvas->fireGEvent(event, MOUSE_EXITED, "mouseexit");
}

void _Internal_QCanvas::mouseDoubleClickEvent(QMouseEvent* event) {
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gcanvas || !_gcanvas->isAcceptingEvent("doubleclick")) {
        return;
    }
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gcanvas);
    mouseEvent.setActionCommand(_gcanvas->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gcanvas->fireEvent(mouseEvent);
}

void _Internal_QCanvas::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }
    if (!_gcanvas->isAcceptingEvent("mousemove")
            && !_gcanvas->isAcceptingEvent("mousedrag")) {
        return;
    }
    _gcanvas->fireGEvent(event, MOUSE_MOVED, "mousemove");
    if (event->buttons() != 0) {
        // mouse drag
        _gcanvas->fireGEvent(event, MOUSE_DRAGGED, "mousedrag");
    }
}

void _Internal_QCanvas::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);   // call super
    if (!_gcanvas || !_gcanvas->isAcceptingEvent("mousepress")) {
        return;
    }
    _gcanvas->fireGEvent(event, MOUSE_PRESSED, "mousepress");
}

void _Internal_QCanvas::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }
    if (_gcanvas->isAcceptingEvent("mouserelease")) {
        _gcanvas->fireGEvent(event, MOUSE_RELEASED, "mouserelease");
    }

    if (_gcanvas->isAcceptingEvent("click")) {
        _gcanvas->fireGEvent(event, MOUSE_CLICKED, "click");
    }
}

void _Internal_QCanvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }

    QPainter painter(this);
    // g.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    // g.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
    _gcanvas->draw(&painter);
    painter.end();
}

void _Internal_QCanvas::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }
    QSize size = event->size();
    _gcanvas->notifyOfResize(size.width(), size.height());
}

void _Internal_QCanvas::setCanvasSize(double width, double height) {
    QWidget::setMinimumSize(QSize((int) width, (int) height));
}

QSize _Internal_QCanvas::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

void _Internal_QCanvas::wheelEvent(QWheelEvent* event) {
    QWidget::wheelEvent(event);   // call super
    if (!_gcanvas) {
        return;
    }
    QPoint delta = event->angleDelta();
    if (delta.y() < 0) {
        // scroll down
        if (_gcanvas->isAcceptingEvent("mousewheeldown")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_DOWN, "mousewheeldown");
        }
    } else if (delta.y() > 0) {
        // scroll up
        if (_gcanvas->isAcceptingEvent("mousewheelup")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_UP, "mousewheelup");
        }
    }
}


