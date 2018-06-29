/*
 * File: qgradiobutton.cpp
 * -----------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgradiobutton.h"
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_RadioButton::_Q_Internal_RadioButton(QGRadioButton* radioButton, bool checked, QWidget* parent)
        : QRadioButton(parent),
          _radioButton(radioButton) {
    _radioButton->setChecked(checked);
}

Map<std::string, QButtonGroup*> QGRadioButton::_buttonGroups;

QGRadioButton::QGRadioButton(const std::string& text, const std::string& group, bool checked, QWidget* parent)
        : _radioButton(this, checked, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    QButtonGroup* buttonGroup = getButtonGroup(group);
    buttonGroup->addButton(&_radioButton);
    setText(text);
}

std::string QGRadioButton::getText() const {
    return _radioButton.text().toStdString();
}

std::string QGRadioButton::getType() const {
    return "QGRadioButton";
}

QWidget* QGRadioButton::getWidget() const {
    return (QWidget*) &_radioButton;
}

bool QGRadioButton::isChecked() const {
    return _radioButton.isChecked();
}

void QGRadioButton::setChecked(bool checked) {
    _radioButton.setChecked(checked);
}

void QGRadioButton::setText(const std::string& text) {
    _radioButton.setText(QString::fromStdString(text));
}

QButtonGroup* QGRadioButton::getButtonGroup(const std::string& group) {
    if (!_buttonGroups.containsKey(group)) {
        _buttonGroups.put(group, new QButtonGroup());
    }
    return _buttonGroups[group];
}
