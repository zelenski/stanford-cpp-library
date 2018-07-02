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
#include <sstream>
#include "gmath.h"
#include "qgcolor.h"


// static constants
static const double LINE_TOLERANCE = 1.5;
static const double ARC_TOLERANCE = 2.5;
static const double DEFAULT_CORNER = 10;
static const std::string DEFAULT_GLABEL_FONT = "Dialog-13";

/*
 * Returns the square of the distance between two points.
 * Used when checking to see if a line touches a given point.
 */
static double dsq(double x0, double y0, double x1, double y1);


/* QGObject class */

bool QGObject::_sAntiAliasing = true;

QGObject::QGObject() {
    _x = 0;
    _y = 0;
    _lineWidth = 1.0;
    _color = "";
    _fillColor = "";
    _font = "";
    _fillFlag = false;
    _transformed = false;
    _visible = true;
    _parent = nullptr;
}

QGObject::~QGObject() {
    // stanfordcpplib::getPlatform()->gobject_delete(this);
}

bool QGObject::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
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
    return getBounds().getHeight();
}

double QGObject::getLineWidth() const {
    return _lineWidth;
}

GPoint QGObject::getLocation() const {
    return GPoint(_x, _y);
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
    return getBounds().getWidth();
}

double QGObject::getX() const {
    return _x;
}

double QGObject::getY() const {
    return _y;
}

void QGObject::initializeBrushAndPen(QPainter* painter) {
    // color
    _pen.setColor(QColor(_colorInt));

    // font

    // line width
    _pen.setWidth((int) _lineWidth);

    if (painter) {
        painter->setPen(_pen);
    }

    // fill color
    if (_fillFlag) {
        _brush.setStyle(Qt::SolidPattern);
        _brush.setColor(QColor(_fillColorInt));

        if (painter) {
            painter->setBrush(_brush);
        }
    } else {
        if (painter) {
            static QBrush defaultBrush;
            defaultBrush.setColor(QColor(0x00ffffff));
            painter->setBrush(defaultBrush);
        }
    }
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
    setLocation(_x + dx, _y + dy);
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

void QGObject::rotate(double /* theta */) {
    // Apply local transform
    _transformed = true;

    // TODO
    // stanfordcpplib::getPlatform()->gobject_rotate(this, theta);
}

void QGObject::scale(double sf) {
    scale(sf, sf);
}

void QGObject::scale(double /* sx */, double /* sy */) {
    // Apply local transform
    _transformed = true;

    // TODO
    // stanfordcpplib::getPlatform()->gobject_scale(this, sx, sy);
}

void QGObject::sendBackward() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendBackward(this);
    }
}

void QGObject::sendForward() {
    QGCompound* parent = getParent();
    if (parent) parent->sendForward(this);
}

void QGObject::sendToBack() {
    QGCompound* parent = getParent();
    if (parent) {
        parent->sendToBack(this);
    }
}

void QGObject::sendToFront() {
    QGCompound* parent = getParent();
    if (parent) parent->sendToFront(this);
}

void QGObject::setAntiAliasing(bool value) {
    _sAntiAliasing = value;
}

void QGObject::setBottomY(double y) {
    setBottomRightLocation(getRightX(), y);
}

void QGObject::setRightX(double x) {
    setBottomRightLocation(x, getBottomY());
}

void QGObject::setBottomRightLocation(double x, double y) {
    setLocation(x - getWidth(), y - getHeight());
}

void QGObject::setBottomRightLocation(const GPoint& pt) {
    setBottomRightLocation(pt.getX(), pt.getY());
}

void QGObject::setCenterX(double x) {
    setCenterLocation(x, getCenterY());
}

void QGObject::setCenterY(double y) {
    setCenterLocation(getCenterX(), y);
}

void QGObject::setCenterLocation(double x, double y) {
    setLocation(x - getWidth() / 2, y - getHeight() / 2);
}

void QGObject::setCenterLocation(const GPoint& pt) {
    setCenterLocation(pt.getX(), pt.getY());
}

void QGObject::setColor(int r, int g, int b) {
    this->_color = QGColor::convertRGBToColor(r, g, b);
    this->_colorInt = QGColor::convertRGBToRGB(r, g, b);
    // stanfordcpplib::getPlatform()->gobject_setColor(this, this->color);
}

