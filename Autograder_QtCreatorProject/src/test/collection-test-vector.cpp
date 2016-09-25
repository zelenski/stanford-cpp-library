/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "stack.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
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
    Vector<int> v1;
    v1 += 1, 2, 3;
    std::cout << "v1: " << v1 << std::endl;

    Vector<std::string> v2;
    v2 += "a", "b", "c";
    std::cout << "v2: " << v2 << std::endl;
}

TIMED_TEST(VectorTests, hashCodeTest_Vector, TEST_TIMEOUT_DEFAULT) {
    HashSet<Vector<int> > hashvec;
    Vector<int> v;
    v.add(69);
    v.add(42);
    hashvec.add(v);
    std::cout << "hashset of vector: " << hashvec << std::endl;
}

TIMED_TEST(VectorTests, initializerListTest_Vector, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};

    Vector<int> v {10, 20, 30};
    std::cout << "init list Vector = " << v << std::endl;
    v += {40, 50};
    std::cout << "after +=, Vector = " << v << std::endl;
    std::cout << "Vector + {} list = " << (v + list) << std::endl;
    std::cout << "at end,   Vector = " << v << std::endl;
    v = {999, 888, 777};
    std::cout << "on =,     Vector = " << v << std::endl;
    v.clear();
    v.add(777);
    std::initializer_list<int> sevenlist = {777};
    if (v == sevenlist) {
        std::cout << "op ==, Vector equal" << std::endl;
    } else {
        std::cout << "op ==, Vector not equal" << std::endl;
    }
}

TIMED_TEST(VectorTests, randomElementTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    Vector<std::string> v;
    v += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(v);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    std::cout << "counts:" << counts << std::endl << std::endl;
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
