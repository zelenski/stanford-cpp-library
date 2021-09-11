/*
 * Common helper functions used by collection tests.
 * @version 2016/10/22
 * - initial version
 */

#ifndef _collection_test_common_h
#define _collection_test_common_h

#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "set.h"
#include "testing/SimpleTest.h"


// for testing collections with non- ==, < elements
class Student {
public:
    std::string name;
};


template <typename CollectionType, typename T>
void assertCollection(std::string ,
                      std::initializer_list<T> expectedList,
                      const CollectionType& collection,
                      bool checkOrder = true) {
    if (checkOrder) {
        CollectionType copy(expectedList);
        EXPECT_EQUAL(copy, collection);
    } else {
        // just needs to contain the same elements in some order
        Set<T> expected(expectedList);
        Set<T> actual;
        for (auto element : collection) {
            actual.add(element);
        }
        EXPECT_EQUAL(expected, actual);
    }
    EXPECT_EQUAL(expectedList.size(), collection.size());
}

template <typename MapType, typename K, typename V>
void assertMap(std::string , std::initializer_list<std::pair<const K, V>> expected,
               const MapType& map,
               bool checkOrder = true) {
    if (checkOrder) {
        MapType copy(expected);
        EXPECT_EQUAL(copy, map);
    } else {
        // just needs to contain the same key/value pairs in some order
        Set<K> expectedKeys;
        Set<V> expectedValues;
        Set<K> actualKeys;
        Set<V> actualValues;
        for (auto pair : expected) {
            expectedKeys.add(pair.first);
            expectedValues.add(pair.second);
        }
        for (auto key : map) {
            actualKeys.add(key);
            actualValues.add(map.get(key));
        }
        EXPECT_EQUAL(expectedKeys, actualKeys);
        EXPECT_EQUAL(expectedValues, actualValues);
    }
    EXPECT_EQUAL(expected.size(), map.size());
}


enum { LessThan = -1, EqualTo = 0, GreaterThan = 1 };

template <typename T>
void testCompareOperators(const T& o1, const T& o2,  int compareTo = 0) {
    EXPECT_EQUAL(compareTo <  0,  o1 <  o2);
    EXPECT_EQUAL(compareTo <= 0,  o1 <= o2);
    EXPECT_EQUAL(compareTo >  0,  o1 >  o2);
    EXPECT_EQUAL(compareTo >= 0,  o1 >= o2);
    EXPECT_EQUAL(compareTo == 0,  o1 == o2);
    EXPECT_EQUAL(compareTo != 0,  o1 != o2);
}

template <typename T>
void clearDuringIteration(T& t) { for (auto& e: t) t.clear(); }

template <typename T>
void doDuringIteration(T& t, std::function<void(T&)> fn) { for (auto& e: t)  fn(t); }


#endif // _collection_test_common_h
