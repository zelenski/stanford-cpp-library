/*
 * File: ginteractor.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added set/removeActionListener
 * - added set/removeClickListener
 * - added set/removeDoubleClickListener
 * - added set/removeKeyListener
 * - added set/removeMouseListener
 * @version 2019/04/22
 * - added setIcon with QIcon and QPixmap
 * @version 2019/04/10
 * - bug fix for setBackground on GTextArea and GBrowserPane
 * @version 2018/09/20
 * - bug fix for requestFocus threading
 * @version 2018/09/04
 * - added get/setName, getID
 * @version 2018/08/23
 * - renamed to ginteractor.cpp to replace Java version
 * @version 2018/06/29
 * - initial version
 */

#include "ginteractor.h"
#include <iostream>
#include <sstream>
#include "gcolor.h"
#include "gfont.h"
#include "gthread.h"
#include "gwindow.h"
#include "qtgui.h"
#include "require.h"

int GInteractor::_interactorCount = 0;

GInteractor::GInteractor()
        : _actionCommand(""),
          _actionEventType("click"),
          _icon(""),
          _name(""),
          _id(-1),
          _container(nullptr),
          _lock(QReadWriteLock::Recursive) {
    QtGui::instance()->initializeQt();   // make sure Qt system is initialized
    _id = ++_interactorCount;            // set ID to number of interactors + 1
}

GInteractor::~GInteractor() {
    // empty
}

void GInteractor::addActionListener() {
    // empty
}

bool GInteractor::eventsEnabled() const {
    return GObservable::eventsEnabled() && getWidget() != nullptr && isVisible();
}

std::string GInteractor::getAccelerator() const {
    // override in subclasses
    return "";
}

std::string GInteractor::getActionCommand() const {
    return _actionCommand;
}

std::string GInteractor::getActionEventType() const {
    return _actionEventType;
}

std::string GInteractor::getBackground() const {
    int rgb = getBackgroundInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getBackgroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->backgroundRole());
    return GColor::convertQColorToRGB(color);
}

GRectangle GInteractor::getBounds() const {
    return GRectangle(getX(), getY(), getWidth(), getHeight());
}

std::string GInteractor::getColor() const {
    int rgb = getColorInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getColorInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return GColor::convertQColorToRGB(color);
}

GContainer* GInteractor::getContainer() const {
    return _container;
}

std::string GInteractor::getDefaultInteractorName() const {
    return getType() + "_" + std::to_string(getID());
}

std::string GInteractor::getFont() const {
    return GFont::toFontString(getWidget()->font());
}

std::string GInteractor::getForeground() const {
    int rgb = getForegroundInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getForegroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return GColor::convertQColorToRGB(color);
}

double GInteractor::getHeight() const {
    return getWidget()->height();
}

int GInteractor::getID() const {
    return _id;
}

std::string GInteractor::getIcon() const {
    return _icon;
}

GPoint GInteractor::getLocation() const {
    return GPoint(getX(), getY());
}

/* static */ QWidget* GInteractor::getInternalParent(QWidget* parent) {
    return parent ? parent : (QWidget*) GWindow::getLastWindow();
}

double GInteractor::getMinimumHeight() const {
    return getMinimumSize().height;
}

GDimension GInteractor::getMinimumSize() const {
    QSize size = getInternalWidget()->getMinimumSize();
    return GDimension(size.width(), size.height());
}

double GInteractor::getMinimumWidth() const {
    return getMinimumSize().width;
}

char GInteractor::getMnemonic() const {
    // TODO
    return '?';
}

std::string GInteractor::getName() const {
    if (_name.empty()) {
        return getDefaultInteractorName();
    } else {
        return _name;
    }
}

double GInteractor::getPreferredHeight() const {
    return getPreferredSize().height;
}

GDimension GInteractor::getPreferredSize() const {
    QSize size;
    if (getInternalWidget()->hasPreferredSize()) {
        size = getInternalWidget()->getPreferredSize();
    } else {
        size = getWidget()->sizeHint();
    }
    return GDimension(size.width(), size.height());
}

double GInteractor::getPreferredWidth() const {
    return getPreferredSize().width;
}

GDimension GInteractor::getSize() const {
    return GDimension(getWidth(), getHeight());
}

double GInteractor::getWidth() const {
    return getWidget()->width();
}

double GInteractor::getX() const {
    return getWidget()->x();
}

double GInteractor::getY() const {
    return getWidget()->y();
}

bool GInteractor::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool GInteractor::inBounds(int x, int y) const {
    return 0 <= x && x < (int) getWidth() && 0 <= y && y < (int) getHeight();
}

bool GInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

bool GInteractor::isVisible() const {
    return getWidget()->isVisible();
}

void GInteractor::lockForRead() {
    _lock.lockForRead();
}

void GInteractor::lockForReadConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->lockForRead();
}

void GInteractor::lockForWrite() {
    _lock.lockForWrite();
}

void GInteractor::lockForWriteConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->lockForWrite();
}

