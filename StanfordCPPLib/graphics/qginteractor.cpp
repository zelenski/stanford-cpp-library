/*
 * File: qginteractor.cpp
 * ----------------------
 *
 * @version 2018/06/29
 * - initial version
 */

#include "qginteractor.h"
#include <iostream>
#include <sstream>
#include "qgcolor.h"
#include "qgfont.h"
#include "qgui.h"
#include "qgwindow.h"

QGInteractor::QGInteractor() {
    ensureThreadSafety();
}

QGInteractor::~QGInteractor() {
    // empty
}

void QGInteractor::clearEventHandlers() {
    _eventMap.clear();
}

void QGInteractor::ensureThreadSafety(const std::string& memberName) {
    QGui::instance()->ensureThatThisIsTheQtGuiThread(memberName);
}

void QGInteractor::fireEvent(QGEvent& event) {
    if (hasEventHandler(event.getName())) {
        event._source = this;
        _eventMap[event.getName()].fireEvent(event);
    }
}

std::string QGInteractor::getAccelerator() const {
    // override in subclasses
    return "";
}

std::string QGInteractor::getActionCommand() const {
    return _actionCommand;
}

std::string QGInteractor::getBackground() const {
    int rgb = getBackgroundInt();
    return QGColor::convertRGBToColor(rgb);
}

int QGInteractor::getBackgroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->backgroundRole());
    return QGColor::convertRGBToRGB(color.red(), color.green(), color.blue());
}

GRectangle QGInteractor::getBounds() const {
    return GRectangle(getX(), getY(), getWidth(), getHeight());
}

std::string QGInteractor::getFont() const {
    return QGFont::toFontString(getWidget()->font());
}

double QGInteractor::getHeight() const {
    return getWidget()->height();
}

std::string QGInteractor::getIcon() const {
    return _icon;
}

GPoint QGInteractor::getLocation() const {
    return GPoint(getX(), getY());
}

QWidget* QGInteractor::getInternalParent(QWidget* parent) const {
    return parent ? parent : (QWidget*) QGWindow::getLastWindow();
}

char QGInteractor::getMnemonic() const {
    // TODO
    return '?';
}

GDimension QGInteractor::getSize() const {
    return GDimension(getWidth(), getHeight());
}

double QGInteractor::getWidth() const {
    return getWidget()->width();
}

double QGInteractor::getX() const {
    return getWidget()->x();
}

double QGInteractor::getY() const {
    return getWidget()->y();
}

bool QGInteractor::hasEventHandler(const std::string& eventName) const {
    return _eventMap.containsKey(eventName);
}

bool QGInteractor::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool QGInteractor::inBounds(int x, int y) const {
    return 0 <= x && x < (int) getWidth() && 0 <= y && y < (int) getHeight();
}

bool QGInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

bool QGInteractor::isVisible() const {
    return getWidget()->isVisible();
}

std::string QGInteractor::normalizeAccelerator(const std::string& accelerator) const {
    std::string acceleratorStr = stringReplace(accelerator, "Alt-", "Alt+");
    acceleratorStr = stringReplace(acceleratorStr, "Command-", "Command+");
    acceleratorStr = stringReplace(acceleratorStr, "Ctrl-", "Ctrl+");
    acceleratorStr = stringReplace(acceleratorStr, "Meta-", "Meta+");
    acceleratorStr = stringReplace(acceleratorStr, "Shift-", "Shift+");
    return acceleratorStr;
}

void QGInteractor::removeEventHandler(const std::string& eventName) {
    _eventMap.remove(eventName);
}

void QGInteractor::removeEventHandlers(std::initializer_list<std::string> eventNames) {
    for (std::string eventName : eventNames) {
        removeEventHandler(eventName);
    }
}

void QGInteractor::requestFocus() {
    getWidget()->setFocus();
}

void QGInteractor::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void QGInteractor::setAccelerator(const std::string& /* accelerator */) {
    // override in subclasses
}

