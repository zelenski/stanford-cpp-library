/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "priorityqueue.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>



/*
 * Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class PriorityQueue<int>;
template class PriorityQueue<std::string>;

PROVIDED_TEST("PQueue, basicSorting") {
    PriorityQueue<std::string> pq;
    pq.enqueue("a", 4);
    pq.enqueue("bb", 3);
    pq.enqueue("c", 1);
    pq.enqueue("ddd", 6);
    pq.enqueue("e", 5);
    pq.enqueue("ffff", 2);
    Queue<std::string> expected {"c", "ffff", "bb", "a", "e", "ddd"};
    Queue<double> expectedPriority {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    while (!pq.isEmpty()) {
        double expPri = expectedPriority.dequeue();
        std::string exp = expected.dequeue();
        double actPri = pq.peekPriority();
        std::string act = pq.dequeue();
        EXPECT_EQUAL(expPri, actPri);
        EXPECT_EQUAL( exp, act);
    }
}

PROVIDED_TEST("PQueue, changePriority") {
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
        EXPECT_EQUAL( found, value);

        for (int i = 0; i < value; i++) {
            int found = pq.dequeue();
            EXPECT_EQUAL( found, i);
        }

        for (int i = value + 1; i < kNumEntries; i++) {
            int found = pq.dequeue();
            EXPECT_EQUAL( found, i);
        }
    }
}

PROVIDED_TEST("PQueue, error if misuse changePriority") {
    static const int kNumEntries = 11;

    /* Edge cases! */
    {
        PriorityQueue<int> pq;
        for (int i = 0; i < kNumEntries; i++) {
            pq.enqueue(i, i);
        }
        EXPECT_ERROR( pq.changePriority(kNumEntries, kNumEntries));
    }

    {
        PriorityQueue<int> pq;
        for (int i = 0; i < kNumEntries; i++) {
            pq.enqueue(i, i);
        }

        /* Increase, rather than decrease, priority. */
        EXPECT_ERROR( pq.changePriority(0, kNumEntries));

    }
}

PROVIDED_TEST("PQueue, hashCode") {
    PriorityQueue<std::string> pq;
    pq.enqueue("a", 4);
    pq.enqueue("bb", 3);
    pq.enqueue("c", 1);
    pq.enqueue("ddd", 6);
    pq.enqueue("e", 5);
    pq.enqueue("ffff", 2);
    EXPECT_EQUAL( hashCode(pq), hashCode(pq));

    PriorityQueue<std::string> copy = pq;
    EXPECT_EQUAL( hashCode(pq), hashCode(copy));

    PriorityQueue<std::string> empty;

    HashSet<PriorityQueue<std::string> > hashpq {pq, copy, empty, empty};
    EXPECT_EQUAL( 2, hashpq.size());
}

PROVIDED_TEST("PQueue, initializerList") {
    PriorityQueue<std::string> pq {{40.0, "Marty"}, {20.0, "Eric"}, {30.0, "Mehran"}};
    Queue<std::string> expected {"Eric", "Mehran", "Marty"};
    Queue<double> expectedPriority {20.0, 30.0, 40.0};
    while (!pq.isEmpty()) {
        double expPri = expectedPriority.dequeue();
        std::string exp = expected.dequeue();
        double actPri = pq.peekPriority();
        std::string act = pq.dequeue();
        EXPECT_EQUAL( expPri, actPri);
        EXPECT_EQUAL( exp, act);
    }
}
