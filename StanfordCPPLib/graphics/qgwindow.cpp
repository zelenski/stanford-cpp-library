/*
 * File: qgwindow.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgwindow.h"
#include <QDesktopWidget>
#include <QStatusBar>
#include <QThread>
#include "qgcolor.h"
#include "strlib.h"

_Q_Internal_Window::_Q_Internal_Window(QGWindow* window, QWidget* parent)
        : QMainWindow(parent),
          _qgwindow(window) {

    QWidget* dummyWidget = new QWidget(this);

    _overallLayout.setSpacing(0);
    _overallLayout.setMargin(0);
    _overallLayout.setContentsMargins(0, 0, 0, 0);
    _northLayout.setSpacing(0);
    _northLayout.setMargin(0);
    _northLayout.setContentsMargins(0, 0, 0, 0);
    _southLayout.setSpacing(0);
    _southLayout.setMargin(0);
    _southLayout.setContentsMargins(0, 0, 0, 0);
    _westLayout.setSpacing(0);
    _westLayout.setMargin(0);
    _westLayout.setContentsMargins(0, 0, 0, 0);
    _eastLayout.setSpacing(0);
    _eastLayout.setMargin(0);
    _eastLayout.setContentsMargins(0, 0, 0, 0);
    _centerLayout.setSpacing(0);
    _centerLayout.setMargin(0);
    _centerLayout.setContentsMargins(0, 0, 0, 0);

    _northLayout.addStretch(99);
    _northLayout.addStretch(99);
    _southLayout.addStretch(99);
    _southLayout.addStretch(99);
    _westLayout.addStretch(99);
    _westLayout.addStretch(99);
    _eastLayout.addStretch(99);
    _eastLayout.addStretch(99);

    _overallLayout.addLayout(&_northLayout, /* stretch */ 1);
    _middleLayout.addLayout(&_westLayout, /* stretch */ 1);
    _middleLayout.addLayout(&_centerLayout, /* stretch */ 99);
    _middleLayout.addLayout(&_eastLayout, /* stretch */ 1);
    _overallLayout.addLayout(&_middleLayout, /* stretch */ 99);
    _overallLayout.addLayout(&_southLayout, /* stretch */ 1);
    dummyWidget->setLayout(&_overallLayout);
    layout()->setSpacing(0);
    layout()->setMargin(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(dummyWidget);
}

QApplication* QGWindow::_app = nullptr;
_Q_Internal_Window* QGWindow::_lastWindow = nullptr;

QGWindow::QGWindow(double width, double height, bool visible)
        : _qwindow(this),
          _canvas(nullptr),
          _lineWidth(1),
          _resizable(true) {
    if (!_app) {
        initializeQt();
    }
    _lastWindow = &_qwindow;

    // go ahead and set up canvas when window is loaded
    ensureCanvas();
    setCanvasSize(width, height);
    setVisible(visible);
}

QGWindow::~QGWindow() {
    if (_lastWindow == &_qwindow) {
        _lastWindow = nullptr;
    }
}

void QGWindow::add(QGInteractor* interactor) {
    addToRegion(interactor, "Center");
}

void QGWindow::addToRegion(QGInteractor* interactor, Region region) {
    addToRegion(interactor, regionToString(region));
}

