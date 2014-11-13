/*
 * File: grid.h
 * ------------
 * This file exports the <code>Grid</code> class, which offers a
 * convenient abstraction for representing a two-dimensional array.
 *
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 * @version 2014/10/10
 * - added resize(true) function with ability to retain old contents
 * - made ==, != operators const as they should be
 * - added comparison operators ==, !=
 * 2014/08/16
 * - added width, height functions; added mapAllColumnMajor
 * 2014/07/09
 * - changed checkGridIndexes range checking function into a private member
 *   function to avoid unused-function errors on some newer compilers
 */

#ifndef _grid_h
#define _grid_h

#include <iostream>
#include <string>
#include <sstream>
#include "error.h"
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
    Grid();
    Grid(int nRows, int nCols);
    Grid(int nRows, int nCols, const ValueType& value);

    /*
     * Destructor: ~Grid
     * -----------------
     * Frees any heap storage associated with this grid.
     */
    virtual ~Grid();
    
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
     * Method: mapAllColumnMajor
     * Usage: grid.mapAllColumnMajor(fn);
     * ----------------------------------
     * Calls the specified function on each element of the grid.  The
     * elements are processed in <b><i>column-major order,</i></b> in which
     * all the elements of column 0 are processed, followed by the elements
     * in column 1, and so on.
     */
    void mapAllColumnMajor(void (*fn)(ValueType value)) const;
    void mapAllColumnMajor(void (*fn)(const ValueType& value)) const;

    template <typename FunctorType>
    void mapAllColumnMajor(FunctorType fn) const;

    /*
     * Method: numCols
     * Usage: int nCols = grid.numCols();
     * ----------------------------------
     * Returns the number of columns in the grid.
     * This is equal to the grid's width.
     */
    int numCols() const;

    /*
     * Method: numRows
     * Usage: int nRows = grid.numRows();
     * ----------------------------------
     * Returns the number of rows in the grid.
     * This is equal to the grid's height.
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
     * get or set individual elements.  This method signals an error if
     * the <code>row</code> and <code>col</code> arguments are outside
     * the grid boundaries.
     */
    GridRow operator [](int row);
    const GridRowConst operator [](int row) const;

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

    /* Instance variables */
    ValueType* elements;  /* A dynamic array of the elements   */
    int nRows;            /* The number of rows in the grid    */
    int nCols;            /* The number of columns in the grid */

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
    void deepCopy(const Grid& grid) {
        int n = grid.nRows * grid.nCols;
        elements = new ValueType[n];
        for (int i = 0; i < n; i++) {
            elements[i] = grid.elements[i];
        }
        nRows = grid.nRows;
        nCols = grid.nCols;
    }

public:
    Grid& operator =(const Grid& src) {
        if (this != &src) {
            delete[] elements;
            deepCopy(src);
        }
        return *this;
    }

    Grid(const Grid& src) {
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
        iterator(const Grid* gp, int index) {
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

        ValueType& operator *() {
            return gp->elements[index];
        }

        ValueType* operator ->() {
            return &gp->elements[index];
        }

    private:
        const Grid* gp;
        int index;
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
        return iterator(this, nRows * nCols);
    }

    /*
     * Private class: Grid<ValType>::GridRow
     * -------------------------------------
     * This section of the code defines a nested class within the Grid template
     * that makes it possible to use traditional subscripting on Grid values.
     */
    class GridRow {
    public:
        GridRow() : gp(NULL), row(0) {
            /* Empty */
        }

        ValueType& operator [](int col) {
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
        }

        ValueType operator [](int col) const {
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
        }

        int size() const {
            return gp->width();
        }

    private:
        GridRow(Grid* gridRef, int index) {
            gp = gridRef;
            row = index;
        }

        Grid* gp;
        int row;
        friend class Grid;
    };
    friend class GridRow;

    class GridRowConst {
    public:
        GridRowConst() : gp(NULL), row(0) {
            /* Empty */
        }

        const ValueType operator [](int col) const {
            gp->checkIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
        }

        int size() const {
            return gp->width();
        }

    private:
        GridRowConst(Grid* const gridRef, int index) : gp(gridRef), row(index) {}

        const Grid* const gp;
        const int row;
        friend class Grid;
    };
    friend class GridRowConst;
};

template <typename ValueType>
Grid<ValueType>::Grid() {
    elements = NULL;
    nRows = 0;
    nCols = 0;
}

template <typename ValueType>
Grid<ValueType>::Grid(int nRows, int nCols) {
    elements = NULL;
    resize(nRows, nCols);
}

template <typename ValueType>
Grid<ValueType>::Grid(int nRows, int nCols, const ValueType& value) {
    elements = NULL;
    resize(nRows, nCols);
    fill(value);
}

template <typename ValueType>
Grid<ValueType>::~Grid() {
    if (elements != NULL) {
        delete[] elements;
    }
}

template <typename ValueType>
bool Grid<ValueType>::equals(const Grid<ValueType>& grid2) const {
    // optimization: if literally same grid, stop
    if (this == &grid2) {
        return true;
    }
    
    if (nRows != grid2.nRows || nCols != grid2.nCols) {
        return false;
    }
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            if (get(row, col) != grid2.get(row, col)) {
                return false;
            }
        }
    }
    return true;
}

