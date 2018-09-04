/*
 * File: gwindow.cpp
 * -----------------
 *
 * @version 2018/08/23
 * - renamed to gwindow.h to replace Java version
 * @version 2018/07/03
 * - thread safety code
 * @version 2018/07/01
 * - 2D graphics/painting functions
 * @version 2018/06/25
 * - initial version
 */

#include "gwindow.h"
#include <QDesktopWidget>
#include <QMenu>
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include "filelib.h"
#include "gcolor.h"
#include "glabel.h"
#include "gthread.h"
#include "qtgui.h"
#include "strlib.h"

const int _Internal_QMainWindow::SPACING = 5;
const int _Internal_QMainWindow::MARGIN = 5;

_Internal_QMainWindow::_Internal_QMainWindow(GWindow* gwindow, QWidget* parent)
        : QMainWindow(parent),
          _gwindow(gwindow),
          _timerID(-1) {
    GThread::ensureThatThisIsTheQtGuiThread("GWindow internal initialization");

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
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_MAXIMIZED, "maximize");
    } else if (!wasMinimized && isMinimized) {
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_MINIMIZED, "minimize");
    } else if ((wasMinimized || wasMaximized) && !isMinimized && !isMaximized) {
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_RESTORED, "restore");
    }
}

void _Internal_QMainWindow::closeEvent(QCloseEvent* event) {
    // send "closing" event before window closes
    _gwindow->fireGEvent(event, WINDOW_CLOSING, "closing");

    GWindow::CloseOperation closeOp = _gwindow->getCloseOperation();
    if (closeOp == GWindow::CLOSE_DO_NOTHING) {
        event->ignore();
        return;
    }

    // send "close" event after window closes
    event->accept();
    QMainWindow::closeEvent(event);   // call super
    _gwindow->fireGEvent(event, WINDOW_CLOSED, "close");

    if (closeOp == GWindow::CLOSE_EXIT) {
        // exit app
        QtGui::instance()->exitGraphics();
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

void _Internal_QMainWindow::handleMenuAction(const std::string& menu, const std::string& item) {
    GEvent actionEvent(
                /* class  */ ACTION_EVENT,
                /* type   */ ACTION_MENU,
                /* name   */ "actionMenu",
                /* source */ _gwindow);
    actionEvent.setActionCommand(menu + "/" + item);
    _gwindow->fireEvent(actionEvent);
}

void _Internal_QMainWindow::keyPressEvent(QKeyEvent* event) {
    QMainWindow::keyPressEvent(event);   // call super
    _gwindow->processKeyPressEventInternal(event);
}

void _Internal_QMainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);   // call super
    _gwindow->fireGEvent(event, WINDOW_RESIZED, "resize");
}

void _Internal_QMainWindow::timerEvent(QTimerEvent* event) {
    QMainWindow::timerEvent(event);   // call super
    _gwindow->fireGEvent(event, TIMER_TICKED, "timer");
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

_Internal_QMainWindow* GWindow::_lastWindow = nullptr;

GWindow::GWindow(double width, double height, bool visible)
        : _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE) {
    GThread::runOnQtGuiThread([this, width, height, visible]() {
        QtGui::instance()->initializeQt();
        _iqmainwindow = new _Internal_QMainWindow(this);
        _lastWindow = _iqmainwindow;

        // go ahead and set up canvas when window is loaded
        ensureForwardTarget();
        if (width > 0 && height > 0) {
            setSize(width, height);
        }
        setVisible(visible);

        _iqmainwindow->updateGeometry();
    });
}

GWindow::~GWindow() {
    if (_lastWindow == _iqmainwindow) {
        _lastWindow = nullptr;
    }
    // TODO: delete _iqmainwindow;
    _iqmainwindow = nullptr;
}

void GWindow::_autograder_setIsAutograderWindow(bool /*isAutograderWindow*/) {
    // TODO
}

void GWindow::_autograder_setExitGraphicsEnabled(bool /*enabled*/) {
    // TODO
}

void GWindow::_autograder_setPauseEnabled(bool /*enabled*/) {
    // TODO
}

