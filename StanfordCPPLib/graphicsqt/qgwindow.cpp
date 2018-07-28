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
#include <QSizePolicy>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include "qgcolor.h"
#include "qgui.h"
#include "strlib.h"

const int _Internal_QMainWindow::SPACING = 5;
const int _Internal_QMainWindow::MARGIN = 5;

_Internal_QMainWindow::_Internal_QMainWindow(QGWindow* qgwindow, QWidget* parent)
        : QMainWindow(parent),
          _qgwindow(qgwindow),
          _timerID(-1) {
    QGui::instance()->ensureThatThisIsTheQtGuiThread("QGWindow");

    QWidget* dummyWidget = new QWidget(this);   // TODO: memory leak?

    _overallLayout = new QVBoxLayout;   // TODO: set parent? to dummyWidget or this?
    _northLayout   = new QHBoxLayout;
    _southLayout   = new QHBoxLayout;
    _westLayout    = new QVBoxLayout;
    _eastLayout    = new QVBoxLayout;
    _centerLayout  = new QHBoxLayout;
    _middleLayout  = new QHBoxLayout;

    // squish margins/padding
    _overallLayout->setSpacing(0);
    _northLayout->setSpacing(SPACING);
    _southLayout->setSpacing(SPACING);
    _westLayout->setSpacing(SPACING);
    _eastLayout->setSpacing(SPACING);
    _centerLayout->setSpacing(0);
    _middleLayout->setSpacing(0);

    _overallLayout->setMargin(0);
    _northLayout->setMargin(0);
    _southLayout->setMargin(0);
    _westLayout->setMargin(0);
    _eastLayout->setMargin(0);
    _centerLayout->setMargin(0);
    _middleLayout->setMargin(0);

//    _overallLayout->setContentsMargins(0, 0, 0, 0);
//    _northLayout->setContentsMargins(0, 0, 0, 0);
//    _southLayout->setContentsMargins(0, 0, 0, 0);
//    _westLayout->setContentsMargins(0, 0, 0, 0);
//    _eastLayout->setContentsMargins(0, 0, 0, 0);
//    _centerLayout->setContentsMargins(0, 0, 0, 0);
//    _middleLayout->setContentsMargins(0, 0, 0, 0);

    _northLayout->addStretch(99);
    _northLayout->addStretch(99);
    _southLayout->addStretch(99);
    _southLayout->addStretch(99);
    _westLayout->addStretch(99);
    _westLayout->addStretch(99);
    _eastLayout->addStretch(99);
    _eastLayout->addStretch(99);

    _overallLayout->addLayout(_northLayout, /* stretch */ 0);
    _middleLayout->addLayout(_westLayout, /* stretch */ 0);
    _middleLayout->addLayout(_centerLayout, /* stretch */ 99);
    _middleLayout->addLayout(_eastLayout, /* stretch */ 0);
    _overallLayout->addLayout(_middleLayout, /* stretch */ 99);
    _overallLayout->addLayout(_southLayout, /* stretch */ 0);
    dummyWidget->setLayout(_overallLayout);
    layout()->setSpacing(0);
    layout()->setMargin(0);
//    layout()->setContentsMargins(0, 0, 0, 0);

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
    // send "closing" event before window closes
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_CLOSING, "closing");

    QGWindow::CloseOperation closeOp = _qgwindow->getCloseOperation();
    if (closeOp == QGWindow::CLOSE_DO_NOTHING) {
        event->ignore();
        return;
    }

    // send "close" event after window closes
    event->accept();
    QMainWindow::closeEvent(event);   // call super
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_CLOSED, "close");

    if (closeOp == QGWindow::CLOSE_EXIT) {
        // exit app
        QGui::instance()->exitGraphics();
    }
}

