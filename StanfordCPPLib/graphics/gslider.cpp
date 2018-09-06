/*
 * File: GSlider.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gslider.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "gslider.h"
#include "gthread.h"
#include "require.h"
#include "gwindow.h"

const int GSlider::DEFAULT_MIN_VALUE = 0;
const int GSlider::DEFAULT_MAX_VALUE = 100;
const int GSlider::DEFAULT_INITIAL_VALUE = 50;

GSlider::GSlider(int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + integerToString(min) + ") cannot be greater than max (" + integerToString(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this, getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::~GSlider() {
    // TODO: delete _iqslider;
    _iqslider = nullptr;
}

_Internal_QWidget* GSlider::getInternalWidget() const {
    return _iqslider;
}

int GSlider::getMajorTickSpacing() const {
    return _iqslider->tickInterval();
}

int GSlider::getMax() const {
    return _iqslider->maximum();
}

int GSlider::getMin() const {
    return _iqslider->minimum();
}

int GSlider::getMinorTickSpacing() const {
    return _iqslider->tickInterval();
}

bool GSlider::getPaintLabels() const {
    // TODO
    return false;
}

bool GSlider::getPaintTicks() const {
    // TODO
    return true;
}

bool GSlider::getSnapToTicks() const {
    // TODO
    return true;
}

std::string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() const {
    return _iqslider->value();
}

QWidget* GSlider::getWidget() const {
    return static_cast<QWidget*>(_iqslider);
}

void GSlider::removeActionListener() {
    removeEventListener("change");
}

void GSlider::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GSlider::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GSlider::setMajorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GSlider::setMax", "max (" + integerToString(max) + ") cannot be less than min (" + integerToString(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqslider->setMaximum(max);
    });
}

void GSlider::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GSlider::setMin", "min (" + integerToString(min) + ") cannot be greater than max (" + integerToString(max) + ")");
    GThread::runOnQtGuiThread([this, min]() {
        _iqslider->setMinimum(min);
    });
}

void GSlider::setMinorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setPaintLabels(bool /* value */) {
    // not supported
}

void GSlider::setPaintTicks(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickPosition(value ? QSlider::TicksBothSides : QSlider::NoTicks);
    });
}

void GSlider::setRange(int min, int max) {
    require::require(min <= max, "GSlider::setRange", "min (" + integerToString(min) + ") cannot be greater than max (" + integerToString(max) + ")");
    GThread::runOnQtGuiThread([this, min, max]() {
        _iqslider->setRange(min, max);
    });
}

void GSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void GSlider::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GSlider::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setValue(value);
    });
}


_Internal_QSlider::_Internal_QSlider(GSlider* gslider, QWidget* parent)
        : QSlider(Qt::Horizontal, parent),
          _gslider(gslider) {
    require::nonNull(gslider, "_Internal_QSlider::constructor");
    setObjectName(QString::fromStdString("_Internal_QSlider_" + integerToString(gslider->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Internal_QSlider::handleChange(int /* value */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gslider);
    changeEvent.setActionCommand(_gslider->getActionCommand());
    _gslider->fireEvent(changeEvent);
}

QSize _Internal_QSlider::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSlider::sizeHint();
    }
}