void GWindow::add(GInteractor* interactor) {
    addToRegion(interactor, "Center");
}

void GWindow::add(GInteractor* interactor, double x, double y) {
    interactor->setLocation(x, y);
    addToRegion(interactor, "Center");
}

void GWindow::add(GInteractor& interactor) {
    add(&interactor);
}

void GWindow::add(GInteractor& interactor, double x, double y) {
    add(&interactor, x, y);
}

void GWindow::add(GObject* obj) {
    ensureForwardTarget();
    _canvas->add(obj);
}

void GWindow::add(GObject* obj, double x, double y) {
    ensureForwardTarget();
    _canvas->add(obj, x, y);
}

void GWindow::add(GObject& obj) {
    ensureForwardTarget();
    _canvas->add(obj);
}

void GWindow::add(GObject& obj, double x, double y) {
    ensureForwardTarget();
    _canvas->add(obj, x, y);
}

QMenu* GWindow::addMenu(const std::string& menu) {
    QMenu* qmenu = nullptr;
    GThread::runOnQtGuiThread([this, menu, &qmenu]() {
        qmenu = _iqmainwindow->menuBar()->addMenu(QString::fromStdString(menu));
        std::string menuKey = toLowerCase(stringReplace(stringReplace(menu, "/", ""), "&", ""));
        _menuMap[menuKey] = qmenu;
    });
    return qmenu;
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const std::string& icon) {
    GEventListenerVoid func = [this, menu, item]() {
        this->_iqmainwindow->handleMenuAction(menu, item);
    };
    return addMenuItem(menu, item, icon, func);
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const std::string& icon, GEventListenerVoid func) {
    QAction* action = nullptr;
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    GThread::runOnQtGuiThread([this, menu, item, icon, func, menuKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        if (!icon.empty() && fileExists(icon)) {
            QIcon qicon(QString::fromStdString(icon));
            action->setIcon(qicon);
        }

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });

        std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
        _menuActionMap[menuKey + "/" + itemKey] = action;
    });
    return action;
}

QAction* GWindow::addMenuItemCheckBox(const std::string& menu,
                                      const std::string& item,
                                      bool checked,
                                      const std::string& icon) {
    GEventListenerVoid func = [this, menu, item]() {
        this->_iqmainwindow->handleMenuAction(menu, item);
    };
    return addMenuItemCheckBox(menu, item, checked, icon, func);
}

QAction* GWindow::addMenuItemCheckBox(const std::string& menu,
                                      const std::string& item,
                                      bool checked,
                                      const std::string& icon,
                                      GEventListenerVoid func) {
    QAction* action = nullptr;
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    GThread::runOnQtGuiThread([this, menu, item, icon, checked, func, menuKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        action->setCheckable(true);
        action->setChecked(checked);
        if (!icon.empty() && fileExists(icon)) {
            QIcon qicon(QString::fromStdString(icon));
            action->setIcon(qicon);
        }

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });

        std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
        _menuActionMap[menuKey + "/" + itemKey] = action;
    });
    return action;
}


QAction* GWindow::addMenuSeparator(const std::string& menu) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    QAction* separator = nullptr;
    GThread::runOnQtGuiThread([this, menuKey, &separator]() {
        QMenu* qmenu = _menuMap[menuKey];
        separator = qmenu->addSeparator();
    });
    return separator;
}

QMenu* GWindow::addSubMenu(const std::string& menu, const std::string& submenu) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    } else {
        QMenu* qsubmenu = nullptr;
        GThread::runOnQtGuiThread([this, menu, menuKey, submenu, &qsubmenu]() {
            QMenu* qmenu = _menuMap[menuKey];
            qsubmenu = qmenu->addMenu(QString::fromStdString(submenu));
            std::string subMenuKey = menuKey + "/"
                    + toLowerCase(stringReplace(submenu, "&", ""));
            _menuMap[subMenuKey] = qsubmenu;
        });
        return qsubmenu;
    }
}

