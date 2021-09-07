/*
 * File: hashcode.h
 * ----------------
 * This file declares global hashing functions for various common data types.
 * These functions are used by the HashMap and HashSet collections, as well as
 * by other collections that wish to be used as elements within HashMaps/Sets.
 *
 * @version 2017/10/21
 * - added hash codes for short, unsigned integers
 * @version 2017/09/29
 * - added composite hashCode functions (hashCode2, 3, 4, ...)
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 *   (hashSeed(), hashMultiplier(), and hashMask())
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _hashcode_h
#define _hashcode_h

#include <string>
#include <utility>

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
int hashCode(long double key);
int hashCode(int key);
int hashCode(unsigned int key);
int hashCode(long key);
int hashCode(unsigned long key);
int hashCode(short key);
int hashCode(unsigned short key);
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

/*
 * Computes a composite hash code from a list of multiple values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2, typename... Others>
int hashCode(T1&& first, T2&& second, Others&&... remaining) {
    int result = hashSeed();

    /* Compute the hash code for the last n - 1 arguments. */
    result += hashCode(std::forward<T2>(second), std::forward<Others>(remaining)...);

    /* Update the hash to factor in the hash of the first element. */
    result *= hashMultiplier();
    result += hashCode(std::forward<T1>(first));

    /* Hash the resulting integer to mask off any unneeded bits. */
    return hashCode(result);
}

#endif // _hashcode_h
