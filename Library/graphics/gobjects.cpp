/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the gobjects.h interface.
 *
 * @author Marty Stepp
 * @version 2019/08/13
 * - bug fix for loading GImage (hasError -> !hasError) - thanks to Tyler Conklin
 * @version 2019/05/05
 * - added predictable GLine point ordering * @version 2019/04/23
 * - bug fix for loading GImage from file on Windows related to istream change
 * @version 2019/03/07
 * - added support for loading a GImage directly from istream (htiek)
 * @version 2018/09/14
 * - added opacity support
 * - added GCanvas-to-GImage conversion support
 * @version 2018/08/23
 * - renamed to gobjects.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#include "gobjects.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <QBrush>
#include <QFont>
#include <QPointF>
#include <QPolygon>
#include <QVector>
#include <sstream>
#include <string>
#include "filelib.h"
#include "gmath.h"
#include "gcolor.h"
#include "gfont.h"
#include "gthread.h"
#include "require.h"
#include "strlib.h"
#include "private/static.h"

const double GRoundRect::DEFAULT_CORNER = 10.0;
const std::string GText::DEFAULT_FONT = "Dialog-13";

// static constants
STATIC_CONST_VARIABLE_DECLARE(double, LINE_TOLERANCE, 1.5)
STATIC_CONST_VARIABLE_DECLARE(double, ARC_TOLERANCE, 2.5)
STATIC_VARIABLE_DECLARE_BLANK(QBrush, DEFAULT_BRUSH)
STATIC_VARIABLE_DECLARE_BLANK(QFont, DEFAULT_QFONT)
STATIC_VARIABLE_DECLARE(bool, DEFAULT_QFONT_SET, false)

/**
 * Returns the square of the distance between two points.
 * Used when checking to see if a line touches a given point.
 * @private
 */
static double dsq(double x0, double y0, double x1, double y1);


/* GObject class */

bool GObject::_sAntiAliasing = true;

GObject::GObject(double x, double y, double width, double height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height),
          _lineWidth(1),
          _opacity(1.0),
          _lineStyle(GObject::LINE_SOLID),
          _color(""),
          _colorInt(0),
          _fillColor(""),
          _fillColorInt(0),
          _font(""),
          _fillFlag(false),
          _visible(true),
          _transformed(false),
          _parent(nullptr) {
    // http://doc.qt.io/qt-5/qpen.html#cap-style
    _pen.setJoinStyle(Qt::MiterJoin);   // don't round corners of line edges
    _pen.setMiterLimit(99.0);
    _pen.setCapStyle(Qt::FlatCap);      // don't overextend line endpoint
    _brush.setStyle(Qt::SolidPattern);
}

GObject::~GObject() {
    // empty
}

bool GObject::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        return getBounds().contains(x, y);
    } else {
        return getBounds().contains(x, y);
    }
}

bool GObject::contains(const GPoint& pt) const {
    return contains(pt.x, pt.y);
}

GPoint GObject::getBottomRightLocation() const {
    return GPoint(getRightX(), getBottomY());
}

double GObject::getBottomY() const {
    return getY() + getHeight();
}

GRectangle GObject::getBounds() const {
    if (isTransformed()) {
        // TODO
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    } else {
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    }
}

GPoint GObject::getCenterLocation() const {
    return GPoint(getCenterX(), getCenterY());
}

double GObject::getCenterX() const {
    return getX() + getWidth() / 2;
}

double GObject::getCenterY() const {
    return getY() + getHeight() / 2;
}

std::string GObject::getColor() const {
    return _color;
}

std::string GObject::getFillColor() const {
    return _fillColor;
}

double GObject::getHeight() const {
    return _height;
}

GObject::LineStyle GObject::getLineStyle() const {
    return _lineStyle;
}

double GObject::getLineWidth() const {
    return _lineWidth;
}

GPoint GObject::getLocation() const {
    return GPoint(getX(), getY());
}

double GObject::getOpacity() const {
    return _opacity;
}

GCompound* GObject::getParent() const {
    return _parent;
}

double GObject::getRightX() const {
    return getX() + getWidth();
}

GDimension GObject::getSize() const {
    GRectangle bounds = getBounds();
    return GDimension(bounds.width, bounds.height);
}

double GObject::getWidth() const {
    return _width;
}

double GObject::getX() const {
    return _x;
}

double GObject::getY() const {
    return _y;
}

