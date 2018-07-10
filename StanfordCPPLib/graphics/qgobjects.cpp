/*
 * File: qgobjects.cpp
 * -------------------
 * This file implements the qgobjects.h interface.
 *
 * @version 2018/06/30
 * - initial version
 */

#include "qgobjects.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <QFont>
#include <QPointF>
#include <QPolygon>
#include <QVector>
#include <sstream>
#include <string>
#include "filelib.h"
#include "gmath.h"
#include "qgcolor.h"
#include "qgfont.h"
#include "private/static.h"


// static constants
STATIC_CONST_VARIABLE_DECLARE(double, LINE_TOLERANCE, 1.5)
STATIC_CONST_VARIABLE_DECLARE(double, ARC_TOLERANCE, 2.5)
STATIC_CONST_VARIABLE_DECLARE(double, DEFAULT_CORNER, 10)
STATIC_CONST_VARIABLE_DECLARE(std::string, DEFAULT_GLABEL_FONT, "Dialog-13")

STATIC_VARIABLE_DECLARE_BLANK(QBrush, DEFAULT_BRUSH)
STATIC_VARIABLE_DECLARE_BLANK(QFont, DEFAULT_QFONT)
STATIC_VARIABLE_DECLARE(bool, DEFAULT_QFONT_SET, false)

/*
 * Returns the square of the distance between two points.
 * Used when checking to see if a line touches a given point.
 */
static double dsq(double x0, double y0, double x1, double y1);


/* QGObject class */

bool QGObject::_sAntiAliasing = true;

QGObject::QGObject(double x, double y, double width, double height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height),
          _lineWidth(1),
          _color(""),
          _colorInt(0),
          _fillColor(""),
          _fillColorInt(0),
          _font(""),
          _fillFlag(false),
          _visible(true),
          _transformed(false),
          _parent(nullptr) {
    // empty
}

QGObject::~QGObject() {
    // empty
}

bool QGObject::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        return getBounds().contains(x, y);
    } else {
        return getBounds().contains(x, y);
    }
}

bool QGObject::contains(const GPoint& pt) const {
    return contains(pt.getX(), pt.getY());
}

GPoint QGObject::getBottomRightLocation() const {
    return GPoint(getRightX(), getBottomY());
}

double QGObject::getBottomY() const {
    return getY() + getHeight();
}

GRectangle QGObject::getBounds() const {
    if (_transformed) {
        // TODO
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    } else {
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    }
}

GPoint QGObject::getCenterLocation() const {
    return GPoint(getCenterX(), getCenterY());
}

double QGObject::getCenterX() const {
    return getX() + getWidth() / 2;
}

double QGObject::getCenterY() const {
    return getY() + getHeight() / 2;
}

std::string QGObject::getColor() const {
    return _color;
}

std::string QGObject::getFillColor() const {
    return _fillColor;
}

double QGObject::getHeight() const {
    return _height;
}

double QGObject::getLineWidth() const {
    return _lineWidth;
}

GPoint QGObject::getLocation() const {
    return GPoint(getX(), getY());
}

QGCompound* QGObject::getParent() const {
    return _parent;
}

double QGObject::getRightX() const {
    return getX() + getWidth();
}

GDimension QGObject::getSize() const {
    GRectangle bounds = getBounds();
    return GDimension(bounds.getWidth(), bounds.getHeight());
}

double QGObject::getWidth() const {
    return _width;
}

double QGObject::getX() const {
    return _x;
}

double QGObject::getY() const {
    return _y;
}

void QGObject::initializeBrushAndPen(QPainter* painter) {
    if (!painter) {
        return;
    }
    _pen.setColor(QColor(_colorInt));
    _pen.setWidth((int) _lineWidth);
    painter->setPen(_pen);

    // font
    if (!STATIC_VARIABLE(DEFAULT_QFONT_SET)) {
        STATIC_VARIABLE(DEFAULT_QFONT) = painter->font();
        STATIC_VARIABLE(DEFAULT_BRUSH).setColor(QColor(0x00ffffff));
    }
    if (_font.empty()) {
        painter->setFont(STATIC_VARIABLE(DEFAULT_QFONT));
    } else {
        painter->setFont(QGFont::toQFont(_font));
    }


    // fill color
    if (_fillFlag) {
        _brush.setStyle(Qt::SolidPattern);
        _brush.setColor(QColor(_fillColorInt));
        painter->setBrush(_brush);
    } else {
        painter->setBrush(STATIC_VARIABLE(DEFAULT_BRUSH));
    }

    // transform
    painter->setTransform(_transform, /* combine */ false);
}

