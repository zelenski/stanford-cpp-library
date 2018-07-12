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

//bool _Internal_QMainWindow::event(QEvent* event) {
//    // TODO
//    if (event->type() == (QEvent::Type) (QEvent::User + 106)) {
//        // cout << "user event! type=" << event->type() << endl;
//    }

//    return QMainWindow::event(event);
//}

void _Internal_QMainWindow::changeEvent(QEvent* event) {
    QMainWindow::changeEvent(event);   // call super
    if (event->type() != QEvent::WindowStateChange) {
        return;
    }

    // https://doc.qt.io/Qt-5/qt.html#WindowState-enum
    QWindowStateChangeEvent* stateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);
    Qt::WindowStates state = windowState();
    bool wasMaximized = (stateChangeEvent->oldState() & Qt::WindowMaximized) != 0;
    bool wasMinimized = (stateChangeEvent->oldState() & Qt::WindowMinimized) != 0;
    bool isMaximized = (state & Qt::WindowMaximized) != 0;
    bool isMinimized = (state & Qt::WindowMinimized) != 0;
    if (!wasMaximized && isMaximized) {
        _qgwindow->fireQGEvent(stateChangeEvent, QGEvent::WINDOW_MAXIMIZED, "maximize");
    } else if (!wasMinimized && isMinimized) {
        _qgwindow->fireQGEvent(stateChangeEvent, QGEvent::WINDOW_MINIMIZED, "minimize");
    } else if ((wasMinimized || wasMaximized) && !isMinimized && !isMaximized) {
        _qgwindow->fireQGEvent(stateChangeEvent, QGEvent::WINDOW_RESTORED, "restore");
    }
}

void _Internal_QMainWindow::closeEvent(QCloseEvent* event) {
    QMainWindow::closeEvent(event);   // call super
    QGWindow::CloseOperation closeOp = _qgwindow->getCloseOperation();
    if (closeOp == QGWindow::CLOSE_DO_NOTHING) {
        event->ignore();
        return;
    }

    // send "closing" event before window closes
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_CLOSING, "closing");

    // send "close" event after window closes
    event->accept();
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_CLOSED, "close");

    if (closeOp == QGWindow::CLOSE_EXIT) {
        // TODO: exit app
    }
}

void _Internal_QMainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);   // call super
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_RESIZED, "resize");
}

_Internal_QMainWindow* QGWindow::_lastWindow = nullptr;

QGWindow::QGWindow(double width, double height, bool visible)
        : _canvas(nullptr),
          _resizable(true),
          _closeOperation(QGWindow::CLOSE_DISPOSE) {
    _iqmainwindow = new _Internal_QMainWindow(this);
    _lastWindow = _iqmainwindow;

    // go ahead and set up canvas when window is loaded
    ensureForwardTarget();
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

void QGWindow::add(QGInteractor* interactor, double x, double y) {
    interactor->setLocation(x, y);
    addToRegion(interactor, "Center");
}

void QGWindow::add(QGObject* obj) {
    ensureForwardTarget();
    _canvas->add(obj);
}

void QGWindow::add(QGObject* obj, double x, double y) {
    ensureForwardTarget();
    _canvas->add(obj, x, y);
}

void QGWindow::add(QGObject& obj) {
    ensureForwardTarget();
    _canvas->add(obj);
}

void QGWindow::add(QGObject& obj, double x, double y) {
    ensureForwardTarget();
    _canvas->add(obj, x, y);
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
        if (_canvas && widget != _canvas->getWidget()) {
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
    QGForwardDrawingSurface::clear();
    clearRegion(REGION_NORTH);
    clearRegion(REGION_SOUTH);
    clearRegion(REGION_WEST);
    clearRegion(REGION_EAST);
    clearRegion(REGION_CENTER);
    repaint();
}

void QGWindow::clearCanvas() {
    QGForwardDrawingSurface::clear();
}

void QGWindow::clearCanvasObjects() {
    if (_canvas) {
        _canvas->clearObjects();
    }
}

void QGWindow::clearCanvasPixels() {
    if (_canvas) {
        _canvas->clearPixels();
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

void QGWindow::ensureForwardTarget() {
    if (!_canvas) {
        _canvas = new QGCanvas(_iqmainwindow);
        setDrawingForwardTarget(_canvas);
        addToRegion(_canvas, "Center");
    }
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

QGWindow::CloseOperation QGWindow::getCloseOperation() const {
    return _closeOperation;
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

std::string QGWindow::getType() const {
    return "QGWindow";
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

bool QGWindow::isMaximized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMaximized) != 0
            || (_iqmainwindow->windowState() & Qt::WindowFullScreen) != 0;
}

bool QGWindow::isMinimized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMinimized) != 0;
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

void QGWindow::removeClickHandler() {
    if (_canvas) {
        _canvas->removeClickHandler();
    }
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

void QGWindow::removeKeyHandler() {
    if (_canvas) {
        _canvas->removeKeyHandler();
    }
}

void QGWindow::removeMouseHandler() {
    if (_canvas) {
        _canvas->removeMouseHandler();
    }
}

void QGWindow::requestFocus() {
    _iqmainwindow->setFocus();
}

void QGWindow::saveCanvasPixels(const std::string& /* filename */) {
    // TODO
    ensureForwardTarget();
}

void QGWindow::setBackground(int color) {
    QGForwardDrawingSurface::setBackground(color);
    // TODO: set background of N/S/E/W regions and central region?
}

void QGWindow::setBackground(const std::string& color) {
    QGForwardDrawingSurface::setBackground(color);
    // TODO: set background of N/S/E/W regions and central region?
}

void QGWindow::setCanvasHeight(double height) {
    // TODO
    ensureForwardTarget();
    setHeight(height);
}

void QGWindow::setCanvasSize(double width, double height) {
    // TODO
    ensureForwardTarget();
    setSize(width, height);
}

void QGWindow::setCanvasSize(const GDimension& size) {
    // TODO
    ensureForwardTarget();
    setSize(size);
}

void QGWindow::setCanvasWidth(double width) {
    // TODO
    ensureForwardTarget();
    setWidth(width);
}

void QGWindow::setCloseOperation(CloseOperation op) {
    _closeOperation = op;
    _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, op == QGWindow::CLOSE_EXIT);
}

void QGWindow::setExitOnClose(bool /* exitOnClose */) {
    // TODO
}

void QGWindow::setHeight(double height) {
    setSize(getWidth(), height);
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

void QGWindow::setClickHandler(QGEventHandler func) {
    _canvas->setClickHandler(func);
}

void QGWindow::setClickHandler(QGEventHandlerVoid func) {
    _canvas->setClickHandler(func);
}

void QGWindow::setKeyHandler(QGEventHandler func) {
    _canvas->setKeyHandler(func);
}

void QGWindow::setKeyHandler(QGEventHandlerVoid func) {
    _canvas->setKeyHandler(func);
}

void QGWindow::setMouseHandler(QGEventHandler func) {
    _canvas->setMouseHandler(func);
}

void QGWindow::setMouseHandler(QGEventHandlerVoid func) {
    _canvas->setMouseHandler(func);
}

void QGWindow::setRegionAlignment(Region /* region */, Alignment /* align */) {
    // TODO
}

void QGWindow::setRegionAlignment(const std::string& /* region */, const std::string& /* align */) {
    // TODO
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

void QGWindow::setWindowHandler(QGEventHandler func) {
    setEventHandlers({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void QGWindow::setWindowHandler(QGEventHandlerVoid func) {
    setEventHandlers({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
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
