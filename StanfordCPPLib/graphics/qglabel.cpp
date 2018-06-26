/*
 * File: qglabel.cpp
 * -----------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qglabel.h"
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_Label::_Q_Internal_Label(QGLabel* label, QWidget* parent)
        : QLabel(parent),
          _label(label) {
    // empty
}

QGLabel::QGLabel(const std::string& text, QWidget* parent)
        : _label(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    setText(text);
}

std::string QGLabel::getText() const {
    return _label.text().toStdString();
}

QWidget* QGLabel::getWidget() const {
    return (QWidget*) &_label;
}

void QGLabel::setText(const std::string& text) {
    _label.setText(QString::fromStdString(text));
}
