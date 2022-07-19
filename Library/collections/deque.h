/*
 * File: deque.h
 * -------------
 * This file exports the <code>Deque</code> class, a collection
 * in which values can be added and removed from the front or back.
 * It combines much of the functionality of a stack and a queue.
 */

#ifndef _deque_h
#define _deque_h

#include <deque>
#include <initializer_list>

#include "collections.h"
#include "error.h"
#include "hashcode.h"

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
     * Method: clear
     * Usage: deque.clear();
     * ---------------------
     * Removes all elements from the deque.
     */
    void clear();

    /*
     * Method: dequeueBack, dequeueFront
     * Usage: ValueType first = deque.dequeueFront();
     * ----------------------------------------------
     * Removes and returns the frontmost/backmost item in the deque.
     */
    ValueType dequeueBack();
    ValueType dequeueFront();

    /*
     * Method: enqueueBack, enqueueFront
     * Usage: deque.enqueueBack(value);
     * --------------------------------
     * Adds <code>value</code> to the front/back of the deque.
     */
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
     * Method: isEmpty
     * Usage: if (deque.isEmpty()) ...
     * -------------------------------
     * Returns <code>true</code> if the deque contains no elements.
     */
    bool isEmpty() const;

    /*
     * Method: peekBack, peekFront
     * Usage: ValueType first = deque.peekFront();
     * -------------------------------------------
     * Returns the frontmost/lastmost value in the deque, without removing it.
     */
    const ValueType& peekBack() const;
    const ValueType& peekFront() const;

    /*
     * Method: size
     * Usage: int n = deque.size();
     * ----------------------------
     * Returns the number of values in the deque.
     */
    int size() const;

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
    ASSERT_HAS_EQUALITY(ValueType);
    return _elements == deque2._elements;
}

template <typename ValueType>
bool Deque<ValueType>::isEmpty() const {
    return _elements.empty();
}

template <typename ValueType>
const ValueType& Deque<ValueType>::peekBack() const {
    if (isEmpty()) {
        error("Deque::peekBack: Attempting to peek at an empty deque");
    }
    return _elements.back();
}

template <typename ValueType>
const ValueType& Deque<ValueType>::peekFront() const {
    if (isEmpty()) {
        error("Deque::peekFront: Attempting to peek at an empty deque");
    }
    return _elements.front();
}

template <typename ValueType>
int Deque<ValueType>::size() const {
    return _elements.size();
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
    void readOne(Deque<ValueType>& deque, const ValueType& value)
        { deque.enqueueBack(value); }

template <typename ValueType>
std::istream& operator >>(std::istream& is, Deque<ValueType>& deque) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, deque, element, /* descriptor */ "Deque::operator >>", readOne<ValueType>);
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
