/*
 * File: gwindow.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2019/05/05
 * - added static method for isDarkMode checking support
 * - added static methods to ask for system default widget bg/fg color
 * @version 2019/04/27
 * - fixed more bugs with getting/setting window size and location
 * @version 2019/04/25
 * - fixed bugs with getting window geometry and requesting focus
 * @version 2019/04/12
 * - moved pause() headless mode implementation (empty) to console.cpp
 * @version 2019/04/09
 * - added toolbar support
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/10/20
 * - added high-density screen features
 * @version 2018/10/11
 * - bug fix for compareToImage function
 * @version 2018/09/23
 * - bug fixes for Windows/MinGW compatibility
 * @version 2018/09/19
 * - bug fix for clear() method
 * @version 2018/09/13
 * - updated cast syntax to remove warnings in new compiler versions
 * - updated window parent semantics
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
#include <QMenu>
#include <QMenuBar>
#include <QScreen>
#include <QSizePolicy>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include <QToolBar>
#include "filelib.h"
#include "gcolor.h"
#include "gdiffgui.h"
#include "gdiffimage.h"
#include "glabel.h"
#include "glayout.h"
#include "gtextfield.h"
#include "gthread.h"
#include "qtgui.h"
#include "require.h"
#include "strlib.h"

_Internal_QMainWindow* GWindow::_lastWindow = nullptr;
/*static*/ const int GWindow::DEFAULT_WIDTH = 500;
/*static*/ const int GWindow::DEFAULT_HEIGHT = 300;
/*static*/ const int GWindow::HIGH_DPI_SCREEN_THRESHOLD = 200;
/*static*/ const int GWindow::STANDARD_SCREEN_DPI = 96;
/*static*/ const std::string GWindow::DEFAULT_ICON_FILENAME = "splicon-large.png";

GWindow::GWindow(bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE),
          _toolbar(nullptr) {
    _init(DEFAULT_WIDTH, DEFAULT_HEIGHT, visible);
}

GWindow::GWindow(double width, double height, bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE),
          _toolbar(nullptr) {
    _init(width, height, visible);
}

GWindow::GWindow(double x, double y, double width, double height, bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE),
          _toolbar(nullptr) {
    _init(width, height, visible);
    setLocation(x, y);
}

void GWindow::_init(double width, double height, bool visible) {
    require::nonNegative2D(width, height, "GWindow::constructor", "width", "height");
    if (static_cast<int>(width) == 0) {
        width = DEFAULT_WIDTH;
    }
    if (static_cast<int>(height) == 0) {
        height = DEFAULT_HEIGHT;
    }

    GThread::runOnQtGuiThread([this]() {
        QtGui::instance()->initializeQt();
        _iqmainwindow = new _Internal_QMainWindow(this);
        _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, false);
        _lastWindow = _iqmainwindow;
        _contentPane = new GContainer(GContainer::LAYOUT_BORDER);
        _iqmainwindow->setCentralWidget(_contentPane->getWidget());
    });

    ensureForwardTarget();
    setCanvasSize(width, height);
    setWindowIcon(DEFAULT_ICON_FILENAME);
    setVisible(visible);
}

GWindow::~GWindow() {
    if (_lastWindow == _iqmainwindow) {
        _lastWindow = nullptr;
    }
    // TODO: delete _iqmainwindow;
    if (_iqmainwindow) {
        _iqmainwindow->_gwindow = nullptr;
        _iqmainwindow = nullptr;
    }
}

void GWindow::add(GInteractor* interactor) {
    require::nonNull(interactor, "GWindow::add");
    addToRegion(interactor, REGION_CENTER);
}

void GWindow::add(GInteractor* interactor, double x, double y) {
    require::nonNull(interactor, "GWindow::add");
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
    require::nonNull(obj, "GWindow::add");
    ensureForwardTarget();
    _canvas->add(obj);
}

void GWindow::add(GObject* obj, double x, double y) {
    require::nonNull(obj, "GWindow::add");
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
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (_menuMap.containsKey(menuKey)) {
        // duplicate; do not create again
        return _menuMap[menuKey];
    }

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
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, menu, item, icon, func, menuKey, menuItemKey, &action]() {
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
        _menuActionMap[menuItemKey] = action;
    });
    return action;
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const QIcon& icon, GEventListenerVoid func) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, menu, item, &icon, func, menuKey, menuItemKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        action->setIcon(icon);

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;
    });
    return action;
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const QPixmap& icon, GEventListenerVoid func) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, menu, item, &icon, func, menuKey, menuItemKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        action->setIcon(icon);

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;
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
    }

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

