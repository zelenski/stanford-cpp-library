 /*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "hashcode.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "stl.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <numeric>

/*
 * Force instantiation of HashSet on a few types to make sure we didn't miss anything.
 * The types must be hashable.
 */
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::HashSetTraits<int>>;
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::HashSetTraits<std::string>>;

/*
 * Uncomment this code to include tests that the nice error messages for types missing
 * hashing show up properly.
 */
#if 0
void causeCompilerError() {
    struct Bad {};

    HashSet<Bad> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

/*
 * Uncomment this code to check whether nice error messages show up when you try to initialize
 * a HashSet improperly.
 */
#if 0
void badInitializationError() {
    HashSet<std::string> mySet = "137";
}
#endif

PROVIDED_TEST("HashSet, commaOperator") {
    /* Confirm that commas work properly. */
    HashSet<int> one = {1, 2, 3};

    /* Begin by adding some elements in. */
    one += 3, 4, 5; // {1, 2, 3, 4, 5}
    EXPECT_EQUAL( one.size(), 5);

    /* Now remove some elements. */
    one -= 3, 4, 5; // {1, 2}
    EXPECT_EQUAL( one.size(), 2);

    /* Now add a collection of elements. */
    HashSet<int> two = {3, 4, 5};
    one += two, 6; // {1, 2, 3, 4, 5, 6}
    EXPECT_EQUAL( one.size(), 6);

    /* Now remove a collection of elements. */
    one -= two, 6; // {1, 2}
    EXPECT_EQUAL( one.size(), 2);
}

PROVIDED_TEST("HashSet, forEach") {
    HashSet<int> hset {40, 20, 10, 30};
    Set<int> expected {10, 20, 30, 40};
    for (int n : hset) {
        EXPECT(expected.contains(n));
    }
}

PROVIDED_TEST("HashSet, firstLast") {
    HashSet<int> hset {10, 20, 30, 40};
    int front = hset.first();
    int back = hset.last();
    EXPECT (front == 10 || front == 20 || front == 30 || front == 40);
    EXPECT ((back == 10 || back == 20 || back == 30 || back == 40) && (front != back));
}

PROVIDED_TEST("HashSet, hashCode") {
    HashSet<int> hset;
    hset.add(69);
    hset.add(42);
    EXPECT_EQUAL( hashCode(hset), hashCode(hset));

    HashSet<int> copy = hset;
    EXPECT_EQUAL( hashCode(hset), hashCode(copy));

    HashSet<int> empty;
    HashSet<HashSet<int> > hashhashset {hset, copy, empty, empty};
    EXPECT_EQUAL( 2, hashhashset.size());
}

PROVIDED_TEST("HashSet, initializerList") {
    auto list = {60, 70};
    auto list2 = {20, 50};

    HashSet<int> hset {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, hset);

    hset += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, hset);

    HashSet<int> copy = hset + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);

    copy = hset - list2;
    assertCollection("after - (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after - copy", {10, 30, 40}, copy);

    copy = hset * list2;
    assertCollection("after * (shouldn't modify)", {10, 20, 30, 40, 50}, hset);
    assertCollection("after * copy", {20, 50}, copy);

    hset -= {20, 50};
    assertCollection("after -=", {10, 30, 40}, hset);
    hset *= {0, 10, 40, 99};
    assertCollection("after *=", {10, 40}, hset);
}


void addDuring(HashSet<int>& v) { for (int m : v) v.add(0); }
void removeDuring(HashSet<int>& v) { for (int m : v) v.remove(0); }

PROVIDED_TEST("HashSet, error on modify during iterate") {
    HashSet<int> set {1, 2, 3, 4, 1, 6, 1, 8, 2, 10};
    EXPECT_ERROR(addDuring(set));
    EXPECT_ERROR(removeDuring(set));
}


PROVIDED_TEST("HashSet, mapAll") {
    HashSet<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) {
        total += value;
    });

    EXPECT_EQUAL( std::accumulate(set.begin(), set.end(), 0), total);
}

PROVIDED_TEST("HashSet, randomElement") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    HashSet<std::string> hset(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0);
    }
}

PROVIDED_TEST("HashSet, removeAndRetain") {
    HashSet<int> all   = { 1, 2, 3, 4, 5, 6, 7, 8 };
    HashSet<int> evens = {    2,    4,    6,    8,   10 };
    HashSet<int> odds  = { 1,    3,    5,    7    };

    all.difference(evens);
    EXPECT_EQUAL(all, odds);

    HashSet<int> primes   = { 2, 3, 5, 7, 11 };
    HashSet<int> expected = { 3, 5, 7 };

    all.intersect(primes);
    EXPECT_EQUAL(all, expected);
}
