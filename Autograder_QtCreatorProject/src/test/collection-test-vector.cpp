/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "stack.h"
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

TEST_CATEGORY(VectorTests, "Vector tests");

TIMED_TEST(VectorTests, compareTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v1 {1, 2, 4, 5};
    Vector<int> v2 {1, 3, 1, 4, 8};
    Vector<int> v3 {1, 1, 7};
    Vector<int> v4 {2, 0};
    Vector<int> v5 {1, 2, 4, 5, 6, 7};
    Vector<int> v6;  // empty
    compareTestHelper(v1, v2, "Vector", /* compareTo */ -1);
    compareTestHelper(v1, v3, "Vector", /* compareTo */  1);
    compareTestHelper(v1, v4, "Vector", /* compareTo */ -1);
    compareTestHelper(v1, v5, "Vector", /* compareTo */ -1);
    compareTestHelper(v1, v6, "Vector", /* compareTo */  1);
    compareTestHelper(v2, v3, "Vector", /* compareTo */  1);
    compareTestHelper(v2, v4, "Vector", /* compareTo */ -1);
    compareTestHelper(v2, v5, "Vector", /* compareTo */  1);
    compareTestHelper(v2, v6, "Vector", /* compareTo */  1);
    compareTestHelper(v3, v4, "Vector", /* compareTo */ -1);
    compareTestHelper(v3, v5, "Vector", /* compareTo */ -1);
    compareTestHelper(v3, v6, "Vector", /* compareTo */  1);
    compareTestHelper(v4, v5, "Vector", /* compareTo */  1);
    compareTestHelper(v4, v6, "Vector", /* compareTo */  1);
    compareTestHelper(v5, v6, "Vector", /* compareTo */  1);

    Set<Vector<int> > sv {v1, v2, v3, v4, v5, v6};
    assertEqualsString("sv", "{{}, {1, 1, 7}, {1, 2, 4, 5}, {1, 2, 4, 5, 6, 7}, {1, 3, 1, 4, 8}, {2, 0}}", sv.toString());
}

TIMED_TEST(VectorTests, forEachTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v1 {1, 2, 3};
    assertCollection("foreach Vector", {1, 2, 3}, v1);

    Vector<std::string> v2 {"a", "b", "c"};
    std::initializer_list<std::string> list {"a", "b", "c"};
    assertCollection("foreach Vector", list, v2);
}

TIMED_TEST(VectorTests, hashCodeTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v;
    v.add(69);
    v.add(42);
    assertEqualsInt("hashcode of self Vector", hashCode(v), hashCode(v));

    Vector<int> copy = v;
    assertEqualsInt("hashcode of copy Vector", hashCode(v), hashCode(copy));

    Vector<int> empty;
    HashSet<Vector<int> > hashvec {v, copy, empty, empty};
    assertEqualsInt("hashset of Vector size", 2, hashvec.size());
}

TIMED_TEST(VectorTests, initializerListTest_Vector, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};

    Vector<int> vec {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, vec);

    vec += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, vec);

    Vector<int> copy = vec + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, vec);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);
}

TIMED_TEST(VectorTests, randomElementTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Vector<std::string> v(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(v);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(VectorTests, shuffleTest, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v {10, 20, 30, 40, 50};
    Map<int, Map<int, int> > valueIndexCount;

    // shuffle 100 times
    for (int i = 0; i < 100; i++) {
        shuffle(v);
        for (int j = 0; j < v.size(); j++) {
            valueIndexCount[v[j]][j]++;
        }
    }

    // make sure each value appeared at each index at least once
    for (int n : v) {
        for (int j = 0; j < v.size(); j++) {
            assertNotEqualsInt("", 0, valueIndexCount[n][j]);
        }
    }
}

TIMED_TEST(VectorTests, streamExtractTest_Vector, TEST_TIMEOUT_DEFAULT) {
    std::istringstream vstream("{1, 2, 3}");
    Vector<int> v;
    vstream >> v;
    assertEqualsString("v", "{1, 2, 3}", v.toString());
}

TIMED_TEST(VectorTests, streamExtractTest_Vector2bad, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v;
    std::istringstream vstreambad("1, 2, 3}");
    bool result = bool(vstreambad >> v);
    assertFalse("operator >> on bad vector", result);
}