void GWindow::addToRegion(GInteractor* interactor, Region region) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    // special case: labels in "GText mode" are added to canvas
    if (layout == _iqmainwindow->_centerLayout && interactor->getType() == "GLabel") {
        GLabel* label = (GLabel*) interactor;
        if (label->hasGText()) {
            add(label->getGText());
            return;
        }
    }

    GThread::runOnQtGuiThread([this, region, widget, layout]() {
        if (layout == _iqmainwindow->_centerLayout) {
            // center holds at most one widget
            if (_canvas && widget != _canvas->getWidget()) {
                _canvas->setVisible(false);
            }
            GLayout::clearLayout(layout);
            layout->addWidget(widget);
        } else {
            // add to end of the list of widgets in this region
            ((QBoxLayout*) layout)->insertWidget(/* index */ layout->count() - 1, widget);
        }
        if (!widget->isVisible()) {
            widget->setVisible(true);
        }

        // set alignment of widget
        if (_halignMap.containsKey(region) && _valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]) | toQtAlignment(_valignMap[region]));
        } else if (_halignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]));
        } else if (_valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_valignMap[region]));
        }

        _iqmainwindow->fixMargins();
        GLayout::forceUpdate(_iqmainwindow->centralWidget());
    });
}

void GWindow::addToRegion(GInteractor* interactor, const std::string& region) {
    addToRegion(interactor, stringToRegion(region));
}

void GWindow::addToRegion(GInteractor& interactor, Region region) {
    addToRegion(&interactor, region);
}

void GWindow::addToRegion(GInteractor& interactor, const std::string& region) {
    addToRegion(&interactor, region);
}


void GWindow::clear() {
    GThread::runOnQtGuiThread([this]() {
        clearRegion(REGION_NORTH);
        clearRegion(REGION_SOUTH);
        clearRegion(REGION_WEST);
        clearRegion(REGION_EAST);
        clearRegion(REGION_CENTER);
    });
}

void GWindow::clearCanvas() {
    GThread::runOnQtGuiThread([this]() {
        GForwardDrawingSurface::clear();
        if (_canvas) {
            _canvas->clear();
        }
    });
}

void GWindow::clearCanvasObjects() {
    if (_canvas) {
        _canvas->clearObjects();   // runs on Qt GUI thread
    }
}

void GWindow::clearCanvasPixels() {
    if (_canvas) {
        _canvas->clearPixels();   // runs on Qt GUI thread
    }
}

void GWindow::clearRegion(Region region) {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    GThread::runOnQtGuiThread([this, layout]() {
        if (layout == _iqmainwindow->_centerLayout) {
            GLayout::clearLayout(layout);

            // put the canvas back in the center if there is nothing else there
            // TODO: remove this?
            if (_canvas) {
                layout->addWidget(_canvas->getWidget());
                if (!_canvas->isVisible()) {
                    _canvas->setVisible(true);
                }
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
    });
}

void GWindow::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

void GWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.getWidth()  / 2 - windowSize.getWidth()  / 2,
                screenSize.getHeight() / 2 - windowSize.getHeight() / 2);
}

void GWindow::close() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->close();
    });
}

void GWindow::compareToImage(const std::string& /* filename */, bool /* ignoreWindowSize */) const {
    // TODO
}

void GWindow::ensureForwardTarget() {
    if (!_canvas) {
        // tell canvas to take any unclaimed space in the window
        GThread::runOnQtGuiThread([this]() {
            _canvas = new GCanvas(_iqmainwindow);
            _canvas->getWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setDrawingForwardTarget(_canvas);
            addToRegion(_canvas, "Center");
        });
    }
//    else if (!_canvas->isVisible()) {
//        // put canvas back in center region
//        GThread::runOnQtGuiThread([this]() {
//            QLayout* centerLayout = layoutForRegion(REGION_CENTER);
//            if (!GLayout::contains(centerLayout, _canvas->getWidget())) {
//                GLayout::clearLayout(centerLayout);
//                addToRegion(_canvas, "Center");
//                _canvas->setVisible(true);
//            }
//        });
//    }
}

bool GWindow::eventsEnabled() const {
    return getWidget() != nullptr && isVisible();
}

double GWindow::getCanvasHeight() const {
    return _canvas->getHeight();
}

