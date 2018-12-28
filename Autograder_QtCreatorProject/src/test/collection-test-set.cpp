/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "set.h"
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
#include <numeric>

TEST_CATEGORY(SetTests, "Set tests");

/* Force instantiation of Set on a few types to make sure we didn't miss anything.
 * The types must be comparable.
 */
template class Set<int>;
template class Set<std::string>;

TIMED_TEST(SetTests, compareTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set1 {7, 5, 1, 2, 8};
    Set<int> set2 {1, 2, 3, 4};
    Set<int> set3 {2, 1, 3, 4};
    Set<int> set4;
    compareTestHelper(set1, set2, "Set", /* compareTo */  1);
    compareTestHelper(set2, set1, "Set", /* compareTo */ -1);
    compareTestHelper(set1, set3, "Set", /* compareTo */  1);
    compareTestHelper(set2, set3, "Set", /* compareTo */  0);
    compareTestHelper(set2, set4, "Set", /* compareTo */  1);
    compareTestHelper(set4, set1, "Set", /* compareTo */ -1);

    // note: shouldn't add set3 because it is 'equal' to set2 (duplicate)
    Set<Set<int> > sset {set1, set2, set3, set4};
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

TIMED_TEST(SetTests, frontBackTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set {20, 10, 40, 30};
    assertEqualsInt("Set front", 10, set.front());
    assertEqualsInt("Set back",  40, set.back());
}

TIMED_TEST(SetTests, hashCodeTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set;
    set.add(69);
    set.add(42);
    assertEqualsInt("hashcode of self Set", hashCode(set), hashCode(set));

    Set<int> copy = set;
    assertEqualsInt("hashcode of copy Set", hashCode(set), hashCode(copy));

    Set<int> empty;
    HashSet<Set<int> > hashset {set, copy, empty, empty};
    assertEqualsInt("hashset of Set size", 2, hashset.size());
}

TIMED_TEST(SetTests, initializerListTest_Set, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    auto list2 = {20, 50};

    Set<int> set {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, set);

    set += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, set);

    Set<int> copy = set + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);

    copy = set - list2;
    assertCollection("after - (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after - copy", {10, 30, 40}, copy);

    copy = set * list2;
    assertCollection("after * (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after * copy", {20, 50}, copy);

    set -= {20, 50};
    assertCollection("after -=", {10, 30, 40}, set);
    set *= {0, 10, 40, 99};
    assertCollection("after *=", {10, 40}, set);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(SetTests, iteratorVersionTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set {1, 2, 3, 4, 1, 6, 1, 8, 2, 10};
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

TIMED_TEST(SetTests, mapAllTest_Set, TEST_TIMEOUT_DEFAULT) {
    Set<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) {
        total += value;
    });

    assertEqualsInt("mapAll produces correct sum.", std::accumulate(set.begin(), set.end(), 0), total);
}

TIMED_TEST(SetTests, randomElementTest_Set, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Set<std::string> hset(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}
