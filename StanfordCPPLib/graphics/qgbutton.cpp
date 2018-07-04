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
    _qgbutton->fireEvent("click");
}

QGButton::QGButton(const std::string& text, QWidget* parent)
        : _button(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    ensureThreadSafety();
    setText(text);
}

std::string QGButton::getText() const {
    return _button.text().toStdString();
}

std::string QGButton::getType() const {
    return "QGButton";
}

QWidget* QGButton::getWidget() const {
    return (QWidget*) &_button;
}

void QGButton::setClickHandler(std::function<void()> func) {
    setEventHandler("click", func);
}

void QGButton::setText(const std::string& text) {
    _button.setText(QString::fromStdString(text));
}

void QGButton::setTextPosition(QGBorderLayout::Position /* horizontal */, QGBorderLayout::Position /* vertical */) {
    // TODO
}
