/*
 * File: deque.h
 * -------------
 * This file exports the <code>Deque</code> class, a collection
 * in which values can be added and removed from the front or back.
 * It combines much of the functionality of a stack and a queue.
 * 
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - added constructor support for std initializer_list usage, such as {1, 2, 3}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/10/29
 * - added comparison operators ==, !=, <, etc.
 * @since 2014/10/29
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _deque_h
#define _deque_h

#include <deque>
#include <initializer_list>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

/*
 * Class: Deque<ValueType>
 * -----------------------
 * This class models a linear structure called a <b><i>deque</i></b>
 * in which values can be added and removed at either end.
 * This discipline allows first-in/first-out (FIFO) and/or
 * last-in/first-out (LIFO) behavior. That is the defining
 * feature of deques.
 */
template <typename ValueType>
class Deque {
public:
    /*
     * Constructor: Deque
     * Usage: Deque<ValueType> deque;
     * ------------------------------
     * Initializes a new empty deque.
     */
    Deque() = default;

    /*
     * Constructor: Deque
     * Usage: Deque<ValueType> queue {1, 2, 3};
     * ----------------------------------------
     * Initializes a new deque that stores the given elements from front-back.
     */
    Deque(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~Deque
     * ------------------
     * Frees any heap storage associated with this deque.
     */
    virtual ~Deque() = default;
    
    /*
     * Method: add
     * Usage: deque.add(value);
     * ------------------------
     * Adds <code>value</code> to the end of the deque.
     * A synonym for the enqueueBack method.
     */
    void add(const ValueType& value);
    void addBack(const ValueType& value);
    void addFront(const ValueType& value);

    /*
     * Method: back
     * Usage: ValueType last = deque.back();
     * -------------------------------------
     * Returns the last value in the deque by reference.
     */
    const ValueType& back() const;

    /*
     * Method: clear
     * Usage: deque.clear();
     * ---------------------
     * Removes all elements from the deque.
     */
    void clear();
    
    /*
     * Method: dequeue
     * Usage: ValueType first = deque.dequeue();
     * -----------------------------------------
     * Removes and returns the first item in the deque.
     * A synonym for the dequeueFront method.
     */
    ValueType dequeue();
    ValueType dequeueBack();
    ValueType dequeueFront();

    /*
     * Method: enqueue
     * Usage: deque.enqueue(value);
     * ----------------------------
     * Adds <code>value</code> to the end of the deque.
     * A synonym for the enqueueBack method.
     */
    void enqueue(const ValueType& value);
    void enqueueBack(const ValueType& value);
    void enqueueFront(const ValueType& value);
    
    /*
     * Method: equals
     * Usage: if (deque.equals(deque2)) ...
     * ------------------------------------
     * Compares two deques for equality.
     * Returns <code>true</code> if this deque contains exactly the same
     * values as the given other deque.
     * Identical in behavior to the == operator.
     */
    bool equals(const Deque<ValueType>& deque2) const;
    
    /*
     * Method: front
     * Usage: ValueType first = deque.front();
     * ---------------------------------------
     * Returns the first value in the deque by reference.
     */
    const ValueType& front() const;

    /*
     * Method: isEmpty
     * Usage: if (deque.isEmpty()) ...
     * -------------------------------
     * Returns <code>true</code> if the deque contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: peek
     * Usage: ValueType first = deque.peek();
     * --------------------------------------
     * Returns the first value in the deque, without removing it.  For
     * compatibility with the STL classes, this method is also exported
     * under the name <code>front</code>, in which case it returns the
     * value by reference.
     * A synonym for the peekFront method.
     */
    const ValueType& peek() const;
    const ValueType& peekBack() const;
    const ValueType& peekFront() const;

    /*
     * Method: remove
     * Usage: ValueType first = deque.remove();
     * ----------------------------------------
     * Removes and returns the first item in the deque.
     * A synonym for the dequeue method.
     */
    ValueType remove();
    ValueType removeBack();
    ValueType removeFront();

    /*
     * Method: size
     * Usage: int n = deque.size();
     * ----------------------------
     * Returns the number of values in the deque.
     */
    int size() const;
    
    /*
     * Returns an STL deque object with the same elements as this Deque.
     */
    std::deque<ValueType> toStlDeque() const;
    
    /*
     * Method: toString
     * Usage: string str = deque.toString();
     * -------------------------------------
     * Converts the deque to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operators: ==, !=, <, >, <=, >=
     * Usage: if (deque1 == deque2) ...
     * Usage: if (deque1 < deque2) ...
     * ...
     * --------------------------------
     * Relational operators to compare two deques.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator ==(const Deque& deque2) const;
    bool operator !=(const Deque& deque2) const;
    bool operator <(const Deque& deque2) const;
    bool operator <=(const Deque& deque2) const;
    bool operator >(const Deque& deque2) const;
    bool operator >=(const Deque& deque2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    template <typename T>
    friend int hashCode(const Deque<T>& s);
    
    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const Deque<T>& deque);

private:
    // Instance variables
    std::deque<ValueType> _elements;
    stanfordcpplib::collections::VersionTracker _version;

public:

    using iterator = stanfordcpplib::collections::CheckedIterator<typename std::deque<ValueType>::iterator>;
    using const_iterator = stanfordcpplib::collections::CheckedIterator<typename std::deque<ValueType>::const_iterator>;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
};

template <typename ValueType>
Deque<ValueType>::Deque(std::initializer_list<ValueType> list) : _elements(list) {

}

template <typename ValueType>
void Deque<ValueType>::add(const ValueType& value) {
    enqueue(value);
}

template <typename ValueType>
void Deque<ValueType>::addBack(const ValueType& value) {
    enqueueBack(value);
}

template <typename ValueType>
void Deque<ValueType>::addFront(const ValueType& value) {
    enqueueFront(value);
}

template <typename ValueType>
const ValueType& Deque<ValueType>::back() const {
    if (isEmpty()) {
        error("Deque::back: Attempting to read back of an empty deque");
    }
    return _elements.back();
}

template <typename ValueType>
void Deque<ValueType>::clear() {
    _elements.clear();
    _version.update();
}

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty deque and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType Deque<ValueType>::dequeue() {
    return dequeueFront();
}

template <typename ValueType>
ValueType Deque<ValueType>::dequeueBack() {
    if (isEmpty()) {
        error("Deque::dequeueBack: Attempting to dequeue from an empty deque");
    }
    auto result = _elements.back();
    _elements.pop_back();
    _version.update();
    return result;
}

template <typename ValueType>
ValueType Deque<ValueType>::dequeueFront() {
    if (isEmpty()) {
        error("Deque::dequeueFront: Attempting to dequeue from an empty deque");
    }
    auto result = _elements.front();
    _elements.pop_front();
    _version.update();
    return result;
}

template <typename ValueType>
void Deque<ValueType>::enqueue(const ValueType& value) {
    enqueueBack(value);
}

template <typename ValueType>
void Deque<ValueType>::enqueueBack(const ValueType& value) {
    _elements.push_back(value);
    _version.update();
}

template <typename ValueType>
void Deque<ValueType>::enqueueFront(const ValueType& value) {
    _elements.push_front(value);
    _version.update();
}

template <typename ValueType>
bool Deque<ValueType>::equals(const Deque<ValueType>& deque2) const {
    return _elements == deque2._elements;
}

template <typename ValueType>
const ValueType& Deque<ValueType>::front() const {
    if (isEmpty()) {
        error("Deque::front: Attempting to read front of an empty deque");
    }
    return _elements.front();
}

template <typename ValueType>
bool Deque<ValueType>::isEmpty() const {
    return _elements.empty();
}

template <typename ValueType>
const ValueType& Deque<ValueType>::peek() const {
    return peekFront();
}

template <typename ValueType>
const ValueType& Deque<ValueType>::peekBack() const {
    if (isEmpty()) {
        error("Deque::peekBack: Attempting to peek at an empty deque");
    }
    return back();
}

template <typename ValueType>
const ValueType& Deque<ValueType>::peekFront() const {
    if (isEmpty()) {
        error("Deque::peekFront: Attempting to peek at an empty deque");
    }
    return front();
}

template <typename ValueType>
ValueType Deque<ValueType>::remove() {
    return dequeue();
}

template <typename ValueType>
ValueType Deque<ValueType>::removeBack() {
    if (isEmpty()) {
        error("Deque::removeBack: Attempting to remove from an empty deque");
    }
    return dequeueBack();
}

template <typename ValueType>
ValueType Deque<ValueType>::removeFront() {
    if (isEmpty()) {
        error("Deque::removeFront: Attempting to remove from an empty deque");
    }
    return dequeueFront();
}

template <typename ValueType>
int Deque<ValueType>::size() const {
    return _elements.size();
}

template <typename ValueType>
std::deque<ValueType> Deque<ValueType>::toStlDeque() const {
    return _elements;
}

template <typename ValueType>
std::string Deque<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
bool Deque<ValueType>::operator ==(const Deque& deque2) const {
    return equals(deque2);
}

template <typename ValueType>
bool Deque<ValueType>::operator !=(const Deque& deque2) const {
    return !equals(deque2);
}

template <typename ValueType>
bool Deque<ValueType>::operator <(const Deque& deque2) const {
    return stanfordcpplib::collections::compare(_elements, deque2._elements) < 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator <=(const Deque& deque2) const {
    return stanfordcpplib::collections::compare(_elements, deque2._elements) <= 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator >(const Deque& deque2) const {
    return stanfordcpplib::collections::compare(_elements, deque2._elements) > 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator >=(const Deque& deque2) const {
    return stanfordcpplib::collections::compare(_elements, deque2._elements) >= 0;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Deque<ValueType>& deque) {
    return stanfordcpplib::collections::writeCollection(os, deque);
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Deque<ValueType>& deque) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, deque, element, /* descriptor */ "Deque::operator >>");
}

template <typename ValueType>
typename Deque<ValueType>::iterator Deque<ValueType>::begin() {
    return { &_version, _elements.begin(), _elements };
}
template <typename ValueType>
typename Deque<ValueType>::const_iterator Deque<ValueType>::begin() const {
    return { &_version, _elements.begin(), _elements };
}

template <typename ValueType>
typename Deque<ValueType>::iterator Deque<ValueType>::end() {
    return { &_version, _elements.end(), _elements };
}
template <typename ValueType>
typename Deque<ValueType>::const_iterator Deque<ValueType>::end() const {
    return { &_version, _elements.end(), _elements };
}

/*
 * Template hash function for deques.
 * Requires the element type in the deque to have a hashCode function.
 */
template <typename T>
int hashCode(const Deque<T>& deq) {
    return stanfordcpplib::collections::hashCodeCollection(deq);
}

#endif // _deque_h