void QGWindow::addToRegion(QGInteractor* interactor, const std::string& region) {
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    if (position == QGBorderLayout::North) {
        // _qwindow._northLayout.addWidget(interactor->getWidget());
        _qwindow._northLayout.insertWidget(/* index */ _qwindow._northLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::South) {
        // _qwindow._southLayout.addWidget(interactor->getWidget());
        _qwindow._southLayout.insertWidget(/* index */ _qwindow._southLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::West) {
        // _qwindow._westLayout.addWidget(interactor->getWidget());
        _qwindow._westLayout.insertWidget(/* index */ _qwindow._westLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::East) {
        // _qwindow._eastLayout.addWidget(interactor->getWidget());
        _qwindow._eastLayout.insertWidget(/* index */ _qwindow._eastLayout.count() - 1, interactor->getWidget());
    } else {
        // center holds at most one widget
        if (_qwindow._centerLayout.isEmpty()) {
            _qwindow._centerLayout.addWidget(interactor->getWidget());
        } else {
            _qwindow._centerLayout.replaceWidget(
                        /* from */ _qwindow._centerLayout.takeAt(0)->widget(),
                        /* to   */ interactor->getWidget());
        }
    }
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
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    if (position == QGBorderLayout::North) {
        QGBorderLayout::clearLayout(&_qwindow._northLayout);
    } else if (position == QGBorderLayout::South) {
        QGBorderLayout::clearLayout(&_qwindow._southLayout);
    } else if (position == QGBorderLayout::West) {
        QGBorderLayout::clearLayout(&_qwindow._westLayout);
    } else if (position == QGBorderLayout::East) {
        QGBorderLayout::clearLayout(&_qwindow._eastLayout);
    } else {
        QGBorderLayout::clearLayout(&_qwindow._centerLayout);
    }
    _qwindow.update();
}

void QGWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.getWidth()  / 2 - windowSize.getWidth()  / 2,
                screenSize.getHeight() / 2 - windowSize.getHeight() / 2);
}

void QGWindow::close() {
    _qwindow.close();
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
        _canvas = new QGCanvas(&_qwindow);
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
    QRect geom = _qwindow.geometry();
    return Point(geom.x(), geom.y());
}

double QGWindow::getHeight() const {
    return _qwindow.geometry().height();
}

double QGWindow::getLineWidth() const {
    return _lineWidth;
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
    QRect geom = _qwindow.geometry();
    return GDimension(geom.width(), geom.height());
}

std::string QGWindow::getTitle() const {
    return _qwindow.windowTitle().toStdString();
}

double QGWindow::getWidth() const {
    return _qwindow.geometry().width();
}

double QGWindow::getX() const {
    return _qwindow.geometry().x();
}

double QGWindow::getY() const {
    return _qwindow.geometry().y();
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

void initializeQt() {
    static int argc = 1;
    static char* argv[1] = {"Stanford Cpp Lib"};
    if (!QGWindow::_app) {
        QGWindow::_app = new QApplication(argc, argv);
    }
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
    return _qwindow.isVisible();
}

void QGWindow::loadCanvasPixels(const std::string& /* filename */) {
    // TODO
}

void QGWindow::pack() {
    // TODO
}

void QGWindow::pause(double ms) {
    QThread* thread = _qwindow.thread();
    if (thread) {
        thread->msleep((long) ms);
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
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    if (position == QGBorderLayout::North) {
        _qwindow._northLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::South) {
        _qwindow._southLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::West) {
        _qwindow._westLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::East) {
        _qwindow._eastLayout.removeWidget(interactor->getWidget());
    } else {
        _qwindow._centerLayout.removeWidget(interactor->getWidget());
    }
}

void QGWindow::repaint() {
    if (_canvas) {
        _canvas->repaint();
    }
}

void QGWindow::requestFocus() {
    _qwindow.setFocus();
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
    _qwindow.setGeometry((int) x, (int) y, getWidth(), getHeight());
}

void QGWindow::setLocation(const GPoint& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setRepaintImmediately(bool repaintImmediately) {
    ensureCanvas();
    _canvas->setAutoRepaint(repaintImmediately);
}

void QGWindow::setResizable(bool resizable) {
    if (resizable) {
        if (!_resizable) {
            _qwindow.resize((int) getWidth(), (int) getHeight());
            _qwindow.setMinimumSize(_qwindow.minimumSizeHint());
            GDimension screenSize = getScreenSize();
            _qwindow.setMaximumSize((int) screenSize.getWidth(), (int) screenSize.getHeight());
        }
    } else {
        if (_resizable) {
            _qwindow.setFixedSize((int) getWidth(), (int) getHeight());
        }
    }
    _resizable = resizable;
}

void QGWindow::setSize(double width, double height) {
    _qwindow.resize((int) width, (int) height);
}

void QGWindow::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void QGWindow::setTitle(const std::string& title) {
    _qwindow.setWindowTitle(QString::fromStdString(title));
}

void QGWindow::setVisible(bool visible) {
    _qwindow.setVisible(visible);
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
    _qwindow.lower();
}

void QGWindow::toFront() {
    _qwindow.raise();
}

class QGWindowThread : public QThread {
public:
    QGWindowThread(int (* mainFunc)(void))
            : _mainFunc(mainFunc) {
        // empty
    }

    int getResult() const {
        return _result;
    }

protected:
    void run() {
        this->msleep(200);
        _result = _mainFunc();
    }

private:
    int (* _mainFunc)(void);
    int _result;
};

void startBackgroundEventLoop(int (* mainFunc)(void)) {
    // TODO: make it possible to stop;  exitGraphics?

    // assumes that this is being called from the main thread
    if (QGWindow::_app) {
        // start student's main function in its own second thread
        static QGWindowThread* thread = nullptr;
        thread = new QGWindowThread(mainFunc);
        thread->start();

        // QGWindow::_app->moveToThread(this);
        QGWindow::_app->exec();   // start Qt event loop on main thread
    }
}

void startEventLoop() {
    if (QGWindow::_app) {
        QGWindow::_app->exec();
    }
}
