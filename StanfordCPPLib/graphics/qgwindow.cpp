/*
 * File: qgwindow.cpp
 * ------------------
 *
 * @version 2018/07/03
 * - thread safety code
 * @version 2018/07/01
 * - 2D graphics/painting functions
 * @version 2018/06/25
 * - initial version
 */

#include "qgwindow.h"
#include <QDesktopWidget>
#include <QStatusBar>
#include <QThread>
#include "qgcolor.h"
#include "qgui.h"
#include "strlib.h"

_Internal_QMainWindow::_Internal_QMainWindow(QGWindow* qgwindow, QWidget* parent)
        : QMainWindow(parent),
          _qgwindow(qgwindow) {
    QGui::instance()->ensureThatThisIsTheQtGuiThread("QGWindow");
    QWidget* dummyWidget = new QWidget(this);   // TODO: memory leak?

    _overallLayout = new QVBoxLayout;   // TODO: set parent? to dummyWidget or this?
    _northLayout = new QHBoxLayout;
    _southLayout = new QHBoxLayout;
    _westLayout = new QVBoxLayout;
    _eastLayout = new QVBoxLayout;
    _centerLayout = new QHBoxLayout;
    _middleLayout = new QHBoxLayout;

    // squish margins/padding
//    _overallLayout->setSpacing(0);
//    _overallLayout->setMargin(0);
//    _overallLayout->setContentsMargins(0, 0, 0, 0);
//    _northLayout->setSpacing(0);
//    _northLayout->setMargin(0);
//    _northLayout->setContentsMargins(0, 0, 0, 0);
//    _southLayout->setSpacing(0);
//    _southLayout->setMargin(0);
//    _southLayout->setContentsMargins(0, 0, 0, 0);
//    _westLayout->setSpacing(0);
//    _westLayout->setMargin(0);
//    _westLayout->setContentsMargins(0, 0, 0, 0);
//    _eastLayout->setSpacing(0);
//    _eastLayout->setMargin(0);
//    _eastLayout->setContentsMargins(0, 0, 0, 0);
//    _centerLayout->setSpacing(0);
//    _centerLayout->setMargin(0);
//    _centerLayout->setContentsMargins(0, 0, 0, 0);

    _northLayout->addStretch(99);
    _northLayout->addStretch(99);
    _southLayout->addStretch(99);
    _southLayout->addStretch(99);
    _westLayout->addStretch(99);
    _westLayout->addStretch(99);
    _eastLayout->addStretch(99);
    _eastLayout->addStretch(99);

    _overallLayout->addLayout(_northLayout, /* stretch */ 1);
    _middleLayout->addLayout(_westLayout, /* stretch */ 1);
    _middleLayout->addLayout(_centerLayout, /* stretch */ 99);
    _middleLayout->addLayout(_eastLayout, /* stretch */ 1);
    _overallLayout->addLayout(_middleLayout, /* stretch */ 99);
    _overallLayout->addLayout(_southLayout, /* stretch */ 1);
    dummyWidget->setLayout(_overallLayout);
    layout()->setSpacing(0);
    layout()->setMargin(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(dummyWidget);
}

bool _Internal_QMainWindow::event(QEvent* event) {
    // TODO
    if (event->type() == (QEvent::Type) (QEvent::User + 106)) {
        // cout << "user event! type=" << event->type() << endl;
    }

    return QWidget::event(event);
}

_Internal_QMainWindow* QGWindow::_lastWindow = nullptr;

QGWindow::QGWindow(double width, double height, bool visible)
        : _canvas(nullptr),
          _lineWidth(1),
          _resizable(true) {
    _iqmainwindow = new _Internal_QMainWindow(this);
    _lastWindow = _iqmainwindow;

    // go ahead and set up canvas when window is loaded
    ensureCanvas();
    setCanvasSize(width, height);
    setVisible(visible);
}

QGWindow::~QGWindow() {
    if (_lastWindow == _iqmainwindow) {
        _lastWindow = nullptr;
    }
    // TODO: delete _iqmainwindow;
    _iqmainwindow = nullptr;
}

void QGWindow::add(QGInteractor* interactor) {
    addToRegion(interactor, "Center");
}

void QGWindow::addToRegion(QGInteractor* interactor, Region region) {
    addToRegion(interactor, regionToString(region));
}

void QGWindow::addToRegion(QGInteractor* interactor, const std::string& region) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = regionToLayout(region);
    if (!layout) {
        return;
    }

    if (layout == _iqmainwindow->_centerLayout) {
        // center holds at most one widget
        if (_canvas) {
            _canvas->setVisible(false);
        }
        QGBorderLayout::clearLayout(layout);
        layout->addWidget(widget);
    } else {
        // add to end of the list of widgets in this region
        ((QBoxLayout*) layout)->insertWidget(/* index */ layout->count() - 1, widget);
    }
    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::clear() {
    clearRegion(REGION_NORTH);
    clearRegion(REGION_SOUTH);
    clearRegion(REGION_WEST);
    clearRegion(REGION_EAST);
    clearRegion(REGION_CENTER);
}

void QGWindow::clearCanvas() {
    if (_canvas) {
        _canvas->clear();
    }
}

void QGWindow::clearRegion(Region region) {
    clearRegion(regionToString(region));
}

void QGWindow::clearRegion(const std::string& region) {
    QLayout* layout = regionToLayout(region);
    if (!layout) {
        return;
    }

    if (layout == _iqmainwindow->_centerLayout) {
        QGBorderLayout::clearLayout(layout);

        // put the canvas back in the center if there is nothing else there
        // TODO: remove this?
        if (_canvas) {
            layout->addWidget(_canvas->getWidget());
            _canvas->setVisible(true);
        }
    } else {
        // for N/S/W/E, leave 2 elements in the layout (stretchers at start/end)
        while (layout->count() > 2) {
            layout->removeItem(layout->itemAt(1));
        }
    }
    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.getWidth()  / 2 - windowSize.getWidth()  / 2,
                screenSize.getHeight() / 2 - windowSize.getHeight() / 2);
}

