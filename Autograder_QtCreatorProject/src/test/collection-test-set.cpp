/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "set.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(SetTests, "Set tests");

TIMED_TEST(SetTests, randomElementTest_Set, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;
    
    std::cout << "Set: ";
    Set<std::string> set;
    set += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(set);
        std::cout << s << " ";
        counts[s]++;
    }
}

TIMED_TEST(SetTests, compareTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set1;
    set1 += 7, 5, 1, 2, 8;
    Set<int> set2;
    set2 += 1, 2, 3, 4;
    Set<int> set3;
    compareTestHelper(set1, set2, "Set", /* compareTo */  1);
    compareTestHelper(set2, set1, "Set", /* compareTo */ -1);
    compareTestHelper(set1, set3, "Set", /* compareTo */  1);
    compareTestHelper(set2, set3, "Set", /* compareTo */  1);

    Set<Set<int> > sset {set1, set2, set3};
    assertEqualsString("sset", "{{}, {1, 2, 3, 4}, {1, 2, 5, 7, 8}}", sset.toString());
}

TIMED_TEST(SetTests, forEachTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set {10, 20, 30, 40};
    Queue<int> expected {10, 20, 30, 40};

    for (int n : set) {
        int exp = expected.dequeue();
        assertEqualsInt("set foreach", exp, n);
    }
}

TIMED_TEST(SetTests, hashCodeTest_Set, TEST_TIMEOUT_DEFAULT) {
    HashSet<Set<int> > hashset;
    Set<int> set;
    set.add(69);
    set.add(42);
    hashset.add(set);
    std::cout << "hashset of set: " << hashset << std::endl;
}

TIMED_TEST(SetTests, initializerListTest_Set, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    auto list2 = {20, 50};

    Set<int> set {10, 20, 30};
    std::cout << "init list Set = " << set << std::endl;
    set += {40, 50};
    std::cout << "after +=, Set = " << set << std::endl;
    std::cout << "Set + {} list = " << (set + list) << std::endl;
    std::cout << "Set - {} list = " << (set - list2) << std::endl;
    std::cout << "Set * {} list = " << (set * list2) << std::endl;
    set -= {20, 50};
    std::cout << "Set -={} list = " << set << std::endl;
    set *= {0, 10, 40, 99};
    std::cout << "Set *={} list = " << set << std::endl;
    std::cout << "at end,   Set = " << set << std::endl;
}
