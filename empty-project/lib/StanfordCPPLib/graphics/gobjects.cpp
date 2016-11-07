/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the gobjects.h interface.
 * 
 * @version 2016/11/07
 * - alphabetized all members
 * - modified all members that accept std::string to take const std::string&
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2016/09/27
 * - added get/setText methods to GLabel
 * @version 2015/10/13
 * - replaced 'fabs' with 'std::fabs'
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * - made top global vars static to reduce visibility
 * @version 2015/05/28
 * - replaced some calls on 'abs' with 'fabs' to remove Mac compiler warnings
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "gobjects.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include "gevents.h"
#include "gmath.h"
#include "private/platform.h"


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


/* GObject class */

GObject::GObject() {
    x = 0;
    y = 0;
    color = "";
    lineWidth = 1.0;
    transformed = false;
    visible = true;
    parent = nullptr;
}

GObject::~GObject() {
    stanfordcpplib::getPlatform()->gobject_delete(this);
}

bool GObject::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    } else {
        return getBounds().contains(x, y);
    }
}

bool GObject::contains(const GPoint& pt) const {
    return contains(pt.getX(), pt.getY());
}

std::string GObject::getColor() const {
    return color;
}

double GObject::getHeight() const {
    return getBounds().getHeight();
}

double GObject::getLineWidth() const {
    return lineWidth;
}

GPoint GObject::getLocation() const {
    return GPoint(x, y);
}

GCompound* GObject::getParent() const {
    return parent;
}

GDimension GObject::getSize() const {
    GRectangle bounds = getBounds();
    return GDimension(bounds.getWidth(), bounds.getHeight());
}

double GObject::getWidth() const {
    return getBounds().getWidth();
}

double GObject::getX() const {
    return x;
}

double GObject::getY() const {
    return y;
}

bool GObject::isVisible() const {
    return visible;
}

void GObject::move(double dx, double dy) {
    setLocation(x + dx, y + dy);
}

void GObject::rotate(double theta) {
    // Apply local transform
    transformed = true;
    stanfordcpplib::getPlatform()->gobject_rotate(this, theta);
}

void GObject::scale(double sf) {
    scale(sf, sf);
}

void GObject::scale(double sx, double sy) {
    // Apply local transform
    transformed = true;
    stanfordcpplib::getPlatform()->gobject_scale(this, sx, sy);
}

void GObject::sendBackward() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendBackward(this);
    }
}

void GObject::sendForward() {
    GCompound* parent = getParent();
    if (parent) parent->sendForward(this);
}

void GObject::sendToBack() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendToBack(this);
    }
}

void GObject::sendToFront() {
    GCompound* parent = getParent();
    if (parent) parent->sendToFront(this);
}

void GObject::setAntiAliasing(bool value) {
    stanfordcpplib::getPlatform()->gobject_setAntialiasing(value);
}

void GObject::setColor(int rgb) {
    this->color = convertRGBToColor(rgb);
    stanfordcpplib::getPlatform()->gobject_setColor(this, this->color);
}

void GObject::setColor(const std::string& color) {
    setColor(convertColorToRGB(color));
}

void GObject::setLineWidth(double lineWidth) {
    this->lineWidth = lineWidth;
    stanfordcpplib::getPlatform()->gobject_setLineWidth(this, lineWidth);
}

void GObject::setLocation(double x, double y) {
    this->x = x;
    this->y = y;
    stanfordcpplib::getPlatform()->gobject_setLocation(this, x, y);
}

void GObject::setLocation(const GPoint& pt) {
    setLocation(pt.getX(), pt.getY());
}

void GObject::setVisible(bool flag) {
    visible = flag;
    stanfordcpplib::getPlatform()->gobject_setVisible(this, flag);
}


/*
 * Implementation notes: GRect class
 * ---------------------------------
 * The GRect class is the most straightforward of the shape classes.
 */

GRect::GRect(double width, double height) {
    createGRect(width, height);
}

GRect::GRect() : width(0), height(0), fillFlag(false) {
    /* Called only by the GRoundRect and G3DRect subclasses */
}

GRect::GRect(double x, double y, double width, double height) {
    createGRect(width, height);
    setLocation(x, y);
}

GRect::~GRect() {
    /* Empty */
}

void GRect::createGRect(double width, double height) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->grect_constructor(this, width, height);
}

