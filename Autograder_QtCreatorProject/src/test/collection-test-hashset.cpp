/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "hashcode.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(HashSetTests, "HashSet tests");

TIMED_TEST(HashSetTests, forEachTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> hset;
    hset += 40, 20, 10, 30;

    Set<int> expected {10, 20, 30, 40};
    for (int n : hset) {
        assertTrue("HashSet must contain " + integerToString(n), expected.contains(n));
    }
}

TIMED_TEST(HashSetTests, hashCodeTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> hset;
    hset.add(69);
    hset.add(42);
    assertEqualsInt("hashcode of self HashSet", hashCode(hset), hashCode(hset));

    HashSet<int> copy = hset;
    assertEqualsInt("hashcode of copy HashSet", hashCode(hset), hashCode(copy));

    HashSet<int> empty;
    HashSet<HashSet<int> > hashhashset {hset, copy, empty, empty};
    assertEqualsInt("hashset of Hashset size", 2, hashhashset.size());
}

TIMED_TEST(HashSetTests, initializerListTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    auto list2 = {20, 50};

    HashSet<int> hset {10, 20, 30};
    Vector<int> expected {10, 20, 30};
    for (int n : expected) {
        assertTrue("init list HashSet must contain " + integerToString(n), hset.contains(n));
    }
    assertEqualsInt("after +=, HashSet size", 3, hset.size());

    hset += {40, 50};
    expected = {10, 20, 30, 40, 50};
    for (int n : expected) {
        assertTrue("after +=, HashSet must contain " + integerToString(n), hset.contains(n));
    }
    assertEqualsInt("after +=, HashSet size", 5, hset.size());

    HashSet<int> copy = hset + list;
    assertEqualsInt("after +, HashSet size", 5, hset.size());
    std::cout << "HashSet + {} list = " << (hset + list) << std::endl;
    std::cout << "HashSet - {} list = " << (hset - list2) << std::endl;
    std::cout << "HashSet * {} list = " << (hset * list2) << std::endl;
    hset -= {20, 50};
    std::cout << "HashSet -={} list = " << hset << std::endl;
    hset *= {0, 10, 40, 99};
    std::cout << "HashSet *={} list = " << hset << std::endl;
    std::cout << "at end,   HashSet = " << hset << std::endl;
}

TIMED_TEST(HashSetTests, randomKeyTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::cout << "HashSet: ";
    HashSet<std::string> hset;
    hset += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        std::cout << s << " ";
        counts[s]++;
    }
}