bool QGObject::isAntiAliasing() {
    return _sAntiAliasing;
}

bool QGObject::isFilled() const {
    return _fillFlag;
}

bool QGObject::isVisible() const {
    return _visible;
}

void QGObject::move(double dx, double dy) {
    setLocation(getX() + dx, getY() + dy);   // calls repaint
}

void QGObject::repaint() {
    QGCompound* parent = getParent();
    while (parent && parent->getParent()) {
        parent = parent->getParent();
    }
    if (parent) {
        parent->conditionalRepaint();
    }
}

void QGObject::resetTransform() {
    _transform = QTransform();
    _transformed = false;
    repaint();
}

void QGObject::rotate(double theta) {
    _transformed = true;
    _transform = _transform.rotate(theta);
    repaint();
}

void QGObject::scale(double sf) {
    scale(sf, sf);   // calls repaint
}

void QGObject::scale(double sx, double sy) {
    _transformed = true;
    _transform = _transform.scale(sx, sy);
    repaint();
}

void QGObject::sendBackward() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendBackward(this);
    }
}

void QGObject::sendForward() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendForward(this);
    }
}

void QGObject::sendToBack() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendToBack(this);
    }
}

void QGObject::sendToFront() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendToFront(this);
    }
}

void QGObject::setAntiAliasing(bool value) {
    _sAntiAliasing = value;
}

void QGObject::setBottomY(double y) {
    setBottomRightLocation(getRightX(), y);   // calls repaint
}

void QGObject::setRightX(double x) {
    setBottomRightLocation(x, getBottomY());   // calls repaint
}

void QGObject::setBottomRightLocation(double x, double y) {
    setLocation(x - getWidth(), y - getHeight());   // calls repaint
}

void QGObject::setBottomRightLocation(const GPoint& pt) {
    setBottomRightLocation(pt.getX(), pt.getY());   // calls repaint
}

void QGObject::setBounds(double x, double y, double width, double height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    repaint();
}

void QGObject::setBounds(const GRectangle& bounds) {
    setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void QGObject::setCenterX(double x) {
    setCenterLocation(x, getCenterY());   // calls repaint
}

void QGObject::setCenterY(double y) {
    setCenterLocation(getCenterX(), y);   // calls repaint
}

void QGObject::setCenterLocation(double x, double y) {
    setLocation(x - getWidth() / 2, y - getHeight() / 2);   // calls repaint
}

void QGObject::setCenterLocation(const GPoint& pt) {
    setCenterLocation(pt.getX(), pt.getY());   // calls repaint
}

void QGObject::setColor(int r, int g, int b) {
    _color = QGColor::convertRGBToColor(r, g, b);
    _colorInt = QGColor::convertRGBToRGB(r, g, b);
    repaint();
}

void QGObject::setColor(int rgb) {
    _color = QGColor::convertRGBToColor(rgb);
    _colorInt = rgb;
    repaint();
}

void QGObject::setColor(const std::string& color) {
    setColor(QGColor::convertColorToRGB(color));
}

void QGObject::setFillColor(int r, int g, int b) {
    _fillColor = QGColor::convertRGBToColor(r, g, b);
    _fillColorInt = QGColor::convertRGBToRGB(r, g, b);
    repaint();
}

void QGObject::setFillColor(int rgb) {
    _fillColor = QGColor::convertRGBToColor(rgb);
    _fillColorInt = rgb;
    repaint();
}

void QGObject::setFillColor(const std::string& color) {
    _fillColor = color;
    _fillColorInt = QGColor::convertColorToRGB(color);
    if (_fillColor == "") {
        _fillFlag = false;
    } else {
        _fillColor = QGColor::convertRGBToColor(QGColor::convertColorToRGB(color));
        _fillFlag = true;
    }
    repaint();
}

void QGObject::setFilled(bool flag) {
    _fillFlag = flag;
    repaint();
}

void QGObject::setFont(const std::string& font) {
    _font = font;
    repaint();
}

void QGObject::setLineWidth(double lineWidth) {
    _lineWidth = lineWidth;
    repaint();
}

void QGObject::setLocation(double x, double y) {
    _x = x;
    _y = y;
    repaint();
}

void QGObject::setLocation(const GPoint& pt) {
    setLocation(pt.getX(), pt.getY());   // calls repaint
}

void QGObject::setSize(double width, double height) {
    if (_transformed) {
        error("QGObject::setSize: Object has been transformed");
    }
    _width = width;
    _height = height;
    repaint();
}

void QGObject::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());   // calls repaint
}

