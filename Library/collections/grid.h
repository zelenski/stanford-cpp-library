/*
 * File: grid.h
 * ------------
 * This file exports the <code>Grid</code> class, which offers a
 * convenient abstraction for representing a two-dimensional array.
 */

#ifndef _grid_h
#define _grid_h

#include <initializer_list>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>

#include "collections.h"
#include "error.h"
#include "gridlocation.h"
#include "hashcode.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

/*
 * Class: Grid<ValueType>
 * ----------------------
 * This class stores an indexed, two-dimensional array.  The following code,
 * for example, creates an identity matrix of size <code>n</code>, in which
 * the elements are 1.0 along the main diagonal and 0.0 everywhere else:
 *
 *<pre>
 *    Grid&lt;double&gt; createIdentityMatrix(int n) {
 *       Grid&lt;double&gt; matrix(n, n);
 *       for (int i = 0; i &lt; n; i++) {
 *          matrix[i][i] = 1.0;
 *       }
 *       return matrix;
 *    }
 *</pre>
 */

template <typename ValueType>
class Grid {
public:
    /* Forward reference */
    class GridRow;
    class GridRowConst;

    /*
     * Constructor: Grid
     * Usage: Grid<ValueType> grid;
     *        Grid<ValueType> grid(nRows, nCols);
     * ------------------------------------------
     * Initializes a new grid.  The second form of the constructor is
     * more common and creates a grid with the specified number of rows
     * and columns.  Each element of the grid is initialized to the
     * default value for the type.  The default constructor creates an
     * empty grid for which the client must call <code>resize</code> to
     * set the dimensions.
     * The three-argument constructor also accepts an initial value and
     * fills every cell of the grid with that value.
     */
    Grid() = default;
    Grid(int _rowCount, int _columnCount);
    Grid(int _rowCount, int _columnCount, const ValueType& value);

    /*
     * This constructor uses an initializer list to set up the grid.
     * Usage: Grid<int> grid {{1, 2, 3}, {4, 5, 6}};
     */
    Grid(std::initializer_list<std::initializer_list<ValueType>> list);

    /*
     * Destructor: ~Grid
     * -----------------
     * Frees any heap storage associated with this grid.
     */
    virtual ~Grid() = default;

    /*
     * Method: clear
     * Usage: grid.clear();
     * --------------------
     * Sets every value in the grid to its element type's default value.
     */
    void clear();

    /*
     * Method: equals
     * Usage: if (grid.equals(grid2)) ...
     * ----------------------------------
     * Returns <code>true</code> if this grid contains exactly the same
     * values as the given other grid.
     * Identical in behavior to the == operator.
     */
    bool equals(const Grid<ValueType>& grid2) const;

    /*
     * Method: fill
     * Usage: grid.fill(value);
     * ------------------------
     * Stores the given value in every cell of this grid.
     */
    void fill(const ValueType& value);

    /*
     * Method: get
     * Usage: ValueType value = grid.get(row, col);
     * --------------------------------------------
     * Returns the element at the specified <code>row</code>/<code>col</code>
     * position in this grid.  This method signals an error if the
     * <code>row</code> and <code>col</code> arguments are outside
     * the grid boundaries.
     */
    const ValueType& get(int row, int col) const;
    const ValueType& get(const GridLocation& loc) const;

    /*
     * Method: inBounds
     * Usage: if (grid.inBounds(row, col)) ...
     * ---------------------------------------
     * Returns <code>true</code> if the specified row and column position
     * is inside the bounds of the grid.
     */
    bool inBounds(int row, int col) const;
    bool inBounds(const GridLocation& loc) const;

    /*
     * Method: isEmpty
     * Usage: if (grid.isEmpty()) ...
     * ---------------------------------------
     * Returns <code>true</code> if the grid has 0 rows and/or 0 columns.
     */
    bool isEmpty() const;

    /*
     * Method: locations
     * Usage: for (GridLocation loc : grid.locations()) ...
     * ----------------------------------------------------
     * Returns a range of (row,col) locations found in this grid.
     * This allows a nice abstraction for looping over the 2D grid range
     * of indexes using a single for loop.
     * By default the locations are arranged in row-major order,
     * but if you pass the rowMajor parameter of false, the locations will be
     * returned in column-major order instead.
     */
    GridLocationRange locations(bool rowMajor = true) const;

