/*
 * File: qgscrollbar.cpp
 * ---------------------
 *
 * @version 2018/07/16
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgscrollbar.h"
#include "strlib.h"

_Internal_QScrollBar::_Internal_QScrollBar(QGScrollBar* qgscrollbar, Qt::Orientation orientation, QWidget* parent)
        : QScrollBar(orientation, parent),
          _qgscrollbar(qgscrollbar) {
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleValueChange(int)));
}

void _Internal_QScrollBar::handleValueChange(int /* value */) {
    QGEvent changeEvent(
                /* class  */ QGEvent::CHANGE_EVENT,
                /* type   */ QGEvent::STATE_CHANGED,
                /* name   */ "change",
                /* source */ _qgscrollbar);
    changeEvent.setActionCommand(_qgscrollbar->getActionCommand());
    _qgscrollbar->fireEvent(changeEvent);
}

QSize _Internal_QScrollBar::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollBar::sizeHint();
    }
}


QGScrollBar::QGScrollBar(QGScrollBar::Orientation orientation,
                         int value,
                         int extent,
                         int min,
                         int max,
                         QWidget* parent) {
    _iqscrollbar = new _Internal_QScrollBar(this, orientation == VERTICAL ? Qt::Vertical : Qt::Horizontal, getInternalParent(parent));
    setState(value, extent, min, max);
}

QGScrollBar::~QGScrollBar() {
    // TODO: delete _iqscrollbar;
    _iqscrollbar = nullptr;
}

int QGScrollBar::getExtent() const {
    return _iqscrollbar->pageStep();
}

_Internal_QWidget* QGScrollBar::getInternalWidget() const {
    return _iqscrollbar;
}

int QGScrollBar::getMax() const {
    return _iqscrollbar->maximum();
}

int QGScrollBar::getMin() const {
    return _iqscrollbar->minimum();
}

QGScrollBar::Orientation QGScrollBar::getOrientation() const {
    return _iqscrollbar->orientation() == Qt::Vertical ? VERTICAL : HORIZONTAL;
}

std::string QGScrollBar::getType() const {
    return "QGScrollBar";
}

int QGScrollBar::getValue() const {
    return _iqscrollbar->value();
}

QWidget* QGScrollBar::getWidget() const {
    return static_cast<QWidget*>(_iqscrollbar);
}

void QGScrollBar::removeActionListener() {
    removeEventListener("change");
}

void QGScrollBar::setActionListener(QGEventListener func) {
    setEventListener("change", func);
}

void QGScrollBar::setActionListener(QGEventListenerVoid func) {
    setEventListener("change", func);
}

void QGScrollBar::setExtent(int extent) {
    _iqscrollbar->setPageStep(extent);
}

void QGScrollBar::setMax(int max) {
    _iqscrollbar->setMaximum(max);
    updateSize();
}

void QGScrollBar::setMin(int min) {
    _iqscrollbar->setMinimum(min);
    updateSize();
}

void QGScrollBar::setState(int value, int extent, int min, int max) {
    _iqscrollbar->setRange(min, max);
    _iqscrollbar->setValue(value);
    _iqscrollbar->setPageStep(extent);
    updateSize();
}

void QGScrollBar::setValue(int value) {
    _iqscrollbar->setValue(value);
}

void QGScrollBar::updateSize() {
    _iqscrollbar->update();
    _iqscrollbar->updateGeometry();
    if (getOrientation() == VERTICAL) {
        _iqscrollbar->setPreferredSize(_iqscrollbar->sizeHint().width(), getMax() - getMin() + 1);
    } else {
        _iqscrollbar->setPreferredSize(getMax() - getMin() + 1, _iqscrollbar->sizeHint().height());
    }
}

#endif // SPL_QT_GUI
