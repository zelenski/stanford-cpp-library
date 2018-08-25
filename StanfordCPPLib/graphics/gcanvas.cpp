/*
 * File: GCanvas.cpp
 * ------------------
 *
 * @version 2018/06/30
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "gcanvas.h"
#include "gcolor.h"
#include "gthread.h"
#include "gwindow.h"
#include "error.h"
#include "filelib.h"
#include "strlib.h"

_Internal_QCanvas::_Internal_QCanvas(GCanvas* gcanvas, QWidget* parent)
        : QWidget(parent),
          _gcanvas(gcanvas) {
    // set default white background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
    setMouseTracking(true);   // causes mouse move events to occur
}

void _Internal_QCanvas::enterEvent(QEvent* event) {
    QWidget::enterEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mouseenter")) return;
    _gcanvas->fireGEvent(event, MOUSE_ENTERED, "mouseenter");
}

void _Internal_QCanvas::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("keypress")) return;
    _gcanvas->fireGEvent(event, KEY_PRESSED, "keypress");
}

void _Internal_QCanvas::keyReleaseEvent(QKeyEvent* event) {
    QWidget::keyReleaseEvent(event);   // call super
    if (_gcanvas->isAcceptingEvent("keyrelease")) {
        _gcanvas->fireGEvent(event, KEY_RELEASED, "keyrelease");
    }
    if (_gcanvas->isAcceptingEvent("keytype")) {
        _gcanvas->fireGEvent(event, KEY_TYPED, "keytype");
    }
}

void _Internal_QCanvas::leaveEvent(QEvent* event) {
    QWidget::leaveEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mouseexit")) return;
    _gcanvas->fireGEvent(event, MOUSE_EXITED, "mouseexit");
}

void _Internal_QCanvas::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mousemove")
            && !_gcanvas->isAcceptingEvent("mousedrag")) return;
    _gcanvas->fireGEvent(event, MOUSE_MOVED, "mousemove");
    if (event->buttons() != 0) {
        // mouse drag
        _gcanvas->fireGEvent(event, MOUSE_DRAGGED, "mousedrag");
    }
}

void _Internal_QCanvas::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mousepress")) return;
    _gcanvas->fireGEvent(event, MOUSE_PRESSED, "mousepress");
}

void _Internal_QCanvas::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);   // call super
    if (_gcanvas->isAcceptingEvent("mouserelease")) {
        _gcanvas->fireGEvent(event, MOUSE_RELEASED, "mouserelease");
    }

    if (_gcanvas->isAcceptingEvent("click")) {
        _gcanvas->fireGEvent(event, MOUSE_CLICKED, "click");
    }
}

void _Internal_QCanvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);   // call super

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
    QSize size = event->size();
    _gcanvas->notifyOfResize(size.width(), size.height());
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
    if (event->pixelDelta().y() < 0) {
        // scroll down
        if (_gcanvas->isAcceptingEvent("mousewheeldown")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_DOWN, "mousewheeldown");
        }
    } else if (event->pixelDelta().y() > 0) {
        // scroll up
        if (_gcanvas->isAcceptingEvent("mousewheelup")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_UP, "mousewheelup");
        }
    }
}


#define CHAR_TO_HEX(ch) ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10))

const int GCanvas::WIDTH_HEIGHT_MAX = 65535;

int GCanvas::createRgbPixel(int red, int green, int blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("RGB values must be between 0-255");
    }
    return ((red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff)) & 0xffffff;
}

int GCanvas::getAlpha(int argb) {
    // have to & a second time because of sign-extension on >> shift
    return ((argb & 0xff000000) >> 24) & 0x000000ff;
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
    init(/* width */ -1, /* height */ -1, /* background */ 0x0, parent);
}

