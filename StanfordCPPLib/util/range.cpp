/*
 * File: range.cpp
 * ---------------
 * This file implements the <code>Range</code> class.
 * See range.h for the declarations of each member.
 *
 * @version 2018/03/12
 * - initial version
 */

#include "range.h"
#include <sstream>
#include "hashcode.h"

Range::Range(int length)
    : _min(0),
      _max(length - 1) {
    // empty
}

Range::Range(int minValue, int maxValue)
    : _min(minValue),
      _max(maxValue) {
    // empty
}

Range::RangeIterator Range::begin() const {
    return RangeIterator(this, /* end */ false);
}

bool Range::contains(int n) const {
    return _min <= n && n <= _max;
}

bool Range::contains(const Range& r) const {
    return _min <= r._min && _max >= r._max;
}

bool Range::empty() const {
    return _max < _min;
}

Range::RangeIterator Range::end() const {
    return RangeIterator(this, /* end */ true);
}

//bool Range::intersects(const Range& r) const {
//    return (_min <= r._min && r._min <= _max)
//            || (_min <= r._max && r._max <= _max);
//}

bool Range::isEmpty() const {
    return _max < _min;
}

int Range::length() const {
    if (isEmpty()) {
        return 0;
    } else {
        return _max - _min + 1;
    }
}

int Range::max() const {
    return _max;
}

int Range::min() const {
    return _min;
}

int Range::size() const {
    return length();
}

std::string Range::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const Range& r) {
    return hashCode2(r.min(), r.max());
}

Range range(int length) {
    return Range(length);
}

Range range(int min, int max) {
    return Range(min, max);
}

bool operator <(const Range& r1, const Range& r2) {
    return r1.min() < r2.min() ||
            (r1.min() == r2.min() && r1.max() < r2.max());
}

bool operator <=(const Range& r1, const Range& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const Range& r1, const Range& r2) {
    return r1.min() == r2.min() && r1.max() == r2.max();
}

bool operator !=(const Range& r1, const Range& r2) {
    return !(r1 == r2);
}

bool operator >(const Range& r1, const Range& r2) {
    return r2 < r1;
}

bool operator >=(const Range& r1, const Range& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const Range& r) {
    return out << "[" << r.min() << " .. " << r.max() << "]";
}

std::istream& operator >>(std::istream& input, Range& r) {
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
// Range2D
/////////////////////////////////////////////////////////////////////

Range2D::Range2D(int width, int height, bool yMajor)
    : _minX(0),
      _minY(0),
      _maxX(width - 1),
      _maxY(height - 1),
      _yMajor(yMajor) {
    // empty
}

Range2D::Range2D(int minX, int minY, int maxX, int maxY, bool yMajor)
    : _minX(minX),
      _minY(minY),
      _maxX(maxX),
      _maxY(maxY),
      _yMajor(yMajor) {
    // empty
}

Range2D::Range2DIterator Range2D::begin() const {
    return Range2DIterator(this, /* end */ false);
}

bool Range2D::contains(int x, int y) const {
    return _minX <= x && x <= _maxX
            && _minY <= y && y <= _maxY;
}

bool Range2D::contains(const Range2D& r) const {
    return contains(r._minX, r._minY) && contains(r._maxX, r._maxY);
}

bool Range2D::empty() const {
    return _maxX < _minX || _maxY < _minY;
}

Range2D::Range2DIterator Range2D::end() const {
    return Range2DIterator(this, /* end */ true);
}

int Range2D::height() const {
    if (_maxY < _minY) {
        return 0;
    } else {
        return _maxY - _minY + 1;
    }
}

//bool Range2D::intersects(const Range2D& r) const {
//    // TODO
//}

bool Range2D::isEmpty() const {
    return _maxX < _minX || _maxY < _minY;
}

bool Range2D::isYMajor() const {
    return _yMajor;
}

int Range2D::maxX() const {
    return _maxX;
}

int Range2D::maxY() const {
    return _maxY;
}

int Range2D::minX() const {
    return _minX;
}

int Range2D::minY() const {
    return _minY;
}

int Range2D::size() const {
    return width() * height();
}

std::string Range2D::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int Range2D::width() const {
    if (_maxX < _minX) {
        return 0;
    } else {
        return _maxX - _minX + 1;
    }
}

int hashCode(const Range2D& r) {
    return hashCode4(r.minX(), r.minY(), r.maxX(), r.maxY());
}

Range2D range2d(int width, int height, bool yMajor) {
    return Range2D(width, height, yMajor);
}

Range2D range2d(int minX, int minY, int maxX, int maxY, bool yMajor) {
    return Range2D(minX, minY, maxX, maxY, yMajor);
}

bool operator <(const Range2D& r1, const Range2D& r2) {
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

bool operator <=(const Range2D& r1, const Range2D& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const Range2D& r1, const Range2D& r2) {
    return r1.minX() == r2.minX()
            && r1.minY() == r2.minY()
            && r1.maxX() == r2.maxX()
            && r1.maxY() == r2.maxY()
            && r1.isYMajor() == r2.isYMajor();
}

bool operator !=(const Range2D& r1, const Range2D& r2) {
    return !(r1 == r2);
}

bool operator >(const Range2D& r1, const Range2D& r2) {
    return r2 < r1;
}

bool operator >=(const Range2D& r1, const Range2D& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const Range2D& r) {
    return out << "["
               << r.minX() << "," << r.minY()
               << " .. "
               << r.maxX() << "," << r.maxY()
               << "]";
}

std::istream& operator >>(std::istream& input, Range2D& r) {
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
