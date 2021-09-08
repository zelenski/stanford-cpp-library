/*
 * File: gscrollbar.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/08/23
 * - renamed to gscrollbar.cpp to replace Java version
 * @version 2018/07/16
 * - initial version
 */

#include "gscrollbar.h"
#include "gthread.h"
#include "require.h"

GScrollBar::GScrollBar(GScrollBar::Orientation orientation,
                         int value,
                         int extent,
                         int min,
                         int max,
                         QWidget* parent) {
    GThread::runOnQtGuiThread([this, orientation, parent]() {
        _iqscrollbar = new _Internal_QScrollBar(this, orientation == VERTICAL ? Qt::Vertical : Qt::Horizontal, getInternalParent(parent));
    });
    setState(value, extent, min, max);
    setVisible(false);   // all widgets are not shown until added to a window
}

GScrollBar::~GScrollBar() {
    // TODO: delete _iqscrollbar;
    _iqscrollbar->detach();
    _iqscrollbar = nullptr;
}

std::string GScrollBar::getActionEventType() const {
    return "change";
}

int GScrollBar::getExtent() const {
    return _iqscrollbar->pageStep();
}

_Internal_QWidget* GScrollBar::getInternalWidget() const {
    return _iqscrollbar;
}

int GScrollBar::getMax() const {
    return _iqscrollbar->maximum();
}

int GScrollBar::getMin() const {
    return _iqscrollbar->minimum();
}

GScrollBar::Orientation GScrollBar::getOrientation() const {
    return _iqscrollbar->orientation() == Qt::Vertical ? VERTICAL : HORIZONTAL;
}

std::string GScrollBar::getType() const {
    return "QGScrollBar";
}

int GScrollBar::getValue() const {
    return _iqscrollbar->value();
}

QWidget* GScrollBar::getWidget() const {
    return static_cast<QWidget*>(_iqscrollbar);
}

void GScrollBar::setExtent(int extent) {
    GThread::runOnQtGuiThread([this, extent]() {
        _iqscrollbar->setPageStep(extent);
    });
}

void GScrollBar::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GScrollBar::setMax", "max (" + std::to_string(max) + ") cannot be less than min (" + std::to_string(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqscrollbar->setMaximum(max);
    });
    updateSize();
}

void GScrollBar::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GScrollBar::setMin", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min]() {
        _iqscrollbar->setMinimum(min);
    });
    updateSize();
}

void GScrollBar::setState(int value, int extent, int min, int max) {
    require::require(min <= max, "GScrollBar::setState", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GScrollBar::setState", "value");
    GThread::runOnQtGuiThread([this, value, extent, min, max]() {
        _iqscrollbar->setRange(min, max);
        _iqscrollbar->setValue(value);
        _iqscrollbar->setPageStep(extent);
    });
    updateSize();
}

void GScrollBar::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GScrollBar::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqscrollbar->setValue(value);
    });
}

void GScrollBar::updateSize() {
    GThread::runOnQtGuiThread([this]() {
        _iqscrollbar->update();
        _iqscrollbar->updateGeometry();
        if (getOrientation() == VERTICAL) {
            _iqscrollbar->setPreferredSize(_iqscrollbar->sizeHint().width(), getMax() - getMin() + 1);
        } else {
            _iqscrollbar->setPreferredSize(getMax() - getMin() + 1, _iqscrollbar->sizeHint().height());
        }
    });
}


_Internal_QScrollBar::_Internal_QScrollBar(GScrollBar* gscrollbar, Qt::Orientation orientation, QWidget* parent)
        : QScrollBar(orientation, parent),
          _gscrollbar(gscrollbar) {
    require::nonNull(gscrollbar, "_Internal_QScrollBar::constructor");
    setObjectName(QString::fromStdString("_Internal_QScrollBar_" + std::to_string(gscrollbar->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleValueChange(int)));
}

void _Internal_QScrollBar::detach() {
    _gscrollbar = nullptr;
}

void _Internal_QScrollBar::handleValueChange(int /* value */) {
    if (!_gscrollbar) {
        return;
    }
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gscrollbar);
    changeEvent.setActionCommand(_gscrollbar->getActionCommand());
    _gscrollbar->fireEvent(changeEvent);
}

QSize _Internal_QScrollBar::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollBar::sizeHint();
    }
}


