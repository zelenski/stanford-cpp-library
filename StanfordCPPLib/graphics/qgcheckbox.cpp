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
    connect(this, SIGNAL(clicked(bool)), this, SLOT(handleChange(bool)));
}

void _Q_Internal_CheckBox::handleChange(bool /* checked */) {
    _checkBox->fireEvent("change");
}

QGCheckBox::QGCheckBox(const std::string& text, bool checked, QWidget* parent)
        : _checkBox(this, checked, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    ensureThreadSafety();
    setText(text);
}

std::string QGCheckBox::getText() const {
    return _checkBox.text().toStdString();
}

std::string QGCheckBox::getType() const {
    return "QGCheckBox";
}

QWidget* QGCheckBox::getWidget() const {
    return (QWidget*) &_checkBox;
}

bool QGCheckBox::isChecked() const {
    return _checkBox.isChecked();
}

bool QGCheckBox::isSelected() const {
    return _checkBox.isChecked();
}

void QGCheckBox::setChecked(bool checked) {
    _checkBox.setChecked(checked);
}

void QGCheckBox::setSelected(bool selected) {
    _checkBox.setChecked(selected);
}

void QGCheckBox::setChangeHandler(std::function<void()> func) {
    setEventHandler("change", func);
}

void QGCheckBox::setText(const std::string& text) {
    _checkBox.setText(QString::fromStdString(text));
}
