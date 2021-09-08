/*
 * File: linkedlist.h
 * ------------------
 * This file exports the <code>LinkedList</code> class, which provides an
 * implementation of a doubly-linked list of objects and provides a
 * public interface similar to that of the <code>Vector</code> class.
 *
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
 * @version 2018/01/07
 * - added removeFront, removeBack
 * @version 2017/11/15
 * - added contains, indexOf, lastIndexOf, removeValue, reverse, shuffle, sort
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/10
 * - added support for std initializer_list usage, such as {1, 2, 3}
 *   in constructor, addAll, +, +=
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 * @version 2014/10/19
 * - added subList method
 * @version 2014/10/10
 * - removed usage of __foreach macro
 * 2014/07/10
 *  - fixed compiler errors/bugs in initial implementation:
 *    back(), front() members, etc.
 *  - changed checkLinkedListIndex into a member function to avoid warnings
 * @since 2014/07/10
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

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

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "random.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

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
     * Adds a new value to the end of this LinkedList.  To ensure compatibility
     * with the <code>LinkedList</code> class in the Standard Template Library,
     * this method is also called <code>push_back</code>.
     */
    void add(ValueType value);
    
    /*
     * Method: addAll
     * Usage: list.addAll(l2);
     * -----------------------
     * Adds all elements of the given other linked list to this list.
     * Returns a reference to this list.
     * Identical in behavior to the += operator.
     * You may also pass an initializer list such as {1, 2, 3}.
     */
    LinkedList<ValueType>& addAll(const LinkedList<ValueType>& list);
    LinkedList<ValueType>& addAll(std::initializer_list<ValueType> list);

    /*
     * Method: back
     * Usage: ValueType val = list.back();
     * -----------------------------------
     * Returns the element at the back (index size-1) of this LinkedList.
     * This method signals an error if the list is empty.
     */
    ValueType& back();
    const ValueType& back() const;

    /*
     * Method: clear
     * Usage: list.clear();
     * --------------------
     * Removes all elements from this LinkedList.
     */
    void clear();
    
    /*
     * Method: contains
     * Usage: if (list.contains(value)) ...
     * ------------------------------------
     * Returns true if the list contains the given value.
     * The ValueType must have an == operator to use this method.
     */
    bool contains(const ValueType& value) const;

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
     * Method: front
     * Usage: ValueType val = list.front();
     * ------------------------------------
     * Returns the element at the front (index 0) of this LinkedList.
     * This method signals an error if the list is empty.
     */
    ValueType& front();
    const ValueType& front() const;

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
     * Method: indexOf
     * Usage: int index = list.indexOf(value);
     * ---------------------------------------
     * Returns the index of the first occurrence of the given value.
     * If the value is not found in the vector, returns -1.
     * The ValueType must have an == operator to use this method.
     */
    int indexOf(const ValueType& value) const;

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
     * Method: lastIndexOf
     * Usage: int index = list.lastIndexOf(value);
     * -------------------------------------------
     * Returns the index of the last occurrence of the given value.
     * If the value is not found in the vector, returns -1.
     * The ValueType must have an == operator to use this method.
     */
    int lastIndexOf(const ValueType& value) const;

    /*
     * Method: mapAll
     * Usage: list.mapAll(fn);
     * ----------------------
     * Calls the specified function on each element of the LinkedList in
     * ascending index order.
     */
    void mapAll(std::function<void (const ValueType &)> fn) const;
    
    /*
     * Method: pop_back
     * Usage: ValueType back = list.pop_back();
     * ------------------------------------------
     * Removes and returns the last value of this LinkedList.
     * Throws an error if the list is empty.
     */
    ValueType pop_back();

    /*
     * Method: pop_front
     * Usage: ValueType front = list.pop_front();
     * ------------------------------------------
     * Removes and returns the first value of this LinkedList.
     * Throws an error if the list is empty.
     */
    ValueType pop_front();
    
    /*
     * Method: push_back
     * Usage: list.push_back(value);
     * -----------------------------
     * Adds a new value to the end of this LinkedList.
     * Same behavior as the <code>add</code> method.
     */
    void push_back(const ValueType& value);

    /*
     * Method: push_front
     * Usage: list.push_front(value);
     * ------------------------------
     * Adds a new value to the front of this LinkedList.
     */
    void push_front(const ValueType& value);
    
    /*
     * Method: remove
     * Usage: list.remove(index);
     * -------------------------
     * Removes the element at the specified index from this LinkedList.
     * This method signals an error if the index is outside the list range.
     */
    void remove(int index);

    /*
     * Method: removeFirst
     * Usage: ValueType val = list.removeFirst();
     * ------------------------------------------
     * Removes and returns the element at index 0 in this list.
     * This method signals an error if list is empty.
     */
    ValueType removeFront();

    /*
     * Method: removeLast
     * Usage: ValueType val = list.removeLast();
     * -----------------------------------------
     * Removes and returns the element at index (size - 1) in this list.
     * This method signals an error if list is empty.
     */
    ValueType removeBack();

    /*
     * Method: removeValue
     * Usage: list.removeValue(value);
     * -------------------------------
     * Removes the first occurrence of the element value from this list.
     * All subsequent elements are shifted one position to the left.
     * If the vector does not contain the given value, has no effect.
     * The ValueType must have an == operator to use this method.
     */
    void removeValue(const ValueType& value);

    /*
     * Method: reverse
     * Usage: list.reverse();
     * ----------------------
     * Reverses the order of the elements in this list.
     * For example, if the list stores {1, 3, 4, 9}, changes it to store {9, 4, 3, 1}.
     */
    void reverse();

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
     * Method: shuffle
     * Usage: list.shuffle();
     * ----------------------
     * Rearranges the order of the elements in this list into a random order.
     */
    void shuffle();

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
     * You can also pass an initializer list such as {1, 2, 3}.
     */
    LinkedList& operator +=(const LinkedList& l2);
    LinkedList& operator +=(std::initializer_list<ValueType> list);
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
LinkedList<ValueType>& LinkedList<ValueType>::addAll(std::initializer_list<ValueType> list) {
    for (const ValueType& value : list) {
        add(value);
    }
    _version.update();
    return *this;
}

