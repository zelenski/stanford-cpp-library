/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "priorityqueue.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(PriorityQueueTests, "PriorityQueue tests");

TIMED_TEST(PriorityQueueTests, forEachTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    PriorityQueue<std::string> pq;
    pq.add("a", 4);
    pq.add("bb", 3);
    pq.add("c", 1);
    pq.add("ddd", 6);
    pq.add("e", 5);
    pq.add("ffff", 2);
}

TIMED_TEST(PriorityQueueTests, hashCodeTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    PriorityQueue<std::string> pq;
    pq.add("a", 4);
    pq.add("bb", 3);
    pq.add("c", 1);
    pq.add("ddd", 6);
    pq.add("e", 5);
    pq.add("ffff", 2);
    PriorityQueue<std::string> pq2;
    HashSet<PriorityQueue<std::string> > hashpq;
    hashpq.add(pq);
    hashpq.add(pq2);
}

TIMED_TEST(PriorityQueueTests, initializerListTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};
    std::initializer_list<std::string> lexlist2 = {"twenty", "fifty"};
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    PriorityQueue<std::string> pqueue {{40.0, "Marty"}, {20.0, "Eric"}, {30.0, "Mehran"}};
    std::cout << "init list PQueue = " << pqueue << std::endl;
    while (!pqueue.isEmpty()) {
        std::cout << "  " << pqueue.peekPriority() << " " << pqueue.peek() << std::endl;
        pqueue.dequeue();
    }
}
