/*
 * File: point.cpp
 * ---------------
 * This file implements the point.h interface.
 * 
 * @version 2018/11/22
 * - added headless mode support
 * @version 2017/09/29
 * - updated to use composite hashCode function
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "point.h"
#include <string>
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

#ifndef SPL_HEADLESS_MODE
Point::Point(const GPoint& point) {
    this->x = (int) point.getX();
    this->y = (int) point.getY();
}
#endif // SPL_HEADLESS_MODE

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

std::string Point::toString() const {
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
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
    return hashCode(pt.getX(), pt.getY());
}
