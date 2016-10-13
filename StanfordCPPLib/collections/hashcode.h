/*
 * File: hashcode.h
 * ----------------
 * This file declares global hashing functions for various common data types.
 * These functions are used by the HashMap and HashSet collections, as well as
 * by other collections that wish to be used as elements within HashMaps/Sets.
 * 
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 *   (hashSeed(), hashMultiplier(), and hashMask())
 */

#ifndef _hashcode_h
#define _hashcode_h

#include <string>

/*
 * Function: hashCode
 * Usage: int hash = hashCode(key);
 * --------------------------------
 * Returns a hash code for the specified key, which is always a
 * nonnegative integer.  This function is overloaded to support
 * all of the primitive types and the C++ <code>string</code> type.
 */
int hashCode(bool key);
int hashCode(char key);
int hashCode(double key);
int hashCode(float key);
int hashCode(int key);
int hashCode(long key);
int hashCode(const char* str);
int hashCode(const std::string& str);
int hashCode(void* key);

/*
 * Constants that are used to help implement these functions
 * (see hashcode.h for example usage)
 */
int hashSeed();         // Starting point for first cycle
int hashMultiplier();   // Multiplier for each cycle
int hashMask();         // All 1 bits except the sign

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _hashcode_h
