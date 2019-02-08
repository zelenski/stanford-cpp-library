/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedhashset.h"
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

/*
 * Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::LinkedHashSetTraits<int>>;
template class stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::LinkedHashSetTraits<std::string>>;

TEST_CATEGORY(LinkedHashSetTests, "LinkedHashSet tests");

#if 0
void causeCompilerError() {
    struct Bad {};

    LinkedHashSet<Bad> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

#if 0
void badInitializationError() {
    LinkedHashSet<std::string> mySet = "137";
}
#endif

TIMED_TEST(LinkedHashSetTests, commaOperatorLinkedHashTest_Set, TEST_TIMEOUT_DEFAULT) {
    /* Confirm that commas work properly. */
    LinkedHashSet<int> one = {1, 2, 3};

    /* Begin by adding some elements in. */
    one += 3, 4, 5; // {1, 2, 3, 4, 5}
    assertEqualsInt("elements were added", one.size(), 5);

    /* Now remove some elements. */
    one -= 3, 4, 5; // {1, 2}
    assertEqualsInt("elements were removed", one.size(), 2);

    /* Now add a collection of elements. */
    LinkedHashSet<int> two = {3, 4, 5};
    one += two, 6; // {1, 2, 3, 4, 5, 6}
    assertEqualsInt("elements were added", one.size(), 6);

    /* Now remove a collection of elements. */
    one -= two, 6; // {1, 2}
    assertEqualsInt("elements were removed", one.size(), 2);
}

TIMED_TEST(LinkedHashSetTests, compareTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> set1 {7, 5, 1, 2, 8};
    LinkedHashSet<int> set2 {1, 2, 3, 4};
    LinkedHashSet<int> set3 {2, 1, 3, 4};
    LinkedHashSet<int> set4;
    compareTestHelper(set1, set2, "LinkedHashSet", /* compareTo */  1);
    compareTestHelper(set2, set1, "LinkedHashSet", /* compareTo */ -1);
    compareTestHelper(set1, set3, "LinkedHashSet", /* compareTo */  1);
    compareTestHelper(set2, set3, "LinkedHashSet", /* compareTo */  0);
    compareTestHelper(set3, set2, "LinkedHashSet", /* compareTo */  0);
    compareTestHelper(set2, set4, "LinkedHashSet", /* compareTo */  1);

    // note: shouldn't add set3 because it is 'equal' to set2 (duplicate)
    Set<LinkedHashSet<int> > slhset {set1, set2, set3, set4};
    assertEqualsString("slhset", "{{}, {1, 2, 3, 4}, {7, 5, 1, 2, 8}}", slhset.toString());
}

TIMED_TEST(LinkedHashSetTests, forEachTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> lhset {30, 10, 40, 66, -1, 42, 99};
    Queue<int> expected {30, 10, 40, 66, -1, 42, 99};
    for (int n : lhset) {
        int exp = expected.dequeue();
        assertEqualsInt("LinkedHashSet foreach", exp, n);
    }
}

TIMED_TEST(LinkedHashSetTests, frontBackTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> lhset {20, 10, 40, 30};
    assertEqualsInt("LinkedHashSet front", 20, lhset.front());
    assertEqualsInt("LinkedHashSet back",  30, lhset.back());
}

TIMED_TEST(LinkedHashSetTests, hashcodeTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    LinkedHashSet<int> hset;
    hset.add(69);
    hset.add(42);
    assertEqualsInt("hashcode of self LinkedHashSet", hashCode(hset), hashCode(hset));

    LinkedHashSet<int> hset2;
    hset2.add(42);
    hset2.add(69);
    assertEqualsInt("hashcode of LinkedHashSet other order", hashCode(hset), hashCode(hset2));

    LinkedHashSet<int> copy = hset;
    assertEqualsInt("hashcode of copy LinkedHashSet", hashCode(hset), hashCode(copy));

    LinkedHashSet<int> empty;
    HashSet<LinkedHashSet<int> > hashhashset {hset, copy, hset2, empty, empty};
    assertEqualsInt("hashset of LinkedHashset size", 2, hashhashset.size());
}

TIMED_TEST(LinkedHashSetTests, initializerListTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};
    auto list2 = {20, 50};

    LinkedHashSet<int> hset {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, hset);

    hset += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, hset);

    LinkedHashSet<int> copy = hset + list;
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

TIMED_TEST(LinkedHashSetTests, randomElementTest_LinkedHashSet, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    LinkedHashSet<std::string> hset(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}