void GWindow::addToRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GWindow::addToRegion");
    if (region == REGION_CENTER) {
        // labels in "GText mode" are added as GText objects to canvas
        if (interactor->getType() == "GLabel") {
            GLabel* label = static_cast<GLabel*>(interactor);
            if (label->hasGText()) {
                add(label->getGText());
                return;
            }
        }
    }
    _contentPane->addToRegion(interactor, static_cast<GContainer::Region>(region));
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

void GWindow::addToolbar(const std::string& title) {
    if (_toolbar) {
        return;
    }
    GThread::runOnQtGuiThread([this, title]() {
        _toolbar = _iqmainwindow->addToolBar(QString::fromStdString(title));
        _toolbar->setFloatable(false);
        _toolbar->setMovable(false);
        _toolbar->setBaseSize(0, 0);
    });
}

QAction* GWindow::addToolbarItem(const std::string& item,
                                 const std::string& icon) {
    GEventListenerVoid func = [this, item]() {
        this->_iqmainwindow->handleMenuAction(/* menu */ "toolbar", item);
    };
    return addToolbarItem(item, icon, func);
}

QAction* GWindow::addToolbarItem(const std::string& item,
                                 const std::string& icon,
                                 GEventListenerVoid func) {
    if (!_toolbar) {
        addToolbar();
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = "toolbar/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, item, icon, func, menuItemKey, &action]() {
        if (icon.empty()) {
            action = _toolbar->addAction(QString::fromStdString(item));
        } else {
            // toolbar item with icon doesn't show text
            QIcon qicon(QString::fromStdString(icon));
            action = _toolbar->addAction(qicon, QString::fromStdString(""));
            action->setToolTip(QString::fromStdString(item));
        }

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;

    });
    return action;
}

QAction* GWindow::addToolbarItem(const std::string& item,
                                 const QIcon& icon,
                                 GEventListenerVoid func) {
    if (!_toolbar) {
        addToolbar();
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = "toolbar/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, item, &icon, func, menuItemKey, &action]() {
        // toolbar item with icon doesn't show text
        action = _toolbar->addAction(icon, QString::fromStdString(""));
        action->setToolTip(QString::fromStdString(item));

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;

    });
    return action;
}

QAction* GWindow::addToolbarItem(const std::string& item,
                                 const QPixmap& icon,
                                 GEventListenerVoid func) {
    if (!_toolbar) {
        addToolbar();
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = "toolbar/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, item, &icon, func, menuItemKey, &action]() {
        // toolbar item with icon doesn't show text
        action = _toolbar->addAction(icon, QString::fromStdString(""));
        action->setToolTip(QString::fromStdString(item));

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;

    });
    return action;
}

QAction* GWindow::addToolbarSeparator() {
    if (!_toolbar) {
        addToolbar();
    }

    QAction* action = nullptr;
    GThread::runOnQtGuiThread([this, &action]() {
        action = _toolbar->addSeparator();
    });
    return action;
}

void GWindow::clear() {
    // TODO: reimplement to clear out widgets rather than just canvas
    clearCanvas();

//    bool hasCanvas = _canvas && _contentPane->regionContains(_canvas, GContainer::REGION_CENTER);
//    _contentPane->clearRegion(GContainer::REGION_NORTH);
//    _contentPane->clearRegion(GContainer::REGION_SOUTH);
//    _contentPane->clearRegion(GContainer::REGION_WEST);
//    _contentPane->clearRegion(GContainer::REGION_EAST);
//    if (hasCanvas) {
//        clearCanvas();
//    } else {
//        // don't remove canvas, but do remove any other widgets in center
//        _contentPane->clearRegion(GContainer::REGION_CENTER);
//        ensureForwardTarget();
//    }
}

void GWindow::clearCanvas() {
    if (_canvas) {
        _canvas->clear();
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
    _contentPane->clearRegion(static_cast<GContainer::Region>(region));
}

void GWindow::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

void GWindow::clearToolbarItems() {
    if (!_toolbar) {
        return;
    }
    GThread::runOnQtGuiThread([this]() {
        _toolbar->clear();
    });
}

void GWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.width  / 2 - windowSize.width  / 2,
                screenSize.height / 2 - windowSize.height / 2);
}