    /*
     * Method: mapAll
     * Usage: grid.mapAll(fn);
     * -----------------------
     * Calls the specified function on each element of the grid.  The
     * elements are processed in <b><i>row-major order,</i></b> in which
     * all the elements of row 0 are processed, followed by the elements
     * in row 1, and so on.
     */
    void mapAll(std::function<void (const ValueType &)>) const;

    /*
     * Method: numCols
     * Usage: int nCols = grid.numCols();
     * ----------------------------------
     * Returns the number of columns in the grid.
     */
    int numCols() const;

    /*
     * Method: numRows
     * Usage: int nRows = grid.numRows();
     * ----------------------------------
     * Returns the number of rows in the grid.
     */
    int numRows() const;

    /*
     * Method: resize
     * Usage: grid.resize(nRows, nCols);
     * ---------------------------------
     * Reinitializes the grid to have the specified number of rows
     * and columns.  If the 'retain' parameter is true,
     * the previous grid contents are retained as much as possible.
     * If 'retain' is not passed or is false, any previous grid contents
     * are discarded.
     */
    void resize(int _rowCount, int _columnCount, bool retain = false);

    /*
     * Method: set
     * Usage: grid.set(row, col, value);
     * ---------------------------------
     * Replaces the element at the specified <code>row</code>/<code>col</code>
     * location in this grid with a new value.  This method signals an error
     * if the <code>row</code> and <code>col</code> arguments are outside
     * the grid boundaries.
     */
    void set(int row, int col, const ValueType& value);
    void set(const GridLocation& loc, const ValueType& value);

    /*
     * Method: size
     * Usage: int size = grid.size();
     * ------------------------------
     * Returns the total number of elements in the grid, which is equal to the
     * number of rows times the number of columns.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = grid.toString();
     * ------------------------------------
     * Converts the grid to a printable string representation.
     * The string returned is a 1-dimensional representation such as:
     * "{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}"
     */
    std::string toString() const;

    /*
     * Method: toString2D
     * Usage: string str = grid.toString2D();
     * --------------------------------------
     * Converts the grid to a printable string representation.
     * The string returned is a 2-dimensional representation such as:
     * "{{1, 2, 3},\n
     *   {4, 5, 6},\n
     *   {7, 8, 9}}"
     */
    std::string toString2D(
            std::string rowStart = "{",
            std::string rowEnd = "}",
            std::string colSeparator = ", ",
            std::string rowSeparator = ",\n ") const;


    /*
     * Operator: []
     * Usage:  grid[row][col]
     * ----------------------
     * Overloads <code>[]</code> to select elements from this grid.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.  This method signals an error if
     * the <code>row</code> and <code>col</code> arguments are outside
     * the grid boundaries.
     */
    GridRow operator [](int row);
    const GridRowConst operator [](int row) const;
    ValueType& operator [](const GridLocation& loc);
    const ValueType& operator [](const GridLocation& loc) const;

    /*
     * Additional Grid operations
     * --------------------------
     * In addition to the methods listed in this interface, the Grid
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * The iteration forms process the grid in row-major order.
     */

    /*
     * Operator: ==
     * Usage: if (grid1 == grid2) ...
     * ------------------------------
     * Compares two grids for equality.
     */
    bool operator ==(const Grid& grid2) const;

