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

_Internal_QCheckBox::_Internal_QCheckBox(QGCheckBox* qgcheckBox, bool checked, QWidget* parent)
        : QCheckBox(parent),
          _qgcheckBox(qgcheckBox) {
    setChecked(checked);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(handleStateChange(int)));
}

void _Internal_QCheckBox::handleStateChange(int /* state */) {
    QGEvent changeEvent(
                /* class  */ QGEvent::CHANGE_EVENT,
                /* type   */ QGEvent::STATE_CHANGED,
                /* name   */ "change");
    _qgcheckBox->fireEvent(changeEvent);
}

QGCheckBox::QGCheckBox(const std::string& text, bool checked, QWidget* parent) {
    _iqcheckBox = new _Internal_QCheckBox(this, checked, getInternalParent(parent));
    setText(text);
}

QGCheckBox::~QGCheckBox() {
    // TODO: delete _iqcheckBox;
    _iqcheckBox = nullptr;
}

std::string QGCheckBox::getText() const {
    return _iqcheckBox->text().toStdString();
}

std::string QGCheckBox::getType() const {
    return "QGCheckBox";
}

QWidget* QGCheckBox::getWidget() const {
    return static_cast<QWidget*>(_iqcheckBox);
}

bool QGCheckBox::isChecked() const {
    return _iqcheckBox->isChecked();
}

bool QGCheckBox::isSelected() const {
    return _iqcheckBox->isChecked();
}

void QGCheckBox::setChecked(bool checked) {
    _iqcheckBox->setChecked(checked);
}

void QGCheckBox::setSelected(bool selected) {
    _iqcheckBox->setChecked(selected);
}

void QGCheckBox::setChangeHandler(QGEventHandler func) {
    setEventHandler("change", func);
}

void QGCheckBox::setChangeHandler(QGEventHandlerVoid func) {
    setEventHandler("change", func);
}

void QGCheckBox::setText(const std::string& text) {
    _iqcheckBox->setText(QString::fromStdString(text));
}
