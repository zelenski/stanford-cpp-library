/*
 * CS 106B, Marty Stepp
 * ArrayIntList is our example of implementing a basic data structure.
 * The list can store only ints for now.
 * See ArrayIntList.cpp for documentation of each member.
 *
 * @version 2016/08/23
 * - added initializer_list support to match other lib collections
 * @version 2016/07/10
 * - added removeDuplicates() declaration
 */

#ifndef _arrayintlist_h
#define _arrayintlist_h

#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include "set.h"
using namespace std;

class ArrayIntList {
public:
    // constructor
    ArrayIntList();
    ArrayIntList(int capacity);
    ArrayIntList(std::initializer_list<int> list);
    ~ArrayIntList();
    
    // member functions (methods)
    void add(int value);
    void clear();
    void ensureCapacity(int capacity);
    int get(int index) const;
    void insert(int index, int value);
    bool isEmpty() const;
    void remove(int index);
    void set(int index, int value);
    int size() const;
    void debug() const;
    
    // section problems
    int maxCount() const;
    
    // exam problems
    void stretch(int k);
    void mirror();
    int longestSortedSequence() const;
    bool filter(const Set<int>& set);
    void removeDuplicates();

    ArrayIntList& operator =(const ArrayIntList& src);

private:
    // member variables (fields)
    int* elements;   // array storing element data
    int mysize;      // number of elements in the array
    int capacity;    // array's length
    
    void checkIndex(int index, int min, int max) const;
};

// overloaded operators
ostream& operator <<(ostream& out, const ArrayIntList& list);
istream& operator >>(istream& in, ArrayIntList& list);

#endif







