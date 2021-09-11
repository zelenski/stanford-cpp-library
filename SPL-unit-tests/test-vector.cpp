/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "hashcode.h"
#include "hashset.h"
#include "vector.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <numeric>

/* Force instantiation of Vector on a few types to make sure we didn't miss anything.
 * These types must be comparable and hashable to avoid instantiating functions we
 * can't support.
 */
template class Vector<int>;
template class Vector<bool>;
template class Vector<std::string>;



PROVIDED_TEST("Vector link check") {
    Vector<int> v;
    v.add(0);
    v.remove(0);
    v.insert(0,0);
}

PROVIDED_TEST("Vector add/get") {
    Vector<int> values;
    for (int i = 0; i < 1000; i++) {
        values += i;
    }

    EXPECT_EQUAL(values.size(), 1000);
    EXPECT(!values.isEmpty());

    for (int i = 0; i < 1000; i++) {
        EXPECT_EQUAL(values[i], i);
    }

    /* Mutate things. */
    for (int& elem: values) {
        elem *= 2;
    }

    EXPECT_EQUAL(values.size(), 1000);
    EXPECT(!values.isEmpty());

    for (int i = 0; i < 1000; i++) {
        EXPECT_EQUAL(values[i], 2*i);
    }
}

PROVIDED_TEST("Vector comparision operators") {
    Vector<int> v1 {1, 2, 4, 5};
    Vector<int> v2 {1, 3, 1, 4, 8};
    Vector<int> v3 {1, 1, 7};
    Vector<int> v4 {2, 0};
    Vector<int> v5 {1, 2, 4, 5, 6, 7};
    Vector<int> v6;  // empty
    testCompareOperators(v1, v1, EqualTo);
    testCompareOperators(v1, v2, LessThan);
    testCompareOperators(v1, v3, GreaterThan);
    testCompareOperators(v1, v4, LessThan);
    testCompareOperators(v1, v5, LessThan);
    testCompareOperators(v1, v6, GreaterThan);
    testCompareOperators(v2, v3, GreaterThan);
    testCompareOperators(v2, v4, LessThan);
    testCompareOperators(v2, v5, GreaterThan);
    testCompareOperators(v2, v6, GreaterThan);
    testCompareOperators(v3, v4, LessThan);
    testCompareOperators(v3, v5, LessThan);
    testCompareOperators(v3, v6, GreaterThan);
    testCompareOperators(v4, v5, GreaterThan);
    testCompareOperators(v4, v6, GreaterThan);
    testCompareOperators(v5, v6, GreaterThan);
    testCompareOperators(v6, v6, EqualTo);

    Set<Vector<int> > sv {v1, v2, v3, v4, v5, v6};
    EXPECT_EQUAL("{{}, {1, 1, 7}, {1, 2, 4, 5}, {1, 2, 4, 5, 6, 7}, {1, 3, 1, 4, 8}, {2, 0}}", sv.toString());
}

PROVIDED_TEST("Vector + Vector") {
    Vector<int> lhs =      {  1,  3,  5,  7,  9 };
    Vector<int> rhs =      { 11, 13, 15, 17, 19 };
    auto expected   =      {  1,  3,  5,  7,  9,
                             11, 13, 15, 17, 19 };

    auto lhsBackup  =      {  1,  3,  5,  7,  9 };
    auto rhsBackup  =      { 11, 13, 15, 17, 19 };

    /* Confirm that Vector + Vector works. */
    assertCollection("basic concat", expected, lhs + rhs);
    assertCollection("don't change operands", lhsBackup, lhs);
    assertCollection("don't change operands", rhsBackup, rhs);

    /* Try appending individual values. */
    Vector<int> before = { 1, 3, 5, 7 };
    auto beforeBackup  = { 1, 3, 5, 7 };
    auto after         = { 1, 3, 5, 7, 9 };

    assertCollection("single elem concat", after, before + 9);
    assertCollection("don't change operands", beforeBackup, before);
}

PROVIDED_TEST("Vector for-each loop") {
    Vector<int> v1 {1, 2, 3};
    assertCollection("foreach Vector", {1, 2, 3}, v1);

    Vector<std::string> v2 {"a", "b", "c"};
    std::initializer_list<std::string> list {"a", "b", "c"};
    assertCollection("foreach Vector", list, v2);
}


PROVIDED_TEST("Vector hashCode") {
    Vector<int> v;
    v.add(69);
    v.add(42);
    EXPECT_EQUAL( hashCode(v), hashCode(v));

    Vector<int> copy = v;
    EXPECT_EQUAL( hashCode(v), hashCode(copy));

    Vector<int> empty;
    HashSet<Vector<int> > hashvec {v, copy, empty, empty};
    EXPECT_EQUAL( 2, hashvec.size());
}


PROVIDED_TEST("Vector initializer list constructor") {
    auto list = {60, 70};

    Vector<int> vec {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, vec);

    vec += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, vec);

    Vector<int> copy = vec + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, vec);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);
}

PROVIDED_TEST("Vector insert") {
    /* Confirm that insert works at all the right positions, and no other positions. */
    static const int kNumElems = 137;

    for (int i = 0; i <= kNumElems; i++) {
        Vector<int> values;
        for (int j = 0; j < kNumElems; j++) {
            values += j;
        }
        EXPECT_EQUAL(values.size(), kNumElems);

        values.insert(i, kNumElems + 1);
        EXPECT_EQUAL(values.size(), kNumElems +1);

        /* Confirm all the values are correct. */
        for (int j = 0; j < i; j++) {
            EXPECT_EQUAL(values[j], j);
        }
        EXPECT_EQUAL(values[i], kNumElems + 1);
        for (int j = i + 1; j < values.size(); j++) {
            EXPECT_EQUAL(values[j], j - 1);
        }
        if (i == kNumElems) EXPECT_ERROR(values.insert(values.size()+1, values.size()+1));
    }
    Vector<int> v; // empty
    EXPECT_ERROR(v.insert(1, 1));
    v.insert(0, 0);
}

