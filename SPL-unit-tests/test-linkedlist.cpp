/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "linkedlist.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>



/*
 * Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class LinkedList<int>;
template class LinkedList<std::string>;

PROVIDED_TEST("LinkedList, compare") {
    LinkedList<int> ll1;
    ll1 += 1, 2, 4, 5;
    LinkedList<int> ll2;
    ll2 += 1, 3, 1, 4, 8;
    LinkedList<int> ll3;
    ll3 += 1, 1, 7;
    LinkedList<int> ll4;  // empty
    testCompareOperators(ll1, ll2, LessThan);
    testCompareOperators(ll1, ll3, GreaterThan);
    testCompareOperators(ll1, ll4, GreaterThan);
    testCompareOperators(ll2, ll3, GreaterThan);
    testCompareOperators(ll2, ll4, GreaterThan);
    testCompareOperators(ll3, ll4, GreaterThan);
    testCompareOperators(ll2, ll2, EqualTo);

    Set<LinkedList<int> > sll {ll1, ll2, ll3, ll4};
    EXPECT_EQUAL( "{{}, {1, 1, 7}, {1, 2, 4, 5}, {1, 3, 1, 4, 8}}", sll.toString());
}

PROVIDED_TEST("LinkedList, forEach") {
    LinkedList<int> v1 {1, 2, 3};
    assertCollection("foreach LinkedList", {1, 2, 3}, v1);

    LinkedList<std::string> v2 {"a", "b", "c"};
    std::initializer_list<std::string> list {"a", "b", "c"};
    assertCollection("foreach LinkedList", list, v2);
}

PROVIDED_TEST("LinkedList, hashCode") {
    LinkedList<int> llist;
    llist.add(69);
    llist.add(42);
    EXPECT_EQUAL( hashCode(llist), hashCode(llist));

    LinkedList<int> copy = llist;
    EXPECT_EQUAL( hashCode(llist), hashCode(copy));

    LinkedList<int> empty;
    HashSet<LinkedList<int> > hashllist {llist, copy, empty, empty};
    EXPECT_EQUAL( 2, hashllist.size());
}


PROVIDED_TEST("LinkedList, initializerList") {
    auto list = {60, 70};

    LinkedList<int> llist {10, 20, 30};
    assertCollection("initial", {10, 20, 30}, llist);

    llist += {40, 50};
    assertCollection("after +=", {10, 20, 30, 40, 50}, llist);

    LinkedList<int> copy = llist + list;
    assertCollection("after + (shouldn't modify)", {10, 20, 30, 40, 50}, llist);
    assertCollection("after + copy", {10, 20, 30, 40, 50, 60, 70}, copy);
}

void addDuring(LinkedList<int>& v) { for (int m : v) v.add(0); }
void removeDuring(LinkedList<int>& v) { for (int m : v) v.remove(0); }

PROVIDED_TEST("LinkedList, error on modify during iterate") {
    LinkedList<int> ll {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   EXPECT_ERROR(addDuring(ll));
   EXPECT_ERROR(removeDuring(ll));
}


PROVIDED_TEST("LinkedList, randomElement") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    LinkedList<std::string> llist(list);
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(llist);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0);
    }
}

PROVIDED_TEST("LinkedList, sort") {
    //                    0   1   2   3   4   5   6   7   8   9  10
    LinkedList<int> vec {70, 30, 20, 10, 40, 90, 10, 50,  0, 60, 50};
    vec.sort();
    LinkedList<int> exp { 0, 10, 10, 20, 30, 40, 50, 50, 60, 70, 90};
    EXPECT_EQUAL( exp.toString(), vec.toString());
}
