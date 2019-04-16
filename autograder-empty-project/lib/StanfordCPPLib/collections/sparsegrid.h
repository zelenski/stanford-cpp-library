/*
 * File: sparsegrid.h
 * ------------------
 * This file exports the <code>SparseGrid</code> class, which offers a
 * convenient abstraction for representing a two-dimensional array.
 * Unlike Grid, SparseGrid is optimized for grids in which most of the cells
 * are empty.  It uses far less memory in such cases than a Grid does.
 * If the grid is expected to be mostly full of meaningful data,
 * Grid is recommended for use over SparseGrid.
 * 
 * @author Marty Stepp
 * @version 2018/03/12
 * - added overloads that accept GridLocation: get, inBounds, isSet, locations,
 *   set, unset, operator []
 * @version 2018/03/10
 * - added methods front, back, clear
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2016/10/22
 * - bug fix for size method
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * - added size() method
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as
 *   {{1, 2, 3}, {4, 5, 6}}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 * @version 2014/10/20
 * - added width(), height() methods to be consistent with Grid
 * @version 2014/10/10
 * - removed usage of __foreach macro
 * - made ==, != operators const as they should be
 * - added resize(true) function with ability to retain old contents
 * @since 2014/07/09
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _sparsegrid_h
#define _sparsegrid_h

#include <initializer_list>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gridlocation.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "random.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/*
 * Class: SparseGrid<ValueType>
 * ----------------------------
 * This class stores an indexed, two-dimensional array.
 */

template <typename ValueType>
class SparseGrid {
public:
    /* Forward reference */
    class SparseGridRow;
    class SparseGridRowConst;

    /*
     * Constructor: SparseGrid
     * Usage: SparseGrid<ValueType> grid;
     *        SparseGrid<ValueType> grid(nRows, nCols);
     * ------------------------------------------------
     * Initializes a new grid.  The second form of the constructor is
     * more common and creates a grid with the specified number of rows
     * and columns.  Each element of the grid is initialized to the
     * default value for the type.  The default constructor creates an
     * empty grid for which the client must call <code>resize</code> to
     * set the dimensions.
     * The three-argument constructor also accepts an initial value and
     * fills every cell of the grid with that value.
     */
    SparseGrid();
    SparseGrid(int _rowCount, int _columnCount);
    SparseGrid(int _rowCount, int _columnCount, const ValueType& value);

    /*
     * This constructor uses an initializer list to set up the grid.
     * Usage: SparseGrid<int> grid {{1, 2, 3}, {4, 5, 6}};
     */
    SparseGrid(std::initializer_list<std::initializer_list<ValueType>> list);

    /*
     * Destructor: ~SparseGrid
     * -----------------------
     * Frees any heap storage associated with this grid.
     */
    virtual ~SparseGrid();

    /*
     * Method: back
     * Usage: ValueType value = grid.back();
     * -------------------------------------
     * Returns the last value in the grid in the order established by the
     * <code>foreach</code> macro.  This is the highest row/col value that has
     * been explicitly set.
     * If the grid is empty, generates an error.
     */
    ValueType back() const;

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
    bool equals(const SparseGrid<ValueType>& grid) const;

    /*
     * Method: fill
     * Usage: grid.fill(value);
     * ------------------------
     * Stores the given value in every cell of this grid.
     */
    void fill(const ValueType& value);

    /*
     * Method: front
     * Usage: ValueType value = grid.front();
     * --------------------------------------
     * Returns the first value in the grid in the order established by the
     * <code>foreach</code> macro.  Typically this is equivalent to grid[0][0].
     * If the grid is empty, generates an error.
     */
    ValueType front() const;

    /*
     * Method: get
     * Usage: ValueType value = grid.get(row, col);
     * --------------------------------------------
     * Returns the element at the specified <code>row</code>/<code>col</code>
     * position in this grid.
     *
     * If no data was set at the given row/column position, this method returns
     * a default value for the grid's value type.
     *
     * This method signals an error if the <code>row</code> and <code>col</code>
     * arguments are outside the grid boundaries.
     */
    ValueType get(int row, int col);
    const ValueType& get(int row, int col) const;
    ValueType get(const GridLocation& loc);
    const ValueType& get(const GridLocation& loc) const;

