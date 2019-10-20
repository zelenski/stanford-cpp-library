/*
 * File: stack.h
 * -------------
 * This interface exports an abstraction for stacks.
 */

#ifndef _stack_h
#define _stack_h

#include "cslib.h"
#include "generic.h"

/*
 * Type: Stack
 * -----------
 * The abstract data type for the stack.
 */

typedef struct StackCDT *Stack;

/*
 * Function: newStack
 * Usage: stack = newStack();
 * --------------------------
 * Allocates and returns a new stack, which is initially empty.
 */

Stack newStack(void);

/*
 * Function: freeStack
 * Usage: freeStack(stack);
 * ------------------------
 * Frees the storage associated with the stack.
 */

void freeStack(Stack stack);

/*
 * Function: push
 * Usage: push(stack, element);
 * ----------------------------
 * Pushes the specified element onto the stack.
 */

void push(Stack stack, void *element);

/*
 * Function: pop
 * Usage: element = pop(stack);
 * ----------------------------
 * Pops the top element from the stack and returns that value.
 * The first value popped is always the last one  that was pushed.
 * If the stack is empty when <code>pop</code> is called, the
 * implementation calls <code>error</code> with an appropriate message.
 */

void *pop(Stack stack);

/*
 * Function: peek
 * Usage: element = peek(stack);
 * -----------------------------
 * Returns the top element from the stack without removing it.
 * If the stack is empty when <code>pop</code> is called, the
 * implementation calls <code>error</code> with an appropriate message.
 */

void *peekStack(Stack stack);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(stack)) . . .
 * --------------------------------
 * Tests whether the stack is empty.
 */

bool isEmptyStack(Stack stack);

/*
 * Function: size
 * Usage: n = size(stack);
 * -----------------------
 * Returns the number of elements currently pushed on the stack.
 */

int sizeStack(Stack stack);

/*
 * Function: clear
 * Usage: clear(stack);
 * --------------------
 * Removes all elements from the stack.
 */

void clearStack(Stack stack);

/*
 * Function: clone
 * Usage: newstack = clone(stack);
 * -------------------------------
 * Creates a copy of the stack.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

Stack cloneStack(Stack stack);

#endif