void GObject::initializeBrushAndPen(QPainter* painter) {
    if (!painter) {
        return;
    }
    if (GColor::hasAlpha(_color)) {
        _pen.setColor(GColor::toQColorARGB(_colorInt));   // allow alpha
    } else {
        _pen.setColor(QColor(_colorInt));
    }
    _pen.setWidth((int) _lineWidth);
    _pen.setStyle(toQtPenStyle(_lineStyle));

    // http://doc.qt.io/qt-5/qpen.html#join-style
    painter->setPen(_pen);

    // font
    if (!STATIC_VARIABLE(DEFAULT_QFONT_SET)) {
        STATIC_VARIABLE(DEFAULT_QFONT) = painter->font();
        STATIC_VARIABLE(DEFAULT_BRUSH).setColor(QColor(0x00ffffff));
    }
    if (_font.empty()) {
        painter->setFont(STATIC_VARIABLE(DEFAULT_QFONT));
    } else {
        painter->setFont(GFont::toQFont(_font));
    }


    // fill color
    if (_fillFlag) {
        if (GColor::hasAlpha(_fillColor)) {
            _brush.setColor(GColor::toQColorARGB(_fillColorInt));   // allow alpha
        } else {
            _brush.setColor(QColor(_fillColorInt));
        }
        painter->setBrush(_brush);
    } else {
        painter->setBrush(STATIC_VARIABLE(DEFAULT_BRUSH));
    }

    // anti-aliasing
    painter->setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
    painter->setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());

    // opacity
    painter->setOpacity(_opacity);

    // transform
    painter->setTransform(_transform, /* combine */ false);
}

bool GObject::isAntiAliasing() {
    return _sAntiAliasing;
}

bool GObject::isFilled() const {
    return _fillFlag;
}

bool GObject::isTransformed() const {
    return _transformed;
}

bool GObject::isVisible() const {
    return _visible;
}

void GObject::move(double dx, double dy) {
    setLocation(getX() + dx, getY() + dy);   // calls repaint
}

void GObject::repaint() {
    // really instructs the GCompound parent to redraw itself
    GCompound* parent = getParent();
    while (parent && parent->getParent()) {
        parent = parent->getParent();
    }
    if (parent) {
        parent->conditionalRepaint();
    }
}

void GObject::resetTransform() {
    _transform = QTransform();
    _transformed = false;
    repaint();
}

void GObject::rotate(double theta) {
    _transformed = true;
    _transform = _transform.rotate(theta);
    repaint();
}

void GObject::scale(double sf) {
    scale(sf, sf);   // calls repaint
}

void GObject::scale(double sx, double sy) {
    _transformed = true;
    _transform = _transform.scale(sx, sy);
    repaint();
}

void GObject::sendBackward() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendBackward(this);
    }
}

void GObject::sendForward() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendForward(this);
    }
}

void GObject::sendToBack() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendToBack(this);
    }
}

void GObject::sendToFront() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendToFront(this);
    }
}

void GObject::setAntiAliasing(bool value) {
    _sAntiAliasing = value;
}

void GObject::setBottomY(double y) {
    setBottomRightLocation(getRightX(), y);   // calls repaint
}

void GObject::setRightX(double x) {
    setBottomRightLocation(x, getBottomY());   // calls repaint
}

void GObject::setBottomRightLocation(double x, double y) {
    setLocation(x - getWidth(), y - getHeight());   // calls repaint
}

void GObject::setBottomRightLocation(const GPoint& pt) {
    setBottomRightLocation(pt.x, pt.y);   // calls repaint
}

void GObject::setBounds(double x, double y, double width, double height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    repaint();
}

void GObject::setBounds(const GRectangle& bounds) {
    setBounds(bounds.x, bounds.y, bounds.width, bounds.height);
}

void GObject::setCenterX(double x) {
    setCenterLocation(x, getCenterY());   // calls repaint
}

void GObject::setCenterY(double y) {
    setCenterLocation(getCenterX(), y);   // calls repaint
}

void GObject::setCenterLocation(double x, double y) {
    setLocation(x - getWidth() / 2, y - getHeight() / 2);   // calls repaint
}

void GObject::setCenterLocation(const GPoint& pt) {
    setCenterLocation(pt.x, pt.y);   // calls repaint
}

void GObject::setColor(int r, int g, int b) {
    _color = GColor::convertRGBToColor(r, g, b);
    _colorInt = GColor::convertRGBToRGB(r, g, b);
    repaint();
}

void GObject::setColor(int rgb) {
    _color = GColor::convertRGBToColor(rgb);
    _colorInt = rgb;
    repaint();
}

void GObject::setColor(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        _color = color;
        _colorInt = GColor::convertColorToRGB(color);
        repaint();
    } else {
        setColor(GColor::convertColorToRGB(color));
    }
}

void GObject::setFillColor(int r, int g, int b) {
    _fillColor = GColor::convertRGBToColor(r, g, b);
    _fillColorInt = GColor::convertRGBToRGB(r, g, b);
    repaint();
}

void GObject::setFillColor(int rgb) {
    _fillColor = GColor::convertRGBToColor(rgb);
    _fillColorInt = rgb;
    repaint();
}