GRectangle GRect::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    } else {
        return GRectangle(x, y, width, height);
    }
}

std::string GRect::getFillColor() const {
    return fillColor;
}

std::string GRect::getType() const {
    return "GRect";
}

bool GRect::isFilled() const {
    return fillFlag;
}

void GRect::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

void GRect::setBounds(const GRectangle& bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void GRect::setFillColor(int rgb) {
    fillColor = convertRGBToColor(rgb);
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GRect::setFillColor(const std::string& color) {
    fillColor = color;
    if (fillColor == "") {
        if (isFilled()) {
            setFilled(false);
        }
    } else {
        fillColor = convertRGBToColor(convertColorToRGB(color));
        if (!isFilled()) {
            setFilled(true);
        }
    }
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GRect::setFilled(bool flag) {
    fillFlag = true;
    stanfordcpplib::getPlatform()->gobject_setFilled(this, flag);
}

void GRect::setSize(double width, double height) {
    if (transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
}

void GRect::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

std::string GRect::toString() const {
    std::ostringstream oss;
    oss << "GRect(" << x << ", " << y << ", "
        << width << ", " << height << ")";
    return oss.str();
}


/*
 * Implementation notes: GRoundRect class
 * --------------------------------------
 * Most of the GRoundRect class is inherited from the GRect class.
 */

GRoundRect::GRoundRect(double width, double height) {
    createGRoundRect(width, height, DEFAULT_CORNER);
}

GRoundRect::GRoundRect(double width, double height, double corner) {
    createGRoundRect(width, height, corner);
}

GRoundRect::GRoundRect(double x, double y, double width, double height) {
    createGRoundRect(width, height, DEFAULT_CORNER);
    setLocation(x, y);
}

GRoundRect::GRoundRect(double x, double y, double width, double height,
                       double corner) {
    createGRoundRect(width, height, corner);
    setLocation(x, y);
}

GRoundRect::~GRoundRect() {
    /* Empty */
}

bool GRoundRect::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
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

void GRoundRect::createGRoundRect(double width, double height, double corner) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    this->corner = corner;
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->groundrect_constructor(this, width, height, corner);
}

std::string GRoundRect::getType() const {
    return "GRoundRect";
}

std::string GRoundRect::toString() const {
    std::ostringstream oss;
    oss << "GRoundRect(" << x << ", " << y << ", "
        << width << ", " << height << ", " << corner << ")";
    return oss.str();
}


/*
 * Implementation notes: G3DRect class
 * -----------------------------------
 * Most of the G3DRect class is inherited from the GRect class.
 */

G3DRect::G3DRect(double width, double height) {
    createG3DRect(width, height, false);
}

G3DRect::G3DRect(double width, double height, bool raised) {
    createG3DRect(width, height, raised);
}

G3DRect::G3DRect(double x, double y, double width, double height) {
    createG3DRect(width, height, false);
    setLocation(x, y);
}

G3DRect::G3DRect(double x, double y, double width, double height,
                 bool raised) {
    createG3DRect(width, height, raised);
    setLocation(x, y);
}

G3DRect::~G3DRect() {
    /* Empty */
}

void G3DRect::createG3DRect(double width, double height, bool raised) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    this->raised = raised;
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->g3drect_constructor(this, width, height, raised);
}

std::string G3DRect::getType() const {
    return "G3DRect";
}

bool G3DRect::isRaised() const {
    return raised;
}

void G3DRect::setRaised(bool raised) {
    this->raised = raised;
    stanfordcpplib::getPlatform()->g3drect_setRaised(this, raised);
}

std::string G3DRect::toString() const {
    std::ostringstream oss;
    oss << std::boolalpha << "G3DRect(" << x << ", " << y << ", "
        << width << ", " << height << ", " << raised << ")";
    return oss.str();
}


/* GOval class */

GOval::GOval(double width, double height) {
    createGOval(width, height);
}

GOval::GOval(double x, double y, double width, double height) {
    createGOval(width, height);
    setLocation(x, y);
}

GOval::~GOval() {
    /* Empty */
}

bool GOval::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = width / 2;
    double ry = height / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (this->x + rx);
    double dy = y - (this->y + ry);
    return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void GOval::createGOval(double width, double height) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->goval_constructor(this, width, height);
}

GRectangle GOval::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    } else {
        return GRectangle(x, y, width, height);
    }
}

