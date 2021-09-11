/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "hashmap.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>


/*
 * Force instantiation of HashMap on a few types to make sure we didn't miss anything.
 * The key types must be hashable.
 */
template class HashMap<int, int>;
template class HashMap<std::string, int>;

/*
 * Uncomment this code to include tests that the nice error messages for types missing
 * hashing show up properly.
 */
#if 0
void causeCompilerError() {
    struct Bad {};

    HashMap<Bad, int> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

PROVIDED_TEST("HashMap, forEach") {
    HashMap<std::string, int> hmap;
    hmap["a"] = 1;
    hmap["bbbb"] = 2;
    hmap["zz"] = 26;
    Queue<std::string> expectedKeys {"a", "bbbb", "zz"};
    Queue<int> expectedValues {1, 2, 26};
    while (!expectedKeys.isEmpty()) {
        std::string key = expectedKeys.dequeue();
        EXPECT(hmap.containsKey(key));
        int value = expectedValues.dequeue();
        EXPECT_EQUAL(hmap[key], value);
    }
}

PROVIDED_TEST("HashMap, firstLast") {
    HashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};

    std::string front = hmap.firstKey();
    std::string back  = hmap.lastKey();
    EXPECT(front == "a" || front != "b" || front != "c");
    EXPECT((back == "a" || back != "b" || back != "c") && (front != back));
}

PROVIDED_TEST("HashMap, initializerList") {
    std::initializer_list<std::pair<const std::string, int>> pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<const std::string, int>> pairlist2 = {{"b", 20}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int>> expected;

    HashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};
    EXPECT_EQUAL( 10, hmap.get("a"));
    EXPECT_EQUAL( 20, hmap.get("b"));
    EXPECT_EQUAL( 30, hmap.get("c"));
    EXPECT_EQUAL( 3, hmap.size());

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

PROVIDED_TEST("HashMap, iterator") {
    HashMap<std::string, int> map;

    map["A"] = 1;
    map["B"] = 2;
    map["C"] = 3;
    map["D"] = 4;

    EXPECT_EQUAL( map.size(), 4);

    Map<std::string, int> ref;

    for (const auto& key: map) {
        ref[key] = map[key];
    }

    EXPECT_EQUAL( ref.size(), 4);
    EXPECT( ref.containsKey("A"));
    EXPECT( ref.get("A") == 1);
    EXPECT( ref.containsKey("B"));
    EXPECT( ref.get("B") == 2);
    EXPECT( ref.containsKey("C"));
    EXPECT( ref.get("C") == 3);
    EXPECT( ref.containsKey("D"));
    EXPECT( ref.get("D") == 4);
}

void addDuring(HashMap<std::string, int>& m) { for (auto k:m) m.put(k+k, 3); }
void removeDuring(HashMap<std::string, int> & m) { for (auto k:m) m.remove(k); }

PROVIDED_TEST("HashMap, error on modify during iterate") {
    HashMap<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"f", 60}};
    EXPECT_ERROR(addDuring(map));
    EXPECT_ERROR(removeDuring(map));
}


PROVIDED_TEST("HashMap, randomKey") {
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
        EXPECT(counts[s] > 0);
    }
}

PROVIDED_TEST("HashMap, streamExtract") {
    HashMap<int, int> hm;
    std::istringstream hmstreambad("1:1, 2, 33}");
    bool result = bool(hmstreambad >> hm);
    EXPECT(! result);
}
