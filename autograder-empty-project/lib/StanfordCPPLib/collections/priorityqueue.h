/*
 * File: priorityqueue.h
 * ---------------------
 * This file exports the <code>PriorityQueue</code> class, a
 * collection in which values are processed in priority order.
 * 
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2016/11/07
 * - small const-correctness bug fix in front() / back() (courtesy Truman Cranor)
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - renamed from pqueue.h to priorityqueue.h (old name left as an alias)
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as
 *   {{1.0, "a"}, {2.0, "b"}, {3.0, "c"}}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2015/06/22
 * - added optional compiler flag PQUEUE_PRINT_IN_HEAP_ORDER to indicate
 *   that PQ should be printed in heap-internal order rather than sorted order
 * - added optional compiler flag PQUEUE_ALLOW_HEAP_ACCESS and corresponding
 *   semi-private methods to access value/priority at a given index
 *   (wanted internally for some testing and for practice exam problems;
 *    not meant to be called explicitly by students or most clients)
 * @version 2014/12/04
 * - moved comparison operators <, >=, etc. behind a (default-disabled)
 *   compiler flag because implementation is inefficient
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added add() method as synonym for enqueue()
 * - added remove() method as synonym for dequeue()
 * - added template hashCode function
 * @version 2014/10/20
 * - added equals method, ==, != operators
 * @version 2014/10/10
 * - removed usage of __foreach macro
 * 2014/02/01
 * - added changePriority member to raise a given value's priority
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _priorityqueue_h
#define _priorityqueue_h

#include <cmath>
#include <initializer_list>
#include <utility>
#include <queue>
#include <algorithm>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/*
 * Class: PriorityQueue<ValueType>
 * -------------------------------
 * This class models a structure called a <b><i>priority&nbsp;queue</i></b>
 * in which values are processed in order of priority.  As in conventional
 * English usage, lower priority numbers correspond to higher effective
 * priorities, so that a priority 1 item takes precedence over a
 * priority 2 item.
 */

template <typename ValueType>
class PriorityQueue {
public:
    /*
     * Constructor: PriorityQueue
     * Usage: PriorityQueue<ValueType> pq;
     * -----------------------------------
     * Initializes a new priority queue, which is initially empty.
     */
    PriorityQueue() = default;

    /*
     * Constructor: PriorityQueue
     * Usage: PriorityQueue<ValueType> pq {{1.0, "a"}, {2.0, "b"}, {3.0, "c"}};
     * -------------------------------------------------------------------------
     * Initializes a new priority that stores the given pairs.
     * Note that the pairs are stored in priority order and not
     * necessarily the order in which they are written in the initializer list.
     */
    PriorityQueue(std::initializer_list<std::pair<double, ValueType>> list);

    /*
     * Destructor: ~PriorityQueue
     * --------------------------
     * Frees any heap storage associated with this priority queue.
     */
    virtual ~PriorityQueue() = default;
    
    /*
     * Method: add
     * Usage: pq.add(value, priority);
     * -------------------------------
     * A synonym for the enqueue method.
     */
    void add(const ValueType& value, double priority);
    
    /*
     * Method: back
     * Usage: ValueType last = pq.back();
     * ----------------------------------
     * Returns the last value in the queue by reference.
     */
    ValueType& back();
    
    /*
     * Method: changePriority
     * Usage: pq.changePriority(value, newPriority);
     * ---------------------------------------------
     * Adjusts <code>value</code> in the queue to now have the specified new priority,
     * which must be at least as urgent (lower number) than that value's previous
     * priority in the queue.
     * Throws an error if the element value is not present in the queue, or if the
     * new priority passed is not at least as urgent as its current priority.
     */
    void changePriority(ValueType value, double newPriority);

    /*
     * Method: clear
     * Usage: pq.clear();
     * ------------------
     * Removes all elements from the priority queue.
     */
    void clear();
    
    /*
     * Method: dequeue
     * Usage: ValueType first = pq.dequeue();
     * --------------------------------------
     * Removes and returns the highest priority value.  If multiple
     * entries in the queue have the same priority, those values are
     * dequeued in the same order in which they were enqueued.
     */
    ValueType dequeue();