GDimension GWindow::getCanvasSize() const {
    return GDimension(_canvas->getWidth(), _canvas->getHeight());
}

double GWindow::getCanvasWidth() const {
    return _canvas->getWidth();
}

GWindow::CloseOperation GWindow::getCloseOperation() const {
    return _closeOperation;
}

GObject* GWindow::getGObject(int index) const {
    if (_canvas) {
        return _canvas->getElement(index);
    } else {
        return nullptr;
    }
}

GObject* GWindow::getGObjectAt(double x, double y) const {
    if (_canvas) {
        return _canvas->getElementAt(x, y);
    } else {
        return nullptr;
    }
}

int GWindow::getGObjectCount() const {
    if (_canvas) {
        return _canvas->getElementCount();
    } else {
        return 0;
    }
}

QMainWindow* GWindow::getLastWindow() {
    return _lastWindow;
}

GPoint GWindow::getLocation() const {
    QRect geom = _iqmainwindow->geometry();
    return GPoint(geom.x(), geom.y());
}

double GWindow::getHeight() const {
    return _iqmainwindow->geometry().height();
}

GDimension GWindow::getPreferredSize() const {
    // make sure the layout has calculated everybody's position/size
    GLayout::forceUpdate(_iqmainwindow->centralWidget());

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
    return GDimension(windowPreferredWidth, windowPreferredHeight);
}

double GWindow::getRegionHeight(Region region) const {
    return getRegionSize(region).getHeight();
}

double GWindow::getRegionHeight(const std::string& region) const {
    return getRegionHeight(stringToRegion(region));
}

GDimension GWindow::getRegionSize(Region region) const {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return GDimension();
    } else {
        QRect geom = layout->geometry();
        return GDimension(geom.width(), geom.height());
    }
}

GDimension GWindow::getRegionSize(const std::string& region) const {
    return getRegionSize(stringToRegion(region));
}

double GWindow::getRegionWidth(Region region) const {
    return getRegionSize(region).getWidth();
}

double GWindow::getRegionWidth(const std::string& region) const {
    return getRegionWidth(stringToRegion(region));
}

double GWindow::getScreenHeight() {
    return getScreenSize().getHeight();
}

GDimension GWindow::getScreenSize() {
    QRect rec = QApplication::desktop()->availableGeometry();
    return GDimension(rec.width(), rec.height());
}

double GWindow::getScreenWidth() {
    return getScreenSize().getWidth();
}

GDimension GWindow::getSize() const {
    QRect geom = _iqmainwindow->geometry();
    return GDimension(geom.width(), geom.height());
}

std::string GWindow::getTitle() const {
    return _iqmainwindow->windowTitle().toStdString();
}

std::string GWindow::getType() const {
    return "GWindow";
}

QWidget* GWindow::getWidget() const {
    return static_cast<QWidget*>(_iqmainwindow);
}

double GWindow::getWidth() const {
    return _iqmainwindow->geometry().width();
}

double GWindow::getX() const {
    return _iqmainwindow->geometry().x();
}

double GWindow::getY() const {
    return _iqmainwindow->geometry().y();
}

void GWindow::hide() {
    setVisible(false);
}

bool GWindow::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool GWindow::inCanvasBounds(double x, double y) const {
    return 0 <= x && x < getCanvasWidth() && 0 <= y && y < getCanvasHeight();
}

bool GWindow::isMaximized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMaximized) != 0
            || (_iqmainwindow->windowState() & Qt::WindowFullScreen) != 0;
}

bool GWindow::isMinimized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMinimized) != 0;
}

bool GWindow::isOpen() const {
    return isVisible();
}

bool GWindow::isRepaintImmediately() const {
    // TODO
    return true;
}

bool GWindow::isResizable() const {
    return _resizable;
}

bool GWindow::isVisible() const {
    return _iqmainwindow->isVisible();
}

void GWindow::loadCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->load(filename);   // runs on Qt GUI thread
}

void GWindow::pack() {
    setSize(getPreferredSize());
}

void GWindow::pause(double ms) {
    GThread::sleep(ms);
}

