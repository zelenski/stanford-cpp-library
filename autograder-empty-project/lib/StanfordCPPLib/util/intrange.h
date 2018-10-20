/*
 * File: intrange.h
 * ----------------
 * This file exports the <code>IntRange</code> class, which represents an iterable
 * contiguous range of integers that can be processed using a for-each loop.
 * The range is not stored all in memory, so it does not require O(N) space.
 *
 * See intrange.cpp for the implementation of each member.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2018/08/25
 * - renamed from range.h to intrange.h
 * @version 2018/03/12
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _intrange_h
#define _intrange_h

#include <iostream>
#include <iterator>
#include <limits>

#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "point.h"
#undef INTERNAL_INCLUDE

/**
 * An IntRange is an iterable contiguous range of integers.
 * It can be processed using a for-each loop.
 * The integers are not actually all stored in this object, so it does not
 * require O(N) memory usage.
 */
class IntRange {
private:
    /**
     * Internal iterator for looping over a range.
     */
    class IntRangeIterator : public std::iterator<std::random_access_iterator_tag, int> {
    private:
        const IntRange* r;
        int val;

    public:
        IntRangeIterator(const IntRange* r, bool end) {
            this->r = r;
            this->val = (end ? r->max() + 1 : r->min());
        }

        IntRangeIterator(const IntRangeIterator& itr)
            : r(itr.r),
              val(itr.val) {
            // empty
        }

        IntRangeIterator& operator ++() {
            val++;
            if (!r->contains(val)) {
                val = r->max() + 1;
            }
            return *this;
        }

        IntRangeIterator operator ++(int) {
            IntRangeIterator copy(*this);
            operator++();
            return copy;
        }

        IntRangeIterator& operator --() {
            val--;
            return *this;
        }

        IntRangeIterator operator --(int) {
            IntRangeIterator copy(*this);
            operator--();
            return copy;
        }

        bool operator ==(const IntRangeIterator& rhs) {
            return r == rhs.r && val == rhs.val;
        }

        bool operator !=(const IntRangeIterator& rhs) {
            return !(*this == rhs);
        }

        bool operator <(const IntRangeIterator& rhs) {
            if (r != rhs.r) {
                error("Range Iterator::operator <: Iterators are in different ranges");
            }
            return val < rhs.val;
        }

        bool operator <=(const IntRangeIterator& rhs) {
            if (r != rhs.r) {
                error("Range Iterator::operator <=: Iterators are in different ranges");
            }
            return val <= rhs.val;
        }

        bool operator >(const IntRangeIterator& rhs) {
            if (r != rhs.r) {
                error("Range Iterator::operator >: Iterators are in different ranges");
            }
            return val > rhs.val;
        }

        bool operator >=(const IntRangeIterator& rhs) {
            if (r != rhs.r) {
                error("Range Iterator::operator >=: Iterators are in different ranges");
            }
            return val >= rhs.val;
        }

        IntRangeIterator operator +(const int& rhs) {
            return IntRangeIterator(r, val + rhs);
        }

        IntRangeIterator operator +=(const int& rhs) {
            val += rhs;
            return *this;
        }

        IntRangeIterator operator -(const int& rhs) {
            return IntRangeIterator(r, val - rhs);
        }

        IntRangeIterator operator -=(const int& rhs) {
            val -= rhs;
            return *this;
        }

        int operator -(const IntRangeIterator& rhs) {
            if (r != rhs.r) {
                error("Range Iterator::operator -: Iterators are in different ranges");
            }
            return val - rhs.val;
        }

        int operator *() {
            return val;
        }

        int* operator ->() {
            return &val;
        }

        int operator [](int k) {
            return val + k;
        }
    };

    int _min;
    int _max;

    friend std::istream& operator >>(std::istream& input, IntRange& r);

public:
    /**
     * Constructs a range from 0 (inclusive) of the given length.
     * Its endpoints are [0 .. length-1].
     */
    IntRange(int length = 0);

    /**
     * Constructs a range from minValue to maxValue (inclusive).
     */
    IntRange(int minValue, int maxValue);

    /**
     * Returns an iterator over the elements of this range.
     */
    IntRangeIterator begin() const;

    /**
     * Returns true if the given integer is within the bounds of this range.
     */
    bool contains(int n) const;

    /**
     * Returns true if the given range is entirely contained within this range.
     */
    bool contains(const IntRange& r) const;

    /**
     * Returns true if this range contains no integers, meaning that its max
     * is less than its min.
     * Equivalent to isEmpty.
     */
    bool empty() const;

    IntRangeIterator end() const;

//    bool intersects(const Range& r) const;

