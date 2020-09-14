/*
 * File: queue.h
 * -------------
 * This file exports the <code>Queue</code> class, a collection
 * in which values are ordinarily processed in a first-in/first-out
 * (FIFO) order.
 *
 * @version 2020/09/12
 * - simplify interface in preparation for install
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
 * @version 2018/01/23
 * - fixed bad reference bug on queue.enqueue(queue.peek())
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * - added iterators begin(), end()
 * @version 2016/09/22
 * - optimized equals, ==, != to avoid deep-copy
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {1, 2, 3}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators ==, !=, <, etc.
 * - added add() method as synonym for enqueue()
 * - added remove() method as synonym for dequeue()
 * - added template hashCode function
 * - optimized some functions (operator <<, toStlQueue) to avoid making unneeded deep copy
 * @version 2014/10/10
 * - removed dependency on 'using namespace' statement
 * - removed usage of __foreach macro
 */


#ifndef _queue_h
#define _queue_h

#include <initializer_list>

#include "collections.h"
#include "deque.h"
#include "error.h"
#include "hashcode.h"
#include "vector.h"

/*
 * Class: Queue<ValueType>
 * -----------------------
 * This class models a linear structure called a <b><i>queue</i></b>
 * in which values are added at one end and removed from the other.
 * This discipline gives rise to a first-in/first-out behavior (FIFO)
 * that is the defining feature of queues.
 */
template <typename ValueType>
class Queue {
public:
    /*
     * Constructor: Queue
     * Usage: Queue<ValueType> queue;
     * ------------------------------
     * Initializes a new empty queue.
     */
    Queue() = default;

    /*
     * Constructor: Queue
     * Usage: Queue<ValueType> queue {1, 2, 3};
     * ----------------------------------------
     * Initializes a new queue that stores the given elements from front-back.
     */
    Queue(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~Queue
     * ------------------
     * Frees any heap storage associated with this queue.
     */
    virtual ~Queue() = default;

    /*
     * Method: clear
     * Usage: queue.clear();
     * ---------------------
     * Removes all elements from the queue.
     */
    void clear();

    /*
     * Method: dequeue
     * Usage: ValueType first = queue.dequeue();
     * -----------------------------------------
     * Removes and returns the first item in the queue.
     */
    ValueType dequeue();

    /*
     * Method: enqueue
     * Usage: queue.enqueue(value);
     * ----------------------------
     * Adds <code>value</code> to the end of the queue.
     */
    void enqueue(const ValueType& value);

    /*
     * Method: equals
     * Usage: if (queue.equals(queue2)) ...
     * ------------------------------------
     * Compares two queues for equality.
     * Returns <code>true</code> if this queue contains exactly the same
     * values as the given other queue.
     * Identical in behavior to the == operator.
     */
    bool equals(const Queue<ValueType>& queue2) const;

    /*
     * Method: isEmpty
     * Usage: if (queue.isEmpty()) ...
     * -------------------------------
     * Returns <code>true</code> if the queue contains no elements.
     */
    bool isEmpty() const;

    /*
     * Method: peek
     * Usage: ValueType first = queue.peek();
     * --------------------------------------
     * Returns the first value in the queue, without removing it.
     */
    const ValueType& peek() const;

    /*
     * Method: size
     * Usage: int n = queue.size();
     * ----------------------------
     * Returns the number of values in the queue.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = queue.toString();
     * -------------------------------------
     * Converts the queue to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: ==
     * Usage: queue1 == queue2
     * -------------------
     * Returns <code>true</code> if <code>queue1</code> and <code>queue2</code>
     * contain the same elements.
     */
    bool operator ==(const Queue& queue2) const;

    /*
     * Operator: !=
     * Usage: queue1 != queue2
     * -------------------
     * Returns <code>true</code> if <code>queue1</code> and <code>queue2</code>
     * do not contain the same elements.
     */
    bool operator !=(const Queue& queue2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: queue1 < queue2 ...
     * --------------------------
     * Relational operators to compare two queues.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Queue& queue2) const;
    bool operator <=(const Queue& queue2) const;
    bool operator >(const Queue& queue2) const;
    bool operator >=(const Queue& queue2) const;

    template <typename T>
    friend int hashCode(const Queue<T>& s);

    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const Queue<T>& queue);

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    /* Instance variables */
    Deque<ValueType> _elements;
};

template <typename ValueType>
Queue<ValueType>::Queue(std::initializer_list<ValueType> list) : _elements(list) {
    // empty
}

template <typename ValueType>
void Queue<ValueType>::clear() {
    _elements.clear();
}

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType Queue<ValueType>::dequeue() {
    if (isEmpty()) {
        error("Queue::dequeue: Attempting to dequeue an empty queue");
    }
    return _elements.dequeueFront();
}

template <typename ValueType>
void Queue<ValueType>::enqueue(const ValueType& value) {
    _elements.enqueueBack(value);
}

template <typename ValueType>
bool Queue<ValueType>::equals(const Queue<ValueType>& queue2) const {
    return *this == queue2;
}

template <typename ValueType>
bool Queue<ValueType>::isEmpty() const {
    return _elements.isEmpty();
}

template <typename ValueType>
const ValueType& Queue<ValueType>::peek() const {
    if (isEmpty()) {
        error("Queue::peek: Attempting to peek at an empty queue");
    }
    return _elements.peekFront();
}

template <typename ValueType>
int Queue<ValueType>::size() const {
    return _elements.size();
}

template <typename ValueType>
std::string Queue<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
bool Queue<ValueType>::operator ==(const Queue& queue2) const {
    return _elements == queue2._elements;
}

template <typename ValueType>
bool Queue<ValueType>::operator !=(const Queue& queue2) const {
    return _elements != queue2._elements;
}

template <typename ValueType>
bool Queue<ValueType>::operator <(const Queue& queue2) const {
    return _elements < queue2._elements;
}

template <typename ValueType>
bool Queue<ValueType>::operator <=(const Queue& queue2) const {
    return _elements <= queue2._elements;
}

template <typename ValueType>
bool Queue<ValueType>::operator >(const Queue& queue2) const {
    return _elements > queue2._elements;
}

template <typename ValueType>
bool Queue<ValueType>::operator >=(const Queue& queue2) const {
    return _elements >= queue2._elements;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Queue<ValueType>& queue) {
    return os << queue._elements;
}

template <typename ValueType>
    void readOne(Queue<ValueType>& queue, const ValueType& value)
        { queue.enqueue(value); }

template <typename ValueType>
std::istream& operator >>(std::istream& is, Queue<ValueType>& queue) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, queue, element, /* descriptor */ "Queue::operator >>", readOne<ValueType>);
}


/*
 * Template hash function for queues.
 * Requires the element type in the queue to have a hashCode function.
 */
template <typename T>
int hashCode(const Queue<T>& q) {
    return hashCode(q._elements);
}

#endif // _queue_h