std::string GOval::getFillColor() const {
    return fillColor;
}

std::string GOval::getType() const {
    return "GOval";
}

bool GOval::isFilled() const {
    return fillFlag;
}

void GOval::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

void GOval::setBounds(const GRectangle& bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void GOval::setFillColor(int color) {
    fillColor = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GOval::setFillColor(const std::string& color) {
    fillColor = color;
    if (fillColor == "") {
        if (isFilled()) {
            setFilled(false);
        }
    } else {
        fillColor = convertRGBToColor(convertColorToRGB(color));
        if (!isFilled()) {
            setFilled(true);
        }
    }
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GOval::setFilled(bool flag) {
    fillFlag = true;
    stanfordcpplib::getPlatform()->gobject_setFilled(this, flag);
}

void GOval::setSize(double width, double height) {
    if (transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    stanfordcpplib::getPlatform()->gobject_setSize(this, width, height);
}

void GOval::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

std::string GOval::toString() const {
    std::ostringstream oss;
    oss << "GOval(" << x << ", " << y << ", "
        << width << ", " << height << ")";
    return oss.str();
}


/* GArc class */

GArc::GArc(double width, double height, double start, double sweep) {
    createGArc(width, height, start, sweep);
}

GArc::GArc(double x, double y, double width, double height,
           double start, double sweep) {
    createGArc(width, height, start, sweep);
    setLocation(x, y);
}

bool GArc::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (this->x + rx);
    double dy = y - (this->y + ry);
    double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    if (fillFlag) {
        if (r > 1.0) return false;
    } else {
        double t = ARC_TOLERANCE / ((rx + ry) / 2);
        if (std::fabs(1.0 - r) > t) return false;
    }

    // JL BUGFIX: must scale by ry, rx.
    return containsAngle(atan2(-dy/ry, dx/rx) * 180 / PI);
}

bool GArc::containsAngle(double theta) const {
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

void GArc::createGArc(double width, double height, double start, double sweep) {
    this->x = 0;
    this->y = 0;
    frameWidth = width;
    frameHeight = height;
    this->start = start;
    this->sweep = sweep;
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->garc_constructor(this, width, height, start, sweep);
}

GPoint GArc::getArcPoint(double theta) const {
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    double cx = x + rx;
    double cy = y + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

GRectangle GArc::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    double cx = x + rx;
    double cy = y + ry;
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

GPoint GArc::getEndPoint() const {
    return getArcPoint(start + sweep);
}

std::string GArc::getFillColor() const {
    return fillColor;
}

GRectangle GArc::getFrameRectangle() const {
    return GRectangle(0, 0, 0, 0);
}

double GArc::getStartAngle() const {
    return start;
}

GPoint GArc::getStartPoint() const {
    return getArcPoint(start);
}

double GArc::getSweepAngle() const {
    return sweep;
}

std::string GArc::getType() const {
    return "GArc";
}

bool GArc::isFilled() const {
    return fillFlag;
}

void GArc::setFillColor(int color) {
    fillColor = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GArc::setFillColor(const std::string& color) {
    fillColor = color;
    if (fillColor == "") {
        if (isFilled()) {
            setFilled(false);
        }
    } else {
        fillColor = convertRGBToColor(convertColorToRGB(color));
        if (!isFilled()) {
            setFilled(true);
        }
    }
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GArc::setFilled(bool flag) {
    fillFlag = true;
    stanfordcpplib::getPlatform()->gobject_setFilled(this, flag);
}

void GArc::setFrameRectangle(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    frameWidth = width;
    frameHeight = height;
    stanfordcpplib::getPlatform()->garc_setFrameRectangle(this, x, y, width, height);
}

void GArc::setFrameRectangle(const GRectangle& rect) {
    setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(),
                      rect.getHeight());
}

void GArc::setStartAngle(double start) {
    this->start = start;
    stanfordcpplib::getPlatform()->garc_setStartAngle(this, start);
}

void GArc::setSweepAngle(double sweep) {
    this->sweep = sweep;
    stanfordcpplib::getPlatform()->garc_setSweepAngle(this, sweep);
}

std::string GArc::toString() const {
    std::ostringstream oss;
    oss << "GArc(" << x << ", " << y << ", " << frameWidth << ", "
        << frameHeight << ", " << start << ", " << sweep << ")";
    return oss.str();
}


/* GCompound class */

GCompound::GCompound() {
    stanfordcpplib::getPlatform()->gcompound_constructor(this);
}

void GCompound::add(GObject* gobj) {
    stanfordcpplib::getPlatform()->gcompound_add(this, gobj);
    contents.add(gobj);
    gobj->parent = this;
}

void GCompound::add(GObject* gobj, double x, double y) {
    gobj->setLocation(x, y);
    add(gobj);
}

bool GCompound::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    for (int i = 0, sz = contents.size(); i < sz; i++) {
        if (contents[i]->contains(x, y)) return true;
    }
    return false;
}

int GCompound::findGObject(GObject* gobj) const {
    int n = contents.size();
    for (int i = 0; i < n; i++) {
        if (contents.get(i) == gobj) {
            return i;
        }
    }
    return -1;
}

GRectangle GCompound::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
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

GObject* GCompound::getElement(int index) const {
    return contents.get(index);
}

GObject* GCompound::getElementAt(double x, double y) const {
    for (GObject* gobj : contents) {
        if (gobj && gobj->contains(x, y)) {
            return gobj;
        }
    }
    return nullptr;
}

int GCompound::getElementCount() const {
    return contents.size();
}

std::string GCompound::getType() const {
    return "GCompound";
}

void GCompound::remove(GObject* gobj) {
    int index = findGObject(gobj);
    if (index != -1) removeAt(index);
}

void GCompound::removeAll() {
    while (!contents.isEmpty()) {
        removeAt(0);
    }
}

void GCompound::removeAt(int index) {
    GObject* gobj = contents[index];
    contents.remove(index);
    stanfordcpplib::getPlatform()->gobject_remove(gobj);
    gobj->parent = nullptr;
}

void GCompound::sendBackward(GObject* gobj) {
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        contents.remove(index);
        contents.insert(index - 1, gobj);
        stanfordcpplib::getPlatform()->gobject_sendBackward(gobj);
    }
}

void GCompound::sendForward(GObject* gobj) {
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.insert(index + 1, gobj);
        stanfordcpplib::getPlatform()->gobject_sendForward(gobj);
    }
}

void GCompound::sendToBack(GObject* gobj) {
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        contents.remove(index);
        contents.insert(0, gobj);
        stanfordcpplib::getPlatform()->gobject_sendToBack(gobj);
    }
}

void GCompound::sendToFront(GObject* gobj) {
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.add(gobj);
        stanfordcpplib::getPlatform()->gobject_sendToFront(gobj);
    }
}

std::string GCompound::toString() const {
    return "GCompound(...)";
}


/* GImage class */

GImage::GImage(const std::string& filename) {
    createGImage(filename);
}

GImage::GImage(const std::string& filename, double x, double y) {
    createGImage(filename);
    setLocation(x, y);
}

void GImage::createGImage(const std::string& filename) {
    this->filename = filename;
    GDimension size = stanfordcpplib::getPlatform()->gimage_constructor(this, filename);
    width = size.getWidth();
    height = size.getHeight();
}

GRectangle GImage::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(x, y, width, height);
}

