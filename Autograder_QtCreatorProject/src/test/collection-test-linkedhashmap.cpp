/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedhashmap.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(LinkedHashMapTests, "LinkedHashMap tests");


TIMED_TEST(LinkedHashMapTests, randomKeyTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;
    
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
}

TIMED_TEST(LinkedHashMapTests, streamExtractTest_HashMap, TEST_TIMEOUT_DEFAULT) {
    std::istringstream lhmstream("{2:20, 1:10, 4:40, 3:30}");
    LinkedHashMap<int, int> lhm;
    lhmstream >> lhm;
    assertEqualsString("lhm", "{2:20, 1:10, 4:40, 3:30}", lhm.toString());
}

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
    std::cout << "linkedhashmap: " << lhmap << std::endl;
    for (std::string k : lhmap) {
        std::cout << k << " => " << lhmap[k] << std::endl;
    }
    lhmap.remove("ss");
    lhmap.remove("zz");
    lhmap.remove("yZ44");
    lhmap.remove("notfound");
    std::cout << "after removes:" << std::endl;
    std::cout << "linkedhashmap: " << lhmap << std::endl;
    for (std::string k : lhmap) {
        std::cout << k << " => " << lhmap[k] << std::endl;
    }
    std::cout << std::endl;
}

TIMED_TEST(LinkedHashMapTests, hashCodeTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    HashSet<LinkedHashMap<int, int> > hashhashmap;
    LinkedHashMap<int, int> lhmap;
    lhmap.add(69, 96);
    lhmap.add(42, 24);
    hashhashmap.add(lhmap);
}

TIMED_TEST(LinkedHashMapTests, initializerListTest_LinkedHashMap, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    LinkedHashMap<std::string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};
    assertEqualsInt("init list LinkedHashMap get a", 10, hmap.get("a"));
    assertEqualsInt("init list LinkedHashMap get b", 20, hmap.get("b"));
    assertEqualsInt("init list LinkedHashMap get c", 30, hmap.get("c"));
    assertEqualsInt("init list LinkedHashMap size", 3, hmap.size());

    hmap += {{"d", 40}, {"e", 50}};
    assertEqualsInt("after +=, LinkedHashMap get d", 40, hmap.get("d"));
    assertEqualsInt("after +=, LinkedHashMap get e", 50, hmap.get("e"));
    assertEqualsInt("after +=, LinkedHashMap size", 5, hmap.size());

    hmap -= {{"d", 40}, {"e", 50}};
    assertFalse("after -=, LinkedHashMap containsKey d", hmap.containsKey("d"));
    assertFalse("after -=, LinkedHashMap containsKey e", hmap.containsKey("e"));
    assertEqualsInt("after +=, LinkedHashMap size", 3, hmap.size());

    LinkedHashMap<std::string, int> copy = hmap + pairlist;
    assertEqualsInt("after +, LinkedHashMap size", 3, hmap.size());
    assertEqualsInt("after +, LinkedHashMap copy size", 5, copy.size());
    assertEqualsInt("after +, LinkedHashMap copy get k", 60, copy.get("k"));
    assertEqualsInt("after +, LinkedHashMap copy get t", 70, copy.get("t"));

    copy = hmap - pairlist2;
    assertEqualsInt("after -, LinkedHashMap size", 3, hmap.size());
    assertEqualsInt("after -, LinkedHashMap get a", 10, copy.get("a"));
    assertFalse("after -, LinkedHashMap containsKey b", copy.containsKey("b"));
    assertEqualsInt("after -, LinkedHashMap get c", 30, hmap.get("c"));

    copy = hmap * pairlist2;
    assertEqualsInt("after *, LinkedHashMap size", 3, hmap.size());
    assertEqualsInt("after *, LinkedHashMap copy size", 1, copy.size());
    assertFalse("after *, LinkedHashMap containsKey a", copy.containsKey("a"));
    assertEqualsInt("after *, LinkedHashMap get b", 20, copy.get("b"));

    hmap *= pairlist2;
    assertEqualsInt("after *=, LinkedHashMap size", 1, hmap.size());
    assertFalse("after *=, LinkedHashMap containsKey a", hmap.containsKey("a"));
    assertEqualsInt("after -, LinkedHashMap get b", 20, hmap.get("b"));
}
