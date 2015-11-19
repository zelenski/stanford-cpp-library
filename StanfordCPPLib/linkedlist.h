/*
 * File: linkedlist.h
 * ------------------
 * This file exports the <code>LinkedList</code> class, which provides an
 * implementation of a doubly-linked list of objects and provides the same
 * public interface of members as the <code>LinkedList</code> class.
 *
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

#ifndef _linkedlist_h
#define _linkedlist_h

#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include "error.h"
#include "hashcode.h"
#include "random.h"
#include "strlib.h"

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
     * --------------------------------------------
     * Initializes a new LinkedList.  The default constructor creates an
     * empty LinkedList.
     */
    LinkedList();
    /* implicit */ LinkedList(const std::list<ValueType>& v);

    /*
     * Destructor: ~LinkedList
     * -------------------
     * Frees any heap storage allocated by this LinkedList.
     */
    virtual ~LinkedList();
    
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
     */
    LinkedList<ValueType>& addAll(const LinkedList<ValueType>& list);
    
    /*
     * Method: back
     * Usage: ValueType val = list.back();
     * ---------------------------------------
     * Returns the element at the back of this LinkedList.
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
     * ---------------------------------------
     * Returns the element at the front of this LinkedList.
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
    void mapAll(void (*fn)(ValueType)) const;
    void mapAll(void (*fn)(const ValueType &)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;
    
    /*
     * Method: pop_front
     * Usage: ValueType front = list.pop_front();
     * ------------------------------------------
     * Removes and returns the first value of this LinkedList.
     * Throws an error if the list is empty.
     */
    ValueType pop_front();
    
    /*
     * Method: pop_back
     * Usage: ValueType back = list.pop_back();
     * ------------------------------------------
     * Removes and returns the last value of this LinkedList.
     * Throws an error if the list is empty.
     */
    ValueType pop_back();
    
    /*
     * Method: push_front
     * Usage: list.push_front(value);
     * ------------------------------
     * Adds a new value to the front of this LinkedList.
     */
    void push_front(const ValueType& value);
    
    /*
     * Method: push_back
     * Usage: list.push_back(value);
     * -----------------------------
     * Adds a new value to the end of this LinkedList.
     * Same behavior as the <code>add</code> method.
     */
    void push_back(const ValueType& value);
    
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
     * Returns an STL list object with the same elements as this LinkedList.
     */
    std::list<ValueType> toStlList() const;

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
     * Concatenates two LinkedLists.
     */
    LinkedList operator +(const LinkedList& l2) const;

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
    std::list<ValueType> m_elements;   // STL linked list as backing storage

    /* Private methods */

    /*
     * Throws an ErrorException if the given index is not within the range of
     * [min..max] inclusive.
     * This is a consolidated error handler for all various LinkedList members that
     * accept index parameters.
     * The prefix parameter represents a text string to place at the start of
     * the error message, generally to help indicate which member threw the error.
     */
    void checkIndex(int index, int min, int max, std::string prefix) const;

    void deepCopy(const LinkedList& src);

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
     * making it possible to pass or return LinkedLists by value and assign
     * from one LinkedList to another.
     */
    LinkedList(const LinkedList& src);
    LinkedList& operator =(const LinkedList& src);

    /*
     * Operator: ,
     * -----------
     * Adds an element to the LinkedList passed as the left-hand operatand.
     * This form makes it easier to initialize LinkedLists in old versions of C++.
     */
    LinkedList& operator ,(const ValueType& value);

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::list<ValueType>::iterator {
    public:
        iterator() : std::list<ValueType>::iterator() {}
        iterator(const iterator& it) : std::list<ValueType>::iterator(it) {}
        iterator(const typename std::list<ValueType>::iterator& it) : std::list<ValueType>::iterator(it) {}
    };
    class const_iterator : public std::list<ValueType>::const_iterator {
    public:
        const_iterator() : std::list<ValueType>::const_iterator() {}
        const_iterator(const const_iterator& it) : std::list<ValueType>::const_iterator(it) {}
        const_iterator(const typename std::list<ValueType>::const_iterator& it) : std::list<ValueType>::const_iterator(it) {}
    };

    /*
     * Returns an iterator positioned at the first element of the list.
     */
    iterator begin() {
        return iterator(m_elements.begin());
    }

    /*
     * Returns an iterator positioned at the first element of the list.
     */
    const_iterator begin() const {
        auto itr = m_elements.begin();
        return const_iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the list.
     */
    iterator end() {
        auto itr = m_elements.end();
        return iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the list.
     */
    const_iterator end() const {
        auto itr = m_elements.end();
        return const_iterator(itr);
    }
};

/* Implementation section */

/*
 * Implementation notes: LinkedList constructor and destructor
 * -------------------------------------------------------
 * The constructor allocates storage for the dynamic array
 * and initializes the other fields of the object.  The
 * destructor frees the memory used for the array.
 */
template <typename ValueType>
LinkedList<ValueType>::LinkedList() {
    // empty
}

template <typename ValueType>
LinkedList<ValueType>::LinkedList(const std::list<ValueType>& v) {
    m_elements = v;
}

template <typename ValueType>
LinkedList<ValueType>::~LinkedList() {
    // empty
}

template <typename ValueType>
void LinkedList<ValueType>::add(ValueType value) {
    m_elements.push_back(value);
}

template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::addAll(const LinkedList<ValueType>& list) {
    for (ValueType value : list) {
        add(value);
    }
}

template <typename ValueType>
ValueType& LinkedList<ValueType>::back() {
    if (isEmpty()) {
        error("LinkedList::back: list is empty");
    }
    return m_elements.back();
}

template <typename ValueType>
const ValueType& LinkedList<ValueType>::back() const {
    if (isEmpty()) {
        error("LinkedList::back: list is empty");
    }
    return m_elements.back();
}

template <typename ValueType>
void LinkedList<ValueType>::clear() {
    m_elements.clear();
}

template <typename ValueType>
bool LinkedList<ValueType>::equals(const LinkedList<ValueType>& list2) const {
    return m_elements == list2.m_elements;
}

template <typename ValueType>
ValueType& LinkedList<ValueType>::front() {
    if (isEmpty()) {
        error("LinkedList::front: list is empty");
    }
    return m_elements.front();
}

template <typename ValueType>
const ValueType& LinkedList<ValueType>::front() const {
    if (isEmpty()) {
        error("LinkedList::front: list is empty");
    }
    return m_elements.front();
}

template <typename ValueType>
const ValueType & LinkedList<ValueType>::get(int index) const {
    checkIndex(index, 0, size()-1, "get");
    auto itr = m_elements.begin();
    for (int i = 0; i < index; i++) {
        ++itr;
    }
    return *itr;
}

template <typename ValueType>
void LinkedList<ValueType>::insert(int index, ValueType value) {
    checkIndex(index, 0, size(), "insert");
    auto itr = m_elements.begin();
    std::advance(itr, index);
    m_elements.insert(itr, value);
}

template <typename ValueType>
bool LinkedList<ValueType>::isEmpty() const {
    return m_elements.empty();
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */
template <typename ValueType>
void LinkedList<ValueType>::mapAll(void (*fn)(ValueType)) const {
    for (ValueType element : *this) {
        fn(element);
    }
}

template <typename ValueType>
void LinkedList<ValueType>::mapAll(void (*fn)(const ValueType &)) const {
    for (ValueType element : *this) {
        fn(element);
    }
}

template <typename ValueType>
template <typename FunctorType>
void LinkedList<ValueType>::mapAll(FunctorType fn) const {
    for (ValueType element : *this) {
        fn(element);
    }
}

template <typename ValueType>
ValueType LinkedList<ValueType>::pop_back() {
    if (isEmpty()) {
        error("LinkedList::pop_back: list is empty");
    }
    ValueType back = m_elements.back();
    m_elements.pop_back();
    return back;
}

template <typename ValueType>
ValueType LinkedList<ValueType>::pop_front() {
    if (isEmpty()) {
        error("LinkedList::pop_front: list is empty");
    }
    ValueType front = m_elements.front();
    m_elements.pop_front();
    return front;
}

template <typename ValueType>
void LinkedList<ValueType>::push_back(const ValueType& value) {
    m_elements.push_back(value);
}

template <typename ValueType>
void LinkedList<ValueType>::push_front(const ValueType& value) {
    m_elements.push_front(value);
}

template <typename ValueType>
void LinkedList<ValueType>::remove(int index) {
    checkIndex(index, 0, size()-1, "remove");
    auto itr = m_elements.begin();
    advance(itr, index);
    m_elements.erase(itr);
}

template <typename ValueType>
void LinkedList<ValueType>::set(int index, const ValueType & value) {
    checkIndex(index, 0, size()-1, "set");
    m_elements[index] = value;
}

template <typename ValueType>
int LinkedList<ValueType>::size() const {
    return m_elements.size();
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
std::list<ValueType> LinkedList<ValueType>::toStlList() const {
    return m_elements;
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
    return m_elements == list2.m_elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator !=(const LinkedList& list2) const {
    return m_elements != list2.m_elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <(const LinkedList& list2) const {
    return m_elements < list2.m_elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator <=(const LinkedList& list2) const {
    return m_elements <= list2.m_elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >(const LinkedList& list2) const {
    return m_elements > list2.m_elements;
}

template <typename ValueType>
bool LinkedList<ValueType>::operator >=(const LinkedList& list2) const {
    return this->m_elements >= list2.m_elements;
}

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow a standard paradigm,
 * as described in the associated textbook.
 */
template <typename ValueType>
LinkedList<ValueType>::LinkedList(const LinkedList& src) {
    deepCopy(src);
}

template <typename ValueType>
LinkedList<ValueType>&
LinkedList<ValueType>::operator =(const LinkedList& src) {
    if (this != &src) {
        deepCopy(src);
    }
    return *this;
}

template <typename ValueType>
void LinkedList<ValueType>::checkIndex(int index, int min, int max, std::string prefix) const {
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

template <typename ValueType>
void LinkedList<ValueType>::deepCopy(const LinkedList& src) {
    m_elements = src.m_elements;
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
    os << "{";
    if (!list.isEmpty()) {
        auto itr = list.begin(), end = list.end();
        writeGenericValue(os, *itr, /* forceQuotes */ true);
        itr++;
        while (itr != end) {
            os << ", ";
            writeGenericValue(os, *itr, /* forceQuotes */ true);
            itr++;
        }
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator>>(std::istream& is, LinkedList<ValueType>& list) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
        error("LinkedList::operator >>: Missing {");
    }
    list.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            ValueType value;
            readGenericValue(is, value);
            list.add(value);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("LinkedList::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

/*
 * Template hash function for linked lists.
 * Requires the element type in the LinkedList to have a hashCode function.
 */
template <typename T>
int hashCode(const LinkedList<T>& list) {
    int code = hashSeed();
    for (T element : list) {
        code = hashMultiplier() * code + hashCode(element);
    }
    return int(code & hashMask());
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
    if (list.isEmpty()) {
        error("randomElement: empty list was passed");
    }
    int index = randomInteger(0, list.size() - 1);
    return list[index];
}

/*
 * Randomly rearranges the elements of the given list.
 * Because it is slow to arbitrarily access/modify indexes in a linked list,
 * this function uses an auxiliary Vector to assist in its implementation,
 * although doing so increases the memory consumption of the algorithm.
 */
template <typename T>
void shuffle(LinkedList<T>& list) {
    // actually shuffle a vector to avoid O(N^2) runtime
    // at the cost of O(N) extra memory usage
    std::vector<T> vec;
    for (T element : list) {
        vec.push_back(element);
    }
    for (int i = 0, length = vec.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
        }
    }
    list.clear();
    for (T element : vec) {
        list.add(element);
    }
}

#endif
