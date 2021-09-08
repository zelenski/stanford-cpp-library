/*
 * File: gridlocation.h
 * --------------------
 * This file exports the <code>GridLocation</code> structure, which is a small
 * structure representing a row and column.
 * The row/column values are allowed to be negative or out of bounds; if an
 * out-of-bounds location is passed to a grid, the grid will throw an error.
 *
 * Several members of the <code>Grid</code> and <code>SparseGrid</code> classes
 * have been retrofitted to accept <code>GridLocation</code>s in place of integers
 * for row/column indexes.
 *
 * This file also declares the <code>GridLocationRange</code> class,
 * which represents a 2D range of grid locations that can be looped over.
 *
 * See gridlocation.cpp for the implementation of each member.
 *
 * @version 2018/03/12
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gridlocation_h
#define _gridlocation_h

#include <iostream>
#include <iterator>
#include <string>

#define INTERNAL_INCLUDE 1
#include "error.h"
#undef INTERNAL_INCLUDE

class GridLocationRange;   // forward declaration

struct GridLocation {
public:
    /*
     * Constructs a location representing the given row and column (default 0).
     * Any indexes are allowed, including negatives and out-of-bounds indexes.
     */
    GridLocation(int row = 0, int col = 0);

    /*
     * Returns a range of locations that are <= the given range away from this one.
     * For example, if you pass 1, will return the 9 locations in range (r-1, c-1) thru (r+1, c+1).
     * The rowMajor parameter indicates whether the range will emit its members
     * in row-major (default) or column-major order.
     */
    GridLocationRange neighbors(int range = 1, bool rowMajor = true) const;

    /*
     * Returns a string representation of this location, such as "r2c17".
     */
    std::string toString() const;

    /* row and column data - may be directly accessed or modified */
    int row;
    int col;
};

/*
 * Returns an integer hash code for this grid location.
 */
int hashCode(const GridLocation& loc);

/*
 * Relational operators for comparing grid locations.
 */
bool operator <(const GridLocation& loc1, const GridLocation& loc2);
bool operator <=(const GridLocation& loc1, const GridLocation& loc2);
bool operator ==(const GridLocation& loc1, const GridLocation& loc2);
bool operator !=(const GridLocation& loc1, const GridLocation& loc2);
bool operator >(const GridLocation& loc1, const GridLocation& loc2);
bool operator >=(const GridLocation& loc1, const GridLocation& loc2);

/*
 * I/O stream operators for reading or writing locations in their toString format.
 */
std::ostream& operator <<(std::ostream& out, const GridLocation& loc);
std::istream& operator >>(std::istream& input, GridLocation& loc);


/*
 * Represents a range of grid locations.
 * The actual individual grid locations are not all created and stored in
 * this object; that would require a lot of memory usage.
 * Instead, we primarily use this class for for-each looping over a given range
 * of locations using its internal iterator.
 *
 * Common usage pattern:
 * GridLocationRange range(0, 0, 10, 5);
 * for (GridLocation loc : range) { ... }
 *
 * or, if you have a Grid collection, its locations() method returns a GridLocationRange
 * object that you can loop over directly.
 *
 * for (GridLocation loc : grid.locations()) { ... }
 */
class GridLocationRange {
private:
    /*
     * Internal iterator over range of indexes.
     */
    class GridLocationRangeIterator : public std::iterator<std::input_iterator_tag, GridLocation> {
    private:
        const GridLocationRange* glr;
        GridLocation loc;

    public:
        GridLocationRangeIterator(const GridLocationRange* glr, bool end)
                : glr(glr) {
            if (end) {
                loc.row = glr->endRow() + 1;
                loc.col = glr->endCol() + 1;
            } else {
                loc = glr->startLocation();
            }
        }

        GridLocationRangeIterator(const GridLocationRangeIterator& itr)
                : glr(itr.glr),
                  loc(itr.loc) {
            // empty
        }

        GridLocationRangeIterator& operator ++() {
            if (glr->isRowMajor()) {
                loc.col++;
                if (loc.col > glr->endCol()) {
                    loc.col = glr->startCol();
                    loc.row++;
                }
            } else {
                loc.row++;
                if (loc.row > glr->endRow()) {
                    loc.row = glr->startRow();
                    loc.col++;
                }
            }
            if (!glr->contains(loc)) {
                loc.row = glr->endRow() + 1;
                loc.col = glr->endCol() + 1;
            }
            return *this;
        }