QLayout* GWindow::layoutForRegion(Region region) const {
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

QLayout* GWindow::layoutForRegion(const std::string& region) const {
    return layoutForRegion(stringToRegion(region));
}

void GWindow::processKeyPressEventInternal(QKeyEvent* /* event */) {
    // empty; override me
}

std::string GWindow::regionToString(Region region) {
    return region == REGION_NORTH ? "North" :
           region == REGION_SOUTH ? "South" :
           region == REGION_WEST  ? "West" :
           region == REGION_EAST  ? "East" : "Center";
}

void GWindow::rememberPosition() {
    // TODO
}

void GWindow::remove(GObject* obj) {
    if (_canvas) {
        _canvas->remove(obj);   // runs on Qt GUI thread
    }
}

void GWindow::remove(GObject& obj) {
    if (_canvas) {
        _canvas->remove(&obj);   // runs on Qt GUI thread
    }
}

void GWindow::remove(GInteractor* interactor) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    Vector<Region> regions = {REGION_CENTER, REGION_NORTH, REGION_SOUTH, REGION_WEST, REGION_EAST};
    for (Region region : regions) {
        QLayout* layout = layoutForRegion(region);
        if (GLayout::contains(layout, widget)) {
            removeFromRegion(interactor, region);
        }
    }
}

void GWindow::remove(GInteractor& interactor) {
    remove(&interactor);
}

void GWindow::removeClickListener() {
    if (_canvas) {
        _canvas->removeClickListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeFromRegion(GInteractor* interactor, Region region) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    // special case: labels in "GText mode" are added to canvas
    if (layout == _iqmainwindow->_centerLayout && interactor->getType() == "GLabel") {
        GLabel* label = (GLabel*) interactor;
        if (label->hasGText()) {
            remove(label->getGText());
            return;
        }
    }

    GThread::runOnQtGuiThread([this, widget, layout]() {
        widget->setVisible(false);
        layout->removeWidget(widget);
        layout->update();
        _iqmainwindow->updateGeometry();
        _iqmainwindow->update();
    });
}

void GWindow::removeFromRegion(GInteractor* interactor, const std::string& region) {
    removeFromRegion(interactor, stringToRegion(region));
}

void GWindow::removeFromRegion(GInteractor& interactor, Region region) {
    removeFromRegion(&interactor, region);
}

void GWindow::removeFromRegion(GInteractor& interactor, const std::string& region) {
    removeFromRegion(&interactor, region);
}

void GWindow::removeKeyListener() {
    if (_canvas) {
        _canvas->removeKeyListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeMenuListener() {
    removeEventListener("actionMenu");
}

void GWindow::removeMouseListener() {
    if (_canvas) {
        _canvas->removeMouseListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeTimerListener() {
    removeEventListener("timer");
}

void GWindow::removeWindowListener() {
    removeEventListeners({"close",
                         "closing",
                         "maximize",
                         "minimize",
                         "open",
                         "resize",
                         "restore"});
}

void GWindow::requestFocus() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setFocus();
    });
}

void GWindow::saveCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->save(filename);   // runs on Qt GUI thread
}

void GWindow::setBackground(int color) {
    GThread::runOnQtGuiThread([this, color]() {
        GForwardDrawingSurface::setBackground(color);
        // TODO: set background of N/S/E/W regions and central region?
    });
}

void GWindow::setBackground(const std::string& color) {
    GThread::runOnQtGuiThread([this, color]() {
        GForwardDrawingSurface::setBackground(color);
        // TODO: set background of N/S/E/W regions and central region?
    });
}

void GWindow::setCanvasHeight(double height) {
    ensureForwardTarget();
    setCanvasSize(getCanvasWidth(), height);
}

void GWindow::setCanvasSize(double width, double height) {
    ensureForwardTarget();
    _canvas->setMinimumSize(width, height);    // runs on Qt GUI thread
    _canvas->setPreferredSize(width, height);
    pack();
}

void GWindow::setCanvasSize(const GDimension& size) {
    setCanvasSize(size.getWidth(), size.getHeight());
}

void GWindow::setCanvasWidth(double width) {
    ensureForwardTarget();
    setCanvasSize(width, getCanvasHeight());
}

void GWindow::setCloseOperation(CloseOperation op) {
    GThread::runOnQtGuiThread([this, op]() {
        _closeOperation = op;
        _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, op == GWindow::CLOSE_EXIT);
    });
}

