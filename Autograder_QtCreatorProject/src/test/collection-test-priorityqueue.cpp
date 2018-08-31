/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "priorityqueue.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "collection-test-common.h"
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
    Queue<std::string> expected {"c", "ffff", "bb", "a", "e", "ddd"};
    Queue<double> expectedPriority {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    while (!pq.isEmpty()) {
        double expPri = expectedPriority.dequeue();
        std::string exp = expected.dequeue();
        double actPri = pq.peekPriority();
        std::string act = pq.dequeue();
        assertEqualsDouble("foreach PriorityQueue", expPri, actPri);
        assertEqualsString("foreach PriorityQueue", exp, act);
    }
}

TIMED_TEST(PriorityQueueTests, hashCodeTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    PriorityQueue<std::string> pq;
    pq.add("a", 4);
    pq.add("bb", 3);
    pq.add("c", 1);
    pq.add("ddd", 6);
    pq.add("e", 5);
    pq.add("ffff", 2);
    assertEqualsInt("hashcode of self PriorityQueue", hashCode(pq), hashCode(pq));

    PriorityQueue<std::string> copy = pq;
    assertEqualsInt("hashcode of copy PriorityQueue", hashCode(pq), hashCode(copy));

    PriorityQueue<std::string> empty;

    HashSet<PriorityQueue<std::string> > hashpq {pq, copy, empty, empty};
    assertEqualsInt("hashset of PriorityQueue size", 2, hashpq.size());
}

TIMED_TEST(PriorityQueueTests, initializerListTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    PriorityQueue<std::string> pq {{40.0, "Marty"}, {20.0, "Eric"}, {30.0, "Mehran"}};
    Queue<std::string> expected {"Eric", "Mehran", "Marty"};
    Queue<double> expectedPriority {20.0, 30.0, 40.0};
    while (!pq.isEmpty()) {
        double expPri = expectedPriority.dequeue();
        std::string exp = expected.dequeue();
        double actPri = pq.peekPriority();
        std::string act = pq.dequeue();
        assertEqualsDouble("initializer list PriorityQueue", expPri, actPri);
        assertEqualsString("initializer list PriorityQueue", exp, act);
    }
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(PriorityQueueTests, iteratorVersionTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    PriorityQueue<std::string> pq {{40.0, "Marty"}, {20.0, "Eric"}, {30.0, "Mehran"}, {50.0, "Bill"}, {10.0, "John"}};
    try {
        for (std::string s : pq) {
            if (s.length() % 2 == 0) {
                pq.dequeue();
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR
