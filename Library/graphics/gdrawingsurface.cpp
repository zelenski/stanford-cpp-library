/*
 * File: gdrawingsurface.cpp
 * -------------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gdrawingsurface.cpp to replace Java version
 * @version 2018/07/11
 * - initial version
 */

#include "gdrawingsurface.h"
#include <QPainter>
#include "gcolor.h"
#include "gfont.h"
#include "require.h"

GDrawingSurface::GDrawingSurface()
        : _forwardTarget(nullptr),
          _backgroundColor("0xffffffff"),
          _color(""),
          _fillColor(""),
          _font(""),
          _backgroundColorInt(0xffffffff),
          _colorInt(0),
          _fillColorInt(0),
          _lineStyle(GObject::LINE_SOLID),
          _lineWidth(1),
          _autoRepaint(true) {
    // empty
}

GDrawingSurface::~GDrawingSurface() {
    _forwardTarget = nullptr;
}

void GDrawingSurface::checkBounds(const std::string& member, double x, double y, double width, double height) const {
    require::inRange2D(x, y, width - 1, height - 1, member);
}

void GDrawingSurface::checkColor(const std::string& /* member */, int /* rgb */) const {
    // I think this code is wrong; it ignores the possibility of alpha values
    // or of the top bits being set to 255 (all 1) by default by Qt libraries
//    if (rgb < 0x0 || rgb > 0xffffff) {
//        error(member + ": color is outside of range 0x000000 through 0xffffff");
//    }
}

void GDrawingSurface::checkSize(const std::string& /* member */, double /* width */, double /* height */) const {
//    if (width < 0 || height < 0) {
//        error(member + ": width/height cannot be negative");
//    }
//    if (width > GCanvas::WIDTH_HEIGHT_MAX
//            || height > GCanvas::WIDTH_HEIGHT_MAX) {
//        error(getType() + "::" + member + ": width/height too large (cannot exceed "
//              + std::to_string(GCanvas::WIDTH_HEIGHT_MAX));
//    }
}

void GDrawingSurface::conditionalRepaint() {
    if (_forwardTarget) {
        _forwardTarget->conditionalRepaint();
    } else {
        if (isAutoRepaint()) {
            repaint();
        }
    }
}

void GDrawingSurface::conditionalRepaintRegion(int x, int y, int width, int height) {
    if (isAutoRepaint()) {
        repaintRegion(x, y, width, height);
    }
}

void GDrawingSurface::conditionalRepaintRegion(const GRectangle& bounds) {
    if (isAutoRepaint()) {
        repaintRegion(bounds);
    }
}

void GDrawingSurface::draw(GObject* gobj, double x, double y) {
    require::nonNull(gobj, "GDrawingSurface::draw");
    gobj->setLocation(x, y);
    draw(gobj);
}

void GDrawingSurface::draw(GObject& gobj) {
    draw(&gobj);
}

void GDrawingSurface::draw(GObject& gobj, double x, double y) {
    gobj.setLocation(x, y);
    draw(&gobj);
}

void GDrawingSurface::drawArc(double x, double y, double width, double height, double start, double sweep) {
    GArc arc(x, y, width, height, start, sweep);
    initializeGObject(arc);
    draw(arc);
}

void GDrawingSurface::drawImage(const std::string& filename, double x, double y) {
    GImage image(filename, x, y);
    draw(image);
}

void GDrawingSurface::drawLine(const GPoint& p0, const GPoint& p1) {
    drawLine(p0.x, p0.y, p1.x, p1.y);
}

void GDrawingSurface::drawLine(double x0, double y0, double x1, double y1) {
    GLine line(x0, y0, x1, y1);
    initializeGObject(line);
    draw(line);
}

void GDrawingSurface::drawOval(const GRectangle& bounds) {
    drawOval(bounds.x, bounds.y, bounds.width, bounds.height);
}

void GDrawingSurface::drawOval(double x, double y, double width, double height) {
    GOval oval(x, y, width, height);
    initializeGObject(oval);
    draw(oval);
}

