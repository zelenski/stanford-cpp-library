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

_Internal_QLabel::_Internal_QLabel(QGLabel* qglabel, QWidget* parent)
        : QLabel(parent),
          _qglabel(qglabel) {
    // empty
}

QGLabel::QGLabel(const std::string& text, QWidget* parent) {
    _iqlabel = new _Internal_QLabel(this, getInternalParent(parent));
    setText(text);
}

QGLabel::~QGLabel() {
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

std::string QGLabel::getLabel() const {
    return getText();
}

std::string QGLabel::getText() const {
    return _iqlabel->text().toStdString();
}

std::string QGLabel::getType() const {
    return "QGLabel";
}

QWidget* QGLabel::getWidget() const {
    return static_cast<QWidget*>(_iqlabel);
}

void QGLabel::setIcon(const std::string& filename) {
    QPixmap pixmap(QString::fromStdString(filename));
    _iqlabel->setPixmap(pixmap);
    // TODO: loses text; how to have both icon and text in same label?
}

void QGLabel::setLabel(const std::string& text) {
    setText(text);
}

void QGLabel::setText(const std::string& text) {
    _iqlabel->setText(QString::fromStdString(text));
    _iqlabel->updateGeometry();
}
