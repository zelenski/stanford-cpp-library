/*
 * Test file for verifying the Stanford C++ lib HashMap class.
 */

#include "hashmap.h"
#include "queue.h"
#include "common.h"
#include "SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

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

PROVIDED_TEST("HashMap, operator[]") {
    const HashMap<std::string, bool> m1 = {
        { "A", true }
    };
    const HashMap<std::string, int> m2 = {
        { "A", 1 }
    };
    const HashMap<std::string, float> m3 = {
        { "A", 1.0f }
    };
    const HashMap<std::string, double> m4 = {
        { "A", 1.0 }
    };

    auto ptr = std::make_shared<std::istringstream>();
    const HashMap<std::string, std::shared_ptr<std::istringstream>> m5 = {
        { "A", ptr }
    };

    EXPECT_EQUAL(m1["A"], true);
    EXPECT_EQUAL(m1["B"], false);

    EXPECT_EQUAL(m2["A"], 1);
    EXPECT_EQUAL(m2["B"], 0);

    EXPECT_EQUAL(m3["A"], 1.0f);
    EXPECT_EQUAL(m3["B"], 0.0f);

    EXPECT_EQUAL(m4["A"], 1.0);
    EXPECT_EQUAL(m4["B"], 0.0);

    EXPECT_EQUAL(m5["A"], ptr);
    EXPECT_EQUAL(m5["B"], nullptr);
}
