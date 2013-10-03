/*
 * File: point.cpp
 * ---------------
 * This file implements the point.h interface.
 */

#include <string>
#include "point.h"
#include "strlib.h"
using namespace std;

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

string Point::toString() const {
   return "(" + integerToString(x) + "," + integerToString(y) + ")";
}

bool Point::operator==(const Point & p2) const {
   return (x == p2.x) && (y == p2.y);
}

bool Point::operator!=(const Point & p2) const {
   return (x != p2.x) || (y != p2.y);
}

ostream & operator<<(ostream & os, const Point & pt) {
   return os << pt.toString();
}
