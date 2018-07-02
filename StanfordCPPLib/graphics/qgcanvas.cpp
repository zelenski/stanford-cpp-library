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

_Q_Internal_Canvas::_Q_Internal_Canvas(QGCanvas* canvas, QWidget* parent)
        : QWidget(parent),
          _canvas(canvas) {
    // set default white background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
}

void _Q_Internal_Canvas::paintEvent(QPaintEvent* event) {
    QPainter g(this);
    // g.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    // g.setRenderHints(QPainter::HighQualityAntialiasing);
    g.setRenderHint(QPainter::Antialiasing, QGObject::isAntiAliasing());

    _canvas->draw(&g);
    g.end();

    QWidget::paintEvent(event);
}

QGCanvas::QGCanvas(QWidget* parent)
        : _canvas(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    _compound.setWidget(&_canvas);
}

void QGCanvas::add(QGObject* gobj) {
    _compound.add(gobj);
}

void QGCanvas::add(QGObject* gobj, double x, double y) {
    _compound.add(gobj, x, y);
}

void QGCanvas::clear() {
    _compound.clear();
}

bool QGCanvas::contains(double x, double y) const {
    return _compound.contains(x, y);
}

void QGCanvas::draw(QPainter* painter) {
    _compound.draw(painter);
}

QGObject* QGCanvas::getElement(int index) const {
    return _compound.getElement(index);
}

QGObject* QGCanvas::getElementAt(double x, double y) const {
    return _compound.getElementAt(x, y);
}

int QGCanvas::getElementCount() const {
    return _compound.getElementCount();
}

bool QGCanvas::isAutoRepaint() const {
    return _compound.isAutoRepaint();
}

void QGCanvas::remove(QGObject* gobj) {
    _compound.remove(gobj);
}

void QGCanvas::removeAll() {
    _compound.removeAll();
}

void QGCanvas::repaint() {
    _compound.repaint();

}

void QGCanvas::setAutoRepaint(bool autoRepaint) {
    _compound.setAutoRepaint(autoRepaint);
}

std::string QGCanvas::getType() const {
    return "QGCanvas";
}

QWidget* QGCanvas::getWidget() const {
    return (QWidget*) &_canvas;
}
