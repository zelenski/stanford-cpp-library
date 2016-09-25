/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "map.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(MapTests, "Map tests");

TIMED_TEST(MapTests, compareTest_Map, TEST_TIMEOUT_DEFAULT) {
    // TODO
}

TIMED_TEST(MapTests, forEachTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> map;
    map["a"] = 1;
    map["bbbb"] = 2;
    map["zz"] = 26;
    std::cout << "map: " << map << std::endl;
    for (std::string k : map) {
        std::cout << k << " => " << map[k] << std::endl;
    }
}

TIMED_TEST(MapTests, hashCodeTest_Map, TEST_TIMEOUT_DEFAULT) {
    HashSet<Map<int, int> > hashmap;
    Map<int, int> map;
    map.add(69, 96);
    map.add(42, 24);
    hashmap.add(map);
    std::cout << "hashset of map: " << hashmap << std::endl;
}

TIMED_TEST(MapTests, initializerListTest_Map, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    std::cout << "init list Map = " << map << std::endl;
    map += {{"d", 40}, {"e", 50}};
    std::cout << "after +=, Map = " << map << std::endl;
    std::cout << "Map + {} list = " << (map + pairlist) << std::endl;
    std::cout << "Map - {} list = " << (map - pairlist2) << std::endl;
    std::cout << "Map * {} list = " << (map * pairlist2) << std::endl;
    map -= {{"b", 20}, {"e", 50}, {"a", 999}};
    std::cout << "Map -={} list = " << map << std::endl;
    map *= {{"z", 0}, {"a", 10}, {"d", 40}, {"x", 99}};
    std::cout << "Map *={} list = " << map << std::endl;
    std::cout << "at end,   Map = " << map << std::endl;
}

TIMED_TEST(MapTests, randomKeyTest_Map, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    Map<std::string, int> map;
    map["a"] = 50;
    map["b"] = 40;
    map["c"] = 30;
    map["d"] = 20;
    map["e"] = 10;
    map["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(map);
        std::cout << s << " ";
        counts[s]++;
    }
}
