/*
 * File: qgcheckbox.cpp
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgcheckbox.h"
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_CheckBox::_Q_Internal_CheckBox(QGCheckBox* checkBox, bool checked, QWidget* parent)
        : QCheckBox(parent),
          _checkBox(checkBox) {
    _checkBox->setChecked(checked);
}

QGCheckBox::QGCheckBox(const std::string& text, bool checked, QWidget* parent)
        : _checkBox(this, checked, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    setText(text);
}

std::string QGCheckBox::getText() const {
    return _checkBox.text().toStdString();
}

QWidget* QGCheckBox::getWidget() const {
    return (QWidget*) &_checkBox;
}

bool QGCheckBox::isChecked() const {
    return _checkBox.isChecked();
}

bool QGCheckBox::isEnabled() const {
    return _checkBox.isEnabled();
}

void QGCheckBox::setChecked(bool checked) {
    _checkBox.setChecked(checked);
}

void QGCheckBox::setEnabled(bool enabled) {
    _checkBox.setEnabled(enabled);
}

void QGCheckBox::setText(const std::string& text) {
    _checkBox.setText(QString::fromStdString(text));
}
