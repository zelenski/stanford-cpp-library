/*
 * File: hashcode.h
 * ----------------
 * This file declares global hashing functions for various common data types.
 * These functions are used by the HashMap and HashSet collections, as well as
 * by other collections that wish to be used as elements within HashMaps/Sets.
 * 
 * @version 2017/09/29
 * - added composite hashCode functions (hashCode2, 3, 4, ...)
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

/*
 * Computes a composite hash code from two values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2>
int hashCode2(T1 t1, T2 t2) {
    int code = hashSeed();
    code += hashCode(t1);
    code *= hashMultiplier();
    code += hashCode(t2);
    return int(code & hashMask());
}

/*
 * Computes a composite hash code from three values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2, typename T3>
int hashCode3(T1 t1, T2 t2, T3 t3) {
    int code = hashSeed();
    code += hashCode(t1);
    code *= hashMultiplier();
    code += hashCode(t2);
    code *= hashMultiplier();
    code += hashCode(t3);
    return int(code & hashMask());
}

/*
 * Computes a composite hash code from four values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2, typename T3, typename T4>
int hashCode4(T1 t1, T2 t2, T3 t3, T4 t4) {
    int code = hashSeed();
    code += hashCode(t1);
    code *= hashMultiplier();
    code += hashCode(t2);
    code *= hashMultiplier();
    code += hashCode(t3);
    code *= hashMultiplier();
    code += hashCode(t4);
    return int(code & hashMask());
}

/*
 * Computes a composite hash code from five values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2, typename T3, typename T4, typename T5>
int hashCode5(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) {
    int code = hashSeed();
    code += hashCode(t1);
    code *= hashMultiplier();
    code += hashCode(t2);
    code *= hashMultiplier();
    code += hashCode(t3);
    code *= hashMultiplier();
    code += hashCode(t4);
    code *= hashMultiplier();
    code += hashCode(t5);
    return int(code & hashMask());
}

/*
 * Computes a composite hash code from six values.
 * The components are scaled up so as to spread out the range of values
 * and reduce collisions.
 * The type of each value passed must have a suitable hashCode() function.
 */
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
int hashCode6(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) {
    int code = hashSeed();
    code += hashCode(t1);
    code *= hashMultiplier();
    code += hashCode(t2);
    code *= hashMultiplier();
    code += hashCode(t3);
    code *= hashMultiplier();
    code += hashCode(t4);
    code *= hashMultiplier();
    code += hashCode(t5);
    code *= hashMultiplier();
    code += hashCode(t6);
    return int(code & hashMask());
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _hashcode_h
