/*
 * File: qgbutton.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgbutton.h"
#include "qgwindow.h"

_Internal_QPushButton::_Internal_QPushButton(QGButton* button, QWidget* parent)
        : QPushButton(parent),
          _qgbutton(button) {
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QPushButton::handleClick() {
    _qgbutton->fireEvent("click");
}

QGButton::QGButton(const std::string& text, QWidget* parent) {
    _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    setText(text);
}

QGButton::~QGButton() {
    // TODO: delete _button;
    _iqpushbutton = nullptr;
}

std::string QGButton::getText() const {
    return _iqpushbutton->text().toStdString();
}

std::string QGButton::getType() const {
    return "QGButton";
}

QWidget* QGButton::getWidget() const {
    return static_cast<QWidget*>(_iqpushbutton);
}

void QGButton::setClickHandler(std::function<void()> func) {
    setEventHandler("click", func);
}

void QGButton::setIcon(const std::string& filename) {
    QPixmap pixmap(QString::fromStdString(filename));
    QIcon icon(pixmap);
    _iqpushbutton->setIcon(icon);
    // TODO: scale button to fit icon?
}

void QGButton::setText(const std::string& text) {
    _iqpushbutton->setText(QString::fromStdString(text));
}

void QGButton::setTextPosition(QGBorderLayout::Position /* horizontal */, QGBorderLayout::Position /* vertical */) {
    // TODO
}
