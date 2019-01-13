/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedhashmap.h"
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

TEST_CATEGORY(LinkedHashMapTests, "LinkedHashMap tests");

/* Force instantiation of the template on a type to ensure that we don't have anything
 * invidious lurking that just didn't get compiled.
 */
template class LinkedHashMap<int, int>;
template class LinkedHashMap<int, std::string>;
template class LinkedHashMap<std::string, int>;
template class LinkedHashMap<std::string, std::string>;

TIMED_TEST(LinkedHashMapTests, compareTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    // TODO
}

TIMED_TEST(LinkedHashMapTests, forEachTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    LinkedHashMap<std::string, int> lhmap;
    lhmap.put("zz", 26);
    lhmap.put("a", 1);
    lhmap.put("ss", 97);
    lhmap.put("bbbb", 2);
    lhmap.put("E", 4);
    lhmap.put("yZ44", 33);
    Queue<std::string> expected {"zz", "a", "ss", "bbbb", "E", "yZ44"};
    for (const std::string& key : lhmap) {
        std::string expkey = expected.dequeue();
        assertEqualsString("foreach LinkedHashMap", expkey, key);
    }
    lhmap.remove("ss");
    lhmap.remove("zz");
    lhmap.remove("yZ44");
    lhmap.remove("notfound");
    expected = {"a", "bbbb", "E"};
    for (const std::string& key : lhmap) {
        std::string expkey = expected.dequeue();
        assertEqualsString("foreach LinkedHashMap after removes", expkey, key);
    }
}

TIMED_TEST(LinkedHashMapTests, frontBackTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    LinkedHashMap<std::string, int> lhmap {{"b", 20}, {"c", 30}, {"a", 10}};
    assertEqualsString("LinkedHashMap front", "b", lhmap.front());
    assertEqualsString("LinkedHashMap back",  "a", lhmap.back());
}

TIMED_TEST(LinkedHashMapTests, hashCodeTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    LinkedHashMap<int, int> lhmap;
    lhmap.add(69, 96);
    lhmap.add(42, 24);
    assertEqualsInt("hashcode of self LinkedHashMap", hashCode(lhmap), hashCode(lhmap));

    LinkedHashMap<int, int> copy = lhmap;
    assertEqualsInt("hashcode of copy LinkedHashMap", hashCode(lhmap), hashCode(copy));

    LinkedHashMap<int, int> empty;

    HashSet<LinkedHashMap<int, int> > hashhashmap {lhmap, copy, empty, empty};
    assertEqualsInt("hashset of LinkedHashMap size", 2, hashhashmap.size());
}

TIMED_TEST(LinkedHashMapTests, initializerListTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<const std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<const std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int> > expected;

    LinkedHashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};
    assertEqualsInt("init list LinkedHashMap get a", 10, hmap.get("a"));
    assertEqualsInt("init list LinkedHashMap get b", 20, hmap.get("b"));
    assertEqualsInt("init list LinkedHashMap get c", 30, hmap.get("c"));
    assertEqualsInt("init list LinkedHashMap size", 3, hmap.size());

    hmap += {{"d", 40}, {"e", 50}};
    expected = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after +=", expected, hmap);

    LinkedHashMap<std::string, int> copy = hmap + pairlist;
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

TIMED_TEST(LinkedHashMapTests, randomKeyTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    LinkedHashMap<std::string, int> lhmap;
    lhmap.put("a", 50);
    lhmap.put("b", 40);
    lhmap.put("c", 30);
    lhmap.put("d", 20);
    lhmap.put("e", 10);
    lhmap.put("f",  0);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(lhmap);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(LinkedHashMapTests, streamExtractTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    std::istringstream lhmstream("{2:20, 1:10, 4:40, 3:30}");
    LinkedHashMap<int, int> lhm;
    lhmstream >> lhm;
    assertEqualsString("lhm", "{2:20, 1:10, 4:40, 3:30}", lhm.toString());
}
