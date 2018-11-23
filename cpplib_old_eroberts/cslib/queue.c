/*
 * File: queue.c
 * Last modified on Sun Dec 16 14:21:51 2012 by eroberts
 * -----------------------------------------------------
 * This interface implements the queue.h interface, which implements
 * first-in/first-out queues using a linked list.
 */

#include <stdio.h>
#include "cslib.h"
#include "generic.h"
#include "queue.h"
#include "unittest.h"

/*
 * Type: Cell
 * ----------
 * This type defines the cells used for the linked list that stores
 * the items in the queue.
 */

typedef struct Cell {
   void *value;
   struct Cell *link;
} Cell;

/*
 * Type: QueueCDT
 * --------------
 * This type defines the concrete representation of a queue.
 * In this representation, the queue is a linked list of cells.
 * The next item to be dequeued is found at the cell addressed
 * by the head field.  The tail field points to the last element
 * in the queue, which allows enqueue to operate in constant time.
 * The structure includes a count field to ensure that the size
 * method runs in constant time.
 */

struct QueueCDT {
   Cell *head;
   Cell *tail;
   int count;
};

/* Exported entries */

Queue newQueue(void) {
   Queue queue;

   queue = newBlock(Queue);
   queue->head = NULL;
   queue->tail = NULL;
   queue->count = 0;
   return queue;
}

void freeQueue(Queue queue) {
   Cell *cp, *next;

   cp = queue->head;
   while (cp != NULL) {
      next = cp->link;
      freeBlock(cp);
      cp = next;
   }
   freeBlock(queue);
}

void enqueueQueue(Queue queue, void *value) {
   Cell *cp;

   cp = newBlock(Cell *);
   cp->value = value;
   cp->link = NULL;
   if (queue->tail == NULL) {
      queue->head = cp;
      queue->tail = cp;
   } else {
      queue->tail->link = cp;
   }
   queue->tail = cp;
   queue->count++;
}

void *dequeueQueue(Queue queue) {
   void *result;
   Cell *cp;

   if (queue->count == 0) error("dequeue: queue is empty");
   cp = queue->head;
   result = cp->value;
   queue->head = cp->link;
   if (queue->head == NULL) queue->tail = NULL;
   queue->count--;
   freeBlock(cp);
   return result;
}

void *peekQueue(Queue queue) {
   if (queue->count == 0) error("peek: queue is empty");
   return queue->head->value;
}

bool isEmptyQueue(Queue queue) {
   return queue->count == 0;
}

int sizeQueue(Queue queue) {
   return queue->count;
}

void clearQueue(Queue queue) {
   while (queue->count) dequeue(queue);
}

Queue cloneQueue(Queue queue) {
   Queue newqueue;
   Cell *cp;

   newqueue = newQueue();
   for (cp = queue->head; cp != NULL; cp = cp->link) {
      enqueueQueue(newqueue, cp->value);
   }
   return newqueue;
}

/**********************************************************************/
/* Unit test for the queue module                                     */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

void testQueueModule(void) {
   Queue queue, queue2;

   trace(queue = newQueue());
   test(isEmpty(queue), true);
   trace(enqueue(queue, "A"));
   test(isEmpty(queue), false);
   trace(enqueue(queue, "B"));
   trace(enqueue(queue, "C"));
   test(peek(queue), "A");
   test(dequeue(queue), "A");
   trace(enqueue(queue, "D"));
   test(size(queue), 3);
   trace(queue2 = clone(queue));
   test(dequeue(queue), "B");
   test(dequeue(queue), "C");
   test(dequeue(queue), "D");
   test(isEmpty(queue), true);
   testError(dequeue(queue));
   trace(enqueue(queue, "E"));
   trace(enqueue(queue, "F"));
   test(dequeue(queue), "E");
   test(dequeue(queue), "F");
   test(dequeue(queue2), "B");
   test(dequeue(queue2), "C");
   test(dequeue(queue2), "D");
}

#endif
