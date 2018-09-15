/*
 * File: ref.h
 * -----------
 * This interface exports several simple functions for allocating
 * pointers to the atomic types.
 */

#ifndef _ref_h
#define _ref_h

#include "cslib.h"

/*
 * Function: newRefInt
 * Usage: ref = newRefInt(value);
 * ------------------------------
 * Allocates a ref pointing to the specified <code>int</code> value.
 */

void *newRefInt(int value);

/*
 * Function: newRefShort
 * Usage: ref = newRefShort(value);
 * --------------------------------
 * Allocates a ref pointing to the specified <code>short</code> value.
 */

void *newRefShort(short value);

/*
 * Function: newRefLong
 * Usage: ref = newRefLong(value);
 * -------------------------------
 * Allocates a ref pointing to the specified <code>long</code> value.
 */

void *newRefLong(long value);

/*
 * Function: newRefFloat
 * Usage: ref = newRefFloat(value);
 * --------------------------------
 * Allocates a ref pointing to the specified <code>float</code> value.
 */

void *newRefFloat(float value);

/*
 * Function: newRefDouble
 * Usage: ref = newRefDouble(value);
 * ---------------------------------
 * Allocates a ref pointing to the specified <code>double</code> value.
 */

void *newRefDouble(double value);

/*
 * Function: newRefChar
 * Usage: ref = newRefChar(value);
 * -------------------------------
 * Allocates a ref pointing to the specified <code>char</code> value.
 */

void *newRefChar(char value);

/*
 * Function: newRefBool
 * Usage: ref = newRefBool(value);
 * -------------------------------
 * Allocates a ref pointing to the specified <code>bool</code> value.
 */

void *newRefBool(bool value);

/*
 * Function: newRefUnsigned
 * Usage: ref = newRefUnsigned(value);
 * -----------------------------------
 * Allocates a ref pointing to the specified <code>unsigned</code> value.
 */

void *newRefUnsigned(unsigned value);

/*
 * Function: newRefUnsignedShort
 * Usage: ref = newRefUnsignedShort(value);
 * ----------------------------------------
 * Allocates a ref pointing to the specified <code>unsigned short</code> value.
 */

void *newRefUnsignedShort(unsigned short value);

/*
 * Function: newRefUnsignedLong
 * Usage: ref = newRefUnsignedLong(value);
 * ---------------------------------------
 * Allocates a ref pointing to the specified <code>unsigned long</code> value.
 */

void *newRefUnsignedLong(unsigned long value);

/*
 * Function: newRefUnsignedChar
 * Usage: ref = newRefUnsignedChar(value);
 * ---------------------------------------
 * Allocates a ref pointing to the specified <code>unsigned char</code> value.
 */

void *newRefUnsignedChar(unsigned char value);

/*
 * Part 2 -- Functions to dereference generic pointers
 * ---------------------------------------------------
 * These functions take a generic pointer of type void * and return
 * the value to which it points, which must be of the type indicated
 * by the function name.  For example, refToInt(ref) interprets ref
 * as a pointer to an int and returns the integer at that address.
 */

/*
 * Function: refToInt
 * Usage: i = refToInt(ref);
 * -------------------------
 * Returns the <code>int</code> to which this reference points.
 */

int refToInt(void *ref);

/*
 * Function: refToShort
 * Usage: s = refToShort(ref);
 * ---------------------------
 * Returns the <code>short</code> to which this reference points.
 */

short refToShort(void *ref);

/*
 * Function: refToLong
 * Usage: l = refToLong(ref);
 * --------------------------
 * Returns the <code>long</code> to which this reference points.
 */

long refToLong(void *ref);

/*
 * Function: refToFloat
 * Usage: f = refToFloat(ref);
 * ---------------------------
 * Returns the <code>float</code> to which this reference points.
 */

float refToFloat(void *ref);

/*
 * Function: refToDouble
 * Usage: d = refToDouble(ref);
 * ----------------------------
 * Returns the <code>double</code> to which this reference points.
 */

double refToDouble(void *ref);

/*
 * Function: refToChar
 * Usage: c = refToChar(ref);
 * --------------------------
 * Returns the <code>char</code> to which this reference points.
 */

char refToChar(void *ref);

/*
 * Function: refToBool
 * Usage: b = refToBool(ref);
 * --------------------------
 * Returns the <code>bool</code> to which this reference points.
 */

bool refToBool(void *ref);

/*
 * Function: refToUnsigned
 * Usage: u = refToUnsigned(ref);
 * ------------------------------
 * Returns the <code>unsigned</code> to which this reference points.
 */

unsigned refToUnsigned(void *ref);

/*
 * Function: refToUnsignedShort
 * Usage: us = refToUnsignedShort(ref);
 * ------------------------------------
 * Returns the <code>unsigned short</code> to which this reference points.
 */

unsigned short refToUnsignedShort(void *ref);

/*
 * Function: refToUnsignedLong
 * Usage: ul = refToUnsignedLong(ref);
 * -----------------------------------
 * Returns the <code>unsigned long</code> to which this reference points.
 */

unsigned long refToUnsignedLong(void *ref);

/*
 * Function: refToUnsignedChar
 * Usage: uc = refToUnsignedChar(ref);
 * -----------------------------------
 * Returns the <code>unsigned char</code> to which this reference points.
 */

unsigned char refToUnsignedChar(void *ref);

#endif
