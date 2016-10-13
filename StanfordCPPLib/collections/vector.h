/*
 * File: vector.h
 * --------------
 * This file exports the <code>Vector</code> class, which provides an
 * efficient, safe, convenient replacement for the array type in C++.
 *
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/12
 * - bug fix for constructor based on initializer list
 * @version 2016/08/10
 * - added support for std initializer_list usage, such as {1, 2, 3}
 *   in constructor, addAll, +, +=
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/10/13
 * - nulled out pointer fields in destructor after deletion to avoid double-free
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 * @version 2014/10/19
 * - added subList method
 * @version 2014/10/10
 * - removed usage of __foreach macro
 * 2014/07/09
 * - changed checkVectorIndex range checking function into a private member
 *   function to avoid unused-function errors on some newer compilers
 * 2014/04/27
 * - fixed bug in addAll method that was not returning reference properly.
 */

#ifndef _vector_h
#define _vector_h

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "random.h"
#include "strlib.h"

/*
 * Class: Vector<ValueType>
 * ------------------------
 * This class stores an ordered list of values similar to an array.
 * It supports traditional array selection using square brackets, but
 * also supports inserting and deleting elements.  It is similar in
 * function to the STL <code>vector</code> type, but is simpler both
 * to use and to implement.
 */
template <typename ValueType>
class Vector {
public:
    /*
     * Constructor: Vector
     * Usage: Vector<ValueType> vec;
     *        Vector<ValueType> vec(n, value);
     * ---------------------------------------
     * Initializes a new vector.  The default constructor creates an
     * empty vector.  The second form creates an array with <code>n</code>
     * elements, each of which is initialized to <code>value</code>;
     * if <code>value</code> is missing, the elements are initialized
     * to the default value for the type.
     */
    Vector();
    explicit Vector(int n, ValueType value = ValueType());

    /*
     * This constructor copies an STL vector.
     */
    /* implicit */ Vector(const std::vector<ValueType>& v);