void GObject::setFillColor(const std::string& color) {
    _fillColor = color;
    _fillColorInt = GColor::convertColorToRGB(color);
    if (_fillColor == "") {
        _fillFlag = false;
    } else {
        if (!GColor::hasAlpha(color)) {
            _fillColor = GColor::convertRGBToColor(_fillColorInt);
        }
        _fillFlag = true;
    }
    repaint();
}

void GObject::setFilled(bool flag) {
    _fillFlag = flag;
    repaint();
}

void GObject::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GObject::setFont(const std::string& font) {
    _font = font;
    repaint();
}

void GObject::setForeground(int r, int g, int b) {
    setColor(r, g, b);
}

void GObject::setForeground(int rgb) {
    setColor(rgb);
}

void GObject::setForeground(const std::string& color) {
    setColor(color);
}

void GObject::setHeight(double height) {
    setSize(getWidth(), height);
}

void GObject::setLineStyle(GObject::LineStyle lineStyle) {
    _lineStyle = lineStyle;
    repaint();
}

void GObject::setLineWidth(double lineWidth) {
    _lineWidth = lineWidth;
    repaint();
}

void GObject::setLocation(double x, double y) {
    _x = x;
    _y = y;
    repaint();
}

void GObject::setLocation(const GPoint& pt) {
    setLocation(pt.x, pt.y);   // calls repaint
}

void GObject::setOpacity(double opacity) {
    require::inRange(opacity, 0.0, 1.0, "GObject::setOpacity");
    _opacity = opacity;
    repaint();
}

void GObject::setSize(double width, double height) {
    if (isTransformed()) {
        error("GObject::setSize: Object has been transformed");
    }
    _width = width;
    _height = height;
    repaint();
}

void GObject::setSize(const GDimension& size) {
    setSize(size.width, size.height);   // calls repaint
}

void GObject::setVisible(bool flag) {
    _visible = flag;
    repaint();
}

void GObject::setWidth(double width) {
    setSize(width, getHeight());
}

void GObject::setX(double x) {
    setLocation(x, getY());   // calls repaint
}

void GObject::setY(double y) {
    setLocation(getX(), y);   // calls repaint
}

std::string GObject::toString() const {
    std::string extra = toStringExtra();
    return getType()
            + "("
            + "x=" + std::to_string(_x)
            + ",y=" + std::to_string(_y)
            + ",w=" + std::to_string(_width)
            + ",h=" + std::to_string(_height)
            + (_lineWidth <= 1 ? "" : (",lineWidth=" + std::to_string(_lineWidth)))
            + (_color.empty() ? "" : (",color=" + _color))
            + (_fillColor.empty() ? "" : (",fillColor=" + _fillColor))
            + (_font.empty() ? "" : (",font=" + _font))
            + (_visible ? "" : (",visible=" + boolToString(_visible)))
            + (extra.empty() ? "" : ("," + extra))
            + ")";
}

Qt::PenStyle GObject::toQtPenStyle(GObject::LineStyle lineStyle) {
    switch (lineStyle) {
    case GObject::LINE_DASH:
        return Qt::DashLine;
    case GObject::LINE_DASH_DOT:
        return Qt::DashDotLine;
    case GObject::LINE_DASH_DOT_DOT:
        return Qt::DashDotDotLine;
    case GObject::LINE_DOT:
        return Qt::DotLine;
    case GObject::LINE_NONE:
        return Qt::NoPen;
    case GObject::LINE_SOLID:
    default:
        return Qt::SolidLine;
    }
}

std::string GObject::toStringExtra() const {
    return "";
}