template <typename ValueType>
void Grid<ValueType>::fill(const ValueType& value) {
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            set(row, col, value);
        }
    }
}

template <typename ValueType>
ValueType Grid<ValueType>::get(int row, int col) {
    checkIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[(row * nCols) + col];
}

template <typename ValueType>
const ValueType& Grid<ValueType>::get(int row, int col) const {
    checkIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[(row * nCols) + col];
}

template <typename ValueType>
int Grid<ValueType>::height() const {
    return nRows;
}

template <typename ValueType>
bool Grid<ValueType>::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < nRows && col < nCols;
}

template <typename ValueType>
void Grid<ValueType>::mapAll(void (*fn)(ValueType value)) const {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
void Grid<ValueType>::mapAll(void (*fn)(const ValueType & value)) const {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
template <typename FunctorType>
void Grid<ValueType>::mapAll(FunctorType fn) const {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
void Grid<ValueType>::mapAllColumnMajor(void (*fn)(ValueType value)) const {
    for (int j = 0; j < nCols; j++) {
        for (int i = 0; i < nRows; i++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
void Grid<ValueType>::mapAllColumnMajor(void (*fn)(const ValueType& value)) const {
    for (int j = 0; j < nCols; j++) {
        for (int i = 0; i < nRows; i++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
template <typename FunctorType>
void Grid<ValueType>::mapAllColumnMajor(FunctorType fn) const {
    for (int j = 0; j < nCols; j++) {
        for (int i = 0; i < nRows; i++) {
            fn(get(i, j));
        }
    }
}

template <typename ValueType>
int Grid<ValueType>::numCols() const {
    return nCols;
}

template <typename ValueType>
int Grid<ValueType>::numRows() const {
    return nRows;
}

template <typename ValueType>
void Grid<ValueType>::resize(int nRows, int nCols, bool retain) {
    if (nRows < 0 || nCols < 0) {
        std::ostringstream out;
        out << "Grid::resize: Attempt to resize grid to invalid size ("
               << nRows << ", " << nCols << ")";
        error(out.str());
    }
    
    // save backup of old array/size
    ValueType* oldElements = elements;
    int oldnRows = this->nRows;
    int oldnCols = this->nCols;
    
    // create new empty array and set new size
    this->nRows = nRows;
    this->nCols = nCols;
    elements = new ValueType[nRows * nCols];
    
    // initialize to empty/default state
    ValueType value = ValueType();
    for (int i = 0; i < nRows * nCols; i++) {
        elements[i] = value;
    }
    
    // possibly retain old contents
    if (retain) {
        int minRows = oldnRows < nRows ? oldnRows : nRows;
        int minCols = oldnCols < nCols ? oldnCols : nCols;
        for (int row = 0; row < minRows; row++) {
            for (int col = 0; col < minCols; col++) {
                elements[(row * nCols) + col] = oldElements[(row * oldnRows) + col];
            }
        }
    }
    
    // free old array memory
    if (oldElements != NULL) {
        delete[] oldElements;
    }
}

template <typename ValueType>
void Grid<ValueType>::set(int row, int col, const ValueType& value) {
    checkIndexes(row, col, nRows-1, nCols-1, "set");
    elements[(row * nCols) + col] = value;
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
    int nRows = numRows();
    int nCols = numCols();
    for (int i = 0; i < nRows; i++) {
        if (i > 0) {
            os << rowSeparator;
        }
        os << rowStart;
        for (int j = 0; j < nCols; j++) {
            if (j > 0) {
                os << colSeparator;
            }
            writeGenericValue(os, get(i, j), true);
        }
        os << rowEnd;
    }
    os << rowEnd;
    return os.str();
}

template <typename ValueType>
int Grid<ValueType>::width() const {
    return nCols;
}

template <typename ValueType>
typename Grid<ValueType>::GridRow Grid<ValueType>::operator [](int row) {
    return GridRow(this, row);
}

template <typename ValueType>
const typename Grid<ValueType>::GridRowConst
Grid<ValueType>::operator [](int row) const {
    return GridRowConst(const_cast<Grid*>(this), row);
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
            
            if (get(r, c) < grid2.get(r, c)) {
                return -1;
            } else if (grid2.get(r, c) < get(r, c)) {
                return 1;
            }
        }
    }
    return 0;
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
            writeGenericValue(os, grid.get(i, j), true);
        }
        os << "}";
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Grid<ValueType>& grid) {
    Vector<Vector<ValueType> > vec2d;
    is >> vec2d;
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
 * Template hash function for grids.
 * Requires the element type in the Grid to have a hashCode function.
 */
template <typename T>
int hashCode(const Grid<T>& g) {
    int code = HASH_SEED;
    for (T n : g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

/*
 * Randomly rearranges the elements of the given grid.
 */
template <typename T>
void shuffle(Grid<T>& grid) {
    int rows = grid.numRows();
    int cols = grid.numCols();
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;
            T temp = grid[r1][c1];
            grid[r1][c1] = grid[r2][c2];
            grid[r2][c2] = temp;
        }
    }
}

#endif
