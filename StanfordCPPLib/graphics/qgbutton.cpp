/*
 * File: qgbutton.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgbutton.h"
#include "qgwindow.h"

_Q_Internal_Button::_Q_Internal_Button(QGButton* button, QWidget* parent)
        : QPushButton(parent),
          _qgbutton(button) {
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Q_Internal_Button::handleClick() {
    if (_qgbutton->_clickHandler) {
        _qgbutton->_clickHandler();
    }
}

QGButton::QGButton(const std::string& text, QWidget* parent)
        : _button(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _clickHandler(nullptr) {
    setText(text);
}

std::string QGButton::getText() const {
    return _button.text().toStdString();
}

QWidget* QGButton::getWidget() const {
    return (QWidget*) &_button;
}

bool QGButton::isEnabled() const {
    return _button.isEnabled();
}

void QGButton::setClickHandler(void (* func)()) {
    _clickHandler = func;
}

void QGButton::setEnabled(bool enabled) {
    _button.setEnabled(enabled);
}

void QGButton::setText(const std::string& text) {
    _button.setText(QString::fromStdString(text));
}
