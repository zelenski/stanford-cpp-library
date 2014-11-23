/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the gobjects.h interface.
 * 
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
#include "gtypes.h"
#include "gwindow.h"
#include "platform.h"
#include "vector.h"

static Platform *pp = getPlatform();

const double LINE_TOLERANCE = 1.5;
const double ARC_TOLERANCE = 2.5;
const double DEFAULT_CORNER = 10;
const std::string DEFAULT_GLABEL_FONT = "Dialog-13";

static double dsq(double x0, double y0, double x1, double y1);

void GObject::setAntiAliasing(bool value) {
    pp->gobject_setAntialiasing(value);
}

double GObject::getX() const {
    return x;
}

double GObject::getY() const {
    return y;
}

GPoint GObject::getLocation() const {
    return GPoint(x, y);
}

void GObject::setLocation(const GPoint & pt) {
    setLocation(pt.getX(), pt.getY());
}

void GObject::setLocation(double x, double y) {
    this->x = x;
    this->y = y;
    pp->gobject_setLocation(this, x, y);
}

void GObject::move(double dx, double dy) {
    setLocation(x + dx, y + dy);
}

double GObject::getWidth() const {
    return getBounds().getWidth();
}

double GObject::getHeight() const {
    return getBounds().getHeight();
}

GDimension GObject::getSize() const {
    GRectangle bounds = getBounds();
    return GDimension(bounds.getWidth(), bounds.getHeight());
}

void GObject::setLineWidth(double lineWidth) {
    this->lineWidth = lineWidth;
    pp->gobject_setLineWidth(this, lineWidth);
}

double GObject::getLineWidth() const {
    return lineWidth;
}

void GObject::setColor(std::string color) {
    setColor(convertColorToRGB(color));
}

void GObject::setColor(int rgb) {
    this->color = convertRGBToColor(rgb);
    pp->gobject_setColor(this, this->color);
}

std::string GObject::getColor() const {
    return color;
}

void GObject::scale(double sf) {
    scale(sf, sf);
}

void GObject::scale(double sx, double sy) {
    // Apply local transform
    transformed = true;
    pp->gobject_scale(this, sx, sy);
}

void GObject::rotate(double theta) {
    // Apply local transform
    transformed = true;
    pp->gobject_rotate(this, theta);
}

void GObject::setVisible(bool flag) {
    visible = flag;
    pp->gobject_setVisible(this, flag);
}

bool GObject::isVisible() const {
    return visible;
}

void GObject::sendForward() {
    GCompound *parent = getParent();
    if (parent != NULL) parent->sendForward(this);
}

void GObject::sendToFront() {
    GCompound *parent = getParent();
    if (parent != NULL) parent->sendToFront(this);
}

void GObject::sendBackward() {
    GCompound *parent = getParent();
    if (parent != NULL) parent->sendBackward(this);
}

void GObject::sendToBack() {
    GCompound *parent = getParent();
    if (parent != NULL) parent->sendToBack(this);
}

bool GObject::contains(GPoint pt) const {
    return contains(pt.getX(), pt.getY());
}

bool GObject::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
    return getBounds().contains(x, y);
}

GCompound *GObject::getParent() const {
    return parent;
}

GObject::GObject() {
    x = 0;
    y = 0;
    color = "";
    lineWidth = 1.0;
    transformed = false;
    visible = true;
    parent = NULL;
}

GObject::~GObject() {
    pp->gobject_delete(this);
}

/*
 * Implementation notes: GRect class
 * ---------------------------------
 * The GRect class is the most straightforward of the shape classes.
 */

GRect::GRect(double width, double height) {
    createGRect(width, height);
}

GRect::GRect(double x, double y, double width, double height) {
    createGRect(width, height);
    setLocation(x, y);
}

GRect::~GRect() {
    /* Empty */
}

void GRect::setSize(const GDimension & size) {
    setSize(size.getWidth(), size.getHeight());
}

