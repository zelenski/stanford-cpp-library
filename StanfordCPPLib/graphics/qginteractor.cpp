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

GRectangle QGInteractor::getBounds() const {
    return GRectangle(getX(), getY(), getWidth(), getHeight());
}

std::string QGInteractor::getFont() const {
    // TODO
    return "?";
}

std::string QGInteractor::getIcon() const {
    // TODO
    return "?";
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

bool QGInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

void QGInteractor::requestFocus() {
    getWidget()->setFocus();
}

void QGInteractor::setAccelerator(const std::string& /* accelerator */) {
    // TODO
}

void QGInteractor::setBackground(int /* rgb */) {
    // TODO
}

void QGInteractor::setBackground(const std::string& /* color */) {
    // TODO
}

void QGInteractor::setBounds(double x, double y, double width, double height) {
    getWidget()->setGeometry((int) x, (int) y, (int) width, (int) height);
}

void QGInteractor::setBounds(const GRectangle& size) {
    setBounds(size.getX(), size.getY(), size.getWidth(), size.getHeight());
}

void QGInteractor::setEnabled(bool value) {
    getWidget()->setEnabled(value);
}

void QGInteractor::setForeground(int /* rgb */) {
    // TODO
}

void QGInteractor::setForeground(const std::string& /* color */) {
    // TODO
}

void QGInteractor::setFont(const std::string& /* font */) {
    // TODO
    // getWidget()->setFont(QFont&);
}

void QGInteractor::setIcon(const std::string& /* filename */) {
    // TODO
}

void QGInteractor::setMnemonic(char /* mnemonic */) {
    // TODO
}

void QGInteractor::setSize(double width, double height) {
    setBounds(GRectangle(getX(), getY(), width, height));
}

void QGInteractor::setSize(const GDimension& size) {
    setBounds(GRectangle(getX(), getY(), size.getWidth(), size.getHeight()));
}

void QGInteractor::setTooltip(const std::string& tooltipText) {
    getWidget()->setToolTip(QString::fromStdString(tooltipText));
}

std::string QGInteractor::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}
