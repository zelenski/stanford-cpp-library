/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "queue.h"
#include "hashcode.h"
#include "hashset.h"
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
template class Queue<int>;
template class Queue<std::string>;

PROVIDED_TEST("Queue, compare") {
    Queue<int> q1;
    q1.enqueue(1);
    q1.enqueue(2);
    q1.enqueue(1);
    q1.enqueue(5);
    Queue<int> q2;
    q2.enqueue(1);
    q2.enqueue(2);
    q2.enqueue(1);
    q2.enqueue(4);
    q2.enqueue(7);
    testCompareOperators(q1, q2, GreaterThan);
    testCompareOperators(q2, q1, LessThan);
    testCompareOperators(q1, q1, EqualTo);

    Set<Queue<int> > sq {q1, q2};
    EXPECT_EQUAL( "{{1, 2, 1, 4, 7}, {1, 2, 1, 5}}", sq.toString());
}

PROVIDED_TEST("Queue, forEach") {
    Queue<std::string> q;
    q.enqueue("a");
    q.enqueue("bb");
    q.enqueue("c");
    q.enqueue("ddd");
    q.enqueue("e");
    q.enqueue("ffff");

    Queue<std::string> expected {"a", "bb", "c", "ddd", "e", "ffff"};
    while (!q.isEmpty()) {
        EXPECT_EQUAL( expected.dequeue(), q.dequeue());
    }
}

PROVIDED_TEST("Queue, hashCode") {
    Queue<std::string> queue;
    queue.enqueue("a");
    queue.enqueue("bb");
    queue.enqueue("c");
    queue.enqueue("ddd");
    queue.enqueue("e");
    queue.enqueue("ffff");
    EXPECT_EQUAL( hashCode(queue), hashCode(queue));

    Queue<std::string> copy = queue;
    EXPECT_EQUAL( hashCode(queue), hashCode(copy));

    Queue<std::string> empty;

    HashSet<Queue<std::string> > hashq {queue, copy, empty, empty};
    EXPECT_EQUAL( 2, hashq.size());
}

PROVIDED_TEST("Queue, initializerList") {
    Queue<int> queue {10, 20, 30};
    EXPECT_EQUAL( "{10, 20, 30}", queue.toString());
}

PROVIDED_TEST("Queue, peekEnqueueBug") {
    {
        Queue<int> queue {10, 20, 30, 40, 50, 60, 70, 80};
        EXPECT_EQUAL( "{10, 20, 30, 40, 50, 60, 70, 80}", queue.toString());
        queue.enqueue(queue.peek());
        EXPECT_EQUAL( "{10, 20, 30, 40, 50, 60, 70, 80, 10}", queue.toString());
    }
    {
        Queue<int> queue {10, 20, 30, 40, 50, 60, 70, 80, 90};
        EXPECT_EQUAL( "{10, 20, 30, 40, 50, 60, 70, 80, 90}", queue.toString());
        queue.enqueue(queue.peek());
        EXPECT_EQUAL( "{10, 20, 30, 40, 50, 60, 70, 80, 90, 10}", queue.toString());
    }
}
