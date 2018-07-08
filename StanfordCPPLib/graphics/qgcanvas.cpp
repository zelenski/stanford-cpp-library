/*
 * File: qgcanvas.cpp
 * ------------------
 *
 * @version 2018/06/30
 * - initial version
 */

#include "qgcanvas.h"
#include <QPainter>
#include "qgwindow.h"
#include "strlib.h"

_Internal_QCanvas::_Internal_QCanvas(QGCanvas* qgcanvas, QWidget* parent)
        : QWidget(parent),
          _qgcanvas(qgcanvas) {
    // set default white background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
    setMouseTracking(true);   // causes mouse move events to occur
}

void _Internal_QCanvas::enterEvent(QEvent* event) {
    if (!_qgcanvas->hasEventHandler("mouseenter")) return;
    fireQGEvent(event, QGEvent::MOUSE_ENTERED, "mouseenter");
}

void _Internal_QCanvas::leaveEvent(QEvent* event) {
    if (!_qgcanvas->hasEventHandler("mouseexit")) return;
    fireQGEvent(event, QGEvent::MOUSE_EXITED, "mouseexit");
}

void _Internal_QCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (!_qgcanvas->hasEventHandler("mousemove")
            && !_qgcanvas->hasEventHandler("mousedrag")) return;
    fireQGEvent(event, QGEvent::MOUSE_MOVED, "mousemove");
    if (event->buttons() != 0) {
        // mouse drag
        fireQGEvent(event, QGEvent::MOUSE_DRAGGED, "mousedrag");
    }
}

void _Internal_QCanvas::mousePressEvent(QMouseEvent* event) {
    if (!_qgcanvas->hasEventHandler("mousepress")) return;
    fireQGEvent(event, QGEvent::MOUSE_PRESSED, "mousepress");
}

void _Internal_QCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (_qgcanvas->hasEventHandler("mouserelease")) {
        fireQGEvent(event, QGEvent::MOUSE_RELEASED, "mouserelease");
    }

    if (_qgcanvas->hasEventHandler("click")) {
        fireQGEvent(event, QGEvent::MOUSE_CLICKED, "click");
    }
}

void _Internal_QCanvas::paintEvent(QPaintEvent* event) {
    QPainter g(this);
    // g.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    // g.setRenderHints(QPainter::HighQualityAntialiasing);
    g.setRenderHint(QPainter::Antialiasing, QGObject::isAntiAliasing());

    _qgcanvas->draw(&g);
    g.end();

    QWidget::paintEvent(event);
}

void _Internal_QCanvas::fireQGEvent(QEvent* /* event */,
                                    QGEvent::EventType eventType,
                                    const std::string& eventName) {
    QGEvent mouseEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _qgcanvas);
    // TODO
//    mouseEvent._button = (int) event->button();
//    mouseEvent._x = event->x;
//    mouseEvent._y = event->y;
    _qgcanvas->fireEvent(mouseEvent);
}

void _Internal_QCanvas::fireQGEvent(QMouseEvent* event,
                                    QGEvent::EventType eventType,
                                    const std::string& eventName) {
    QGEvent mouseEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _qgcanvas);
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _qgcanvas->fireEvent(mouseEvent);
}

QGCanvas::QGCanvas(QWidget* parent) {
    _iqcanvas = new _Internal_QCanvas(this, getInternalParent(parent));
    _qgcompound.setWidget(_iqcanvas);
}

QGCanvas::~QGCanvas() {
    // TODO: delete _qgcanvas;
    _iqcanvas = nullptr;
}

void QGCanvas::add(QGObject* gobj) {
    _qgcompound.add(gobj);
}

void QGCanvas::add(QGObject* gobj, double x, double y) {
    _qgcompound.add(gobj, x, y);
}

void QGCanvas::add(QGObject& gobj) {
    _qgcompound.add(gobj);
}

void QGCanvas::add(QGObject& gobj, double x, double y) {
    _qgcompound.add(gobj, x, y);
}

void QGCanvas::clear() {
    _qgcompound.clear();
}

bool QGCanvas::contains(double x, double y) const {
    return _qgcompound.contains(x, y);
}

void QGCanvas::draw(QPainter* painter) {
    _qgcompound.draw(painter);
}

QGObject* QGCanvas::getElement(int index) const {
    return _qgcompound.getElement(index);
}

QGObject* QGCanvas::getElementAt(double x, double y) const {
    return _qgcompound.getElementAt(x, y);
}

int QGCanvas::getElementCount() const {
    return _qgcompound.getElementCount();
}

QWidget* QGCanvas::getWidget() const {
    return static_cast<QWidget*>(_iqcanvas);
}

bool QGCanvas::isAutoRepaint() const {
    return _qgcompound.isAutoRepaint();
}

void QGCanvas::remove(QGObject* gobj) {
    _qgcompound.remove(gobj);
}

void QGCanvas::remove(QGObject& gobj) {
    _qgcompound.remove(gobj);
}

void QGCanvas::removeAll() {
    _qgcompound.removeAll();
}

void QGCanvas::repaint() {
    _qgcompound.repaint();
}

void QGCanvas::setAutoRepaint(bool autoRepaint) {
    _qgcompound.setAutoRepaint(autoRepaint);
}

std::string QGCanvas::getType() const {
    return "QGCanvas";
}