void GWindow::setExitOnClose(bool exitOnClose) {
    if (exitOnClose) {
        setCloseOperation(GWindow::CLOSE_EXIT);
    } else {
        setCloseOperation(_closeOperation == GWindow::CLOSE_EXIT ? GWindow::CLOSE_HIDE : _closeOperation);
    }
}

void GWindow::setHeight(double height) {
    setSize(getWidth(), height);
}

void GWindow::setLocation(double x, double y) {
    GThread::runOnQtGuiThread([this, x, y]() {
        _iqmainwindow->setGeometry((int) x, (int) y, getWidth(), getHeight());
    });
}

void GWindow::setLocation(const GPoint& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled) {
    GThread::runOnQtGuiThread([this, menu, item, enabled]() {
        std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
        std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
        std::string menuItemKey = menuKey + "/" + itemKey;
        if (!_menuMap.containsKey(menuKey)) {
            error("GWindow::setMenuItemEnabled: menu \"" + menu + "\" does not exist");
        } else if (!_menuActionMap.containsKey(menuItemKey)) {
            error("GWindow::setMenuItemEnabled: menu item \"" + item + "\" does not exist");
        } else {
            QAction* action = _menuActionMap[menuItemKey];
            action->setEnabled(enabled);
        }
    });
}

void GWindow::setClickListener(GEventListener func) {
    _canvas->setClickListener(func);   // runs on Qt GUI thread
}

void GWindow::setClickListener(GEventListenerVoid func) {
    _canvas->setClickListener(func);   // runs on Qt GUI thread
}

void GWindow::setKeyListener(GEventListener func) {
    _canvas->setKeyListener(func);   // runs on Qt GUI thread
}

void GWindow::setKeyListener(GEventListenerVoid func) {
    _canvas->setKeyListener(func);   // runs on Qt GUI thread
}

void GWindow::setMenuListener(GEventListener func) {
    setEventListener("actionMenu", func);   // runs on Qt GUI thread
}

void GWindow::setMenuListener(GEventListenerVoid func) {
    setEventListener("actionMenu", func);   // runs on Qt GUI thread
}

void GWindow::setMouseListener(GEventListener func) {
    _canvas->setMouseListener(func);   // runs on Qt GUI thread
}

void GWindow::setMouseListener(GEventListenerVoid func) {
    _canvas->setMouseListener(func);   // runs on Qt GUI thread
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign) {
    setRegionHorizontalAlignment(region, halign);
}

void GWindow::setRegionAlignment(Region region, VerticalAlignment valign) {
    setRegionVerticalAlignment(region, valign);
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    setRegionHorizontalAlignment(region, halign);
    setRegionVerticalAlignment(region, valign);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    HorizontalAlignment halignment = toHorizontalAlignment(align);
    VerticalAlignment valignment = toVerticalAlignment(align);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    HorizontalAlignment halignment = toHorizontalAlignment(halign);
    VerticalAlignment valignment = toVerticalAlignment(valign);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GWindow::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    GThread::runOnQtGuiThread([this, region, halign, layout]() {
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
            if (halign == ALIGN_LEFT) {
                ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 0);
                ((QHBoxLayout*) layout)->addStretch(/* stretch */ 99);
            } else if (halign == ALIGN_RIGHT) {
                ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
                ((QHBoxLayout*) layout)->addStretch(/* stretch */ 0);
            } else {   // halign == ALIGN_CENTER
                ((QHBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
                ((QHBoxLayout*) layout)->addStretch(/* stretch */ 99);
            }
        }

        layout->update();
        _iqmainwindow->updateGeometry();
        _iqmainwindow->update();
    });
}

void GWindow::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    setRegionHorizontalAlignment(stringToRegion(region), toHorizontalAlignment(halign));
}

