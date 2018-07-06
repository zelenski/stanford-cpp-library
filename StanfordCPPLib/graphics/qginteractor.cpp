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

void QGInteractor::clearEventHandlers() {
    _eventMap.clear();
}

void QGInteractor::ensureThreadSafety() {
    QGui::instance()->ensureThatThisIsTheQtGuiThread();
}

void QGInteractor::fireEvent(const std::string& eventName) {
    if (hasEventHandler(eventName)) {
        _eventMap[eventName]();
    }
}

std::string QGInteractor::getAccelerator() const {
    // TODO
    return "?";
}

std::string QGInteractor::getActionCommand() const {
    // TODO
    return "?";
}

GRectangle QGInteractor::getBounds() const {
    return GRectangle(getX(), getY(), getWidth(), getHeight());
}

std::function<void()> QGInteractor::getEventHandler(const std::string& eventName) const {
    return _eventMap[eventName];
}

std::string QGInteractor::getFont() const {
    return QGFont::toFontString(getWidget()->font());
}

std::string QGInteractor::getIcon() const {
    // TODO
    return "?";
}

QWidget* QGInteractor::getInternalParent(QWidget* parent) const {
    return parent ? parent : (QWidget*) QGWindow::getLastWindow();
}

char QGInteractor::getMnemonic() const {
    // TODO
    return '?';
}

double QGInteractor::getHeight() const {
    return getWidget()->height();
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

bool QGInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

bool QGInteractor::isVisible() const {
    return getWidget()->isVisible();
}

void QGInteractor::requestFocus() {
    getWidget()->setFocus();
}

void QGInteractor::setActionCommand(const std::string& /* actionCommand */) {
    // TODO
}

void QGInteractor::setAccelerator(const std::string& /* accelerator */) {
    // TODO
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

void QGInteractor::setEventHandler(const std::string& eventName, std::function<void()> func) {
    _eventMap[eventName] = func;
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

void QGInteractor::setIcon(const std::string& /* filename */) {
    // TODO: implement in subclasses as appropriate
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

std::string QGInteractor::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}
