/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "hashcode.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "stl.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <numeric>

TEST_CATEGORY(HashSetTests, "HashSet tests");

/* Force instantiation of HashSet on a few types to make sure we didn't miss anything.
 * The types must be hashable.
 */
template class HashSet<int>;
template class HashSet<std::string>;

TIMED_TEST(HashSetTests, forEachTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> hset {40, 20, 10, 30};
    Set<int> expected {10, 20, 30, 40};
    for (int n : hset) {
        assertTrue("HashSet must contain " + std::to_string(n), expected.contains(n));
    }
}

TIMED_TEST(HashSetTests, frontBackTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> hset {10, 20, 30, 40};
    int front = hset.front();
    int back = hset.back();
    if (front != 10 && front != 20 && front != 30 && front != 40) {
        assertFail("HashSet front fail!");
    }
    if ((back != 10 && back != 20 && back != 30 && back != 40) || (front == back)) {
        assertFail("HashSet back fail!");
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
    assertCollection("initial", {10, 20, 30}, hset);

    hset += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, hset);

    HashSet<int> copy = hset + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);

    copy = hset - list2;
    assertCollection("after - (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after - copy", {10, 30, 40}, copy);

    copy = hset * list2;
    assertCollection("after * (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after * copy", {20, 50}, copy);

    hset -= {20, 50};
    assertCollection("after -=", {10, 30, 40}, hset);
    hset *= {0, 10, 40, 99};
    assertCollection("after *=", {10, 40}, hset);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(HashSetTests, iteratorVersionTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> set {1, 2, 3, 4, 1, 6, 1, 8, 2, 10};
    try {
        for (int n : set) {
            if (n % 2 == 0) {
                set.remove(n);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

TIMED_TEST(HashSetTests, mapAllTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    HashSet<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) {
        total += value;
    });

    assertEqualsInt("mapAll produces correct sum.", std::accumulate(set.begin(), set.end(), 0), total);
}

TIMED_TEST(HashSetTests, randomElementTest_HashSet, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    HashSet<std::string> hset(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}
