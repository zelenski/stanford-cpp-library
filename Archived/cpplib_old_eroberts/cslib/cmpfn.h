/*
 * File: cmpfn.h
 * -------------
 * This interface exports several comparison functions for use with the
 * other library packages.
 */

#ifndef _cmpfn_h
#define _cmpfn_h

#include "cslib.h"

/*
 * Type: CompareFn
 * ---------------
 * This type defines the type space of comparison functions,
 * each of which take the addresses of their arguments and
 * return an integer from the set {-1, 0, +1} depending on
 * whether the first argument is less than, equal to, or
 * greater than the second.
 */

typedef int (*CompareFn)(const void *p1, const void *p2);

/*
 * Function: intCmpFn
 * ------------------
 * Compares two values of type <code>int</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int intCmpFn(const void *p1, const void *p2);

/*
 * Function: shortCmpFn
 * --------------------
 * Compares two values of type <code>short</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int shortCmpFn(const void *p1, const void *p2);

/*
 * Function: longCmpFn
 * -------------------
 * Compares two values of type <code>long</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int longCmpFn(const void *p1, const void *p2);

/*
 * Function: charCmpFn
 * -------------------
 * Compares two values of type <code>char</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int charCmpFn(const void *p1, const void *p2);

/*
 * Function: floatCmpFn
 * --------------------
 * Compares two values of type <code>float</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int floatCmpFn(const void *p1, const void *p2);

/*
 * Function: doubleCmpFn
 * ---------------------
 * Compares two values of type <code>double</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int doubleCmpFn(const void *p1, const void *p2);

/*
 * Function: unsignedCmpFn
 * -----------------------
 * Compares two values of type <code>unsigned</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int unsignedCmpFn(const void *p1, const void *p2);

/*
 * Function: unsignedShortCmpFn
 * ----------------------------
 * Compares two values of type <code>unsignedshort</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int unsignedShortCmpFn(const void *p1, const void *p2);

/*
 * Function: unsignedLongCmpFn
 * ---------------------------
 * Compares two values of type <code>unsignedlong</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int unsignedLongCmpFn(const void *p1, const void *p2);

/*
 * Function: unsignedCharCmpFn
 * ---------------------------
 * Compares two values of type <code>unsignedchar</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int unsignedCharCmpFn(const void *p1, const void *p2);

/*
 * Function: stringCmpFn
 * ---------------------
 * Compares two values of type <code>string</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 */

int stringCmpFn(const void *p1, const void *p2);

/*
 * Function: pointerCmpFn
 * ----------------------
 * Compares two values of type <code>pointer</code> whose addresses are
 * supplied by the pointer arguments <code>p1</code> and <code>p2</code>.
 * Pointer comparison is based on the numeric equivalent of the pointer.
 */

int pointerCmpFn(const void *p1, const void *p2);

#endif
