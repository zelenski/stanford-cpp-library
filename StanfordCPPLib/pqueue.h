/*
 * File: pqueue.h
 * --------------
 * This file exports the <code>PriorityQueue</code> class, a
 * collection in which values are processed in priority order.
 */

#ifndef _pqueue_h
#define _pqueue_h

#include "error.h"
#include "vector.h"

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
    PriorityQueue();

    /*
     * Destructor: ~PriorityQueue
     * --------------------------
     * Frees any heap storage associated with this priority queue.
     */
    virtual ~PriorityQueue();
    
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
    };

    /* Instance variables */
    Vector<HeapEntry> heap;
    long enqueueCount;
    int backIndex;
    int count;
    int capacity;

    /* Private function prototypes */
    void enqueueHeap(ValueType& value, double priority);
    ValueType dequeueHeap();
    bool takesPriority(int i1, int i2);
    void swapHeapEntries(int i1, int i2);
};

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
    clear();
}

/*
 * Implementation notes: ~PriorityQueue destructor
 * -----------------------------------------------
 * All of the dynamic memory is allocated in the Vector class,
 * so no work is required at this level.
 */
template <typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
    /* Empty */
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::back() {
    if (count == 0) {
        error("PriorityQueue::back: Attempting to read back of an empty queue");
    }
    return heap.get(backIndex).value;
}

/*
 * changePriority function added by Marty Stepp.
 * Parts of this implementation are adapted from TrailblazerPQueue.h,
 * which was written by Keith Schwarz.
 */
template <typename ValueType>
void PriorityQueue<ValueType>::changePriority(ValueType value, double newPriority) {
    if (!(newPriority == newPriority)) {
        error("PriorityQueue::changePriority: Attempted to use NaN as a priority.");
    }
    if (newPriority == -0.0) {
        newPriority = 0.0;
    }

    // find the element in the pqueue; must use a simple iteration over elements
    for (int i = 0; i < count; i++) {
        if (heap[i].value == value) {
            if (heap[i].priority < newPriority) {
                error("PriorityQueue::changePriority: new priority cannot be less urgent than current priority.");
            }
            heap[i].priority = newPriority;

            // after changing the priority, must percolate up to proper level
            // to maintain heap ordering
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (takesPriority(parent, i)) {
                    break;
                }
                swapHeapEntries(parent, i);
                i = parent;
            }

            return;
        }
    }

    // if we get here, the element was not ever found
    error("PriorityQueue::changePriority: Element value not found.");
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
    heap.clear();
    count = 0;
}

/*
 * Implementation notes: dequeue, peek, peekPriority
 * -------------------------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    if (count == 0) {
        error("PriorityQueue::dequeue: Attempting to dequeue an empty queue");
    }
    count--;
    bool wasBack = (backIndex == count);
    ValueType value = heap[0].value;
    swapHeapEntries(0, count);
    int index = 0;
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left >= count) {
            break;
        }
        int child = left;
        if (right < count && takesPriority(right, left)) {
            child = right;
        }
        if (takesPriority(index, child)) {
            break;
        }
        swapHeapEntries(index, child);
        index = child;
    }
    if (wasBack) {
        backIndex = index;
    }
    return value;
}

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(const ValueType& value, double priority) {
    if (!(priority == priority)) {
        error("PriorityQueue::enqueue: Attempted to use NaN as a priority.");
    }
    if (priority == -0.0) {
        priority = 0.0;
    }

    if (count == heap.size()) {
        heap.add(HeapEntry());
    }
    int index = count++;
    heap[index].value = value;
    heap[index].priority = priority;
    heap[index].sequence = enqueueCount++;
    if (index == 0 || takesPriority(backIndex, index)) {
        backIndex = index;
    }
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (takesPriority(parent, index)) {
            break;
        }
        swapHeapEntries(parent, index);
        index = parent;
    }
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::front() {
    if (count == 0) {
        error("PriorityQueue::front: Attempting to read front of an empty queue");
    }
    return heap.get(0).value;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (count == 0) {
        error("PriorityQueue::peek: Attempting to peek at an empty queue");
    }
    return heap.get(0).value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
    if (count == 0) {
        error("PriorityQueue::peekPriority: Attempting to peek at an empty queue");
    }
    return heap.get(0).priority;
}

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
std::string PriorityQueue<ValueType>::toString() const {
    ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
void PriorityQueue<ValueType>::swapHeapEntries(int i1, int i2) {
    HeapEntry entry = heap[i1];
    heap[i1] = heap[i2];
    heap[i2] = entry;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::takesPriority(int i1, int i2) {
    if (heap[i1].priority < heap[i2].priority) {
        return true;
    }
    if (heap[i1].priority > heap[i2].priority) {
        return false;
    }
    return (heap[i1].sequence < heap[i2].sequence);
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os,
                          const PriorityQueue<ValueType>& pq) {
    os << "{";
    PriorityQueue<ValueType> copy = pq;
    int len = pq.size();
    for (int i = 0; i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << copy.peekPriority() << ":";
        writeGenericValue(os, copy.dequeue(), true);
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, PriorityQueue<ValueType>& pq) {
    char ch;
    is >> ch;
    if (ch != '{') {
        error("PriorityQueue::operator >>: Missing {");
    }
    pq.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            double priority;
            is >> priority >> ch;
            if (ch != ':') {
                error("PriorityQueue::operator >>: Missing colon after priority");
            }
            ValueType value;
            readGenericValue(is, value);
            pq.enqueue(value, priority);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("PriorityQueue::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

#endif
