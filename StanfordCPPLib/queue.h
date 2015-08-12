/*
 * File: queue.h
 * -------------
 * This file exports the <code>Queue</code> class, a collection
 * in which values are ordinarily processed in a first-in/first-out
 * (FIFO) order.
 * 
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

#include <deque>
#include <queue>
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
    Queue();

    /*
     * Destructor: ~Queue
     * ------------------
     * Frees any heap storage associated with this queue.
     */
    virtual ~Queue();
    
    /*
     * Method: add
     * Usage: queue.add(value);
     * ------------------------
     * Adds <code>value</code> to the end of the queue.
     * A synonym for the enqueue method.
     */
    void add(const ValueType& value);

    /*
     * Method: back
     * Usage: ValueType last = queue.back();
     * -------------------------------------
     * Returns the last value in the queue by reference.
     */
    const ValueType& back() const;

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
     * Method: front
     * Usage: ValueType first = queue.front();
     * ---------------------------------------
     * Returns the first value in the queue by reference.
     */
    const ValueType& front() const;

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
     * Returns the first value in the queue, without removing it.  For
     * compatibility with the STL classes, this method is also exported
     * under the name <code>front</code>, in which case it returns the
     * value by reference.
     */
    const ValueType& peek() const;

    /*
     * Method: remove
     * Usage: ValueType first = queue.remove();
     * ----------------------------------------
     * Removes and returns the first item in the queue.
     * A synonym for the dequeue method.
     */
    ValueType remove();

    /*
     * Method: size
     * Usage: int n = queue.size();
     * ----------------------------
     * Returns the number of values in the queue.
     */
    int size() const;
    
    /*
     * Returns an STL deque object with the same elements as this Queue.
     */
    std::queue<ValueType> toStlDeque() const;
    
    /*
     * Returns an STL queue object with the same elements as this Queue.
     */
    std::queue<ValueType> toStlQueue() const;

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

    /*
     * Implementation notes: Queue data structure
     * ------------------------------------------
     * The Queue class is implemented using a ring buffer.
     */

private:
    /* Instance variables */
    Vector<ValueType> ringBuffer;
    int count;
    int capacity;
    int head;
    int tail;

    /* Private functions */
    void expandRingBufferCapacity();
    int queueCompare(const Queue& queue2) const;
};

/*
 * Implementation notes: Queue data structure
 * ------------------------------------------
 * The array-based queue stores the elements in successive index
 * positions in a vector, just as a stack does.  What makes the
 * queue structure more complex is the need to avoid shifting
 * elements as the queue expands and contracts.  In the array
 * model, this goal is achieved by keeping track of both the
 * head and tail indices.  The tail index increases by one each
 * time an element is enqueued, and the head index increases by
 * one each time an element is dequeued.  Each index therefore
 * marches toward the end of the allocated vector and will
 * eventually reach the end.  Rather than allocate new memory,
 * this implementation lets each index wrap around back to the
 * beginning as if the ends of the array of elements were joined
 * to form a circle.  This representation is called a ring buffer.
 */
static const int INITIAL_CAPACITY = 10;

/*
 * Implementation notes: Queue constructor
 * ---------------------------------------
 * The constructor must allocate the array storage for the queue
 * elements and initialize the fields of the object.
 */
template <typename ValueType>
Queue<ValueType>::Queue() {
    clear();
}

/*
 * Implementation notes: ~Queue destructor
 * ---------------------------------------
 * All of the dynamic memory is allocated in the Vector class,
 * so no work is required at this level.
 */
template <typename ValueType>
Queue<ValueType>::~Queue() {
    // empty
}

template <typename ValueType>
void Queue<ValueType>::add(const ValueType& value) {
    enqueue(value);
}

template <typename ValueType>
const ValueType& Queue<ValueType>::back() const {
    if (count == 0) {
        error("Queue::back: Attempting to read back of an empty queue");
    }
    return ringBuffer[(tail + capacity - 1) % capacity];
}

template <typename ValueType>
void Queue<ValueType>::clear() {
    capacity = INITIAL_CAPACITY;
    ringBuffer = Vector<ValueType>(capacity);
    head = 0;
    tail = 0;
    count = 0;
}

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType Queue<ValueType>::dequeue() {
    if (count == 0) {
        error("Queue::dequeue: Attempting to dequeue an empty queue");
    }
    ValueType result = ringBuffer[head];
    head = (head + 1) % capacity;
    count--;
    return result;
}

template <typename ValueType>
void Queue<ValueType>::enqueue(const ValueType& value) {
    if (count >= capacity - 1) {
        expandRingBufferCapacity();
    }
    ringBuffer[tail] = value;
    tail = (tail + 1) % capacity;
    count++;
}