/*static*/ std::string GWindow::chooseLightDarkModeColor(
        const std::string& lightColor, const std::string& darkColor) {
    return isDarkMode() ? darkColor : lightColor;
}

/*static*/ int GWindow::chooseLightDarkModeColorInt(int lightColor, int darkColor) {
    return isDarkMode() ? darkColor : lightColor;
}

void GWindow::close() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->close();
    });
}

void GWindow::compareToImage(const std::string& filename, bool /*ignoreWindowSize*/) const {
    ensureForwardTargetConstHack();
    GCanvas* fileCanvas = new GCanvas(filename);
    GDiffImage::showDialog("expected output", fileCanvas,
                           "your output", _canvas);

    // TODO
    // delete fileCanvas;
}

void GWindow::ensureForwardTarget() {
    if (!_canvas) {
        // tell canvas to take any unclaimed space in the window
        GThread::runOnQtGuiThread([this]() {
            _canvas = new GCanvas(_iqmainwindow);
            _canvas->setBackground(GColor::WHITE);
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

GCanvas* GWindow::getCanvas() const {
    ensureForwardTargetConstHack();
    return _canvas;
}

double GWindow::getCanvasHeight() const {
    ensureForwardTargetConstHack();
    return _canvas->getHeight();
}

GDimension GWindow::getCanvasSize() const {
    ensureForwardTargetConstHack();
    return GDimension(_canvas->getWidth(), _canvas->getHeight());
}

double GWindow::getCanvasWidth() const {
    ensureForwardTargetConstHack();
    return _canvas->getWidth();
}

GWindow::CloseOperation GWindow::getCloseOperation() const {
    return _closeOperation;
}

/*static*/ std::string GWindow::getDefaultInteractorBackgroundColor() {
    return GColor::convertRGBToColor(getDefaultInteractorBackgroundColorInt());
}

/*static*/ int GWindow::getDefaultInteractorBackgroundColorInt() {
    static bool everCheckedBefore = false;
    static int previousBg = 0;
    if (!everCheckedBefore) {
        GTextField* tempTextField = new GTextField();
        previousBg = tempTextField->getBackgroundInt();
        everCheckedBefore = true;
    }
    return previousBg;
}

/*static*/ std::string GWindow::getDefaultInteractorTextColor() {
    return GColor::convertRGBToColor(getDefaultInteractorTextColorInt());
}

/*static*/ int GWindow::getDefaultInteractorTextColorInt() {
    static bool everCheckedBefore = false;
    static int previousFg = 0;
    if (!everCheckedBefore) {
        GTextField* tempTextField = new GTextField();
        previousFg = tempTextField->getForegroundInt();
        everCheckedBefore = true;
    }
    return previousFg;
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

/* static */ QMainWindow* GWindow::getLastWindow() {
    return _lastWindow;
}

double GWindow::getHeight() const {
    return _iqmainwindow->height();
}

GPoint GWindow::getLocation() const {
    return GPoint(_iqmainwindow->x(), _iqmainwindow->y());
}

GDimension GWindow::getPreferredSize() const {
    return _contentPane->getPreferredSize();
}

double GWindow::getRegionHeight(Region region) const {
    return _contentPane->getRegionHeight(static_cast<GContainer::Region>(region));
}

double GWindow::getRegionHeight(const std::string& region) const {
    return _contentPane->getRegionHeight(region);
}

GDimension GWindow::getRegionSize(Region region) const {
    return _contentPane->getRegionSize(static_cast<GContainer::Region>(region));
}

GDimension GWindow::getRegionSize(const std::string& region) const {
    return _contentPane->getRegionSize(region);
}

double GWindow::getRegionWidth(Region region) const {
    return _contentPane->getRegionWidth(static_cast<GContainer::Region>(region));
}

double GWindow::getRegionWidth(const std::string& region) const {
    return _contentPane->getRegionWidth(region);
}

/*static*/ int GWindow::getScreenDpi() {
    return QGuiApplication::primaryScreen()->logicalDotsPerInchX();
}

/*static*/ double GWindow::getScreenDpiScaleRatio() {
    double ratio = (double) getScreenDpi() / STANDARD_SCREEN_DPI;
    return (ratio >= 1.0) ? ratio : 1.0;
}

/*static*/ double GWindow::getScreenHeight() {
    return getScreenSize().height;
}

/*static*/ GDimension GWindow::getScreenSize() {
    QRect rec;
    GThread::runOnQtGuiThread([&rec]() {
        rec = QGuiApplication::primaryScreen()->availableGeometry();
    });
    return GDimension(rec.width(), rec.height());
}

/*static*/ double GWindow::getScreenWidth() {
    return getScreenSize().width;
}

GDimension GWindow::getSize() const {
    return GDimension(_iqmainwindow->width(), _iqmainwindow->height());
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
    return _iqmainwindow->width();
}

double GWindow::getX() const {
    return _iqmainwindow->x();
}

double GWindow::getY() const {
    return _iqmainwindow->y();
}

bool GWindow::hasToolbar() const {
    return _toolbar != nullptr;
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

/*static*/ bool GWindow::isDarkMode() {
    if (!getLastWindow()) {
        // cannot check yet
        return false;
    }
    int bg = getDefaultInteractorBackgroundColorInt();
    int fg = getDefaultInteractorTextColorInt();

    // our heuristic: if the text is brighter than the background,
    // we'll assume they are in dark mode
    double bgLum = GColor::getLuminance(bg);
    double fgLum = GColor::getLuminance(fg);
    return fgLum > bgLum;
}

/*static*/ bool GWindow::isHighDensityScreen() {
    return getScreenDpi() >= HIGH_DPI_SCREEN_THRESHOLD;
}

/*static*/ bool GWindow::isHighDpiScalingEnabled() {
#ifdef SPL_SCALE_HIGH_DPI_SCREEN
    return true;
#else
    return false;
#endif // SPL_SCALE_HIGH_DPI_SCREEN
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

void GWindow::maximize() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowMaximized);
    });
}

void GWindow::minimize() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowMinimized);
    });
}

