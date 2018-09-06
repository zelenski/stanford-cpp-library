/*
 * File: gwindow.cpp
 * -----------------
 *
 * @version 2018/09/05
 * - refactored to use a border layout GContainer "content pane" for storing all interactors
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

_Internal_QMainWindow* GWindow::_lastWindow = nullptr;
const int GWindow::DEFAULT_WIDTH = 500;
const int GWindow::DEFAULT_HEIGHT = 300;
const std::string GWindow::DEFAULT_ICON_FILENAME = "splicon-large.png";

GWindow::GWindow(double width, double height, bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE) {
    GThread::runOnQtGuiThread([this, width, height, visible]() {
        QtGui::instance()->initializeQt();
        _iqmainwindow = new _Internal_QMainWindow(this);
        _lastWindow = _iqmainwindow;

        _contentPane = new GContainer(GContainer::LAYOUT_BORDER);
//        _contentPane->setMargin(0);
//        _contentPane->setPadding(0);
//        _contentPane->setSpacing(0);
        _iqmainwindow->setCentralWidget(_contentPane->getWidget());

        // go ahead and set up canvas when window is loaded
        // ensureForwardTarget();
        if (width > 0 && height > 0) {
            setSize(width, height);
        }

        setWindowIcon(DEFAULT_ICON_FILENAME);
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
    addToRegion(interactor, REGION_CENTER);
}

void GWindow::add(GInteractor* interactor, double x, double y) {
    interactor->setLocation(x, y);
    addToRegion(interactor, REGION_CENTER);
}

void GWindow::add(GInteractor& interactor) {
    addToRegion(&interactor, REGION_CENTER);
}

void GWindow::add(GInteractor& interactor, double x, double y) {
    interactor.setLocation(x, y);
    addToRegion(&interactor, REGION_CENTER);
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
    if (region == REGION_CENTER) {
        // labels in "GText mode" are added as GText objects to canvas
        if (interactor->getType() == "GLabel") {
            GLabel* label = (GLabel*) interactor;
            if (label->hasGText()) {
                add(label->getGText());
                return;
            }
        }
    }
    _contentPane->addToRegion(interactor, (GContainer::Region) region);
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
    _contentPane->clear();
}

void GWindow::clearCanvas() {
    if (_canvas) {
        GThread::runOnQtGuiThread([this]() {
            GForwardDrawingSurface::clear();
            _canvas->clear();
        });
    }
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
    _contentPane->clearRegion((GContainer::Region) region);
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
            addToRegion(_canvas, REGION_CENTER);
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
    return _contentPane->getPreferredSize();
}

double GWindow::getRegionHeight(Region region) const {
    return _contentPane->getRegionHeight((GContainer::Region) region);
}

double GWindow::getRegionHeight(const std::string& region) const {
    return _contentPane->getRegionHeight(region);
}

GDimension GWindow::getRegionSize(Region region) const {
    return _contentPane->getRegionSize((GContainer::Region) region);
}

GDimension GWindow::getRegionSize(const std::string& region) const {
    return _contentPane->getRegionSize(region);
}

double GWindow::getRegionWidth(Region region) const {
    return _contentPane->getRegionWidth((GContainer::Region) region);
}

double GWindow::getRegionWidth(const std::string& region) const {
    return _contentPane->getRegionWidth(region);
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
    return _canvas && _canvas->isRepaintImmediately();
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
//    QSize minimumSize = _iqmainwindow->centralWidget()->minimumSizeHint();
//    setSize(minimumSize.width(), minimumSize.height());
}

void GWindow::pause(double ms) {
    GThread::sleep(ms);
}

void GWindow::processKeyPressEventInternal(QKeyEvent* /* event */) {
    // empty; override me
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
    _contentPane->remove(interactor);
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
    // special case: labels in "GText mode" are added to canvas
    if (region == REGION_CENTER && interactor->getType() == "GLabel") {
        GLabel* label = (GLabel*) interactor;
        if (label->hasGText()) {
            remove(label->getGText());
            return;
        }
    }

    _contentPane->removeFromRegion(interactor, (GContainer::Region) region);
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
    _contentPane->setBackground(color);
    GThread::runOnQtGuiThread([this, color]() {
        GForwardDrawingSurface::setBackground(color);
    });
}

void GWindow::setBackground(const std::string& color) {
    _contentPane->setBackground(color);
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
    _contentPane->setRegionAlignment((GContainer::Region) region, halign);
}

void GWindow::setRegionAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionAlignment((GContainer::Region) region, valign);
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    _contentPane->setRegionAlignment((GContainer::Region) region, halign, valign);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    _contentPane->setRegionAlignment(region, align);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    _contentPane->setRegionAlignment(region, halign, valign);
}

void GWindow::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    _contentPane->setRegionHorizontalAlignment((GContainer::Region) region, halign);
}

void GWindow::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    _contentPane->setRegionHorizontalAlignment(region, halign);
}

void GWindow::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionVerticalAlignment((GContainer::Region) region, valign);
}

void GWindow::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    _contentPane->setRegionVerticalAlignment(region, valign);
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

void GWindow::setWindowIcon(const std::string& iconFile) {
    if (fileExists(iconFile)) {
        GThread::runOnQtGuiThread([this, iconFile]() {
            QIcon qicon(QString::fromStdString(iconFile));
            _iqmainwindow->setWindowIcon(qicon);
        });
    }
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
    if (stringContains(regionLC, "north") || stringContains(regionLC, "top")) {
        return GWindow::REGION_NORTH;
    } else if (stringContains(regionLC, "south") || stringContains(regionLC, "bottom")) {
        return GWindow::REGION_SOUTH;
    } else if (stringContains(regionLC, "west") || stringContains(regionLC, "left")) {
        return GWindow::REGION_WEST;
    } else if (stringContains(regionLC, "east") || stringContains(regionLC, "right")) {
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
    QtGui::instance()->exitGraphics();
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


_Internal_QMainWindow::_Internal_QMainWindow(GWindow* gwindow, QWidget* parent)
        : QMainWindow(parent),
          _gwindow(gwindow) {
    GThread::ensureThatThisIsTheQtGuiThread("GWindow internal initialization");
    setObjectName(QString::fromStdString("_Internal_QMainWindow"));
}

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

bool _Internal_QMainWindow::timerExists(int id) {
    if (id >= 0) {
        return _timerIDs.contains(id);
    } else {
        return !_timerIDs.isEmpty();
    }
}

int _Internal_QMainWindow::timerStart(double ms) {
    // TODO: start from Qt GUI thread?
    // QTimer* timer = new QTimer(this);
    // connect(timer, SIGNAL(timeout()), this, SLOT(processTimerEvent()));
    // timer->start((int) ms);
    // TODO: when to free timer memory?

    int timerID = startTimer((int) ms);
    _timerIDs.add(timerID);
    return timerID;
}

void _Internal_QMainWindow::timerStop(int id) {
    if (id < 0 && timerExists()) {
        id = _timerIDs.first();
    }

    if (timerExists(id)) {
        killTimer(id);
        _timerIDs.remove(id);
    }
}