GCanvas::GCanvas(const std::string& filename, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename(filename) {
    init(/* width */ -1, /* height */ -1, /* background */ 0x0, parent);
    load(filename);
}

GCanvas::GCanvas(double width, double height, int rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(width, height, rgbBackground, parent);
}

GCanvas::GCanvas(double width, double height, const std::string& rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(width, height, GColor::convertColorToRGB(rgbBackground), parent);
}

void GCanvas::init(double width, double height, int rgbBackground, QWidget* parent) {
    checkSize("constructor", width, height);
    checkColor("constructor", rgbBackground);

    GThread::runOnQtGuiThread([this, parent, rgbBackground]() {
        _iqcanvas = new _Internal_QCanvas(this, getInternalParent(parent));
        _gcompound.setWidget(_iqcanvas);
        _backgroundColor = rgbBackground;
    });

    if (width > 0 && height > 0) {
        setSize(width, height);
        if (rgbBackground != 0) {
            fill(rgbBackground);
        }
    }
}



GCanvas::~GCanvas() {
    // TODO: delete _GCanvas;
    _iqcanvas = nullptr;
}

void GCanvas::add(GObject* gobj) {
    _gcompound.add(gobj);   // calls conditionalRepaint
}

void GCanvas::add(GObject* gobj, double x, double y) {
    _gcompound.add(gobj, x, y);   // calls conditionalRepaint
}

void GCanvas::add(GObject& gobj) {
    _gcompound.add(gobj);   // calls conditionalRepaint
}

void GCanvas::add(GObject& gobj, double x, double y) {
    _gcompound.add(gobj, x, y);   // calls conditionalRepaint
}

void GCanvas::clear() {
    clearPixels();
    clearObjects();
}

void GCanvas::clearObjects() {
    _gcompound.clear();   // calls conditionalRepaint
}

void GCanvas::clearPixels() {
    if (_backgroundImage) {
        // delete _backgroundImage;
        // _backgroundImage = nullptr;
        // keep background image buffer but fill with background color instead
        GThread::runOnQtGuiThread([this]() {
            _backgroundImage->fill(_backgroundColorInt);
        });
    }
    conditionalRepaint();
}

bool GCanvas::contains(double x, double y) const {
    return _gcompound.contains(x, y);
}

int GCanvas::countDiffPixels(const GCanvas& image) const {
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

int GCanvas::countDiffPixels(const GCanvas& image, int xmin, int ymin, int xmax, int ymax) const {
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

int GCanvas::countDiffPixels(const GCanvas* image) const {
    return countDiffPixels(*image);
}

int GCanvas::countDiffPixels(const GCanvas* image, int xmin, int ymin, int xmax, int ymax) const {
    return countDiffPixels(*image, xmin, ymin, xmax, ymax);
}

GCanvas* GCanvas::diff(const GCanvas& image, int diffPixelColor) const {
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
            resultGrid[r][c] = _backgroundColorInt;
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
    GCanvas* result = new GCanvas(wmax, hmax);
    result->fromGrid(resultGrid);
    return result;
}

GCanvas* GCanvas::diff(const GCanvas* image, int diffPixelColor) const {
    return diff(*image, diffPixelColor);
}

void GCanvas::draw(QPainter* painter) {
    if (_backgroundImage) {
        painter->drawImage(/* x */ 0, /* y */ 0, *_backgroundImage);
    }
    _gcompound.draw(painter);   // calls conditionalRepaint
}

void GCanvas::draw(GObject* gobj) {
    ensureBackgroundImage();
    QPainter painter(_backgroundImage);
    painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
    gobj->draw(&painter);
    painter.end();
    conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
}

void GCanvas::ensureBackgroundImage() {
    if (!_backgroundImage) {
        GThread::runOnQtGuiThread([this]() {
            _backgroundImage = new QImage((int) getWidth(), (int) getHeight(), QImage::Format_ARGB32);
            if (!_backgroundColor.empty()) {
                _backgroundImage->fill(_backgroundColorInt | 0xff000000);
            }
        });
    }
}

void GCanvas::ensureBackgroundImageConstHack() const {
    if (!_backgroundImage) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        GCanvas* hack = (GCanvas*) this;
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
    checkColor("fill", rgb);
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
    checkBounds("fillRegion", x, y);
    checkBounds("fillRegion", x + width - 1, y + height - 1);
    checkColor("fillRegion", rgb);
    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);
    for (int r = (int) y; r < y + height; r++) {
        for (int c = (int) x; c < x + width; c++) {
            setRGB(/* x */ c, /* y */ r, rgb);
        }
    }
    setAutoRepaint(wasAutoRepaint);
    conditionalRepaint();
}

void GCanvas::fillRegion(double x, double y, double width, double height, const std::string& rgb) {
    fillRegion(x, y, width, height, GColor::convertColorToRGB(rgb));
}

void GCanvas::flatten() {
    GThread::runOnQtGuiThread([this]() {
        ensureBackgroundImage();
        QPainter painter(_backgroundImage);
        painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
        painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
        _gcompound.draw(&painter);
        painter.end();
        _gcompound.clear();   // calls conditionalRepaint
    });
}

void GCanvas::fromGrid(const Grid<int>& grid) {
    checkSize("fromGrid", grid.width(), grid.height());
    setSize(grid.width(), grid.height());

    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);

    GThread::runOnQtGuiThread([this, &grid]() {
        ensureBackgroundImage();
        for (int row = 0, width = grid.width(), height = grid.height(); row < height; row++) {
            for (int col = 0; col < width; col++) {
                // setPixel(col, row, grid[row][col]);
                _backgroundImage->setPixel(col, row, grid[row][col] | 0xff000000);
            }
        }
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
    return _gcompound.getElement(index);
}

GObject* GCanvas::getElementAt(double x, double y) const {
    return _gcompound.getElementAt(x, y);
}

int GCanvas::getElementCount() const {
    return _gcompound.getElementCount();
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
    ensureBackgroundImageConstHack();
    return _backgroundImage->pixel((int) x, (int) y) & 0x00ffffff;
}

int GCanvas::getPixelARGB(double x, double y) const {
    ensureBackgroundImageConstHack();
    return _backgroundImage->pixel((int) x, (int) y);
}

Grid<int> GCanvas::getPixels() const {
    ensureBackgroundImageConstHack();
    Grid<int> grid((int) getHeight(), (int) getWidth());
    for (int y = 0; y < (int) getHeight(); y++) {
        for (int x = 0; x < (int) getWidth(); x++) {
            grid[y][x] = _backgroundImage->pixel(x, y) & 0x00ffffff;
        }
    }
    return grid;
}

Grid<int> GCanvas::getPixelsARGB() const {
    ensureBackgroundImageConstHack();
    Grid<int> grid((int) getHeight(), (int) getWidth());
    for (int y = 0; y < (int) getHeight(); y++) {
        for (int x = 0; x < (int) getWidth(); x++) {
            grid[y][x] = _backgroundImage->pixel(x, y);
        }
    }
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
    GThread::runOnQtGuiThread([this, filename]() {
        ensureBackgroundImage();
        if (!_backgroundImage->load(QString::fromStdString(filename))) {
            error("GCanvas::load: failed to load from " + filename);
        }
        _filename = filename;
        GInteractor::setSize(_backgroundImage->width(), _backgroundImage->height());
        // setSize(_qimage->width(), _qimage->height());
        conditionalRepaint();
    });
}

void GCanvas::notifyOfResize(double width, double height) {
    if (_backgroundImage) {
        GThread::runOnQtGuiThread([this, width, height]() {
            // make new image buffer of the new size
            QImage* newImage = new QImage((int) width, (int) height, QImage::Format_ARGB32);
            newImage->fill(_backgroundColorInt);

            // draw any previous contents onto it
            QPainter painter(newImage);
            painter.drawImage(0, 0, *_backgroundImage);

            // TODO: delete _backgroundImage;
            _backgroundImage = newImage;
            conditionalRepaint();
        });
    }
}

void GCanvas::remove(GObject* gobj) {
    _gcompound.remove(gobj);
}

void GCanvas::remove(GObject& gobj) {
    _gcompound.remove(gobj);
}

void GCanvas::removeAll() {
    _gcompound.removeAll();
}

void GCanvas::removeClickListener() {
    removeEventListener("click");
}

void GCanvas::removeKeyListener() {
    removeEventListeners({"keypress",
                         "keyrelease",
                         "keytype"});
}

void GCanvas::removeMouseListener() {
    removeEventListeners({"click",
                         "mousedrag",
                         "mouseenter",
                         "mouseexit",
                         "mousemove",
                         "mousepress",
                         "mouserelease",
                         "mousewheeldown",
                         "mousewheelup"});
}

void GCanvas::repaint() {
    _gcompound.repaint();   // runs on Qt GUI thread
}

void GCanvas::repaintRegion(int x, int y, int width, int height) {
    _gcompound.repaintRegion(x, y, width, height);   // runs on Qt GUI thread
}

void GCanvas::resize(double width, double height, bool /* retain */) {
    checkSize("resize", width, height);

    // TODO
    setSize(width, height);

    conditionalRepaint();
}

void GCanvas::save(const std::string& filename) {
    GThread::runOnQtGuiThread([this, filename]() {
        ensureBackgroundImage();
        if (!_gcompound.isEmpty()) {
            // flatten image in a copy object, then save
            QImage imageCopy = this->_backgroundImage->copy(0, 0, (int) getWidth(), (int) getHeight());
            QPainter painter(&imageCopy);
            painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
            painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
            _gcompound.draw(&painter);
            painter.end();
            if (!imageCopy.save(QString::fromStdString(filename))) {
                error("GCanvas::save: failed to save to " + filename);
            }
        } else {
            // save it myself
            if (!_backgroundImage->save(QString::fromStdString(filename))) {
                error("GCanvas::save: failed to save to " + filename);
            }
        }
    });
    _filename = filename;
}

void GCanvas::setAutoRepaint(bool autoRepaint) {
    _gcompound.setAutoRepaint(autoRepaint);
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
            _backgroundImage->fill(color);
        });
        conditionalRepaint();
    }
}

