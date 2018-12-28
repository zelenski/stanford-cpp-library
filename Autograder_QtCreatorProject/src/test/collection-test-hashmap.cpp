/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "hashmap.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include "strlib.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(HashMapTests, "HashMap tests");

/* Force instantiation of HashMap on a few types to make sure we didn't miss anything.
 * The key types must be hashable.
 */
template class HashMap<int, int>;
template class HashMap<std::string, int>;

TIMED_TEST(HashMapTests, forEachTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    HashMap<std::string, int> hmap;
    hmap["a"] = 1;
    hmap["bbbb"] = 2;
    hmap["zz"] = 26;
    Queue<std::string> expectedKeys {"a", "bbbb", "zz"};
    Queue<int> expectedValues {1, 2, 26};
    while (!expectedKeys.isEmpty()) {
        std::string key = expectedKeys.dequeue();
        assertTrue("HashMap must contain key " + key, hmap.containsKey(key));
        int value = expectedValues.dequeue();
        assertEqualsInt("HashMap[" + key + "] must equal " + std::to_string(value), value, hmap[key]);
    }
}

TIMED_TEST(HashMapTests, frontBackTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    HashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};

    std::string front = hmap.front();
    std::string back  = hmap.back();
    if (front != "a" && front != "b" && front != "c") {
        assertFail("HashMap front fail!");
    }
    if ((back != "a" && back != "b" && back != "c") || (front == back)) {
        assertFail("HashMap back fail!");
    }
}

TIMED_TEST(HashMapTests, initializerListTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<const std::string, int>> pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<const std::string, int>> pairlist2 = {{"b", 20}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int>> expected;

    HashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};
    assertEqualsInt("init list HashMap get a", 10, hmap.get("a"));
    assertEqualsInt("init list HashMap get b", 20, hmap.get("b"));
    assertEqualsInt("init list HashMap get c", 30, hmap.get("c"));
    assertEqualsInt("init list HashMap size", 3, hmap.size());

    hmap += {{"d", 40}, {"e", 50}};
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after +=", expected, hmap);

    HashMap<std::string, int> copy = hmap + pairlist;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after + (shouldn't modify)", expected, hmap);
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"k", 60}, {"t", 70}};
    // assertMap("after + copy", expected, copy);

    copy = hmap - pairlist2;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after - (shouldn't modify)", expected, hmap);
    expected = {{"a", 10}, {"c", 30}, {"d", 40}};
    assertMap("after - copy", expected, copy);

    copy = hmap * pairlist2;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after * (shouldn't modify)", expected, hmap);
    expected = {{"b", 20}, {"e", 50}};
    assertMap("after * copy", expected, copy);

    hmap -= {{"d", 40}, {"e", 50}};
    expected = {{"a", 10}, {"b", 20}, {"c", 30}};
    assertMap("after -=", expected, hmap);

    hmap *= pairlist2;
    expected = {{"b", 20}};
    assertMap("after *=", expected, hmap);
}

TIMED_TEST(HashMapTests, iteratorBasic_HashMap, TEST_TIMEOUT_DEFAULT) {
    HashMap<std::string, int> map;

    map["A"] = 1;
    map["B"] = 2;
    map["C"] = 3;
    map["D"] = 4;

    assertEquals("Map should have all elements present.", map.size(), 4);

    Map<std::string, int> ref;

    for (const auto& key: map) {
        ref[key] = map[key];
    }

    assertEquals("Reference map should have all elements present.", ref.size(), 4);
    assertTrue("Reference map should contain A.", ref.containsKey("A"));
    assertTrue("Reference map should have A = 1.", ref.get("A") == 1);
    assertTrue("Reference map should contain B.", ref.containsKey("B"));
    assertTrue("Reference map should have B = 2.", ref.get("B") == 2);
    assertTrue("Reference map should contain C.", ref.containsKey("C"));
    assertTrue("Reference map should have C = 3.", ref.get("C") == 3);
    assertTrue("Reference map should contain D.", ref.containsKey("D"));
    assertTrue("Reference map should have D = 4.", ref.get("D") == 4);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(HashMapTests, iteratorVersionTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    HashMap<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"f", 60}};
    try {
        for (std::string key : map) {
            int val = map[key];
            if (val % 2 == 0) {
                map.remove(key);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

TIMED_TEST(HashMapTests, randomKeyTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    HashMap<std::string, int> hmap;
    hmap["a"] = 50;
    hmap["b"] = 40;
    hmap["c"] = 30;
    hmap["d"] = 20;
    hmap["e"] = 10;
    hmap["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(hmap);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(HashMapTests, streamExtractTest_HashMap2bad, TEST_TIMEOUT_DEFAULT) {
    HashMap<int, int> hm;
    std::istringstream hmstreambad("1:1, 2, 33}");
    bool result = bool(hmstreambad >> hm);
    assertFalse("operator >> on bad hashmap", result);
}