GPoint GDrawingSurface::drawPolarLine(const GPoint& p0, double r, double theta) {
    return drawPolarLine(p0.x, p0.y, r, theta);
}

GPoint GDrawingSurface::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return GPoint(x1, y1);
}

void GDrawingSurface::drawPixel(double x, double y) {
    setPixel(x, y, _colorInt);
}

void GDrawingSurface::drawPixel(double x, double y, int color) {
    setPixel(x, y, color);
}

void GDrawingSurface::drawPixel(double x, double y, const std::string& color) {
    setPixel(x, y, color);
}

void GDrawingSurface::drawPolygon(std::initializer_list<double> coords) {
    GPolygon polygon(coords);
    initializeGObject(polygon);
    draw(polygon);
}

void GDrawingSurface::drawPolygon(std::initializer_list<GPoint> points) {
    GPolygon polygon(points);
    initializeGObject(polygon);
    draw(polygon);
}

void GDrawingSurface::drawRect(const GRectangle& bounds) {
    drawRect(bounds.x, bounds.y, bounds.width, bounds.height);
}

void GDrawingSurface::drawRect(double x, double y, double width, double height) {
    GRect rect(x, y, width, height);
    initializeGObject(rect);
    draw(rect);
}

void GDrawingSurface::drawString(const std::string& text, double x, double y) {
    GText str(text, x, y);
    initializeGObject(str);
    draw(str);
}

void GDrawingSurface::fillArc(double x, double y, double width, double height, double start, double sweep) {
    GArc arc(x, y, width, height, start, sweep);
    initializeGObject(arc, /* filled */ true);
    draw(arc);
}

void GDrawingSurface::fillOval(const GRectangle& bounds) {
    fillOval(bounds.x, bounds.y, bounds.width, bounds.height);
}

void GDrawingSurface::fillOval(double x, double y, double width, double height) {
    GOval oval(x, y, width, height);
    initializeGObject(oval, /* filled */ true);
    draw(oval);
}

void GDrawingSurface::fillPolygon(std::initializer_list<double> coords) {
    GPolygon polygon(coords);
    initializeGObject(polygon, /* filled */ true);
    draw(polygon);
}

void GDrawingSurface::fillPolygon(std::initializer_list<GPoint> points) {
    GPolygon polygon(points);
    initializeGObject(polygon, /* fllled */ true);
    draw(polygon);
}

void GDrawingSurface::fillRect(const GRectangle& bounds) {
    fillRect(bounds.x, bounds.y, bounds.width, bounds.height);
}

void GDrawingSurface::fillRect(double x, double y, double width, double height) {
    GRect rect(x, y, width, height);
    initializeGObject(rect, /* filled */ true);
    draw(rect);
}

int GDrawingSurface::getARGB(double x, double y) const {
    return getPixelARGB(x, y);
}

std::string GDrawingSurface::getBackground() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackground();
    } else {
        return _backgroundColor;
    }
}

int GDrawingSurface::getBackgroundInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackgroundInt();
    } else {
        return _backgroundColorInt;
    }
}

std::string GDrawingSurface::getColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getColor();
    } else {
        return _color;
    }
}

int GDrawingSurface::getColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getColorInt();
    } else {
        return _colorInt;
    }
}

std::string GDrawingSurface::getFillColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColor();
    } else {
        return _fillColor;
    }
}

int GDrawingSurface::getFillColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColorInt();
    } else {
        return _fillColorInt;
    }
}

std::string GDrawingSurface::getFont() const {
    if (_forwardTarget) {
        return _forwardTarget->getFont();
    } else {
        return _font;
    }
}

std::string GDrawingSurface::getForeground() const {
    return getColor();
}

int GDrawingSurface::getForegroundInt() const {
    return getColorInt();
}

GObject::LineStyle GDrawingSurface::getLineStyle() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineStyle();
    } else {
        return _lineStyle;
    }
}