    /*
     * This constructor uses an initializer list to set up the vector.
     * Usage: Vector<int> vec {1, 2, 3};
     */
    Vector(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~Vector
     * -------------------
     * Frees any heap storage allocated by this vector.
     */
    virtual ~Vector();

    /*
     * Method: add
     * Usage: vec.add(value);
     * ----------------------
     * Adds a new value to the end of this vector.
     */
    void add(const ValueType& value);

    /*
     * Method: addAll
     * Usage: vec.addAll(v2);
     * ----------------------
     * Adds all elements of the given other vector to this vector.
     * You may also pass an initializer list such as {1, 2, 3}.
     * Returns a reference to this vector.
     * Identical in behavior to the += operator.
     */
    Vector<ValueType>& addAll(const Vector<ValueType>& v);
    Vector<ValueType>& addAll(std::initializer_list<ValueType> list);

    /*
     * Method: clear
     * Usage: vec.clear();
     * -------------------
     * Removes all elements from this vector.
     */
    void clear();
    
    /*
     * Method: ensureCapacity
     * Usage: vec.ensureCapacity(n);
     * -----------------------------
     * Guarantees that the vector's internal array is at least the given length.
     * If necessary, resizes the array to be the given length or larger.
     */
    void ensureCapacity(int cap);

    /*
     * Method: equals
     * Usage: if (vec.equals(v2)) ...
     * ------------------------------
     * Compares two vectors for equality.
     * Returns <code>true</code> if this vector contains exactly the same
     * values as the given other vector.
     * Identical in behavior to the == operator.
     */
    bool equals(const Vector<ValueType>& v) const;
    
    /*
     * Method: get
     * Usage: ValueType val = vec.get(index);
     * --------------------------------------
     * Returns the element at the specified index in this vector.  This
     * method signals an error if the index is not in the array range.
     */
    const ValueType& get(int index) const;
    
    /*
     * Method: insert
     * Usage: vec.insert(0, value);
     * ----------------------------
     * Inserts the element into this vector before the specified index.
     * All subsequent elements are shifted one position to the right.  This
     * method signals an error if the index is outside the range from 0
     * up to and including the length of the vector.
     */
    void insert(int index, const ValueType& value);

    /*
     * Method: isEmpty
     * Usage: if (vec.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this vector contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: mapAll
     * Usage: vec.mapAll(fn);
     * ----------------------
     * Calls the specified function on each element of the vector in
     * ascending index order.
     */
    void mapAll(void (*fn)(ValueType)) const;
    void mapAll(void (*fn)(const ValueType&)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;
    
    /*
     * Method: push_back
     * Usage: vec.push_back(value);
     * ----------------------
     * Adds a new value to the end of this vector.  This method is a synonym
     * of the add method that is provided to ensure compatibility
     * with the <code>vector</code> class in the Standard Template Library.
     */
    void push_back(const ValueType& value);

    /*
     * Method: remove
     * Usage: vec.remove(index);
     * -------------------------
     * Removes the element at the specified index from this vector.
     * All subsequent elements are shifted one position to the left.  This
     * method signals an error if the index is outside the array range.
     */
    void remove(int index);
    
    /*
     * Method: set
     * Usage: vec.set(index, value);
     * -----------------------------
     * Replaces the element at the specified index in this vector with
     * a new value.  The previous value at that index is overwritten.
     * This method signals an error if the index is not in the array range.
     */
    void set(int index, const ValueType& value);
    
    /*
     * Method: size
     * Usage: int nElems = vec.size();
     * -------------------------------
     * Returns the number of elements in this vector.
     */
    int size() const;
    
    /*
     * Method: subList
     * Usage: Vector<ValueType> sub = v.subList(start, length);
     * --------------------------------------------------------
     * Returns a new vector containing the given subset range of elements
     * from this vector. The new vector is a deep copy, not linked to this one.
     * Throws an error if the range (start .. start + length) is not contained
     * within the bounds of this vector, or if length is negative.
     */
    Vector<ValueType> subList(int start, int length) const;

    /*
     * Returns an STL vector object with the same elements as this Vector.
     */
    std::vector<ValueType> toStlVector() const;

    /*
     * Method: toString
     * Usage: string str = vec.toString();
     * -----------------------------------
     * Converts the vector to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: []
     * Usage: vec[index]
     * -----------------
     * Overloads <code>[]</code> to select elements from this vector.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.  This method signals an error if
     * the index is outside the array range.  The file supports two
     * versions of this operator, one for <code>const</code> vectors and
     * one for mutable vectors.
     */
    ValueType& operator [](int index);
    const ValueType& operator [](int index) const;

    /*
     * Operator: +
     * Usage: v1 + v2
     * --------------
     * Concatenates two vectors, or concatenates this vector with an
     * initializer list such as {1, 2, 3}.
     */
    Vector operator +(const Vector& v2) const;
    Vector operator +(std::initializer_list<ValueType> list) const;

    /*
     * Operator: +=
     * Usage: v1 += v2;
     *        v1 += value;
     * -------------------
     * Adds all of the elements from <code>v2</code> (or the single
     * specified value) to <code>v1</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     * As a convenience, the <code>Vector</code> package also overloads the
     * comma operator so that it is possible to initialize a vector like this:
     *
     *<pre>
     *    Vector&lt;int&gt; digits;
     *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
     *</pre>
     */
    Vector& operator +=(const Vector& v2);
    Vector& operator +=(std::initializer_list<ValueType> list);
    Vector& operator +=(const ValueType& value);


    /*
     * Operator: ==
     * Usage: if (vec1 == vec2) ...
     * ----------------------------
     * Compares two vectors for equality.
     */
    bool operator ==(const Vector& v2) const;

    /*
     * Operator: !=
     * Usage: if (vec1 != vec2) ...
     * ----------------------------
     * Compares two vectors for inequality.
     */
    bool operator !=(const Vector& v2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: if (vec1 < vec2) ...
     * ---------------------------
     * Relational operators to compare two vectors.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Vector& v2) const;
    bool operator <=(const Vector& v2) const;
    bool operator >(const Vector& v2) const;
    bool operator >=(const Vector& v2) const;

    /*
     * Additional Vector operations
     * ----------------------------
     * In addition to the methods listed in this interface, the Vector
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement or STL iterators
     *
     * The iteration forms process the Vector in index order.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    /*
     * Implementation notes: Vector data structure
     * -------------------------------------------
     * The elements of the Vector are stored in a dynamic array of
     * the specified element type.  If the space in the array is ever
     * exhausted, the implementation doubles the array capacity.
     */

    /* Instance variables */
    ValueType* elements;        /* A dynamic array of the elements   */
    int capacity;               /* The allocated size of the array   */
    int count;                  /* The number of elements in use     */

    /* Private methods */

    /*
     * Throws an ErrorException if the given index is not within the range of
     * [min..max] inclusive.
     * This is a consolidated error handler for all various Vector members that
     * accept index parameters.
     * The prefix parameter represents a text string to place at the start of
     * the error message, generally to help indicate which member threw the error.
     */
    void checkIndex(int index, int min, int max, std::string prefix) const;

    void expandCapacity();
    void deepCopy(const Vector& src);

    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support deep copying and iteration.  Including these methods
     * in the public interface would make that interface more
     * difficult to understand for the average client.
     */

public:
    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a deep copy,
     * making it possible to pass or return vectors by value and assign
     * from one vector to another.
     */
    Vector(const Vector& src);
    Vector& operator =(const Vector& src);

    /*
     * Operator: ,
     * -----------
     * Adds an element to the vector passed as the left-hand operatand.
     * This form makes it easier to initialize vectors in old versions of C++.
     */
    Vector& operator ,(const ValueType& value);

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator :
            public std::iterator<std::random_access_iterator_tag, ValueType> {
    private:
        const Vector* vp;
        int index;

    public:
        iterator() {
            this->vp = nullptr;
            this->index = 0;
        }

        iterator(const iterator& it) {
            this->vp = it.vp;
            this->index = it.index;
        }

        iterator(const Vector* vp, int index) {
            this->vp = vp;
            this->index = index;
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

        iterator& operator --() {
            index--;
            return *this;
        }

        iterator operator --(int) {
            iterator copy(*this);
            operator--();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return vp == rhs.vp && index == rhs.index;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        bool operator <(const iterator& rhs) {
            if (vp != rhs.vp) {
                error("Vector Iterator::operator <: Iterators are in different vectors");
            }
            return index < rhs.index;
        }

        bool operator <=(const iterator& rhs) {
            if (vp != rhs.vp) {
                error("Vector Iterator::operator <=: Iterators are in different vectors");
            }
            return index <= rhs.index;
        }

        bool operator >(const iterator& rhs) {
            if (vp != rhs.vp) {
                error("Vector Iterator::operator >: Iterators are in different vectors");
            }
            return index > rhs.index;
        }

        bool operator >=(const iterator& rhs) {
            if (vp != rhs.vp) {
                error("Vector Iterator::operator >=: Iterators are in different vectors");
            }
            return index >= rhs.index;
        }

        iterator operator +(const int& rhs) {
            return iterator(vp, index + rhs);
        }

        iterator operator +=(const int& rhs) {
            index += rhs;
            return *this;
        }

        iterator operator -(const int& rhs) {
            return iterator(vp, index - rhs);
        }

        iterator operator -=(const int& rhs) {
            index -= rhs;
            return *this;
        }

        int operator -(const iterator& rhs) {
            extern void error(std::string msg);
            if (vp != rhs.vp) {
                error("Vector Iterator::operator -: Iterators are in different vectors");
            }
            return index - rhs.index;
        }

        ValueType& operator *() {
            return vp->elements[index];
        }

        ValueType* operator ->() {
            return &vp->elements[index];
        }

        ValueType& operator [](int k) {
            return vp->elements[index + k];
        }
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
        return iterator(this, count);
    }
};

/* Implementation section */

/*
 * Implementation notes: Vector constructor and destructor
 * -------------------------------------------------------
 * The constructor allocates storage for the dynamic array
 * and initializes the other fields of the object.  The
 * destructor frees the memory used for the array.
 */
template <typename ValueType>
Vector<ValueType>::Vector() {
    count = capacity = 0;
    elements = nullptr;
}

template <typename ValueType>
Vector<ValueType>::Vector(int n, ValueType value) {
    count = capacity = n;
    elements = (n == 0) ? nullptr : new ValueType[n];
    for (int i = 0; i < n; i++) {
        elements[i] = value;
    }
}

template <typename ValueType>
Vector<ValueType>::Vector(const std::vector<ValueType>& v) {
    count = capacity = v.size();
    elements = new ValueType[count];
    for (int i = 0; i < count; i++) {
        elements[i] = v[i];
    }
}

template <typename ValueType>
Vector<ValueType>::Vector(std::initializer_list<ValueType> list) {
    capacity = list.size();
    count = 0;
    elements = new ValueType[capacity];
    addAll(list);
}

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow a standard paradigm,
 * as described in the associated textbook.
 */
template <typename ValueType>
Vector<ValueType>::Vector(const Vector& src) {
    deepCopy(src);
}

template <typename ValueType>
Vector<ValueType>::~Vector() {
    if (elements) {
        delete[] elements;
        elements = nullptr;
    }
}

/*
 * Implementation notes: Vector methods
 * ------------------------------------
 * The basic Vector methods are straightforward and should require
 * no detailed documentation.
 */
template <typename ValueType>
void Vector<ValueType>::add(const ValueType& value) {
    insert(count, value);
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::addAll(const Vector<ValueType>& v) {
    for (const ValueType& value : v) {
        add(value);
    }
    return *this;   // BUGFIX 2014/04/27
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::addAll(std::initializer_list<ValueType> list) {
    for (const ValueType& value : list) {
        add(value);
    }
    return *this;
}

template <typename ValueType>
void Vector<ValueType>::clear() {
    if (elements) {
        delete[] elements;
    }
    count = capacity = 0;
    elements = nullptr;
}

// implementation note: This method is public so clients can guarantee a given
// capacity.  Internal resizing is automatically done by expandCapacity.
// See also: expandCapacity
template <typename ValueType>
void Vector<ValueType>::ensureCapacity(int cap) {
    if (cap >= 1 && capacity < cap) {
        capacity = std::max(cap, capacity * 2);
        ValueType* array = new ValueType[capacity];
        if (elements) {
            for (int i = 0; i < count; i++) {
                array[i] = elements[i];
            }
            delete[] elements;
        }
        elements = array;
    }
}

template <typename ValueType>
bool Vector<ValueType>::equals(const Vector<ValueType>& v) const {
    return stanfordcpplib::collections::equals(*this, v);
}

/*
 * Implementation notes: expandCapacity
 * ------------------------------------
 * This function doubles the array capacity, copies the old elements
 * into the new array, and then frees the old one.
 * See also: ensureCapacity
 */
template <typename ValueType>
void Vector<ValueType>::expandCapacity() {
    capacity = std::max(1, capacity * 2);
    ValueType *array = new ValueType[capacity];
    if (elements) {
        for (int i = 0; i < count; i++) {
            array[i] = elements[i];
        }
        delete[] elements;
    }
    elements = array;
}

template <typename ValueType>
const ValueType& Vector<ValueType>::get(int index) const {
    checkIndex(index, 0, count-1, "get");
    return elements[index];
}

/*
 * Implementation notes: insert, remove, add
 * -----------------------------------------
 * These methods must shift the existing elements in the array to
 * make room for a new element or to close up the space left by a
 * deleted one.
 */
template <typename ValueType>
void Vector<ValueType>::insert(int index, const ValueType& value) {
    checkIndex(index, 0, count, "insert");
    if (count == capacity) {
        expandCapacity();
    }
    for (int i = count; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index] = value;
    count++;
}

template <typename ValueType>
bool Vector<ValueType>::isEmpty() const {
    return count == 0;
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */
template <typename ValueType>
void Vector<ValueType>::mapAll(void (*fn)(ValueType)) const {
    for (int i = 0; i < count; i++) {
        fn(elements[i]);
    }
}

template <typename ValueType>
void Vector<ValueType>::mapAll(void (*fn)(const ValueType&)) const {
    for (int i = 0; i < count; i++) {
        fn(elements[i]);
    }
}

template <typename ValueType>
template <typename FunctorType>
void Vector<ValueType>::mapAll(FunctorType fn) const {
    for (int i = 0; i < count; i++) {
        fn(elements[i]);
    }
}

template <typename ValueType>
void Vector<ValueType>::push_back(const ValueType& value) {
    insert(count, value);
}

template <typename ValueType>
void Vector<ValueType>::remove(int index) {
    checkIndex(index, 0, count-1, "remove");
    for (int i = index; i < count - 1; i++) {
        elements[i] = elements[i + 1];
    }
    count--;
}

template <typename ValueType>
void Vector<ValueType>::set(int index, const ValueType& value) {
    checkIndex(index, 0, count-1, "set");
    elements[index] = value;
}

template <typename ValueType>
int Vector<ValueType>::size() const {
    return count;
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::subList(int start, int length) const {
    checkIndex(start, 0, count, "subList");
    checkIndex(start + length, 0, count, "subList");
    if (length < 0) {
        error("Vector::subList: length cannot be negative");
    }
    Vector<ValueType> result;
    for (int i = start; i < start + length; i++) {
        result.add(get(i));
    }
    return result;
}

template <typename ValueType>
std::vector<ValueType> Vector<ValueType>::toStlVector() const {
    std::vector<ValueType> v;
    for (int i = 0; i < count; i++) {
        v.push_back(elements[i]);
    }
    return v;
}

template <typename ValueType>
std::string Vector<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: Vector selection
 * --------------------------------------
 * The following code implements traditional array selection using
 * square brackets for the index.
 */
template <typename ValueType>
ValueType& Vector<ValueType>::operator [](int index) {
    checkIndex(index, 0, count-1, "operator []");
    return elements[index];
}
template <typename ValueType>
const ValueType& Vector<ValueType>::operator [](int index) const {
    checkIndex(index, 0, count-1, "operator []");
    return elements[index];
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::operator +(const Vector& v2) const {
    Vector<ValueType> result = *this;
    return result.addAll(v2);
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::operator +(std::initializer_list<ValueType> list) const {
    Vector<ValueType> result = *this;
    return result.addAll(list);
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::operator +=(const Vector& v2) {
    return addAll(v2);
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::operator +=(std::initializer_list<ValueType> list) {
    return addAll(list);
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::operator +=(const ValueType& value) {
    add(value);
    return *this;
}

template <typename ValueType>
bool Vector<ValueType>::operator ==(const Vector& v2) const {
    return equals(v2);
}

template <typename ValueType>
bool Vector<ValueType>::operator !=(const Vector& v2) const {
    return !equals(v2);
}

template <typename ValueType>
bool Vector<ValueType>::operator <(const Vector& v2) const {
    return stanfordcpplib::collections::compare(*this, v2) < 0;
}

template <typename ValueType>
bool Vector<ValueType>::operator <=(const Vector& v2) const {
    return stanfordcpplib::collections::compare(*this, v2) <= 0;
}

template <typename ValueType>
bool Vector<ValueType>::operator >(const Vector& v2) const {
    return stanfordcpplib::collections::compare(*this, v2) > 0;
}

template <typename ValueType>
bool Vector<ValueType>::operator >=(const Vector& v2) const {
    return stanfordcpplib::collections::compare(*this, v2) >= 0;
}

template <typename ValueType>
Vector<ValueType> & Vector<ValueType>::operator =(const Vector& src) {
    if (this != &src) {
        if (elements) {
            delete[] elements;
        }
        deepCopy(src);
    }
    return *this;
}

template <typename ValueType>
void Vector<ValueType>::checkIndex(int index, int min, int max, std::string prefix) const {
    if (index < min || index > max) {
        std::ostringstream out;
        out << "Vector::" << prefix << ": index of " << index
            << " is outside of valid range ";
        if (isEmpty()) {
            out << " (empty vector)";
        } else {
            out << "[";
            if (min < max) {
                out << min << ".." << max;
            } else if (min == max) {
                out << min;
            } // else min > max, no range, empty vector
            out << "]";
        }
        error(out.str());
    }
}

template <typename ValueType>
void Vector<ValueType>::deepCopy(const Vector& src) {
    count = capacity = src.count;
    elements = (capacity == 0) ? nullptr : new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        elements[i] = src.elements[i];
    }
}

/*
 * Implementation notes: The , operator
 * ------------------------------------
 * The comma operator works adding the right operand to the vector and
 * then returning the vector by reference so that it is set for the next
 * value in the chain.
 */
template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::operator ,(const ValueType& value) {
    add(value);
    return *this;
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Vector<ValueType>& vec) {
    return stanfordcpplib::collections::writeCollection(os, vec);
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Vector<ValueType>& vec) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, vec, element, /* descriptor */ "Vector::operator >>");
}

/*
 * Template hash function for vectors.
 * Requires the element type in the Vector to have a hashCode function.
 */
template <typename ValueType>
int hashCode(const Vector<ValueType>& vec) {
    return stanfordcpplib::collections::hashCodeCollection(vec);
}

/*
 * Function: randomElement
 * Usage: element = randomElement(v);
 * ----------------------------------
 * Returns a randomly chosen element of the given vector.
 * Throws an error if the vector is empty.
 */
template <typename T>
const T& randomElement(const Vector<T>& vec) {
    return stanfordcpplib::collections::randomElement(vec);
}

/*
 * Randomly rearranges the elements of the given vector.
 */
template <typename T>
void shuffle(Vector<T>& v) {
    for (int i = 0, length = v.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _vector_h
