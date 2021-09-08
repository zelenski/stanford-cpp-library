/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "map.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "set.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(MapTests, "Map tests");

/*
 * Force instantiation of Map on a few types to make sure we didn't miss anything.
 * The key types must be comparable.
 */
template class Map<int, int>;
template class Map<std::string, int>;

/*
 * Uncomment this code to include tests that the nice error messages for types missing
 * hashing show up properly.
 */
#if 0
void causeCompilerError() {
    struct Bad {};

    Map<Bad, int> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

/* This code, on the other hand, should NOT cause a compiler error, since we gave an
 * explicit comparison function.
 */
static void customComparatorNoError() {
    struct Meh {};

    Map<Meh, int> okay([](const Meh&, const Meh&) {
        return true;
    });

    (void) okay;
}

TIMED_TEST(MapTests, compareTest_Map, TEST_TIMEOUT_DEFAULT) {
    // TODO
}

TIMED_TEST(MapTests, forEachTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> map;
    map.put("zz", 26);
    map.put("a", 1);
    map.put("ss", 97);
    map.put("bbbb", 2);
    map.put("E", 4);
    map.put("yZ44", 33);
    Set<std::string> expected {"zz", "a", "ss", "bbbb", "E", "yZ44"};
    for (const std::string& key : map) {
        assertTrue("foreach Map contains " + key, expected.contains(key));
    }
    map.remove("ss");
    map.remove("zz");
    map.remove("yZ44");
    map.remove("notfound");
    expected = {"a", "bbbb", "E"};
    for (const std::string& key : map) {
        assertTrue("foreach Map contains " + key, expected.contains(key));
    }
}

TIMED_TEST(MapTests, frontBackTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    assertEqualsString("Map front", "a", map.front());
    assertEqualsString("Map back",  "c", map.back());
}

TIMED_TEST(MapTests, hashCodeTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<int, int> map;
    map.add(69, 96);
    map.add(42, 24);
    assertEqualsInt("hashcode of self Map", hashCode(map), hashCode(map));

    Map<int, int> copy = map;
    assertEqualsInt("hashcode of copy Map", hashCode(map), hashCode(copy));

    Map<int, int> empty;

    HashSet<Map<int, int> > hashmap {map, copy, empty, empty};
    assertEqualsInt("hashset of Map size", 2, hashmap.size());
}

TIMED_TEST(MapTests, initializerListTest_Map, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<const std::string, int>> pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<const std::string, int>> pairlist2 = {{"b", 20}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int>> expected;

    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    assertEqualsInt("init list Map get a", 10, map.get("a"));
    assertEqualsInt("init list Map get b", 20, map.get("b"));
    assertEqualsInt("init list Map get c", 30, map.get("c"));
    assertEqualsInt("init list Map size", 3, map.size());

    map += {{"d", 40}, {"e", 50}};
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after +=", expected, map);

    Map<std::string, int> copy = map + pairlist;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after + (shouldn't modify)", expected, map);
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"k", 60}, {"t", 70}};
    // assertMap("after + copy", expected, copy);

    copy = map - pairlist2;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after - (shouldn't modify)", expected, map);
    expected = {{"a", 10}, {"c", 30}, {"d", 40}};
    assertMap("after - copy", expected, copy);

    copy = map * pairlist2;
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after * (shouldn't modify)", expected, map);
    expected = {{"b", 20}, {"e", 50}};
    assertMap("after * copy", expected, copy);

    map -= {{"d", 40}, {"e", 50}};
    expected = {{"a", 10}, {"b", 20}, {"c", 30}};
    assertMap("after -=", expected, map);

    map *= pairlist2;
    expected = {{"b", 20}};
    assertMap("after *=", expected, map);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(MapTests, iteratorVersionTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"f", 60}};
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

TIMED_TEST(MapTests, randomKeyTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Map<std::string, int> map;
    map["a"] = 50;
    map["b"] = 40;
    map["c"] = 30;
    map["d"] = 20;
    map["e"] = 10;
    map["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(map);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(MapTests, streamExtractTest_Map, TEST_TIMEOUT_DEFAULT) {
    std::istringstream stream("{2:20, 1:10, 4:40, 3:30}");
    Map<int, int> map;
    stream >> map;
    assertEqualsString("map", "{1:10, 2:20, 3:30, 4:40}", map.toString());
}