void GCanvas::setBackground(const std::string& color) {
    setBackground(GColor::convertColorToRGB(color));
}

void GCanvas::setClickListener(GEventListener func) {
    setEventListener("click", func);
}

void GCanvas::setClickListener(GEventListenerVoid func) {
    setEventListener("click", func);
}

void GCanvas::setColor(int color) {
    GDrawingSurface::setColor(color);
}

void GCanvas::setColor(const std::string& color) {
    setColor(GColor::convertColorToRGB(color));
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
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GCanvas::setKeyListener(GEventListenerVoid func) {
    GThread::runOnQtGuiThread([this]() {
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GCanvas::setMouseListener(GEventListener func) {
    setEventListeners({"click",
                       "mousedrag",
                       "mouseenter",
                       "mouseexit",
                       "mousemove",
                       "mousepress",
                       "mouserelease",
                       "mousewheeldown",
                       "mousewheelup"}, func);
}

void GCanvas::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"click",
                       "mousedrag",
                       "mouseenter",
                       "mouseexit",
                       "mousemove",
                       "mousepress",
                       "mouserelease",
                       "mousewheeldown",
                       "mousewheelup"}, func);
}

void GCanvas::setPixel(double x, double y, int rgb) {
    GThread::runOnQtGuiThread([this, x, y, rgb]() {
        ensureBackgroundImage();
        _backgroundImage->setPixel((int) x, (int) y, rgb | 0xff000000);
        conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
    });
}

void GCanvas::setPixel(double x, double y, int r, int g, int b) {
    setPixel(x, y, GColor::convertRGBToRGB(r, g, b) | 0xff000000);
}

void GCanvas::setPixelARGB(double x, double y, int argb) {
    GThread::runOnQtGuiThread([this, x, y, argb]() {
        ensureBackgroundImage();
        _backgroundImage->setPixel((int) x, (int) y, argb);
        conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
    });
}

void GCanvas::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, GColor::convertARGBToARGB(a, r, g, b));
}

void GCanvas::setPixels(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }
    GThread::runOnQtGuiThread([this, &pixels]() {
        for (int y = 0; y < pixels.height(); y++) {
            for (int x = 0; x < pixels.width(); x++) {
                _backgroundImage->setPixel(x, y, pixels[y][x]);
            }
        }
        conditionalRepaint();
    });
}

void GCanvas::setPixelsARGB(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }

    GThread::runOnQtGuiThread([this, &pixels]() {
        for (int y = 0; y < pixels.height(); y++) {
            for (int x = 0; x < pixels.width(); x++) {
                _backgroundImage->setPixel(x, y, pixels[y][x]);
            }
        }
        conditionalRepaint();
    });
}

Grid<int> GCanvas::toGrid() const {
    Grid<int> grid;
    toGrid(grid);
    return grid;
}

void GCanvas::toGrid(Grid<int>& grid) const {
    grid.resize(getHeight(), getWidth());
    for (int row = 0, width = (int) getWidth(), height = (int) getHeight(); row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = _backgroundImage->pixel(col, row);
        }
    }
}

#endif // SPL_QT_GUI