void GRect::setSize(double width, double height) {
    if (transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    pp->gobject_setSize(this, width, height);
}

void GRect::setBounds(const GRectangle & bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void GRect::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

GRectangle GRect::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    return GRectangle(x, y, width, height);
}

void GRect::setFilled(bool flag) {
    fillFlag = true;
    pp->gobject_setFilled(this, flag);
}

bool GRect::isFilled() const {
    return fillFlag;
}

void GRect::setFillColor(std::string color) {
    fillColor = color;
    if (fillColor != "") {
        fillColor = convertRGBToColor(convertColorToRGB(color));
    }
    pp->gobject_setFillColor(this, fillColor);
}

void GRect::setFillColor(int rgb) {
    fillColor = convertRGBToColor(rgb);
    pp->gobject_setFillColor(this, fillColor);
}

std::string GRect::getFillColor() const {
    return fillColor;
}

std::string GRect::getType() const {
    return "GRect";
}

std::string GRect::toString() const {
    std::ostringstream oss;
    oss << "GRect(" << x << ", " << y << ", "
        << width << ", " << height << ")";
    return oss.str();
}

GRect::GRect() : width(0), height(0), fillFlag(false) {
    /* Called only by the GRoundRect and G3DRect subclasses */
}

void GRect::createGRect(double width, double height) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    fillFlag = false;
    fillColor = "";
    pp->grect_constructor(this, width, height);
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

std::string GRoundRect::getType() const {
    return "GRoundRect";
}

std::string GRoundRect::toString() const {
    std::ostringstream oss;
    oss << "GRoundRect(" << x << ", " << y << ", "
        << width << ", " << height << ", " << corner << ")";
    return oss.str();
}

void GRoundRect::createGRoundRect(double width, double height, double corner) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    this->corner = corner;
    fillFlag = false;
    fillColor = "";
    pp->groundrect_constructor(this, width, height, corner);
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

void G3DRect::setRaised(bool raised) {
    this->raised = raised;
    pp->g3drect_setRaised(this, raised);
}

bool G3DRect::isRaised() const {
    return raised;
}

std::string G3DRect::getType() const {
    return "G3DRect";
}

std::string G3DRect::toString() const {
    std::ostringstream oss;
    oss << std::boolalpha << "G3DRect(" << x << ", " << y << ", "
        << width << ", " << height << ", " << raised << ")";
    return oss.str();
}

void G3DRect::createG3DRect(double width, double height, bool raised) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    this->raised = raised;
    fillFlag = false;
    fillColor = "";
    pp->g3drect_constructor(this, width, height, raised);
}

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

void GOval::setSize(const GDimension & size) {
    setSize(size.getWidth(), size.getHeight());
}

void GOval::setSize(double width, double height) {
    if (transformed) error("setSize: Object has been transformed");
    this->width = width;
    this->height = height;
    pp->gobject_setSize(this, width, height);
}

void GOval::setBounds(const GRectangle & bounds) {
    setLocation(bounds.getX(), bounds.getY());
    setSize(bounds.getWidth(), bounds.getHeight());
}

void GOval::setBounds(double x, double y, double width, double height) {
    setLocation(x, y);
    setSize(width, height);
}

GRectangle GOval::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    return GRectangle(x, y, width, height);
}

bool GOval::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
    double rx = width / 2;
    double ry = height / 2;
    if (rx == 0 || ry == 0) return false;
    double dx = x - (this->x + rx);
    double dy = y - (this->y + ry);
    return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void GOval::setFilled(bool flag) {
    fillFlag = true;
    pp->gobject_setFilled(this, flag);
}

bool GOval::isFilled() const {
    return fillFlag;
}

void GOval::setFillColor(std::string color) {
    fillColor = color;
    if (fillColor != "") {
        fillColor = convertRGBToColor(convertColorToRGB(color));
    }
    pp->gobject_setFillColor(this, fillColor);
}