void QGObject::setColor(int rgb) {
    this->_color = QGColor::convertRGBToColor(rgb);
    this->_colorInt = rgb;
    // stanfordcpplib::getPlatform()->gobject_setColor(this, this->color);
}

void QGObject::setColor(const std::string& color) {
    setColor(QGColor::convertColorToRGB(color));
}

void QGObject::setFillColor(int r, int g, int b) {
    _fillColor = QGColor::convertRGBToColor(r, g, b);
    _fillColorInt = QGColor::convertRGBToRGB(r, g, b);
    // stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void QGObject::setFillColor(int rgb) {
    _fillColor = QGColor::convertRGBToColor(rgb);
    _fillColorInt = rgb;
    // stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void QGObject::setFillColor(const std::string& color) {
    _fillColor = color;
    _fillColorInt = QGColor::convertColorToRGB(color);
    if (_fillColor == "") {
        if (isFilled()) {
            setFilled(false);
        }
    } else {
        _fillColor = QGColor::convertRGBToColor(QGColor::convertColorToRGB(color));
        if (!isFilled()) {
            setFilled(true);
        }
    }
    // stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void QGObject::setFilled(bool flag) {
    _fillFlag = flag;
    // stanfordcpplib::getPlatform()->gobject_setFilled(this, flag);
}

void QGObject::setFont(const std::string& font) {
    this->_font = font;
}

void QGObject::setLineWidth(double lineWidth) {
    this->_lineWidth = lineWidth;
    // stanfordcpplib::getPlatform()->gobject_setLineWidth(this, lineWidth);
}

void QGObject::setLocation(double x, double y) {
    this->_x = x;
    this->_y = y;
    // stanfordcpplib::getPlatform()->gobject_setLocation(this, x, y);
}

void QGObject::setLocation(const GPoint& pt) {
    setLocation(pt.getX(), pt.getY());
}

void QGObject::setVisible(bool flag) {
    _visible = flag;
    // stanfordcpplib::getPlatform()->gobject_setVisible(this, flag);
}

void QGObject::setX(double x) {
    setLocation(x, getY());
}

void QGObject::setY(double y) {
    setLocation(getX(), y);
}

std::string QGObject::toString() const {
    return getType()
            + "(" + toStringExtra()
            + " x=" + doubleToString(_x)
            + " y=" + doubleToString(_y)
            + (_lineWidth <= 1 ? "" : (" lineWidth=" + doubleToString(_lineWidth)))
            + (_color.empty() ? "" : (" color=" + _color))
            + (_fillColor.empty() ? "" : (" fillColor=" + _fillColor))
            + (_font.empty() ? "" : (" font=" + _font))
            + (" visible=" + boolToString(_visible))
            + ")";
}

std::string QGObject::toStringExtra() const {
    return "";
}


/*
 * Implementation notes: QG3DRect class
 * -----------------------------------
 * Most of the QG3DRect class is inherited from the QGRect class.
 */

QG3DRect::QG3DRect(double width, double height) {
    createQG3DRect(width, height, false);
}

QG3DRect::QG3DRect(double width, double height, bool raised) {
    createQG3DRect(width, height, raised);
}

QG3DRect::QG3DRect(double x, double y, double width, double height) {
    createQG3DRect(width, height, false);
    setLocation(x, y);
}

QG3DRect::QG3DRect(double x, double y, double width, double height,
                 bool raised) {
    createQG3DRect(width, height, raised);
    setLocation(x, y);
}

QG3DRect::~QG3DRect() {
    // empty
}

void QG3DRect::createQG3DRect(double width, double height, bool raised) {
    this->_x = 0;
    this->_y = 0;
    this->width = width;
    this->height = height;
    this->raised = raised;
    _fillFlag = false;
    _fillColor = "";
}

std::string QG3DRect::getType() const {
    return "QG3DRect";
}

void QG3DRect::draw(QPainter* painter) {
    // TODO
    initializeBrushAndPen(painter);
}

bool QG3DRect::isRaised() const {
    return raised;
}

void QG3DRect::setRaised(bool raised) {
    this->raised = raised;
    // stanfordcpplib::getPlatform()->g3drect_setRaised(this, raised);
}

std::string QG3DRect::toStringExtra() const {
    std::ostringstream oss;
    oss << "width=" << width << " height=" << height << " raised=" << std::boolalpha << raised << ")";
    return oss.str();
}


