/*
 * File: vector.h
 * --------------
 * This file exports the <code>Vector</code> class, which provides an
 * efficient, safe, convenient replacement for the array type in C++.
 */

#ifndef _vector_h
#define _vector_h

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <type_traits>
#include <functional>

#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "random.h"

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
    Vector() = default;

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
    virtual ~Vector() = default;

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
     * Removes all elements from this vector.
     * @bigoh O(1)
     */
    void clear();

    /**
     * Compares two vectors for equality.
     * Returns <code>true</code> if this vector contains exactly the same
     * values as the given other vector.
     * Identical in behavior to the == operator.
     * @bigoh O(N)
     */
    bool equals(const Vector<ValueType>& v) const;

    /**
     * Returns the element at the specified index in this vector.
     * Similar in behavior to the [] operator.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(1)
     */
    const ValueType& get(int index) const;

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
     * Calls the specified function on each element of the vector in
     * ascending index order.
     * @bigoh O(N)
     */
    void mapAll(std::function<void (const ValueType&)> fn) const;

    /**
     * Removes the element at the specified index from this vector
     * and returns it. All subsequent elements are shifted one
     * position to the left.
     * @throw ErrorException if the index is not in the array range
     * @bigoh O(N)
     */
    ValueType remove(int index);

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
     * Returns a new vector containing the elements from the start position
     * to the end of the vector.
     *
     * @throw ErrorException if start > size()
     * @bigoh O(N)
     */
    Vector<ValueType> subList(int start) const;

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
     * Produces a vector formed by appending the given element to this vector.
     * @bigoh O(N)
     */
    Vector operator +(const ValueType& elem) const;

    /**
     * Adds all of the elements from <code>v2</code> to the end of this vector.
     * @bigoh O(N)
     */
    Vector& operator +=(const Vector& v2);

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
     * The elements are stored in a std::vector, the regular C++ library
     * type representing a sequence of elements. We wrap std::vector because
     * it has no runtime safety checks, something that's tricky to get used
     * to when you're first learning to use these types.
     *
     * There's an edge case in the C++ libraries where std::vector<bool> doesn't
     * work as you might think it does. This is widely regarded as a mistake
     * in the language design and there's been a proposal to fix it for many
     * years now. In the interim, we get around this by falling back on the
     * std::deque type in the event that the client wants to make a
     * Vector<bool>
     */
    using ContainerType = typename std::conditional<std::is_same<ValueType, bool>::value,
                                                    std::deque<bool>,
                                                    std::vector<ValueType>>::type;

    /* Instance variables */
    ContainerType _elements;
    stanfordcpplib::collections::VersionTracker _version;

    /* Private methods */

    /*
     * Throws an ErrorException if the given index is not within the range of
     * [min..max] inclusive.
     * This is a consolidated error handler for all various Vector members that
     * accept index parameters.
     * The prefix parameter represents a text string to place at the start of
     * the error message, generally to help indicate which member threw the error.
     *
     * We make prefix a const char* rather than a std::string to avoid having to
     * construct and then destroy the prefix with each call.
     */
    void checkIndex(int index, int min, int max, const char* prefix) const;

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
     * Adds an element to the vector passed as the left-hand operatand.
     * This form makes it easier to initialize vectors in old versions of C++.
     * @bigoh O(1)
     */
    Vector& operator ,(const ValueType& value);

    using iterator = stanfordcpplib::collections::CheckedIterator<typename ContainerType::iterator>;
    using const_iterator = stanfordcpplib::collections::CheckedIterator<typename ContainerType::const_iterator>;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    /* Updates the internal version count. Only our libraries need this, and they only
     * need it in rare cases where an operation that's semantically mutating but bitwise
     * non-mutating occurs.
     */
    void updateVersion();
};

/* Implementation section */

template <typename ValueType>
Vector<ValueType>::Vector(int n, ValueType value) {
    if (n < 0) error("Cannot create a Vector with a negative number of elements.");
    _elements.assign(n, value);
}

