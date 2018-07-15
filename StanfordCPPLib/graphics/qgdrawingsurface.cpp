/*
 * File: qgdrawingsurface.cpp
 * --------------------------
 *
 * @version 2018/07/11
 * - initial version
 */

#include "qgdrawingsurface.h"
#include <QPainter>
#include "qgcolor.h"
#include "strlib.h"

QGDrawingSurface::QGDrawingSurface()
        : _forwardTarget(nullptr),
          _backgroundColor("0xffffffff"),
          _color(""),
          _fillColor(""),
          _font(""),
          _backgroundColorInt(0xffffffff),
          _colorInt(0),
          _fillColorInt(0),
          _lineStyle(QGObject::LINE_SOLID),
          _lineWidth(1),
          _autoRepaint(true) {
    // empty
}

QGDrawingSurface::~QGDrawingSurface() {
    _forwardTarget = nullptr;
}

void QGDrawingSurface::checkBounds(const std::string& /* member */, double /* x */, double /* y */) const {
//    if (!inBounds(x, y)) {
//        error(getType() + "::" + member
//              + ": (x=" + integerToString((int) x)
//              + ", y=" + integerToString((int) y)
//              + ") is out of valid range of (0, 0) through ("
//              + integerToString((int) getWidth() - 1) + ", "
//              + integerToString((int) getHeight() - 1) + ")");
//    }
}

void QGDrawingSurface::checkColor(const std::string& /* member */, int /* rgb */) const {
    // I think this code is wrong; it ignores the possibility of alpha values
    // or of the top bits being set to 255 (all 1) by default by Qt libraries
//    if (rgb < 0x0 || rgb > 0xffffff) {
//        error("QGBufferedImage::" + member
//              + ": color is outside of range 0x000000 through 0xffffff");
//    }
}

void QGDrawingSurface::checkSize(const std::string& /* member */, double /* width */, double /* height */) const {
//    if (width < 0 || height < 0) {
//        error(getType() + "::" + member + ": width/height cannot be negative");
//    }
//    if (width > QGCanvas::WIDTH_HEIGHT_MAX
//            || height > QGCanvas::WIDTH_HEIGHT_MAX) {
//        error(getType() + "::" + member + ": width/height too large (cannot exceed "
//              + integerToString(QGCanvas::WIDTH_HEIGHT_MAX));
//    }
}

void QGDrawingSurface::conditionalRepaint() {
    if (_forwardTarget) {
        _forwardTarget->conditionalRepaint();
    } else {
        if (isAutoRepaint()) {
            repaint();
        }
    }
}

void QGDrawingSurface::conditionalRepaintRegion(int x, int y, int width, int height) {
    if (isAutoRepaint()) {
        repaintRegion(x, y, width, height);
    }
}

void QGDrawingSurface::conditionalRepaintRegion(const QGRectangle& bounds) {
    if (isAutoRepaint()) {
        repaintRegion(bounds);
    }
}

void QGDrawingSurface::draw(QGObject* gobj, double x, double y) {
    gobj->setLocation(x, y);
    draw(gobj);
}

void QGDrawingSurface::draw(QGObject& gobj) {
    draw(&gobj);
}

void QGDrawingSurface::draw(QGObject& gobj, double x, double y) {
    gobj.setLocation(x, y);
    draw(&gobj);
}

void QGDrawingSurface::drawArc(double x, double y, double width, double height, double start, double sweep) {
    QGArc* arc = new QGArc(x, y, width, height, start, sweep);
    initializeQGObject(arc);
    draw(arc);
}

void QGDrawingSurface::drawImage(const std::string& filename, double x, double y) {
    QGImage* image = new QGImage(filename, x, y);
    draw(image);
}

void QGDrawingSurface::drawLine(const QGPoint& p0, const QGPoint& p1) {
    drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
}

void QGDrawingSurface::drawLine(double x0, double y0, double x1, double y1) {
    QGLine* line = new QGLine(x0, y0, x1, y1);
    initializeQGObject(line);
    draw(line);
}

void QGDrawingSurface::drawOval(const QGRectangle& bounds) {
    drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGDrawingSurface::drawOval(double x, double y, double width, double height) {
    QGOval* oval = new QGOval(x, y, width, height);
    initializeQGObject(oval);
    draw(oval);
}

QGPoint QGDrawingSurface::drawPolarLine(const QGPoint& p0, double r, double theta) {
    return drawPolarLine(p0.getX(), p0.getY(), r, theta);
}

QGPoint QGDrawingSurface::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return QGPoint(x1, y1);
}

void QGDrawingSurface::drawPixel(double x, double y) {
    setPixel(x, y, _colorInt);
}

void QGDrawingSurface::drawPixel(double x, double y, int color) {
    setPixel(x, y, color);
}

void QGDrawingSurface::drawPixel(double x, double y, const std::string& color) {
    setPixel(x, y, color);
}

void QGDrawingSurface::drawPolygon(std::initializer_list<double> coords) {
    QGPolygon* polygon = new QGPolygon(coords);
    initializeQGObject(polygon);
    draw(polygon);
}

