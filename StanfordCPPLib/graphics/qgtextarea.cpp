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
    connect(this, SIGNAL(textChanged()), this, SLOT(handleTextChange()));
}

void _Q_Internal_TextArea::handleTextChange() {
    if (_qgtextarea->_textChangeHandler) {
        _qgtextarea->_textChangeHandler();
    }
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

std::string QGTextArea::getType() const {
    return "QGTextArea";
}

QWidget* QGTextArea::getWidget() const {
    return (QWidget*) &_qtextarea;
}

bool QGTextArea::isEditable() const {
    return !_qtextarea.isReadOnly();
}

void QGTextArea::setEditable(bool value) {
    _qtextarea.setReadOnly(!value);
}

void QGTextArea::setPlaceholder(const std::string& text) {
    _qtextarea.setPlaceholderText(QString::fromStdString(text));
}

void QGTextArea::setText(const std::string& text) {
    _qtextarea.setText(QString::fromStdString(text));
}

void QGTextArea::setTextChangeHandler(void (* func)()) {
    _textChangeHandler = func;
}
