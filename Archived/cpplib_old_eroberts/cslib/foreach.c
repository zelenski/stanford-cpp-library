/*
 * File: foreach.c
 * Last modified on Sun Jul 26 16:56:49 1998 by eroberts
 * -----------------------------------------------------
 * This file implements the foreach construct.
 */

#include <stdio.h>
#include "foreach.h"
#include "cslib.h"
#include "iterator.h"

typedef struct Cell {
   void *dst;
   Iterator iterator;
   struct Cell *link;
} Cell;

static Cell *iteratorList = NULL;

/* Entry points */

void initForEach(void *dst, void *collection) {
   Cell *cp;

   for (cp = iteratorList; cp != NULL && cp->dst != dst; cp = cp->link) {
      /* Empty */
   }
   if (cp == NULL) {
      cp = newBlock(Cell *);
      cp->dst = dst;
      cp->link = iteratorList;
      iteratorList = cp;
   } else {
      if (cp->iterator != NULL) freeIterator(cp->iterator);
   }
   cp->iterator = newIterator(collection);
}

bool stepForEach(void *dst) {
   Cell *cp;
   bool result;

   for (cp = iteratorList; cp != NULL && cp->dst != dst; cp = cp->link) {
      /* Empty */
   }
   if (cp == NULL) error("foreach iterator undefined");
   result = stepIterator(cp->iterator, dst);
   if (!result) {
      freeIterator(cp->iterator);
      cp->iterator = NULL;
   }
   return result;
}
