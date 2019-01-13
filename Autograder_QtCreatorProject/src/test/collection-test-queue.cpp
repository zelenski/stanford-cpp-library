/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "queue.h"
#include "hashcode.h"
#include "hashset.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(QueueTests, "Queue tests");

/* Force instantiation of the template on a type to ensure that we don't have anything
 * invidious lurking that just didn't get compiled.
 */
template class Queue<int>;
template class Queue<std::string>;

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

    Queue<std::string> expected {"a", "bb", "c", "ddd", "e", "ffff"};
    while (!q.isEmpty()) {
        assertEqualsString("foreach Queue", expected.dequeue(), q.dequeue());
    }
}

TIMED_TEST(QueueTests, hashCodeTest_Queue, TEST_TIMEOUT_DEFAULT) {
    Queue<std::string> queue;
    queue.add("a");
    queue.add("bb");
    queue.add("c");
    queue.add("ddd");
    queue.add("e");
    queue.add("ffff");
    assertEqualsInt("hashcode of self Queue", hashCode(queue), hashCode(queue));

    Queue<std::string> copy = queue;
    assertEqualsInt("hashcode of copy Queue", hashCode(queue), hashCode(copy));

    Queue<std::string> empty;

    HashSet<Queue<std::string> > hashq {queue, copy, empty, empty};
    assertEqualsInt("hashset of Queue size", 2, hashq.size());
}

TIMED_TEST(QueueTests, initializerListTest_Queue, TEST_TIMEOUT_DEFAULT) {
    Queue<int> queue {10, 20, 30};
    assertEqualsString("initializer list Queue", "{10, 20, 30}", queue.toString());
}

TIMED_TEST(QueueTests, peekEnqueueBugTest_Queue, TEST_TIMEOUT_DEFAULT) {
    {
        Queue<int> queue {10, 20, 30, 40, 50, 60, 70, 80};
        assertEqualsString("Queue 1 initial", "{10, 20, 30, 40, 50, 60, 70, 80}", queue.toString());
        queue.enqueue(queue.peek());
        assertEqualsString("Queue 1 after enqueue", "{10, 20, 30, 40, 50, 60, 70, 80, 10}", queue.toString());
    }
    {
        Queue<int> queue {10, 20, 30, 40, 50, 60, 70, 80, 90};
        assertEqualsString("Queue 2 initial", "{10, 20, 30, 40, 50, 60, 70, 80, 90}", queue.toString());
        queue.enqueue(queue.peek());
        assertEqualsString("Queue 2 after enqueue", "{10, 20, 30, 40, 50, 60, 70, 80, 90, 10}", queue.toString());
    }
}
