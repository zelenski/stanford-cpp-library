/*
 * File: qgcanvas.cpp
 * ------------------
 *
 * @version 2018/06/30
 * - initial version
 */

#include "qgcanvas.h"
#include "qgcolor.h"
#include "qgwindow.h"
#include "error.h"
#include "strlib.h"

_Internal_QCanvas::_Internal_QCanvas(QGCanvas* qgcanvas, QWidget* parent)
        : QWidget(parent),
          _qgcanvas(qgcanvas) {
    // set default white background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
    setMouseTracking(true);   // causes mouse move events to occur
}

void _Internal_QCanvas::enterEvent(QEvent* event) {
    QWidget::enterEvent(event);   // call super
    if (!_qgcanvas->hasEventHandler("mouseenter")) return;
    _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_ENTERED, "mouseenter");
}

void _Internal_QCanvas::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);   // call super
    if (!_qgcanvas->hasEventHandler("keypress")) return;
    _qgcanvas->fireQGEvent(event, QGEvent::KEY_PRESSED, "keypress");
}

void _Internal_QCanvas::keyReleaseEvent(QKeyEvent* event) {
    QWidget::keyReleaseEvent(event);   // call super
    if (_qgcanvas->hasEventHandler("keyrelease")) {
        _qgcanvas->fireQGEvent(event, QGEvent::KEY_RELEASED, "keyrelease");
    }
    if (_qgcanvas->hasEventHandler("keytype")) {
        _qgcanvas->fireQGEvent(event, QGEvent::KEY_TYPED, "keytype");
    }
}

void _Internal_QCanvas::leaveEvent(QEvent* event) {
    QWidget::leaveEvent(event);   // call super
    if (!_qgcanvas->hasEventHandler("mouseexit")) return;
    _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_EXITED, "mouseexit");
}

void _Internal_QCanvas::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);   // call super
    if (!_qgcanvas->hasEventHandler("mousemove")
            && !_qgcanvas->hasEventHandler("mousedrag")) return;
    _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_MOVED, "mousemove");
    if (event->buttons() != 0) {
        // mouse drag
        _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_DRAGGED, "mousedrag");
    }
}

void _Internal_QCanvas::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);   // call super
    if (!_qgcanvas->hasEventHandler("mousepress")) return;
    _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_PRESSED, "mousepress");
}

void _Internal_QCanvas::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);   // call super
    if (_qgcanvas->hasEventHandler("mouserelease")) {
        _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_RELEASED, "mouserelease");
    }

    if (_qgcanvas->hasEventHandler("click")) {
        _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_CLICKED, "click");
    }
}

void _Internal_QCanvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);   // call super

    QPainter painter(this);
    // g.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    // g.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::Antialiasing, QGObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, QGObject::isAntiAliasing());
    _qgcanvas->draw(&painter);
    painter.end();
}

void _Internal_QCanvas::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);   // call super
    QSize size = event->size();
    _qgcanvas->notifyOfResize(size.width(), size.height());
}

void _Internal_QCanvas::wheelEvent(QWheelEvent* event) {
    QWidget::wheelEvent(event);   // call super
    if (event->pixelDelta().y() < 0) {
        // scroll down
        if (_qgcanvas->hasEventHandler("mousewheeldown")) {
            _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_WHEEL_DOWN, "mousewheeldown");
        }
    } else if (event->pixelDelta().y() > 0) {
        // scroll up
        if (_qgcanvas->hasEventHandler("mousewheelup")) {
            _qgcanvas->fireQGEvent(event, QGEvent::MOUSE_WHEEL_UP, "mousewheelup");
        }
    }
}


QGCanvas::QGCanvas(QWidget* parent)
        : _backgroundImage(nullptr) {
    _iqcanvas = new _Internal_QCanvas(this, getInternalParent(parent));
    _qgcompound.setWidget(_iqcanvas);
}

QGCanvas::~QGCanvas() {
    // TODO: delete _qgcanvas;
    _iqcanvas = nullptr;
}

void QGCanvas::add(QGObject* gobj) {
    _qgcompound.add(gobj);   // calls conditionalRepaint
}

void QGCanvas::add(QGObject* gobj, double x, double y) {
    _qgcompound.add(gobj, x, y);   // calls conditionalRepaint
}

void QGCanvas::add(QGObject& gobj) {
    _qgcompound.add(gobj);   // calls conditionalRepaint
}

void QGCanvas::add(QGObject& gobj, double x, double y) {
    _qgcompound.add(gobj, x, y);   // calls conditionalRepaint
}

void QGCanvas::clear() {
    clearPixels();
    clearObjects();
}

void QGCanvas::clearObjects() {
    _qgcompound.clear();   // calls conditionalRepaint
}