void GWindow::pack() {
    setSize(getPreferredSize());
}

void GWindow::pause(double ms) {
    require::nonNegative(ms, "GWindow::pause", "milliseconds");
    GThread::getCurrentThread()->sleep(ms);
}

void GWindow::processKeyPressEventInternal(QKeyEvent* /* event */) {
    // empty; override me
}

void GWindow::rememberPosition() {
    // TODO
}

void GWindow::remove(GObject* obj) {
    require::nonNull(obj, "GWindow::remove");
    if (_canvas) {
        _canvas->remove(obj);
    }
}

void GWindow::remove(GObject& obj) {
    if (_canvas) {
        _canvas->remove(&obj);   // runs on Qt GUI thread
    }
}

void GWindow::remove(GInteractor* interactor) {
    require::nonNull(interactor, "GWindow::remove");
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
    require::nonNull(interactor, "GWindow::removeFromRegion");

    // special case: labels in "GText mode" are added to canvas
    if (region == REGION_CENTER && interactor->getType() == "GLabel") {
        GLabel* label = static_cast<GLabel*>(interactor);
        if (label->hasGText()) {
            remove(label->getGText());
            return;
        }
    }

    _contentPane->removeFromRegion(interactor, static_cast<GContainer::Region>(region));
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

void GWindow::removeToolbar() {
    if (!_toolbar) {
        return;
    }
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->removeToolBar(_toolbar);
        _toolbar = nullptr;
    });
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
        _iqmainwindow->activateWindow();
    });
}

void GWindow::restore() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowActive);
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
    require::nonNegative2D(width, height, "GWindow::setCanvasSize", "width", "height");
    ensureForwardTarget();
    _canvas->setMinimumSize(width, height);    // runs on Qt GUI thread
    _canvas->setPreferredSize(width, height);
    pack();
}

void GWindow::setCanvasSize(const GDimension& size) {
    setCanvasSize(size.width, size.height);
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
        _iqmainwindow->move(static_cast<int>(x), static_cast<int>(y));
    });
}

void GWindow::setLocation(const GPoint& p) {
    setLocation(p.x, p.y);
}