    /**
     * Returns true if this range contains no integers, meaning that its max
     * is less than its min.
     * Equivalent to empty.
     */
    bool isEmpty() const;

    /**
     * Returns the number of integers in the range.
     * Equivalent to size.
     */
    int length() const;

    /**
     * Returns the maximum value in the range, inclusive.
     */
    int max() const;

    /**
     * Returns the minimum value in the range.
     */
    int min() const;

    /**
     * Returns the number of integers in the range.
     * Equivalent to length.
     */
    int size() const;

    /**
     * Returns a string representation of this range,
     * such as "[1 .. 10]".
     */
    std::string toString() const;
};

/**
 * Returns an integer hash code for this range.
 */
int hashCode(const IntRange& r);

/**
 * Convenience functions for constructing a range.
 * See Range constructor.
 */
IntRange range(int length = 0);
IntRange range(int min, int max);

/**
 * Relational operators for comparing ranges.
 * Ranges are ordered by their min values with ties broken by max values.
 */
bool operator <(const IntRange& r1, const IntRange& r2);
bool operator <=(const IntRange& r1, const IntRange& r2);
bool operator ==(const IntRange& r1, const IntRange& r2);
bool operator !=(const IntRange& r1, const IntRange& r2);
bool operator >(const IntRange& r1, const IntRange& r2);
bool operator >=(const IntRange& r1, const IntRange& r2);

/**
 * Writes the given range to the given output stream in its toString format.
 */
std::ostream& operator <<(std::ostream& out, const IntRange& r);

/**
 * Reads the given range to the given input stream in its toString format.
 */
std::istream& operator >>(std::istream& input, IntRange& r);


/////////////////////////////////////////////////////////////////////
// IntRange2D
/////////////////////////////////////////////////////////////////////

/**
 * An IntRange2D is an iterable contiguous 2-dimensional range of integers.
 * It can be processed using a for-each loop to emit the range as a series
 * of Point objects in y-major (default) or x-major order.
 * The points are not actually all stored in this object, so it does not
 * require O(WxH) memory usage.
 */
class IntRange2D {
private:
    /**
     * Internal iterator for looping over a 2D range.
     */
    class IntRange2DIterator : public std::iterator<std::input_iterator_tag, Point> {
    private:
        const IntRange2D* r;
        int x;
        int y;

    public:
        IntRange2DIterator(const IntRange2D* r, bool end) {
            this->r = r;
            if (end) {
                this->x = r->maxX() + 1;
                this->y = r->maxY() + 1;
            } else {
                this->x = r->minX();
                this->y = r->minY();
            }
        }

        IntRange2DIterator(const IntRange2DIterator& itr)
            : r(itr.r),
              x(itr.x),
              y(itr.y) {
            // empty
        }

        IntRange2DIterator& operator ++() {
            if (r->isYMajor()) {
                x++;
                if (x > r->maxX()) {
                    x = r->minX();
                    y++;
                }
            } else {
                y++;
                if (y > r->maxY()) {
                    y = r->minY();
                    x++;
                }
            }
            if (!r->contains(x, y)) {
                x = r->maxX() + 1;
                y = r->maxY() + 1;
            }
            return *this;
        }

        IntRange2DIterator operator ++(int) {
            IntRange2DIterator copy(*this);
            operator++();
            return copy;
        }

        IntRange2DIterator& operator --() {
            if (r->isYMajor()) {
                x--;
                if (x < r->minX()) {
                    x = r->maxX();
                    y--;
                }
            } else {
                y--;
                if (y < r->minY()) {
                    y = r->maxY();
                    x--;
                }
            }
            return *this;
        }

        IntRange2DIterator operator --(int) {
            IntRange2DIterator copy(*this);
            operator--();
            return copy;
        }

        bool operator ==(const IntRange2DIterator& rhs) const {
            return r == rhs.r && x == rhs.x && y == rhs.y;
        }

        bool operator !=(const IntRange2DIterator& rhs) const {
            return !(*this == rhs);
        }

        bool operator <(const IntRange2DIterator& rhs) const {
            if (r != rhs.r) {
                error("Range2D Iterator::operator <: Iterators are in different ranges");
            }
            return x < rhs.x || (x == rhs.x && y < rhs.y);
        }

        bool operator <=(const IntRange2DIterator& rhs) const {
            if (r != rhs.r) {
                error("Range2D Iterator::operator <=: Iterators are in different ranges");
            }
            return (*this < rhs) || (*this == rhs);
        }

        bool operator >(const IntRange2DIterator& rhs) const {
            if (r != rhs.r) {
                error("Range2D Iterator::operator >: Iterators are in different ranges");
            }
            return (rhs < *this);
        }

