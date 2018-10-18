/*
 * Functions used for searching and sorting.
 *
 * @version 2015/07/21
 */

#ifndef _searchingsorting_h
#define _searchingsorting_h

#include <iostream>
#include <string>
#include "vector.h"

using namespace std;

void fillRandomIntVector(Vector<int>& v, int length);
void fillSortedIntVector(Vector<int>& v, int length);

bool isSorted(const Vector<int>& v);
void selectionSort(Vector<int>& v, int passes = -1);
void mergeSort(Vector<int>& v, string indent = "");

int binarySearch(const Vector<int>& v, int target);
int sequentialSearch(const Vector<int>& v, int value);

#endif
