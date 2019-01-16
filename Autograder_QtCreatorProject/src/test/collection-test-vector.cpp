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
#include <numeric>

TEST_CATEGORY(VectorTests, "Vector tests");

/* Force instantiation of Vector on a few types to make sure we didn't miss anything.
 * These types must be comparable and hashable to avoid instaniating functions we
 * can't support.
 */
template class Vector<int>;
template class Vector<bool>;
template class Vector<std::string>;

TIMED_TEST(VectorTests, basicsTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> values;
    for (int i = 0; i < 1000; i++) {
        values += i;
    }

    assertEqualsInt("Size is 1000", values.size(), 1000);
    assertFalse("Vector isn't empty.", values.isEmpty());

    for (int i = 0; i < 1000; i++) {
        assertEquals("Vector entry at position " + std::to_string(i) + " is correct.",
                     values[i], i);
    }

    /* Mutate things. */
    for (int& elem: values) {
        elem *= 2;
    }

    assertEqualsInt("Size is 1000", values.size(), 1000);
    assertFalse("Vector isn't empty.", values.isEmpty());

    for (int i = 0; i < 1000; i++) {
        assertEquals("Vector entry at position " + std::to_string(i) + " is correct.",
                     values[i], 2 * i);
    }
}

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

TIMED_TEST(VectorTests, frontBackTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v {10, 20, 30, 40};
    assertEqualsInt("Vector front", 10, v.front());
    assertEqualsInt("Vector back",  40, v.back());
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

TIMED_TEST(VectorTests, indexOfTest_Vector, TEST_TIMEOUT_DEFAULT) {
    //                0   1   2   3   4   5   6   7  8
    Vector<int> vec {10, 20, 30, 10, 40, 10, 50, 60, 0};
    assertEqualsInt("indexOf 10", 0, vec.indexOf(10));
    assertEqualsInt("indexOf 20", 1, vec.indexOf(20));
    assertEqualsInt("indexOf 50", 6, vec.indexOf(50));
    assertEqualsInt("indexOf 99", -1, vec.indexOf(99));

    assertEqualsInt("lastIndexOf 10", 5, vec.lastIndexOf(10));
    assertEqualsInt("lastIndexOf 20", 1, vec.lastIndexOf(20));
    assertEqualsInt("lastIndexOf 0",  8, vec.lastIndexOf(0));
    assertEqualsInt("lastIndexOf 99", -1, vec.lastIndexOf(99));

    assertEqualsBool("contains 10", true, vec.contains(10));
    assertEqualsBool("contains 20", true, vec.contains(20));
    assertEqualsBool("contains 50", true, vec.contains(50));
    assertEqualsBool("contains 70", false, vec.contains(70));
    assertEqualsBool("contains -1", false, vec.contains(-1));
    assertEqualsBool("contains 99", false, vec.contains(99));
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

TIMED_TEST(VectorTests, insertTest_Vector, TEST_TIMEOUT_DEFAULT) {
    /* Confirm that insert works at all the right positions, and no other positions. */
    static const int kNumElems = 137;

    for (int i = 0; i <= kNumElems; i++) {
        Vector<int> values;
        for (int j = 0; j < kNumElems; j++) {
            values += j;
        }
        assertEqualsInt("Vector has proper size", kNumElems, values.size());

        values.insert(i, kNumElems + 1);
        assertEqualsInt("Vector was resized.", kNumElems + 1, values.size());

        /* Confirm all the values are correct. */
        for (int j = 0; j < i; j++) {
            assertEqualsInt("prior elements unchanged", values[j], j);
        }
        assertEqualsInt("new element inserted", values[i], kNumElems + 1);
        for (int j = i + 1; j < values.size(); j++) {
            assertEqualsInt("post elements unchanged", values[j], j - 1);
        }
    }
}

TIMED_TEST(VectorTests, iteratorConversionTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto& cv = v;

    assertTrue("Const and non-const iterators to same element compare equal.", v.begin() == cv.begin());
    assertTrue("Const and non-const iterators to past-end compare equal.", v.end() == cv.end());
}

TIMED_TEST(VectorTests, iteratorRangeCheckingTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Should fail if we try to back up past beginning. */
    try {
        auto itr = v.begin();
        --itr;
        assertFail("Should have triggered an error decrementing start-of-range");
    } catch (const ErrorException &) {

    }
    try {
        auto itr = v.begin();
        itr--;
        assertFail("Should have triggered an error decrementing start-of-range");
    } catch (const ErrorException &) {

    }

    /* Should fail if we try to advance past end. */
    try {
        auto itr = v.end();
        ++itr;
        assertFail("Should have triggered an error incrementing end-of-range.");
    } catch (const ErrorException &) {

    }
    try {
        auto itr = v.end();
        ++itr;
        assertFail("Should have triggered an error incrementing end-of-range.");
    } catch (const ErrorException &) {

    }

    /* Should fail if we try to use random access to jump out of range. */
    try {
        auto itr = v.begin();
        itr[137] = 42;
        assertFail("Should have triggered an error incrementing end-of-range.");
    } catch (const ErrorException &) {

    }
    try {
        auto itr = v.begin();
        itr[137] = 42;
        assertFail("Should have triggered an error incrementing end-of-range.");
    } catch (const ErrorException &) {

    }

    /* Should NOT fail if we try to use random access to jump into range. */
    try {
        auto itr = v.begin();
        itr[9] = 137;
    } catch (const ErrorException &) {
        assertFail("Triggered an exception trying to go in range.");
    }
    try {
        auto itr = v.end();
        itr[-10] = 42;
    } catch (const ErrorException &) {
        assertFail("Triggered an exception trying to go in range.");
    }

    /* Should fail doing ANYTHING except comparisons with a singular iterator. */
    try {
        Vector<int>::iterator itr;
        itr++;
        assertFail("No exception triggered incrementing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        ++itr;
        assertFail("No exception triggered incrementing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr--;
        assertFail("No exception triggered decrementing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        --itr;
        assertFail("No exception triggered incrementing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<std::string>::iterator itr;
        *itr;
        assertFail("No exception triggered dereferencing singular iterator.");
    } catch (const ErrorException &) {

    }

    try {
        Vector<std::string>::iterator itr;
        itr->size();
        assertFail("No exception triggered dereferencing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<std::string>::iterator itr;
        itr[137];
        assertFail("No exception triggered dereferencing singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr - v.begin();
        assertFail("No exception triggered subtracting singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() - itr;
        assertFail("No exception triggered subtracting singular iterator.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr == v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr != v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr < v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr <= v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr >= v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        itr >  v.begin();
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }

    try {
        Vector<int>::iterator itr;
        v.begin() == itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() != itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() < itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() <= itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() >= itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int>::iterator itr;
        v.begin() > itr;
        assertFail("No exception comparing singular iterators.");
    } catch (const ErrorException &) {

    }

    /* Shouldn't be able to do anything with iterators from two different
     * containers.
     */
    try {
        Vector<int> v2;
        v.begin() == v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() != v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() < v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() <= v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() >= v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() > v2.begin();
        assertFail("No exception comparing iterators from different containers.");
    } catch (const ErrorException &) {

    }
    try {
        Vector<int> v2;
        v.begin() - v2.begin();
        assertFail("No exception subtracting iterators from different containers.");
    } catch (const ErrorException &) {

    }
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(VectorTests, iteratorVersionTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v1 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    try {
        for (int n : v1) {
            if (n % 2 == 0) {
                v1.remove(v1.size() - 1);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

TIMED_TEST(VectorTests, mapAllTest_Vector, TEST_TIMEOUT_DEFAULT) {
    Vector<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) {
        total += value;
    });

    assertEqualsInt("mapAll produces correct sum.", std::accumulate(set.begin(), set.end(), 0), total);
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

TIMED_TEST(VectorTests, removeValueTest_Vector, TEST_TIMEOUT_DEFAULT) {
    //                0   1   2   3   4   5   6   7  8
    Vector<int> vec {10, 20, 30, 10, 40, 10, 50, 60, 0};
    vec.removeValue(40);
    vec.removeValue(10);
    vec.removeValue(10);
    vec.removeValue(0);
    Vector<int> exp {20, 30, 10, 50, 60};
    assertEqualsString("after removes", exp.toString(), vec.toString());
}

TIMED_TEST(VectorTests, reverseTest_Vector, TEST_TIMEOUT_DEFAULT) {
    //                0   1   2   3   4   5   6   7   8   9  10
    Vector<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.reverse();
    Vector<int> exp {50, 60, 0, 50, 10, 90, 40, 10, 20, 30, 70};
    assertEqualsString("after reverse", exp.toString(), vec.toString());
}

TIMED_TEST(VectorTests, shuffleTest, TEST_TIMEOUT_DEFAULT) {
    Vector<int> v {10, 20, 30, 40, 50};
    Map<int, Map<int, int> > valueIndexCount;

    // shuffle 100 times
    for (int i = 0; i < 100; i++) {
        v.shuffle();
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

TIMED_TEST(VectorTests, sortTest_Vector, TEST_TIMEOUT_DEFAULT) {
    //                0   1   2   3   4   5   6   7   8   9  10
    Vector<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.sort();
    Vector<int> exp { 0, 10, 10, 20, 30, 40, 50, 50, 60, 70, 90};
    assertEqualsString("after sort", exp.toString(), vec.toString());
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
