/*
 * File: vector.h
 * --------------
 * This file exports the <code>Vector</code> class, which provides an
 * efficient, safe, convenient replacement for the array type in C++.
 *
 * @version 2018/09/06
 * - refreshed doc comments for new documentation generation
 * @version 2018/01/07
 * - added front, back, removeFront, removeBack, pop_front, pop_back, push_front
 * @version 2017/11/15
 * - added contains, indexOf, lastIndexOf, removeValue, reverse, shuffle, sort
 * @version 2017/10/18
 * - fix compiler warnings
 * @version 2016/12/09
 * - added iterator version checking support
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

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _vector_h
#define _vector_h

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "random.h"
#undef INTERNAL_INCLUDE

/**
 * This class stores an ordered list of values similar to an array.
 * It supports traditional array selection using square brackets, but
 * also supports inserting and deleting elements.  It is similar in
 * function to the STL <code>vector</code> type, but is simpler both
 * to use and to implement.
 */
template <typename ValueType>
class Vector {
public:
    /**
     * Initializes a new empty vector.
     * @bigoh O(1)
     */
    Vector();

    /**
     * Initializes a new vector, creating an array with <code>n</code>
     * elements, each of which is initialized to <code>value</code>.
     * If <code>value</code> is missing, the elements are initialized
     * to the default value for the type.
     * @bigoh O(N)
     */
    explicit Vector(int n, ValueType value = ValueType());

    /**
     * Uses an initializer list to set up the vector.
     * @bigoh O(N)
     */
    Vector(std::initializer_list<ValueType> list);

    /**
     * Frees any heap storage allocated by this vector.
     * @bigoh O(1)
     */
    virtual ~Vector();

    /**
     * Adds a new value to the end of this vector.
     * @bigoh O(1)
     */
    void add(const ValueType& value);

    /**
     * Adds all elements of the given other vector to this vector.
     * Returns a reference to this vector.
     * Identical in behavior to the += operator.
     * @bigoh O(N)
     */
    Vector<ValueType>& addAll(const Vector<ValueType>& v);

    /**
     * Adds all elements of the given initializer list to this vector.
     * Returns a reference to this vector.
     * Identical in behavior to the += operator.
     * @bigoh O(N)
     */
    Vector<ValueType>& addAll(std::initializer_list<ValueType> list);

    /**
     * Returns the element at index (size - 1) in this vector (without removing it).
     * @throw ErrorException if vector is empty
     * @bigoh O(1)
     */
    ValueType& back();

    /**
     * Returns the element at index (size - 1) in this vector (without removing it).
     * @throw ErrorException if vector is empty
     * @bigoh O(1)
     */
    const ValueType& back() const;

    /**
     * Removes all elements from this vector.
     * @bigoh O(1)
     */
    void clear();

    /**
     * Returns true if the vector contains the given value.
     * The ValueType must have an == operator to use this method.
     * @bigoh O(N)
     */
    bool contains(const ValueType& value) const;

    /**
     * Guarantees that the vector's internal array is at least the given length.
     * If necessary, resizes the array to be the given length or larger.
     * @bigoh O(N)
     */
    void ensureCapacity(int cap);

    /**
     * Compares two vectors for equality.
     * Returns <code>true</code> if this vector contains exactly the same
     * values as the given other vector.
     * Identical in behavior to the == operator.
     * @bigoh O(N)
     */
    bool equals(const Vector<ValueType>& v) const;

    /**
     * Returns the element at index 0 in this vector (without removing it).
     * @throw ErrorExceptoin if vector is empty
     * @bigoh O(1)
     */
    ValueType& front();

    /**
     * Returns the element at index 0 in this vector (without removing it).
     * @throw ErrorExceptoin if vector is empty
     * @bigoh O(1)
     */
    const ValueType& front() const;

    /**
     * Returns the element at the specified index in this vector.
     * Similar in behavior to the [] operator.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(1)
     */
    const ValueType& get(int index) const;

    /**
     * Returns the index of the first occurrence of the given value.
     * If the value is not found in the vector, returns -1.
     * The ValueType must have an == operator to use this method.
     * @bigoh O(N)
     */
    int indexOf(const ValueType& value) const;

    /**
     * Inserts the element into this vector before the specified index.
     * All subsequent elements are shifted one position to the right.
     * @throw ErrorException if the index is not in the array range from 0
     * up to and including the length of the vector.
     * @bigoh O(N)
     */
    void insert(int index, const ValueType& value);

    /**
     * Returns <code>true</code> if this vector contains no elements.
     * @bigoh O(1)
     */
    bool isEmpty() const;

    /**
     * Returns the index of the last occurrence of the given value.
     * If the value is not found in the vector, returns -1.
     * The ValueType must have an == operator to use this method.
     * @bigoh O(N)
     */
    int lastIndexOf(const ValueType& value) const;

