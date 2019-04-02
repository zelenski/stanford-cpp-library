/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "linkedlist.h"
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

TEST_CATEGORY(LinkedListTests, "LinkedList tests");

/*
 * Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class LinkedList<int>;
template class LinkedList<std::string>;

TIMED_TEST(LinkedListTests, compareTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> ll1;
    ll1 += 1, 2, 4, 5;
    LinkedList<int> ll2;
    ll2 += 1, 3, 1, 4, 8;
    LinkedList<int> ll3;
    ll3 += 1, 1, 7;
    LinkedList<int> ll4;  // empty
    compareTestHelper(ll1, ll2, "LinkedList", /* compareTo */ -1);
    compareTestHelper(ll1, ll3, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll1, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll3, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll3, ll4, "LinkedList", /* compareTo */ 1);
    compareTestHelper(ll2, ll2, "LinkedList", /* compareTo */ 0);

    Set<LinkedList<int> > sll {ll1, ll2, ll3, ll4};
    assertEqualsString("sll", "{{}, {1, 1, 7}, {1, 2, 4, 5}, {1, 3, 1, 4, 8}}", sll.toString());
}

TIMED_TEST(LinkedListTests, forEachTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> v1 {1, 2, 3};
    assertCollection("foreach LinkedList", {1, 2, 3}, v1);

    LinkedList<std::string> v2 {"a", "b", "c"};
    std::initializer_list<std::string> list {"a", "b", "c"};
    assertCollection("foreach LinkedList", list, v2);
}

TIMED_TEST(LinkedListTests, hashCodeTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> llist;
    llist.add(69);
    llist.add(42);
    assertEqualsInt("hashcode of self LinkedList", hashCode(llist), hashCode(llist));

    LinkedList<int> copy = llist;
    assertEqualsInt("hashcode of copy LinkedList", hashCode(llist), hashCode(copy));

    LinkedList<int> empty;
    HashSet<LinkedList<int> > hashllist {llist, copy, empty, empty};
    assertEqualsInt("hashset of LinkedList size", 2, hashllist.size());
}

TIMED_TEST(LinkedListTests, indexOfTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    //                    0   1   2   3   4   5   6   7  8
    LinkedList<int> vec {10, 20, 30, 10, 40, 10, 50, 60, 0};
    assertEqualsInt("indexOf 10", 0, vec.indexOf(10));
    assertEqualsInt("indexOf 20", 1, vec.indexOf(20));
    assertEqualsInt("indexOf 50", 6, vec.indexOf(50));

    assertEqualsInt("lastIndexOf 10", 5, vec.lastIndexOf(10));
    assertEqualsInt("lastIndexOf 20", 1, vec.lastIndexOf(20));
    assertEqualsInt("lastIndexOf 0",  8, vec.lastIndexOf(0));

    assertEqualsBool("contains 10", true, vec.contains(10));
    assertEqualsBool("contains 20", true, vec.contains(20));
    assertEqualsBool("contains 50", true, vec.contains(50));
    assertEqualsBool("contains 70", false, vec.contains(70));
    assertEqualsBool("contains -1", false, vec.contains(-1));
    assertEqualsBool("contains 99", false, vec.contains(99));
}

TIMED_TEST(LinkedListTests, initializerListTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    auto list = {60, 70};

    LinkedList<int> llist {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, llist);

    llist += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, llist);

    LinkedList<int> copy = llist + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, llist);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(LinkedListTests, iteratorVersionTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> ll {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    try {
        for (int n : ll) {
            if (n % 2 == 0) {
                ll.remove(ll.size() - 1);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

TIMED_TEST(LinkedListTests, randomElementTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    LinkedList<std::string> llist(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(llist);
        counts[s]++;
    }

    for (const std::string& s : list) {
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}

TIMED_TEST(LinkedListTests, removeValueTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    //                    0   1   2   3   4   5   6   7  8
    LinkedList<int> vec {10, 20, 30, 10, 40, 10, 50, 60, 0};
    vec.removeValue(40);
    vec.removeValue(10);
    vec.removeValue(10);
    vec.removeValue(0);
    LinkedList<int> exp {20, 30, 10, 50, 60};
    assertEqualsString("after removes", exp.toString(), vec.toString());
}

TIMED_TEST(LinkedListTests, reverseTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    //                    0   1   2   3   4   5   6   7   8   9  10
    LinkedList<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.reverse();
    LinkedList<int> exp {50, 60, 0, 50, 10, 90, 40, 10, 20, 30, 70};
    assertEqualsString("after reverse", exp.toString(), vec.toString());
}

TIMED_TEST(LinkedListTests, shuffleTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    LinkedList<int> v {10, 20, 30, 40, 50};
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

TIMED_TEST(LinkedListTests, sortTest_LinkedList, TEST_TIMEOUT_DEFAULT) {
    //                    0   1   2   3   4   5   6   7   8   9  10
    LinkedList<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.sort();
    LinkedList<int> exp { 0, 10, 10, 20, 30, 40, 50, 50, 60, 70, 90};
    assertEqualsString("after sort", exp.toString(), vec.toString());
}
