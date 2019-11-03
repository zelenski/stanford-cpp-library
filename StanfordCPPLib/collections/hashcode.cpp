/*
 * File: hashcode.cpp
 * ------------------
 * This file implements the interface declared in hashcode.h.
 *
 * @version 2019/04/16
 * - bugfix for win64 involving hashCode for void* pointers
 * @version 2018/08/10
 * - bugfixes involving negative hash codes, unified string hashing
 * @version 2017/10/21
 * - added hash codes for short, unsigned integers
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 */

#include "hashcode.h"
#include <cstddef>       // For size_t
#include <cstdint>       // For uintptr_t
#include <cstring>       // For strlen

static const int HASH_SEED = 5381;               // Starting point for first cycle
static const int HASH_MULTIPLIER = 33;           // Multiplier for each cycle
static const int HASH_MASK = unsigned(-1) >> 1;  // All 1 bits except the sign

int hashSeed() {
    return HASH_SEED;
}

int hashMultiplier() {
    return HASH_MULTIPLIER;
}

int hashMask() {
    return HASH_MASK;
}

/* 
 * Implementation notes: hashCode(int)
 * -----------------------------------
 * Hash code for integers masks off the sign bit, guaranteeing a nonnegative value.
 */
int hashCode(int key) {
    return key & HASH_MASK;
}

/* 
 * Implementation notes: hashCode(other primitive types)
 * -----------------------------------------------------
 * Hash codes for all other primitive types forward to the hash code for integers.
 * This ensures that all hash codes get the proper masking treatment.
 *
 * Thanks to Jeremy Barenholtz for identifying that the original versions of these
 * functions, which just cast their arguments to integers, could lead to negative
 * results.
 */
int hashCode(bool key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(char key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned int key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(long key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned long key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(short key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned short key) {
    return hashCode(static_cast<int>(key));
}

#ifdef _WIN64
int hashCode(uintptr_t key) {
    return hashCode(static_cast<unsigned long>(key));
}
#endif // _WIN64

/* 
 * Implementation notes: hashCode(void*)
 * -----------------------------------------------------
 * Catch-all handler for pointers not matched by other
 * overloads just treats the pointer value numerically.
 */
int hashCode(void* key) {
    return hashCode(reinterpret_cast<uintptr_t>(key));
}

/*
 * Implementation notes: hashCode(string), hashCode(double)
 * --------------------------------------------------------
 * This function takes a string key and uses it to derive a hash code,
 * which is a nonnegative integer related to the key by a deterministic
 * function that distributes keys well across the space of integers.
 * The general method is called linear congruence, which is also used
 * in random-number generators.  The specific algorithm used here is
 * called djb2 after the initials of its inventor, Daniel J. Bernstein,
 * Professor of Mathematics at the University of Illinois at Chicago.
 */
int hashCode(const char* base, size_t numBytes) {
    unsigned hash = HASH_SEED;
    for (size_t i = 0; i < numBytes; i++) {
        hash = HASH_MULTIPLIER * hash + base[i];
    }
    return hashCode(hash);
} 

int hashCode(const char* str) {
    return hashCode(str, strlen(str));
}

int hashCode(const std::string& str) {
    return hashCode(str.data(), str.length());
}

int hashCode(double key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(double));
}

int hashCode(float key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(float));
}

int hashCode(long double key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(long double));
}