GArc::GArc(double width, double height, double start, double sweep)
        : GObject(/* x */ 0, /* y */ 0, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

GArc::GArc(double x, double y, double width, double height, double start, double sweep)
        : GObject(x, y, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

bool GArc::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (getX() + rx);
    double dy = y - (getY() + ry);
    double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    if (isFilled()) {
        if (r > 1.0) {
            return false;
        }
    } else {
        double t = STATIC_VARIABLE(ARC_TOLERANCE) / ((rx + ry) / 2);
        if (std::fabs(1.0 - r) > t) {
            return false;
        }
    }

    // JL BUGFIX: must scale by ry, rx.
    return containsAngle(atan2(-dy/ry, dx/rx) * 180 / PI);
}

bool GArc::containsAngle(double theta) const {
    double start = std::min(_start, _start + _sweep);
    double sweep = std::abs(_sweep);
    if (sweep >= 360) {
        return true;
    }
    theta = (theta < 0) ? 360 - fmod(-theta, 360) : fmod(theta, 360);
    start = (start < 0) ? 360 - fmod(-start, 360) : fmod(start, 360);
    if (start + sweep > 360) {
        return theta >= start || theta <= start + sweep - 360;
    } else {
        return theta >= start && theta <= start + sweep;
    }
}

void GArc::draw(QPainter* painter) {
    // for some reason, Qt's arc-drawing functionality asks for angles in
    // 1/16ths of a degree. okay sure whatever
    static const int QT_ANGLE_SCALE_FACTOR = 16;
    initializeBrushAndPen(painter);
    painter->drawChord((int) getX(), (int) getY(),
                       (int) getWidth(), (int) getHeight(),
                       (int) (_start * QT_ANGLE_SCALE_FACTOR),
                       (int) (_sweep * QT_ANGLE_SCALE_FACTOR));
}

GPoint GArc::getArcPoint(double theta) const {
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    double cx = getX() + rx;
    double cy = getY() + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

GRectangle GArc::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    double cx = getX() + rx;
    double cy = getY() + ry;
    double startRadians = _start * PI / 180;
    double sweepRadians = _sweep * PI / 180;
    double p1x = cx + cos(startRadians) * rx;
    double p1y = cy - sin(startRadians) * ry;
    double p2x = cx + cos(startRadians + sweepRadians) * rx;
    double p2y = cy - sin(startRadians + sweepRadians) * ry;
    double xMin = std::min(p1x, p2x);
    double xMax = std::max(p1x, p2x);
    double yMin = std::min(p1y, p2y);
    double yMax = std::max(p1y, p2y);
    if (containsAngle(0)) xMax = cx + rx;
    if (containsAngle(90)) yMin = cy - ry;
    if (containsAngle(180)) xMin = cx - rx;
    if (containsAngle(270)) yMax = cy + ry;
    if (isFilled()) {
        xMin = std::min(xMin, cx);
        yMin = std::min(yMin, cy);
        xMax = std::max(xMax, cx);
        yMax = std::max(yMax, cy);
    }
    return GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

GPoint GArc::getEndPoint() const {
    return getArcPoint(_start + _sweep);
}

GRectangle GArc::getFrameRectangle() const {
    return getBounds();
}

double GArc::getStartAngle() const {
    return _start;
}

GPoint GArc::getStartPoint() const {
    return getArcPoint(_start);
}

double GArc::getSweepAngle() const {
    return _sweep;
}

std::string GArc::getType() const {
    return "GArc";
}

void GArc::setFrameRectangle(double x, double y, double width, double height) {
    setBounds(x, y, width, height);   // calls repaint
}

void GArc::setFrameRectangle(const GRectangle& rect) {
    setFrameRectangle(rect.x, rect.y, rect.width, rect.height);   // calls repaint
}

void GArc::setStartAngle(double start) {
    _start = start;
    repaint();
}

void GArc::setSweepAngle(double sweep) {
    _sweep = sweep;
    repaint();
}

std::string GArc::toStringExtra() const {
    std::ostringstream oss;
    oss << " start=" << _start << " sweep=" << _sweep;
    return oss.str();
}


GCompound::GCompound()
        : _autoRepaint(true) {
    // empty
}

void GCompound::add(GObject* gobj) {
    require::nonNull(gobj, "GCompound::add");
    for (int i = _contents.size() - 1; i >= 0; i--) {   // avoid duplicates
        if (_contents[i] == gobj) {
            return;
        }
    }
    _contents.add(gobj);
    gobj->_parent = this;
    if (gobj->isTransformed()) {
        conditionalRepaint();
    } else {
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCompound::add(GObject* gobj, double x, double y) {
    require::nonNull(gobj, "GCompound::add");
    gobj->setLocation(x, y);
    add(gobj);   // calls conditionalRepaint
}

void GCompound::add(GObject& gobj) {
    add(&gobj);
}

void GCompound::add(GObject& gobj, double x, double y) {
    add(&gobj, x, y);
}

void GCompound::clear() {
    removeAll();   // calls conditionalRepaint
}

void GCompound::conditionalRepaint() {
    if (_autoRepaint) {
        repaint();
    }
}

void GCompound::conditionalRepaintRegion(int x, int y, int width, int height) {
    if (_autoRepaint) {
        repaintRegion(x, y, width, height);
    }
}

void GCompound::conditionalRepaintRegion(const GRectangle& bounds) {
    if (_autoRepaint) {
        repaintRegion(bounds);
    }
}

bool GCompound::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    for (int i = 0, sz = _contents.size(); i < sz; i++) {
        if (_contents[i]->contains(x, y)) {
            return true;
        }
    }
    return false;
}

void GCompound::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    // TODO: uncomment this? need settings to apply to every shape
    // initializeBrushAndPen(painter);   //
    for (GObject* obj : _contents) {
        if (obj->isVisible()) {
            obj->draw(painter);
        }
    }
}

int GCompound::findGObject(GObject* gobj) const {
    int n = _contents.size();
    for (int i = 0; i < n; i++) {
        if (_contents.get(i) == gobj) {
            return i;
        }
    }
    return -1;
}

GRectangle GCompound::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double xMin = +1E20;
    double yMin = +1E20;
    double xMax = -1E20;
    double yMax = -1E20;
    for (int i = 0; i < _contents.size(); i++) {
        GRectangle bounds = _contents.get(i)->getBounds();
        xMin = std::min(xMin, bounds.x);
        yMin = std::min(yMin, bounds.y);
        xMax = std::max(xMax, bounds.x);
        yMax = std::max(yMax, bounds.y);   // JL BUGFIX 2016/10/11
    }
    // JL BUGFIX: shifted anchor point
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

GObject* GCompound::getElement(int index) const {
    return _contents.get(index);
}

GObject* GCompound::getElementAt(double x, double y) const {
    for (GObject* gobj : _contents) {
        if (gobj && gobj->contains(x, y)) {
            return gobj;
        }
    }
    return nullptr;
}

int GCompound::getElementCount() const {
    return _contents.size();
}

std::string GCompound::getType() const {
    return "GCompound";
}

QWidget* GCompound::getWidget() const {
    return _widget;
}

bool GCompound::isAutoRepaint() const {
    return _autoRepaint;
}

bool GCompound::isEmpty() const {
    return _contents.size() == 0;
}

void GCompound::remove(GObject* gobj) {
    require::nonNull(gobj, "GCompound::remove");
    int index = findGObject(gobj);
    if (index != -1) {
        removeAt(index);   // calls conditionalRepaint
    }
}

void GCompound::remove(GObject& gobj) {
    remove(&gobj);
}

void GCompound::removeAll() {
    bool wasEmpty = _contents.isEmpty();
    Vector<GObject*> contentsCopy = _contents;
    _contents.clear();
    for (GObject* obj : contentsCopy) {
        obj->_parent = nullptr;
        // TODO: delete obj;
    }
    if (!wasEmpty) {
        conditionalRepaint();
    }
}

void GCompound::removeAt(int index) {
    GObject* gobj = _contents[index];
    _contents.remove(index);
    gobj->_parent = nullptr;
    if (gobj->isTransformed()) {
        conditionalRepaint();
    } else {
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCompound::repaint() {
    if (!_widget) {
        return;
    }

    // actual repainting must be done in the Qt GUI thread
    if (GThread::iAmRunningOnTheQtGuiThread()) {
        _widget->repaint();   // TODO: change to update()?
    } else {
        GThread::runOnQtGuiThread([this]() {
            _widget->repaint();   // TODO: change to update()?
        });
    }
}

void GCompound::repaintRegion(int x, int y, int width, int height) {
    if (!_widget) {
        return;
    }

    // actual repainting must be done in the Qt GUI thread
    if (GThread::iAmRunningOnTheQtGuiThread()) {
        _widget->repaint(x, y, width, height);
    } else {
        GThread::runOnQtGuiThread([this, x, y, width, height]() {
            _widget->repaint(x, y, width, height);
        });
    }
}

void GCompound::repaintRegion(const GRectangle& bounds) {
    repaintRegion((int) bounds.x, (int) bounds.y,
                  (int) bounds.width, (int) bounds.height);
}

void GCompound::sendBackward(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendBackward");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        _contents.remove(index);
        _contents.insert(index - 1, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendBackward(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendForward(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendForward");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != _contents.size() - 1) {
        _contents.remove(index);
        _contents.insert(index + 1, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendForward(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendToBack(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendToBack");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        _contents.remove(index);
        _contents.insert(0, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendToBack(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendToFront(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendToFront");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != _contents.size() - 1) {
        _contents.remove(index);
        _contents.add(gobj);
        conditionalRepaint();
    }
}

void GCompound::setAutoRepaint(bool autoRepaint) {
    _autoRepaint = autoRepaint;
}

void GCompound::setWidget(QWidget* widget) {
    _widget = widget;
}

std::string GCompound::toString() const {
    return "GCompound(...)";
}


GImage::GImage(const std::string& filename, double x, double y)
        : GObject(x, y),
          _filename(filename),
          _qimage(nullptr) {
    if (!load(filename)) {
        error("GImage::constructor: unable to load image from: \"" + filename + "\"");
    }
}

GImage::GImage(std::istream& source, double x, double y)
        : GObject(x, y),
          _filename("std::istream source"),
          _qimage(nullptr) {
    if (!loadFromStream(source)) {
        error("GImage::constructor: unable to load image from stream input");
    }
}

GImage::GImage(double width, double height) {
    require::nonNegative2D(width, height, "GImage::constructor", "width", "height");
    _width = width;
    _height = height;
    GThread::runOnQtGuiThread([this]() {
        _qimage = new QImage(static_cast<int>(_width), static_cast<int>(_height), QImage::Format_ARGB32);
    });
}

GImage::GImage(QImage* qimage) {
    require::nonNull(qimage, "GImage::constructor");
    _qimage = qimage;
    _width = _qimage->width();
    _height = _qimage->height();
}

GImage::~GImage() {
    // TODO: delete _image;
    _qimage = nullptr;
}

bool GImage::load(const std::string& filename) {
    if (filename.empty() || !fileExists(filename)) {
        return false;
    }
    bool hasError = false;
    GThread::runOnQtGuiThread([this, filename, &hasError]() {
        _qimage = new QImage;
        if (_qimage->load(QString::fromStdString(_filename))) {
            _width = _qimage->width();
            _height = _qimage->height();
        } else {
            hasError = true;
        }
    });
    return !hasError;   // *** BUGFIX thanks to Tyler Conklin
}

bool GImage::loadFromStream(std::istream& input) {
    // transfer bytes to a string through std::stringstream
    std::ostringstream byteStream;
    byteStream << input.rdbuf();
    std::string bytes = byteStream.str();

    // load image
    bool hasError = false;
    GThread::runOnQtGuiThread([&, this]() {
        _qimage = new QImage;
        if (_qimage->loadFromData(reinterpret_cast<const uchar *>(bytes.data()), bytes.size())) {
            _width = _qimage->width();
            _height = _qimage->height();
        } else {
            hasError = true;
        }
    });

    return !hasError;
}

void GImage::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    double myX = getX();
    double myY = getY();
    double myWidth = getWidth();
    double myHeight = getHeight();
    double imgWidth = _qimage->width();
    double imgHeight = _qimage->height();
    painter->setOpacity(_opacity);
    painter->setTransform(_transform, /* combine */ false);
    if (floatingPointEqual(myWidth, imgWidth) && floatingPointEqual(myHeight, imgHeight)) {
        // draw unscaled
        painter->drawImage((int) myX, (int) myY, *_qimage);
    } else {
        // draw scaled
        QRectF rect(myX, myY, myWidth, myHeight);
        painter->drawImage(rect, *_qimage);
    }
}

std::string GImage::getFileName() const {
    return _filename;
}

int GImage::getPixel(int x, int y) const {
    require::inRange2D(x, y, (int) getWidth() - 1, (int) getHeight() - 1, "GImage::getPixel", "x", "y");
    return (int) _qimage->pixel(x, y);
}

QImage* GImage::getQImage() const {
    return _qimage;
}

std::string GImage::getType() const {
    return "GImage";
}

void GImage::setPixel(int x, int y, int rgb) {
    _qimage->setPixel(x, y, rgb);
}

std::string GImage::toStringExtra() const {
    return "filename=\"" + _filename + "\"";
}


GLine::GLine(double x0, double y0, double x1, double y1, GObject::LineStyle lineStyle)
        : GObject(x0, y0),
          _dx(x1 - x0),
          _dy(y1 - y0) {
    setPoints(x0, y0, x1, y1);   // checks if point swap is needed
    setLineStyle(lineStyle);
}

GLine::GLine(const GPoint& p0, const GPoint& p1)
        : GObject(p0.x, p0.y),
          _dx(p1.x - p0.x),
          _dy(p1.y - p0.y) {
    // empty
}

bool GLine::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double x0 = getStartX();
    double y0 = getStartY();
    double x1 = getEndX();
    double y1 = getEndY();
    double tSquared = STATIC_VARIABLE(LINE_TOLERANCE) * STATIC_VARIABLE(LINE_TOLERANCE);
    if (dsq(x, y, x0, y0) < tSquared) {
        return true;
    }
    if (dsq(x, y, x1, y1) < tSquared) {
        return true;
    }
    if (x < std::min(x0, x1) - STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (x > std::max(x0, x1) + STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (y < std::min(y0, y1) - STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (y > std::max(y0, y1) + STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (floatingPointEqual(x0 - x1, 0) && floatingPointEqual(y0 - y1, 0)) {
        return false;
    }
    double u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0))
            / dsq(x0, y0, x1, y1);
    return dsq(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tSquared;
}

void GLine::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawLine(static_cast<int>(getStartX()),
                      static_cast<int>(getStartY()),
                      static_cast<int>(getEndX()),
                      static_cast<int>(getEndY()));
}

GRectangle GLine::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double minX = std::min(getStartX(), getEndX());
    double minY = std::min(getStartY(), getEndY());
    return GRectangle(minX, minY, getWidth(), getHeight());
}

GPoint GLine::getEndPoint() const {
    return GPoint(getX() + _dx, getY() + _dy);
}

double GLine::getEndX() const {
    return getX() + _dx;
}

double GLine::getEndY() const {
    return getY() + _dy;
}

double GLine::getHeight() const {
    return std::fabs(_dy);
}

GPoint GLine::getStartPoint() const {
    return getLocation();
}

double GLine::getStartX() const {
    return getX();
}

double GLine::getStartY() const {
    return getY();
}

std::string GLine::getType() const {
    return "GLine";
}

double GLine::getWidth() const {
    return std::fabs(_dx);
}

void GLine::setEndPoint(double x1, double y1) {
    setPoints(getStartX(), getStartY(), x1, y1);
}

void GLine::setEndPoint(const GPoint& p) {
    setEndPoint(p.x, p.y);
}

void GLine::setPoints(double x0, double y0, double x1, double y1) {
    _x = x0;
    _y = y0;
    _dx = x1 - x0;
    _dy = y1 - y0;
    repaint();
}

void GLine::setPoints(const GPoint& p0, const GPoint& p1) {
    setPoints(p0.x, p0.y, p1.x, p1.y);
}

void GLine::setStartPoint(double x0, double y0) {
    setPoints(x0, y0, getEndX(), getEndY());
}

void GLine::setStartPoint(const GPoint& p) {
    setStartPoint(p.x, p.y);
}

std::string GLine::toStringExtra() const {
    std::ostringstream oss;
    oss << "x2=" << getEndX() << " y2=" << getEndY();
    return oss.str();
}


GOval::GOval(double x, double y, double width, double height)
        : GObject(x, y, width, height) {
    // empty
}

bool GOval::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (getX() + rx);
    double dy = y - (getY() + ry);
    return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void GOval::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawEllipse((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
}

std::string GOval::getType() const {
    return "GOval";
}


GPolygon::GPolygon() {
    // empty
}

GPolygon::GPolygon(std::initializer_list<double> coords) {
    addVertexes(coords);
}

GPolygon::GPolygon(std::initializer_list<GPoint> points) {
    addVertexes(points);
}

void GPolygon::addEdge(double dx, double dy) {
    addVertex(_cx + dx, _cy + dy);
}

void GPolygon::addEdge(const GPoint& pt) {
    addEdge(pt.x, pt.y);
}

void GPolygon::addEdges(std::initializer_list<double> coords) {
    int i = 0;
    double dx = 0;
    double dy = 0;
    for (double d : coords) {
        if (i % 2 == 0) {
            dx = d;
        } else {
            dy = d;
            addEdge(dx, dy);
        }
        i++;
    }
}

void GPolygon::addEdges(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addEdge(pt);
    }
}

void GPolygon::addPolarEdge(double r, double theta) {
    addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

void GPolygon::addVertex(double x, double y) {
    _cx = x;
    _cy = y;
    _vertices.append(QPointF(_cx, _cy));
    repaint();
}

void GPolygon::addVertex(const GPoint& pt) {
    addVertex(pt.x, pt.y);
}

void GPolygon::addVertexes(std::initializer_list<double> coords) {
    int i = 0;
    double x = 0;
    double y = 0;
    for (double d : coords) {
        if (i % 2 == 0) {
            x = d;
        } else {
            y = d;
            addVertex(x, y);
        }
        i++;
    }
}

void GPolygon::addVertexes(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addVertex(pt);
    }
}

void GPolygon::clear() {
    _vertices.clear();
    repaint();
}

bool GPolygon::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    int crossings = 0;
    int n = _vertices.size();
    if (n < 2) {
        return false;
    }
    if (_vertices[0] == _vertices[n - 1]) {
        n--;
    }
    double x0 = _vertices[0].x();
    double y0 = _vertices[0].y();
    for (int i = 1; i <= n; i++) {
        double x1 = _vertices[i % n].x();
        double y1 = _vertices[i % n].y();
        if ((y0 > y) != (y1 > y) && x - x0 < (x1 - x0) * (y - y0) / (y1 - y0)) {
            crossings++;
        }
        x0 = x1;
        y0 = y1;
    }
    return (crossings % 2 == 1);
}

void GPolygon::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawPolygon(QPolygonF(_vertices));
}

GRectangle GPolygon::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double xMin = 0;
    double yMin = 0;
    double xMax = 0;
    double yMax = 0;
    for (int i = 0; i < _vertices.size(); i++) {
        double x = _vertices[i].x();
        double y = _vertices[i].y();
        if (i == 0 || x < xMin) xMin = x;
        if (i == 0 || y < yMin) yMin = y;
        if (i == 0 || x > xMax) xMax = x;
        if (i == 0 || y > yMax) yMax = y;
    }
    // JL BUGFIX: add getX, getY
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

double GPolygon::getHeight() const {
    return getBounds().height;
}

std::string GPolygon::getType() const {
    return "GPolygon";
}

GPoint GPolygon::getVertex(int i) const {
    return GPoint(_vertices[i].x(), _vertices[i].y());
}

int GPolygon::getVertexCount() const {
    return _vertices.size();
}

Vector<GPoint> GPolygon::getVertices() const {
    Vector<GPoint> vec;
    for (const QPointF& point : _vertices) {
        vec.add(GPoint(point.x(), point.y()));
    }
    return vec;
}

double GPolygon::getWidth() const {
    return getBounds().width; // BUGFIX JDZ
}

void GPolygon::setVertex(int i, GPoint point) {
    _vertices[i].setX(point.x);
    _vertices[i].setY(point.y);
    repaint();
}

std::string GPolygon::toStringExtra() const {
    std::ostringstream oss;
    oss << "vertices=" << _vertices.size();
    return oss.str();
}


GRect::GRect(double x, double y, double width, double height)
        : GObject(x, y, width, height) {
    // empty
}

void GRect::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawRect((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
    painter->setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
}

std::string GRect::getType() const {
    return "GRect";
}


/*
 * Implementation notes: GRoundRect class
 * ---------------------------------------
 * Most of the GRoundRect class is inherited from the GRect class.
 */

GRoundRect::GRoundRect(double width, double height, double corner)
        : GRect(/* x */ 0, /* y */ 0, width, height),
          _corner(corner) {
    require::nonNegative(corner, "GRoundRect::constructor", "corner");
}

GRoundRect::GRoundRect(double x, double y, double width, double height, double corner)
        : GRect(x, y, width, height),
          _corner(corner) {
    require::nonNegative(corner, "GRoundRect::constructor", "corner");
}

bool GRoundRect::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }

    // JL BUGFIX: The rest of this is code to return correct result in non-transformed case
    // (accounting for corners)
    if (!getBounds().contains(x, y)) {
        return false;
    }

    // If corner diameter is too big, the largest sensible value is used by Java back end.
    double a = std::min(_corner, getWidth()) / 2;
    double b = std::min(_corner, getHeight()) / 2;

    // Get distances from nearest edges of bounding rectangle
    double dx = std::min(std::abs(x - getX()), std::abs(x - getRightX()));
    double dy = std::min(std::abs(y - getY()), std::abs(y - getBottomY()));

    if (dx > a || dy > b) {
        return true;   // in "central cross" of rounded rect
    }

    return (dx - a) * (dx - a) / (a * a) + (dy - b) * (dy - b) / (b * b) <= 1;
}

void GRoundRect::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    // QPainter takes corner as radius, GRoundRect stores as diameter, thus convert
    painter->drawRoundedRect((int) getX(), (int) getY(),
                           (int) getWidth(), (int) getHeight(),
                           (int) (_corner/2), (int) (_corner/2));
}

double GRoundRect::getCorner() const {
    return _corner;
}

std::string GRoundRect::getType() const {
    return "GRoundRect";
}

void GRoundRect::setCorner(double corner) {
    require::nonNegative(corner, "GRoundRect::setCorner", "corner");
    _corner = corner;
    repaint();
}

std::string GRoundRect::toStringExtra() const {
    return "corner=" + std::to_string(_corner);
}


GText::GText(const std::string& str, double x, double y)
        : GObject(x, y),
          _text(str) {
    _font = DEFAULT_FONT;
    updateSize();
}

void GText::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawText((int) getX(), (int) getY(), QString::fromStdString(_text));
}

GRectangle GText::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(getX(), getY() - getFontAscent(), getWidth(), getHeight());
}

std::string GText::getFont() const {
    return _font;
}

double GText::getFontAscent() const {
    QFontMetrics metrics(GFont::toQFont(_font));
    return metrics.ascent();
}

double GText::getFontDescent() const {
    QFontMetrics metrics(GFont::toQFont(_font));
    return metrics.descent();
}

std::string GText::getLabel() const {
    return _text;
}

std::string GText::getText() const {
    return _text;
}

std::string GText::getType() const {
    return "GText";
}

void GText::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GText::setFont(const std::string& font) {
    _font = font;
    updateSize();
    repaint();
}

void GText::setLabel(const std::string& str) {
    _text = str;
    updateSize();
    repaint();
}

void GText::setText(const std::string& str) {
    setLabel(str);
}

std::string GText::toStringExtra() const {
    return "text=\"" + _text + "\"";
}

void GText::updateSize() {
    QFontMetrics metrics(GFont::toQFont(_font));
    _width = metrics.horizontalAdvance(QString::fromStdString(_text));
    _height = metrics.height();
}

std::ostream& operator <<(std::ostream& out, const GObject& obj) {
    return out << obj.toString();
}

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
