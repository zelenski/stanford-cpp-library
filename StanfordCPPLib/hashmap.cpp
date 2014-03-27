/*
 * File: hashmap.cpp
 * -----------------
 * This file contains the hash functions that are used in conjunction
 * with the HashMap class.
 */

#include <iostream>
#include <string>
#include "grid.h"
#include "hashmap.h"
#include "hashset.h"
#include "lexicon.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
using namespace std;

/*
 * Implementation notes: hashCode
 * ------------------------------
 * This function takes a string key and uses it to derive a hash code,
 * which is a nonnegative integer related to the key by a deterministic
 * function that distributes keys well across the space of integers.
 * The general method is called linear congruence, which is also used
 * in random-number generators.  The specific algorithm used here is
 * called djb2 after the initials of its inventor, Daniel J. Bernstein,
 * Professor of Mathematics at the University of Illinois at Chicago.
 */

const int HASH_SEED = 5381;               /* Starting point for first cycle */
const int HASH_MULTIPLIER = 33;           /* Multiplier for each cycle      */
const int HASH_MASK = unsigned(-1) >> 1;  /* All 1 bits except the sign     */

int hashCode(const string& str) {
    unsigned hash = HASH_SEED;
    int n = str.length();
    for (int i = 0; i < n; i++) {
        hash = HASH_MULTIPLIER * hash + str[i];
    }
    return int(hash & HASH_MASK);
}

int hashCode(int key) {
    return key & HASH_MASK;
}

int hashCode(char key) {
    return key;
}

int hashCode(long key) {
    return int(key) & HASH_MASK;
}

int hashCode(double key) {
    char* byte = (char*) &key;
    unsigned hash = HASH_SEED;
    for (int i = 0; i < (int) sizeof(double); i++) {
        hash = HASH_MULTIPLIER * hash + (int) *byte++;
    }
    return hash & HASH_MASK;
}

int hashCode(void* key) {
    return hashCode(reinterpret_cast<long>(key));
}


// hashCode functions for various collections;
// added by Marty Stepp to allow compound collections.
// I'm a bit ashamed to have to rewrite so many prototypes, one for each
// element type; but I can't get it to compile with a template.
int hashCode(const Grid<int>& g) {
    int code = HASH_SEED;
    __foreach__ (int n __in__ g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Grid<double>& g) {
    int code = HASH_SEED;
    __foreach__ (double n __in__ g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Grid<char>& g) {
    int code = HASH_SEED;
    __foreach__ (char n __in__ g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Grid<long>& g) {
    int code = HASH_SEED;
    __foreach__ (long n __in__ g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Grid<string>& g) {
    int code = HASH_SEED;
    __foreach__ (string n __in__ g) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const HashSet<int>& s) {
    int code = HASH_SEED;
    __foreach__ (int n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const HashSet<double>& s) {
    int code = HASH_SEED;
    __foreach__ (double n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const HashSet<char>& s) {
    int code = HASH_SEED;
    __foreach__ (char n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const HashSet<long>& s) {
    int code = HASH_SEED;
    __foreach__ (long n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const HashSet<std::string>& s) {
    int code = HASH_SEED;
    __foreach__ (std::string n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Lexicon& l) {
    int code = HASH_SEED;
    __foreach__ (std::string n __in__ l) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Queue<int>& q) {
    int code = HASH_SEED;
    Queue<int> backup = q;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.dequeue());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Queue<double>& q) {
    int code = HASH_SEED;
    Queue<double> backup = q;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.dequeue());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Queue<char>& q) {
    int code = HASH_SEED;
    Queue<char> backup = q;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.dequeue());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Queue<long>& q) {
    int code = HASH_SEED;
    Queue<long> backup = q;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.dequeue());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Queue<std::string>& q) {
    int code = HASH_SEED;
    Queue<std::string> backup = q;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.dequeue());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Set<int>& s) {
    int code = HASH_SEED;
    __foreach__ (int n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Set<double>& s) {
    int code = HASH_SEED;
    __foreach__ (double n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Set<char>& s) {
    int code = HASH_SEED;
    __foreach__ (char n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Set<long>& s) {
    int code = HASH_SEED;
    __foreach__ (long n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Set<std::string>& s) {
    int code = HASH_SEED;
    __foreach__ (std::string n __in__ s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Stack<int>& s) {
    int code = HASH_SEED;
    Stack<int> backup = s;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.pop());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Stack<double>& s) {
    int code = HASH_SEED;
    Stack<double> backup = s;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.pop());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Stack<char>& s) {
    int code = HASH_SEED;
    Stack<char> backup = s;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.pop());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Stack<long>& s) {
    int code = HASH_SEED;
    Stack<long> backup = s;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.pop());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Stack<std::string>& s) {
    int code = HASH_SEED;
    Stack<std::string> backup = s;
    while (!backup.isEmpty()) {
        code = HASH_MULTIPLIER * code + hashCode(backup.pop());
    }
    return int(code & HASH_MASK);
}

int hashCode(const Vector<int>& v) {
    int code = HASH_SEED;
    for (int i = 0, size = v.size(); i < size; i++) {
        code = HASH_MULTIPLIER * code + hashCode(v[i]);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Vector<double>& v) {
    int code = HASH_SEED;
    for (int i = 0, size = v.size(); i < size; i++) {
        code = HASH_MULTIPLIER * code + hashCode(v[i]);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Vector<char>& v) {
    int code = HASH_SEED;
    for (int i = 0, size = v.size(); i < size; i++) {
        code = HASH_MULTIPLIER * code + hashCode(v[i]);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Vector<long>& v) {
    int code = HASH_SEED;
    for (int i = 0, size = v.size(); i < size; i++) {
        code = HASH_MULTIPLIER * code + hashCode(v[i]);
    }
    return int(code & HASH_MASK);
}

int hashCode(const Vector<std::string>& v) {
    int code = HASH_SEED;
    for (int i = 0, size = v.size(); i < size; i++) {
        code = HASH_MULTIPLIER * code + hashCode(v[i]);
    }
    return int(code & HASH_MASK);
}

//template <typename ValueType>
//int hashCode(const Vector<ValueType>& v) {
//    int code = 0;
//    for (int i = 0, size = v.size(); i < size; i++) {
//        code = 31 * code + hashCode(v[i]);
//    }
//    return code;
//}