void GOval::setFillColor(int color) {
    fillColor = convertRGBToColor(color);
    pp->gobject_setFillColor(this, fillColor);
}

std::string GOval::getFillColor() const {
    return fillColor;
}

std::string GOval::getType() const {
    return "GOval";
}

std::string GOval::toString() const {
    std::ostringstream oss;
    oss << "GOval(" << x << ", " << y << ", "
        << width << ", " << height << ")";
    return oss.str();
}

void GOval::createGOval(double width, double height) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    fillFlag = false;
    fillColor = "";
    pp->goval_constructor(this, width, height);
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

void GArc::setStartAngle(double start) {
    this->start = start;
    pp->garc_setStartAngle(this, start);
}

double GArc::getStartAngle() const {
    return start;
}

void GArc::setSweepAngle(double sweep) {
    this->sweep = sweep;
    pp->garc_setSweepAngle(this, sweep);
}

double GArc::getSweepAngle() const {
    return sweep;
}

GPoint GArc::getStartPoint() const {
    return getArcPoint(start);
}

GPoint GArc::getEndPoint() const {
    return getArcPoint(start + sweep);
}

void GArc::setFrameRectangle(const GRectangle & rect) {
    setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(),
                      rect.getHeight());
}

void GArc::setFrameRectangle(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    frameWidth = width;
    frameHeight = height;
    pp->garc_setFrameRectangle(this, x, y, width, height);
}

GRectangle GArc::getFrameRectangle() const {
    return GRectangle(0, 0, 0, 0);
}

void GArc::setFilled(bool flag) {
    fillFlag = true;
    pp->gobject_setFilled(this, flag);
}

bool GArc::isFilled() const {
    return fillFlag;
}

void GArc::setFillColor(std::string color) {
    fillColor = color;
    if (fillColor != "") {
        fillColor = convertRGBToColor(convertColorToRGB(color));
    }
    pp->gobject_setFillColor(this, fillColor);
}

void GArc::setFillColor(int color) {
    fillColor = convertRGBToColor(color);
    pp->gobject_setFillColor(this, fillColor);
}

std::string GArc::getFillColor() const {
    return fillColor;
}

GRectangle GArc::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
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

bool GArc::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    if (rx == 0 || ry == 0) return false;
    double dx = x - (this->x + rx);
    double dy = y - (this->y + ry);
    double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    if (fillFlag) {
        if (r > 1.0) return false;
    } else {
        double t = ARC_TOLERANCE / ((rx + ry) / 2);
        if (abs(1.0 - r) > t) return false;
    }
    return containsAngle(atan2(-dy, dx) * 180 / PI);
}

std::string GArc::getType() const {
    return "GArc";
}

std::string GArc::toString() const {
    std::ostringstream oss;
    oss << "GArc(" << x << ", " << y << ", " << frameWidth << ", "
        << frameHeight << ", " << start << ", " << sweep << ")";
    return oss.str();
}

GPoint GArc::getArcPoint(double theta) const {
    double rx = frameWidth / 2;
    double ry = frameHeight / 2;
    double cx = x + rx;
    double cy = y + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
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
    pp->garc_constructor(this, width, height, start, sweep);
}

GCompound::GCompound() {
    pp->gcompound_constructor(this);
}

void GCompound::add(GObject *gobj) {
    pp->gcompound_add(this, gobj);
    contents.add(gobj);
    gobj->parent = this;
}

void GCompound::add(GObject *gobj, double x, double y) {
    gobj->setLocation(x, y);
    add(gobj);
}

void GCompound::remove(GObject *gobj) {
    int index = findGObject(gobj);
    if (index != -1) removeAt(index);
}

void GCompound::removeAll() {
    while (!contents.isEmpty()) {
        removeAt(0);
    }
}

int GCompound::getElementCount() {
    return contents.size();
}

GObject *GCompound::getElement(int index) {
    return contents.get(index);
}