void GWindow::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    GThread::runOnQtGuiThread([this, region, valign, layout]() {
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
            if (valign == ALIGN_TOP) {
                ((QVBoxLayout*) layout)->insertStretch(0, /* stretch */ 0);
                ((QVBoxLayout*) layout)->addStretch(/* stretch */ 99);
            } else if (valign == ALIGN_BOTTOM) {
                ((QVBoxLayout*) layout)->insertStretch(0, /* stretch */ 99);
                ((QVBoxLayout*) layout)->addStretch(/* stretch */ 0);
            } else {   // valign == ALIGN_MIDDLE
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
    });
}

void GWindow::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    setRegionVerticalAlignment(stringToRegion(region), toVerticalAlignment(valign));
}

void GWindow::setResizable(bool resizable) {
    GThread::runOnQtGuiThread([this, resizable]() {
        if (resizable) {
            if (!_resizable) {
                _iqmainwindow->resize((int) getWidth(), (int) getHeight());
                _iqmainwindow->setMinimumSize(_iqmainwindow->minimumSizeHint());
                GDimension screenSize = getScreenSize();
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
    });
}

void GWindow::setSize(double width, double height) {
    GThread::runOnQtGuiThread([this, width, height]() {
        if (isResizable()) {
            _iqmainwindow->resize((int) width, (int) height);
        } else {
            _iqmainwindow->setFixedSize((int) width, (int) height);
        }
    });
}

void GWindow::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GWindow::setTimerListener(double ms, GEventListener func) {
    setEventListener("timer", func);
    GThread::runOnQtGuiThread([this, ms]() {
        _iqmainwindow->timerStart(ms);
    });
}

void GWindow::setTimerListener(double ms, GEventListenerVoid func) {
    setEventListener("timer", func);
    GThread::runOnQtGuiThread([this, ms]() {
        _iqmainwindow->timerStart(ms);
    });
}

void GWindow::setTitle(const std::string& title) {
    GThread::runOnQtGuiThread([this, title]() {
        _iqmainwindow->setWindowTitle(QString::fromStdString(title));
    });
}

void GWindow::setVisible(bool visible) {
    GThread::runOnQtGuiThread([this, visible]() {
        _iqmainwindow->setVisible(visible);
    });
}

void GWindow::setWidth(double width) {
    setSize(width, getHeight());
}

void GWindow::setWindowListener(GEventListener func) {
    setEventListeners({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void GWindow::setWindowListener(GEventListenerVoid func) {
    setEventListeners({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void GWindow::setWindowTitle(const std::string& title) {
    setTitle(title);
}

void GWindow::setX(double x) {
    setLocation(x, getY());
}

void GWindow::setY(double y) {
    setLocation(getX(), y);
}

void GWindow::show() {
    setVisible(true);
}

void GWindow::sleep(double ms) {
    GThread::sleep(ms);
}

GWindow::Region GWindow::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north")) {
        return GWindow::REGION_NORTH;
    } else if (stringContains(regionLC, "south")) {
        return GWindow::REGION_SOUTH;
    } else if (stringContains(regionLC, "west")) {
        return GWindow::REGION_WEST;
    } else if (stringContains(regionLC, "east")) {
        return GWindow::REGION_EAST;
    } else {
        return GWindow::REGION_CENTER;
    }
}

void GWindow::toBack() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->lower();
    });
}

void GWindow::toFront() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->raise();
        _iqmainwindow->setFocus();
    });
}


// global functions for compatibility

int convertColorToRGB(const std::string& colorName) {
    return GColor::convertColorToRGB(colorName);
}

std::string convertRGBToColor(int rgb) {
    return GColor::convertRGBToColor(rgb);
}

void exitGraphics() {
    // TODO
}

double getScreenHeight() {
    return GWindow::getScreenHeight();
}

GDimension getScreenSize() {
    return GWindow::getScreenSize();
}

double getScreenWidth() {
    return GWindow::getScreenWidth();
}

void pause(double milliseconds) {
    GThread::sleep(milliseconds);
}

void repaint() {
    QMainWindow* lastWindow = GWindow::getLastWindow();
    if (lastWindow) {
        lastWindow->repaint();
    }
    // TODO: other windows?
}
