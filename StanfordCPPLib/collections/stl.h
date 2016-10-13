/*
 * File: stl.h
 * -----------
 * Contains utility functions related to the STL collections.
 *
 * @author Marty Stepp
 * @version 2016/09/22
 * @since 2016/09/22
 */

#ifndef _stl_h
#define _stl_h

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>
#include "collections.h"

// STL containers:
// array, vector, deque, forward_list, ist, stack, queue, priority_queue,
// set, multiset, map, multimap, unordered_*, bitset, valarray

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::initializer_list<T>& list) {
    return stanfordcpplib::collections::writeCollection(out, list);
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& vec) {
    return stanfordcpplib::collections::writeCollection(out, vec);
}

#endif // _stl_h
