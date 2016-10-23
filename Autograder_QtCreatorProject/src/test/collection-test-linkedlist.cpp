/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedlist.h"
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

TEST_CATEGORY(LinkedListTests, "LinkedList tests");

TIMED_TEST(LinkedListTests, compareTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> ll1;
    ll1 += 1, 2, 4, 5;
    LinkedList<int> ll2;
    ll2 += 1, 3, 1, 4, 8;
    LinkedList<int> ll3;
    ll3 += 1, 1, 7;
    LinkedList<int> ll4;  // empty
    compareTestHelper(ll1, ll2, "LinkedList", /* compareTo */ -1);
    compareTestHelper(ll1, ll3, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll1, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll3, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll3, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll2, "LinkedList", /* compareTo */ 0);

    Set<LinkedList<int> > sll {ll1, ll2, ll3, ll4};
    assertEqualsString("sll", "{{}, {1, 1, 7}, {1, 2, 4, 5}, {1, 3, 1, 4, 8}}", sll.toString());
}

TIMED_TEST(LinkedListTests, forEachTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> v1 {1, 2, 3};
    assertCollection("foreach LinkedList", {1, 2, 3}, v1);

    LinkedList<std::string> v2 {"a", "b", "c"};
    std::initializer_list<std::string> list {"a", "b", "c"};
    assertCollection("foreach LinkedList", list, v2);
}

TIMED_TEST(LinkedListTests, hashCodeTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> llist;
    llist.add(69);
    llist.add(42);
    assertEqualsInt("hashcode of self LinkedList", hashCode(llist), hashCode(llist));

    LinkedList<int> copy = llist;
    assertEqualsInt("hashcode of copy LinkedList", hashCode(llist), hashCode(copy));

    LinkedList<int> empty;
    HashSet<LinkedList<int> > hashllist {llist, copy, empty, empty};
    assertEqualsInt("hashset of LinkedList size", 2, hashllist.size());
}

TIMED_TEST(LinkedListTests, initializerListTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};

    LinkedList<int> llist {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, llist);

    llist += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, llist);

    LinkedList<int> copy = llist + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, llist);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);
}

TIMED_TEST(LinkedListTests, randomElementTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    LinkedList<std::string> llist(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(llist);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(LinkedListTests, shuffleTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> v {10, 20, 30, 40, 50};
    Map<int, Map<int, int> > valueIndexCount;

    // shuffle 100 times
    for (int i = 0; i < 100; i++) {
        shuffle(v);
        for (int j = 0; j < v.size(); j++) {
            valueIndexCount[v[j]][j]++;
        }
    }

    // make sure each value appeared at each index at least once
    for (int n : v) {
        for (int j = 0; j < v.size(); j++) {
            assertNotEqualsInt("", 0, valueIndexCount[n][j]);
        }
    }
}