template <typename ValueType>
ValueType& LinkedList<ValueType>::back() {
    return const_cast<ValueType&>(static_cast<const LinkedList &>(*this).back());
}

template <typename ValueType>
const ValueType& LinkedList<ValueType>::back() const {
    if (isEmpty()) {
        error("LinkedList::back: list is empty");
    }
    return _elements.back();
}

template <typename ValueType>
void LinkedList<ValueType>::clear() {
    _elements.clear();
    _version.update();
}

template <typename ValueType>
bool LinkedList<ValueType>::contains(const ValueType& value) const {
    return indexOf(value) >= 0;
}

template <typename ValueType>
bool LinkedList<ValueType>::equals(const LinkedList<ValueType>& list2) const {
    return _elements == list2._elements;
}

template <typename ValueType>
ValueType& LinkedList<ValueType>::front() {
    return const_cast<ValueType&>(static_cast<const LinkedList &>(*this).front());
}

template <typename ValueType>
const ValueType& LinkedList<ValueType>::front() const {
    if (isEmpty()) {
        error("LinkedList::front: list is empty");
    }
    return _elements.front();
}

template <typename ValueType>
const ValueType & LinkedList<ValueType>::get(int index) const {
    checkIndex(index, 0, size()-1, "get");
    return *std::next(_elements.begin(), index);
}

template <typename ValueType>
int LinkedList<ValueType>::indexOf(const ValueType& value) const {
    // loop using iterator to avoid O(N^2) runtime
    int i = 0;
    for (const ValueType& element : *this) {
        if (element == value) {
            return i;
        }
        i++;
    }
    return -1;
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

template <typename ValueType>
int LinkedList<ValueType>::lastIndexOf(const ValueType& value) const {
    // loop using iterator to avoid O(N^2) runtime
    int i = size();
    auto begin = this->begin();
    auto itr = this->end();
    while (itr != begin) {
        itr--;
        i--;
        if (*itr == value) {
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
void LinkedList<ValueType>::mapAll(std::function<void (const ValueType &)> fn) const {
    for (ValueType element : *this) {
        fn(element);
    }
}

template <typename ValueType>
ValueType LinkedList<ValueType>::pop_back() {
    if (isEmpty()) {
        error("LinkedList::pop_back: list is empty");
    }
    ValueType back = _elements.back();
    _elements.pop_back();
    _version.update();
    return back;
}

template <typename ValueType>
ValueType LinkedList<ValueType>::pop_front() {
    if (isEmpty()) {
        error("LinkedList::pop_front: list is empty");
    }
    ValueType front = _elements.front();
    _elements.pop_front();
    _version.update();
    return front;
}

template <typename ValueType>
void LinkedList<ValueType>::push_back(const ValueType& value) {
    _elements.push_back(value);
    _version.update();
}

template <typename ValueType>
void LinkedList<ValueType>::push_front(const ValueType& value) {
    _elements.push_front(value);
    _version.update();
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
ValueType LinkedList<ValueType>::removeBack() {
    return pop_back();
}

template <typename ValueType>
ValueType LinkedList<ValueType>::removeFront() {
    return pop_front();
}

template <typename ValueType>
void LinkedList<ValueType>::removeValue(const ValueType& value) {
    auto itr = std::find(_elements.begin(), _elements.end(), value);
    if (itr != _elements.end()) {
        _elements.erase(itr);
        _version.update();
    }
}

template <typename ValueType>
void LinkedList<ValueType>::reverse() {
    _elements.reverse();
    _version.update();
}

template <typename ValueType>
void LinkedList<ValueType>::set(int index, const ValueType & value) {
    checkIndex(index, 0, size()-1, "set");
    (*this)[index] = value;
}

template <typename ValueType>
void LinkedList<ValueType>::shuffle() {
    // actually shuffle a vector to avoid O(N^2) runtime
    // at the cost of O(N) extra memory usage
    Vector<ValueType> vec;
    for (ValueType element : *this) {
        vec.add(element);
    }
    for (int i = 0, length = vec.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            std::swap(vec[i], vec[j]);
        }
    }

    clear();
    for (const ValueType& element : vec) {
        add(element);
    }
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
LinkedList<ValueType>& LinkedList<ValueType>::operator +=(std::initializer_list<ValueType> list) {
    return addAll(list);
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
    return _elements == list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator !=(const LinkedList& list2) const {
    return _elements != list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <(const LinkedList& list2) const {
    return _elements < list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <=(const LinkedList& list2) const {
    return _elements <= list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >(const LinkedList& list2) const {
    return _elements > list2._elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >=(const LinkedList& list2) const {
    return this->_elements >= list2._elements;
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

/*
 * Randomly rearranges the elements of the given list.
 * Because it is slow to arbitrarily access/modify indexes in a linked list,
 * this function uses an auxiliary Vector to assist in its implementation,
 * although doing so increases the memory consumption of the algorithm.
 */
template <typename T>
void shuffle(LinkedList<T>& list) {
    list.shuffle();
}

#endif // _linkedlist_h
