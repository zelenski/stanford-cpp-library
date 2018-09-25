/*
 * CS 106B, Marty Stepp
 * This program contains code for the recursive binary search algorithm.
 * Included in comments below are some runtime measurements for a standard
 * sequential search (O(N)) and the recursive binary search (O(log N)).
 * Notice how the binary search can perform many more searches over a larger
 * data set in much less time than a sequential search.
 *
 * This program uses a small instructor-provided helper class called Timer
 * to make it easier to measure the runtime of each algorithm.
 *
 * @version 2015/07/21
 */

#define INTERNAL_INCLUDE 1
#include "searchingsorting.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "filelib.h"
#include "random.h"
#include "vector.h"
#include "timer.h"
#undef INTERNAL_INCLUDE

/*
 * This recursive helper function implements the bulk of the
 * binary search behavior.
 */
int binarySearchHelper(const Vector<int>& v, int target,
                       int min, int max) {
    int mid = (max + min) / 2;
    if (min > max) {
        // base case 1: exhausted all data without finding the target value
        return -1;
    }
    
    cout << "  examine index " << mid << endl;
    if (target == v[mid]) {
        // base case 2: found the target value in the middle of our current range
        return mid;
    } else if (target > v[mid]) {
        // recursive case 1: target value is larger, so go right
        return binarySearchHelper(v, target, mid + 1, max);
    } else {
        // recursive case 2: target value is smaller, so go left
        return binarySearchHelper(v, target, min, mid - 1);
    }
}

/*
 * Performs a binary search for the given target value in the given vector.
 * Precondition: The vector's elements are assumed to be in sorted order.
 * If they are not sorted, the behavior is undefined and likely incorrect.
 */
int binarySearch(const Vector<int>& v, int target) {
    int result = binarySearchHelper(v, target, 0, v.size() - 1);
    cout << "  return " << result << endl;
    return result;
}

// Returns the index of the given value in the given vector of integers,
// or -1 if the value is not found in the vector.
int sequentialSearch(const Vector<int>& v, int value) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == value) {
            return i;
        }
    }
    return -1;
}