void QGCanvas::clearPixels() {
    if (_backgroundImage) {
        // delete _backgroundImage;
        // _backgroundImage = nullptr;
        // keep background image buffer but fill with background color instead
        _backgroundImage->fill(_backgroundColorInt);
    }
    conditionalRepaint();
}

bool QGCanvas::contains(double x, double y) const {
    return _qgcompound.contains(x, y);
}

void QGCanvas::draw(QPainter* painter) {
    if (_backgroundImage) {
        painter->drawImage(/* x */ 0, /* y */ 0, *_backgroundImage);
    }
    _qgcompound.draw(painter);   // calls conditionalRepaint
}

void QGCanvas::draw(QGObject* gobj) {
    ensureBackgroundImage();
    QPainter painter(_backgroundImage);
    painter.setRenderHint(QPainter::Antialiasing, QGObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, QGObject::isAntiAliasing());
    gobj->draw(&painter);
    painter.end();
    conditionalRepaintRegion(gobj->getBounds().enlargedBy(1));
}

void QGCanvas::flatten() {
    ensureBackgroundImage();
    QPainter painter(_backgroundImage);
    painter.setRenderHint(QPainter::Antialiasing, QGObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, QGObject::isAntiAliasing());
    _qgcompound.draw(&painter);
    painter.end();
    _qgcompound.clear();   // calls conditionalRepaint
}

void QGCanvas::ensureBackgroundImage() {
    if (!_backgroundImage) {
        _backgroundImage = new QImage((int) getWidth(), (int) getHeight(), QImage::Format_ARGB32);
        _backgroundImage->fill(_backgroundColorInt);
    }
}

void QGCanvas::ensureBackgroundImageConstHack() const {
    if (!_backgroundImage) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        QGCanvas* hack = (QGCanvas*) this;
        hack->ensureBackgroundImage();
    }
}

std::string QGCanvas::getBackground() const {
    return QGDrawingSurface::getBackground();
}

int QGCanvas::getBackgroundInt() const {
    return QGDrawingSurface::getBackgroundInt();
}

QGObject* QGCanvas::getElement(int index) const {
    return _qgcompound.getElement(index);
}

QGObject* QGCanvas::getElementAt(double x, double y) const {
    return _qgcompound.getElementAt(x, y);
}

int QGCanvas::getElementCount() const {
    return _qgcompound.getElementCount();
}

int QGCanvas::getPixel(double x, double y) const {
    ensureBackgroundImageConstHack();
    return _backgroundImage->pixel((int) x, (int) y) & 0x00ffffff;
}

int QGCanvas::getPixelARGB(double x, double y) const {
    ensureBackgroundImageConstHack();
    return _backgroundImage->pixel((int) x, (int) y);
}

Grid<int> QGCanvas::getPixels() const {
    ensureBackgroundImageConstHack();
    Grid<int> grid((int) getHeight(), (int) getWidth());
    for (int y = 0; y < (int) getHeight(); y++) {
        for (int x = 0; x < (int) getWidth(); x++) {
            grid[y][x] = _backgroundImage->pixel(x, y) & 0x00ffffff;
        }
    }
    return grid;
}

Grid<int> QGCanvas::getPixelsARGB() const {
    ensureBackgroundImageConstHack();
    Grid<int> grid((int) getHeight(), (int) getWidth());
    for (int y = 0; y < (int) getHeight(); y++) {
        for (int x = 0; x < (int) getWidth(); x++) {
            grid[y][x] = _backgroundImage->pixel(x, y);
        }
    }
    return grid;
}

std::string QGCanvas::getType() const {
    return "QGCanvas";
}

QWidget* QGCanvas::getWidget() const {
    return static_cast<QWidget*>(_iqcanvas);
}

bool QGCanvas::isAutoRepaint() const {
    return _qgcompound.isAutoRepaint();
}

void QGCanvas::notifyOfResize(double width, double height) {
    if (_backgroundImage) {
        // make new image buffer of the new size
        QImage* newImage = new QImage((int) width, (int) height, QImage::Format_ARGB32);
        newImage->fill(_backgroundColorInt);

        // draw any previous contents onto it
        QPainter painter(newImage);
        painter.drawImage(0, 0, *_backgroundImage);

        // TODO: delete _backgroundImage;
        _backgroundImage = newImage;
        conditionalRepaint();
    }
}

void QGCanvas::remove(QGObject* gobj) {
    _qgcompound.remove(gobj);
}

void QGCanvas::remove(QGObject& gobj) {
    _qgcompound.remove(gobj);
}

void QGCanvas::removeAll() {
    _qgcompound.removeAll();
}

void QGCanvas::removeClickHandler() {
    removeEventHandler("click");
}