void _Internal_QMainWindow::fixMargins() {
//    _northLayout->setSpacing(_northLayout->isEmpty() ? 0 : SPACING);
//    _southLayout->setSpacing(_southLayout->isEmpty() ? 0 : SPACING);
//    _westLayout->setSpacing(_westLayout->isEmpty() ? 0 : SPACING);
//    _eastLayout->setSpacing(_eastLayout->isEmpty() ? 0 : SPACING);

    _northLayout->setMargin(_northLayout->isEmpty() ? 0 : MARGIN);
    _southLayout->setMargin(_southLayout->isEmpty() ? 0 : MARGIN);
    _westLayout->setMargin(_westLayout->isEmpty() ? 0 : MARGIN);
    _eastLayout->setMargin(_eastLayout->isEmpty() ? 0 : MARGIN);
}

void _Internal_QMainWindow::keyPressEvent(QKeyEvent* event) {
    QMainWindow::keyPressEvent(event);   // call super
    _qgwindow->processKeyPressEventInternal(event);
}

void _Internal_QMainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);   // call super
    _qgwindow->fireQGEvent(event, QGEvent::WINDOW_RESIZED, "resize");
}

void _Internal_QMainWindow::timerEvent(QTimerEvent* event) {
    QMainWindow::timerEvent(event);   // call super
    _qgwindow->fireQGEvent(event, QGEvent::TIMER_TICKED, "timer");
}

bool _Internal_QMainWindow::timerExists() {
    return _timerID >= 0;
}

void _Internal_QMainWindow::timerStart(double ms) {
    // TODO: start from Qt GUI thread?
    // QTimer* timer = new QTimer(this);
    // connect(timer, SIGNAL(timeout()), this, SLOT(processTimerEvent()));
    // timer->start((int) ms);
    // TODO: when to free timer memory?

    if (timerExists()) {
        timerStop();
    }
    _timerID = startTimer((int) ms);
}

void _Internal_QMainWindow::timerStop() {
    // TODO: start from Qt GUI thread?
    // QTimer* timer = new QTimer(this);
    // connect(timer, SIGNAL(timeout()), this, SLOT(processTimerEvent()));
    // timer->start((int) ms);
    // TODO: when to free timer memory?

    if (timerExists()) {
        killTimer(_timerID);
        _timerID = -1;
    }
}

_Internal_QMainWindow* QGWindow::_lastWindow = nullptr;

QGWindow::QGWindow(double width, double height, bool visible)
        : _canvas(nullptr),
          _resizable(true),
          _closeOperation(QGWindow::CLOSE_DISPOSE) {
    QGui::instance()->ensureThatThisIsTheQtGuiThread("QGWindow::constructor");
    QGui::instance()->initializeQt();
    _iqmainwindow = new _Internal_QMainWindow(this);
    _lastWindow = _iqmainwindow;

    // go ahead and set up canvas when window is loaded
    ensureForwardTarget();
    if (width > 0 && height > 0) {
        setSize(width, height);
    }
    setVisible(visible);

    _iqmainwindow->updateGeometry();
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
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = layoutForRegion(region);
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

    // set alignment of widget
    if (_halignMap.containsKey(region) && _valignMap.containsKey(region)) {
        layout->setAlignment(widget, toQtAlignment(_halignMap[region]) | toQtAlignment(_valignMap[region]));
    } else if (_halignMap.containsKey(region)) {
        layout->setAlignment(widget, toQtAlignment(_halignMap[region]));
    } else if (_valignMap.containsKey(region)) {
        layout->setAlignment(widget, toQtAlignment(_valignMap[region]));
    }

//    layout->update();
//    _iqmainwindow->updateGeometry();
//    _iqmainwindow->update();
    _iqmainwindow->fixMargins();
    QGBorderLayout::forceUpdate(_iqmainwindow->centralWidget());
}

void QGWindow::addToRegion(QGInteractor* interactor, const std::string& region) {
    addToRegion(interactor, stringToRegion(region));
}

void QGWindow::clearConsole() {
    QGForwardDrawingSurface::clearConsole();
    clearRegion(REGION_NORTH);
    clearRegion(REGION_SOUTH);
    clearRegion(REGION_WEST);
    clearRegion(REGION_EAST);
    clearRegion(REGION_CENTER);
    repaint();
}

