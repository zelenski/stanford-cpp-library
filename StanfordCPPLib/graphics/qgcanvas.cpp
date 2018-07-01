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
    g.setRenderHint(QPainter::Antialiasing, true);

    _canvas->draw(&g);
    g.end();

    QWidget::paintEvent(event);
}

QGCanvas::QGCanvas(QWidget* parent)
        : _canvas(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    // empty
}

std::string QGCanvas::getType() const {
    return "QGCanvas";
}

QWidget* QGCanvas::getWidget() const {
    return (QWidget*) &_canvas;
}
