/*
 * File: qgtextfield.cpp
 * ---------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgtextfield.h"
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_TextField::_Q_Internal_TextField(QGTextField* textField, QWidget* parent)
        : QLineEdit(parent),
          _qgtextfield(textField) {
    // empty
}

QGTextField::QGTextField(const std::string& text, QWidget* parent)
        : _qtextfield(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    setText(text);
}

std::string QGTextField::getPlaceholder() const {
    return _qtextfield.placeholderText().toStdString();
}

std::string QGTextField::getText() const {
    return _qtextfield.text().toStdString();
}

std::string QGTextField::getType() const {
    return "QGTextField";
}

double QGTextField::getValueAsDouble() const {
    std::string text = trim(getText());
    return stringToDouble(text);
}

int QGTextField::getValueAsInt() const {
    return getValueAsInteger();
}

int QGTextField::getValueAsInteger() const {
    std::string text = trim(getText());
    return stringToInteger(text);
}


QWidget* QGTextField::getWidget() const {
    return (QWidget*) &_qtextfield;
}

bool QGTextField::isEditable() const {
    return !_qtextfield.isReadOnly();
}

void QGTextField::setEditable(bool value) {
    _qtextfield.setReadOnly(!value);
}

void QGTextField::setPlaceholder(const std::string& text) {
    _qtextfield.setPlaceholderText(QString::fromStdString(text));
}

void QGTextField::setText(const std::string& text) {
    _qtextfield.setText(QString::fromStdString(text));
}