void GWindow::setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::setMenuItemEnabled: menu \"" + menu + "\" does not exist");
    } else if (!_menuActionMap.containsKey(menuItemKey)) {
        error("GWindow::setMenuItemEnabled: menu item \"" + item + "\" does not exist");
    }

    QAction* action = _menuActionMap[menuItemKey];
    GThread::runOnQtGuiThread([action, enabled]() {
        action->setEnabled(enabled);
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
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), halign);
}

void GWindow::setRegionAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), valign);
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), halign, valign);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    _contentPane->setRegionAlignment(region, align);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    _contentPane->setRegionAlignment(region, halign, valign);
}

void GWindow::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    _contentPane->setRegionHorizontalAlignment(static_cast<GContainer::Region>(region), halign);
}

void GWindow::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    _contentPane->setRegionHorizontalAlignment(region, halign);
}

void GWindow::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionVerticalAlignment(static_cast<GContainer::Region>(region), valign);
}

void GWindow::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    _contentPane->setRegionVerticalAlignment(region, valign);
}

void GWindow::setResizable(bool resizable) {
    GThread::runOnQtGuiThread([this, resizable]() {
        if (resizable) {
            if (!_resizable) {
                _iqmainwindow->resize(static_cast<int>(getWidth()),
                                      static_cast<int>(getHeight()));
                _iqmainwindow->setMinimumSize(_iqmainwindow->minimumSizeHint());
                GDimension screenSize = getScreenSize();
                _iqmainwindow->setMaximumSize(static_cast<int>(screenSize.width),
                                              static_cast<int>(screenSize.height));
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
    require::nonNegative2D(width, height, "GWindow::setSize", "width", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        if (isResizable()) {
            _iqmainwindow->resize(static_cast<int>(width),
                                  static_cast<int>(height));
        } else {
            _iqmainwindow->setFixedSize(static_cast<int>(width),
                                        static_cast<int>(height));
        }
    });
}

void GWindow::setSize(const GDimension& size) {
    setSize(size.width, size.height);
}

void GWindow::setTimerListener(double ms, GEventListener func) {
    require::nonNegative(ms, "GWindow::setTimerListener", "delay (ms)");
    setEventListener("timer", func);
    GThread::runOnQtGuiThread([this, ms]() {
        _iqmainwindow->timerStart(ms);
    });
}

void GWindow::setTimerListener(double ms, GEventListenerVoid func) {
    require::nonNegative(ms, "GWindow::setTimerListener", "delay (ms)");
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
    require::nonNegative(ms, "GWindow::sleep", "delay (ms)");
    GThread::getCurrentThread()->sleep(ms);
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

#ifndef SPL_HEADLESS_MODE
void pause(double milliseconds) {
    GThread::getCurrentThread()->sleep(milliseconds);
}
#endif // SPL_HEADLESS_MODE

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
    require::nonNull(gwindow, "_Internal_QMainWindow::constructor");
    GThread::ensureThatThisIsTheQtGuiThread("GWindow internal initialization");
    setObjectName(QString::fromStdString("_Internal_QMainWindow"));
}

void _Internal_QMainWindow::changeEvent(QEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::changeEvent", "event");
    QMainWindow::changeEvent(event);   // call super
    if (!_gwindow || event->type() != QEvent::WindowStateChange) {
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
    require::nonNull(event, "_Internal_QMainWindow::closeEvent", "event");
    if (!_gwindow) {
        QMainWindow::closeEvent(event);   // call super
        return;
    }

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
        QtGui::instance()->exitGraphics(EXITING_DUE_TO_WINDOW_CLOSE);
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
    require::nonNull(event, "_Internal_QMainWindow::keyPressEvent", "event");
    QMainWindow::keyPressEvent(event);   // call super
    if (!_gwindow) {
        return;
    }
    _gwindow->processKeyPressEventInternal(event);
}

void _Internal_QMainWindow::resizeEvent(QResizeEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::resizeEvent", "event");
    QMainWindow::resizeEvent(event);   // call super
    if (!_gwindow) {
        return;
    }
    _gwindow->fireGEvent(event, WINDOW_RESIZED, "resize");
}

void _Internal_QMainWindow::timerEvent(QTimerEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::timerEvent", "event");
    QMainWindow::timerEvent(event);   // call super
    if (!_gwindow) {
        return;
    }
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
    require::nonNegative(ms, "_Internal_QMainWindow::timerStart", "delay (ms)");
    int timerID = startTimer(static_cast<int>(ms));
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


