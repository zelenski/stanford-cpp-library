/*
 * File: pqueue.c
 * Last modified on Wed Dec 12 08:08:03 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the pqueue.h interface using a heap structure.
 */

#include <stdio.h>
#include <limits.h>
#include "cslib.h"
#include "generic.h"
#include "pqueue.h"
#include "unittest.h"
#include "vector.h"

/*
 * Constant: INITIAL_CAPACITY
 * --------------------------
 * This constant defines the initial capacity of the heap that implements
 * the priority queue. Any positive value will work correctly, although
 * changing this parameter can affect performance.  Making this value
 * larger postpones the first reallocation but causes priority queues
 * to consume more memory.
 */

#define INITIAL_CAPACITY 10

/*
 * Type: HeapEntry
 * ---------------
 * This type defines the individual elements in the heap.
 */

typedef struct {
   void *value;
   double priority;
   long sequence;
} HeapEntry;

/*
 * Type: PriorityQueueCDT
 * ----------------------
 * This type defines the concrete representation of a priority queue.
 * In this representation, the queue is a dynamic array organized as
 * a "heap."
 */

struct PriorityQueueCDT {
   HeapEntry *heap;
   int capacity;
   int count;
   int back;
   long sequence;
};

/* Function prototypes */

static void expandCapacity(PriorityQueue pq);
static bool takesPriority(PriorityQueue pq, int i1, int i2);
static void swapHeapEntries(PriorityQueue pq, int i1, int i2);

/* Exported entries */

PriorityQueue newPriorityQueue(void) {
   PriorityQueue pq;

   pq = newBlock(PriorityQueue);
   pq->count = 0;
   pq->capacity = INITIAL_CAPACITY;
   pq->heap = newArray(INITIAL_CAPACITY, HeapEntry);
   pq->sequence = 0;
   return pq;
}

void freePriorityQueue(PriorityQueue pq) {
   freeBlock(pq->heap);
   freeBlock(pq);
}

void enqueuePriorityQueue(PriorityQueue pq, void *value, double priority) {
   HeapEntry *hp;
   int index, parent;

   if (pq->count == pq->capacity) expandCapacity(pq);
   index = pq->count++;
   hp = &pq->heap[index];
   hp->value = value;
   hp->priority = priority;
   hp->sequence = pq->sequence++;
   if (index == 0 || takesPriority(pq, pq->back, index)) pq->back = index;
   while (index > 0) {
      parent = (index - 1) / 2;
      if (takesPriority(pq, parent, index)) break;
      swapHeapEntries(pq, parent, index);
      index = parent;
   }
}

void *dequeuePriorityQueue(PriorityQueue pq) {
   int index, left, right, child;
   bool wasBack;
   void *value;

   if (pq->count == 0) error("dequeue: queue is empty");
   pq->count--;
   wasBack = (pq->back == pq->count);
   value = pq->heap[0].value;
   swapHeapEntries(pq, 0, pq->count);
   index = 0;
   while (true) {
      left = 2 * index + 1;
      right = 2 * index + 2;
      if (left >= pq->count) break;
      child = left;
      if (right < pq->count && takesPriority(pq, right, left)) child = right;
      if (takesPriority(pq, index, child)) break;
      swapHeapEntries(pq, index, child);
      index = child;
   }
   if (wasBack) pq->back = index;
   return value;
}

void *peekPriorityQueue(PriorityQueue pq) {
   if (pq->count == 0) error("peek: queue is empty");
   return pq->heap[0].value;
}

double peekPriority(PriorityQueue pq) {
   if (pq->count == 0) error("peek: queue is empty");
   return pq->heap[0].priority;
}

bool isEmptyPriorityQueue(PriorityQueue pq) {
   return pq->count == 0;
}

int sizePriorityQueue(PriorityQueue pq) {
   return pq->count;
}

void clearPriorityQueue(PriorityQueue pq) {
   clear(pq->heap);
}

PriorityQueue clonePriorityQueue(PriorityQueue pq) {
   PriorityQueue newpq;
   int i;

   newpq = newBlock(PriorityQueue);
   newpq->count = pq->count;
   newpq->capacity = pq->capacity;
   newpq->sequence = pq->sequence;
   newpq->heap = newArray(pq->capacity, HeapEntry);
   for (i = 0; i < pq->count; i++) {
      newpq->heap[i] = pq->heap[i];
   }
   return newpq;
}

/* Private functions */

static void expandCapacity(PriorityQueue pq) {
   HeapEntry *heap;
   int i, newCapacity;

   newCapacity = pq->capacity * 2;
   heap = newArray(newCapacity, HeapEntry);
   for (i = 0; i < pq->count; i++) {
      heap[i] = pq->heap[i];
   }
   freeBlock(pq->heap);
   pq->heap = heap;
   pq->capacity = newCapacity;
}

static bool takesPriority(PriorityQueue pq, int i1, int i2) {
   double p1, p2;

   p1 = pq->heap[i1].priority;
   p2 = pq->heap[i2].priority;
   if (p1 < p2) return true;
   if (p1 > p2) return false;
   return pq->heap[i1].sequence < pq->heap[i2].sequence;
}

static void swapHeapEntries(PriorityQueue pq, int i1, int i2) {
   HeapEntry tmp;

   tmp = pq->heap[i1];
   pq->heap[i1] = pq->heap[i2];
   pq->heap[i2] = tmp;
}

/**********************************************************************/
/* Unit test for the pqueue module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

void testPriorityQueueModule(void) {
   PriorityQueue pq, pq2;

   trace(pq = newPriorityQueue());
   test(size(pq), 0);
   test(isEmpty(pq), true);
   trace(enqueue(pq, "A", 1));
   test(isEmpty(pq), false);
   test(peek(pq), "A");
   trace(enqueue(pq, "D", 3));
   trace(enqueue(pq, "C", 2));
   trace(enqueue(pq, "B", 1));
   test(peek(pq), "A");
   test(size(pq), 4);
   trace(pq2 = clone(pq));
   test(dequeue(pq), "A");
   test(peek(pq), "B");
   test(dequeue(pq), "B");
   test(dequeue(pq), "C");
   test(dequeue(pq), "D");
   test(isEmpty(pq), true);
   test(dequeue(pq2), "A");
   test(dequeue(pq2), "B");
   test(dequeue(pq2), "C");
   test(dequeue(pq2), "D");
}

#endif