GRectangle GCompound::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    double xMin = +1E20;
    double yMin = +1E20;
    double xMax = -1E20;
    double yMax = -1E20;
    for (int i = 0; i < contents.size(); i++) {
        GRectangle bounds = contents.get(i)->getBounds();
        xMin = std::min(xMin, bounds.getX());
        yMin = std::min(yMin, bounds.getY());
        xMax = std::max(xMax, bounds.getX());
        yMin = std::min(yMax, bounds.getY());
    }
    return GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

bool GCompound::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
    for (int i = 0; i < contents.size(); i++) {
        if (contents.get(i)->contains(x, y)) return true;
    }
    return false;
}

std::string GCompound::getType() const {
    return "GCompound";
}

std::string GCompound::toString() const {
    return "GCompound(...)";
}

void GCompound::sendForward(GObject *gobj) {
    int index = findGObject(gobj);
    if (index == -1) return;
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.insert(index + 1, gobj);
        pp->gobject_sendForward(gobj);
    }
}

void GCompound::sendToFront(GObject *gobj) {
    int index = findGObject(gobj);
    if (index == -1) return;
    if (index != contents.size() - 1) {
        contents.remove(index);
        contents.add(gobj);
        pp->gobject_sendToFront(gobj);
    }
}

void GCompound::sendBackward(GObject *gobj) {
    int index = findGObject(gobj);
    if (index == -1) return;
    if (index != 0) {
        contents.remove(index);
        contents.insert(index - 1, gobj);
        pp->gobject_sendBackward(gobj);
    }
}

void GCompound::sendToBack(GObject *gobj) {
    int index = findGObject(gobj);
    if (index == -1) return;
    if (index != 0) {
        contents.remove(index);
        contents.insert(0, gobj);
        pp->gobject_sendToBack(gobj);
    }
}

int GCompound::findGObject(GObject *gobj) {
    int n = contents.size();
    for (int i = 0; i < n; i++) {
        if (contents.get(i) == gobj) return i;
    }
    return -1;
}

void GCompound::removeAt(int index) {
    GObject* gobj = contents[index];
    contents.remove(index);
    pp->gobject_remove(gobj);
    gobj->parent = NULL;
}

GImage::GImage(std::string filename) {
    createGImage(filename);
}

GImage::GImage(std::string filename, double x, double y) {
    createGImage(filename);
    setLocation(x, y);
}

GRectangle GImage::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    return GRectangle(x, y, width, height);
}

std::string GImage::getType() const {
    return "GImage";
}

std::string GImage::toString() const {
    return "GImage(\"" + filename + "\")";
}

void GImage::createGImage(std::string filename) {
    this->filename = filename;
    GDimension size = pp->gimage_constructor(this, filename);
    width = size.getWidth();
    height = size.getHeight();
}

/*
 * Implementation notes: GLabel class
 * ----------------------------------
 */

GLabel::GLabel(std::string str) {
    createGLabel(str);
}

GLabel::GLabel(std::string str, double x, double y) {
    createGLabel(str);
    setLocation(x, y);
}

void GLabel::createGLabel(const std::string& str) {
    this->str = str;
    pp->glabel_constructor(this, str);
    setFont(DEFAULT_GLABEL_FONT);
    GDimension size = pp->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = pp->glabel_getFontAscent(this);
    descent = pp->glabel_getFontDescent(this);
}

void GLabel::setFont(std::string font) {
    this->font = font;
    pp->glabel_setFont(this, font);
    GDimension size = pp->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
    ascent = pp->glabel_getFontAscent(this);
    descent = pp->glabel_getFontDescent(this);
}

std::string GLabel::getFont() const {
    return font;
}

void GLabel::setLabel(std::string str) {
    this->str = str;
    pp->glabel_setLabel(this, str);
    GDimension size = pp->glabel_getSize(this);
    width = size.getWidth();
    height = size.getHeight();
}

std::string GLabel::getLabel() const {
    return str;
}

double GLabel::getFontAscent() const {
    return ascent;
}