void QGWindow::clearCanvas() {
    QGForwardDrawingSurface::clearConsole();
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
    QLayout* layout = layoutForRegion(region);
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

void QGWindow::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

void QGWindow::center() {
    QGDimension screenSize = getScreenSize();
    QGDimension windowSize = getSize();
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
        // tell canvas to take any unclaimed space in the window
        _canvas = new QGCanvas(_iqmainwindow);
        _canvas->getWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setDrawingForwardTarget(_canvas);
        addToRegion(_canvas, "Center");
    }
}

bool QGWindow::eventsEnabled() const {
    return getWidget() != nullptr && isVisible();
}

double QGWindow::getCanvasHeight() const {
    return _canvas->getHeight();
}

QGDimension QGWindow::getCanvasSize() const {
    return QGDimension(_canvas->getWidth(), _canvas->getHeight());
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

QGPoint QGWindow::getLocation() const {
    QRect geom = _iqmainwindow->geometry();
    return QGPoint(geom.x(), geom.y());
}

double QGWindow::getHeight() const {
    return _iqmainwindow->geometry().height();
}

QGDimension QGWindow::getPreferredSize() const {
    // make sure the layout has calculated everybody's position/size
    QGBorderLayout::forceUpdate(_iqmainwindow->centralWidget());

//    QRect windowGeom = _iqmainwindow->geometry();
//    QRect frameGeom = _iqmainwindow->frameGeometry();

//    QSize centralWidgetSize = _iqmainwindow->centralWidget()->sizeHint();
//    QRect centralWidgetGeom = _iqmainwindow->centralWidget()->geometry();

//    QSize size = _iqmainwindow->sizeHint();
    QRect northGeom = _iqmainwindow->_northLayout->geometry();
    QSize north = _iqmainwindow->_northLayout->sizeHint();
//    QSize northMin = _iqmainwindow->_northLayout->minimumSize();

//    QRect middleGeom = _iqmainwindow->_middleLayout->geometry();
//    QSize middle = _iqmainwindow->_middleLayout->sizeHint();
//    QSize middleMin = _iqmainwindow->_middleLayout->minimumSize();

    QRect southGeom = _iqmainwindow->_southLayout->geometry();
    QSize south = _iqmainwindow->_southLayout->sizeHint();
//    QSize southMin = _iqmainwindow->_southLayout->minimumSize();

    QRect westGeom = _iqmainwindow->_westLayout->geometry();
    QSize west = _iqmainwindow->_westLayout->sizeHint();
//    QSize westMin = _iqmainwindow->_westLayout->minimumSize();

    QRect eastGeom = _iqmainwindow->_eastLayout->geometry();
    QSize east = _iqmainwindow->_eastLayout->sizeHint();
//    QSize eastMin = _iqmainwindow->_eastLayout->minimumSize();

//    QRect centerGeom = _iqmainwindow->_centerLayout->geometry();
    QSize center = _iqmainwindow->_centerLayout->sizeHint();
//    QSize centerMin = _iqmainwindow->_centerLayout->minimumSize();

//    QRect canvasGeom = _canvas->getWidget()->geometry();
//    QSize canvas = _canvas->getWidget()->sizeHint();
//    QSize canvasMin = _canvas->getWidget()->minimumSize();

//    double northHeight = centerGeom.y();
//    double westEastPadding = centerGeom.x() - westMin.width();
//    double northSouthPadding = northHeight - northMin.height();

//    double westEastPadding = _Internal_QMainWindow::MARGIN;
//    double northSouthPadding = _Internal_QMainWindow::MARGIN;

    double northHeight = northGeom.height() > 0 ? northGeom.height() : north.height();
    double southHeight = southGeom.height() > 0 ? southGeom.height() : south.height();
    double westWidth = westGeom.width() > 0 ? westGeom.width() : west.width();
    double eastWidth = eastGeom.width() > 0 ? eastGeom.width() : east.width();

    double centerWidth = _canvas && _canvas->isVisible() ? _canvas->getPreferredSize().getWidth()
                                                         : (double) center.width();
    double centerHeight = _canvas && _canvas->isVisible() ? _canvas->getPreferredSize().getHeight()
                                                          : (double) center.height();

    double windowPreferredWidth = centerWidth + westWidth + eastWidth;
    double windowPreferredHeight = centerHeight + northHeight + southHeight;
    return QGDimension(windowPreferredWidth, windowPreferredHeight);
}

double QGWindow::getRegionHeight(Region region) const {
    return getRegionSize(region).getHeight();
}

double QGWindow::getRegionHeight(const std::string& region) const {
    return getRegionHeight(stringToRegion(region));
}

QGDimension QGWindow::getRegionSize(Region region) const {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return QGDimension();
    } else {
        QRect geom = layout->geometry();
        return QGDimension(geom.width(), geom.height());
    }
}