    /**
     * Calls the specified function on each element of the vector in
     * ascending index order.
     * @bigoh O(N)
     */
    void mapAll(void (*fn)(ValueType)) const;

    /**
     * Calls the specified function on each element of the vector in
     * ascending index order.
     * @bigoh O(N)
     */
    void mapAll(void (*fn)(const ValueType&)) const;

    /**
     * Calls the specified function on each element of the vector in
     * ascending index order.
     * @bigoh O(N)
     */
    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    /**
     * Removes and returns the first value of this vector.
     * Equivalent to removeFront.
     * @throw ErrorException if the vector is empty
     * @bigoh O(N)
     */
    ValueType pop_front();

    /**
     * Removes and returns the last value of this vector.
     * Equivalent to removeBack.
     * @throw ErrorException if the vector is empty
     * @bigoh O(1)
     */
    ValueType pop_back();

    /**
     * Adds a new value to the end of this vector.
     * This method is a synonym of the add method that is provided to
     * ensure compatibility with the STL <code>vector</code> class.
     * @bigoh O(1)
     */
    void push_back(const ValueType& value);

    /**
     * Adds a new value to the start of this vector.
     * This method is equivalent to calling insert(0, value) and is provided to
     * improve compatibility with the STL <code>vector</code> class.
     * @bigoh O(N)
     */
    void push_front(const ValueType& value);

    /**
     * Removes the element at the specified index from this vector.
     * All subsequent elements are shifted one position to the left.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(N)
     */
    void remove(int index);

    /**
     * Removes and returns the element at index 0 in this vector.
     * @throw ErrorException if the vector is empty
     * @bigoh O(N)
     */
    ValueType removeFront();

    /**
     * Removes and returns the element at index (size - 1) in this vector.
     * @throw ErrorException if the vector is empty
     * @bigoh O(1)
     */
    ValueType removeBack();

    /**
     * Removes the first occurrence of the element value from this vector.
     * All subsequent elements are shifted one position to the left.
     * If the vector does not contain the given value, has no effect.
     * The ValueType must have an == operator to use this method.
     * @bigoh O(N)
     */
    void removeValue(const ValueType& value);

    /**
     * Reverses the order of the elements in this vector.
     * For example, if vector stores {1, 3, 4, 9}, changes it to store {9, 4, 3, 1}.
     * @bigoh O(N)
     */
    void reverse();

    /**
     * Replaces the element at the specified index in this vector with
     * a new value.  The previous value at that index is overwritten.
     * Similar in behavior to the [] operator.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(1)
     */
    void set(int index, const ValueType& value);
    
    /**
     * Returns the number of elements in this vector.
     * @bigoh O(1)
     */
    int size() const;

    /**
     * Rearranges the order of the elements in this vector into a random order.
     * @bigoh O(N)
     */
    void shuffle();

    /**
     * Rearranges the order of the elements in this vector into sorted order.
     * For example, if vector stores {9, 1, 4, 3}, changes it to store {1, 3, 4, 9}.
     * The ValueType must have an operator < to call this method.
     * @bigoh O(N log N)
     */
    void sort();

    /**
     * Returns a new vector containing the given subset range of elements
     * from this vector. The new vector is a deep copy, not linked to this one.
     * @throw ErrorException if the range (start .. start + length) is not
     *        within the bounds of this vector, or if length is negative
     * @bigoh O(N)
     */
    Vector<ValueType> subList(int start, int length) const;

    /**
     * Converts the vector to a printable string representation
     * such as "{10, 20, 30, 40}".
     * @bigoh O(N)
     */
    std::string toString() const;

    /**
     * Overloads <code>[]</code> to select elements from this vector.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(1)
     */
    ValueType& operator [](int index);

    /**
     * Overloads <code>[]</code> to select elements from this vector.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(1)
     */
    const ValueType& operator [](int index) const;

    /**
     * Concatenates two vectors and returns the result.
     * @bigoh O(N)
     */
    Vector operator +(const Vector& v2) const;

    /**
     * Concatenates this vector with an initializer list such as {1, 2, 3},
     * returning the result.
     * @bigoh O(N)
     */
    Vector operator +(std::initializer_list<ValueType> list) const;

    /**
     * Adds all of the elements from <code>v2</code> to the end of this vector.
     * @bigoh O(N)
     */
    Vector& operator +=(const Vector& v2);

    /**
     * Adds all of the elements from the given initializer list to the end of
     * the vector.
     * @bigoh O(N)
     */
    Vector& operator +=(std::initializer_list<ValueType> list);

    /**
     * Adds the single specified value) to the end of the vector.
     * @bigoh O(1)
     */
    Vector& operator +=(const ValueType& value);