void QGWindow::close() {
    _iqmainwindow->close();
}

void QGWindow::compareToImage(const std::string& /* filename */, bool /* ignoreWindowSize */) const {
    // TODO
}

void QGWindow::draw(QGObject* obj) {
    _canvas->add(obj);
}

void QGWindow::draw(QGObject& obj, double x, double y) {
    obj.setLocation(x, y);
    _canvas->add(&obj);
}

void QGWindow::draw(QGObject* obj, double x, double y) {
    obj->setLocation(x, y);
    _canvas->add(obj);
}

void QGWindow::drawImage(const std::string& filename, double x, double y) {
    ensureCanvas();
    QGImage* image = new QGImage(filename, x, y);
    _canvas->add(image);
}

void QGWindow::drawLine(const GPoint& p0, const GPoint& p1) {
    drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
}

void QGWindow::drawLine(double x0, double y0, double x1, double y1) {
    ensureCanvas();
    QGLine* line = new QGLine(x0, y0, x1, y1);
    initializeQGObject(line);
    _canvas->add(line);
}

void QGWindow::drawOval(const GRectangle& bounds) {
    drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGWindow::drawOval(double x, double y, double width, double height) {
    ensureCanvas();
    QGOval* oval = new QGOval(x, y, width, height);
    initializeQGObject(oval);
    _canvas->add(oval);
}

GPoint QGWindow::drawPolarLine(const GPoint& p0, double r, double theta) {
    return drawPolarLine(p0.getX(), p0.getY(), r, theta);
}

GPoint QGWindow::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return GPoint(x1, y1);
}

void QGWindow::drawPixel(double /* x */, double /* y */) {
    // TODO
}

void QGWindow::drawPixel(double /* x */, double /* y */, int /* color */) {
    // TODO
}

void QGWindow::drawPixel(double /* x */, double /* y */, const std::string& /* color */) {
    // TODO
}

void QGWindow::drawPolygon(std::initializer_list<double> coords) {
    ensureCanvas();
    QGPolygon* polygon = new QGPolygon(coords);
    initializeQGObject(polygon);
    _canvas->add(polygon);
}