QGArc::QGArc(double width, double height, double start, double sweep) {
    createQGArc(width, height, start, sweep);
}

QGArc::QGArc(double x, double y, double width, double height,
           double start, double sweep) {
    createQGArc(width, height, start, sweep);
    setLocation(x, y);
}

bool QGArc::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (this->_x + rx);
    double dy = y - (this->_y + ry);
    double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    if (_fillFlag) {
        if (r > 1.0) return false;
    } else {
        double t = ARC_TOLERANCE / ((rx + ry) / 2);
        if (std::fabs(1.0 - r) > t) return false;
    }

    // JL BUGFIX: must scale by ry, rx.
    return containsAngle(atan2(-dy/ry, dx/rx) * 180 / PI);
}

bool QGArc::containsAngle(double theta) const {
    double start = std::min(this->start, this->start + this->sweep);
    double sweep = std::abs(this->sweep);
    if (sweep >= 360) return true;
    theta = (theta < 0) ? 360 - fmod(-theta, 360) : fmod(theta, 360);
    start = (start < 0) ? 360 - fmod(-start, 360) : fmod(start, 360);
    if (start + sweep > 360) {
        return theta >= start || theta <= start + sweep - 360;
    } else {
        return theta >= start && theta <= start + sweep;
    }
}

void QGArc::createQGArc(double width, double height, double start, double sweep) {
    this->_x = 0;
    this->_y = 0;
    frameWidth = width;
    frameHeight = height;
    this->start = start;
    this->sweep = sweep;
    _fillFlag = false;
    _fillColor = "";
}

void QGArc::draw(QPainter* painter) {
    // TODO
    initializeBrushAndPen(painter);
}

GPoint QGArc::getArcPoint(double theta) const {
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    double cx = _x + rx;
    double cy = _y + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

GRectangle QGArc::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    double cx = _x + rx;
    double cy = _y + ry;
    double startRadians = start * PI / 180;
    double sweepRadians = sweep * PI / 180;
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
    return getArcPoint(start + sweep);
}

GRectangle QGArc::getFrameRectangle() const {
    return GRectangle(0, 0, 0, 0);
}

double QGArc::getStartAngle() const {
    return start;
}

GPoint QGArc::getStartPoint() const {
    return getArcPoint(start);
}

double QGArc::getSweepAngle() const {
    return sweep;
}

std::string QGArc::getType() const {
    return "QGArc";
}

void QGArc::setFrameRectangle(double x, double y, double width, double height) {
    this->_x = x;
    this->_y = y;
    frameWidth = width;
    frameHeight = height;
    // stanfordcpplib::getPlatform()->garc_setFrameRectangle(this, x, y, width, height);
}

void QGArc::setFrameRectangle(const GRectangle& rect) {
    setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(),
                      rect.getHeight());
}

void QGArc::setStartAngle(double start) {
    this->start = start;
    // stanfordcpplib::getPlatform()->garc_setStartAngle(this, start);
}

void QGArc::setSweepAngle(double sweep) {
    this->sweep = sweep;
    // stanfordcpplib::getPlatform()->garc_setSweepAngle(this, sweep);
}

std::string QGArc::toStringExtra() const {
    std::ostringstream oss;
    oss << "frameWidth=" << frameWidth << " frameHeight=" << frameHeight
        << " start=" << start << " sweep=" << sweep;
    return oss.str();
}


QGCompound::QGCompound()
        : _autoRepaint(true) {
    // stanfordcpplib::getPlatform()->gcompound_constructor(this);
}

void QGCompound::add(QGObject* gobj) {
    // stanfordcpplib::getPlatform()->gcompound_add(this, gobj);
    contents.add(gobj);
    gobj->_parent = this;
    conditionalRepaint();
}

void QGCompound::add(QGObject* gobj, double x, double y) {
    gobj->setLocation(x, y);
    add(gobj);   // calls conditionalRepaint
}

void QGCompound::clear() {
    removeAll();
    conditionalRepaint();
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
    for (int i = 0, sz = contents.size(); i < sz; i++) {
        if (contents[i]->contains(x, y)) {
            return true;
        }
    }
    return false;
}

void QGCompound::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    for (QGObject* obj : contents) {
        obj->draw(painter);
    }
}