QGDimension QGWindow::getRegionSize(const std::string& region) const {
    return getRegionSize(stringToRegion(region));
}

double QGWindow::getRegionWidth(Region region) const {
    return getRegionSize(region).getWidth();
}

double QGWindow::getRegionWidth(const std::string& region) const {
    return getRegionWidth(stringToRegion(region));
}

double QGWindow::getScreenHeight() {
    return getScreenSize().getHeight();
}

QGDimension QGWindow::getScreenSize() {
    QRect rec = QApplication::desktop()->availableGeometry();
    return QGDimension(rec.width(), rec.height());
}

double QGWindow::getScreenWidth() {
    return getScreenSize().getWidth();
}

QGDimension QGWindow::getSize() const {
    QRect geom = _iqmainwindow->geometry();
    return QGDimension(geom.width(), geom.height());
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

void QGWindow::loadCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->load(filename);
}

void QGWindow::pack() {
    setSize(getPreferredSize());
}

void QGWindow::pause(double ms) {
    QThread* thread = _iqmainwindow->thread();
    if (thread) {
        thread->msleep((long) ms);
    }
}

QLayout* QGWindow::layoutForRegion(Region region) const {
    if (region == REGION_NORTH) {
        return _iqmainwindow->_northLayout;
    } else if (region == REGION_SOUTH) {
        return _iqmainwindow->_southLayout;
    } else if (region == REGION_WEST) {
        return _iqmainwindow->_westLayout;
    } else if (region == REGION_EAST) {
        return _iqmainwindow->_eastLayout;
    } else {
        return _iqmainwindow->_centerLayout;
    }
}

QLayout* QGWindow::layoutForRegion(const std::string& region) const {
    return layoutForRegion(stringToRegion(region));
}

void QGWindow::processKeyPressEventInternal(QKeyEvent* /* event */) {
    // empty; override me
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

void QGWindow::removeClickListener() {
    if (_canvas) {
        _canvas->removeClickListener();
    }
}

void QGWindow::removeFromRegion(QGInteractor* interactor, Region region) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    layout->removeWidget(widget);
    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::removeFromRegion(QGInteractor* interactor, const std::string& region) {
    removeFromRegion(interactor, stringToRegion(region));
}

void QGWindow::removeKeyListener() {
    if (_canvas) {
        _canvas->removeKeyListener();
    }
}

void QGWindow::removeMouseListener() {
    if (_canvas) {
        _canvas->removeMouseListener();
    }
}

void QGWindow::removeTimerListener() {
    removeEventListener("timer");
}

void QGWindow::removeWindowListener() {
    removeEventListeners({"close",
                         "closing",
                         "maximize",
                         "minimize",
                         "open",
                         "resize",
                         "restore"});
}

void QGWindow::requestFocus() {
    _iqmainwindow->setFocus();
}

void QGWindow::saveCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->save(filename);
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
    ensureForwardTarget();
    setCanvasSize(getCanvasWidth(), height);
}

void QGWindow::setCanvasSize(double width, double height) {
    ensureForwardTarget();
    _canvas->setMinimumSize(width, height);
    _canvas->setPreferredSize(width, height);
    pack();
}

void QGWindow::setCanvasSize(const QGDimension& size) {
    setCanvasSize(size.getWidth(), size.getHeight());
}