    /*
     * Operator: !=
     * Usage: if (grid1 != grid2) ...
     * ------------------------------
     * Compares two grids for inequality.
     */
    bool operator !=(const Grid& grid2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: if (grid1 < grid2) ...
     * -----------------------------
     * Relational operators to compare two grids.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Grid& grid2) const;
    bool operator <=(const Grid& grid2) const;
    bool operator >(const Grid& grid2) const;
    bool operator >=(const Grid& grid2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes: Grid data structure
     * -----------------------------------------
     * The Grid is internally managed as a dynamic array of elements.
     * The array itself is one-dimensional, the logical separation into
     * rows and columns is done by arithmetic computation.  The layout
     * is in row-major order, which is to say that the entire first row
     * is laid out contiguously, followed by the entire second row,
     * and so on.
     */

private:
    /* Instance variables */
    Vector<ValueType> _elements;   // The elements, in row-major order
    int _rowCount = 0;             // The number of rows in the grid
    int _columnCount = 0;          // The number of columns in the grid

    /* Private method prototypes */

    /*
     * Throws an ErrorException if the given row/col are not within the range of
     * (0,0) through (rowMax-1,colMax-1) inclusive.
     * This is a consolidated error handler for all various Grid members that
     * accept index parameters.
     * The prefix parameter represents a text string to place at the start of
     * the error message, generally to help indicate which member threw the error.
     */
    void checkIndexes(int row, int col,
                      int rowMax, int colMax,
                      std::string prefix) const;
    int gridCompare(const Grid& grid2) const;

    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support deep copying and iteration.  Including these methods
     * in the public interface would make that interface more
     * difficult to understand for the average client.
     */

public:
    using iterator       = typename Vector<ValueType>::iterator;
    using const_iterator = typename Vector<ValueType>::const_iterator;

    iterator begin() {
        return _elements.begin();
    }
    iterator end() {
        return _elements.end();
    }

    const_iterator begin() const {
        return _elements.begin();
    }
    const_iterator end() const {
        return _elements.end();
    }

    /*
     * Private class: Grid<ValType>::GridRow
     * -------------------------------------
     * This section of the code defines a nested class within the Grid template
     * that makes it possible to use traditional subscripting on Grid values.
     */
    class GridRow {
    public:
        GridRow() : _gp(nullptr), _row(0) {
            /* Empty */
        }

        ValueType& operator [](int col) {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[(_row * _gp->_columnCount) + col];
        }

        ValueType operator [](int col) const {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[(_row * _gp->_columnCount) + col];
        }

        int size() const {
            return _gp->numCols();
        }

    private:
        GridRow(Grid* gridRef, int index) {
            _gp = gridRef;
            _row = index;
        }

        Grid* _gp;
        int _row;
        friend class Grid;
    };
    friend class GridRow;

    class GridRowConst {
    public:
        GridRowConst() : _gp(nullptr), _row(0) {
            /* Empty */
        }

        const ValueType operator [](int col) const {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[(_row * _gp->_columnCount) + col];
        }

        int size() const {
            return _gp->numCols();
        }

    private:
        GridRowConst(Grid* const gridRef, int index) : _gp(gridRef), _row(index) {}

        const Grid* const _gp;
        const int _row;
        friend class Grid;
    };
    friend class GridRowConst;

    template <typename T>
    friend int hashCode(const Grid<T>& g);
};

template <typename ValueType>
Grid<ValueType>::Grid(int numRows, int numCols) {
    resize(numRows, numCols);
}

template <typename ValueType>
Grid<ValueType>::Grid(int numRows, int numCols, const ValueType& value) {
    resize(numRows, numCols);
    fill(value);
}

template <typename ValueType>
Grid<ValueType>::Grid(std::initializer_list<std::initializer_list<ValueType>> list) {
    // create the grid at the proper size
    _rowCount = list.size();
    if (list.begin() != list.end()) {
        _columnCount = list.begin()->size();
    }
    resize(_rowCount, _columnCount);

    // copy the data from the initializer list into the Grid
    auto rowItr = list.begin();
    for (int row = 0; row < _rowCount; row++) {
        if (static_cast<int>(rowItr->size()) != _columnCount) {
            error("Grid::constructor: initializer list is not rectangular (must have same # cols in each row)");
        }
        auto colItr = rowItr->begin();
        for (int col = 0; col < _columnCount; col++) {
            set(row, col, *colItr);
            colItr++;
        }
        rowItr++;
    }
}

template <typename ValueType>
void Grid<ValueType>::clear() {
    ValueType defaultValue = ValueType();
    for (int r = 0; r < _rowCount; r++) {
        for (int c = 0; c < _columnCount; c++) {
            set(r, c, defaultValue);
        }
    }
}

template <typename ValueType>
bool Grid<ValueType>::equals(const Grid<ValueType>& grid2) const {
    // optimization: if literally same grid, stop
    if (this == &grid2) {
        return true;
    }

    if (_rowCount != grid2._rowCount || _columnCount != grid2._columnCount) {
        return false;
    }
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            if (get(row, col) != grid2.get(row, col)) {
                return false;
            }
        }
    }
    return true;
}