int QGCompound::findQGObject(QGObject* gobj) const {
    int n = contents.size();
    for (int i = 0; i < n; i++) {
        if (contents.get(i) == gobj) {
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
    for (int i = 0; i < contents.size(); i++) {
        GRectangle bounds = contents.get(i)->getBounds();
        xMin = std::min(xMin, bounds.getX());
        yMin = std::min(yMin, bounds.getY());
        xMax = std::max(xMax, bounds.getX());
        yMin = std::max(yMax, bounds.getY());   // JL BUGFIX 2016/10/11
    }
    // JL BUGFIX: shifted anchor point
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

QGObject* QGCompound::getElement(int index) const {
    return contents.get(index);
}

QGObject* QGCompound::getElementAt(double x, double y) const {
    for (QGObject* gobj : contents) {
        if (gobj && gobj->contains(x, y)) {
            return gobj;
        }
    }
    return nullptr;
}

int QGCompound::getElementCount() const {
    return contents.size();
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

void QGCompound::removeAll() {
    while (!contents.isEmpty()) {
        removeAt(0);   // calls conditionalRepaint
    }
}

void QGCompound::removeAt(int index) {
    QGObject* gobj = contents[index];
    contents.remove(index);
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
        contents.remove(index);
        contents.insert(index - 1, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendBackward(gobj);
        conditionalRepaint();
    }
}

void QGCompound::sendForward(QGObject* gobj) {
    int index = findQGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.insert(index + 1, gobj);
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
        contents.remove(index);
        contents.insert(0, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendToBack(gobj);
        conditionalRepaint();
    }
}

void QGCompound::sendToFront(QGObject* gobj) {
    int index = findQGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.add(gobj);
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


void QGImage::draw(QPainter* painter) {
    // TODO
    initializeBrushAndPen(painter);
}

QGImage::QGImage(const std::string& filename) {
    createQGImage(filename);
}

QGImage::QGImage(const std::string& filename, double x, double y) {
    createQGImage(filename);
    setLocation(x, y);
}

void QGImage::createQGImage(const std::string& filename) {
    this->filename = filename;
    GDimension size;   // TODO = stanfordcpplib::getPlatform()->gimage_constructor(this, filename);
    width = size.getWidth();
    height = size.getHeight();
}

GRectangle QGImage::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(_x, _y, width, height);
}

std::string QGImage::getType() const {
    return "QGImage";
}

std::string QGImage::toStringExtra() const {
    return "filename=" + filename;
}


QGLine::QGLine(double x0, double y0, double x1, double y1) {
    // stanfordcpplib::getPlatform()->gline_constructor(this, x0, y0, x1, y1);
    _x = x0;
    _y = y0;
    dx = x1 - x0;
    dy = y1 - y0;
}

QGLine::QGLine(const GPoint& p0, const GPoint& p1) {
    _x = p0.getX();
    _y = p0.getY();
    dx = p1.getX() - p0.getX();
    dy = p1.getY() - p1.getY();
}

bool QGLine::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double x0 = getX();
    double y0 = getY();
    double x1 = x0 + dx;
    double y1 = y0 + dy;
    double tSquared = LINE_TOLERANCE * LINE_TOLERANCE;
    if (dsq(x, y, x0, y0) < tSquared) {
        return true;
    }
    if (dsq(x, y, x1, y1) < tSquared) {
        return true;
    }
    if (x < std::min(x0, x1) - LINE_TOLERANCE) {
        return false;
    }
    if (x > std::max(x0, x1) + LINE_TOLERANCE) {
        return false;
    }
    if (y < std::min(y0, y1) - LINE_TOLERANCE) {
        return false;
    }
    if (y > std::max(y0, y1) + LINE_TOLERANCE) {
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
    painter->drawLine((int) _x, (int) _y, (int) (_x + dx), (int) _y + dy);
}

GRectangle QGLine::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double x0 = (dx < 0) ? _x + dx : _x;
    double y0 = (dy < 0) ? _y + dy : _y;
    return GRectangle(x0, y0, std::fabs(dx), std::fabs(dy));
}

GPoint QGLine::getEndPoint() const {
    return GPoint(_x + dx, _y + dy);
}

GPoint QGLine::getStartPoint() const {
    return GPoint(_x, _y);
}

std::string QGLine::getType() const {
    return "QGLine";
}

void QGLine::setEndPoint(double x, double y) {
    dx = x - this->_x;
    dy = y - this->_y;
    // stanfordcpplib::getPlatform()->gline_setEndPoint(this, x, y);
}

void QGLine::setStartPoint(double x, double y) {
    dx += this->_x - x;
    dy += this->_y - y;
    this->_x = x;
    this->_y = y;
    // stanfordcpplib::getPlatform()->gline_setStartPoint(this, x, y);
}

std::string QGLine::toStringExtra() const {
    std::ostringstream oss;
    oss << "x2=" << (_x + dx) << " y2=" << (_y + dy);
    return oss.str();
}


QGOval::QGOval(double width, double height) {
    createQGOval(width, height);
}

QGOval::QGOval(double x, double y, double width, double height) {
    createQGOval(width, height);
    setLocation(x, y);
}

QGOval::~QGOval() {
    // empty
}

bool QGOval::contains(double x, double y) const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = width / 2;
    double ry = height / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (this->_x + rx);
    double dy = y - (this->_y + ry);
    return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void QGOval::createQGOval(double width, double height) {
    this->_x = 0;
    this->_y = 0;
    this->width = width;
    this->height = height;
    _fillFlag = false;
    _fillColor = "";
}

void QGOval::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawEllipse((int) _x, (int) _y, (int) width, (int) height);
}

GRectangle QGOval::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
        return GRectangle(_x, _y, width, height);
    } else {
        return GRectangle(_x, _y, width, height);
    }
}

