/*
 * File: gtypes.cpp
 * ----------------
 * This file implements the classes in the gtypes.h interface.
 *
 * @version 2019/05/16
 * - added GRectangle contains(GRectangle), intersects
 * @version 2018/07/14
 * - initial version, based on gtypes.cpp
 */

#include "gtypes.h"
#include <cmath>
#include <sstream>
#include <string>
#include "collections.h"
#include "error.h"
#include "gmath.h"
#include "hashcode.h"
#include "strlib.h"

/*
 * Implementation notes: GDimension class
 * --------------------------------------
 * The GDimension class itself is entirely straightforward.  The
 * relational operators compare the width first, followed by the height.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GDimension::GDimension(double w, double h)
        : width(w),
          height(h) {
    // empty
}

GDimension::GDimension() : GDimension(0, 0) {}

std::string GDimension::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GDimension& dim) {
    return os << "(" << dim.width << ", " << dim.height << ")";
}

bool operator ==(const GDimension& d1, const GDimension& d2) {
    return floatingPointEqual(d1.width, d2.width)
            && floatingPointEqual(d1.height, d2.height);
}

bool operator !=(const GDimension& d1, const GDimension& d2) {
    return !(d1 == d2);
}

bool operator <(const GDimension& d1, const GDimension& d2) {
    return d1.width < d2.width
            || (floatingPointEqual(d1.width, d2.width) && d1.height < d2.height);
}

bool operator <=(const GDimension& d1, const GDimension& d2) {
    return d1 < d2 || d1 == d2;
}

bool operator >(const GDimension& d1, const GDimension& d2) {
    return d2 < d1;
}

bool operator >=(const GDimension& d1, const GDimension& d2) {
    return d1 > d2 || d1 == d2;
}

GDimension operator *(const GDimension& d, double scale) {
    return GDimension(d.width * scale, d.height * scale);
}

int hashCode(const GDimension& dim) {
    return hashCode(dim.width, dim.height);
}

std::string toString(HorizontalAlignment alignment) {
    if (alignment == ALIGN_CENTER) {
        return "Center";
    } else if (alignment == ALIGN_LEFT) {
        return "Left";
    } else {   // if (alignment == ALIGN_RIGHT)
        return "Right";
    }
}

std::string toString(VerticalAlignment alignment) {
    if (alignment == ALIGN_MIDDLE) {
        return "Middle";
    } else if (alignment == ALIGN_TOP) {
        return "Top";
    } else {   // if (alignment == ALIGN_BOTTOM)
        return "Bottom";
    }
}

HorizontalAlignment toHorizontalAlignment(const std::string& alignmentStr) {
    std::string alignLC = toLowerCase(trim(alignmentStr));
    if (stringContains(alignLC, "left") || stringContains(alignLC, "west")) {
        return ALIGN_LEFT;
    } else if (stringContains(alignLC, "right") || stringContains(alignLC, "east")) {
        return ALIGN_RIGHT;
    } else if (stringContains(alignLC, "stretch") || stringContains(alignLC, "justify")) {
        return ALIGN_HORIZONTAL_STRETCH;
    } else {
        return ALIGN_CENTER;
    }
}

Qt::Alignment toQtAlignment(HorizontalAlignment alignment) {
    return alignment == ALIGN_LEFT ? Qt::AlignLeft
            : alignment == ALIGN_CENTER ? Qt::AlignHCenter
            : alignment == ALIGN_HORIZONTAL_STRETCH ? Qt::AlignJustify
            : Qt::AlignRight;
}

Qt::Alignment toQtAlignment(VerticalAlignment alignment) {
    return alignment == ALIGN_TOP ? Qt::AlignTop
            : alignment == ALIGN_MIDDLE ? Qt::AlignVCenter
            : alignment == ALIGN_VERTICAL_STRETCH ? Qt::AlignTop
            : Qt::AlignBottom;
}

VerticalAlignment toVerticalAlignment(const std::string& alignmentStr) {
    std::string alignLC = toLowerCase(trim(alignmentStr));
    if (stringContains(alignLC, "top") || stringContains(alignLC, "north")) {
        return ALIGN_TOP;
    } else if (stringContains(alignLC, "bottom") || stringContains(alignLC, "south")) {
        return ALIGN_BOTTOM;
    } else if (stringContains(alignLC, "stretch") || stringContains(alignLC, "justify")) {
        return ALIGN_VERTICAL_STRETCH;
    } else {
        return ALIGN_MIDDLE;
    }
}

/*
 * Implementation notes: GPoint class
 * ----------------------------------
 * The GPoint class itself is entirely straightforward.  The relational
 * operators compare the x components first, followed by the y component.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GPoint::GPoint(double xVal, double yVal)
        : x(xVal),
          y(yVal) {
    // empty
}

GPoint::GPoint() : GPoint(0, 0) {}

std::string GPoint::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GPoint& pt) {
    return os << "(" << pt.x << ", " << pt.y << ")";
}

bool operator ==(const GPoint& p1, const GPoint& p2) {
    return floatingPointEqual(p1.x, p2.x)
            && floatingPointEqual(p1.y, p2.y);
}

bool operator !=(const GPoint& p1, const GPoint& p2) {
    return !(p1 == p2);
}

bool operator <(const GPoint& p1, const GPoint& p2) {
    return p1.x < p2.x || (floatingPointEqual(p1.x, p2.x) && p1.y < p2.y);
}

bool operator <=(const GPoint& p1, const GPoint& p2) {
    return p1 < p2 || p1 == p2;
}

bool operator >(const GPoint& p1, const GPoint& p2) {
    return p2 < p1;
}

bool operator >=(const GPoint& p1, const GPoint& p2) {
    return p1 > p2 || p1 == p2;
}

GPoint operator *(const GPoint& p, double scale) {
    return GPoint(p.x * scale, p.y * scale);
}

int hashCode(const GPoint& pt) {
    return hashCode(pt.x, pt.y);
}

/*
 * Implementation notes: GRectangle class
 * --------------------------------------
 * The GRectangle class itself is entirely straightforward.  The
 * relational operators compare the components in the following order:
 * x, y, width, height.  The hashCode function computes the exclusive-or
 * of the individual words.
 */