void QGInteractor::setBackground(int rgb) {
    QPalette palette(getWidget()->palette());
    palette.setColor(getWidget()->backgroundRole(), QColor(rgb));

    // additional palette color settings for QGChooser and other widgets
    // TODO: does not totally work for some widgets, e.g. QGChooser popup menu
    palette.setColor(QPalette::Base, QColor(rgb));
    palette.setColor(QPalette::Active, QPalette::Button, QColor(rgb));
    palette.setColor(QPalette::Inactive, QPalette::Button, QColor(rgb));

    getWidget()->setAutoFillBackground(true);
    getWidget()->setPalette(palette);
}

void QGInteractor::setBackground(const std::string& color) {
    int rgb = QGColor::convertColorToRGB(color);
    setBackground(rgb);
}

void QGInteractor::setBounds(double x, double y, double width, double height) {
    getWidget()->setGeometry((int) x, (int) y, (int) width, (int) height);
    getWidget()->setFixedSize((int) width, (int) height);
}

void QGInteractor::setBounds(const GRectangle& size) {
    setBounds(size.getX(), size.getY(), size.getWidth(), size.getHeight());
}

void QGInteractor::setColor(int rgb) {
    setForeground(rgb);
}

void QGInteractor::setColor(const std::string& color) {
    setForeground(color);
}

void QGInteractor::setEnabled(bool value) {
    getWidget()->setEnabled(value);
}

void QGInteractor::setEventHandler(const std::string& eventName, QGEventHandler func) {
    QGEvent::EventHandlerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_EVENT;
    wrapper.handler = func;
    _eventMap[eventName] = wrapper;
}

void QGInteractor::setEventHandler(const std::string& eventName, QGEventHandlerVoid func) {
    QGEvent::EventHandlerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_VOID;
    wrapper.handlerVoid = func;
    _eventMap[eventName] = wrapper;
}

void QGInteractor::setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandler func) {
    for (std::string eventName : eventNames) {
        setEventHandler(eventName, func);
    }
}

void QGInteractor::setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandlerVoid func) {
    for (std::string eventName : eventNames) {
        setEventHandler(eventName, func);
    }
}


void QGInteractor::setForeground(int rgb) {
    QPalette palette(getWidget()->palette());
    palette.setColor(getWidget()->foregroundRole(), QColor(rgb));
    // TODO: does not totally work for some widgets, e.g. QGChooser popup menu
    getWidget()->setPalette(palette);
}

void QGInteractor::setForeground(const std::string& color) {
    int rgb = QGColor::convertColorToRGB(color);
    setForeground(rgb);
}

void QGInteractor::setFont(const std::string& font) {
    QFont qfont = QGFont::toQFont(font);
    getWidget()->setFont(qfont);
}

void QGInteractor::setHeight(double height) {
    setSize(getWidth(), height);
}

void QGInteractor::setIcon(const std::string& filename, bool /* retainIconSize */) {
    _icon = filename;

    // override in subclasses as appropriate; make sure to call super
}

void QGInteractor::setLocation(double x, double y) {
    getWidget()->setGeometry(x, y, getWidth(), getHeight());
}

void QGInteractor::setMnemonic(char /* mnemonic */) {
    // TODO
}

void QGInteractor::setSize(double width, double height) {
    // setBounds(GRectangle(getX(), getY(), width, height));
    getWidget()->setFixedSize((int) width, (int) height);
}

void QGInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void QGInteractor::setTooltip(const std::string& tooltipText) {
    getWidget()->setToolTip(QString::fromStdString(tooltipText));
}

void QGInteractor::setVisible(bool visible) {
    getWidget()->setVisible(visible);
}

void QGInteractor::setWidth(double width) {
    setSize(width, getHeight());
}

void QGInteractor::setX(double x) {
    setLocation(x, getY());
}

void QGInteractor::setY(double y) {
    setLocation(getX(), y);
}

std::string QGInteractor::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}