    /**
     * Compares two vectors for equality.
     * The ValueType must have an == operator.
     * @bigoh O(N)
     */
    bool operator ==(const Vector& v2) const;

    /**
     * Compares two vectors for inequality.
     * The ValueType must have a != operator.
     * @bigoh O(N)
     */
    bool operator !=(const Vector& v2) const;

    /**
     * Relational operators to compare two vectors.
     * Each element is compared pairwise to the corresponding element at the
     * same index in the other vector; for example, we first check the values
     * at index 0, then 1, and so on.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(N)
     */
    bool operator <(const Vector& v2) const;

    /**
     * Relational operators to compare two vectors.
     * Each element is compared pairwise to the corresponding element at the
     * same index in the other vector; for example, we first check the values
     * at index 0, then 1, and so on.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(N)
     */
    bool operator <=(const Vector& v2) const;

    /**
     * Relational operators to compare two vectors.
     * Each element is compared pairwise to the corresponding element at the
     * same index in the other vector; for example, we first check the values
     * at index 0, then 1, and so on.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(N)
     */
    bool operator >(const Vector& v2) const;

    /**
     * Relational operators to compare two vectors.
     * Each element is compared pairwise to the corresponding element at the
     * same index in the other vector; for example, we first check the values
     * at index 0, then 1, and so on.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(N)
     */
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
    ValueType* elements;        // a dynamic array of the elements
    int capacity;               // the allocated size of the array
    int count;                  // the number of elements in use
    unsigned int m_version = 0; // structure version for detecting invalid iterators

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
    /**
     * Makes a deep copy, making it possible to pass or return vectors by value
     * and assign from one vector to another.
     * @bigoh O(N)
     * @private
     */
    Vector(const Vector& src);

    /**
     * Makes a deep copy, making it possible to pass or return vectors by value
     * and assign from one vector to another.
     * @bigoh O(N)
     * @private
     */
    Vector& operator =(const Vector& src);

    /**
     * Adds an element to the vector passed as the left-hand operatand.
     * This form makes it easier to initialize vectors in old versions of C++.
     * @bigoh O(1)
     */
    Vector& operator ,(const ValueType& value);

    /**
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     * @private
     */
    class iterator :
            public std::iterator<std::random_access_iterator_tag, ValueType> {
    private:
        const Vector* vp;
        int index;
        unsigned int itr_version;

    public:
        iterator()
                : vp(nullptr),
                  index(0),
                  itr_version(0) {
            // empty
        }

        iterator(const iterator& it)
                : vp(it.vp),
                  index(it.index),
                  itr_version(it.itr_version) {
            // empty
        }

        iterator(const Vector* theVec, int theIndex)
                : vp(theVec),
                  index(theIndex) {
            itr_version = vp->version();
        }

        iterator& operator ++() {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            index++;
            return *this;
        }

        iterator operator ++(int) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            iterator copy(*this);
            operator++();
            return copy;
        }

        iterator& operator --() {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            index--;
            return *this;
        }

        iterator operator --(int) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
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
            stanfordcpplib::collections::checkVersion(*vp, *this);
            return iterator(vp, index + rhs);
        }

        iterator operator +=(const int& rhs) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            index += rhs;
            return *this;
        }

        iterator operator -(const int& rhs) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            return iterator(vp, index - rhs);
        }

        iterator operator -=(const int& rhs) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            index -= rhs;
            return *this;
        }

        int operator -(const iterator& rhs) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            extern void error(const std::string& msg);
            if (vp != rhs.vp) {
                error("Vector Iterator::operator -: Iterators are in different vectors");
            }
            return index - rhs.index;
        }

        ValueType& operator *() {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            return vp->elements[index];
        }

        ValueType* operator ->() {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            return &vp->elements[index];
        }

        ValueType& operator [](int k) {
            stanfordcpplib::collections::checkVersion(*vp, *this);
            return vp->elements[index + k];
        }

        unsigned int version() const {
            return itr_version;
        }
    };

    /**
     * Returns an iterator pointed at the beginning of this vector.
     * @bigoh O(1)
     */
    iterator begin() const {
        return iterator(this, 0);
    }

    /**
     * Returns an iterator pointed just past the end of this vector.
     * @bigoh O(1)
     */
    iterator end() const {
        return iterator(this, count);
    }

    /**
     * Returns the internal version of this collection.
     * This is used to check for invalid iterators and issue error messages.
     * @bigoh O(1)
     */
    unsigned int version() const;
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
Vector<ValueType>::Vector()
        : elements(nullptr),
          capacity(0),
          count(0) {
    // empty
}

template <typename ValueType>
Vector<ValueType>::Vector(int n, ValueType value)
        : elements(nullptr),
          capacity(n),
          count(n) {
    if (n < 0) {
        error("Vector::constructor: n cannot be negative");
    } else if (n > 0) {
        elements = new ValueType[n];
        for (int i = 0; i < n; i++) {
            elements[i] = value;
        }
    }
}