void QGDrawingSurface::drawPolygon(std::initializer_list<QGPoint> points) {
    QGPolygon* polygon = new QGPolygon(points);
    initializeQGObject(polygon);
    draw(polygon);
}

void QGDrawingSurface::drawRect(const QGRectangle& bounds) {
    drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGDrawingSurface::drawRect(double x, double y, double width, double height) {
    QGRect* rect = new QGRect(x, y, width, height);
    initializeQGObject(rect);
    draw(rect);
}

void QGDrawingSurface::drawString(const std::string& text, double x, double y) {
    QGString* str = new QGString(text, x, y);
    initializeQGObject(str);
    draw(str);
}

void QGDrawingSurface::fillArc(double x, double y, double width, double height, double start, double sweep) {
    QGArc* arc = new QGArc(x, y, width, height, start, sweep);
    initializeQGObject(arc, /* filled */ true);
    draw(arc);
}

void QGDrawingSurface::fillOval(const QGRectangle& bounds) {
    fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGDrawingSurface::fillOval(double x, double y, double width, double height) {
    QGOval* oval = new QGOval(x, y, width, height);
    initializeQGObject(oval, /* filled */ true);
    draw(oval);
}

void QGDrawingSurface::fillPolygon(std::initializer_list<double> coords) {
    QGPolygon* polygon = new QGPolygon(coords);
    initializeQGObject(polygon, /* filled */ true);
    draw(polygon);
}

void QGDrawingSurface::fillRect(const QGRectangle& bounds) {
    fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGDrawingSurface::fillRect(double x, double y, double width, double height) {
    QGRect* rect = new QGRect(x, y, width, height);
    initializeQGObject(rect, /* filled */ true);
    draw(rect);
}

int QGDrawingSurface::getARGB(double x, double y) const {
    return getPixelARGB(x, y);
}

std::string QGDrawingSurface::getBackground() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackground();
    } else {
        return _backgroundColor;
    }
}

int QGDrawingSurface::getBackgroundInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackgroundInt();
    } else {
        return _backgroundColorInt;
    }
}

std::string QGDrawingSurface::getColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getColor();
    } else {
        return _color;
    }
}

int QGDrawingSurface::getColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getColorInt();
    } else {
        return _colorInt;
    }
}

std::string QGDrawingSurface::getFillColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColor();
    } else {
        return _fillColor;
    }
}

int QGDrawingSurface::getFillColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColorInt();
    } else {
        return _fillColorInt;
    }
}

std::string QGDrawingSurface::getFont() const {
    if (_forwardTarget) {
        return _forwardTarget->getFont();
    } else {
        return _font;
    }
}

std::string QGDrawingSurface::getForeground() const {
    return getColor();
}

int QGDrawingSurface::getForegroundInt() const {
    return getColorInt();
}

QGObject::LineStyle QGDrawingSurface::getLineStyle() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineStyle();
    } else {
        return _lineStyle;
    }
}

double QGDrawingSurface::getLineWidth() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineWidth();
    } else {
        return _lineWidth;
    }
}

std::string QGDrawingSurface::getPixelString(double x, double y) const {
    return QGColor::convertRGBToColor(getPixel(x, y));
}

int QGDrawingSurface::getRGB(double x, double y) const {
    return getPixel(x, y);
}

std::string QGDrawingSurface::getRGBString(double x, double y) const {
    return QGColor::convertRGBToColor(getPixel(x, y));
}

void QGDrawingSurface::initializeQGObject(QGObject* obj, bool fill) {
    if (!obj) {
        return;
    }
    obj->setColor(getColor());
    if (fill) {
        obj->setFilled(true);
        obj->setFillColor(getFillColor());
    }
    obj->setFont(getFont());
    obj->setLineStyle(getLineStyle());
    obj->setLineWidth(getLineWidth());
}

bool QGDrawingSurface::isAutoRepaint() const {
    if (_forwardTarget) {
        return _forwardTarget->isAutoRepaint();
    } else {
        return _autoRepaint;
    }
}

bool QGDrawingSurface::isRepaintImmediately() const {
    return isAutoRepaint();
}

void QGDrawingSurface::repaintRegion(const QGRectangle& bounds) {
    repaintRegion((int) bounds.getX(), (int) bounds.getY(),
                  (int) bounds.getWidth(), (int) bounds.getHeight());
}

void QGDrawingSurface::setAutoRepaint(bool autoRepaint) {
    if (_forwardTarget) {
        _forwardTarget->setAutoRepaint(autoRepaint);
    } else {
        _autoRepaint = autoRepaint;
    }
}

void QGDrawingSurface::setBackground(int color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColorInt = color;
        _backgroundColor = QGColor::convertRGBToColor(color);
    }
}

void QGDrawingSurface::setBackground(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColor = color;
        _backgroundColorInt = QGColor::convertColorToRGB(color);
    }
}

void QGDrawingSurface::setColor(int color) {
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
    } else {
        _colorInt = color;
        _color = QGColor::convertRGBToColor(color);
    }
}

void QGDrawingSurface::setColor(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
    } else {
        _color = color;
        _colorInt = QGColor::convertColorToRGB(color);
    }
}

