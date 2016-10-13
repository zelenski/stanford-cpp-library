/*
 * File: point.cpp
 * ---------------
 * This file implements the point.h interface.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "point.h"
#include <string>
#include "strlib.h"

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

std::string Point::toString() const {
    return "(" + integerToString(x) + "," + integerToString(y) + ")";
}

bool Point::operator ==(const Point& p2) const {
    return (x == p2.x) && (y == p2.y);
}

bool Point::operator !=(const Point& p2) const {
    return (x != p2.x) || (y != p2.y);
}

std::ostream& operator <<(std::ostream& os, const Point& pt) {
    return os << pt.toString();
}

int hashCode(const Point& pt) {
    int hash = 0;
    hash += std::abs(pt.getX() & 0x7fff) << 16;
    hash += std::abs(pt.getY());
    return hash;
}
