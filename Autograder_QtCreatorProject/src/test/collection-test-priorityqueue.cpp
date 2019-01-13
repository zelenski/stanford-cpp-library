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

/* Force instantiation of the template on a type to ensure that we don't have anything
 * invidious lurking that just didn't get compiled.
 */
template class PriorityQueue<int>;
template class PriorityQueue<std::string>;

TIMED_TEST(PriorityQueueTests, basicSortingTest_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
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
        assertEqualsDouble("basic sorting PriorityQueue", expPri, actPri);
        assertEqualsString("basic sorting PriorityQueue", exp, act);
    }
}

TIMED_TEST(PriorityQueueTests, changePriority_PriorityQueue, TEST_TIMEOUT_DEFAULT) {
    static const int kNumEntries = 137;

    /* Test changing each index individually. */
    for (int value = 0; value < kNumEntries; value++) {
        PriorityQueue<int> pq;

        /* Add in a bunch of integers, each of which has their own priority. */
        for (int i = 0; i < kNumEntries; i++) {
            pq.enqueue(i, i);
        }

        /* Change the priority of the indicated value to -1 to make it dequeue first. */
        pq.changePriority(value, -1);

        /* Confirm we get back what we expected - that value first, then everything else. */
        int found = pq.dequeue();
        assertEqualsInt("change priority PriorityQueue", found, value);

        for (int i = 0; i < value; i++) {
            int found = pq.dequeue();
            assertEqualsInt("change priority PriorityQueue", found, i);
        }

        for (int i = value + 1; i < kNumEntries; i++) {
            int found = pq.dequeue();
            assertEqualsInt("change priority PriorityQueue", found, i);
        }
    }

    /* Edge cases! */
    {
        PriorityQueue<int> pq;
        for (int i = 0; i < kNumEntries; i++) {
            pq.enqueue(i, i);
        }

        try {
            /* Change a nonexistent element. */
            pq.changePriority(kNumEntries, kNumEntries);
            assertFail("Changed priority of a nonexistent element?");
        } catch (const ErrorException& ) {
            // All is well, this is expected.
        }
    }

    {
        PriorityQueue<int> pq;
        for (int i = 0; i < kNumEntries; i++) {
            pq.enqueue(i, i);
        }

        try {
            /* Increase, rather than decrease, priority. */
            pq.changePriority(0, kNumEntries);
            assertFail("Should have caused error increasing priority.");
        } catch (const ErrorException& ) {
            // All is well, this is expected.
        }
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