    /*
     * Method: enqueue
     * Usage: pq.enqueue(value, priority);
     * -----------------------------------
     * Adds <code>value</code> to the queue with the specified priority.
     * Lower priority numbers correspond to higher priorities, which
     * means that all priority 1 elements are dequeued before any
     * priority 2 elements.
     */
    void enqueue(const ValueType& value, double priority);
    
    /*
     * Method: equals
     * Usage: if (pq.equals(pq2)) ...
     * ------------------------------
     * Compares two priority queues for equality.
     * Returns <code>true</code> if this queue contains exactly the same
     * values and priorities as the given other queue.
     * Identical in behavior to the == operator.
     */
    bool equals(const PriorityQueue<ValueType>& pq2) const;
    
    /*
     * Method: front
     * Usage: ValueType first = pq.front();
     * ------------------------------------
     * Returns the first value in the queue by reference.
     */
    ValueType& front();

    /*
     * Method: isEmpty
     * Usage: if (pq.isEmpty()) ...
     * ----------------------------
     * Returns <code>true</code> if the priority queue contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: peek
     * Usage: ValueType first = pq.peek();
     * -----------------------------------
     * Returns the value of highest priority in the queue, without
     * removing it.
     */
    ValueType peek() const;

    /*
     * Method: peekPriority
     * Usage: double priority = pq.peekPriority();
     * -------------------------------------------
     * Returns the priority of the first element in the queue, without
     * removing it.
     */
    double peekPriority() const;

    /*
     * Method: remove
     * Usage: ValueType first = pq.remove();
     * --------------------------------------
     * A synonym for the dequeue method.
     */
    ValueType remove();

    /*
     * Method: size
     * Usage: int n = pq.size();
     * -------------------------
     * Returns the number of values in the priority queue.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = pq.toString();
     * ----------------------------------
     * Converts the queue to a printable string representation.
     */
    std::string toString() const;
    
    /*
     * Operator: <<
     * Prints the priority queue to the given output stream.
     */
    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const PriorityQueue<T>& pq);
    
    /*
     * Operators: ==, !=
     * Usage: if (pq1 == pq2) ...
     * --------------------------
     * Relational operators to compare two queues to see if they have the same elements.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     */
    bool operator ==(const PriorityQueue& pq2) const;
    bool operator !=(const PriorityQueue& pq2) const;

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
    /*
     * Operators: <, <=, >, >=
     * Usage: if (pq1 < pq2) ...
     * -------------------------
     * Relational operators to compare two queues.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const PriorityQueue& pq2) const;
    bool operator <=(const PriorityQueue& pq2) const;
    bool operator >(const PriorityQueue& pq2) const;
    bool operator >=(const PriorityQueue& pq2) const;
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes: PriorityQueue data structure
     * --------------------------------------------------
     * The PriorityQueue class is implemented using a data structure called
     * a heap.
     */
private:
    /* Type used for each heap entry */
    struct HeapEntry {
        ValueType value;
        double priority;
        long sequence;

        bool operator < (const HeapEntry& rhs) const;
    };

    /* Instance variables */
    Vector<HeapEntry> _heap;
    long _enqueueCount = 0;

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
    int pqCompare(const PriorityQueue& other) const;
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

public:
    /* private implentation section */

    template <typename Collection>
    friend int stanfordcpplib::collections::compare(const Collection& pq1, const Collection& pq2);

#ifdef PQUEUE_ALLOW_HEAP_ACCESS
    const ValueType& __getValueFromHeap(int index) const;
    double __getPriorityFromHeap(int index) const;
#endif // PQUEUE_ALLOW_HEAP_ACCESS
};

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue(
        std::initializer_list<std::pair<double, ValueType>> list) {
    for (std::pair<double, ValueType> pair : list) {
        enqueue(pair.second, pair.first);
    }
}

template <typename ValueType>
void PriorityQueue<ValueType>::add(const ValueType& value, double priority) {
    enqueue(value, priority);
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::back() {
    if (isEmpty()) {
        error("PriorityQueue::back: Attempting to read back of an empty queue");
    }
    return _heap.back().value;
}

/*
 * changePriority function added by Marty Stepp.
 * Parts of this implementation are adapted from TrailblazerPQueue.h,
 * which was written by Keith Schwarz.
 */
template <typename ValueType>
void PriorityQueue<ValueType>::changePriority(ValueType value, double newPriority) {
    if (std::isnan(newPriority)) {
        error("PriorityQueue::changePriority: Attempted to use NaN as a priority.");
    }
    if (floatingPointEqual(newPriority, -0.0)) {
        newPriority = 0.0;
    }

    /* Find the element to change. */
    auto itr = std::find_if(_heap.begin(), _heap.end(), [&](const HeapEntry& entry) {
        return entry.value == value;
    });
    if (itr == _heap.end()) {
        error("PriorityQueue::changePriority: Element not found in priority queue.");
    }

    if (itr->priority < newPriority) {
        error("PriorityQueue::changePriority: new priority cannot be less urgent than current priority.");
    }
    itr->priority = newPriority;
    std::push_heap(_heap.begin(), itr + 1);
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
    _heap.clear();
    _enqueueCount = 0;   // BUGFIX 2014/10/10: was previously using garbage unassigned value
}

/*
 * Implementation notes: dequeue, peek, peekPriority
 * -------------------------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    if (isEmpty()) {
        error("PriorityQueue::dequeue: Attempting to dequeue an empty queue");
    }

    ValueType result = _heap[0].value;
    std::pop_heap(_heap.begin(), _heap.end());
    _heap.pop_back();
    return result;
}

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(const ValueType& value, double priority) {
    if (std::isnan(priority)) {
        error("PriorityQueue::enqueue: Attempted to use NaN as a priority.");
    }
    if (floatingPointEqual(priority, -0.0)) {
        priority = 0.0;
    }

    _heap.add({ value, priority, _enqueueCount++ });
    std::push_heap(_heap.begin(), _heap.end());
}

template <typename ValueType>
bool PriorityQueue<ValueType>::equals(const PriorityQueue<ValueType>& pq2) const {
    // optimization: if literally same pq, stop
    if (this == &pq2) {
        return true;
    }
    if (size() != pq2.size()) {
        return false;
    }
    PriorityQueue<ValueType> backup1 = *this;
    PriorityQueue<ValueType> backup2 = pq2;
    while (!backup1.isEmpty() && !backup2.isEmpty()) {
        if (!floatingPointEqual(backup1.peekPriority(), backup2.peekPriority())) {
            return false;
        }
        if (backup1.dequeue() != backup2.dequeue()) {
            return false;
        }
    }
    return backup1.isEmpty() == backup2.isEmpty();
}

template <typename ValueType>
ValueType& PriorityQueue<ValueType>::front() {
    if (isEmpty()) {
        error("PriorityQueue::front: Attempting to read front of an empty queue");
    }
    return _heap[0].value;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
    return _heap.size() == 0;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (isEmpty()) {
        error("PriorityQueue::peek: Attempting to peek at an empty queue");
    }
    return _heap.front().value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
    if (isEmpty()) {
        error("PriorityQueue::peekPriority: Attempting to peek at an empty queue");
    }
    return _heap.get(0).priority;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::remove() {
    return dequeue();
}

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
    return _heap.size();
}

template <typename ValueType>
std::string PriorityQueue<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
/*
 * Implementation note: Due to the complexity and unpredictable heap ordering of the elements,
 * this function sadly makes a deep copy of both PQs for comparing.
 * Therefore it is recommended not to use PQs in a context where <, <=, etc. are being
 * called on them frequently.
 */
template <typename ValueType>
int PriorityQueue<ValueType>::pqCompare(const PriorityQueue& pq2) const {
    if (this == &pq2) {
        return 0;
    }
    PriorityQueue<ValueType> backup1 = *this;
    PriorityQueue<ValueType> backup2 = pq2;
    while (!backup1.isEmpty() && !backup2.isEmpty()) {
        if (backup1.peek() < backup2.peek()) {
            return -1;
        } else if (backup2.peek() < backup1.peek()) {
            return 1;
        }
        
        double pri1 = backup1.peekPriority();
        double pri2 = backup2.peekPriority();
        if (pri1 < pri2) {
            return -1;
        } else if (pri2 < pri1) {
            return 1;
        }
        
        backup1.dequeue();
        backup2.dequeue();
    }
    if (backup1.size() < backup2.size()) {
        return -1;
    } else if (backup2.size() < backup1.size()) {
        return 1;
    } else {
        return 0;
    }
}
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

/*
 * Comparison function for heap entries. The comparison is lexicographic, first by
 * priority, then by sequence number.
 *
 * Because std::push_heap and std::pop_heap try creating a max-heap whereas we want
 * a min-heap, the priority comparisons are reversed.
 */
template <typename ValueType>
bool PriorityQueue<ValueType>::HeapEntry::operator < (const HeapEntry& rhs) const {
    if (priority > rhs.priority) return true;
    if (rhs.priority > priority) return false;

    return sequence < rhs.sequence;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator ==(const PriorityQueue& pq2) const {
    return equals(pq2);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator !=(const PriorityQueue& pq2) const {
    return !equals(pq2);
}

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
template <typename ValueType>
bool PriorityQueue<ValueType>::operator <(const PriorityQueue& pq2) const {
    return pqCompare(pq2) < 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator <=(const PriorityQueue& pq2) const {
    return pqCompare(pq2) <= 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator >(const PriorityQueue& pq2) const {
    return pqCompare(pq2) > 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator >=(const PriorityQueue& pq2) const {
    return pqCompare(pq2) >= 0;
}
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

/*
 * Template hash function for priority queues.
 * Requires the element type in the priority queue to have a hashCode function.
 */
template <typename T>
int hashCode(const PriorityQueue<T>& pq) {
    // (slow, memory-inefficient) implementation: copy pq, dequeue all, and hash together
    PriorityQueue<T> backup = pq;
    int code = hashSeed();
    while (!backup.isEmpty()) {
        code = hashMultiplier() * code + hashCode(backup.peek());
        code = hashMultiplier() * code + hashCode(backup.peekPriority());
        backup.dequeue();
    }
    return int(code & hashMask());
}

#ifdef PQUEUE_ALLOW_HEAP_ACCESS
template <typename ValueType>
const ValueType& PriorityQueue<ValueType>::__getValueFromHeap(int index) const {
    return _heap[index].value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::__getPriorityFromHeap(int index) const {
    return _heap[index].priority;
}
#endif // PQUEUE_ALLOW_HEAP_ACCESS

template <typename ValueType>
std::ostream& operator <<(std::ostream& os,
                          const PriorityQueue<ValueType>& pq) {
    os << "{";

#ifdef PQUEUE_PRINT_IN_HEAP_ORDER
    // faster implementation: print in heap order
    // (only downside: doesn't print in 'sorted' priority order,
    //  which might confuse student client)
    for (int i = 0, len = pq.size(); i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << pq._heap[i].priority << ":";
        writeGenericValue(os, pq._heap[i].value, /* forceQuotes */ true);
    }
#else
    // (default) slow, memory-inefficient implementation: copy pq and print
    PriorityQueue<ValueType> copy = pq;
    for (int i = 0, len = pq.size(); i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << copy.peekPriority() << ":";
        writeGenericValue(os, copy.dequeue(), /* forceQuotes */ true);
    }
#endif
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, PriorityQueue<ValueType>& pq) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("PriorityQueue::operator >>: Missing {");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }
    pq.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            double priority = 0.0;
            is >> priority >> ch;
            if (ch != ':') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error("PriorityQueue::operator >>: Missing colon after priority");
#endif
                is.setstate(std::ios_base::failbit);
                return is;
            }
            ValueType value;
            if (!readGenericValue(is, value)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error("PriorityQueue::operator >>: parse error");
#endif
                return is;
            }
            pq.enqueue(value, priority);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(std::string("PriorityQueue::operator >>: Unexpected character ") + ch);
#endif
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }
    }
    return is;
}

#endif // _priorityqueue_h