        bool operator >=(const IntRange2DIterator& rhs) const {
            if (r != rhs.r) {
                error("Range2D Iterator::operator >=: Iterators are in different ranges");
            }
            return (*this > rhs) || (*this == rhs);
        }

        Point operator *() const {
            return Point(x, y);
        }

        Point* operator ->() const {
            return new Point(x, y);
        }
    };

    int _minX;
    int _minY;
    int _maxX;
    int _maxY;
    bool _yMajor;

    friend std::istream& operator >>(std::istream& input, IntRange2D& r);

public:
    /**
     * Constructs a 2D range of the given width and height, default 0.
     * Its bounds will be [0,0 .. w-1,h-1] inclusive.
     * The yMajor parameter indicates whether the range will emit its members
     * in y-major (default) or x-major order.
     */
    IntRange2D(int width = 0, int height = 0, bool yMajor = true);

    /**
     * Constructs a 2D range between the given min/max locations, inclusive.
     * The yMajor parameter indicates whether the range will emit its members
     * in y-major (default) or x-major order.
     */
    IntRange2D(int minX, int minY, int maxX, int maxY, bool yMajor = true);

    /**
     * Returns an iterator over the elements of this 2D range.
     */
    IntRange2DIterator begin() const;

    /**
     * Returns true if the given x/y location is contained in this 2D range.
     */
    bool contains(int x, int y) const;

    /**
     * Returns true if the given 2D range is entirely contained in this 2D range.
     */
    bool contains(const IntRange2D& r) const;

    /**
     * Returns true if this range does not contain any integers in one dimension
     * or the other or both (if max X/Y is less than min X/Y).
     * Equivalent to isEmpty.
     */
    bool empty() const;

    /**
     * Returns an iterator at the end of the elements of this 2D range.
     */
    IntRange2DIterator end() const;

    /**
     * Returns the range of y-values in this 2D range.
     * For example, in the range [0,0 .. 4,7], the height is 8.
     */
    int height() const;

//    bool intersects(const Range2D& r) const;

    /**
     * Returns true if this range does not contain any integers in one dimension
     * or the other or both (if max X/Y is less than min X/Y).
     * Equivalent to empty.
     */
    bool isEmpty() const;

    /**
     * Returns true if this range should emit its integers in y-major order
     * as passed to the constructor (default true).
     */
    bool isYMajor() const;

    /**
     * Returns the maximum x-value in this 2D range.
     */
    int maxX() const;

    /**
     * Returns the maximum y-value in this 2D range.
     */
    int maxY() const;

    /**
     * Returns the minimum x-value in this 2D range.
     */
    int minX() const;

    /**
     * Returns the minimum y-value in this 2D range.
     */
    int minY() const;

    /**
     * Returns the total number of integers in this 2D range.
     * For example, in the range [0,0 .. 4,7], the size is 5x8 = 40.
     */
    int size() const;

    /**
     * Returns the range of x-values in this 2D range.
     * For example, in the range [0,0 .. 4,7], the height is 5.
     */
    int width() const;

    /**
     * Returns a string representation of this 2D range,
     * such as "[0,0 .. 4,7]".
     */
    std::string toString() const;
};

/**
 * Returns an integer hash code of this 2D range.
 */
int hashCode(const IntRange2D& r);

/**
 * Convenience function for constructing a 2D range.
 * See Range2D constructors.
 */
IntRange2D range2d(int width = 0, int height = 0, bool yMajor = false);

/**
 * Convenience function for constructing a 2D range.
 * See Range2D constructors.
 */
IntRange2D range2d(int minX, int minY, int maxX, int maxY, bool yMajor = false);

/**
 * Relational operators for comparing 2D ranges.
 * Ranges are ordered by their minX values, then minY, then maxX, then maxY.
 */
bool operator <(const IntRange2D& r1, const IntRange2D& r2);
bool operator <=(const IntRange2D& r1, const IntRange2D& r2);
bool operator ==(const IntRange2D& r1, const IntRange2D& r2);
bool operator !=(const IntRange2D& r1, const IntRange2D& r2);
bool operator >(const IntRange2D& r1, const IntRange2D& r2);
bool operator >=(const IntRange2D& r1, const IntRange2D& r2);

/**
 * Writes the given 2D range to the given output stream in its toString format.
 */
std::ostream& operator <<(std::ostream& out, const IntRange2D& r);

/**
 * Reads the given 2D range to the given input stream in its toString format.
 */
std::istream& operator >>(std::istream& input, IntRange2D& r);

#endif // _intrange_h