std::string GInteractor::normalizeAccelerator(const std::string& accelerator) {
    std::string acceleratorStr = stringReplace(accelerator, "Alt-", "Alt+");
    acceleratorStr = stringReplace(acceleratorStr, "Command-", "Command+");
    acceleratorStr = stringReplace(acceleratorStr, "Ctrl-", "Ctrl+");
    acceleratorStr = stringReplace(acceleratorStr, "Meta-", "Meta+");
    acceleratorStr = stringReplace(acceleratorStr, "Shift-", "Shift+");
    return acceleratorStr;
}

void GInteractor::removeActionListener() {
    removeEventListener(getActionEventType());
}

void GInteractor::removeClickListener() {
    removeEventListener("click");
}

void GInteractor::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GInteractor::removeKeyListener() {
    removeEventListeners({"keypress",
                         "keyrelease",
                         "keytype"});
}

void GInteractor::removeMouseListener() {
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

void GInteractor::requestFocus() {
    GThread::runOnQtGuiThread([this]() {
        getWidget()->setFocus();
    });
}

void GInteractor::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void GInteractor::setAccelerator(const std::string& /* accelerator */) {
    // override in subclasses
}

void GInteractor::setActionListener(GEventListener func) {
    setEventListener(getActionEventType(), func);
}

void GInteractor::setActionListener(GEventListenerVoid func) {
    setEventListener(getActionEventType(), func);
}

void GInteractor::setBackground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->backgroundRole(), QColor(rgb));

        // additional palette color settings for GChooser and other widgets
        // TODO: does not totally work for some widgets, e.g. GChooser popup menu
        if (getType() == "GChooser") {
            palette.setColor(QPalette::Base, QColor(rgb));
            palette.setColor(QPalette::Active, QPalette::Button, QColor(rgb));
            palette.setColor(QPalette::Inactive, QPalette::Button, QColor(rgb));
        } else if (getType() == "GTextArea"
                   || getType() == "GBrowserPane") {
            palette.setColor(QPalette::Base, QColor(rgb));
        }

        getWidget()->setAutoFillBackground(true);
        getWidget()->setPalette(palette);
    });
}

void GInteractor::setBackground(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        int argb = GColor::convertColorToARGB(color);
        GThread::runOnQtGuiThread([this, argb]() {
            QColor qcolor = GColor::toQColorARGB(argb);
            QPalette palette(getWidget()->palette());
            palette.setColor(getWidget()->backgroundRole(), qcolor);

            // additional palette color settings for GChooser and other widgets
            // TODO: does not totally work for some widgets, e.g. GChooser popup menu
            if (getType() == "GChooser") {
                palette.setColor(QPalette::Base, qcolor);
                palette.setColor(QPalette::Active, QPalette::Button, qcolor);
                palette.setColor(QPalette::Inactive, QPalette::Button, qcolor);
            } else if (getType() == "GTextArea"
                       || getType() == "GBrowserPane") {
                palette.setColor(QPalette::Base, qcolor);
            }

            getWidget()->setAutoFillBackground(true);
            getWidget()->setPalette(palette);
        });
    } else {
        int rgb = GColor::convertColorToRGB(color);
        setBackground(rgb);
    }
}

void GInteractor::setBounds(double x, double y, double width, double height) {
    GThread::runOnQtGuiThread([this, x, y, width, height]() {
        getWidget()->setGeometry((int) x, (int) y, (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
    });
}

void GInteractor::setBounds(const GRectangle& size) {
    setBounds(size.x, size.y, size.width, size.height);
}

void GInteractor::setClickListener(GEventListener func) {
    setEventListener("click", func);
}

void GInteractor::setClickListener(GEventListenerVoid func) {
    setEventListener("click", func);
}

void GInteractor::setColor(int rgb) {
    setForeground(rgb);
}

void GInteractor::setColor(const std::string& color) {
    setForeground(color);
}

void GInteractor::setContainer(GContainer* container) {
    _container = container;
    if (!container) {
        // widgets that are not in any container should not be shown on screen
        // (they will awkwardly hover at (0, 0) if they are shown)
        QWidget* widget = getWidget();
        GThread::runOnQtGuiThread([widget]() {
            widget->setParent(nullptr);
        });
        setVisible(false);
    }
}

void GInteractor::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GInteractor::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GInteractor::setEnabled(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        getWidget()->setEnabled(value);
    });
}

void GInteractor::setForeground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->foregroundRole(), QColor(rgb));
        // TODO: does not totally work for some widgets, e.g. GChooser popup menu
        getWidget()->setPalette(palette);
    });
}

void GInteractor::setForeground(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        int argb = GColor::convertColorToARGB(color);
        GThread::runOnQtGuiThread([this, argb]() {
            QPalette palette(getWidget()->palette());
            palette.setColor(getWidget()->foregroundRole(), GColor::toQColorARGB(argb));
            // TODO: does not totally work for some widgets, e.g. GChooser popup menu
            getWidget()->setPalette(palette);
        });
    } else {
        int rgb = GColor::convertColorToRGB(color);
        setForeground(rgb);
    }
}

