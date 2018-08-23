/*
 * File: qgcheckbox.cpp
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgcheckbox.h"
#include "qgthread.h"
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
                /* name   */ "change",
                /* source */ _qgcheckBox);
    changeEvent.setActionCommand(_qgcheckBox->getActionCommand());
    _qgcheckBox->fireEvent(changeEvent);
}

QSize _Internal_QCheckBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QCheckBox::sizeHint();
    }
}

QGCheckBox::QGCheckBox(const std::string& text, bool checked, QWidget* parent) {
    QGThread::runOnQtGuiThread([this, checked, parent]() {
        _iqcheckBox = new _Internal_QCheckBox(this, checked, getInternalParent(parent));
    });
    setText(text);
}

QGCheckBox::~QGCheckBox() {
    // TODO: delete _iqcheckBox;
    _iqcheckBox = nullptr;
}

std::string QGCheckBox::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* QGCheckBox::getInternalWidget() const {
    return _iqcheckBox;
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

void QGCheckBox::removeActionListener() {
    removeEventListener("change");
}

void QGCheckBox::setActionListener(QGEventListener func) {
    setEventListener("change", func);
}

void QGCheckBox::setActionListener(QGEventListenerVoid func) {
    setEventListener("change", func);
}

void QGCheckBox::setChecked(bool checked) {
    QGThread::runOnQtGuiThread([this, checked]() {
        _iqcheckBox->setChecked(checked);
    });
}

void QGCheckBox::setSelected(bool selected) {
    setChecked(selected);
}

void QGCheckBox::setText(const std::string& text) {
    QGThread::runOnQtGuiThread([this, text]() {
        _iqcheckBox->setText(QString::fromStdString(text));
    });
}

#endif // SPL_QT_GUI
