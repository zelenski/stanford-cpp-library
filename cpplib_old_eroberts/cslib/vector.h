/*
 * File: vector.h
 * --------------
 * This interface exports an array-like indexed collection type.
 */

#ifndef _vector_h_
#define _vector_h_

#include "cslib.h"
#include "generic.h"

/*
 * Type: Vector
 * ------------
 * This type defines the abstract vector type.
 */

typedef struct VectorCDT *Vector;

/* Exported entries */

/*
 * Function: newVector
 * Usage: vector = newVector();
 * ----------------------------
 * Returns an empty <code>Vector</code>.
 */

Vector newVector(void);

/*
 * Function: freeVector
 * Usage: freeVector(vector);
 * --------------------------
 * Frees the storage associated with vector.
 */

void freeVector(Vector vector);

/*
 * Function: arrayToVector
 * Usage: vector = arrayToVector(array, n);
 * ----------------------------------------
 * Creates a vector from an array of <code>void&nbsp;*</code> pointers.
 * If the <code>array</code> argument is <code>NULL</code>, this
 * function returns <code>NULL</code>.
 */

Vector arrayToVector(void *array[], int n);

/*
 * Function: vectorToArray
 * Usage: array = vectorToArray(vector);
 * -------------------------------------
 * Returns a <code>NULL</code>-terminated array with the same
 * elements as vector.  If vector is <code>NULL</code>, this
 * function returns <code>NULL</code>.
 */

void **vectorToArray(Vector vector);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(vector)) . . .
 * ---------------------------------
 * Returns <code>true</code> if the vector is empty.
 */

bool isEmptyVector(Vector vector);

/*
 * Function: size
 * Usage: n = size(vector);
 * ------------------------
 * Returns the number of elements in the vector.
 */

int sizeVector(Vector vector);

/*
 * Function: clear
 * Usage: clear(vector);
 * ---------------------
 * Removes all elements from the vector.
 */

void clearVector(Vector vector);

/*
 * Function: clone
 * Usage: newvec = clone(vector);
 * ------------------------------
 * Creates a copy of the vector.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

Vector cloneVector(Vector vector);

/*
 * Function: get
 * Usage: value = get(vector, index);
 * ----------------------------------
 * Gets the element at the specified index position, raising an
 * error if the index is out of range.
 */

void *getVector(Vector vector, int index);

/*
 * Function: set
 * Usage: set(vector, index, value);
 * ---------------------------------
 * Sets the element at the specified index position, raising an
 * error if the index is out of range.
 */

void setVector(Vector vector, int index, void *value);

/*
 * Function: add
 * Usage: add(vector, value);
 * --------------------------
 * Adds a new value to the end of the vector.
 */

void addVector(Vector vector, void *value);

/*
 * Function: insert
 * Usage: insert(vector, index, value);
 * ------------------------------------
 * Inserts a new value before the specified index position.
 */

void insert(Vector vector, int index, void *value);

/*
 * Function: remove
 * Usage: remove(vector, index);
 * -----------------------------
 * Deletes the element at the specified index position.
 */

void removeVector(Vector vector, int index);

#endif
