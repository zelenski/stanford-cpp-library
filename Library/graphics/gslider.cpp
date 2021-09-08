/*
 * File: GSlider.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key events
 * @version 2019/02/02
 * - destructor now stops event processing
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
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          Qt::Horizontal,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::GSlider(Orientation orientation, int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, orientation, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          orientation == HORIZONTAL ? Qt::Horizontal : Qt::Vertical,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::~GSlider() {
    // TODO: delete _iqslider;
    _iqslider->detach();
    _iqslider = nullptr;
}

std::string GSlider::getActionEventType() const {
    return "change";
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

GSlider::Orientation GSlider::getOrientation() const {
    return _iqslider->orientation() == Qt::Horizontal ? HORIZONTAL : VERTICAL;
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

void GSlider::setMajorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GSlider::setMax", "max (" + std::to_string(max) + ") cannot be less than min (" + std::to_string(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqslider->setMaximum(max);
    });
}

void GSlider::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GSlider::setMin", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
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
    require::require(min <= max, "GSlider::setRange", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min, max]() {
        _iqslider->setRange(min, max);
    });
}

void GSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void GSlider::setState(int min, int max, int value) {
    require::require(min <= max, "GSlider::setState", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::setState", "value");
    GThread::runOnQtGuiThread([this, min, max, value]() {
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
}

void GSlider::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GSlider::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setValue(value);
    });
}


_Internal_QSlider::_Internal_QSlider(GSlider* gslider, Qt::Orientation orientation, QWidget* parent)
        : QSlider(orientation, parent),
          _gslider(gslider) {
    require::nonNull(gslider, "_Internal_QSlider::constructor");
    setObjectName(QString::fromStdString("_Internal_QSlider_" + std::to_string(gslider->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Internal_QSlider::detach() {
    _gslider = nullptr;
}

void _Internal_QSlider::handleChange(int /* value */) {
    if (!_gslider) {
        return;
    }
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gslider);
    changeEvent.setActionCommand(_gslider->getActionCommand());
    _gslider->fireEvent(changeEvent);
}

void _Internal_QSlider::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QSlider::keyPressEvent", "event");
    if (_gslider && _gslider->isAcceptingEvent("keypress")) {
        event->accept();
        _gslider->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QSlider::keyPressEvent(event);   // call super
        }
    } else {
        QSlider::keyPressEvent(event);   // call super
    }
}

void _Internal_QSlider::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QSlider::keyPressEvent", "event");
    if (_gslider && _gslider->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gslider->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QSlider::keyReleaseEvent(event);   // call super
        }
    } else {
        QSlider::keyReleaseEvent(event);   // call super
    }
}

QSize _Internal_QSlider::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSlider::sizeHint();
    }
}