        GridLocationRangeIterator operator ++(int) {
            GridLocationRangeIterator copy(*this);
            operator++();
            return copy;
        }

        GridLocationRangeIterator& operator --() {
            if (glr->isRowMajor()) {
                loc.col--;
                if (loc.col < glr->startCol()) {
                    loc.col = glr->endCol();
                    loc.row--;
                }
            } else {
                loc.row--;
                if (loc.row < glr->startRow()) {
                    loc.row = glr->endRow();
                    loc.col--;
                }
            }
            return *this;
        }

        GridLocationRangeIterator operator --(int) {
            GridLocationRangeIterator copy(*this);
            operator--();
            return copy;
        }

        bool operator ==(const GridLocationRangeIterator& rhs) const {
            return loc == rhs.loc;
        }

        bool operator !=(const GridLocationRangeIterator& rhs) const {
            return !(*this == rhs);
        }

        bool operator <(const GridLocationRangeIterator& rhs) const {
            if (glr != rhs.glr) {
                error("GridLocationRange Iterator::operator <: Iterators are in different ranges");
            }
            return loc < rhs.loc;
        }

        bool operator <=(const GridLocationRangeIterator& rhs) const {
            if (glr != rhs.glr) {
                error("GridLocationRange Iterator::operator <=: Iterators are in different ranges");
            }
            return loc <= rhs.loc;
        }

        bool operator >(const GridLocationRangeIterator& rhs) const {
            if (glr != rhs.glr) {
                error("GridLocationRange Iterator::operator >: Iterators are in different ranges");
            }
            return loc > rhs.loc;
        }

        bool operator >=(const GridLocationRangeIterator& rhs) const {
            if (glr != rhs.glr) {
                error("GridLocationRange Iterator::operator >=: Iterators are in different ranges");
            }
            return loc >= rhs.loc;
        }

        const GridLocation& operator *() const {
            return loc;
        }

        const GridLocation* operator ->() const {
            return &loc;
        }
    };

    GridLocation _start;
    GridLocation _end;
    bool _isRowMajor;

public:
    /*
     * Constructs a range over the given start/end locations, inclusive.
     * The isRowMajor flag indicates whether we will loop over the range in
     * row-major order (true, default) or column-major order (false).
     */
    GridLocationRange(int startRow = 0, int startCol = 0, int endRow = 0, int endCol = 0, bool isRowMajor = true);

    /*
     * Constructs a range over the given start/end locations, inclusive.
     * The isRowMajor flag indicates whether we will loop over the range in
     * row-major order (true, default) or column-major order (false).
     */
    GridLocationRange(const GridLocation& startLoc, const GridLocation& endLoc, bool isRowMajor = true);

    /*
     * Returns an iterator over the range.
     */
    GridLocationRangeIterator begin() const;

    /*
     * Returns true if this range entirely contains the given other range.
     */
    bool contains(const GridLocation& loc) const;

    /*
     * Returns an iterator at the end of the range.
     */
    GridLocationRangeIterator end() const;

    /*
     * Returns the last column in this range, inclusive.
     */
    int endCol() const;

    /*
     * Returns the last row/column location in this range, inclusive.
     */
    const GridLocation& endLocation() const;

    /*
     * Returns the last row in this range, inclusive.
     */
    int endRow() const;

    /*
     * Returns true if this range contains no rows or columns.
     */
    bool isEmpty() const;

    /*
     * Returns true if this range should be traversed in row-major order,
     * as specified at time of construction (default true).
     */
    bool isRowMajor() const;

    /*
     * Returns the first column in this range.
     */
    int startCol() const;

    /*
     * Returns the first row/column location in this range.
     */
    const GridLocation& startLocation() const;

    /*
     * Returns the first row in this range.
     */
    int startRow() const;

    /*
     * Returns a string representation of this range,
     * such as "[r1c3 .. r4c7]".
     */
    std::string toString() const;
};

/*
 * I/O stream operators for writing location ranges in their toString format.
 */
std::ostream& operator <<(std::ostream& out, const GridLocationRange& range);

#endif // _gridlocation_h
