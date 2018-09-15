/*
 * File: iterator.c
 * Last modified on Wed Jul 20 23:22:08 2011 by eroberts
 * -----------------------------------------------------
 * This file implements a polymorphic version of the iterator
 * and the tools necessary to construct types that support
 * iteration.  This module implements both the iterator.h and
 * itertype.h interfaces.
 */

#include <stdio.h>
#include <string.h>
#include "cmpfn.h"
#include "cslib.h"
#include "iterator.h"
#include "itertype.h"

/*
 * Constant: ITERATOR_PASSWORD
 * ---------------------------
 * This constant is stored in the header block of collection
 * types to catch the case when a client tries to call
 * newIterator on a value for which iteration is not defined.
 */

#define ITERATOR_PASSWORD 2718281828UL

/*
 * Type: Cell
 * ----------
 * This cell type can vary in size according as to what follows the
 * cell itself.  Iterators are implemented as linked lists chained
 * through the first word in the structure.  Because the size
 * of the actual data element can vary, the data field is not
 * represented as part of the structure.  Instead, each cell is
 * allocated dynamically with enough memory for both the link
 * field and the data value, however large it turns out to be.
 * The list is arranged like a queue, with a tail pointer to
 * its final element for efficiency.  The addToIteratorList
 * function first checks to see whether the element belongs at
 * the end.  By checking for this case explicitly, it is often
 * possible to avoid searching the list at all.
 */

typedef struct Cell {
   struct Cell *link;
   /* The actual data is allocated here */
} Cell;

/*
 * Type: IteratorCDT
 * -----------------
 * This type represents the concrete type for an iterator.
 */

struct IteratorCDT {
   IteratorHeader header;
   void *collection;
   int elementSize;
   StepIteratorFn stepFn;
   CompareFn cmpFn;
   Cell *head, *tail;
   void *data;
};

/* Private functions prototypes */

static bool stepListIterator(Iterator iterator, void *dst);

/* Part 1 -- Implementation of iterator.h */

Iterator newIterator(void *collection) {
   IteratorHeader *hp = (IteratorHeader *) collection;
   Iterator iterator;

   if (hp->password != ITERATOR_PASSWORD) {
      error("Iteration is not defined for this type");
   }
   if (hp->newFn == 0) return (Iterator) collection;
   iterator = (Iterator) hp->newFn(collection);
   iterator->collection = collection;
   return iterator;
}

bool stepIterator(Iterator iterator, void *dst) {
   return iterator->stepFn(iterator, dst);
}

void freeIterator(Iterator iterator) {
   Cell *cp;

   while ((cp = iterator->head) != NULL) {
      iterator->head = cp->link;
      freeBlock(cp);
   }
   freeBlock(iterator);
}

/* Part 2 -- Implementation of itertype.h */

void enableIteration(void *collection, NewIteratorFn newFn) {
   IteratorHeader *hp = (IteratorHeader *) collection;

   hp->password = ITERATOR_PASSWORD;
   hp->newFn = newFn;
}

Iterator newStepIterator(int size, StepIteratorFn stepFn) {
   Iterator iterator;

   iterator = newBlock(Iterator);
   enableIteration(iterator, 0);
   iterator->elementSize = size;
   iterator->stepFn = stepFn;
   iterator->cmpFn = NULL;
   iterator->head = iterator->tail = NULL;
   iterator->data = NULL;
   return iterator;
}

Iterator newListIterator(int size, CompareFn cmpFn) {
   Iterator iterator;

   iterator = newBlock(Iterator);
   enableIteration(iterator, 0);
   iterator->elementSize = size;
   iterator->stepFn = stepListIterator;
   iterator->cmpFn = cmpFn;
   iterator->head = iterator->tail = NULL;
   iterator->data = NULL;
   return iterator;
}

/*
 * Implementation notes: addToIteratorList
 * ---------------------------------------
 * Most of the work of the package occurs in this function, which
 * inserts the element addressed by dst into its correct position.
 * If the list is unordered or if the element belongs at the end,
 * the element is immediately inserted at the tail.  If not, the
 * implementation walks the list to find the correct position.
 * The local pointers have the following interpretations:
 *
 * np -- pointer to the newly allocated cell
 * pp -- pointer to the cell preceding the insertion point
 * ip -- pointer used as an index in the for loop
 * dp -- pointer to the data field in the block
 */

void addToIteratorList(Iterator iterator, void *dst) {
   Cell *np, *pp, *ip;
   void *dp;

   np = (Cell *) getBlock(sizeof (Cell) + iterator->elementSize);
   dp = ((char *) np) + sizeof (Cell);
   memcpy(dp, dst, iterator->elementSize);
   pp = NULL;
   if (iterator->tail != NULL) {
      if (iterator->cmpFn == NULL || iterator->cmpFn == NULL) {
         pp = iterator->tail;
      } else {
         dp = ((char *) iterator->tail) + sizeof (Cell);
         if (iterator->cmpFn(dst, dp) >= 0) pp = iterator->tail;
      }
   }
   if (pp == NULL) {
      for (ip = iterator->head; ip != NULL; ip = ip->link) {
         dp = ((char *) ip) + sizeof (Cell);
         if (iterator->cmpFn(dst, dp) < 0) break;
         pp = ip;
      }
   }
   if (pp == NULL) {
      np->link = iterator->head;
      if (iterator->head == NULL) iterator->tail = np;
      iterator->head = np;
   } else {
      np->link = pp->link;
      if (pp->link == NULL) iterator->tail = np;
      pp->link = np;
   }
}

void setCollection(Iterator iterator, void *collection) {
   iterator->collection = collection;
}

void *getCollection(Iterator iterator) {
   return iterator->collection;
}

void setIteratorData(Iterator iterator, void *data) {
   iterator->data = data;
}

void *getIteratorData(Iterator iterator) {
   return iterator->data;
}

/* Private functions */

static bool stepListIterator(Iterator iterator, void *dst) {
   Cell *cp;
   void *dp;

   cp = iterator->head;
   if (cp == NULL) {
      iterator->tail = NULL;
      return false;
   }
   dp = ((char *) cp) + sizeof (Cell);
   memcpy(dst, dp, iterator->elementSize);
   iterator->head = cp->link;
   freeBlock(cp);
   return true;
}
