/*
 * CS 106B, Marty Stepp
 * This file implements the HeapPriorityQueue class.
 *
 * @version 2015/07/29
 */

#include "HeapPriorityQueue.h"
#include <string>
#include "error.h"
#include "strlib.h"
#include "vector.h"

using namespace std;

HeapPriorityQueue::HeapPriorityQueue(PQEntry* elements, int capacity, int mysize) {
    this->elements = elements;
    this->capacity = capacity;
    this->mysize = mysize;
    this->hackPQ = new PriorityQueue<string>();
}

HeapPriorityQueue::~HeapPriorityQueue() {
    if (elements) {
        delete[] elements;
    }
    delete hackPQ;
}

void HeapPriorityQueue::changePriority(string value, double newPriority) {
    hackPQ->changePriority(value, newPriority);
}

void HeapPriorityQueue::clear() {
    hackPQ->clear();
    mysize = hackPQ->size();
}

string HeapPriorityQueue::dequeue() {
    string result = hackPQ->dequeue();
    mysize = hackPQ->size();
    return result;
}

void HeapPriorityQueue::enqueue(string value, double priority) {
    hackPQ->enqueue(value, priority);
    mysize = hackPQ->size();
}

bool HeapPriorityQueue::isEmpty() const {
    return hackPQ->isEmpty();
}

string HeapPriorityQueue::peek() const {
    return hackPQ->peek();
}

double HeapPriorityQueue::peekPriority() const {
    return hackPQ->peekPriority();
}

void HeapPriorityQueue::printSideways(int index, string indent) const {
    if (index <= size()) {
        printSideways(index*2+1, indent + "    ");   // right
        string value    = hackPQ->__getValueFromHeap(index-1);
        double priority = hackPQ->__getPriorityFromHeap(index-1);
        cout << indent << value << ":" << priority << endl;
        printSideways(index*2, indent + "    ");     // left
    }
}

int HeapPriorityQueue::size() const {
    return mysize;
}

string HeapPriorityQueue::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

ostream& operator <<(ostream& out, const HeapPriorityQueue& pq) {
    out << *pq.hackPQ;
    return out;
}

istream& operator >>(istream& input, HeapPriorityQueue& pq) {
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
        error("HeapPriorityQueue::operator >>: Missing {");
    }
    pq.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            std::string value;
            readGenericValue(input, value);
            input >> ch;
            if (ch != ':') {
                error("HeapPriorityQueue::operator >>: Missing colon after priority");
            }
            double priority = 0.0;
            input >> priority;
            pq.enqueue(value, priority);
            
            input >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("HeapPriorityQueue::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return input;
}

void stringToPQ(HeapPriorityQueue& pq, string elements) {
    Vector<string> pairs = stringSplit(elements, ", ");
    for (string pair : pairs) {
        Vector<string> parts = stringSplit(pair, ":");
        string value = parts[0];
        int priority = stringToInteger(parts[1]);
        pq.enqueue(value, priority);
    }
}