void QGDrawingSurface::setDrawingForwardTarget(QGDrawingSurface* forwardTarget) {
    _forwardTarget = forwardTarget;
}

void QGDrawingSurface::setFillColor(int color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColorInt = color;
        _fillColor = QGColor::convertRGBToColor(color);
    }
}

void QGDrawingSurface::setFillColor(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColor = color;
        _fillColorInt = QGColor::convertColorToRGB(color);
    }
}

void QGDrawingSurface::setFont(const std::string& font) {
    if (_forwardTarget) {
        _forwardTarget->setFont(font);
    } else {
        _font = font;
    }
}

void QGDrawingSurface::setForeground(int color) {
    setColor(color);
}

void QGDrawingSurface::setForeground(const std::string& color) {
    setColor(color);
}

void QGDrawingSurface::setLineStyle(QGObject::LineStyle lineStyle) {
    if (_forwardTarget) {
        _forwardTarget->setLineStyle(lineStyle);
    } else {
        _lineStyle = lineStyle;
    }
}

void QGDrawingSurface::setLineWidth(double lineWidth) {
    if (_forwardTarget) {
        _forwardTarget->setLineWidth(lineWidth);
    } else {
        _lineWidth = lineWidth;
    }
}

void QGDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    setPixel(x, y, QGColor::convertRGBToRGB(r, g, b));
}

void QGDrawingSurface::setPixel(double x, double y, const std::string& color) {
    setPixel(x, y, QGColor::convertColorToRGB(color));
}

void QGDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, QGColor::convertARGBToARGB(a, r, g, b));
}

void QGDrawingSurface::setRepaintImmediately(bool autoRepaint) {
    setAutoRepaint(autoRepaint);
}

void QGDrawingSurface::setRGB(double x, double y, int rgb) {
    setPixel(x, y, rgb);
}

void QGDrawingSurface::setRGB(double x, double y, int r, int g, int b) {
    this->setPixel(x, y, r, g, b);
}

void QGDrawingSurface::setRGB(double x, double y, const std::string& color) {
    setPixel(x, y, QGColor::convertColorToRGB(color));
}


void QGForwardDrawingSurface::clear() {
    if (_forwardTarget) {
        _forwardTarget->clear();
    }
}

void QGForwardDrawingSurface::draw(QGObject* obj) {
    ensureForwardTarget();
    _forwardTarget->draw(obj);
}

void QGForwardDrawingSurface::draw(QPainter* painter) {
    ensureForwardTarget();
    _forwardTarget->draw(painter);
}

void QGForwardDrawingSurface::ensureForwardTargetConstHack() const {
    if (!_forwardTarget) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        QGForwardDrawingSurface* hack = (QGForwardDrawingSurface*) this;
        hack->ensureForwardTarget();
    }
}

int QGForwardDrawingSurface::getPixel(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixel(x, y);
}

int QGForwardDrawingSurface::getPixelARGB(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelARGB(x, y);
}

Grid<int> QGForwardDrawingSurface::getPixels() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixels();
}

Grid<int> QGForwardDrawingSurface::getPixelsARGB() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelsARGB();
}

bool QGForwardDrawingSurface::isAutoRepaint() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->isAutoRepaint();
}

void QGForwardDrawingSurface::repaint() {
    if (_forwardTarget) {
        _forwardTarget->repaint();
    }
}

void QGForwardDrawingSurface::repaintRegion(int x, int y, int width, int height) {
    if (_forwardTarget) {
        _forwardTarget->repaintRegion(x, y, width, height);
    }
}

void QGForwardDrawingSurface::setAutoRepaint(bool autoRepaint) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(autoRepaint);
}

void QGForwardDrawingSurface::setBackground(int color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void QGForwardDrawingSurface::setBackground(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void QGForwardDrawingSurface::setColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void QGForwardDrawingSurface::setColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void QGForwardDrawingSurface::setFillColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void QGForwardDrawingSurface::setFillColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void QGForwardDrawingSurface::setFont(const std::string& font) {
    ensureForwardTarget();
    _forwardTarget->setFont(font);
}

void QGForwardDrawingSurface::setLineWidth(double lineWidth) {
    ensureForwardTarget();
    _forwardTarget->setLineWidth(lineWidth);
}

void QGForwardDrawingSurface::setPixel(double x, double y, int rgb) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, rgb);
}

void QGForwardDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, r, g, b);
}

void QGForwardDrawingSurface::setPixelARGB(double x, double y, int argb) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, argb);
}

void QGForwardDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, a, r, g, b);
}

void QGForwardDrawingSurface::setPixels(const Grid<int>& pixels) {
    ensureForwardTarget();
    _forwardTarget->setPixels(pixels);
}

void QGForwardDrawingSurface::setPixelsARGB(const Grid<int>& pixelsARGB) {
    ensureForwardTarget();
    _forwardTarget->setPixelsARGB(pixelsARGB);
}

void QGForwardDrawingSurface::setRepaintImmediately(bool repaintImmediately) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(repaintImmediately);
}