double GLabel::getFontDescent() const {
    return descent;
}

GRectangle GLabel::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    return GRectangle(x, y - ascent, width, height);
}

std::string GLabel::getType() const {
    return "GLabel";
}

std::string GLabel::toString() const {
    return "GLabel(\"" + str + "\")";
}

/*
 * Implementation notes: GLine class
 * ---------------------------------
 */

GLine::GLine(double x0, double y0, double x1, double y1) {
    pp->gline_constructor(this, x0, y0, x1, y1);
    x = x0;
    y = y0;
    dx = x1 - x0;
    dy = y1 - y0;
}

void GLine::setStartPoint(double x, double y) {
    dx += this->x - x;
    dy += this->y - y;
    this->x = x;
    this->y = y;
    pp->gline_setStartPoint(this, x, y);
}

GPoint GLine::getStartPoint() const {
    return GPoint(x, y);
}

void GLine::setEndPoint(double x, double y) {
    dx = x - this->x;
    dy = y - this->y;
    pp->gline_setEndPoint(this, x, y);
}

GPoint GLine::getEndPoint() const {
    return GPoint(x + dx, y + dy);
}

GRectangle GLine::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
    double x0 = (dx < 0) ? x + dx : x;
    double y0 = (dy < 0) ? y + dy : y;
    return GRectangle(x0, y0, abs(dx), abs(dy));
}

bool GLine::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
    double x0 = getX();
    double y0 = getY();
    double x1 = x0 + dx;
    double y1 = y0 + dy;
    double tSquared = LINE_TOLERANCE * LINE_TOLERANCE;
    if (dsq(x, y, x0, y0) < tSquared) return true;
    if (dsq(x, y, x1, y1) < tSquared) return true;
    if (x < std::min(x0, x1) - LINE_TOLERANCE) return false;
    if (x > std::max(x0, x1) + LINE_TOLERANCE) return false;
    if (y < std::min(y0, y1) - LINE_TOLERANCE) return false;
    if (y > std::max(y0, y1) + LINE_TOLERANCE) return false;
    if ((float) (x0 - x1) == 0 && (float) (y0 - y1) == 0) return false;
    double u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0))
            / dsq(x0, y0, x1, y1);
    return dsq(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tSquared;
}

std::string GLine::getType() const {
    return "GLine";
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
    pp->gpolygon_constructor(this);
}

void GPolygon::addVertex(double x, double y) {
    cx = x;
    cy = y;
    vertices.add(GPoint(cx, cy));
    pp->gpolygon_addVertex(this, cx, cy);
}

void GPolygon::addEdge(double dx, double dy) {
    addVertex(cx + dx, cy + dy);
}

void GPolygon::addPolarEdge(double r, double theta) {
    addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

Vector<GPoint> GPolygon::getVertices() const {
    return vertices;
}

void GPolygon::setFilled(bool flag) {
    fillFlag = true;
    pp->gobject_setFilled(this, flag);
}

bool GPolygon::isFilled() const {
    return fillFlag;
}

void GPolygon::setFillColor(std::string color) {
    fillColor = color;
    if (fillColor != "") {
        fillColor = convertRGBToColor(convertColorToRGB(color));
    }
    pp->gobject_setFillColor(this, fillColor);
}

void GPolygon::setFillColor(int rgb) {
    fillColor = convertRGBToColor(rgb);
    pp->gobject_setFillColor(this, fillColor);
}

std::string GPolygon::getFillColor() const {
    return fillColor;
}

GRectangle GPolygon::getBounds() const {
    if (transformed) return pp->gobject_getBounds(this);
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
    return GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

bool GPolygon::contains(double x, double y) const {
    if (transformed) return pp->gobject_contains(this, x, y);
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

std::string GPolygon::getType() const {
    return "GPolygon";
}

std::string GPolygon::toString() const {
    std::ostringstream oss;
    oss << "GPolygon(" << vertices.size() << " vertices)";
    return oss.str();
}

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
