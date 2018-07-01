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
#include "qgcolor.h"
#include "strlib.h"

_Q_Internal_Window::_Q_Internal_Window(QGWindow* window, QWidget* parent)
        : QMainWindow(parent),
          _qgwindow(window) {

    QWidget* dummyWidget = new QWidget(this);
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
        // TODO: throw: "need to initialize Qt"
    }
    _lastWindow = &_qwindow;
    setSize(width, height);
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
    QGBorderLayout::clearLayout(&_qwindow._northLayout);
    QGBorderLayout::clearLayout(&_qwindow._southLayout);
    QGBorderLayout::clearLayout(&_qwindow._westLayout);
    QGBorderLayout::clearLayout(&_qwindow._eastLayout);
    QGBorderLayout::clearLayout(&_qwindow._centerLayout);
    _qwindow.update();
}

void QGWindow::clearCanvas() {
    if (_canvas) {
        _canvas->clear();
    }
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
    // TODO
    return GPoint();
}

GPoint QGWindow::drawPolarLine(double x0, double y0, double r, double theta) {
    // TODO
    return GPoint();
}

void QGWindow::drawPixel(double x, double y) {
    // TODO
}

void QGWindow::drawPixel(double x, double y, int color) {
    // TODO
}

void QGWindow::drawPixel(double x, double y, const std::string& color) {
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
    // TODO
}

void QGWindow::fillRect(double x, double y, double width, double height) {
    ensureCanvas();
    QGRect* rect = new QGRect(x, y, width, height);
    initializeQGObject(rect, /* filled */ true);
    _canvas->add(rect);
}

double QGWindow::getCanvasHeight() const {
    if (_canvas) {
        // TODO
        // return _canvas->getHeight();
    }
    return 0;
}

GDimension QGWindow::getCanvasSize() const {
    if (_canvas) {
        // TODO
        // return GDimension(_canvas->getWidth(), _canvas->getHeight());
    }
    GDimension size;
    return size;
}

double QGWindow::getCanvasWidth() const {
    if (_canvas) {
        // TODO
        // return _canvas->getWidth();
    }
    return 0;
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

bool QGWindow::isResizable() const {
    return _resizable;
}

bool QGWindow::isVisible() const {
    return _qwindow.isVisible();
}

void QGWindow::pack() {
    // TODO
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
    // TODO
}

void QGWindow::requestFocus() {
    _qwindow.setFocus();
}

void QGWindow::setCanvasHeight(double height) {
    // TODO
}

void QGWindow::setCanvasSize(double width, double height) {
    // TODO
}

void QGWindow::setCanvasSize(const GDimension& size) {
    // TODO
}

void QGWindow::setCanvasWidth(double width) {
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

void setSize(const GDimension& size) {

}

void QGWindow::setTitle(const std::string& title) {
    _qwindow.setWindowTitle(QString::fromStdString(title));
}

void QGWindow::setVisible(bool visible) {
    _qwindow.setVisible(visible);
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

void startEventLoop() {
    if (QGWindow::_app) {
        QGWindow::_app->exec();
    }
}
