/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedhashset.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(LinkedHashSetTests, "LinkedHashSet tests");

TIMED_TEST(LinkedHashSetTests, compareTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> set1;
    set1 += 7, 5, 1, 2, 8;
    LinkedHashSet<int> set2;
    set2 += 1, 2, 3, 4;
    LinkedHashSet<int> set3;
    set3 += 2, 1, 3, 4;
    LinkedHashSet<int> set4;
    compareTestHelper(set1, set2, "Set", /* compareTo */  1);
    compareTestHelper(set2, set1, "Set", /* compareTo */ -1);
    compareTestHelper(set1, set3, "Set", /* compareTo */  1);
    compareTestHelper(set2, set3, "Set", /* compareTo */ -1);
    compareTestHelper(set3, set2, "Set", /* compareTo */  1);

    Set<LinkedHashSet<int> > slhset {set1, set2, set3, set4};
    assertEqualsString("slhset", "{{}, {1, 2, 3, 4}, {2, 1, 3, 4}, {7, 5, 1, 2, 8}}", slhset.toString());
}

TIMED_TEST(LinkedHashSetTests, forEachTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> lhset {30, 10, 40, 66, -1, 42, 99};
    Queue<int> expected {30, 10, 40, 66, -1, 42, 99};
    for (int n : lhset) {
        int exp = expected.dequeue();
        assertEqualsInt("LinkedHashSet foreach", exp, n);
    }
}

TIMED_TEST(LinkedHashSetTests, hashcodeTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<LinkedHashSet<int> > hashhashset;
    LinkedHashSet<int> hset;
    hset.add(69);
    hset.add(42);
    hashhashset.add(hset);
}

TIMED_TEST(LinkedHashSetTests, initializerListTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    auto list2 = {20, 50};

    LinkedHashSet<int> lhset {10, 20, 30};
    std::cout << "init list LinkedHashSet = " << lhset << std::endl;
    lhset += {40, 50};
    std::cout << "after +=, LinkedHashSet = " << lhset << std::endl;
    std::cout << "LinkedHashSet + {} list = " << (lhset + list) << std::endl;
    std::cout << "LinkedHashSet - {} list = " << (lhset - list2) << std::endl;
    std::cout << "LinkedHashSet * {} list = " << (lhset * list2) << std::endl;
    lhset -= {20, 50};
    std::cout << "LinkedHashSet -={} list = " << lhset << std::endl;
    lhset *= {0, 10, 40, 99};
    std::cout << "LinkedHashSet *={} list = " << lhset << std::endl;
    std::cout << "at end,   LinkedHashSet = " << lhset << std::endl;
}