GRectangle::GRectangle(double xVal, double yVal, double w, double h)
        : x(xVal),
          y(yVal),
          width(w),
          height(h) {
    // empty
}

GRectangle::GRectangle(double xVal, double yVal, const GDimension& size)
        : x(xVal),
          y(yVal),
          width(size.width),
          height(size.height) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, double w, double h)
        : x(p.x),
          y(p.y),
          width(w),
          height(h) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, const GDimension& size)
        : x(p.x),
          y(p.y),
          width(size.width),
          height(size.height) {
    // empty
}

bool GRectangle::contains(double x, double y) const {
    return x >= this->x && y >= this->y
            && x < this->x + width
            && y < this->y + height;
}

bool GRectangle::contains(const GPoint& pt) const {
    return contains(pt.x, pt.y);
}

bool GRectangle::contains(const GRectangle& rect) const {
    return contains(rect.x, rect.y)
            && contains(rect.x + rect.width - 1, rect.y + rect.height - 1);
}

GRectangle GRectangle::enlargedBy(double amount) {
    return GRectangle(x - amount, y - amount, width + 2 * amount, height + 2 * amount);
}

bool GRectangle::intersects(const GRectangle& other) const {
    // check for non-intersecting x coordinates
    return !(x + width < other.x            // this is entirely left of other
          || x > other.x + other.width      // this is entirely right of other
          || y + height < other.y           // this is entirely above other
          || y > other.y + other.height);   // this is entirely below other
}

bool GRectangle::isEmpty() const {
    return width <= 0 || height <= 0;
}

std::string GRectangle::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GRectangle& rect) {
    return os << "(" << rect.x << ", " << rect.y << ", "
              << rect.width << ", " << rect.height << ")";
}

bool operator ==(const GRectangle& r1, const GRectangle& r2) {
    return floatingPointEqual(r1.x, r2.x)
            && floatingPointEqual(r1.y, r2.y)
            && floatingPointEqual(r1.width, r2.width)
            && floatingPointEqual(r1.height, r2.height);
}

bool operator !=(const GRectangle& r1, const GRectangle& r2) {
    return !(r1 == r2);
}

bool operator <(const GRectangle& r1, const GRectangle& r2) {
    return stanfordcpplib::collections::compareTo(
            r1.x, r2.x, r1.y, r2.y, r1.width, r2.width, r1.height, r2.height) < 0;
}

bool operator <=(const GRectangle& r1, const GRectangle& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator >(const GRectangle& r1, const GRectangle& r2) {
    return r2 < r1;
}

bool operator >=(const GRectangle& r1, const GRectangle& r2) {
    return r1 > r2 || r1 == r2;
}

int hashCode(const GRectangle& r) {
    return hashCode(r.x, r.y, r.width, r.height);
}
