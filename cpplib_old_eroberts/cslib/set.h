/*
 * File: set.h
 * -----------
 * This interface exports an abstract type that represents sets of values.
 */

#ifndef _set_h
#define _set_h

#include <stdarg.h>
#include "cslib.h"
#include "cmpfn.h"

/*
 * Type: Set
 * ---------
 * Defines the abstract type used to represent sets.
 */

typedef struct SetCDT *Set;

/* Exported entries */

/*
 * Function: newSet
 * Usage: set = newSet(type);
 * --------------------------
 * Creates an empty set of values of the specified base type.  The
 * <code>type</code> parameter must be an explicit type name like
 * <code>int</code> or <code>string</code>.
 */

#define newSet(type) newSetFromType(#type)

/*
 * Function: newSetFromType
 * Usage: set = newSetFromType(baseType);
 * --------------------------------------
 * Creates a new set of values with the specified base type expressed as
 * a string.
 */

Set newSetFromType(string baseType);

/*
 * Function: freeSet
 * Usage: freeSet(set);
 * --------------------
 * Frees the storage associated with a set.
 */

void freeSet(Set set);

/*
 * Function: size
 * Usage: n = size(set);
 * ---------------------
 * Returns the number of elements in the set.
 */

int sizeSet(Set set);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(set)) . . .
 * ------------------------------
 * Returns <code>true</code> if the set has no elements.
 */

bool isEmptySet(Set set);

/*
 * Function: clear
 * Usage: clear(set);
 * ------------------
 * Removes all elements from the set.
 */

void clearSet(Set set);

/*
 * Function: clone
 * Usage: newset = clone(set);
 * ---------------------------
 * Creates a copy of the set.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

Set cloneSet(Set set);

/*
 * Function: contains
 * Usage: if (contains(set, value)) . . .
 * --------------------------------------
 * Returns <code>true</code> if the specified value is a member of the set.
 */

bool containsSet(Set set, ...);

/*
 * Friend function: containsSetFromArgs
 * Usage: if (containsSetFromArgs(set, args)) . . .
 * ------------------------------------------------
 * Identical to <code>containsSet</code> except that the argument comes
 * from the <code>args</code> list.
 */

bool containsSetFromArgs(Set set, va_list args);

/*
 * Function: add
 * Usage: add(set, value);
 * -----------------------
 * Adds the specified value to the set.
 */

void addSet(Set set, ...);

/*
 * Friend function: addSetFromArgs
 * Usage: addSetFromArgs(set, args);
 * ---------------------------------
 * Identical to <code>addSet</code> except that the argument comes
 * from the <code>args</code> list.
 */

void addSetFromArgs(Set set, va_list args);

/*
 * Function: remove
 * Usage: remove(set, value);
 * --------------------------
 * Removes the specified value from the set.
 */

void removeSet(Set set, ...);

/*
 * Friend function: removeSetFromArgs
 * Usage: removeSetFromArgs(set, args);
 * ------------------------------------
 * Identical to <code>removeSet</code> except that the argument comes
 * from the <code>args</code> list.
 */

void removeSetFromArgs(Set set, va_list args);

/*
 * Function: equals
 * Usage: if (equals(s1, s2)) . . .
 * --------------------------------
 * Returns <code>true</code> if <code>s1</code> and <code>s2</code>
 * are equal, which means that they contain the same elements.
 */

bool equalsSet(Set s1, Set s2);

/*
 * Function: isSubset
 * Usage: if (isSubset(s1, s2)) . . .
 * ----------------------------------
 * Returns <code>true</code> if <code>s1</code> is a subset of
 * <code>s2</code>.
 */

bool isSubsetSet(Set s1, Set s2);

/*
 * Function: union
 * Usage: set = union(s1, s2);
 * ---------------------------
 * Returns a new set consisting of all elements in either <code>s1</code>
 * or <code>s2</code>.
 */

Set unionSet(Set s1, Set s2);

/*
 * Function: intersection
 * Usage: set = intersection(s1, s2);
 * ----------------------------------
 * Returns a new set consisting of all elements in both <code>s1</code>
 * and <code>s2</code>.
 */

Set intersectionSet(Set s1, Set s2);

/*
 * Function: setDifference
 * Usage: set = setDifference(s1, s2);
 * -----------------------------------
 * Returns a new set consisting of all elements in <code>s1</code>
 * that are not elements of <code>s2</code>.
 */

Set setDifferenceSet(Set s1, Set s2);

/*
 * Function: setCompareFn
 * Usage: setCompareFn(set, cmpFn);
 * --------------------------------
 * Sets the comparison function for elements in the set.  Clients who are
 * using primitive types as base types will not need to call this function.
 */

void setCompareFn(Set set, CompareFn cmpFn);

/*
 * Function: getCompareFn
 * Usage: cmpFn = getCompareFn(set);
 * ---------------------------------
 * Returns the comparison function for elements in the set.
 */

CompareFn getCompareFn(Set set);

#endif