    /*
     * Method: height
     * Usage: int nRows = grid.height();
     * ---------------------------------
     * Returns the grid's height, that is, the number of rows in the grid.
     */
    int height() const;
    
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
     * Returns <code>true</code> if the specified grid has 0 rows and/or columns,
     * or if none of the individual cell values has been set yet.
     */
    bool isEmpty() const;

    /*
     * Method: isSet
     * Usage: if (grid.isSet(row, col)) ...
     * ---------------------------------------
     * Returns <code>true</code> if the specified row and column position
     * stores meaningful data; in other words, if it has had a value set
     * there by a previous call to fill, set, [], etc.
     */
    bool isSet(int row, int col) const;
    bool isSet(const GridLocation& loc) const;

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
    void mapAll(void (*fn)(ValueType value)) const;
    void mapAll(void (*fn)(const ValueType& value)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

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
     * Returns the total number of elements that have been set in the grid.
     * Note that this may not be equal to the number of rows times the number of
     * columns, because not every index may have been set.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = grid.toString();
     * ------------------------------------
     * Converts the grid to a printable string representation.
     * Unlike the representation of a Grid, this one displays only the
     * cells in which data has been explicitly set, such as:
     * "{0:{2:88}, 1:{3:42}}, 3 x 4"
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
     * Method: unset
     * Usage: grid.unset(row, col, value);
     * -----------------------------------
     * Replaces the element at the specified <code>row</code>/<code>col</code>
     * location in this grid with an empty default value.
     * If the element at the specified row and column was not set, has no effect.
     * This method signals an error if the <code>row</code> and <code>col</code>
     * arguments are outside the grid boundaries.
     */
    void unset(int row, int col);
    void unset(const GridLocation& loc);

    /*
     * Method: width
     * Usage: int nCols = grid.width();
     * --------------------------------
     * Returns the grid's width, that is, the number of columns in the grid.
     */
    int width() const;

    /*
     * Operator: []
     * Usage:  grid[row][col]
     * ----------------------
     * Overloads <code>[]</code> to select elements from this grid.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.
     *
     * If no data was set at the given row/column position, this method returns
     * a default value for the grid's value type.
     *
     * This method signals an error if the <code>row</code> and <code>col</code>
     * arguments are outside the grid boundaries.
     */
    SparseGridRow operator [](int row);
    const SparseGridRowConst operator [](int row) const;
    ValueType& operator [](const GridLocation& loc);
    const ValueType& operator [](const GridLocation& loc) const;

    /*
     * Additional SparseGrid operations
     * --------------------------
     * In addition to the methods listed in this interface, the SparseGrid
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
    bool operator ==(const SparseGrid& grid2) const;

    /*
     * Operator: !=
     * Usage: if (grid1 != grid2) ...
     * ------------------------------
     * Compares two grids for inequality.
     */
    bool operator !=(const SparseGrid& grid2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: if (grid1 < grid2) ...
     * -----------------------------
     * Relational operators to compare two grids.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const SparseGrid& grid2) const;
    bool operator <=(const SparseGrid& grid2) const;
    bool operator >(const SparseGrid& grid2) const;
    bool operator >=(const SparseGrid& grid2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes: SparseGrid data structure
     * -----------------------------------------------
     * The SparseGrid is internally managed as a dynamic array of elements.
     * The array itself is one-dimensional, the logical separation into
     * rows and columns is done by arithmetic computation.  The layout
     * is in row-major order, which is to say that the entire first row
     * is laid out contiguously, followed by the entire second row,
     * and so on.
     */

private:
    /* Instance variables */
    Map<int, Map<int, ValueType>> _elements;   // 2D map of the elements
    int _rowCount;                             // the number of rows in the grid
    int _columnCount;                          // the number of columns in the grid
    unsigned int _version = 0;                 // structure version for detecting invalid iterators

    /* Private method prototypes */

    /*
     * Throws an ErrorException if the given row/col are not within the range of
     * (0,0) through (rowMax-1,colMax-1) inclusive.
     * This is a consolidated error handler for all various SparseGrid members that
     * accept index parameters.
     * The prefix parameter represents a text string to place at the start of
     * the error message, generally to help indicate which member threw the error.
     */
    void checkIndexes(int row, int col,
                      int rowMax, int colMax,
                      std::string prefix) const;
    int gridCompare(const SparseGrid& grid2) const;

    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support deep copying and iteration.  Including these methods
     * in the public interface would make that interface more
     * difficult to understand for the average client.
     */

    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a
     * deep copy, making it possible to pass/return grids by value
     * and assign from one grid to another.  The entire contents of
     * the grid, including all elements, are copied.  Each grid
     * element is copied from the original grid to the copy using
     * assignment (operator=).  Making copies is generally avoided
     * because of the expense and thus, grids are typically passed
     * by reference, however, when a copy is needed, these operations
     * are supported.
     */
    void deepCopy(const SparseGrid& grid) {
        _elements = grid._elements;
        _rowCount = grid._rowCount;
        _columnCount = grid._columnCount;
    }

    template <typename T>
    friend const T& randomElement(const SparseGrid<T>& grid);

    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const SparseGrid<T>& grid);

    template <typename T>
    friend std::istream& operator >>(std::istream& is, SparseGrid<T>& grid);

public:
    SparseGrid& operator =(const SparseGrid& src) {
        if (this != &src) {
            deepCopy(src);
        }
        return *this;
    }

    SparseGrid(const SparseGrid& src) {
        deepCopy(src);
    }

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::iterator<std::input_iterator_tag, ValueType> {
    public:
        iterator(const SparseGrid* gp, int index)
                : gp(gp),
                  index(index),
                  itr_version(gp->version()) {
            // empty
        }

        iterator(const iterator& it)
                : gp(it.gp),
                  index(it.index),
                  itr_version(it.itr_version) {
            // empty
        }

        iterator& operator ++() {
            stanfordcpplib::collections::checkVersion(*gp, *this);
            index++;
            return *this;
        }

        iterator operator ++(int) {
            stanfordcpplib::collections::checkVersion(*gp, *this);
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return gp == rhs.gp && index == rhs.index;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        ValueType operator *() {
            stanfordcpplib::collections::checkVersion(*gp, *this);
            int row = index / gp->_columnCount;
            int col = index % gp->_columnCount;
            return gp->_elements[row][col];
        }

        ValueType* operator ->() {
            stanfordcpplib::collections::checkVersion(*gp, *this);
            int row = index / gp->_columnCount;
            int col = index % gp->_columnCount;
            return &gp->_elements[row][col];
        }

        unsigned int version() const {
            return itr_version;
        }

    private:
        const SparseGrid* gp;
        int index;
        unsigned int itr_version;
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
        return iterator(this, _rowCount * _columnCount);
    }

    /*
     * Returns the internal version of this collection.
     * This is used to check for invalid iterators and issue error messages.
     */
    unsigned int version() const;

    /*
     * Private class: SparseGrid<ValType>::SparseGridRow
     * -------------------------------------
     * This section of the code defines a nested class within the SparseGrid template
     * that makes it possible to use traditional subscripting on SparseGrid values.
     */
    class SparseGridRow {
    public:
        SparseGridRow() {
            /* Empty */
        }

        ValueType& operator [](int col) {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[_row][col];
        }

        const ValueType& operator [](int col) const {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[_row][col];
        }

    private:
        SparseGridRow(SparseGrid* gridRef, int index) {
            _gp = gridRef;
            _row = index;
        }

        SparseGrid* _gp;
        int _row;
        friend class SparseGrid;
    };
    friend class SparseGridRow;

    class SparseGridRowConst {
    public:
        const ValueType operator [](int col) const {
            _gp->checkIndexes(_row, col, _gp->_rowCount-1, _gp->_columnCount-1, "operator [][]");
            return _gp->_elements[_row][col];
        }

    private:
        SparseGridRowConst(SparseGrid* const gridRef, int index) : _gp(gridRef), _row(index) {
            // empty
        }

        const SparseGrid* const _gp;
        const int _row;
        friend class SparseGrid;
    };
    friend class SparseGridRowConst;
};

template <typename ValueType>
SparseGrid<ValueType>::SparseGrid()
        : _rowCount(0),
          _columnCount(0) {
    // empty
}

template <typename ValueType>
SparseGrid<ValueType>::SparseGrid(int nRows, int nCols) {
    resize(nRows, nCols);
}

template <typename ValueType>
SparseGrid<ValueType>::SparseGrid(int nRows, int nCols, const ValueType& value) {
    resize(nRows, nCols);
    fill(value);
}

template <typename ValueType>
SparseGrid<ValueType>::SparseGrid(std::initializer_list<std::initializer_list<ValueType>> list)
        : _rowCount(0),
          _columnCount(0) {
    // create the grid at the proper size
    _rowCount = list.size();
    if (list.begin() != list.end()) {
        _columnCount = list.begin()->size();
    }
    resize(_rowCount, _columnCount);

    // copy the data from the initializer list into the Grid
    auto rowItr = list.begin();
    for (int row = 0; row < _rowCount; row++) {
        if ((int) rowItr->size() != _columnCount) {
            error("SparseGrid::constructor: initializer list is not rectangular (must have same # cols in each row)");
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
SparseGrid<ValueType>::~SparseGrid() {
    // empty
}

template <typename ValueType>
ValueType SparseGrid<ValueType>::back() const {
    if (isEmpty()) {
        error("SparseGrid::back: grid is empty");
    }
    int lastRow = _elements.back();
    int lastCol = _elements[lastRow].back();
    return _elements[lastRow][lastCol];
}

template <typename ValueType>
void SparseGrid<ValueType>::clear() {
    _elements.clear();
}

template <typename ValueType>
bool SparseGrid<ValueType>::equals(const SparseGrid<ValueType>& grid2) const {
    // optimization: if literally same grid, stop
    if (this == &grid2) {
        return true;
    }
    if (_rowCount != grid2._rowCount || _columnCount != grid2._columnCount) {
        return false;
    }
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            if (isSet(row, col)) {
                // I have data there; he must, too, and it must be the same data
                if (!grid2.isSet(row, col) || get(row, col) != grid2.get(row, col)) {
                    return false;
                }
            } else {
                // I don't have data there; he must also not have it there
                if (grid2.isSet(row, col)) {
                    return false;
                }
            }
        }
    }
    return true;
}

template <typename ValueType>
void SparseGrid<ValueType>::fill(const ValueType& value) {
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            set(row, col, value);
        }
    }
}

template <typename ValueType>
ValueType SparseGrid<ValueType>::front() const {
    if (isEmpty()) {
        error("SparseGrid::front: grid is empty");
    }
    return *begin();
}

template <typename ValueType>
ValueType SparseGrid<ValueType>::get(int row, int col) {
    checkIndexes(row, col, _rowCount-1, _columnCount-1, "get");
    return _elements[row][col];
}

template <typename ValueType>
const ValueType& SparseGrid<ValueType>::get(int row, int col) const {
    checkIndexes(row, col, _rowCount-1, _columnCount-1, "get");
    return _elements[row][col];
}

template <typename ValueType>
ValueType SparseGrid<ValueType>::get(const GridLocation& loc) {
    return get(loc.row, loc.col);
}

template <typename ValueType>
const ValueType& SparseGrid<ValueType>::get(const GridLocation& loc) const {
    return get(loc.row, loc.col);
}

template <typename ValueType>
int SparseGrid<ValueType>::height() const {
    return _rowCount;
}

template <typename ValueType>
bool SparseGrid<ValueType>::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < _rowCount && col < _columnCount;
}

