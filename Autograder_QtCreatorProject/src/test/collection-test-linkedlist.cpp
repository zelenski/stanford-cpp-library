/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedlist.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
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
    LinkedList<int> ll;
    ll += 10, 20, 30, 40;
    std::cout << "linkedlist: " << ll << std::endl;
    for (int n : ll) {
        std::cout << n << std::endl;
    }
}

TIMED_TEST(LinkedListTests, hashCodeTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    HashSet<LinkedList<int> > hashlink;
    LinkedList<int> llist;
    llist.add(69);
    llist.add(42);
    hashlink.add(llist);
}

TIMED_TEST(LinkedListTests, initializerListTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};
    std::initializer_list<std::string> lexlist2 = {"twenty", "fifty"};
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    LinkedList<int> llist {10, 20, 30};
    std::cout << "init list LinkedList = " << llist << std::endl;
    llist += {40, 50};
    std::cout << "after +=, LinkedList = " << llist << std::endl;
    std::cout << "LinkedList + {} list = " << (llist + list) << std::endl;
    std::cout << "at end,   LinkedList = " << llist << std::endl;
}

TIMED_TEST(LinkedListTests, randomElementTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    LinkedList<std::string> list;
    list += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(list);
        std::cout << s << " ";
        counts[s]++;
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