template <typename ValueType>
Vector<ValueType>::Vector(std::initializer_list<ValueType> list)
        : count(0) {
    capacity = list.size();
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
ValueType& Vector<ValueType>::back() {
    if (isEmpty()) {
        error("Vector::back: vector is empty");
    }
    return elements[count - 1];
}

template <typename ValueType>
const ValueType& Vector<ValueType>::back() const {
    if (isEmpty()) {
        error("Vector::back: vector is empty");
    }
    return elements[count - 1];
}

template <typename ValueType>
void Vector<ValueType>::clear() {
    if (elements) {
        delete[] elements;
    }
    count = 0;
    capacity = 0;
    elements = nullptr;
    m_version++;
}

template <typename ValueType>
bool Vector<ValueType>::contains(const ValueType& value) const {
    return indexOf(value) >= 0;
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
    ValueType* array = new ValueType[capacity];
    if (elements) {
        for (int i = 0; i < count; i++) {
            array[i] = elements[i];
        }
        delete[] elements;
    }
    elements = array;
}

template <typename ValueType>
ValueType& Vector<ValueType>::front() {
    if (isEmpty()) {
        error("Vector::front: vector is empty");
    }
    return elements[0];
}

template <typename ValueType>
const ValueType& Vector<ValueType>::front() const {
    if (isEmpty()) {
        error("Vector::front: vector is empty");
    }
    return elements[0];
}

template <typename ValueType>
const ValueType& Vector<ValueType>::get(int index) const {
    checkIndex(index, 0, count-1, "get");
    return elements[index];
}

template <typename ValueType>
int Vector<ValueType>::indexOf(const ValueType& value) const {
    for (int i = 0; i < count; i++) {
        if (elements[i] == value) {
            return i;
        }
    }
    return -1;
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
    m_version++;
}

template <typename ValueType>
bool Vector<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
int Vector<ValueType>::lastIndexOf(const ValueType& value) const {
    for (int i = count - 1; i >= 0; i--) {
        if (elements[i] == value) {
            return i;
        }
    }
    return -1;
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
ValueType Vector<ValueType>::pop_back() {
    if (isEmpty()) {
        error("Vector::pop_back: vector is empty");
    }
    ValueType last = elements[count - 1];
    remove(count - 1);
    return last;
}

template <typename ValueType>
ValueType Vector<ValueType>::pop_front() {
    if (isEmpty()) {
        error("Vector::pop_front: vector is empty");
    }
    ValueType first = elements[0];
    remove(0);
    return first;
}

template <typename ValueType>
void Vector<ValueType>::push_back(const ValueType& value) {
    insert(count, value);
}

template <typename ValueType>
void Vector<ValueType>::push_front(const ValueType& value) {
    insert(0, value);
}

template <typename ValueType>
void Vector<ValueType>::remove(int index) {
    checkIndex(index, 0, count-1, "remove");
    for (int i = index; i < count - 1; i++) {
        elements[i] = elements[i + 1];
    }
    count--;
    m_version++;
}

template <typename ValueType>
ValueType Vector<ValueType>::removeBack() {
    return pop_back();
}

template <typename ValueType>
ValueType Vector<ValueType>::removeFront() {
    return pop_front();
}

template <typename ValueType>
void Vector<ValueType>::removeValue(const ValueType& value) {
    int index = indexOf(value);
    if (index >= 0) {
        remove(index);
    }
}

template <typename ValueType>
void Vector<ValueType>::reverse() {
    for (int i = 0; i < count / 2; i++) {
        std::swap(elements[i], elements[count - 1 - i]);
    }
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
void Vector<ValueType>::shuffle() {
    for (int i = 0; i < count; i++) {
        int j = randomInteger(i, count - 1);
        if (i != j) {
            std::swap(elements[i], elements[j]);
        }
    }
}

template <typename ValueType>
void Vector<ValueType>::sort() {
    std::sort(begin(), end());
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
std::string Vector<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
unsigned int Vector<ValueType>::version() const {
    return m_version;
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

// implementation notes:
// doesn't free this->elements because deepCopy is only called in cases where
// elements is either null (at construction) or has just been freed (operator =)
template <typename ValueType>
void Vector<ValueType>::deepCopy(const Vector& src) {
    count = src.count;
    capacity = src.count;
    elements = (capacity == 0) ? nullptr : new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        elements[i] = src.elements[i];
    }
    m_version++;
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
    return stanfordcpplib::collections::randomElementIndexed(vec);
}

/*
 * Randomly rearranges the elements of the given vector.
 */
template <typename T>
void shuffle(Vector<T>& v) {
    v.shuffle();
}

#endif // _vector_h
