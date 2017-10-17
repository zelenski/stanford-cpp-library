/*
 * File: gtypes.cpp
 * ----------------
 * This file implements the classes in the gtypes.h interface.
 * 
 * @version 2017/10/16
 * - added GPoint operators *, *=
 * - added GPoint::toPolar
 * - added better GPoint support in GRectangle
 * - alphabetized members
 * @version 2017/09/29
 * - updated to use composite hashCode function
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/10/08
 * - removed 'using namespace' statement
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

GDimension::GDimension() {
    width = 0;
    height = 0;
}

GDimension::GDimension(double width, double height) {
    this->width = width;
    this->height = height;
}

double GDimension::getWidth() const {
    return width;
}

double GDimension::getHeight() const {
    return height;
}

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

GDimension& operator *=(GDimension& d, double scale) {
    d.width *= scale;
    d.height *= scale;
    return d;
}

int hashCode(const GDimension& dim) {
    return hashCode2(dim.width, dim.height);
}

/*
 * Implementation notes: GPoint class
 * ----------------------------------
 * The GPoint class itself is entirely straightforward.  The relational
 * operators compare the x components first, followed by the y component.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GPoint::GPoint() {
    x = 0;
    y = 0;
}

GPoint::GPoint(double x, double y) {
    this->x = x;
    this->y = y;
}

double GPoint::getX() const {
    return x;
}

double GPoint::getY() const {
    return y;
}

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

GPoint& operator *=(GPoint& p, double scale) {
    p.x *= scale;
    p.y *= scale;
    return p;
}


int hashCode(const GPoint& pt) {
    return hashCode2(pt.x, pt.y);
}

/*
 * Implementation notes: GRectangle class
 * --------------------------------------
 * The GRectangle class itself is entirely straightforward.  The
 * relational operators compare the components in the following order:
 * x, y, width, height.  The hashCode function computes the exclusive-or
 * of the individual words.
 */

GRectangle::GRectangle() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

GRectangle::GRectangle(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

GRectangle::GRectangle(double x, double y, const GDimension& size) {
    this->x = x;
    this->y = y;
    this->width = size.getWidth();
    this->height = size.getHeight();
}

GRectangle::GRectangle(const GPoint& p, double width, double height) {
    this->x = p.getX();
    this->y = p.getY();
    this->width = width;
    this->height = height;
}

GRectangle::GRectangle(const GPoint& p, const GDimension& size) {
    this->x = p.getX();
    this->y = p.getY();
    this->width = size.getWidth();
    this->height = size.getHeight();
}

double GRectangle::getX() const {
    return x;
}

double GRectangle::getY() const {
    return y;
}

double GRectangle::getWidth() const {
    return width;
}

double GRectangle::getHeight() const {
    return height;
}

bool GRectangle::isEmpty() const {
    return width <= 0 || height <= 0;
}

bool GRectangle::contains(double x, double y) const {
    return x >= this->x && y >= this->y
            && x < this->x + width
            && y < this->y + height;
}

bool GRectangle::contains(const GPoint& pt) const {
    return contains(pt.getX(), pt.getY());
}

std::string GRectangle::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GRectangle& rect) {
    return os << "(" << rect.getX() << ", " << rect.getY() << ", "
              << rect.getWidth() << ", " << rect.getHeight() << ")";
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
    return stanfordcpplib::collections::compareTo4(
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
    return hashCode4(r.x, r.y, r.width, r.height);
}