void QGCanvas::removeKeyHandler() {
    removeEventHandlers({"keypress",
                         "keyrelease",
                         "keytype"});
}

void QGCanvas::removeMouseHandler() {
    removeEventHandlers({"click",
                         "mousedrag",
                         "mouseenter",
                         "mouseexit",
                         "mousemove",
                         "mousepress",
                         "mouserelease",
                         "mousewheeldown",
                         "mousewheelup"});
}

void QGCanvas::repaint() {
    _qgcompound.repaint();
}

void QGCanvas::repaintRegion(int x, int y, int width, int height) {
    _qgcompound.repaintRegion(x, y, width, height);
}

void QGCanvas::setAutoRepaint(bool autoRepaint) {
    _qgcompound.setAutoRepaint(autoRepaint);
}

void QGCanvas::setBackground(int color) {
    QGDrawingSurface::setBackground(color);
    QGInteractor::setBackground(color);
    if (_backgroundImage) {
        // Slightly confusing semantics here;
        // if you draw some shapes on a canvas (with draw() variants) and then set its background,
        // the shapes will get wiped out.
        // The lesson is, set the background first before drawing stuff.
        // Or add your shapes using add() rather than draw() so they sit atop the background.
        _backgroundImage->fill(color);
        conditionalRepaint();
    }
}

void QGCanvas::setBackground(const std::string& color) {
    setBackground(QGColor::convertColorToRGB(color));
}

void QGCanvas::setClickHandler(QGEventHandler func) {
    setEventHandler("click", func);
}

void QGCanvas::setClickHandler(QGEventHandlerVoid func) {
    setEventHandler("click", func);
}

void QGCanvas::setColor(int color) {
    QGDrawingSurface::setColor(color);
    QGInteractor::setColor(color);
}

void QGCanvas::setColor(const std::string& color) {
    setColor(QGColor::convertColorToRGB(color));
}

void QGCanvas::setFont(const std::string& font) {
    QGDrawingSurface::setFont(font);
    QGInteractor::setFont(font);
}

void QGCanvas::setKeyHandler(QGEventHandler func) {
    _iqcanvas->setFocusPolicy(Qt::StrongFocus);
    setEventHandlers({"keypress",
                      "keyrelease",
                      "keytype"}, func);
}

void QGCanvas::setKeyHandler(QGEventHandlerVoid func) {
    _iqcanvas->setFocusPolicy(Qt::StrongFocus);
    setEventHandlers({"keypress",
                      "keyrelease",
                      "keytype"}, func);
}

void QGCanvas::setMouseHandler(QGEventHandler func) {
    setEventHandlers({"click",
                      "mousedrag",
                      "mouseenter",
                      "mouseexit",
                      "mousemove",
                      "mousepress",
                      "mouserelease",
                      "mousewheeldown",
                      "mousewheelup"}, func);
}

void QGCanvas::setMouseHandler(QGEventHandlerVoid func) {
    setEventHandlers({"click",
                      "mousedrag",
                      "mouseenter",
                      "mouseexit",
                      "mousemove",
                      "mousepress",
                      "mouserelease",
                      "mousewheeldown",
                      "mousewheelup"}, func);
}

void QGCanvas::setPixel(double x, double y, int rgb) {
    ensureBackgroundImage();
    _backgroundImage->setPixel((int) x, (int) y, rgb | 0xff000000);
    conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
}

void QGCanvas::setPixel(double x, double y, int r, int g, int b) {
    ensureBackgroundImage();
    _backgroundImage->setPixel((int) x, (int) y, QGColor::convertRGBToRGB(r, g, b) | 0xff000000);
    conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
}

void QGCanvas::setPixelARGB(double x, double y, int argb) {
    ensureBackgroundImage();
    _backgroundImage->setPixel((int) x, (int) y, argb);
    conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
}

void QGCanvas::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    ensureBackgroundImage();
    _backgroundImage->setPixel((int) x, (int) y, QGColor::convertARGBToARGB(a, r, g, b));
    conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
}

void QGCanvas::setPixels(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("QGCanvas::setPixels: wrong size");
    }
    for (int y = 0; y < pixels.height(); y++) {
        for (int x = 0; x < pixels.width(); x++) {
            _backgroundImage->setPixel(x, y, pixels[y][x]);
        }
    }
    conditionalRepaint();
}

void QGCanvas::setPixelsARGB(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("QGCanvas::setPixels: wrong size");
    }
    for (int y = 0; y < pixels.height(); y++) {
        for (int x = 0; x < pixels.width(); x++) {
            _backgroundImage->setPixel(x, y, pixels[y][x]);
        }
    }
    conditionalRepaint();
}
