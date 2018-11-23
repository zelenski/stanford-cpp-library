/*
 * File: vector.c
 * Last modified on Tue Jan 15 15:27:23 2013 by eroberts
 * -----------------------------------------------------
 * This file implements the vector.h interface.
 */

#include <stdio.h>
#include "cslib.h"
#include "generic.h"
#include "iterator.h"
#include "itertype.h"
#include "strlib.h"
#include "unittest.h"
#include "vector.h"

/*
 * Constant: INITIAL_CAPACITY
 * --------------------------
 * This constant defines the initial capacity of the array.
 * Any positive value will work correctly, although changing
 * this parameter can affect performance.  Making this value
 * larger postpones the first reallocation but causes vectors
 * to consume more memory.
 */

#define INITIAL_CAPACITY 10

/*
 * Type: VectorCDT
 * ---------------
 * The type VectorCDT is the concrete representation of the type
 * Vector defined by the interface.
 */

struct VectorCDT {
   IteratorHeader header;
   void **elements;
   int count;
   int capacity;
};

/* Private function prototypes */

static void expandCapacity(Vector vector);
static Iterator newVectorIterator(void *collection);

/* Exported entries */

Vector newVector(void) {
   Vector vector;

   vector = newBlock(Vector);
   enableIteration(vector, newVectorIterator);
   vector->elements = newArray(INITIAL_CAPACITY, void *);
   vector->count = 0;
   vector->capacity = INITIAL_CAPACITY;
   return vector;
}

void freeVector(Vector vector) {
   freeBlock(vector->elements);
   freeBlock(vector);
}

Vector arrayToVector(void *array[], int n) {
   Vector vector;
   int i;

   if (array == NULL) return NULL;
   vector = newBlock(Vector);
   enableIteration(vector, newVectorIterator);
   vector->elements = newArray(n, void *);
   vector->count = n;
   vector->capacity = n;
   for (i = 0; i < n; i++) {
      vector->elements[i] = array[i];
   }
   return vector;
}

void **vectorToArray(Vector vector) {
   void **array;
   int i, n;

   if (vector == NULL) return NULL;
   n = vector->count;
   array = newArray(n + 1, void *);
   for (i = 0; i < n; i++) {
      array[i] = vector->elements[i];
   }
   array[n] = NULL;
   return array;
}

bool isEmptyVector(Vector vector) {
   return vector->count == 0;
}

int sizeVector(Vector vector) {
   return vector->count;
}

void clearVector(Vector vector) {
   vector->count = 0;
}

Vector cloneVector(Vector vector) {
   Vector newvec;
   int i;

   newvec = newBlock(Vector);
   enableIteration(newvec, newVectorIterator);
   newvec->count = vector->count;
   newvec->capacity = vector->capacity;
   newvec->elements = newArray(vector->capacity, void *);
   for (i = 0; i < vector->count; i++) {
      newvec->elements[i] = vector->elements[i];
   }
   return newvec;
}

void *getVector(Vector vector, int index) {
   if (index < 0 || index >= vector->count) {
      error("get: Index value out of range");
   }
   return vector->elements[index];
}

void setVector(Vector vector, int index, void *value) {
   if (index < 0 || index >= vector->count) {
      error("get: Index value out of range");
   }
   vector->elements[index] = value;
}

void addVector(Vector vector, void *value) {
   if (vector->count == vector->capacity) {
      expandCapacity(vector);
   }
   vector->elements[vector->count++] = value;
}

void insert(Vector vector, int index, void *value) {
   int i;

   if (index < 0 || index > vector->count) {
      error("insert: Index value out of range");
   }
   if (vector->count == vector->capacity) {
      expandCapacity(vector);
   }
   for (i = vector->count; i > index; i--) {
      vector->elements[i] = vector->elements[i - 1];
   }
   vector->elements[index] = value;
   vector->count++;
}

void removeVector(Vector vector, int index) {
   int i;

   if (index < 0 || index >= vector->count) {
      error("remove: Index value out of range");
   }
   vector->count--;
   for (i = index; i < vector->count; i++) {
      vector->elements[i] = vector->elements[i + 1];
   }
}

/* Private functions */

static void expandCapacity(Vector vector) {
   void **array;
   int i, newCapacity;

   newCapacity = vector->capacity * 2;
   array = newArray(newCapacity, void *);
   for (i = 0; i < vector->count; i++) {
      array[i] = vector->elements[i];
   }
   freeBlock(vector->elements);
   vector->elements = array;
   vector->capacity = newCapacity;
}

/*
 * Implementation notes: newVectorIterator
 * ---------------------------------------
 * This functions makes it possible to use the general iterator
 * facility on vectors.  For details on the general strategy, see
 * the comments in the itertype.h interface.
 */

static Iterator newVectorIterator(void *collection) {
   Vector vector;
   Iterator iterator;
   int i;

   vector = (Vector) collection;
   iterator = newListIterator(sizeof (void *), NULL);
   for (i = 0; i < vector->count; i++) {
      addToIteratorList(iterator, &vector->elements[i]);
   }
   return iterator;
}

/**********************************************************************/
/* Unit test for the vector module                                    */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

void testVectorModule(void) {
   Vector vec, vec2;

   trace(vec = newVector());
   test(isEmpty(vec), true);
   trace(add(vec, "A"));
   test(isEmpty(vec), false);
   trace(add(vec, "C"));
   trace(add(vec, "D"));
   trace(insert(vec, 1, "B"));
   test(size(vec), 4);
   test(get(vec, 0), "A");
   test(get(vec, 1), "B");
   test(get(vec, 2), "C");
   test(get(vec, 3), "D");
   testError(get(vec, 4));
   testError(get(vec, -1));
   trace(vec2 = clone(vec));
   trace(set(vec, 0, "a"));
   test(get(vec, 0), "a");
   trace(remove(vec, 0));
   test(size(vec), 3);
   trace(insert(vec, 0, "A"));
   test(get(vec, 0), "A");
   trace(insert(vec, size(vec), "E"));
   test(get(vec, 4), "E");
   test(get(vec2, 0), "A");
   test(get(vec2, 1), "B");
   test(get(vec2, 2), "C");
   test(get(vec2, 3), "D");
}

#endif