void GInteractor::setFont(const QFont& font) {
    GThread::runOnQtGuiThread([this, font]() {
        getWidget()->setFont(font);
    });
}

void GInteractor::setFont(const std::string& font) {
    setFont(GFont::toQFont(font));
}

void GInteractor::setHeight(double height) {
    require::nonNegative(height, "GInteractor::setHeight", "height");
    GThread::runOnQtGuiThread([this, height]() {
        getWidget()->setFixedHeight((int) height);
    });
}

void GInteractor::setIcon(const QIcon& /*icon*/) {
    // override in subclasses as appropriate
}

void GInteractor::setIcon(const QPixmap& /*icon*/) {
    // override in subclasses as appropriate
}

void GInteractor::setIcon(const std::string& filename, bool /* retainIconSize */) {
    _icon = filename;

    // override in subclasses as appropriate; make sure to call super
}

void GInteractor::setKeyListener(GEventListener func) {
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GInteractor::setKeyListener(GEventListenerVoid func) {
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GInteractor::setLocation(double x, double y) {
    GThread::runOnQtGuiThread([this, x, y]() {
        getWidget()->setGeometry(x, y, getWidth(), getHeight());
    });
}

void GInteractor::setMinimumSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setMinimumSize", "width");
    require::nonNegative(height, "GInteractor::setMinimumSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setMinimumSize(width, height);
    });
}

void GInteractor::setMinimumSize(const GDimension& size) {
    setMinimumSize(size.width, size.height);
}

void GInteractor::setMnemonic(char /* mnemonic */) {
    // empty; use an & before mnemonic character in interactor's text instead
}

void GInteractor::setMouseListener(GEventListener func) {
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

void GInteractor::setMouseListener(GEventListenerVoid func) {
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

void GInteractor::setName(const std::string& name) {
    _name = name;
    // TODO: getWidget()->setObjectName() ?
}

void GInteractor::setPreferredHeight(double height) {
    setPreferredSize(getPreferredWidth(), height);
}

void GInteractor::setPreferredSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setPreferredSize", "width");
    require::nonNegative(height, "GInteractor::setPreferredSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setPreferredSize(width, height);
    });
}

void GInteractor::setPreferredSize(const GDimension& size) {
    GThread::runOnQtGuiThread([this, size]() {
        getInternalWidget()->setPreferredSize(size.width, size.height);
    });
}

void GInteractor::setPreferredWidth(double width) {
    setPreferredSize(width, getPreferredHeight());
}

void GInteractor::setSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setSize", "width");
    require::nonNegative(height, "GInteractor::setSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        // setBounds(GRectangle(getX(), getY(), width, height));
        getWidget()->setGeometry((int) getX(), (int) getY(), (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
        getWidget()->setMinimumSize((int) width, (int) height);
    });
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.width, size.height);
}

void GInteractor::setTooltip(const std::string& tooltipText) {
    GThread::runOnQtGuiThread([this, tooltipText]() {
        getWidget()->setToolTip(QString::fromStdString(tooltipText));
    });
}

void GInteractor::setVisible(bool visible) {
    // don't allow setting visible to true unless widget is in a container
    if (!visible || _container) {
        GThread::runOnQtGuiThread([this, visible]() {
            getWidget()->setVisible(visible);
        });
    }
}

void GInteractor::setWidth(double width) {
    GThread::runOnQtGuiThread([this, width]() {
        getWidget()->setFixedWidth((int) width);
    });
}

void GInteractor::setX(double x) {
    setLocation(x, getY());
}

void GInteractor::setY(double y) {
    setLocation(getX(), y);
}

void GInteractor::unlock() {
    _lock.unlock();
}

void GInteractor::unlockConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->unlock();
}


_Internal_QWidget::_Internal_QWidget()
        : _minimumSize(-1, -1),
          _preferredSize(-1, -1) {
    // empty
}

_Internal_QWidget::~_Internal_QWidget() {
    detach();
}

void _Internal_QWidget::detach() {
    // empty
}

QSize _Internal_QWidget::getMinimumSize() const {
    return QSize((int) _minimumSize.width, (int) _minimumSize.height);
}

bool _Internal_QWidget::hasMinimumSize() const {
    return _minimumSize.width >= 0 && _minimumSize.height >= 0;
}

QSize _Internal_QWidget::getPreferredSize() const {
    return QSize((int) _preferredSize.width, (int) _preferredSize.height);
}

bool _Internal_QWidget::hasPreferredSize() const {
    return _preferredSize.width >= 0 && _preferredSize.height >= 0;
}

void _Internal_QWidget::setMinimumSize(double width, double height) {
    _minimumSize = GDimension(width, height);
}

void _Internal_QWidget::setMinimumSize(const QSize& size) {
    setMinimumSize(size.width(), size.height());
}

void _Internal_QWidget::setPreferredSize(double width, double height) {
    _preferredSize = GDimension(width, height);
}

void _Internal_QWidget::setPreferredSize(const QSize& size) {
    setPreferredSize(size.width(), size.height());
}