void QGWindow::drawPolygon(std::initializer_list<GPoint> points) {
    ensureCanvas();
    QGPolygon* polygon = new QGPolygon(points);
    initializeQGObject(polygon);
    _canvas->add(polygon);
}

void QGWindow::drawRect(const GRectangle& bounds) {
    drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGWindow::drawRect(double x, double y, double width, double height) {
    ensureCanvas();
    QGRect* rect = new QGRect(x, y, width, height);
    initializeQGObject(rect);
    _canvas->add(rect);
}

void QGWindow::drawString(const std::string& text, double x, double y) {
    ensureCanvas();
    QGString* str = new QGString(text, x, y);
    initializeQGObject(str);
    _canvas->add(str);
}

void QGWindow::ensureCanvas() {
    if (!_canvas) {
        _canvas = new QGCanvas(_iqmainwindow);
        addToRegion(_canvas, "Center");
    }
}

void QGWindow::fillOval(const GRectangle& bounds) {
    fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGWindow::fillOval(double x, double y, double width, double height) {
    ensureCanvas();
    QGOval* oval = new QGOval(x, y, width, height);
    initializeQGObject(oval, /* filled */ true);
    _canvas->add(oval);
}

void QGWindow::fillPolygon(std::initializer_list<double> coords) {
    ensureCanvas();
    QGPolygon* polygon = new QGPolygon(coords);
    initializeQGObject(polygon, /* filled */ true);
    _canvas->add(polygon);
}

void QGWindow::fillRect(const GRectangle& bounds) {
    fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGWindow::fillRect(double x, double y, double width, double height) {
    ensureCanvas();
    QGRect* rect = new QGRect(x, y, width, height);
    initializeQGObject(rect, /* filled */ true);
    _canvas->add(rect);
}

double QGWindow::getCanvasHeight() const {
    return _canvas->getHeight();
}

GDimension QGWindow::getCanvasSize() const {
    return GDimension(_canvas->getWidth(), _canvas->getHeight());
}

double QGWindow::getCanvasWidth() const {
    return _canvas->getWidth();
}

std::string QGWindow::getColor() const {
    return _color;
}

int QGWindow::getColorInt() const {
    return _colorInt;
}

std::string QGWindow::getFillColor() const {
    return _fillColor;
}

int QGWindow::getFillColorInt() const {
    return _fillColorInt;
}

QGObject* QGWindow::getQGObject(int index) const {
    if (_canvas) {
        return _canvas->getElement(index);
    } else {
        return nullptr;
    }
}

QGObject* QGWindow::getQGObjectAt(double x, double y) const {
    if (_canvas) {
        return _canvas->getElementAt(x, y);
    } else {
        return nullptr;
    }
}

int QGWindow::getQGObjectCount() const {
    if (_canvas) {
        return _canvas->getElementCount();
    } else {
        return 0;
    }
}

QMainWindow* QGWindow::getLastWindow() {
    return _lastWindow;
}

Point QGWindow::getLocation() const {
    QRect geom = _iqmainwindow->geometry();
    return Point(geom.x(), geom.y());
}

double QGWindow::getHeight() const {
    return _iqmainwindow->geometry().height();
}

double QGWindow::getLineWidth() const {
    return _lineWidth;
}

int QGWindow::getPixel(double /* x */, double /* y */) const {
    // TODO
    return 0;
}

int QGWindow::getPixelARGB(double /* x */, double /* y */) const {
    // TODO
    return 0;
}

Grid<int> QGWindow::getPixels() const {
    // TODO
    Grid<int> grid;
    return grid;
}

Grid<int> QGWindow::getPixelsARGB() const {
    // TODO
    Grid<int> grid;
    return grid;
}

double QGWindow::getRegionHeight(Region /* region */) const {
    // TODO
    return 0.0;
}

double QGWindow::getRegionHeight(const std::string& /* region */) const {
    // TODO
    return 0.0;
}

GDimension QGWindow::getRegionSize(Region /* region */) const {
    // TODO
    return GDimension();
}

GDimension QGWindow::getRegionSize(const std::string& /* region */) const {
    // TODO
    return GDimension();
}

double QGWindow::getRegionWidth(Region /* region */) const {
    // TODO
    return 0.0;
}

double QGWindow::getRegionWidth(const std::string& /* region */) const {
    // TODO
    return 0.0;
}

double QGWindow::getScreenHeight() {
    return getScreenSize().getHeight();
}

GDimension QGWindow::getScreenSize() {
    QRect rec = QApplication::desktop()->availableGeometry();
    return GDimension(rec.width(), rec.height());
}

double QGWindow::getScreenWidth() {
    return getScreenSize().getWidth();
}

GDimension QGWindow::getSize() const {
    QRect geom = _iqmainwindow->geometry();
    return GDimension(geom.width(), geom.height());
}

std::string QGWindow::getTitle() const {
    return _iqmainwindow->windowTitle().toStdString();
}

QWidget* QGWindow::getWidget() const {
    return static_cast<QWidget*>(_iqmainwindow);
}

double QGWindow::getWidth() const {
    return _iqmainwindow->geometry().width();
}

double QGWindow::getX() const {
    return _iqmainwindow->geometry().x();
}

double QGWindow::getY() const {
    return _iqmainwindow->geometry().y();
}

bool QGWindow::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool QGWindow::inCanvasBounds(double x, double y) const {
    return 0 <= x && x < getCanvasWidth() && 0 <= y && y < getCanvasHeight();
}

void QGWindow::initializeQGObject(QGObject* obj, bool fill) {
    if (!obj) {
        return;
    }
    obj->setColor(_color);
    if (fill) {
        obj->setFilled(true);
        obj->setFillColor(_fillColor);
    }
    obj->setFont(_font);
    obj->setLineWidth(_lineWidth);
}

bool QGWindow::isOpen() const {
    return isVisible();
}

bool QGWindow::isRepaintImmediately() const {
    // TODO
    return true;
}

bool QGWindow::isResizable() const {
    return _resizable;
}

bool QGWindow::isVisible() const {
    return _iqmainwindow->isVisible();
}

void QGWindow::loadCanvasPixels(const std::string& /* filename */) {
    // TODO
}

void QGWindow::pack() {
    // TODO
}

void QGWindow::pause(double ms) {
    QThread* thread = _iqmainwindow->thread();
    if (thread) {
        thread->msleep((long) ms);
    }
}

QLayout* QGWindow::regionToLayout(Region region) {
    return regionToLayout(regionToString(region));
}

QLayout* QGWindow::regionToLayout(const std::string& region) {
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    if (position == QGBorderLayout::North) {
        return _iqmainwindow->_northLayout;
    } else if (position == QGBorderLayout::South) {
        return _iqmainwindow->_southLayout;
    } else if (position == QGBorderLayout::West) {
        return _iqmainwindow->_westLayout;
    } else if (position == QGBorderLayout::East) {
        return _iqmainwindow->_eastLayout;
    } else if (position == QGBorderLayout::Center) {
        return _iqmainwindow->_centerLayout;
    } else {
        return nullptr;
    }
}

std::string QGWindow::regionToString(Region region) {
    return region == REGION_NORTH ? "North" :
           region == REGION_SOUTH ? "South" :
           region == REGION_WEST  ? "West" :
           region == REGION_EAST  ? "East" : "Center";
}

void QGWindow::remove(QGObject* obj) {
    if (_canvas) {
        _canvas->remove(obj);
    }
}

void QGWindow::remove(QGObject& obj) {
    if (_canvas) {
        _canvas->remove(&obj);
    }
}

void QGWindow::remove(QGInteractor* interactor) {
    removeFromRegion(interactor, "North");
    removeFromRegion(interactor, "South");
    removeFromRegion(interactor, "East");
    removeFromRegion(interactor, "West");
    removeFromRegion(interactor, "Center");
}

void QGWindow::removeFromRegion(QGInteractor* interactor, Region region) {
    removeFromRegion(interactor, regionToString(region));
}

void QGWindow::removeFromRegion(QGInteractor* interactor, const std::string& region) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = regionToLayout(region);
    if (!layout) {
        return;
    }

    layout->removeWidget(widget);
    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::repaint() {
    if (_canvas) {
        _canvas->repaint();
    }
}

void QGWindow::requestFocus() {
    _iqmainwindow->setFocus();
}

void QGWindow::saveCanvasPixels(const std::string& /* filename */) {
    // TODO
    ensureCanvas();
}

void QGWindow::setCanvasHeight(double height) {
    // TODO
    ensureCanvas();
    setHeight(height);
}

void QGWindow::setCanvasSize(double width, double height) {
    // TODO
    ensureCanvas();
    setSize(width, height);
}

void QGWindow::setCanvasSize(const GDimension& size) {
    // TODO
    ensureCanvas();
    setSize(size);
}

void QGWindow::setCanvasWidth(double width) {
    // TODO
    ensureCanvas();
    setWidth(width);
}

void QGWindow::setCloseOperation(CloseOperation /* op */) {
    // TODO
}

void QGWindow::setColor(int color) {
    _colorInt = color;
    _color = QGColor::convertRGBToColor(color);
}

void QGWindow::setColor(const std::string& color) {
    _color = color;
    _colorInt = QGColor::convertColorToRGB(color);
}

void QGWindow::setExitOnClose(bool /* exitOnClose */) {
    // TODO
}

void QGWindow::setFillColor(int color) {
    _fillColorInt = color;
    _fillColor = QGColor::convertRGBToColor(color);
}

void QGWindow::setFillColor(const std::string& color) {
    _fillColor = color;
    _fillColorInt = QGColor::convertColorToRGB(color);
}

void QGWindow::setFont(const std::string& font) {
    _font = font;
}

void QGWindow::setHeight(double height) {
    setSize(getWidth(), height);
}

void QGWindow::setLineWidth(double lineWidth) {
    _lineWidth = lineWidth;
}

void QGWindow::setLocation(double x, double y) {
    _iqmainwindow->setGeometry((int) x, (int) y, getWidth(), getHeight());
}

void QGWindow::setLocation(const GPoint& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setPixel(double /* x */, double /* y */, int /* rgb */) {
    // TODO
}

void QGWindow::setPixelARGB(double /* x */, double /* y */, int /* argb */) {
    // TODO
}

void QGWindow::setPixels(const Grid<int>& /* pixels */) {
    // TODO
}

void QGWindow::setPixelsARGB(const Grid<int>& /* pixelsARGB */) {
    // TODO
}

void QGWindow::setRegionAlignment(Region /* region */, Alignment /* align */) {
    // TODO
}

void QGWindow::setRegionAlignment(const std::string& /* region */, const std::string& /* align */) {
    // TODO
}

void QGWindow::setRepaintImmediately(bool repaintImmediately) {
    ensureCanvas();
    _canvas->setAutoRepaint(repaintImmediately);
}

void QGWindow::setResizable(bool resizable) {
    if (resizable) {
        if (!_resizable) {
            _iqmainwindow->resize((int) getWidth(), (int) getHeight());
            _iqmainwindow->setMinimumSize(_iqmainwindow->minimumSizeHint());
            GDimension screenSize = getScreenSize();
            _iqmainwindow->setMaximumSize((int) screenSize.getWidth(), (int) screenSize.getHeight());
        }
    } else {
        if (_resizable) {
            _iqmainwindow->setFixedSize((int) getWidth(), (int) getHeight());
        }
    }
    _resizable = resizable;
}

void QGWindow::setSize(double width, double height) {
    _iqmainwindow->resize((int) width, (int) height);
}

void QGWindow::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void QGWindow::setTitle(const std::string& title) {
    _iqmainwindow->setWindowTitle(QString::fromStdString(title));
}

void QGWindow::setVisible(bool visible) {
    _iqmainwindow->setVisible(visible);
}

void QGWindow::setWidth(double width) {
    setSize(width, getHeight());
}

void QGWindow::setWindowTitle(const std::string& title) {
    setTitle(title);
}

void QGWindow::setX(double x) {
    setLocation(x, getY());
}

void QGWindow::setY(double y) {
    setLocation(getX(), y);
}

void QGWindow::toBack() {
    _iqmainwindow->lower();
}

void QGWindow::toFront() {
    _iqmainwindow->raise();
}
