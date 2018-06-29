/*
 * File: qgslider.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgslider.h"
#include "qgwindow.h"

_Q_Internal_Slider::_Q_Internal_Slider(QGSlider* slider, QWidget* parent)
        : QSlider(Qt::Horizontal, parent),
          _qgslider(slider) {
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Q_Internal_Slider::handleChange(int /* value */) {
    if (_qgslider->_changeHandler) {
        _qgslider->_changeHandler();
    }
}

const int QGSlider::DEFAULT_MIN_VALUE = 0;
const int QGSlider::DEFAULT_MAX_VALUE = 100;
const int QGSlider::DEFAULT_INITIAL_VALUE = 50;

QGSlider::QGSlider(int min, int max, int value, QWidget* parent)
        : _slider(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _changeHandler(nullptr) {
    _slider.setRange(min, max);
    _slider.setValue(value);
}

int QGSlider::getMajorTickSpacing() const {
    return _slider.tickInterval();
}

int QGSlider::getMinorTickSpacing() const {
    return _slider.tickInterval();
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

int QGSlider::getValue() const {
    return _slider.value();
}

QWidget* QGSlider::getWidget() const {
    return (QWidget*) &_slider;
}

bool QGSlider::isEnabled() const {
    return _slider.isEnabled();
}

void QGSlider::setChangeHandler(void (* func)()) {
    _changeHandler = func;
}

void QGSlider::setEnabled(bool enabled) {
    _slider.setEnabled(enabled);
}

void QGSlider::setMajorTickSpacing(int value) {
    _slider.setTickInterval(value);
}

void QGSlider::setMinorTickSpacing(int value) {
    _slider.setTickInterval(value);
}

void QGSlider::setPaintLabels(bool /* value */) {
    // not supported
}

void QGSlider::setPaintTicks(bool value) {
    _slider.setTickPosition(value ? QSlider::TicksBothSides : QSlider::NoTicks);
}

void QGSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void QGSlider::setValue(int value) {
    _slider.setValue(value);
}