double GDrawingSurface::getLineWidth() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineWidth();
    } else {
        return _lineWidth;
    }
}

std::string GDrawingSurface::getPixelString(double x, double y) const {
    return GColor::convertRGBToColor(getPixel(x, y));
}

int GDrawingSurface::getRGB(double x, double y) const {
    return getPixel(x, y);
}

std::string GDrawingSurface::getRGBString(double x, double y) const {
    return GColor::convertRGBToColor(getPixel(x, y));
}

void GDrawingSurface::initializeGObject(GObject& obj, bool fill) {
    initializeGObject(&obj, fill);
}

void GDrawingSurface::initializeGObject(GObject* obj, bool fill) {
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

bool GDrawingSurface::isAutoRepaint() const {
    if (_forwardTarget) {
        return _forwardTarget->isAutoRepaint();
    } else {
        return _autoRepaint;
    }
}

bool GDrawingSurface::isRepaintImmediately() const {
    return isAutoRepaint();
}

void GDrawingSurface::repaintRegion(const GRectangle& bounds) {
    repaintRegion((int) bounds.x, (int) bounds.y,
                  (int) bounds.width, (int) bounds.height);
}

void GDrawingSurface::setAutoRepaint(bool autoRepaint) {
    if (_forwardTarget) {
        _forwardTarget->setAutoRepaint(autoRepaint);
    } else {
        _autoRepaint = autoRepaint;
    }
}

void GDrawingSurface::setBackground(int color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColorInt = color;
        _backgroundColor = GColor::convertRGBToColor(color);
    }
}

void GDrawingSurface::setBackground(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColor = color;
        _backgroundColorInt = GColor::convertColorToRGB(color);
    }
}

void GDrawingSurface::setColor(int color) {
    // sets both color and fillColor
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
        _forwardTarget->setFillColor(color);
    } else {
        _colorInt = color;
        _color = GColor::convertRGBToColor(color);
        _fillColorInt = color;
        _fillColor = _color;
    }
}

void GDrawingSurface::setColor(const std::string& color) {
    // sets both color and fillColor
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
        _forwardTarget->setFillColor(color);
    } else {
        _color = color;
        _colorInt = GColor::convertColorToRGB(color);
        _fillColor = color;
        _fillColorInt = _colorInt;
    }
}

void GDrawingSurface::setDrawingForwardTarget(GDrawingSurface* forwardTarget) {
    _forwardTarget = forwardTarget;
}

void GDrawingSurface::setFillColor(int color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColorInt = color;
        _fillColor = GColor::convertRGBToColor(color);
    }
}

void GDrawingSurface::setFillColor(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColor = color;
        _fillColorInt = GColor::convertColorToRGB(color);
    }
}

void GDrawingSurface::setFont(const QFont& font) {
    if (_forwardTarget) {
        _forwardTarget->setFont(font);
    } else {
        _font = GFont::toFontString(font);
    }
}

void GDrawingSurface::setFont(const std::string& font) {
    if (_forwardTarget) {
        _forwardTarget->setFont(font);
    } else {
        _font = font;
    }
}

void GDrawingSurface::setForeground(int color) {
    setColor(color);
}

void GDrawingSurface::setForeground(const std::string& color) {
    setColor(color);
}

void GDrawingSurface::setLineStyle(GObject::LineStyle lineStyle) {
    if (_forwardTarget) {
        _forwardTarget->setLineStyle(lineStyle);
    } else {
        _lineStyle = lineStyle;
    }
}

void GDrawingSurface::setLineWidth(double lineWidth) {
    if (_forwardTarget) {
        _forwardTarget->setLineWidth(lineWidth);
    } else {
        _lineWidth = lineWidth;
    }
}

void GDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    setPixel(x, y, GColor::convertRGBToRGB(r, g, b));
}

void GDrawingSurface::setPixel(double x, double y, const std::string& color) {
    setPixel(x, y, GColor::convertColorToRGB(color));
}

void GDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, GColor::convertARGBToARGB(a, r, g, b));
}