template <typename ValueType>
void Grid<ValueType>::fill(const ValueType& value) {
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            set(row, col, value);
        }
    }

    /* This counts as a semantic update, so we must update the version. */
    _elements.updateVersion();
}

template <typename ValueType>
const ValueType& Grid<ValueType>::get(int row, int col) const {
    checkIndexes(row, col, _rowCount-1, _columnCount-1, "get");
    return _elements[(row * _columnCount) + col];
}

template <typename ValueType>
const ValueType& Grid<ValueType>::get(const GridLocation& loc) const {
    return get(loc.row, loc.col);
}

template <typename ValueType>
bool Grid<ValueType>::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < _rowCount && col < _columnCount;
}

template <typename ValueType>
bool Grid<ValueType>::inBounds(const GridLocation& loc) const {
    return inBounds(loc.row, loc.col);
}

template <typename ValueType>
bool Grid<ValueType>::isEmpty() const {
    return _rowCount == 0 || _columnCount == 0;
}

template <typename ValueType>
GridLocationRange Grid<ValueType>::locations(bool rowMajor) const {
    return GridLocationRange(0, 0, numRows() - 1, numCols() - 1, rowMajor);
}

template <typename ValueType>
void Grid<ValueType>::mapAll(std::function<void (const ValueType &)> fn) const {
    for (int i = 0; i < _rowCount; i++) {
        for (int j = 0; j < _columnCount; j++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
int Grid<ValueType>::numCols() const {
    return _columnCount;
}

template <typename ValueType>
int Grid<ValueType>::numRows() const {
    return _rowCount;
}

template <typename ValueType>
void Grid<ValueType>::resize(int numRows, int numCols, bool retain) {
    if (numRows < 0 || numCols < 0) {
        std::ostringstream out;
        out << "Grid::resize: Attempt to resize grid to invalid size ("
               << numRows << ", " << numCols << ")";
        error(out.str());
    }

    // optimization: don't do the resize if we are already that size
    if (numRows == this->_rowCount && numCols == this->_columnCount && retain) {
        /* We need to update the version because semantically we've changed the grid,
         * but we haven't touched our vector.
         */
        _elements.updateVersion();
        return;
    }

    // save backup of old array/size
    Vector<ValueType> oldElements = std::move(_elements);
    int oldnRows = this->_rowCount;
    int oldnCols = this->_columnCount;

    // create new empty array and set new size
    this->_rowCount = numRows;
    this->_columnCount = numCols;
    this->_elements = Vector<ValueType>(numRows * numCols, ValueType());

    // possibly retain old contents
    if (retain) {
        int minRows = oldnRows < numRows ? oldnRows : numRows;
        int minCols = oldnCols < numCols ? oldnCols : numCols;
        for (int row = 0; row < minRows; row++) {
            for (int col = 0; col < minCols; col++) {
                this->_elements[(row * numCols) + col] = oldElements[(row * oldnCols) + col];
            }
        }
    }
}

template <typename ValueType>
void Grid<ValueType>::set(int row, int col, const ValueType& value) {
    checkIndexes(row, col, _rowCount - 1, _columnCount - 1, "set");
    _elements[(row * _columnCount) + col] = value;
}

template <typename ValueType>
void Grid<ValueType>::set(const GridLocation& loc, const ValueType& value) {
    set(loc.row, loc.col, value);
}

template <typename ValueType>
int Grid<ValueType>::size() const {
    return _rowCount * _columnCount;
}

template <typename ValueType>
std::string Grid<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
std::string Grid<ValueType>::toString2D(
        std::string rowStart, std::string rowEnd,
        std::string colSeparator, std::string rowSeparator) const {
    std::ostringstream os;
    os << rowStart;
    int nr = numRows();
    int nc = numCols();
    for (int i = 0; i < nr ; i++) {
        if (i > 0) {
            os << rowSeparator;
        }
        os << rowStart;
        for (int j = 0; j < nc; j++) {
            if (j > 0) {
                os << colSeparator;
            }
            writeGenericValue(os, get(i, j), /* forceQuotes */ true);
        }
        os << rowEnd;
    }
    os << rowEnd;
    return os.str();
}

template <typename ValueType>
typename Grid<ValueType>::GridRow Grid<ValueType>::operator [](int row) {
    return GridRow(this, row);
}

template <typename ValueType>
ValueType& Grid<ValueType>::operator [](const GridLocation& loc) {
    checkIndexes(loc.row, loc.col, _rowCount-1, _columnCount-1, "operator []");
    return _elements[(loc.row * _columnCount) + loc.col];
}

template <typename ValueType>
const typename Grid<ValueType>::GridRowConst
Grid<ValueType>::operator [](int row) const {
    return GridRowConst(const_cast<Grid*>(this), row);
}

template <typename ValueType>
const ValueType& Grid<ValueType>::operator [](const GridLocation& loc) const {
    checkIndexes(loc.row, loc.col, _rowCount-1, _columnCount-1, "operator []");
    return _elements[(loc.row * _columnCount) + loc.col];
}

template <typename ValueType>
bool Grid<ValueType>::operator ==(const Grid& grid2) const {
    return equals(grid2);
}

template <typename ValueType>
bool Grid<ValueType>::operator !=(const Grid& grid2) const {
    return !equals(grid2);
}

template <typename ValueType>
bool Grid<ValueType>::operator <(const Grid& grid2) const {
    return gridCompare(grid2) < 0;
}

template <typename ValueType>
bool Grid<ValueType>::operator <=(const Grid& grid2) const {
    return gridCompare(grid2) <= 0;
}

template <typename ValueType>
bool Grid<ValueType>::operator >(const Grid& grid2) const {
    return gridCompare(grid2) > 0;
}

template <typename ValueType>
bool Grid<ValueType>::operator >=(const Grid& grid2) const {
    return gridCompare(grid2) >= 0;
}

template <typename ValueType>
void Grid<ValueType>::checkIndexes(int row, int col,
                                   int rowMax, int colMax,
                                   std::string prefix) const {
    const int rowMin = 0;
    const int colMin = 0;
    if (row < rowMin || row > rowMax || col < colMin || col > colMax) {
        std::ostringstream out;
        out << "Grid::" << prefix << ": (" << row << ", " << col << ")"
            << " is outside of valid range [";
        if (rowMin < rowMax && colMin < colMax) {
            out << "(" << rowMin << ", " << colMin <<  ")..("
                << rowMax << ", " << colMax << ")";
        } else if (rowMin == rowMax && colMin == colMax) {
            out << "(" << rowMin << ", " << colMin <<  ")";
        } // else min > max, no range, empty grid
        out << "]";
        error(out.str());
    }
}

template <typename ValueType>
int Grid<ValueType>::gridCompare(const Grid& grid2) const {
    if (_rowCount != grid2._rowCount) return _rowCount - grid2._rowCount;
    if (_columnCount != grid2._columnCount) return _columnCount - grid2._columnCount;
    return stanfordcpplib::collections::compare(_elements, grid2._elements);
}

template <typename ValueType>
int hashCode(const Grid<ValueType>& g) {
    return hashCode(g._rowCount, g._columnCount, g._elements);
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Grid<ValueType>& grid) {
    os << "{";
    int nRows = grid.numRows();
    int nCols = grid.numCols();
    for (int i = 0; i < nRows; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << "{";
        for (int j = 0; j < nCols; j++) {
            if (j > 0) {
                os << ", ";
            }
            writeGenericValue(os, grid.get(i, j), /* forceQuotes */ true);
        }
        os << "}";
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Grid<ValueType>& grid) {
    Vector<Vector<ValueType>> vec2d;
    if (!(is >> vec2d)) {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    int nRows = vec2d.size();
    int nCols = (nRows == 0) ? 0 : vec2d[0].size();
    grid.resize(nRows, nCols);
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            grid[i][j] = vec2d[i][j];
        }
    }

    return is;
}

/*
 * Function: randomElement
 * Usage: element = randomElement(grid);
 * -------------------------------------
 * Returns a randomly chosen element of the given grid.
 * Throws an error if the grid is empty.
 */
template <typename T>
const T& randomElement(const Grid<T>& grid) {
    if (grid.isEmpty()) {
        error("randomElement: empty grid was passed");
    }

    int randomIndex = randomInteger(0, grid.size() - 1);
    int row = randomIndex / grid.numCols();
    int col = randomIndex % grid.numCols();
    return grid.get(row, col);
}


#endif // _grid_h