void QGObject::setVisible(bool flag) {
    _visible = flag;
    repaint();
}

void QGObject::setX(double x) {
    setLocation(x, getY());   // calls repaint
}

void QGObject::setY(double y) {
    setLocation(getX(), y);   // calls repaint
}

std::string QGObject::toString() const {
    std::string extra = toStringExtra();
    return getType()
            + "("
            + "x=" + doubleToString(_x)
            + ",y=" + doubleToString(_y)
            + ",w=" + doubleToString(_width)
            + ",h=" + doubleToString(_height)
            + (_lineWidth <= 1 ? "" : (",lineWidth=" + doubleToString(_lineWidth)))
            + (_color.empty() ? "" : (",color=" + _color))
            + (_fillColor.empty() ? "" : (",fillColor=" + _fillColor))
            + (_font.empty() ? "" : (",font=" + _font))
            + (_visible ? "" : (",visible=" + boolToString(_visible)))
            + (extra.empty() ? "" : ("," + extra))
            + ")";
}

std::string QGObject::toStringExtra() const {
    return "";
}


QGArc::QGArc(double width, double height, double start, double sweep)
        : QGObject(/* x */ 0, /* y */ 0, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

QGArc::QGArc(double x, double y, double width, double height, double start, double sweep)
        : QGObject(x, y, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

bool QGArc::contains(double x, double y) const {
    if (_transformed) {
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

bool QGArc::containsAngle(double theta) const {
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

void QGArc::draw(QPainter* painter) {
    // for some reason, Qt's arc-drawing functionality asks for angles in
    // 1/16ths of a degree. okay sure whatever
    static const int QT_ANGLE_SCALE_FACTOR = 16;
    initializeBrushAndPen(painter);
    painter->drawChord((int) getX(), (int) getY(),
                       (int) getWidth(), (int) getHeight(),
                       (int) (_start * QT_ANGLE_SCALE_FACTOR),
                       (int) (_sweep * QT_ANGLE_SCALE_FACTOR));
}

GPoint QGArc::getArcPoint(double theta) const {
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    double cx = getX() + rx;
    double cy = getY() + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

GRectangle QGArc::getBounds() const {
    if (_transformed) {
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

GPoint QGArc::getEndPoint() const {
    return getArcPoint(_start + _sweep);
}

GRectangle QGArc::getFrameRectangle() const {
    return getBounds();
}

double QGArc::getStartAngle() const {
    return _start;
}

GPoint QGArc::getStartPoint() const {
    return getArcPoint(_start);
}

double QGArc::getSweepAngle() const {
    return _sweep;
}

std::string QGArc::getType() const {
    return "QGArc";
}

void QGArc::setFrameRectangle(double x, double y, double width, double height) {
    setBounds(x, y, width, height);   // calls repaint
}

void QGArc::setFrameRectangle(const GRectangle& rect) {
    setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());   // calls repaint
}

void QGArc::setStartAngle(double start) {
    _start = start;
    repaint();
}

void QGArc::setSweepAngle(double sweep) {
    _sweep = sweep;
    repaint();
}

std::string QGArc::toStringExtra() const {
    std::ostringstream oss;
    oss << " start=" << _start << " sweep=" << _sweep;
    return oss.str();
}


QGCompound::QGCompound()
        : _autoRepaint(true) {
    // empty
}

void QGCompound::add(QGObject* gobj) {
    _contents.add(gobj);
    gobj->_parent = this;
    conditionalRepaint();
}

void QGCompound::add(QGObject* gobj, double x, double y) {
    gobj->setLocation(x, y);
    add(gobj);   // calls conditionalRepaint
}

void QGCompound::add(QGObject& gobj) {
    add(&gobj);
}

void QGCompound::add(QGObject& gobj, double x, double y) {
    add(&gobj, x, y);
}

void QGCompound::clear() {
    removeAll();
}

void QGCompound::conditionalRepaint() {
    if (_autoRepaint) {
        repaint();
    }
}

bool QGCompound::contains(double x, double y) const {
    if (_transformed) {
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

void QGCompound::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    for (QGObject* obj : _contents) {
        obj->draw(painter);
    }
}

int QGCompound::findQGObject(QGObject* gobj) const {
    int n = _contents.size();
    for (int i = 0; i < n; i++) {
        if (_contents.get(i) == gobj) {
            return i;
        }
    }
    return -1;
}

GRectangle QGCompound::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double xMin = +1E20;
    double yMin = +1E20;
    double xMax = -1E20;
    double yMax = -1E20;
    for (int i = 0; i < _contents.size(); i++) {
        GRectangle bounds = _contents.get(i)->getBounds();
        xMin = std::min(xMin, bounds.getX());
        yMin = std::min(yMin, bounds.getY());
        xMax = std::max(xMax, bounds.getX());
        yMin = std::max(yMax, bounds.getY());   // JL BUGFIX 2016/10/11
    }
    // JL BUGFIX: shifted anchor point
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

QGObject* QGCompound::getElement(int index) const {
    return _contents.get(index);
}

QGObject* QGCompound::getElementAt(double x, double y) const {
    for (QGObject* gobj : _contents) {
        if (gobj && gobj->contains(x, y)) {
            return gobj;
        }
    }
    return nullptr;
}

int QGCompound::getElementCount() const {
    return _contents.size();
}

std::string QGCompound::getType() const {
    return "QGCompound";
}

QWidget* QGCompound::getWidget() const {
    return _widget;
}

bool QGCompound::isAutoRepaint() const {
    return _autoRepaint;
}

void QGCompound::remove(QGObject* gobj) {
    int index = findQGObject(gobj);
    if (index != -1) {
        removeAt(index);   // calls conditionalRepaint
    }
}

void QGCompound::remove(QGObject& gobj) {
    remove(&gobj);
}

void QGCompound::removeAll() {
    Vector<QGObject*> contentsCopy = _contents;
    _contents.clear();
    for (QGObject* obj : contentsCopy) {
        obj->_parent = nullptr;
        // TODO: delete obj;
    }
    conditionalRepaint();
}

void QGCompound::removeAt(int index) {
    QGObject* gobj = _contents[index];
    _contents.remove(index);
    // stanfordcpplib::getPlatform()->gobject_remove(gobj);
    gobj->_parent = nullptr;
    conditionalRepaint();
}

void QGCompound::repaint() {
    if (_widget) {
        _widget->repaint();
    }
}

void QGCompound::sendBackward(QGObject* gobj) {
    int index = findQGObject(gobj);
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

void QGCompound::sendForward(QGObject* gobj) {
    int index = findQGObject(gobj);
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

void QGCompound::sendToBack(QGObject* gobj) {
    int index = findQGObject(gobj);
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

void QGCompound::sendToFront(QGObject* gobj) {
    int index = findQGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != _contents.size() - 1) {
        _contents.remove(index);
        _contents.add(gobj);
        conditionalRepaint();
    }
}

void QGCompound::setAutoRepaint(bool autoRepaint) {
    _autoRepaint = autoRepaint;
}

void QGCompound::setWidget(QWidget* widget) {
    _widget = widget;
}

std::string QGCompound::toString() const {
    return "QGCompound(...)";
}


QGImage::QGImage(const std::string& filename, double x, double y)
        : QGObject(x, y),
          _filename(filename),
          _qimage(nullptr) {
    if (!_filename.empty()) {
        if (!fileExists(_filename)) {
            error("QGImage: file not found: \"" + filename + "\"");
        }
        // load image
        _qimage = new QImage;
        if (_qimage->load(QString::fromStdString(_filename))) {
            _width = _qimage->width();
            _height = _qimage->height();
        } else {
            error("QGImage: unable to load image from: \"" + filename + "\"");
        }
    }
}

QGImage::~QGImage() {
    // TODO: delete _image;
    _qimage = nullptr;
}

void QGImage::draw(QPainter* painter) {
    painter->drawImage((int) getX(), (int) getY(), *_qimage);
}

std::string QGImage::getFileName() const {
    return _filename;
}

std::string QGImage::getType() const {
    return "QGImage";
}

std::string QGImage::toStringExtra() const {
    return "filename=\"" + _filename + "\"";
}


QGLine::QGLine(double x0, double y0, double x1, double y1)
        : QGObject(x0, y0),
          _dx(x1 - x0),
          _dy(y1 - y0) {
    // empty
}

QGLine::QGLine(const GPoint& p0, const GPoint& p1)
    : QGObject(p0.getX(), p0.getY()),
      _dx(p1.getX() - p0.getX()),
      _dy(p1.getY() - p0.getY()) {
    // empty
}

bool QGLine::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double x0 = getX();
    double y0 = getY();
    double x1 = x0 + _dx;
    double y1 = y0 + _dy;
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

void QGLine::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawLine((int) getX(), (int) getY(), (int) (getX() + _dx), (int) getY() + _dy);
}

GRectangle QGLine::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double x0 = (_dx < 0) ? getX() + _dx : getX();
    double y0 = (_dy < 0) ? getY() + _dy : getY();
    return GRectangle(x0, y0, getWidth(), getHeight());
}

GPoint QGLine::getEndPoint() const {
    return GPoint(getX() + _dx, getY() + _dy);
}

GPoint QGLine::getStartPoint() const {
    return getLocation();
}

double QGLine::getHeight() const {
    return std::fabs(_dy);
}

std::string QGLine::getType() const {
    return "QGLine";
}

double QGLine::getWidth() const {
    return std::fabs(_dx);
}

void QGLine::setEndPoint(double x, double y) {
    _dx = x - this->getX();
    _dy = y - this->getY();
    repaint();
}

void QGLine::setStartPoint(double x, double y) {
    _dx += getX() - x;
    _dy += getY() - y;
    setLocation(x, y);   // calls repaint
}

std::string QGLine::toStringExtra() const {
    std::ostringstream oss;
    oss << "x2=" << (_x + _dx) << " y2=" << (_y + _dy);
    return oss.str();
}


QGOval::QGOval(double x, double y, double width, double height)
        : QGObject(x, y, width, height) {
    // empty
}

bool QGOval::contains(double x, double y) const {
    if (_transformed) {
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

void QGOval::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawEllipse((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
}

std::string QGOval::getType() const {
    return "QGOval";
}


QGPolygon::QGPolygon() {
    // empty
}

QGPolygon::QGPolygon(std::initializer_list<double> coords) {
    addVertexes(coords);
}

QGPolygon::QGPolygon(std::initializer_list<GPoint> points) {
    addVertexes(points);
}

void QGPolygon::addEdge(double dx, double dy) {
    addVertex(_cx + dx, _cy + dy);
}

void QGPolygon::addEdge(const GPoint& pt) {
    addEdge(pt.getX(), pt.getY());
}

void QGPolygon::addEdges(std::initializer_list<double> coords) {
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

void QGPolygon::addEdges(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addEdge(pt);
    }
}

void QGPolygon::addPolarEdge(double r, double theta) {
    addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

void QGPolygon::addVertex(double x, double y) {
    _cx = x;
    _cy = y;
    _vertices.append(QPointF(_cx, _cy));
    repaint();
}

void QGPolygon::addVertex(const GPoint& pt) {
    addVertex(pt.getX(), pt.getY());
}

void QGPolygon::addVertexes(std::initializer_list<double> coords) {
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

void QGPolygon::addVertexes(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addVertex(pt);
    }
}

bool QGPolygon::contains(double x, double y) const {
    if (_transformed) {
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

void QGPolygon::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawPolygon(QPolygonF(_vertices));
}

GRectangle QGPolygon::getBounds() const {
    if (_transformed) {
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

double QGPolygon::getHeight() const {
    return getBounds().getHeight();
}

std::string QGPolygon::getType() const {
    return "QGPolygon";
}

Vector<GPoint> QGPolygon::getVertices() const {
    Vector<GPoint> vec;
    for (const QPointF& point : _vertices) {
        vec.add(GPoint(point.x(), point.y()));
    }
    return vec;
}

double QGPolygon::getWidth() const {
    return getBounds().getHeight();
}

std::string QGPolygon::toStringExtra() const {
    std::ostringstream oss;
    oss << "vertices=" << _vertices.size();
    return oss.str();
}


QGRect::QGRect(double x, double y, double width, double height)
        : QGObject(x, y, width, height) {
    // empty
}

void QGRect::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawRect((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
}

std::string QGRect::getType() const {
    return "QGRect";
}


/*
 * Implementation notes: QGRoundRect class
 * ---------------------------------------
 * Most of the QGRoundRect class is inherited from the QGRect class.
 */

QGRoundRect::QGRoundRect(double width, double height)
        : QGRect(/* x */ 0, /* y */ 0, width, height),
          _corner(STATIC_VARIABLE(DEFAULT_CORNER)) {
    // empty
}

QGRoundRect::QGRoundRect(double width, double height, double corner)
        : QGRect(/* x */ 0, /* y */ 0, width, height),
          _corner(corner) {
    // empty
}

QGRoundRect::QGRoundRect(double x, double y, double width, double height)
        : QGRect(x, y, width, height),
          _corner(STATIC_VARIABLE(DEFAULT_CORNER)) {
    // empty
}

QGRoundRect::QGRoundRect(double x, double y, double width, double height, double corner)
        : QGRect(x, y, width, height),
          _corner(corner) {
    // empty
}

bool QGRoundRect::contains(double x, double y) const {
    if (_transformed) {
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

void QGRoundRect::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawRoundRect((int) getX(), (int) getY(),
                           (int) getWidth(), (int) getHeight(),
                           (int) _corner, (int) _corner);
}

double QGRoundRect::getCorner() const {
    return _corner;
}

std::string QGRoundRect::getType() const {
    return "QGRoundRect";
}

void QGRoundRect::setCorner(double corner) {
    _corner = corner;
    repaint();
}

std::string QGRoundRect::toStringExtra() const {
    return "corner=" + doubleToString(_corner);
}


QGString::QGString(const std::string& str, double x, double y)
        : QGObject(x, y),
          _text(str) {
    _font = STATIC_VARIABLE(DEFAULT_GLABEL_FONT);
    updateSize();
}

void QGString::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawText((int) getX(), (int) getY(), QString::fromStdString(_text));
}

GRectangle QGString::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(getX(), getY() - getFontAscent(), getWidth(), getHeight());
}

std::string QGString::getFont() const {
    return _font;
}

double QGString::getFontAscent() const {
    QFontMetrics metrics(QGFont::toQFont(_font));
    return metrics.ascent();
}

double QGString::getFontDescent() const {
    QFontMetrics metrics(QGFont::toQFont(_font));
    return metrics.descent();
}

std::string QGString::getLabel() const {
    return _text;
}

std::string QGString::getText() const {
    return _text;
}

std::string QGString::getType() const {
    return "QGString";
}

void QGString::setFont(const std::string& font) {
    _font = font;
    updateSize();
    repaint();
}

void QGString::setLabel(const std::string& str) {
    _text = str;
    updateSize();
    repaint();
}

void QGString::setText(const std::string& str) {
    setLabel(str);
}

std::string QGString::toStringExtra() const {
    return "text=\"" + _text + "\"";
}

void QGString::updateSize() {
    QFontMetrics metrics(QGFont::toQFont(_font));
    _width = metrics.width(QString::fromStdString(_text));
    _height = metrics.height();
}

std::ostream& operator <<(std::ostream& out, const QGObject& obj) {
    return out << obj.toString();
}

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