void GDrawingSurface::setRepaintImmediately(bool autoRepaint) {
    setAutoRepaint(autoRepaint);
}

void GDrawingSurface::setRGB(double x, double y, int rgb) {
    setPixel(x, y, rgb);
}

void GDrawingSurface::setRGB(double x, double y, int r, int g, int b) {
    this->setPixel(x, y, r, g, b);
}

void GDrawingSurface::setRGB(double x, double y, const std::string& color) {
    setPixel(x, y, GColor::convertColorToRGB(color));
}


void GForwardDrawingSurface::clear() {
    if (_forwardTarget) {
        _forwardTarget->clear();
    }
}

void GForwardDrawingSurface::draw(GObject* obj) {
    ensureForwardTarget();
    _forwardTarget->draw(obj);
}

void GForwardDrawingSurface::draw(GObject* obj, double x, double y) {
    ensureForwardTarget();
    _forwardTarget->draw(obj, x, y);
}

void GForwardDrawingSurface::draw(GObject& obj) {
    ensureForwardTarget();
    _forwardTarget->draw(obj);
}

void GForwardDrawingSurface::draw(GObject& obj, double x, double y) {
    ensureForwardTarget();
    _forwardTarget->draw(obj, x, y);
}

void GForwardDrawingSurface::draw(QPainter* painter) {
    ensureForwardTarget();
    _forwardTarget->draw(painter);
}

void GForwardDrawingSurface::ensureForwardTargetConstHack() const {
    if (!_forwardTarget) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        GForwardDrawingSurface* hack = (GForwardDrawingSurface*) this;
        hack->ensureForwardTarget();
    }
}

int GForwardDrawingSurface::getPixel(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixel(x, y);
}

int GForwardDrawingSurface::getPixelARGB(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelARGB(x, y);
}

Grid<int> GForwardDrawingSurface::getPixels() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixels();
}

Grid<int> GForwardDrawingSurface::getPixelsARGB() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelsARGB();
}

bool GForwardDrawingSurface::isAutoRepaint() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->isAutoRepaint();
}

void GForwardDrawingSurface::repaint() {
    if (_forwardTarget) {
        _forwardTarget->repaint();
    }
}

void GForwardDrawingSurface::repaintRegion(int x, int y, int width, int height) {
    if (_forwardTarget) {
        _forwardTarget->repaintRegion(x, y, width, height);
    }
}

void GForwardDrawingSurface::setAutoRepaint(bool autoRepaint) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(autoRepaint);
}

void GForwardDrawingSurface::setBackground(int color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void GForwardDrawingSurface::setBackground(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void GForwardDrawingSurface::setColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void GForwardDrawingSurface::setColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void GForwardDrawingSurface::setFillColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void GForwardDrawingSurface::setFillColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void GForwardDrawingSurface::setFont(const QFont& font) {
    ensureForwardTarget();
    _forwardTarget->setFont(font);
}

void GForwardDrawingSurface::setFont(const std::string& font) {
    ensureForwardTarget();
    _forwardTarget->setFont(font);
}

void GForwardDrawingSurface::setLineWidth(double lineWidth) {
    ensureForwardTarget();
    _forwardTarget->setLineWidth(lineWidth);
}

void GForwardDrawingSurface::setPixel(double x, double y, int rgb) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, rgb);
}

void GForwardDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, r, g, b);
}

void GForwardDrawingSurface::setPixelARGB(double x, double y, int argb) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, argb);
}

void GForwardDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, a, r, g, b);
}

void GForwardDrawingSurface::setPixels(const Grid<int>& pixels) {
    ensureForwardTarget();
    _forwardTarget->setPixels(pixels);
}

void GForwardDrawingSurface::setPixelsARGB(const Grid<int>& pixelsARGB) {
    ensureForwardTarget();
    _forwardTarget->setPixelsARGB(pixelsARGB);
}

void GForwardDrawingSurface::setRepaintImmediately(bool repaintImmediately) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(repaintImmediately);
}
