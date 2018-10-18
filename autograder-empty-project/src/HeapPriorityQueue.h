/*
 * CS 106B, Marty Stepp
 * This file declares the HeapPriorityQueue class.
 *
 * @version 2015/07/29
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <fstream>
#include <iostream>
#include <string>
#include "pqueue.h"

using namespace std;

struct PQEntry {
    string data;
    double priority;
};

class HeapPriorityQueue {
public:
    HeapPriorityQueue(PQEntry* elements = nullptr, int capacity = 10, int mysize = 0);
    ~HeapPriorityQueue();
    void changePriority(string value, double newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, double priority);
    bool isEmpty() const;
    string peek() const;
    double peekPriority() const;
    void printSideways(int index = 1, string indent = "") const;
    int size() const;
    string toString() const;
    
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& pq);
    friend istream& operator >>(istream& input, HeapPriorityQueue& pq);
    
    // exam problems
    int atLevel(int level) const;

private:
    PQEntry* elements;   // array of element data
    int capacity;        // length of elements array
    int mysize;          // number of elements that have been added
    
    PriorityQueue<string>* hackPQ;
};

// "a:4, b:16, c:7" => PQ
void stringToPQ(HeapPriorityQueue& pq, string elements);

#endif
