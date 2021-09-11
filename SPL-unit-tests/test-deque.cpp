/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "deque.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

/*
 * Force instantiation of Deque on a few types to make sure we didn't miss anything.
 * These types must be comparable and hashable to avoid instaniating functions we
 * can't support.
 */
template class Deque<int>;
template class Deque<bool>;
template class Deque<std::string>;

PROVIDED_TEST("Deque, compare") {
    Deque<std::string> deq;
    deq.enqueueBack("a");
    deq.enqueueBack("b");
    deq.enqueueBack("c");
    Deque<std::string> deq2;
    deq2.enqueueBack("a");
    deq2.enqueueBack("ab");
    deq2.enqueueBack("bc");
    Deque<std::string> deq3;
    testCompareOperators(deq, deq2, GreaterThan);
    testCompareOperators(deq2, deq, LessThan);
    testCompareOperators(deq, deq, EqualTo);
    Set<Deque<std::string> > sdeque {deq, deq2, deq3};
    EXPECT_EQUAL( "{{}, {\"a\", \"ab\", \"bc\"}, {\"a\", \"b\", \"c\"}}", sdeque.toString());
}

PROVIDED_TEST("Deque, forEach") {
    Deque<int> deq;
    deq.enqueueBack(1);
    deq.enqueueFront(2);
    deq.enqueueBack(3);
    deq.enqueueFront(4);

    assertCollection("Deque foreach", {4, 2, 1, 3}, deq);
}

PROVIDED_TEST("Deque, frontBack") {
    Deque<int> deq {10, 20, 30, 40};
    EXPECT_EQUAL( 10, deq.peekFront());
    EXPECT_EQUAL( 40, deq.peekBack());
}

PROVIDED_TEST("Deque, hashCode") {
    Deque<int> deq;
    deq.enqueueBack(1);
    deq.enqueueFront(2);
    deq.enqueueBack(3);
    deq.enqueueFront(4);
    EXPECT_EQUAL( hashCode(deq), hashCode(deq));

    Deque<int> copy = deq;
    EXPECT_EQUAL( hashCode(deq), hashCode(copy));

    Deque<int> empty;

    HashSet<Deque<int> > hashdeq {deq, copy, empty, empty};
    EXPECT_EQUAL( 2, hashdeq.size());
}

PROVIDED_TEST("Deque, initializerList") {
    Deque<int> deque {10, 20, 30};
    EXPECT_EQUAL( "{10, 20, 30}", deque.toString());
}

void addDuring(Deque<int>& v) { for (int m : v) v.enqueueBack(0); }
void removeDuring(Deque<int>& v) { for (int m : v) v.dequeueBack(); }

PROVIDED_TEST("Deque, error on modify during iterate") {
    Deque<int> deque {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   EXPECT_ERROR(addDuring(deque));
   EXPECT_ERROR(removeDuring(deque));
}

