/*
 * File: stack.c
 * Last modified on Wed Dec  5 21:24:19 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the stack.h interface.  Note that the
 * implementation is independent of the type of value stored
 * in the stack.  That type is defined by the interface as
 * the type name void *.
 */

#include <stdio.h>
#include "cslib.h"
#include "generic.h"
#include "stack.h"
#include "unittest.h"

/*
 * Constant: INITIAL_CAPACITY
 * --------------------------
 * This constant defines the initial capacity of the stack array.
 * Any positive value will work correctly, although changing
 * this parameter can affect performance.  Making this value
 * larger postpones the first reallocation but causes stacks
 * to consume more memory.
 */

#define INITIAL_CAPACITY 100

/*
 * Type: StackCDT
 * --------------
 * The type StackCDT is the concrete representation of the type
 * Stack defined by the interface.  In this implementation,
 * the elements are stored in a dynamic array that doubles in
 * size if the old stack becomes full.
 */

struct StackCDT {
   void **elements;
   int count;
   int capacity;
};

/* Function prototypes */

static void expandCapacity(Stack stack);

/* Exported entries */

Stack newStack(void) {
   Stack stack;

   stack = newBlock(Stack);
   stack->elements = newArray(INITIAL_CAPACITY, void *);
   stack->count = 0;
   stack->capacity = INITIAL_CAPACITY;
   return stack;
}

void freeStack(Stack stack) {
   freeBlock(stack->elements);
   freeBlock(stack);
}

void push(Stack stack, void *element) {
   if (stack->count == stack->capacity) expandCapacity(stack);
   stack->elements[stack->count++] = element;
}

void *pop(Stack stack) {
   if (stack->count == 0) error("pop: Stack is empty");
   return stack->elements[--stack->count];
}

void *peekStack(Stack stack) {
   if (stack->count == 0) error("pop: Stack is empty");
   return stack->elements[stack->count - 1];
}

bool isEmptyStack(Stack stack) {
   return stack->count == 0;
}

int sizeStack(Stack stack) {
   return stack->count;
}

void clearStack(Stack stack) {
   stack->count = 0;
}

Stack cloneStack(Stack stack) {
   Stack newstack;
   int i;

   newstack = newBlock(Stack);
   newstack->count = stack->count;
   newstack->capacity = stack->capacity;
   newstack->elements = newArray(stack->capacity, void *);
   for (i = 0; i < stack->count; i++) {
      newstack->elements[i] = stack->elements[i];
   }
   return newstack;
}

/* Private functions */

/*
 * Implementation notes: expandCapacity
 * ------------------------------------
 * This function expands a full stack by doubling the capacity of its
 * dynamic array, copying the old elements to the new array, and
 * then freeing the old array storage.
 */

static void expandCapacity(Stack stack) {
   void **array;
   int i, newCapacity;

   newCapacity = stack->capacity * 2;
   array = newArray(newCapacity, void *);
   for (i = 0; i < stack->capacity; i++) {
      array[i] = stack->elements[i];
   }
   freeBlock(stack->elements);
   stack->elements = array;
   stack->capacity = newCapacity;
}

/**********************************************************************/
/* Unit test for the stack module                                     */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

void testStackModule(void) {
   Stack stack, stack2;

   trace(stack = newStack());
   test(isEmpty(stack), true);
   trace(push(stack, "A"));
   test(isEmpty(stack), false);
   trace(push(stack, "B"));
   trace(push(stack, "C"));
   test(pop(stack), "C");
   trace(push(stack, "D"));
   test(size(stack), 3);
   test(peek(stack), "D");
   trace(stack2 = clone(stack));
   test(pop(stack), "D");
   test(pop(stack), "B");
   test(pop(stack), "A");
   test(isEmpty(stack), true);
   testError(pop(stack));
   test(pop(stack2), "D");
   test(pop(stack2), "B");
   test(pop(stack2), "A");
}

#endif
