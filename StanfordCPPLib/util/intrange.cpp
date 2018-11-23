/*
 * File: intrange.cpp
 * ------------------
 * This file implements the <code>IntRange</code> class.
 * See intrange.h for the declarations of each member.
 *
 * @version 2018/08/25
 * - renamed from range.cpp to intrange.cpp
 * @version 2018/03/12
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "intrange.h"
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

IntRange::IntRange(int length)
    : _min(0),
      _max(length - 1) {
    // empty
}

IntRange::IntRange(int minValue, int maxValue)
    : _min(minValue),
      _max(maxValue) {
    // empty
}

IntRange::IntRangeIterator IntRange::begin() const {
    return IntRangeIterator(this, /* end */ false);
}

bool IntRange::contains(int n) const {
    return _min <= n && n <= _max;
}

bool IntRange::contains(const IntRange& r) const {
    return _min <= r._min && _max >= r._max;
}

bool IntRange::empty() const {
    return _max < _min;
}

IntRange::IntRangeIterator IntRange::end() const {
    return IntRangeIterator(this, /* end */ true);
}

//bool IntRange::intersects(const IntRange& r) const {
//    return (_min <= r._min && r._min <= _max)
//            || (_min <= r._max && r._max <= _max);
//}

bool IntRange::isEmpty() const {
    return _max < _min;
}

int IntRange::length() const {
    if (isEmpty()) {
        return 0;
    } else {
        return _max - _min + 1;
    }
}

int IntRange::max() const {
    return _max;
}

int IntRange::min() const {
    return _min;
}

int IntRange::size() const {
    return length();
}

std::string IntRange::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const IntRange& r) {
    return hashCode(r.min(), r.max());
}

IntRange range(int length) {
    return IntRange(length);
}

IntRange range(int min, int max) {
    return IntRange(min, max);
}

bool operator <(const IntRange& r1, const IntRange& r2) {
    return r1.min() < r2.min() ||
            (r1.min() == r2.min() && r1.max() < r2.max());
}

bool operator <=(const IntRange& r1, const IntRange& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const IntRange& r1, const IntRange& r2) {
    return r1.min() == r2.min() && r1.max() == r2.max();
}

bool operator !=(const IntRange& r1, const IntRange& r2) {
    return !(r1 == r2);
}

bool operator >(const IntRange& r1, const IntRange& r2) {
    return r2 < r1;
}

bool operator >=(const IntRange& r1, const IntRange& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const IntRange& r) {
    return out << "[" << r.min() << " .. " << r.max() << "]";
}

std::istream& operator >>(std::istream& input, IntRange& r) {
    // read '['
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read min
    int min;
    if (!(input >> min)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ' .. '
    input.get();
    input.get();
    input.get();
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read max
    int max;
    if (!(input >> max)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ']'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    r._min = min;
    r._max = max;
    return input;
}

/////////////////////////////////////////////////////////////////////
// IntRange2D
/////////////////////////////////////////////////////////////////////

IntRange2D::IntRange2D(int width, int height, bool yMajor)
    : _minX(0),
      _minY(0),
      _maxX(width - 1),
      _maxY(height - 1),
      _yMajor(yMajor) {
    // empty
}

IntRange2D::IntRange2D(int minX, int minY, int maxX, int maxY, bool yMajor)
    : _minX(minX),
      _minY(minY),
      _maxX(maxX),
      _maxY(maxY),
      _yMajor(yMajor) {
    // empty
}

IntRange2D::IntRange2DIterator IntRange2D::begin() const {
    return IntRange2DIterator(this, /* end */ false);
}

bool IntRange2D::contains(int x, int y) const {
    return _minX <= x && x <= _maxX
            && _minY <= y && y <= _maxY;
}

bool IntRange2D::contains(const IntRange2D& r) const {
    return contains(r._minX, r._minY) && contains(r._maxX, r._maxY);
}

bool IntRange2D::empty() const {
    return _maxX < _minX || _maxY < _minY;
}

IntRange2D::IntRange2DIterator IntRange2D::end() const {
    return IntRange2DIterator(this, /* end */ true);
}

int IntRange2D::height() const {
    if (_maxY < _minY) {
        return 0;
    } else {
        return _maxY - _minY + 1;
    }
}

//bool IntRange2D::intersects(const IntRange2D& r) const {
//    // TODO
//}

bool IntRange2D::isEmpty() const {
    return _maxX < _minX || _maxY < _minY;
}

bool IntRange2D::isYMajor() const {
    return _yMajor;
}

int IntRange2D::maxX() const {
    return _maxX;
}

int IntRange2D::maxY() const {
    return _maxY;
}

int IntRange2D::minX() const {
    return _minX;
}

int IntRange2D::minY() const {
    return _minY;
}

int IntRange2D::size() const {
    return width() * height();
}

std::string IntRange2D::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int IntRange2D::width() const {
    if (_maxX < _minX) {
        return 0;
    } else {
        return _maxX - _minX + 1;
    }
}

int hashCode(const IntRange2D& r) {
    return hashCode(r.minX(), r.minY(), r.maxX(), r.maxY());
}

IntRange2D range2d(int width, int height, bool yMajor) {
    return IntRange2D(width, height, yMajor);
}

IntRange2D range2d(int minX, int minY, int maxX, int maxY, bool yMajor) {
    return IntRange2D(minX, minY, maxX, maxY, yMajor);
}

bool operator <(const IntRange2D& r1, const IntRange2D& r2) {
    if (r1.minX() != r2.minX()) {
        return r1.minX() < r2.minX();
    } else if (r1.minY() != r2.minY()) {
        return r1.minY() < r2.minY();
    } else if (r1.maxX() != r2.maxX()) {
        return r1.maxX() < r2.maxX();
    } else if (r1.maxY() != r2.maxY()) {
        return r1.maxY() < r2.maxY();
    } else if (r1.isYMajor() != r2.isYMajor()) {
        return (int) r1.isYMajor() < (int) r2.isYMajor();
    } else {
        return false;
    }
}

bool operator <=(const IntRange2D& r1, const IntRange2D& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const IntRange2D& r1, const IntRange2D& r2) {
    return r1.minX() == r2.minX()
            && r1.minY() == r2.minY()
            && r1.maxX() == r2.maxX()
            && r1.maxY() == r2.maxY()
            && r1.isYMajor() == r2.isYMajor();
}

bool operator !=(const IntRange2D& r1, const IntRange2D& r2) {
    return !(r1 == r2);
}

bool operator >(const IntRange2D& r1, const IntRange2D& r2) {
    return r2 < r1;
}

bool operator >=(const IntRange2D& r1, const IntRange2D& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const IntRange2D& r) {
    return out << "["
               << r.minX() << "," << r.minY()
               << " .. "
               << r.maxX() << "," << r.maxY()
               << "]";
}

std::istream& operator >>(std::istream& input, IntRange2D& r) {
    // read '['
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read minX
    int minX;
    if (!(input >> minX)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ','
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read minY
    int minY;
    if (!(input >> minY)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ' .. '
    input.get();
    input.get();
    input.get();
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read maxX
    int maxX;
    if (!(input >> maxX)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ','
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read maxY
    int maxY;
    if (!(input >> maxY)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ']'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    r._minX = minX;
    r._minY = minY;
    r._maxX = maxX;
    r._maxY = maxY;
    return input;
}
