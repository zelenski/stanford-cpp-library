/*
 * File: cslib.h
 * -------------
 * This interface defines a basic set of definitions that are shared
 * among all interfaces in the package.  These basic definitions include:
 *
 *<ol>
 *  <li>Declarations for several new &ldquo;primitive&rdquo; types used
 *      throughout the libraries as fundamental types.
 *
 *  <li>A function for error handling.
 *
 *  <li>A flexible set of functions for memory allocation.
 *</ol>
 */

#ifndef _cslib_h
#define _cslib_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* Section 1 -- Define new "primitive" types */

/*
 * Type: bool
 * ----------
 * This type defines the space of Boolean data using the constants
 * <code>false</code> and <code>true</code>.
 */

#ifdef bool
#  undef bool
#endif

#ifdef false
#  undef false
#endif

#ifdef true
#  undef true
#endif

typedef enum {false, true} bool;

/*
 * Type: string
 * ------------
 * This type is defined to be identical with <code>char *</code>.
 */

typedef char *string;

/*
 * Type: proc
 * ----------
 * This function type represents an arbitrary procedure that can be passed
 * to an abstraction and then called back from the implementation.
 */

typedef void (*proc)();

/* Section 2 -- Memory allocation */

/*
 * General notes
 * -------------
 * These functions provide a common interface for memory allocation.
 * All functions in the library that allocate memory do so using
 * <code>getBlock</code> and <code>freeBlock</code>.  Even though the
 * ANSI standard defines <code>malloc</code> and <code>free</code> for
 * the same purpose, using <code>getBlock</code> and <code>freeBlock</code>
 * provides greater compatibility with non-ANSI implementations, automatic
 * out-of-memory error detection, limited type checking, and the possibility
 * of installing a garbage-collecting allocator.
 */

/*
 * Function: getBlock
 * Usage: ptr = getBlock(nbytes);
 * ------------------------------
 * Allocates a block of memory of the given size.  If no memory is
 * available, <code>getBlock</code> generates an error.
 */

void *getBlock(size_t nbytes);

/*
 * Private function: getTypedBlock
 * -------------------------------
 * Returns a block with the indicated type marker.  This function is
 * called from the <code>newBlock</code> and <code>newArray</code>
 * macros and should not be invoked by clients.
 */

void *getTypedBlock(size_t nbytes, string type);

/*
 * Function: freeBlock
 * Usage: freeBlock(ptr);
 * ----------------------
 * Frees the memory associated with <code>ptr</code>, which must have
 * been allocated using <code>getBlock</code>, <code>newBlock</code>, or
 * <code>newArray</code>.  If the block appears to be in static memory
 * or allocated by <code>malloc</code>, the call to <code>freeBlock</code>
 * has no effect.
 */

void freeBlock(void *ptr);

/*
 * Function: getBlockType
 * Usage: type = getBlockType(ptr);
 * --------------------------------
 * Returns a string indicating the type of the block.  If the block is
 * created using the <code>newBlock</code> macro, the string is the type
 * argument.  If the block is created using <code>newArray</code>, the
 * string consists of the base type followed by the string <code>"[]"</code>.
 * In all other cases, the type is returned as <code>"?"</code>.  This
 * string is constant and should not be freed.
 */

string getBlockType(void *ptr);

/*
 * Function: setBlockData
 * Usage: setBlockData(ptr, value);
 * --------------------------------
 * Sets the data field inside the block to the specified value.
 */

void setBlockData(void *ptr, void *value);

/*
 * Function: getBlockData
 * Usage: value = getBlockData(ptr);
 * ---------------------------------
 * Returns the data field inside the block.
 */

void *getBlockData(void *ptr);

/*
 * Macro: newBlock
 * Usage: ptr = (type) newBlock(type);
 * -----------------------------------
 * Allocates enough space to hold an object of the type to which
 * <code>type</code> points (note that <code>type</code> must be
 * a pointer type).  Note that <code>newBlock</code> is different from
 * the <code>new</code> operator used in C++; the former takes a
 * pointer type and the latter takes the target type.
 */

#define newBlock(type) ((type) getTypedBlock(sizeof *((type) NULL), #type))

/*
 * Macro: newArray
 * Usage: ptr = newArray(n, type);
 * -------------------------------
 * Allocates enough space to hold an array of <code>n</code> values
 * of the specified type.
 */

#define newArray(n, type) \
   ((type *) getTypedBlock((n) * sizeof(type), #type "[]"))

/* Section 3 -- error handling */

/*
 * Function: error
 * Usage: error(msg, . . .);
 * -------------------------
 * Generates an error string, expanding <code>%</code> constructions
 * appearing in the error message string just as <code>printf</code> does.
 * The behavior depends on whether the call is compiled in C or C++.
 *
 * <p>In C, calling <code>error</code> first checks to see if there is a
 * handler for <code>ErrorException</code>.  If so, calling
 * <code>error</code> throws an <code>ErrorException</code>
 * exception with the expanded error string as argument.  If
 * there is no <code>ErrorException</code> handler, the program
 * prints the message to the standard error channel and exits
 * with a status code indicating failure (as given by the
 * constant <code>ERROR_EXIT_STATUS</code>).
 *
 * <p>In C++, calling <code>error</code> throws an exception with the
 * expanded error string (defined as a C++ string) as its value.
 */

   void error(string msg, ...);

/* Section 4 -- Redefine main */

#define main void main_

#endif
