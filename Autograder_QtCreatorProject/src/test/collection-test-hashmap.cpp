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

TIMED_TEST(HashMapTests, hashCodeTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    HashMap<int, int> hmap;
    hmap.add(69, 96);
    hmap.add(42, 24);
    assertEqualsInt("hashcode of self hashmap", hashCode(hmap), hashCode(hmap));

    HashMap<int, int> copy = hmap;
    assertEqualsInt("hashcode of copy hashmap", hashCode(hmap), hashCode(copy));

    HashMap<int, int> empty;

    // shouldn't add two copies of equivalent maps
    HashSet<HashMap<int, int> > hashhashmap {hmap, copy, empty, empty};
    assertEqualsInt("hashset of hashmap size", 2, hashhashmap.size());
    
    HashMap< HashSet<Vector<std::string> >, Vector<std::string> > ngram;
    HashSet<Vector<std::string> > key1;
    HashSet<Vector<std::string> > key2;
    Vector<std::string> keySub;
    keySub.add("fooo");
    key2.add(keySub);
    Vector<std::string> v1;
    v1.add("a");
    v1.add("b");
    Vector<std::string> v2;
    v2.add("c");
    ngram.put(key1, v1);
    ngram.put(key2, v2);
    assertEqualsString("hashmap of hashset of vector", "{{}:{\"a\", \"b\"}, {{\"fooo\"}}:{\"c\"}}", ngram.toString());

    // hash code of hash collections after they are deep-copied
    HashMap<int, int> hmapcode;
    for (int i = 0; i < 99; i++) {
        int rand = randomInteger(-9999, 9999);
        hmapcode[rand] = i*i;
    }
    int hash1 = hashCode(hmapcode);
    HashMap<int, int> hmapcode2 = hmapcode;
    int hash2 = hashCode(hmapcode2);
    assertEqualsString("hashmap copies must be equal", hmapcode.toString(), hmapcode2.toString());
    assertEqualsInt("hashmap copies must have equal hashCodes", hash1, hash2);
    assertEqualsInt("hashmap copies must have equal sizes", hmapcode.size(), hmapcode2.size());

    HashSet<int> hsetcode;
    for (int i = 0; i < 99; i++) {
        int rand = randomInteger(-9999, 9999);
        hsetcode.add(rand);
    }
    hash1 = hashCode(hsetcode);
    HashSet<int> hsetcode2 = hsetcode;
    hash2 = hashCode(hsetcode2);
    assertEqualsString("hashset copies must be equal", hsetcode.toString(), hsetcode2.toString());
    assertEqualsInt("hashset copies must have equal hashCodes", hash1, hash2);
    assertEqualsInt("hashset copies must have equal sizes", hsetcode.size(), hsetcode2.size());
}

TIMED_TEST(HashMapTests, initializerListTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};
    std::initializer_list<std::pair<std::string, int> > expected;

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

TIMED_TEST(HashMapTests, streamExtractTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    std::istringstream hmstream("{1:10, 2:20, 3:30}");
    HashMap<int, int> hm;
    hmstream >> hm;
    assertEqualsString("hm", "{1:10, 2:20, 3:30}", hm.toString());
}

TIMED_TEST(HashMapTests, streamExtractTest_HashMap2bad, TEST_TIMEOUT_DEFAULT) {
    HashMap<int, int> hm;
    std::istringstream hmstreambad("1:1, 2, 33}");
    bool result = bool(hmstreambad >> hm);
    assertFalse("operator >> on bad hashmap", result);
}
