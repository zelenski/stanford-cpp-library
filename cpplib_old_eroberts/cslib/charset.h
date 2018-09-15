/*
 * File: charset.h
 * ---------------
 * This interface exports an abstract type that represents sets of characters.
 */

#ifndef _charset_h
#define _charset_h

#include <stdarg.h>
#include "cslib.h"
#include "generic.h"

/*
 * Type: CharSet
 * -------------
 * Defines the abstract type used to represent sets of characters
 */

typedef struct CharSetCDT *CharSet;

/* Exported entries */

/*
 * Function: newCharSet
 * Usage: set = newCharSet();
 * --------------------------
 * Creates an empty set capable of holding characters.
 */

CharSet newCharSet();

/*
 * Function: freeCharSet
 * Usage: freeCharSet(set);
 * ------------------------
 * Frees the storage associated with a character set.
 */

void freeCharSet(CharSet set);

/*
 * Function: size
 * Usage: n = size(set);
 * ---------------------
 * Returns the number of elements in the set.
 */

int sizeCharSet(CharSet set);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(set)) . . .
 * ------------------------------
 * Returns <code>true</code> if the set has no elements.
 */

bool isEmptyCharSet(CharSet set);

/*
 * Function: clear
 * Usage: clear(set);
 * ------------------
 * Removes all characters from the set.
 */

void clearCharSet(CharSet set);

/*
 * Function: clone
 * Usage: newset = clone(set);
 * ---------------------------
 * Creates a copy of the set.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

CharSet cloneCharSet(CharSet set);

/*
 * Function: contains
 * Usage: if (contains(set, ch)) . . .
 * -----------------------------------
 * Returns <code>true</code> if the character <code>ch</code> is a member
 * of the set.
 */

bool containsCharSet(CharSet set, char ch);

/*
 * Function: add
 * Usage: add(set, ch);
 * --------------------
 * Adds the character <code>ch</code> to the set.
 */

void addCharSet(CharSet set, char ch);

/*
 * Function: addString
 * Usage: addString(set, str);
 * ---------------------------
 * Adds the characters in <code>str</code> to the set.
 */

void addString(CharSet set, string str);

/*
 * Function: remove
 * Usage: remove(set, ch);
 * -----------------------
 * Removes an element from the set.
 */

void removeCharSet(CharSet set, char ch);

/*
 * Function: equals
 * Usage: if (equals(s1, s2)) . . .
 * --------------------------------
 * Returns <code>true</code> if <code>s1</code> and <code>s2</code>
 * are equal, which means that they contain the same elements.
 */

bool equalsCharSet(CharSet s1, CharSet s2);

/*
 * Function: isSubset
 * Usage: if (isSubset(s1, s2)) . . .
 * ----------------------------------
 * Returns <code>true</code> if <code>s1</code> is a subset of <code>s2</code>.
 */

bool isSubsetCharSet(CharSet s1, CharSet s2);

/*
 * Function: union
 * Usage: set = union(s1, s2);
 * ---------------------------
 * Returns a new set consisting of all elements in either <code>s1</code>
 * or <code>s2</code>.
 */

CharSet unionCharSet(CharSet s1, CharSet s2);

/*
 * Function: intersection
 * Usage: set = intersection(s1, s2);
 * ----------------------------------
 * Returns a new set consisting of all elements in both <code>s1</code>
 * and <code>s2</code>.
 */

CharSet intersectionCharSet(CharSet s1, CharSet s2);

/*
 * Function: setDifference
 * Usage: set = setDifference(s1, s2);
 * -----------------------------------
 * Returns a new set consisting of all elements in <code>s1</code> that
 * are not elements of <code>s2</code>.
 */

CharSet setDifferenceCharSet(CharSet s1, CharSet s2);

#endif
