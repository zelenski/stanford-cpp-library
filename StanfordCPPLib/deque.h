/*
 * File: deque.h
 * -------------
 * This file exports the <code>Deque</code> class, a collection
 * in which values can be added and removed from the front or back.
 * It combines much of the functionality of a stack and a queue.
 * 
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/10/29
 * - added comparison operators ==, !=, <, etc.
 * @since 2014/10/29
 */

#ifndef _deque_h
#define _deque_h

#include <deque>
#include "compare.h"
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
    Deque();

    /*
     * Destructor: ~Deque
     * ------------------
     * Frees any heap storage associated with this deque.
     */
    virtual ~Deque();
    
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

    template <typename Collection>
    friend int compare::compare(const Collection& pq1, const Collection& pq2);
    
    template <typename T>
    friend int hashCode(const Deque<T>& s);
    
    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const Deque<T>& deque);

private:
    /* Instance variables */
    std::deque<ValueType> elements;

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::deque<ValueType>::iterator {
    public:
        iterator() : std::deque<ValueType>::iterator() {}
        iterator(const iterator& it) : std::deque<ValueType>::iterator(it) {}
        iterator(const typename std::deque<ValueType>::iterator& it) : std::deque<ValueType>::iterator(it) {}
    };
    class const_iterator : public std::deque<ValueType>::const_iterator {
    public:
        const_iterator() : std::deque<ValueType>::const_iterator() {}
        const_iterator(const const_iterator& it) : std::deque<ValueType>::const_iterator(it) {}
        const_iterator(const typename std::deque<ValueType>::const_iterator& it) : std::deque<ValueType>::const_iterator(it) {}
    };

    /*
     * Returns an iterator positioned at the first element of the deque.
     */
    iterator begin() {
        return iterator(elements.begin());
    }

    /*
     * Returns an iterator positioned at the first element of the deque.
     */
    const_iterator begin() const {
        auto itr = elements.begin();
        return const_iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the deque.
     */
    iterator end() {
        auto itr = elements.end();
        return iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the deque.
     */
    const_iterator end() const {
        auto itr = elements.end();
        return const_iterator(itr);
    }
};

/*
 * Implementation notes: Deque constructor
 * ---------------------------------------
 * None.
 */
template <typename ValueType>
Deque<ValueType>::Deque() {
    // empty
}

/*
 * Implementation notes: ~Deque destructor
 * ---------------------------------------
 * None.
 */
template <typename ValueType>
Deque<ValueType>::~Deque() {
    // empty
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
    return elements.back();
}

template <typename ValueType>
void Deque<ValueType>::clear() {
    elements.clear();
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
    ValueType result = elements.back();
    elements.pop_back();
    return result;
}

template <typename ValueType>
ValueType Deque<ValueType>::dequeueFront() {
    if (isEmpty()) {
        error("Deque::dequeueFront: Attempting to dequeue from an empty deque");
    }
    ValueType result = elements.front();
    elements.pop_front();
    return result;
}

template <typename ValueType>
void Deque<ValueType>::enqueue(const ValueType& value) {
    enqueueBack(value);
}

template <typename ValueType>
void Deque<ValueType>::enqueueBack(const ValueType& value) {
    elements.push_back(value);
}

template <typename ValueType>
void Deque<ValueType>::enqueueFront(const ValueType& value) {
    elements.push_front(value);
}

template <typename ValueType>
bool Deque<ValueType>::equals(const Deque<ValueType>& deque2) const {
    return elements == deque2.elements;
}

template <typename ValueType>
const ValueType& Deque<ValueType>::front() const {
    if (isEmpty()) {
        error("Deque::front: Attempting to read front of an empty deque");
    }
    return elements.front();
}

template <typename ValueType>
bool Deque<ValueType>::isEmpty() const {
    return elements.empty();
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
    return elements.size();
}

template <typename ValueType>
std::deque<ValueType> Deque<ValueType>::toStlDeque() const {
    return elements;
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
    return compare::compare(*this, deque2) < 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator <=(const Deque& deque2) const {
    return compare::compare(*this, deque2) <= 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator >(const Deque& deque2) const {
    return compare::compare(*this, deque2) > 0;
}

template <typename ValueType>
bool Deque<ValueType>::operator >=(const Deque& deque2) const {
    return compare::compare(*this, deque2) >= 0;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Deque<ValueType>& deque) {
    os << "{";
    int i = 0;
    for (ValueType element : deque) {
        if (i > 0) {
            os << ", ";
        }
        i++;
        writeGenericValue(os, element, /* forceQuotes */ true);
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Deque<ValueType>& deque) {
    char ch;
    is >> ch;
    if (ch != '{') {
        error("Deque::operator >>: Missing {");
    }
    deque.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            ValueType value;
            readGenericValue(is, value);
            deque.enqueue(value);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("Deque::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

/*
 * Template hash function for deques.
 * Requires the element type in the deque to have a hashCode function.
 */
template <typename T>
int hashCode(const Deque<T>& q) {
    int code = hashSeed();
    for (T element : q) {
        code = hashMultiplier() * code + hashCode(element);
    }
    return int(code & hashMask());
}

#endif
