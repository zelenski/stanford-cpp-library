/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "deque.h"
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

TEST_CATEGORY(DequeTests, "Deque tests");

TIMED_TEST(DequeTests, compareTest_Deque, TEST_TIMEOUT_DEFAULT) {
    Deque<std::string> deq;
    deq.add("a");
    deq.add("b");
    deq.add("c");
    Deque<std::string> deq2;
    deq2.add("a");
    deq2.add("ab");
    deq2.add("bc");
    Deque<std::string> deq3;
    compareTestHelper(deq, deq2, "Deque", /* compareTo */ 1);
    compareTestHelper(deq2, deq, "Deque", /* compareTo */ -1);
    compareTestHelper(deq, deq, "Deque", /* compareTo */ 0);
    Set<Deque<std::string> > sdeque {deq, deq2, deq3};
    assertEqualsString("sdeque", "{{}, {\"a\", \"ab\", \"bc\"}, {\"a\", \"b\", \"c\"}}", sdeque.toString());
}

TIMED_TEST(DequeTests, forEachTest_Deque, TEST_TIMEOUT_DEFAULT) {
    Deque<int> deq;
    deq.addBack(1);
    deq.addFront(2);
    deq.addBack(3);
    deq.addFront(4);

    assertCollection("Deque foreach", {4, 2, 1, 3}, deq);
}

TIMED_TEST(DequeTests, hashCodeTest_Deque, TEST_TIMEOUT_DEFAULT) {
    Deque<int> deq;
    deq.addBack(1);
    deq.addFront(2);
    deq.addBack(3);
    deq.addFront(4);
    assertEqualsInt("hashcode of self Deque", hashCode(deq), hashCode(deq));

    Deque<int> copy = deq;
    assertEqualsInt("hashcode of copy Deque", hashCode(deq), hashCode(copy));

    Deque<int> empty;

    HashSet<Deque<int> > hashdeq {deq, copy, empty, empty};
    assertEqualsInt("hashset of Deque size", 2, hashdeq.size());
}

TIMED_TEST(DequeTests, initializerListTest_Deque, TEST_TIMEOUT_DEFAULT) {
    Deque<int> deque {10, 20, 30};
    assertEqualsString("deque initializer list", "{10, 20, 30}", deque.toString());
}
