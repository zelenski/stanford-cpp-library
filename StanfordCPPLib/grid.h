/*
 * File: grid.h
 * ------------
 * This file exports the <code>Grid</code> class, which offers a
 * convenient abstraction for representing a two-dimensional array.
 */

#ifndef _grid_h
#define _grid_h

#include "private/foreachpatch.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

static void checkGridIndexes(int row, int col,
                             int rowMax, int colMax, string prefix);

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
    bool equals(const Grid<ValueType>& grid) const;
    
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
     * and columns.  Any previous grid contents are discarded.
     */
    void resize(int nRows, int nCols);

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
     */
    std::string toString() const;

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
    bool operator ==(const Grid& grid2);

    /*
     * Operator: !=
     * Usage: if (grid1 != grid2) ...
     * ------------------------------
     * Compares two grids for inequality.
     */
    bool operator !=(const Grid& grid2);

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
    void checkRange(int row, int col);

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
        GridRow() {
            /* Empty */
        }

        ValueType& operator [](int col) {
            checkGridIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
        }

        ValueType operator [](int col) const {
            checkGridIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
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
        GridRowConst() {
            /* Empty */
        }

        const ValueType operator [](int col) const {
            checkGridIndexes(row, col, gp->nRows-1, gp->nCols-1, "operator [][]");
            return gp->elements[(row * gp->nCols) + col];
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
bool Grid<ValueType>::equals(const Grid<ValueType>& grid) const {
    if (nRows != grid.nRows || nCols != grid.nCols) {
        return false;
    }
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            if (get(row, col) != grid.get(row, col)) {
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
    checkGridIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[(row * nCols) + col];
}

template <typename ValueType>
const ValueType& Grid<ValueType>::get(int row, int col) const {
    checkGridIndexes(row, col, nRows-1, nCols-1, "get");
    return elements[(row * nCols) + col];
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
int Grid<ValueType>::numCols() const {
    return nCols;
}

template <typename ValueType>
int Grid<ValueType>::numRows() const {
    return nRows;
}

template <typename ValueType>
void Grid<ValueType>::resize(int nRows, int nCols) {
    if (nRows < 0 || nCols < 0) {
        ostringstream out;
        out << "Grid::resize: Attempt to resize grid to invalid size ("
               << nRows << ", " << nCols << ")";
        error(out.str());
    }
    if (elements != NULL) {
        delete[] elements;
    }
    this->nRows = nRows;
    this->nCols = nCols;
    elements = new ValueType[nRows * nCols];
    ValueType value = ValueType();
    for (int i = 0; i < nRows * nCols; i++) {
        elements[i] = value;
    }
}

template <typename ValueType>
void Grid<ValueType>::set(int row, int col, const ValueType& value) {
    checkGridIndexes(row, col, nRows-1, nCols-1, "set");
    elements[(row * nCols) + col] = value;
}

template <typename ValueType>
std::string Grid<ValueType>::toString() const {
    ostringstream os;
    os << *this;
    return os.str();
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
bool Grid<ValueType>::operator ==(const Grid& grid2) {
    return equals(grid2);
}

template <typename ValueType>
bool Grid<ValueType>::operator !=(const Grid& grid2) {
    return !(*this == grid2);
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

/*
 * Throws an ErrorException if the given row/col are not within the range of
 * (0,0) through (rowMax-1,colMax-1) inclusive.
 * This is a consolidated error handler for all various Grid members that
 * accept index parameters.
 * The prefix parameter represents a text string to place at the start of
 * the error message, generally to help indicate which member threw the error.
 */
static void checkGridIndexes(int row, int col,
                         int rowMax, int colMax, std::string prefix) {
    const int rowMin = 0;
    const int colMin = 0;
    if (row < rowMin || row > rowMax ||
        col < colMin || col > colMax) {
        ostringstream out;
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

#endif