std::string GImage::getType() const {
    return "GImage";
}

std::string GImage::toString() const {
    return "GImage(\"" + filename + "\")";
}


/*
 * Implementation notes: GLabel class
 * ----------------------------------
 */

GLabel::GLabel(const std::string& str) {
    createGLabel(str);
}

GLabel::GLabel(const std::string& str, double x, double y) {
    createGLabel(str);
    setLocation(x, y);
}

void GLabel::createGLabel(const std::string& str) {
    this->str = str;
    stanfordcpplib::getPlatform()->glabel_constructor(this, str);
    setFont(DEFAULT_GLABEL_FONT);
    GDimension size = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = stanfordcpplib::getPlatform()->glabel_getFontAscent(this);
    descent = stanfordcpplib::getPlatform()->glabel_getFontDescent(this);
}

GRectangle GLabel::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(x, y - ascent, width, height);
}

std::string GLabel::getFont() const {
    return font;
}

double GLabel::getFontAscent() const {
    return ascent;
}

double GLabel::getFontDescent() const {
    return descent;
}

std::string GLabel::getLabel() const {
    return str;
}

std::string GLabel::getText() const {
    return str;
}

std::string GLabel::getType() const {
    return "GLabel";
}

void GLabel::setFont(const std::string& font) {
    this->font = font;
    stanfordcpplib::getPlatform()->glabel_setFont(this, font);
    GDimension size = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = stanfordcpplib::getPlatform()->glabel_getFontAscent(this);
    descent = stanfordcpplib::getPlatform()->glabel_getFontDescent(this);
}

