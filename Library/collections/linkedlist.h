/*
 * File: linkedlist.h
 * ------------------
 * This file exports the <code>LinkedList</code> class, which provides an
 * implementation of a doubly-linked list of objects and provides a
 * public interface similar to that of the <code>Vector</code> class.
 */

#ifndef _linkedlist_h
#define _linkedlist_h

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <functional>

#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

/*
 * Class: LinkedList<ValueType>
 * ----------------------------
 * This class stores an ordered list of values similar to an array.
 * It supports traditional array selection using square brackets, but
 * also supports inserting and deleting elements.  It is similar in
 * function to the STL <code>list</code> type.
 */
template <typename ValueType>
class LinkedList {
public:
    /*
     * Constructor: LinkedList
     * Usage: LinkedList<ValueType> list;
     * ----------------------------------
     * Initializes a new LinkedList.  The default constructor creates an
     * empty LinkedList.
     */
    LinkedList() = default;
    /* implicit */ LinkedList(const std::list<ValueType>& v);

    /*
     * This constructor uses an initializer list to set up the linked list.
     * Usage: LinkedList<int> list {1, 2, 3};
     */
    LinkedList(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~LinkedList
     * -------------------
     * Frees any heap storage allocated by this LinkedList.
     */
    virtual ~LinkedList() = default;

    /*
     * Method: add
     * Usage: list.add(value);
     * ----------------------
     * Adds a new value to the end of this LinkedList.
     */
    void add(ValueType value);

    /*
     * Method: addAll
     * Usage: list.addAll(l2);
     * -----------------------
     * Adds all elements of the given other linked list to this list.
     * Returns a reference to this list.
     * Identical in behavior to the += operator.
     */
    LinkedList<ValueType>& addAll(const LinkedList<ValueType>& list);

    /*
     * Method: clear
     * Usage: list.clear();
     * --------------------
     * Removes all elements from this LinkedList.
     */
    void clear();

    /*
     * Method: equals
     * Usage: if (list.equals(l2)) ...
     * -------------------------------
     * Returns <code>true</code> if this linked list contains exactly the same
     * values as the given other list.
     * Identical in behavior to the == operator.
     */
    bool equals(const LinkedList<ValueType>& l2) const;

    /*
     * Method: get
     * Usage: ValueType val = list.get(index);
     * ---------------------------------------
     * Returns the element at the specified index in this LinkedList.  This
     * method signals an error if the index is not in the list range.
     *
     * The client should be mindful that unlike with a vector, this operation
     * is O(N) for linked lists because it must traverse the list to reach
     * the given index.
     */
    const ValueType& get(int index) const;

    /*
     * Method: insert
     * Usage: list.insert(0, value);
     * -----------------------------
     * Inserts the element into this LinkedList before the specified index.
     * This method signals an error if the index is outside the range from 0
     * up to and including the length of the LinkedList.
     */
    void insert(int index, ValueType value);

    /*
     * Method: isEmpty
     * Usage: if (list.isEmpty()) ...
     * ------------------------------
     * Returns <code>true</code> if this LinkedList contains no elements.
     */
    bool isEmpty() const;

    /*
     * Method: mapAll
     * Usage: list.mapAll(fn);
     * ----------------------
     * Calls the specified function on each element of the LinkedList in
     * ascending index order.
     */
    void mapAll(std::function<void (const ValueType &)> fn) const;

    /*
     * Method: remove
     * Usage: list.remove(index);
     * -------------------------
     * Removes the element at the specified index from this LinkedList.
     * This method signals an error if the index is outside the list range.
     */
    void remove(int index);

    /*
     * Method: set
     * Usage: list.set(index, value);
     * ------------------------------
     * Replaces the element at the specified index in this LinkedList with
     * a new value.  The previous value at that index is overwritten.
     * This method signals an error if the index is not in the list range.
     *
     * The client should be mindful that unlike with a vector, this operation
     * is O(N) for linked lists because it must traverse the list to reach
     * the given index.
     */
    void set(int index, const ValueType& value);

    /*
     * Method: size
     * Usage: int nElems = list.size();
     * --------------------------------
     * Returns the number of elements in this LinkedList.
     */
    int size() const;

    /*
     * Method: sort
     * Usage: list.sort();
     * -------------------
     * Rearranges the order of the elements in this list into sorted order.
     * For example, if the list stores {9, 1, 4, 3}, changes it to store {1, 3, 4, 9}.
     * The ValueType must have an operator < to call this method.
     */
    void sort();

    /*
     * Method: subList
     * Usage: LinkedList<ValueType> sub = list.subList(start, length);
     * ---------------------------------------------------------------
     * Returns a new list containing the given subset range of elements
     * from this list. The new list is a deep copy, not linked to this one.
     * Throws an error if the range (start .. start + length) is not contained
     * within the bounds of this list, or if length is negative.
     */
    LinkedList<ValueType> subList(int start, int length) const;

    /*
     * Method: toString
     * Usage: string str = list.toString();
     * ------------------------------------
     * Converts the LinkedList to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: []
     * Usage: list[index]
     * -----------------
     * Overloads <code>[]</code> to select elements from this LinkedList.
     * This extension enables the use of traditional array notation to
     * get or set individual elements.  This method signals an error if
     * the index is outside the list range.  The file supports two
     * versions of this operator, one for <code>const</code> LinkedLists and
     * one for mutable LinkedLists.
     *
     * The client should be mindful that this operation is O(N) for linked
     * lists because it must traverse the list to reach the given index.
     */
    ValueType& operator [](int index);
    const ValueType& operator [](int index) const;

    /*
     * Operator: +
     * Usage: v1 + l2
     * --------------
     * Concatenates two LinkedLists, or concatenates this linked list with an
     * initializer list such as {1, 2, 3}.
     */
    LinkedList operator +(const LinkedList& l2) const;
    LinkedList operator +(std::initializer_list<ValueType> list) const;

    /*
     * Operator: +=
     * Usage: l1 += l2;
     *        l1 += value;
     * -------------------
     * Adds all of the elements from <code>l2</code> (or the single
     * specified value) to <code>l1</code>.  As a convenience, the
     * <code>LinkedList</code> package also overloads the comma operator so
     * that it is possible to initialize a LinkedList like this:
     *
     *<pre>
     *    LinkedList&lt;int&gt; digits;
     *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
     *</pre>
     */
    LinkedList& operator +=(const LinkedList& l2);
    LinkedList& operator +=(const ValueType& value);

    /*
     * Comparing LinkedLists for equality.
     */
    bool operator ==(const LinkedList& list2) const;
    bool operator !=(const LinkedList& list2) const;


    /*
     * Operators: <, >, <=, >=
     * Usage: if (list1 == list2) ...
     * ...
     * -------------------------------
     * Relational operators to compare two lists.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const LinkedList& list2) const;
    bool operator <=(const LinkedList& list2) const;
    bool operator >(const LinkedList& list2) const;
    bool operator >=(const LinkedList& list2) const;

    /*
     * Additional LinkedList operations
     * --------------------------------
     * In addition to the methods listed in this interface, the LinkedList
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement or STL iterators
     *
     * The iteration forms process the LinkedList in index order.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    /*
     * Implementation notes: LinkedList data structure
     * -------------------------------------------
     * The elements of the LinkedList are stored in a dynamic array of
     * the specified element type.  If the space in the array is ever
     * exhausted, the implementation doubles the array capacity.
     */

    /* Instance variables */
    std::list<ValueType> _elements;   // STL linked list as backing storage
    stanfordcpplib::collections::VersionTracker _version;

    /* Private methods */

    /*
     * Throws an ErrorException if the given index is not within the range of
     * [min..max] inclusive.
     * This is a consolidated error handler for all various LinkedList members that
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

    /*
     * Operator: ,
     * -----------
     * Adds an element to the LinkedList passed as the left-hand operatand.
     * This form makes it easier to initialize LinkedLists in old versions of C++.
     */
    LinkedList& operator ,(const ValueType& value);

    /*
     * Iterator support.
     */
    using iterator = stanfordcpplib::collections::CheckedIterator<typename std::list<ValueType>::iterator>;
    using const_iterator = stanfordcpplib::collections::CheckedIterator<typename std::list<ValueType>::const_iterator>;

    iterator begin() {
        return { &_version, _elements.begin(), _elements };
    }
    iterator end() {
        return { &_version, _elements.end(), _elements };
    }
    const_iterator begin() const {
        return { &_version, _elements.begin(), _elements };
    }
    const_iterator end() const {
        return { &_version, _elements.end(), _elements };
    }
};

/* Implementation section */

template <typename ValueType>
LinkedList<ValueType>::LinkedList(const std::list<ValueType>& v)
        : _elements(v) {
    // empty
}

template <typename ValueType>
LinkedList<ValueType>::LinkedList(std::initializer_list<ValueType> list) : _elements(list) {
    // empty
}

template <typename ValueType>
void LinkedList<ValueType>::add(ValueType value) {
    _elements.push_back(value);
    _version.update();
}

template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::addAll(const LinkedList<ValueType>& list) {
    for (const ValueType& value : list) {
        add(value);
    }
    _version.update();
    return *this;
}

template <typename ValueType>
void LinkedList<ValueType>::clear() {
    _elements.clear();
    _version.update();
}


template <typename ValueType>
bool LinkedList<ValueType>::equals(const LinkedList<ValueType>& list2) const {
    return _elements == list2._elements;
}


template <typename ValueType>
const ValueType & LinkedList<ValueType>::get(int index) const {
    checkIndex(index, 0, size()-1, "get");
    return *std::next(_elements.begin(), index);
}

template <typename ValueType>
void LinkedList<ValueType>::insert(int index, ValueType value) {
    checkIndex(index, 0, size(), "insert");
    auto itr = _elements.begin();
    std::advance(itr, index);
    _elements.insert(itr, value);
    _version.update();
}

template <typename ValueType>
bool LinkedList<ValueType>::isEmpty() const {
    return _elements.empty();
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */
template <typename ValueType>
void LinkedList<ValueType>::mapAll(std::function<void (const ValueType &)> fn) const {
    for (ValueType element : *this) {
        fn(element);
    }
}

template <typename ValueType>
void LinkedList<ValueType>::remove(int index) {
    checkIndex(index, 0, size()-1, "remove");
    auto itr = _elements.begin();
    advance(itr, index);
    _elements.erase(itr);
    _version.update();
}

template <typename ValueType>
void LinkedList<ValueType>::set(int index, const ValueType & value) {
    checkIndex(index, 0, size()-1, "set");
    (*this)[index] = value;
}

template <typename ValueType>
int LinkedList<ValueType>::size() const {
    return _elements.size();
}


template <typename ValueType>
void LinkedList<ValueType>::sort() {
    // actually sort a vector to avoid O(N^2) runtime
    // at the cost of O(N) extra memory usage
    Vector<ValueType> vec;
    for (ValueType element : *this) {
        vec.add(element);
    }
    std::sort(vec.begin(), vec.end());

    clear();
    for (const ValueType& element : vec) {
        add(element);
    }
}

template <typename ValueType>
LinkedList<ValueType> LinkedList<ValueType>::subList(int start, int length) const {
    checkIndex(start, 0, size(), "subList");
    checkIndex(start + length, 0, size(), "subList");
    if (length < 0) {
        error("LinkedList::subList: length cannot be negative");
    }
    LinkedList<ValueType> result;
    auto itr = begin();
    for (int i = 0; i < start; i++) {
        ++itr;
    }
    for (int i = 0; i < length; i++) {
        result.add(*itr);
        ++itr;
    }
    return result;
}

template <typename ValueType>
std::string LinkedList<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: LinkedList selection
 * ------------------------------------------
 * The following code implements traditional array selection using
 * square brackets for the index.
 */
template <typename ValueType>
ValueType& LinkedList<ValueType>::operator [](int index) {
    checkIndex(index, 0, size()-1, "operator []");
    auto itr = begin();
    advance(itr, index);
    return *itr;
}
template <typename ValueType>
const ValueType& LinkedList<ValueType>::operator [](int index) const {
    checkIndex(index, 0, size()-1, "operator []");
    auto itr = begin();
    advance(itr, index);
    return *itr;
}

template <typename ValueType>
LinkedList<ValueType>
LinkedList<ValueType>::operator +(const LinkedList& list2) const {
    LinkedList<ValueType> list = *this;
    return list.addAll(list2);
}

template <typename ValueType>
LinkedList<ValueType> LinkedList<ValueType>::operator +(std::initializer_list<ValueType> list) const {
    LinkedList<ValueType> result = *this;
    return result.addAll(list);
}

template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::operator +=(const LinkedList& list2) {
    return addAll(list2);
}

template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::operator +=(const ValueType& value) {
    add(value);
    return *this;
}

/*
 * Implementation notes: relational operators
 * These operators just forward to the underlying STL list.
 */
template <typename ValueType>
bool LinkedList<ValueType>::operator ==(const LinkedList& list2) const {
    ASSERT_HAS_EQUALITY(ValueType);
    return _elements == list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator !=(const LinkedList& list2) const {
    ASSERT_HAS_EQUALITY(ValueType);
    return !(*this == list2);
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <(const LinkedList& list2) const {
    ASSERT_IS_COMPARABLE(ValueType);
    return _elements < list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <=(const LinkedList& list2) const {
    ASSERT_IS_COMPARABLE(ValueType);
    return _elements <= list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >(const LinkedList& list2) const {
    ASSERT_IS_COMPARABLE(ValueType);
    return _elements > list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >=(const LinkedList& list2) const {
    ASSERT_IS_COMPARABLE(ValueType);
    return _elements >= list2._elements;
}

template <typename ValueType>
void LinkedList<ValueType>::checkIndex(int index, int min, int max, const char* prefix) const {
    if (index < min || index > max) {
        std::ostringstream out;
        out << "LinkedList::" << prefix << ": index of " << index
            << " is outside of valid range [";
        if (min < max) {
            out << min << ".." << max;
        } else if (min == max) {
            out << min;
        } // else min > max, no range, empty LinkedList
        out << "]";
        error(out.str());
    }
}

/*
 * Implementation notes: The , operator
 * ------------------------------------
 * The comma operator works adding the right operand to the LinkedList and
 * then returning the LinkedList by reference so that it is set for the next
 * value in the chain.
 */
template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::operator ,(const ValueType& value) {
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
std::ostream& operator <<(std::ostream& os, const LinkedList<ValueType>& list) {
    return stanfordcpplib::collections::writeCollection(os, list);
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, LinkedList<ValueType>& list) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, list, element, /* descriptor */ "LinkedList::operator >>");
}

/*
 * Template hash function for linked lists.
 * Requires the element type in the LinkedList to have a hashCode function.
 */
template <typename T>
int hashCode(const LinkedList<T>& list) {
    return stanfordcpplib::collections::hashCodeCollection(list);
}

/*
 * Function: randomElement
 * Usage: element = randomElement(list);
 * -------------------------------------
 * Returns a randomly chosen element of the given list.
 * Throws an error if the list is empty.
 */
template <typename T>
const T& randomElement(const LinkedList<T>& list) {
    return stanfordcpplib::collections::randomElementIndexed(list);
}

#endif // _linkedlist_h
