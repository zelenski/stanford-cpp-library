/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "set.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <numeric>



/*
 * Force instantiation of Set on a few types to make sure we didn't miss anything.
 * The types must be comparable.
 */
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::SetTraits<int>>;
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::SetTraits<std::string>>;

/*
 * Uncomment this code to include tests that the nice error messages for types missing
 * hashing show up properly.
 */
#if 0
void causeCompilerError(") {
    struct Bad {};

    Set<Bad> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

/*
 * We should NOT get a compiler error trying to stash things in a Set that are not
 * comparable as long as we provide a custom comparator.
 */
static void customComparatorNoError() {
    struct Meh {};

    Set<Meh> okay([](const Meh&, const Meh&) {
        return true;
    });

    (void) okay;
}

#if 0
void badInitializationError() {
    Set<std::string> mySet = "137";
}
#endif

PROVIDED_TEST("Set, commaOperator") {
    /* Confirm that commas work properly. */
    Set<int> one = {1, 2, 3};

    /* Begin by adding some elements in. */
    one += 3, 4, 5; // {1, 2, 3, 4, 5}
    EXPECT_EQUAL( one.size(), 5);

    /* Now remove some elements. */
    one -= 3, 4, 5; // {1, 2}
    EXPECT_EQUAL( one.size(), 2);

    /* Now add a collection of elements. */
    Set<int> two = {3, 4, 5};
    one += two, 6; // {1, 2, 3, 4, 5, 6}
    EXPECT_EQUAL( one.size(), 6);

    /* Now remove a collection of elements. */
    one -= two, 6; // {1, 2}
    EXPECT_EQUAL( one.size(), 2);
}

PROVIDED_TEST("Set, compare") {
    Set<int> set1 {7, 5, 1, 2, 8};
    Set<int> set2 {1, 2, 3, 4};
    Set<int> set3 {2, 1, 3, 4};
    Set<int> set4;
    testCompareOperators(set1, set2, GreaterThan);
    testCompareOperators(set2, set1, LessThan);
    testCompareOperators(set1, set3, GreaterThan);
    testCompareOperators(set2, set3, EqualTo);
    testCompareOperators(set2, set4, GreaterThan);
    testCompareOperators(set4, set1, LessThan);

    // note: shouldn't add set3 because it is 'equal' to set2 (duplicate)
    Set<Set<int> > sset {set1, set2, set3, set4};
    EXPECT_EQUAL( "{{}, {1, 2, 3, 4}, {1, 2, 5, 7, 8}}", sset.toString());
}

PROVIDED_TEST("Set, customComparator") {
    /* Confirm that we can override the comparator if need be. */
    Set<int> descending({1, 2, 3, 4, 5}, std::greater<int>());
    EXPECT_EQUAL( descending.size(), 5);

    auto itr = descending.begin();
    for (int i = 5; i > 0; i--, ++itr) {
        EXPECT_EQUAL( *itr, i);
    }

    /* Now leave the set empty and try overriding the comparator. */
    Set<int> descending2{std::greater<int>()};
    descending2 += 1, 2, 3, 4, 5;

    EXPECT_EQUAL( descending2.size(), 5);

    itr = descending2.begin();
    for (int i = 5; i > 0; i--, ++itr) {
        EXPECT_EQUAL( *itr, i);
    }
}

PROVIDED_TEST("Set, forEach") {
    Set<int> set {10, 20, 30, 40};
    Queue<int> expected {10, 20, 30, 40};

    for (int n : set) {
        int exp = expected.dequeue();
        EXPECT_EQUAL( exp, n);
    }
}

PROVIDED_TEST("Set, firstLast") {
    Set<int> set {20, 10, 40, 30};
    EXPECT_EQUAL( 10, set.first());
    EXPECT_EQUAL(  40, set.last());
}

PROVIDED_TEST("Set, hashCode") {
    Set<int> set;
    set.add(69);
    set.add(42);
    EXPECT_EQUAL( hashCode(set), hashCode(set));

    Set<int> copy = set;
    EXPECT_EQUAL( hashCode(set), hashCode(copy));

    Set<int> empty;
    HashSet<Set<int> > hashset {set, copy, empty, empty};
    EXPECT_EQUAL( 2, hashset.size());
}

PROVIDED_TEST("Set, initializerList") {
    auto list = {60, 70};
    auto list2 = {20, 50};

    Set<int> set {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, set);

    set += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, set);

    Set<int> copy = set + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);

    copy = set - list2;
    assertCollection("after - (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after - copy", {10, 30, 40}, copy);

    copy = set * list2;
    assertCollection("after * (shouldn't modify)", {10, 20, 30, 40, 50}, set);
    assertCollection("after * copy", {20, 50}, copy);

    set -= {20, 50};
    assertCollection("after -=", {10, 30, 40}, set);
    set *= {0, 10, 40, 99};
    assertCollection("after *=", {10, 40}, set);
}

void addDuring(Set<int>& v) { for (int m : v) v.add(0); }
void removeDuring(Set<int>& v) { for (int m : v) v.remove(0); }

PROVIDED_TEST("Set, error on modify during iterate") {
    Set<int> set {1, 2, 3, 4, 1, 6, 1, 8, 2, 10};
    EXPECT_ERROR(addDuring(set));
    EXPECT_ERROR(removeDuring(set));
}


PROVIDED_TEST("Set, mapAll") {
    Set<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) {
        total += value;
    });

    EXPECT_EQUAL( std::accumulate(set.begin(), set.end(), 0), total);
}

PROVIDED_TEST("Set, randomElement") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Set<std::string> hset(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0);
    }
}

PROVIDED_TEST("Set, removeAndRetain") {
    Set<int> all   = { 1, 2, 3, 4, 5, 6, 7, 8 };
    Set<int> evens = {    2,    4,    6,    8,   10 };
    Set<int> odds  = { 1,    3,    5,    7    };

    all.difference(evens);
    EXPECT_EQUAL(all, odds);

    Set<int> primes   = { 2, 3, 5, 7, 11 };
    Set<int> expected = { 3, 5, 7 };

    all.intersect(primes);
     EXPECT_EQUAL(all, expected);
}
