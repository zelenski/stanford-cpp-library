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
#include "assertions.h"
#include "set.h"

template <typename CollectionType, typename T>
void assertCollection(std::string message,
                      std::initializer_list<T> expectedList,
                      const CollectionType& collection,
                      bool checkOrder = true) {
    if (checkOrder) {
        CollectionType copy(expectedList);
        assertEqualsCollection("Collections must be equal", copy, collection);
    } else {
        // just needs to contain the same elements in some order
        Set<T> expected(expectedList);
        Set<T> actual;
        for (auto element : collection) {
            actual.add(element);
        }
        assertEqualsCollection("Collections must be equal", expected, actual);
    }
    assertEquals(message + ": size of collection", (int) expectedList.size(), collection.size());
}

template <typename MapType, typename K, typename V>
void assertMap(std::string message, std::initializer_list<std::pair<const K, V>> expected,
               const MapType& map,
               bool checkOrder = true) {
    if (checkOrder) {
        MapType copy(expected);
        assertEqualsCollection("Collections must be equal", copy, map);
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
        assertEqualsCollection("Keys must be equal", expectedKeys, actualKeys);
        assertEqualsCollection("Values must be equal", expectedValues, actualValues);
    }
    assertEquals(message + ": size of collection", (int) expected.size(), map.size());
}

#endif // _collection_test_common_h