std::string QGOval::getType() const {
    return "QGOval";
}

void QGOval::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

void QGOval::setBounds(const GRectangle& bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void QGOval::setSize(double width, double height) {
    if (_transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    // stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
}

void QGOval::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

std::string QGOval::toStringExtra() const {
    std::ostringstream oss;
    oss << "width=" << width << " height=" << height;
    return oss.str();
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
    addVertex(cx + dx, cy + dy);
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
    cx = x;
    cy = y;
    vertices.add(GPoint(cx, cy));
    // stanfordcpplib::getPlatform()->gpolygon_addVertex(this, cx, cy);
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
    int n = vertices.size();
    if (n < 2) return false;
    if (vertices[0] == vertices[n - 1]) {
        n--;
    }
    double x0 = vertices[0].getX();
    double y0 = vertices[0].getY();
    for (int i = 1; i <= n; i++) {
        double x1 = vertices[i % n].getX();
        double y1 = vertices[i % n].getY();
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
    // for (int i = 1; i <= n; i++) {
        // TODO: drawPolygon?  drawLines?
    // }
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
    for (int i = 0; i < vertices.size(); i++) {
        double x = vertices[i].getX();
        double y = vertices[i].getY();
        if (i == 0 || x < xMin) xMin = x;
        if (i == 0 || y < yMin) yMin = y;
        if (i == 0 || x > xMax) xMax = x;
        if (i == 0 || y > yMax) yMax = y;
    }
    // JL BUGFIX: add getX, getY
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

std::string QGPolygon::getType() const {
    return "QGPolygon";
}

Vector<GPoint> QGPolygon::getVertices() const {
    return vertices;
}

std::string QGPolygon::toStringExtra() const {
    std::ostringstream oss;
    oss << "vertices=" << vertices.size();
    return oss.str();
}


QGRect::QGRect(double width, double height) {
    createQGRect(width, height);
}

QGRect::QGRect() : width(0), height(0) {
    // Called only by the QGRoundRect and QG3DRect subclasses
}

QGRect::QGRect(double x, double y, double width, double height) {
    createQGRect(width, height);
    setLocation(x, y);
}

QGRect::~QGRect() {
    // empty
}

void QGRect::createQGRect(double width, double height) {
    this->_x = 0;
    this->_y = 0;
    this->width = width;
    this->height = height;
    _fillFlag = false;
    _fillColor = "";
}

void QGRect::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawRect((int) _x, (int) _y, (int) width, (int) height);
}

GRectangle QGRect::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
        return GRectangle(_x, _y, width, height);
    } else {
        return GRectangle(_x, _y, width, height);
    }
}

std::string QGRect::getType() const {
    return "QGRect";
}

void QGRect::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

void QGRect::setBounds(const GRectangle& bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void QGRect::setSize(double width, double height) {
    if (_transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    // stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
}

void QGRect::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

std::string QGRect::toStringExtra() const {
    std::ostringstream oss;
    oss << "width=" << width << " height=" << height;
    return oss.str();
}


/*
 * Implementation notes: QGRoundRect class
 * --------------------------------------
 * Most of the QGRoundRect class is inherited from the QGRect class.
 */

QGRoundRect::QGRoundRect(double width, double height) {
    createQGRoundRect(width, height, DEFAULT_CORNER);
}

QGRoundRect::QGRoundRect(double width, double height, double corner) {
    createQGRoundRect(width, height, corner);
}

QGRoundRect::QGRoundRect(double x, double y, double width, double height) {
    createQGRoundRect(width, height, DEFAULT_CORNER);
    setLocation(x, y);
}

QGRoundRect::QGRoundRect(double x, double y, double width, double height,
                       double corner) {
    createQGRoundRect(width, height, corner);
    setLocation(x, y);
}

QGRoundRect::~QGRoundRect() {
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
    double a = std::min(corner, width) / 2;
    double b = std::min(corner, height) / 2;

    // Get distances from nearest edges of bounding rectangle
    double dx = std::min(std::abs(x - getX()), std::abs(x - (getX() + width)));
    double dy = std::min(std::abs(y - getY()), std::abs(y - (getY() + height)));

    if (dx > a || dy > b) {
        return true;   // in "central cross" of rounded rect
    }

    return (dx - a) * (dx - a) / (a * a) + (dy - b) * (dy - b) / (b * b) <= 1;
}

void QGRoundRect::createQGRoundRect(double width, double height, double corner) {
    this->_x = 0;
    this->_y = 0;
    this->width = width;
    this->height = height;
    this->corner = corner;
    _fillFlag = false;
    _fillColor = "";
}

void QGRoundRect::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawRoundRect((int) _x, (int) _y, (int) width, (int) height, (int) corner, (int) corner);
}

std::string QGRoundRect::getType() const {
    return "QGRoundRect";
}

std::string QGRoundRect::toStringExtra() const {
    std::ostringstream oss;
    oss << "width=" << width << " height=" << height << " corner=" << corner;
    return oss.str();
}


QGString::QGString(const std::string& str) {
    createQGString(str);
}

QGString::QGString(const std::string& str, double x, double y) {
    createQGString(str);
    setLocation(x, y);
}

void QGString::createQGString(const std::string& str) {
    this->str = str;
    setFont(DEFAULT_GLABEL_FONT);
    GDimension size;   // TODO = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = 0;   // TODO = stanfordcpplib::getPlatform()->glabel_getFontAscent(this);
    descent = 0;   // TODO = stanfordcpplib::getPlatform()->glabel_getFontDescent(this);
}

void QGString::draw(QPainter* painter) {
    initializeBrushAndPen(painter);
    painter->drawText((int) _x, (int) _y, QString::fromStdString(str));
}

GRectangle QGString::getBounds() const {
    if (_transformed) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(_x, _y - ascent, width, height);
}

std::string QGString::getFont() const {
    return _font;
}

double QGString::getFontAscent() const {
    return ascent;
}

double QGString::getFontDescent() const {
    return descent;
}

std::string QGString::getLabel() const {
    return str;
}

std::string QGString::getText() const {
    return str;
}

std::string QGString::getType() const {
    return "QGString";
}

void QGString::setFont(const std::string& font) {
    QGObject::setFont(font);
    // stanfordcpplib::getPlatform()->glabel_setFont(this, font);
    GDimension size;   // TODO = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = 0;   // TODO = stanfordcpplib::getPlatform()->glabel_getFontAscent(this);
    descent = 0;   // TODO = stanfordcpplib::getPlatform()->glabel_getFontDescent(this);
}

void QGString::setLabel(const std::string& str) {
    this->str = str;
    // stanfordcpplib::getPlatform()->glabel_setLabel(this, str);
    GDimension size;   // TODO = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
}

void QGString::setText(const std::string& str) {
    setLabel(str);
}

std::string QGString::toStringExtra() const {
    return "text=\"" + str + "\"";
}

std::ostream& operator <<(std::ostream& out, const QGObject& obj) {
    return out << obj.toString();
}

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