void QGWindow::setCanvasWidth(double width) {
    ensureForwardTarget();
    setCanvasSize(width, getCanvasHeight());
}

void QGWindow::setCloseOperation(CloseOperation op) {
    _closeOperation = op;
    _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, op == QGWindow::CLOSE_EXIT);
}

void QGWindow::setExitOnClose(bool exitOnClose) {
    if (exitOnClose) {
        setCloseOperation(QGWindow::CLOSE_EXIT);
    } else {
        setCloseOperation(_closeOperation == QGWindow::CLOSE_EXIT ? QGWindow::CLOSE_HIDE : _closeOperation);
    }
}

void QGWindow::setHeight(double height) {
    setSize(getWidth(), height);
}

void QGWindow::setLocation(double x, double y) {
    _iqmainwindow->setGeometry((int) x, (int) y, getWidth(), getHeight());
}

void QGWindow::setLocation(const QGPoint& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void QGWindow::setClickListener(QGEventListener func) {
    _canvas->setClickListener(func);
}

void QGWindow::setClickListener(QGEventListenerVoid func) {
    _canvas->setClickListener(func);
}

void QGWindow::setKeyListener(QGEventListener func) {
    _canvas->setKeyListener(func);
}

void QGWindow::setKeyListener(QGEventListenerVoid func) {
    _canvas->setKeyListener(func);
}

void QGWindow::setMouseListener(QGEventListener func) {
    _canvas->setMouseListener(func);
}

void QGWindow::setMouseListener(QGEventListenerVoid func) {
    _canvas->setMouseListener(func);
}

void QGWindow::setRegionAlignment(Region region, qgenum::HorizontalAlignment halign) {
    setRegionHorizontalAlignment(region, halign);
}

void QGWindow::setRegionAlignment(Region region, qgenum::VerticalAlignment valign) {
    setRegionVerticalAlignment(region, valign);
}

void QGWindow::setRegionAlignment(Region region, qgenum::HorizontalAlignment halign, qgenum::VerticalAlignment valign) {
    setRegionHorizontalAlignment(region, halign);
    setRegionVerticalAlignment(region, valign);
}

void QGWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    qgenum::HorizontalAlignment halignment = qgenum::toHorizontalAlignment(align);
    qgenum::VerticalAlignment valignment = qgenum::toVerticalAlignment(align);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void QGWindow::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    qgenum::HorizontalAlignment halignment = qgenum::toHorizontalAlignment(halign);
    qgenum::VerticalAlignment valignment = qgenum::toVerticalAlignment(valign);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void QGWindow::setRegionHorizontalAlignment(Region region, qgenum::HorizontalAlignment halign) {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    _halignMap[region] = halign;
    if (layout == _iqmainwindow->_centerLayout) {
        // disallow?
        return;
    }

    if (layout == _iqmainwindow->_westLayout || layout == _iqmainwindow->_eastLayout) {
        // set each widget's horizontal alignment individually
        Qt::Alignment qtAlign = toQtAlignment(halign);
        for (int i = 0; i < layout->count(); i++) {
            QWidget* widget = layout->itemAt(i)->widget();
            layout->setAlignment(widget, qtAlign);
        }
    } else if (layout == _iqmainwindow->_northLayout || layout == _iqmainwindow->_southLayout) {
        // to align "left", limit first stretch;
        // to align "right", limit last stretch
        layout->removeItem(layout->itemAt(0));
        layout->removeItem(layout->itemAt(layout->count() - 1));
        if (halign == qgenum::ALIGN_LEFT) {
            ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 0);
            ((QHBoxLayout*) layout)->addStretch(/* stretch */ 99);
        } else if (halign == qgenum::ALIGN_RIGHT) {
            ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
            ((QHBoxLayout*) layout)->addStretch(/* stretch */ 0);
        } else {   // halign == qgenum::ALIGN_CENTER
            ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
            ((QHBoxLayout*) layout)->addStretch(/* stretch */ 99);
        }
    }

    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    setRegionHorizontalAlignment(stringToRegion(region), qgenum::toHorizontalAlignment(halign));
}

