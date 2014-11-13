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

#ifndef _sparsegrid_h
#define _sparsegrid_h

#include "error.h"
#include "hashcode.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

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
    SparseGrid(int nRows, int nCols);
    SparseGrid(int nRows, int nCols, const ValueType& value);

    /*
     * Destructor: ~SparseGrid
     * -----------------------
     * Frees any heap storage associated with this grid.
     */
    virtual ~SparseGrid();

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

    /*
     * Method: isSet
     * Usage: if (grid.isSet(row, col)) ...
     * ---------------------------------------
     * Returns <code>true</code> if the specified row and column position
     * stores meaningful data; in other words, if it has had a value set
     * there by a previous call to fill, set, [], etc.
     */
    bool isSet(int row, int col) const;

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
    void resize(int nRows, int nCols, bool retain = false);

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
     * -----------------------------------------
     * The SparseGrid is internally managed as a dynamic array of elements.
     * The array itself is one-dimensional, the logical separation into
     * rows and columns is done by arithmetic computation.  The layout
     * is in row-major order, which is to say that the entire first row
     * is laid out contiguously, followed by the entire second row,
     * and so on.
     */

    /* Instance variables */
    Map<int, Map<int, ValueType> > elements;  // 2D map of the elements
    int nRows;            // The number of rows in the grid
    int nCols;            // The number of columns in the grid

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
        elements = grid.elements;
        nRows = grid.nRows;
        nCols = grid.nCols;
    }

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
        iterator(const SparseGrid* gp, int index) {
            this->gp = gp;
            this->index = index;
        }

        iterator(const iterator& it) {
            this->gp = it.gp;
            this->index = it.index;
        }

        iterator& operator ++() {
            index++;
            return *this;
        }

        iterator operator ++(int) {
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
            int row = index / gp->nCols;
            int col = index % gp->nCols;
            return gp->elements[row][col];
        }

        // BUGBUG?: Does this work?
        // Aren't I returning a pointer to a stack-allocated local int?
        ValueType* operator ->() {
            int row = index / gp->nCols;
            int col = index % gp->nCols;
            std::cout << "itr opr-> row " << row << ", col " << col << std::endl;
            return &gp->elements[row][col];
        }

    private:
        const SparseGrid* gp;
        int index;
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
        return iterator(this, nRows * nCols);
    }

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
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[row][col];
        }

        ValueType operator [](int col) const {
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[row][col];
        }

    private:
        SparseGridRow(SparseGrid* gridRef, int index) {
            gp = gridRef;
            row = index;
        }

        SparseGrid* gp;
        int row;
        friend class SparseGrid;
    };
    friend class SparseGridRow;

    class SparseGridRowConst {
    public:
        SparseGridRowConst() {
            /* Empty */
        }

        const ValueType operator [](int col) const {
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[row][col];
        }

    private:
        SparseGridRowConst(SparseGrid* const gridRef, int index) : gp(gridRef), row(index) {
            // empty
        }

        const SparseGrid* const gp;
        const int row;
        friend class SparseGrid;
    };
    friend class SparseGridRowConst;
};

template <typename ValueType>
SparseGrid<ValueType>::SparseGrid() {
    nRows = 0;
    nCols = 0;
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
SparseGrid<ValueType>::~SparseGrid() {
    // empty
}

template <typename ValueType>
bool SparseGrid<ValueType>::equals(const SparseGrid<ValueType>& grid2) const {
    // optimization: if literally same grid, stop
    if (this == &grid2) {
        return true;
    }
    if (nRows != grid2.nRows || nCols != grid2.nCols) {
        return false;
    }
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
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
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            set(row, col, value);
        }
    }
}

template <typename ValueType>
ValueType SparseGrid<ValueType>::get(int row, int col) {
    checkIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[row][col];
}

template <typename ValueType>
const ValueType& SparseGrid<ValueType>::get(int row, int col) const {
    checkIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[row][col];
}

template <typename ValueType>
int SparseGrid<ValueType>::height() const {
    return nRows;
}

template <typename ValueType>
bool SparseGrid<ValueType>::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < nRows && col < nCols;
}

template <typename ValueType>
bool SparseGrid<ValueType>::isSet(int row, int col) const {
    return inBounds(row, col) && elements.containsKey(row) &&
            elements[row].containsKey(col);
}

template <typename ValueType>
void SparseGrid<ValueType>::mapAll(void (*fn)(ValueType value)) const {
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
void SparseGrid<ValueType>::mapAll(void (*fn)(const ValueType & value)) const {
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
template <typename FunctorType>
void SparseGrid<ValueType>::mapAll(FunctorType fn) const {
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            if (isSet(row, col)) {
                fn(get(row, col));
            }
        }
    }
}

template <typename ValueType>
int SparseGrid<ValueType>::numCols() const {
    return nCols;
}

template <typename ValueType>
int SparseGrid<ValueType>::numRows() const {
    return nRows;
}

template <typename ValueType>
void SparseGrid<ValueType>::resize(int nRows, int nCols, bool retain) {
    if (nRows < 0 || nCols < 0) {
        std::ostringstream out;
        out << "SparseGrid::resize: Attempt to resize grid to invalid size ("
               << nRows << ", " << nCols << ")";
        error(out.str());
    }
    int oldnRows = this->nRows;
    int oldnCols = this->nCols;
    this->nRows = nRows;
    this->nCols = nCols;
    
    if (retain) {
        // if resizing to a smaller size, must evict any row/col entries
        // that exceed the new grid's bounds
        if (nRows < oldnRows || nCols < oldnCols) {
            Map<int, Map<int, ValueType> > newElements;
            for (int row : elements) {
                if (row >= nRows) {
                    break;   // don't add any entries beyond this row
                }
                for (int col : elements[row]) {
                    if (col >= nCols) {
                        break;   // don't add any entries beyond this row
                    }
                    newElements[row][col] = elements[row][col];
                }
            }
            elements = newElements;
        }
    } else {
        elements.clear();
    }
}

template <typename ValueType>
void SparseGrid<ValueType>::set(int row, int col, const ValueType& value) {
    checkIndexes(row, col, nRows-1, nCols-1, "set");
    elements[row][col] = value;
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
        if (!elements.containsKey(i) || elements[i].isEmpty()) {
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
int SparseGrid<ValueType>::width() const {
    return nCols;
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
 * Template hash function for sparse grids.
 * Requires the element type in the SparseGrid to have a hashCode function.
 */
template <typename T>
int hashCode(const SparseGrid<T>& g) {
    int code = HASH_SEED;
    for (T n : g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
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
    os << grid.elements << ", " << grid.nRows << " x " << grid.nCols;
    return os;
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, SparseGrid<ValueType>& grid) {
    // "{...}, 4 x 3"
    is >> grid.elements;
    std::string comma;
    is >> comma;   // throw away ', ' token
    is >> grid.nRows;
    std::string x;
    is >> x;       // throw away 'x' token
    is >> grid.nCols;
    return is;
}

#endif // _grid_h