template <typename ValueType>
bool SparseGrid<ValueType>::inBounds(const GridLocation& loc) const {
    return inBounds(loc.row, loc.col);
}

template <typename ValueType>
bool SparseGrid<ValueType>::isEmpty() const {
    return _elements.isEmpty();
}

template <typename ValueType>
bool SparseGrid<ValueType>::isSet(int row, int col) const {
    return inBounds(row, col) && _elements.containsKey(row) &&
            _elements[row].containsKey(col);
}

template <typename ValueType>
bool SparseGrid<ValueType>::isSet(const GridLocation& loc) const {
    return isSet(loc.row, loc.col);
}

template <typename ValueType>
GridLocationRange SparseGrid<ValueType>::locations(bool rowMajor) const {
    return GridLocationRange(0, 0, numRows() - 1, numCols() - 1, rowMajor);
}

template <typename ValueType>
void SparseGrid<ValueType>::mapAll(void (*fn)(ValueType value)) const {
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
void SparseGrid<ValueType>::mapAll(void (*fn)(const ValueType& value)) const {
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
template <typename FunctorType>
void SparseGrid<ValueType>::mapAll(FunctorType fn) const {
    for (int row = 0; row < _rowCount; row++) {
        for (int col = 0; col < _columnCount; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
int SparseGrid<ValueType>::numCols() const {
    return _columnCount;
}

template <typename ValueType>
int SparseGrid<ValueType>::numRows() const {
    return _rowCount;
}

template <typename ValueType>
void SparseGrid<ValueType>::resize(int nRows, int nCols, bool retain) {
    if (nRows < 0 || nCols < 0) {
        std::ostringstream out;
        out << "SparseGrid::resize: Attempt to resize grid to invalid size ("
               << nRows << ", " << nCols << ")";
        error(out.str());
    }
    int oldnRows = this->_rowCount;
    int oldnCols = this->_columnCount;
    this->_rowCount = nRows;
    this->_columnCount = nCols;
    
    if (retain) {
        // if resizing to a smaller size, must evict any row/col entries
        // that exceed the new grid's bounds
        if (nRows < oldnRows || nCols < oldnCols) {
            Map<int, Map<int, ValueType>> newElements;
            for (int row : _elements) {
                if (row >= nRows) {
                    break;   // don't add any entries beyond this row
                }
                for (int col : _elements[row]) {
                    if (col >= nCols) {
                        break;   // don't add any entries beyond this row
                    }
                    newElements[row][col] = _elements[row][col];
                }
            }
            _elements = newElements;
        }
    } else {
        _elements.clear();
    }
    _version++;
}

template <typename ValueType>
void SparseGrid<ValueType>::set(int row, int col, const ValueType& value) {
    checkIndexes(row, col, _rowCount-1, _columnCount-1, "set");
    _elements[row][col] = value;
    _version++;
}

template <typename ValueType>
void SparseGrid<ValueType>::set(const GridLocation& loc, const ValueType& value) {
    set(loc.row, loc.col, value);
}

template <typename ValueType>
int SparseGrid<ValueType>::size() const {
    int count = 0;
    for (int row : _elements) {
        count += _elements[row].size();
    }
    return count;
}

template <typename ValueType>
std::string SparseGrid<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
std::string SparseGrid<ValueType>::toString2D(
        std::string rowStart, std::string rowEnd,
        std::string colSeparator, std::string rowSeparator) const {
    std::ostringstream os;
    os << rowStart;
    int nRows = numRows();
    int nCols = numCols();
    for (int i = 0; i < nRows; i++) {
        if (!_elements.containsKey(i) || _elements[i].isEmpty()) {
            continue;
        }
        if (i > 0) {
            os << rowSeparator;
        }
        os << rowStart;
        for (int j = 0; j < nCols; j++) {
            if (j > 0) {
                os << colSeparator;
            }
            if (isSet(i, j)) {
                writeGenericValue(os, get(i, j), true);
            }
        }
        os << rowEnd;
    }
    os << rowEnd;
    return os.str();
}

template <typename ValueType>
void SparseGrid<ValueType>::unset(int row, int col) {
    checkIndexes(row, col, _rowCount-1, _columnCount-1, "unset");
    if (_elements.containsKey(row)) {
        _elements[row].remove(col);
        if (_elements[row].isEmpty()) {
            _elements.remove(row);
        }
    }
    _version++;
}

template <typename ValueType>
void SparseGrid<ValueType>::unset(const GridLocation& loc) {
    unset(loc.row, loc.col);
}

template <typename ValueType>
unsigned int SparseGrid<ValueType>::version() const {
    return _version;
}

template <typename ValueType>
int SparseGrid<ValueType>::width() const {
    return _columnCount;
}

template <typename ValueType>
void SparseGrid<ValueType>::checkIndexes(int row, int col,
                                         int rowMax, int colMax,
                                         std::string prefix) const {
    const int rowMin = 0;
    const int colMin = 0;
    if (row < rowMin || row > rowMax || col < colMin || col > colMax) {
        std::ostringstream out;
        out << "SparseGrid::" << prefix << ": (" << row << ", " << col << ")"
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
int SparseGrid<ValueType>::gridCompare(const SparseGrid& grid2) const {
    int h1 = height();
    int w1 = width();
    int h2 = grid2.height();
    int w2 = grid2.width();
    int rows = h1 > h2 ? h1 : h2;
    int cols = w1 > w2 ? w1 : w2;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (r >= h1) {
                return -1;
            } else if (r >= h2) {
                return 1;
            }
            
            if (c >= w1) {
                return -1;
            } else if (c >= w2) {
                return 1;
            }
            
            if (!isSet(r, c) && grid2.isSet(r, c)) {
                return -1;
            } else if (isSet(r, c) && !grid2.isSet(r, c)) {
                return 1;
            }
            
            if (get(r, c) < grid2.get(r, c)) {
                return -1;
            } else if (grid2.get(r, c) < get(r, c)) {
                return 1;
            }
        }
    }
    return 0;
}

template <typename ValueType>
typename SparseGrid<ValueType>::SparseGridRow SparseGrid<ValueType>::operator [](int row) {
    return SparseGridRow(this, row);
}

template <typename ValueType>
const typename SparseGrid<ValueType>::SparseGridRowConst
SparseGrid<ValueType>::operator [](int row) const {
    return SparseGridRowConst(const_cast<SparseGrid*>(this), row);
}

template <typename ValueType>
ValueType& SparseGrid<ValueType>::operator [](const GridLocation& loc) {
    checkIndexes(loc.row, loc.col, _rowCount-1, _columnCount-1, "operator []");
    return _elements[loc.row][loc.col];
}

template <typename ValueType>
const ValueType& SparseGrid<ValueType>::operator [](const GridLocation& loc) const {
    checkIndexes(loc.row, loc.col, _rowCount-1, _columnCount-1, "operator []");
    return _elements[loc.row][loc.col];
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator ==(const SparseGrid& grid2) const {
    return equals(grid2);
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator !=(const SparseGrid& grid2) const {
    return !equals(grid2);
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator <(const SparseGrid& grid2) const {
    return gridCompare(grid2) < 0;
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator <=(const SparseGrid& grid2) const {
    return gridCompare(grid2) <= 0;
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator >(const SparseGrid& grid2) const {
    return gridCompare(grid2) > 0;
}

template <typename ValueType>
bool SparseGrid<ValueType>::operator >=(const SparseGrid& grid2) const {
    return gridCompare(grid2) >= 0;
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const SparseGrid<ValueType>& grid) {
    os << grid._elements << ", " << grid._rowCount << " x " << grid._columnCount;
    return os;
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, SparseGrid<ValueType>& grid) {
    // "{...}, 4 x 3"

    // read "{...}" (map of elements)
    if (!(is >> grid._elements)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("SparseGrid::operator >>: Invalid elements");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }

    // throw away ', ' token
    std::string comma;
    if (!(is >> comma)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("SparseGrid::operator >>: Invalid format");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }

    if (!(is >> grid._rowCount)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("SparseGrid::operator >>: Invalid number of rows");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }

    std::string x;
    is >> x;       // throw away 'x' token

    if (!(is >> grid._columnCount)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("SparseGrid::operator >>: Invalid number of rows");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }
    return is;
}

/*
 * Template hash function for sparse grids.
 * Requires the element type in the SparseGrid to have a hashCode function.
 */
template <typename T>
int hashCode(const SparseGrid<T>& grid) {
    return stanfordcpplib::collections::hashCodeCollection(grid);
}

/*
 * Function: randomElement
 * Usage: element = randomElement(grid);
 * -------------------------------------
 * Returns a randomly chosen element of the given grid.
 * Throws an error if the grid is empty.
 */
template <typename T>
const T& randomElement(const SparseGrid<T>& grid) {
    if (grid.isEmpty()) {
        error("randomElement: empty sparse grid was passed");
    }
    
    // pick a non-empty row, then pick a random element from it
    // TODO: fix, this is NOT evenly distributed when rows/cols are unequal in size
    int row = randomKey(grid._elements);
    int col = randomKey(grid._elements[row]);
    return grid.get(row, col);
}

#endif // _sparsegrid_h