void QGWindow::setRegionVerticalAlignment(Region region, qgenum::VerticalAlignment valign) {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    _valignMap[region] = valign;
    if (layout == _iqmainwindow->_centerLayout) {
        // disallow?
        return;
    }

    if (layout == _iqmainwindow->_westLayout || layout == _iqmainwindow->_eastLayout) {
        // to align "top", limit first stretch;
        // to align "bottom", limit last stretch
        layout->removeItem(layout->itemAt(0));
        layout->removeItem(layout->itemAt(layout->count() - 1));
        if (valign == qgenum::ALIGN_TOP) {
            ((QVBoxLayout*) layout)->insertStretch(0, /* stretch */ 0);
            ((QVBoxLayout*) layout)->addStretch(/* stretch */ 99);
        } else if (valign == qgenum::ALIGN_BOTTOM) {
            ((QVBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
            ((QVBoxLayout*) layout)->addStretch(/* stretch */ 0);
        } else {   // valign == qgenum::ALIGN_MIDDLE
            ((QVBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
            ((QVBoxLayout*) layout)->addStretch(/* stretch */ 99);
        }
    } else if (layout == _iqmainwindow->_northLayout || layout == _iqmainwindow->_southLayout) {
        // set each widget's vertical alignment individually
        Qt::Alignment qtAlign = toQtAlignment(valign);
        for (int i = 0; i < layout->count(); i++) {
            QWidget* widget = layout->itemAt(i)->widget();
            layout->setAlignment(widget, qtAlign);
        }
    }

    layout->update();
    _iqmainwindow->updateGeometry();
    _iqmainwindow->update();
}

void QGWindow::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    setRegionVerticalAlignment(stringToRegion(region), qgenum::toVerticalAlignment(valign));
}

void QGWindow::setResizable(bool resizable) {
    if (resizable) {
        if (!_resizable) {
            _iqmainwindow->resize((int) getWidth(), (int) getHeight());
            _iqmainwindow->setMinimumSize(_iqmainwindow->minimumSizeHint());
            QGDimension screenSize = getScreenSize();
            _iqmainwindow->setMaximumSize((int) screenSize.getWidth(), (int) screenSize.getHeight());
            _iqmainwindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
    } else {
        if (_resizable) {
            _iqmainwindow->setFixedSize(_iqmainwindow->size());
            _iqmainwindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
    }
    _resizable = resizable;
}

void QGWindow::setSize(double width, double height) {
    if (isResizable()) {
        _iqmainwindow->resize((int) width, (int) height);
    } else {
        _iqmainwindow->setFixedSize((int) width, (int) height);
    }
}

void QGWindow::setSize(const QGDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void QGWindow::setTimerListener(double ms, QGEventListener func) {
    setEventListener("timer", func);
    _iqmainwindow->timerStart(ms);
}

void QGWindow::setTimerListener(double ms, QGEventListenerVoid func) {
    setEventListener("timer", func);
    _iqmainwindow->timerStart(ms);
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

void QGWindow::setWindowListener(QGEventListener func) {
    setEventListeners({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void QGWindow::setWindowListener(QGEventListenerVoid func) {
    setEventListeners({"close",
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

void QGWindow::sleep(double ms) {
    QThread* thread = _iqmainwindow->thread();
    if (thread) {
        thread->msleep((long) ms);
    }
}

QGWindow::Region QGWindow::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north")) {
        return QGWindow::REGION_NORTH;
    } else if (stringContains(regionLC, "south")) {
        return QGWindow::REGION_SOUTH;
    } else if (stringContains(regionLC, "west")) {
        return QGWindow::REGION_WEST;
    } else if (stringContains(regionLC, "east")) {
        return QGWindow::REGION_EAST;
    } else {
        return QGWindow::REGION_CENTER;
    }
}

void QGWindow::toBack() {
    _iqmainwindow->lower();
}

void QGWindow::toFront() {
    _iqmainwindow->raise();
}