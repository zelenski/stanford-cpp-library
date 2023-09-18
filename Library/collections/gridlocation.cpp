/*
 * File: gridlocation.cpp
 * ----------------------
 * This file implements the members of the <code>GridLocation</code> structure
 * and the <code>GridLocationRange</code> class.
 * See gridlocation.h for the declarations of each member.
 *
 * @version 2018/03/12
 * - initial version
 */

#include "gridlocation.h"
#include <sstream>
#include "hashcode.h"

GridLocation::GridLocation(int row, int col) {
    this->row = row;
    this->col = col;
}

GridLocation::GridLocation() : GridLocation(0, 0) {}

std::string GridLocation::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const GridLocation& loc) {
    return hashCode(loc.row, loc.col);
}

bool operator <(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1.row < loc2.row ||
            (loc1.row == loc2.row && loc1.col < loc2.col);
}

bool operator <=(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1 < loc2 || loc1 == loc2;
}

bool operator ==(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1.row == loc2.row && loc1.col == loc2.col;
}

bool operator !=(const GridLocation& loc1, const GridLocation& loc2) {
    return !(loc1 == loc2);
}

bool operator >(const GridLocation& loc1, const GridLocation& loc2) {
    return loc2 < loc1;
}

bool operator >=(const GridLocation& loc1, const GridLocation& loc2) {
    return !(loc1 < loc2);
}

std::ostream& operator <<(std::ostream& out, const GridLocation& loc) {
    return out << "r" << loc.row << "c" << loc.col;
}

std::istream& operator >>(std::istream& input, GridLocation& loc) {
    // read 'r'
    char ch = '\0';
    input >> ch;
    if (!input || ch != 'r' ) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read row
    int row;
    if (!(input >> row)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read 'c'
    ch = input.get();
    if (!input || ch != 'c') {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read col
    int col;
    if (!(input >> col)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    loc.row = row;
    loc.col = col;
    return input;
}

GridLocationRange::GridLocationRange(int startRow, int startCol, int endRow, int endCol, bool isRowMajor)
        : _start(startRow, startCol),
          _end(endRow, endCol),
          _isRowMajor(isRowMajor) {
    // empty
}

GridLocationRange::GridLocationRange(const GridLocation& startLoc, const GridLocation& endLoc, bool isRowMajor)
        : _start(startLoc),
          _end(endLoc),
          _isRowMajor(isRowMajor) {
    // empty
}

GridLocationRange::GridLocationRange()
        : _start(0, 0),
          _end(0, 0),
          _isRowMajor(true) {
    // empty
}

GridLocationRange::GridLocationRangeIterator GridLocationRange::begin() const {
    return GridLocationRangeIterator(this, /* end */ false);
}

bool GridLocationRange::contains(const GridLocation& loc) const {
    /* The location is in range if it's between the relevant
     * rows and columns.
     */
    return _start.row <= loc.row && _start.col <= loc.col &&
           _end.row   >= loc.row && _end.col   >= loc.col;
}

GridLocationRange::GridLocationRangeIterator GridLocationRange::end() const {
    return GridLocationRangeIterator(this, /* end */ true);
}

int GridLocationRange::endCol() const {
    return _end.col;
}

const GridLocation& GridLocationRange::endLocation() const {
    return _end;
}

int GridLocationRange::endRow() const {
    return _end.row;
}

bool GridLocationRange::isEmpty() const {
    /* The range is empty if either the start row or the
     * start column exceeds the end row or end column.
     */
    return _start.row > _end.row || _start.col > _end.col;
}

bool GridLocationRange::isRowMajor() const {
    return _isRowMajor;
}

int GridLocationRange::numRows() const {
    return isEmpty() ? 0 : _end.row - _start.row + 1;
}

int GridLocationRange::numCols() const {
    return isEmpty() ? 0 : _end.col - _start.col + 1;
}

int GridLocationRange::size() const {
    return numRows()*numCols();
}

int GridLocationRange::startCol() const {
    return _start.col;
}

const GridLocation& GridLocationRange::startLocation() const {
    return _start;
}

int GridLocationRange::startRow() const {
    return _start.row;
}

std::string GridLocationRange::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const GridLocationRange& range) {
    return out << range.startLocation() << " .. " << range.endLocation();
}