template <typename ValueType>
Vector<ValueType>::Vector(std::initializer_list<ValueType> list)
        : _elements(list) {
}

/*
 * Implementation notes: Vector methods
 * ------------------------------------
 * The basic Vector methods are straightforward and should require
 * no detailed documentation.
 */
template <typename ValueType>
void Vector<ValueType>::add(const ValueType& value) {
    insert(size(), value);
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::addAll(const Vector<ValueType>& v) {
    for (const ValueType& value : v) {
        add(value);
    }
    return *this;   // BUGFIX 2014/04/27
}

template <typename ValueType>
void Vector<ValueType>::clear() {
    _elements.clear();
    _version.update();
}

template <typename ValueType>
bool Vector<ValueType>::equals(const Vector<ValueType>& v) const {
    return stanfordcpplib::collections::equals(*this, v);
}

template <typename ValueType>
const ValueType& Vector<ValueType>::get(int index) const {
    checkIndex(index, 0, size()-1, "get");
    return _elements[index];
}

template <typename ValueType>
void Vector<ValueType>::insert(int index, const ValueType& value) {
    checkIndex(index, 0, size(), "insert");
    _elements.insert(_elements.begin() + index, value);
    _version.update();
}

template <typename ValueType>
bool Vector<ValueType>::isEmpty() const {
    return _elements.empty();
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */
template <typename ValueType>
void Vector<ValueType>::mapAll(std::function<void (const ValueType&)> fn) const {
    for (const auto& elem: _elements) {
        fn(elem);
    }
}

template <typename ValueType>
ValueType Vector<ValueType>::remove(int index) {
    checkIndex(index, 0, size() - 1, "remove");
    ValueType elem = _elements[index];
    _elements.erase(_elements.begin() + index);
    _version.update();
    return elem;
}

template <typename ValueType>
void Vector<ValueType>::set(int index, const ValueType& value) {
    checkIndex(index, 0, size()-1, "set");
    _elements[index] = value;
}

template <typename ValueType>
int Vector<ValueType>::size() const {
    return _elements.size();
}

template <typename ValueType>
void Vector<ValueType>::sort() {
    std::sort(begin(), end());
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::subList(int start, int length) const {
    checkIndex(start, 0, size(), "subList");
    checkIndex(start + length, 0, size(), "subList");
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
Vector<ValueType> Vector<ValueType>::subList(int start) const {
    return subList(start, size() - start);
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
    return const_cast<ValueType&>(static_cast<const Vector &>(*this)[index]);
}
template <typename ValueType>
const ValueType& Vector<ValueType>::operator [](int index) const {
    checkIndex(index, 0, size()-1, "operator []");
    return _elements[index];
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::operator +(const Vector& v2) const {
    Vector<ValueType> result = *this;
    return result.addAll(v2);
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::operator +(const ValueType& elem) const {
    Vector<ValueType> result = *this;
    return result += elem;
}

template <typename ValueType>
Vector<ValueType>& Vector<ValueType>::operator +=(const Vector& v2) {
    return addAll(v2);
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
void Vector<ValueType>::checkIndex(int index, int min, int max, const char* prefix) const {
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
 * Implementation notes: Iterator support
 * --------------------------------------
 * We used the checked iterator type, which requires us to provide information
 * about the full range of values available.
 */
template <typename ValueType>
typename Vector<ValueType>::iterator Vector<ValueType>::begin() {
    return { &_version, _elements.begin(), _elements };
}
template <typename ValueType>
typename Vector<ValueType>::const_iterator Vector<ValueType>::begin() const {
    return { &_version, _elements.begin(), _elements };
}
template <typename ValueType>
typename Vector<ValueType>::iterator Vector<ValueType>::end() {
    return { &_version, _elements.end(), _elements };
}
template <typename ValueType>
typename Vector<ValueType>::const_iterator Vector<ValueType>::end() const {
    return { &_version, _elements.end(), _elements };
}

template <typename ValueType>
void Vector<ValueType>::updateVersion() {
    _version.update();
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


#endif // _vector_h