template <typename ValueType>
bool Queue<ValueType>::equals(const Queue<ValueType>& queue2) const {
    if (this == &queue2) {
        return true;
    }
    if (size() != queue2.size()) {
        return false;
    }
    Queue<ValueType> copy1 = *this;
    Queue<ValueType> copy2 = queue2;
    while (!copy1.isEmpty() && !copy2.isEmpty()) {
        if (!(copy1.dequeue() == copy2.dequeue())) {
            return false;
        }
    }
    return copy1.isEmpty() == copy2.isEmpty();
}

template <typename ValueType>
const ValueType& Queue<ValueType>::front() const {
    if (count == 0) {
        error("Queue::front: Attempting to read front of an empty queue");
    }
    return ringBuffer[head];
}

template <typename ValueType>
bool Queue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
const ValueType& Queue<ValueType>::peek() const {
    if (count == 0) {
        error("Queue::peek: Attempting to peek at an empty queue");
    }
    return ringBuffer.get(head);
}

template <typename ValueType>
ValueType Queue<ValueType>::remove() {
    // this isEmpty check is also done in dequeue(), but we repeat it
    // here so that the possible error message will be more descriptive.
    if (isEmpty()) {
        error("Queue::remove: Attempting to remove from an empty queue");
    }
    return dequeue();
}

template <typename ValueType>
int Queue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
std::queue<ValueType> Queue<ValueType>::toStlDeque() const {
    std::deque<ValueType> result;
    for (int i = 0; i < count; i++) {
        result.push_back(ringBuffer[(head + i) % capacity]);
    }
    return result;
}

template <typename ValueType>
std::queue<ValueType> Queue<ValueType>::toStlQueue() const {
    std::queue<ValueType> result;
    for (int i = 0; i < count; i++) {
        result.push(ringBuffer[(head + i) % capacity]);
    }
    return result;
}

template <typename ValueType>
std::string Queue<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: expandRingBufferCapacity
 * ----------------------------------------------
 * This private method doubles the capacity of the ringBuffer vector.
 * Note that this implementation also shifts all the elements back to
 * the beginning of the vector.
 */
template <typename ValueType>
void Queue<ValueType>::expandRingBufferCapacity() {
    Vector<ValueType> copy = ringBuffer;
    ringBuffer = Vector<ValueType>(2 * capacity);
    for (int i = 0; i < count; i++) {
        ringBuffer[i] = copy[(head + i) % capacity];
    }
    head = 0;
    tail = count;
    capacity *= 2;
}

template <typename ValueType>
int Queue<ValueType>::queueCompare(const Queue& queue2) const {
    if (*this == queue2) {
        return 0;
    }
    
    for (int i1 = 0, i2 = 0;
         i1 < count && i2 < queue2.count;
         i1++, i2++) {
        if (ringBuffer[(head + i1) % capacity] < queue2.ringBuffer[(queue2.head + i2) % queue2.capacity]) {
            return -1;
        } else if (queue2.ringBuffer[(queue2.head + i2) % queue2.capacity] < ringBuffer[(head + i1) % capacity]) {
            return 1;
        }
    }
    
    if (count < queue2.count) {
        return -1;
    } else if (count > queue2.count) {
        return 1;
    } else {
        return 0;
    }
}

template <typename ValueType>
bool Queue<ValueType>::operator ==(const Queue& queue2) const {
	return equals(queue2);
}

template <typename ValueType>
bool Queue<ValueType>::operator !=(const Queue& queue2) const {
    return !equals(queue2);
}

template <typename ValueType>
bool Queue<ValueType>::operator <(const Queue& queue2) const {
    return queueCompare(queue2) < 0;
}

template <typename ValueType>
bool Queue<ValueType>::operator <=(const Queue& queue2) const {
    return queueCompare(queue2) <= 0;
}

template <typename ValueType>
bool Queue<ValueType>::operator >(const Queue& queue2) const {
    return queueCompare(queue2) > 0;
}

template <typename ValueType>
bool Queue<ValueType>::operator >=(const Queue& queue2) const {
    return queueCompare(queue2) >= 0;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Queue<ValueType>& queue) {
    os << "{";
    if (!queue.isEmpty()) {
        writeGenericValue(os, queue.ringBuffer[queue.head], false);
        for (int i = 1; i < queue.count; i++) {
            os << ", ";
            writeGenericValue(os, queue.ringBuffer[(queue.head + i) % queue.capacity], false);
        }
    }
    os << "}";
    return os;
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Queue<ValueType>& queue) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
        error("Queue::operator >>: Missing {");
    }
    queue.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            ValueType value;
            readGenericValue(is, value);
            queue.enqueue(value);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("Queue::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

/*
 * Template hash function for queues.
 * Requires the element type in the queue to have a hashCode function.
 */
template <typename T>
int hashCode(const Queue<T>& q) {
    int code = hashSeed();
    for (int i = 0; i < q.count; i++) {
        code = hashMultiplier() * code + hashCode(q.ringBuffer[(q.head + i) % q.capacity]);
    }
    return int(code & hashMask());
}

#endif
