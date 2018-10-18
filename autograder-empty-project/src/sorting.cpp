/*
 * CS 106B, Marty Stepp
 * This program contains code for recursive merge sort.
 *
 * @version 2015/07/21
 */

#define INTERNAL_INCLUDE 1
#include <fstream>
#include <iomanip>
#include <iostream>
#include "filelib.h"
#include "random.h"
#include "searchingsorting.h"
#include "vector.h"
#include "timer.h"
#undef INTERNAL_INCLUDE

void mergeSort(Vector<int>& v, string indent) {
    cout << indent << v << endl;
    if (v.size() < 2) {
        return;   // base case; do nothing
    }
    
    // else, recursive case:
    // divide v in halves
    Vector<int> left;
    for (int i = 0; i < v.size() / 2; i++) {
        left.add(v[i]);
    }
    Vector<int> right;
    for (int i = v.size() / 2; i < v.size(); i++) {
        right.add(v[i]);
    }
    
    // sort the halves
    mergeSort(left, indent + "    ");
    mergeSort(right, indent + "    ");
    
    // merge the sorted halves together
    v.clear();
    int i1 = 0;
    int i2 = 0;
    for (int i = 0; i < left.size() + right.size(); i++) {
        // pick smaller one, move into result
        if (i2 >= right.size() || 
                (i1 < left.size() && left[i1] < right[i2])) {
            v.add(left[i1]);
            i1++;
        } else {
            v.add(right[i2]);
            i2++;
        }
    }
}


// Rearranges the elements of v into sorted order using
// the selection sort algorithm.
void selectionSort(Vector<int>& v, int passes) {
    for (int i = 0; i < v.size() - 1; i++) {
        // find index of smallest remaining value
        int min = i;
        for (int j = i + 1; j < v.size(); j++) {
            if (v[j] < v[min]) {
                min = j;
            }
        }
        // swap smallest value to its proper place, a[i]
        int temp = v[i];
        v[i] = v[min];
        v[min] = temp;
        
        cout << "after pass " << (i+1) << ": " << v << endl;
        if (passes > 0 && (i+1) >= passes) {
            break;
        }
    }
}

bool isSorted(const Vector<int>& v) {
    for (int i = 1; i < v.size(); i++) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

// Fills the given vector with the given number of random integers.
void fillRandomIntVector(Vector<int>& v, int length) {
    for (int i = 0; i < length; i++) {
        v += randomInteger(0, 1000000000);
    }
}

// Fills the given vector with the given number of sorted integers.
void fillSortedIntVector(Vector<int>& v, int length) {
    v.add(randomInteger(0, 10));
    for (int i = 1; i < length; i++) {
        // add another value slightly larger than the last
        v += v[i-1] + randomInteger(0, 10);
    }
}