PROVIDED_TEST("Vector remove") {
    static const int kNumElems = 137;
    Vector<int> values;

    for (int i = 0; i < kNumElems; i++) {
        values += i;
    }
    EXPECT_EQUAL(values.size(), kNumElems);

    values.remove(0);
    EXPECT_EQUAL(values.size(), kNumElems - 1);

    /* Confirm all the values are correct. */
    for (int j = 0; j < values.size(); j++) {
        EXPECT_EQUAL(values[j], j + 1);
    }
    EXPECT_ERROR(values.remove(999));
    values.insert(0, 0);
    for (int i = values.size() - 1; i >= 0; i -= 2) {
        EXPECT_EQUAL(i, values.remove(i));
    }
    while (!values.isEmpty()) {
        int sz = values.size();
        values[0] = sz;
        EXPECT(values.remove(0) == sz);
    }
    EXPECT_ERROR(values.remove(0));
}

PROVIDED_TEST("Vector iterator, const iterator") {
    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto& cv = v;

    EXPECT(v.begin() == cv.begin());
    EXPECT(v.end() == cv.end());
}

PROVIDED_TEST("Vector out of bounds detection") {
    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto itr = v.begin();

    /* Should fail if we try to back up past beginning. */
    itr = v.begin();
    EXPECT_ERROR(--itr);

    /* Should fail if we try to advance past end. */
    itr = v.end();
    EXPECT_ERROR(++itr);

    /* Should fail if we try to use random access to jump out of range. */
    itr = v.begin();
    EXPECT_ERROR(itr[137] = 42);

    /* Should NOT fail if we try to use random access to jump into range. */
    itr = v.begin();
    itr[9] = 137;

    itr = v.end();
    itr[-10] = 42;

    Vector<int>::iterator single;
    /* Should fail doing ANYTHING except comparisons with a singular iterator. */
    EXPECT_ERROR(single++);
    EXPECT_ERROR(++single);
    EXPECT_ERROR(single--);
    EXPECT_ERROR(--single);
    EXPECT_ERROR(*single);
    EXPECT_ERROR(single - v.begin());
    EXPECT_ERROR(v.begin() - single);

    Vector<int> v2;
   /* Shouldn't be able to compare iterators from two different
     * containers.
     */
    EXPECT_ERROR(v.begin() == v2.begin());
    EXPECT_ERROR(v.begin() != v2.begin());
    EXPECT_ERROR(v.begin() < v2.begin());
    EXPECT_ERROR(v.begin() <= v2.begin());
    EXPECT_ERROR(v.begin() >= v2.begin());
    EXPECT_ERROR(v.begin() > v2.begin());
    EXPECT_ERROR(v.begin() - v2.begin());
}

void addDuring(Vector<int>& v) { for (int m : v) v.add(0); }
void removeDuring(Vector<int>& v) { for (int m : v) v.remove(0); }

void modify(Vector<int> &v) {v.remove(0);}
PROVIDED_TEST("Vector error if modify during iteration") {
    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_ERROR(addDuring(v));
    EXPECT_ERROR(removeDuring(v));
    EXPECT_ERROR(clearDuringIteration(v));
  //  EXPECT_ERROR(doDuringIteration(v, [=]() { v.clear(); }));
}

PROVIDED_TEST("Vector mapAll") {
    Vector<int> set {7, 5, 1, 2, 8};

    int total = 0;
    set.mapAll([&] (int value) { total += value;});
    EXPECT_EQUAL(total, std::accumulate(set.begin(), set.end(), 0));
}

PROVIDED_TEST("Vector randomElement") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Vector<std::string> v(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(v);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0); //  each element chosen a non-zero number of times
    }
}


PROVIDED_TEST("Vector sort") {
    //                0   1   2   3   4   5   6   7   8   9  10
    Vector<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.sort();
    Vector<int> exp { 0, 10, 10, 20, 30, 40, 50, 50, 60, 70, 90};
    EXPECT_EQUAL( exp.toString(), vec.toString());
}

PROVIDED_TEST("Vector stream extraction") {
    std::istringstream vstream("{1, 2, 3}");
    Vector<int> v;
    Vector<int> exp {1, 2, 3};
    vstream >> v;
    EXPECT_EQUAL(v, exp);
    std::istringstream vstreambad("1, 2, 3}");
    bool result = bool(vstreambad >> v);
    EXPECT(!result);
}

PROVIDED_TEST("Vector sublist") {
    Vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    /* Brute-force check all two-arg sublists. */
    for (int start = 0; start <= values.size(); start++) {
        for (int length = 0; length + start <= values.size(); length++) {
            Vector<int> sub = values.subList(start, length);
            EXPECT_EQUAL(length, sub.size());
            for (int i = 0; i < length; i++) {
                EXPECT_EQUAL(i + start, sub[i]);
            }
        }
    }

    /* Brute-force check all one-arg sublists. */
    for (int start = 0; start <= values.size(); start++) {
        Vector<int> sub = values.subList(start);
        EXPECT_EQUAL(values.size() - start, sub.size());
        for (int i = 0; i < sub.size(); i++) {
            EXPECT_EQUAL(i + start, sub[i]);
        }
    }
}
