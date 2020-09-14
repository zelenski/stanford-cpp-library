/*
 * File: stl.h
 * -----------
 * Contains utility functions related to the STL collections.
 *
 * @author Marty Stepp
 * @version 2018/10/19
 * - added toStl* methods (moved here from being members of our collections)
 *   This is to avoid a default include of STL containers in student code.
 * @version 2016/10/22
 * - added toString
 * @version 2016/09/22
 * - initial version
 * @since 2016/09/22
 */


#ifndef _stl_h
#define _stl_h

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

#include "collections.h"
#include "lexicon.h"
#include "linkedlist.h"
#include "map.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"

// STL containers:
// array, vector, deque, forward_list, ist, stack, queue, priority_queue,
// set, multiset, map, multimap, unordered_*, bitset, valarray

/**
 * Writes the given list of values to the given output stream
 * in a "{10, 20, 30}" format.
 */
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::initializer_list<T>& list) {
    return stanfordcpplib::collections::writeCollection(out, list);
}

/**
 * Writes the given vector of values to the given output stream
 * in a "{10, 20, 30}" format.
 */
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& vec) {
    return stanfordcpplib::collections::writeCollection(out, vec);
}

/**
 * Returns an STL queue object with the same elements as this Queue.
 */
template <typename ValueType>
std::deque<ValueType> toStlDeque(const Queue<ValueType>& q) {
    Queue<ValueType> copy = q;
    std::deque<ValueType> result;
    while (!copy.isEmpty()) {
        result.push_back(copy.dequeue());
    }
    return result;
}

/**
 * Returns an STL list object with the same elements as this LinkedList.
 */
template <typename ValueType>
std::list<ValueType> toStlList(const LinkedList<ValueType>& l) {
    std::list<ValueType> result;
    for (const ValueType& value : l) {
        result.push_back(value);
    }
    return result;
}

/**
 * Returns an STL map object with the same elements as this map.
 */
template <typename KeyType, typename ValueType>
std::map<KeyType, ValueType> toStlMap(const Map<KeyType, ValueType>& m) {
    std::map<KeyType, ValueType> result;
    for (const KeyType& key : m) {
        result[key] = m[key];
    }
    return result;
}

/**
 * Returns an STL queue object with the same elements as this Queue.
 */
template <typename ValueType>
std::queue<ValueType> toStlQueue(const Queue<ValueType>& q) {
    Queue<ValueType> copy = q;
    std::queue<ValueType> result;
    while (!copy.isEmpty()) {
        result.push_back(copy.dequeue());
    }
    return result;
}

/**
 * Returns an STL set object with the same elements as this Lexicon.
 */
std::set<std::string> toStlSet(const Lexicon& lex);

/**
 * Returns an STL set object with the same elements as this Set.
 */
template <typename ValueType>
std::set<ValueType> toStlSet(const Set<ValueType>& s) {
    std::set<ValueType> result;
    for (ValueType& value : s) {
        result.insert(value);
    }
    return result;
}

/**
 * Returns an STL stack object with the same elements as this Stack.
 */
template <typename ValueType>
std::stack<ValueType> toStlStack(const Stack<ValueType>& s) {
    Stack<ValueType> copy = s;
    std::stack<ValueType> result;
    while (!copy.isEmpty()) {
        result.push(copy.pop());
    }
    return result;
}

/**
 * Returns an STL vector object with the same elements as this Vector.
 */
template <typename ValueType>
std::vector<ValueType> toStlVector(const Vector<ValueType>& v) {
    std::vector<ValueType> result;
    for (int i = 0; i < v.size(); i++) {
        result.push_back(v[i]);
    }
    return result;
}

/**
 * Converts the given list of values to a string such as "{1, 2, 3}".
 */
template <typename T>
std::string toString(const std::initializer_list<T>& list) {
    std::ostringstream out;
    out << list;
    return out.str();
}

/**
 * Converts the given vector of values to a string such as "{1, 2, 3}".
 */
template <typename T>
std::string toString(const std::vector<T>& vec) {
    std::ostringstream out;
    out << vec;
    return out.str();
}

#endif // _stl_h
