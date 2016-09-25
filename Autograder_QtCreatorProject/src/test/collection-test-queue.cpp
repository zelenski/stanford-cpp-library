/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "queue.h"
#include "hashcode.h"
#include "hashset.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(QueueTests, "Queue tests");

TIMED_TEST(QueueTests, compareTest_Queue, TEST_TIMEOUT_DEFAULT) {
    Queue<int> q1;
    q1.add(1);
    q1.add(2);
    q1.add(1);
    q1.add(5);
    Queue<int> q2;
    q2.add(1);
    q2.add(2);
    q2.add(1);
    q2.add(4);
    q2.add(7);
    compareTestHelper(q1, q2, "Queue", /* compareTo */  1);
    compareTestHelper(q2, q1, "Queue", /* compareTo */ -1);
    compareTestHelper(q1, q1, "Queue", /* compareTo */  0);

    Set<Queue<int> > sq {q1, q2};
    assertEqualsString("sq", "{{1, 2, 1, 4, 7}, {1, 2, 1, 5}}", sq.toString());
}

TIMED_TEST(QueueTests, forEachTest_Queue, TEST_TIMEOUT_DEFAULT) {
    Queue<std::string> q;
    q.add("a");
    q.add("bb");
    q.add("c");
    q.add("ddd");
    q.add("e");
    q.add("ffff");
}

TIMED_TEST(QueueTests, hashCodeTest_Queue, TEST_TIMEOUT_DEFAULT) {
    HashSet<Queue<int> > hashq;
    Queue<int> q;
    q.add(69);
    q.add(42);
    hashq.add(q);
    std::cout << "hashset of queue: " << hashq << std::endl;
}

TIMED_TEST(QueueTests, initializerListTest_Queue, TEST_TIMEOUT_DEFAULT) {
    Queue<int> queue {10, 20, 30};
    std::cout << "init list Queue = " << queue << std::endl;
}
