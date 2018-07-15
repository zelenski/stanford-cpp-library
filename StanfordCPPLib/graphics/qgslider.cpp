/*
 * File: qgslider.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgslider.h"
#include "qgwindow.h"

_Internal_QSlider::_Internal_QSlider(QGSlider* qgslider, QWidget* parent)
        : QSlider(Qt::Horizontal, parent),
          _qgslider(qgslider) {
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Internal_QSlider::handleChange(int /* value */) {
    QGEvent changeEvent(
                /* class  */ QGEvent::CHANGE_EVENT,
                /* type   */ QGEvent::STATE_CHANGED,
                /* name   */ "change",
                /* source */ _qgslider);
    changeEvent.setActionCommand(_qgslider->getActionCommand());
    _qgslider->fireEvent(changeEvent);
}

QSize _Internal_QSlider::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSlider::sizeHint();
    }
}


const int QGSlider::DEFAULT_MIN_VALUE = 0;
const int QGSlider::DEFAULT_MAX_VALUE = 100;
const int QGSlider::DEFAULT_INITIAL_VALUE = 50;

QGSlider::QGSlider(int min, int max, int value, QWidget* parent) {
    _iqslider = new _Internal_QSlider(this, getInternalParent(parent));
    _iqslider->setRange(min, max);
    _iqslider->setValue(value);
}

QGSlider::~QGSlider() {
    // TODO: delete _iqslider;
    _iqslider = nullptr;
}

_Internal_QWidget* QGSlider::getInternalWidget() const {
    return _iqslider;
}

int QGSlider::getMajorTickSpacing() const {
    return _iqslider->tickInterval();
}

int QGSlider::getMinorTickSpacing() const {
    return _iqslider->tickInterval();
}

bool QGSlider::getPaintLabels() const {
    // TODO
    return false;
}

bool QGSlider::getPaintTicks() const {
    // TODO
    return true;
}

bool QGSlider::getSnapToTicks() const {
    // TODO
    return true;
}

std::string QGSlider::getType() const {
    return "QGSlider";
}

int QGSlider::getValue() const {
    return _iqslider->value();
}

QWidget* QGSlider::getWidget() const {
    return static_cast<QWidget*>(_iqslider);
}

void QGSlider::removeChangeHandler() {
    removeEventHandler("change");
}

void QGSlider::setChangeHandler(QGEventHandler func) {
    setEventHandler("change", func);
}

void QGSlider::setChangeHandler(QGEventHandlerVoid func) {
    setEventHandler("change", func);
}

void QGSlider::setMajorTickSpacing(int value) {
    _iqslider->setTickInterval(value);
}

void QGSlider::setMinorTickSpacing(int value) {
    _iqslider->setTickInterval(value);
}

void QGSlider::setPaintLabels(bool /* value */) {
    // not supported
}

void QGSlider::setPaintTicks(bool value) {
    _iqslider->setTickPosition(value ? QSlider::TicksBothSides : QSlider::NoTicks);
}

void QGSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void QGSlider::setValue(int value) {
    _iqslider->setValue(value);
}
