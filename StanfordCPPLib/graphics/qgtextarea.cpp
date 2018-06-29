/*
 * File: qgtextarea.cpp
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgtextarea.h"
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_TextArea::_Q_Internal_TextArea(QGTextArea* textArea, QWidget* parent)
        : QTextEdit(parent),
          _qgtextarea(textArea) {
    // empty
}

QGTextArea::QGTextArea(const std::string& text, QWidget* parent)
        : _qtextarea(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    setText(text);
}

std::string QGTextArea::getPlaceholder() const {
    return _qtextarea.placeholderText().toStdString();
}

std::string QGTextArea::getText() const {
    return _qtextarea.toPlainText().toStdString();
}

QWidget* QGTextArea::getWidget() const {
    return (QWidget*) &_qtextarea;
}

bool QGTextArea::isEditable() const {
    return !_qtextarea.isReadOnly();
}

bool QGTextArea::isEnabled() const {
    return !_qtextarea.isEnabled();
}

void QGTextArea::setEditable(bool value) {
    _qtextarea.setReadOnly(!value);
}

void QGTextArea::setEnabled(bool value) {
    _qtextarea.setEnabled(value);
}

void QGTextArea::setPlaceholder(const std::string& text) {
    _qtextarea.setPlaceholderText(QString::fromStdString(text));
}

void QGTextArea::setText(const std::string& text) {
    _qtextarea.setText(QString::fromStdString(text));
}