void GLabel::setLabel(const std::string& str) {
    this->str = str;
    stanfordcpplib::getPlatform()->glabel_setLabel(this, str);
    GDimension size = stanfordcpplib::getPlatform()->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
}

void GLabel::setText(const std::string& str) {
    setLabel(str);
}

std::string GLabel::toString() const {
    return "GLabel(\"" + str + "\")";
}


/*
 * Implementation notes: GLine class
 * ---------------------------------
 */

GLine::GLine(double x0, double y0, double x1, double y1) {
    stanfordcpplib::getPlatform()->gline_constructor(this, x0, y0, x1, y1);
    x = x0;
    y = y0;
    dx = x1 - x0;
    dy = y1 - y0;
}

bool GLine::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
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

GRectangle GLine::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double x0 = (dx < 0) ? x + dx : x;
    double y0 = (dy < 0) ? y + dy : y;
    return GRectangle(x0, y0, std::fabs(dx), std::fabs(dy));
}

GPoint GLine::getEndPoint() const {
    return GPoint(x + dx, y + dy);
}

GPoint GLine::getStartPoint() const {
    return GPoint(x, y);
}

std::string GLine::getType() const {
    return "GLine";
}

void GLine::setEndPoint(double x, double y) {
    dx = x - this->x;
    dy = y - this->y;
    stanfordcpplib::getPlatform()->gline_setEndPoint(this, x, y);
}

void GLine::setStartPoint(double x, double y) {
    dx += this->x - x;
    dy += this->y - y;
    this->x = x;
    this->y = y;
    stanfordcpplib::getPlatform()->gline_setStartPoint(this, x, y);
}

std::string GLine::toString() const {
    std::ostringstream oss;
    oss << "GLine(" << x << ", " << y << ", "
        << (x + dx) << ", " << (y + dy) << ")";
    return oss.str();
}


/*
 * Implementation notes: GPolygon class
 * ------------------------------------
 */

GPolygon::GPolygon() {
    fillFlag = false;
    fillColor = "";
    stanfordcpplib::getPlatform()->gpolygon_constructor(this);
}

void GPolygon::addEdge(double dx, double dy) {
    addVertex(cx + dx, cy + dy);
}

void GPolygon::addPolarEdge(double r, double theta) {
    addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

void GPolygon::addVertex(double x, double y) {
    cx = x;
    cy = y;
    vertices.add(GPoint(cx, cy));
    stanfordcpplib::getPlatform()->gpolygon_addVertex(this, cx, cy);
}

bool GPolygon::contains(double x, double y) const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    int crossings = 0;
    int n = vertices.size();
    if (n < 2) return false;
    if (vertices[0] == vertices[n - 1]) n--;
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

GRectangle GPolygon::getBounds() const {
    if (transformed) {
        return stanfordcpplib::getPlatform()->gobject_getBounds(this);
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

std::string GPolygon::getFillColor() const {
    return fillColor;
}

std::string GPolygon::getType() const {
    return "GPolygon";
}

Vector<GPoint> GPolygon::getVertices() const {
    return vertices;
}

bool GPolygon::isFilled() const {
    return fillFlag;
}

void GPolygon::setFillColor(int rgb) {
    fillColor = convertRGBToColor(rgb);
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GPolygon::setFillColor(const std::string& color) {
    fillColor = color;
    if (fillColor == "") {
        if (isFilled()) {
            setFilled(false);
        }
    } else {
        fillColor = convertRGBToColor(convertColorToRGB(color));
        if (!isFilled()) {
            setFilled(true);
        }
    }
    stanfordcpplib::getPlatform()->gobject_setFillColor(this, fillColor);
}

void GPolygon::setFilled(bool flag) {
    fillFlag = true;
    stanfordcpplib::getPlatform()->gobject_setFilled(this, flag);
}

std::string GPolygon::toString() const {
    std::ostringstream oss;
    oss << "GPolygon(" << vertices.size() << " vertices)";
    return oss.str();
}


/* Utility functions */

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
