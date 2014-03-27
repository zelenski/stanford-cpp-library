/*
 * File: queue.h
 * -------------
 * This file exports the <code>Queue</code> class, a collection
 * in which values are ordinarily processed in a first-in/first-out
 * (FIFO) order.
 */

#ifndef _queue_h
#define _queue_h

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
 * Method: size
 * Usage: int n = queue.size();
 * ----------------------------
 * Returns the number of values in the queue.
 */

   int size() const;

/*
 * Method: isEmpty
 * Usage: if (queue.isEmpty()) ...
 * -------------------------------
 * Returns <code>true</code> if the queue contains no elements.
 */

   bool isEmpty() const;

/*
 * Method: clear
 * Usage: queue.clear();
 * ---------------------
 * Removes all elements from the queue.
 */

   void clear();

/*
 * Method: enqueue
 * Usage: queue.enqueue(value);
 * ----------------------------
 * Adds <code>value</code> to the end of the queue.
 */

   void enqueue(ValueType value);

/*
 * Method: dequeue
 * Usage: ValueType first = queue.dequeue();
 * -----------------------------------------
 * Removes and returns the first item in the queue.
 */

   ValueType dequeue();

/*
 * Method: peek
 * Usage: ValueType first = queue.peek();
 * --------------------------------------
 * Returns the first value in the queue, without removing it.  For
 * compatibility with the STL classes, this method is also exported
 * under the name <code>front</code>, in which case it returns the
 * value by reference.
 */

   ValueType peek() const;

/*
 * Method: front
 * Usage: ValueType first = queue.front();
 * ---------------------------------------
 * Returns the first value in the queue by reference.
 */

   ValueType & front();

/*
 * Method: back
 * Usage: ValueType last = queue.back();
 * -------------------------------------
 * Returns the last value in the queue by reference.
 */

   ValueType & back();

/*
 * Method: toString
 * Usage: string str = queue.toString();
 * -------------------------------------
 * Converts the queue to a printable string representation.
 */

   std::string toString();

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

};

extern void error(std::string msg);

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

const int INITIAL_CAPACITY = 10;

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
   /* Empty */
}

template <typename ValueType>
int Queue<ValueType>::size() const {
   return count;
}

template <typename ValueType>
bool Queue<ValueType>::isEmpty() const {
   return count == 0;
}

template <typename ValueType>
void Queue<ValueType>::clear() {
   capacity = INITIAL_CAPACITY;
   ringBuffer = Vector<ValueType>(capacity);
   head = 0;
   tail = 0;
   count = 0;
}

template <typename ValueType>
void Queue<ValueType>::enqueue(ValueType value) {
   if (count >= capacity - 1) expandRingBufferCapacity();
   ringBuffer[tail] = value;
   tail = (tail + 1) % capacity;
   count++;
}

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */

template <typename ValueType>
ValueType Queue<ValueType>::dequeue() {
   if (count == 0) error("dequeue: Attempting to dequeue an empty queue");
   ValueType result = ringBuffer[head];
   head = (head + 1) % capacity;
   count--;
   return result;
}

template <typename ValueType>
ValueType Queue<ValueType>::peek() const {
   if (count == 0) error("peek: Attempting to peek at an empty queue");
   return ringBuffer.get(head);
}

template <typename ValueType>
ValueType & Queue<ValueType>::front() {
   if (count == 0) error("front: Attempting to read front of an empty queue");
   return ringBuffer[head];
}

template <typename ValueType>
ValueType & Queue<ValueType>::back() {
   if (count == 0) error("back: Attempting to read back of an empty queue");
   return ringBuffer[(tail + capacity - 1) % capacity];
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
std::string Queue<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const Queue<ValueType> & queue) {
   os << "{";
   Queue<ValueType> copy = queue;
   int len = queue.size();
   for (int i = 0; i < len; i++) {
      if (i > 0) os << ", ";
      writeGenericValue(os, copy.dequeue(), true);
   }
   return os << "}";
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, Queue<ValueType> & queue) {
   char ch;
   is >> ch;
   if (ch != '{') error("operator >>: Missing {");
   queue.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         queue.enqueue(value);
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

#endif
