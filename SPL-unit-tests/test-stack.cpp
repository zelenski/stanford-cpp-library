/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "stack.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>



/* Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class Stack<int>;
template class Stack<std::string>;

PROVIDED_TEST("Stack push, compare") {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.push(1);
    s1.push(5);
    Stack<int> s2;
    s2.push(1);
    s2.push(2);
    s2.push(1);
    s2.push(4);
    s2.push(7);
    Stack<int> s3; // empty
    testCompareOperators(s1, s2, GreaterThan);
    testCompareOperators(s2, s1, LessThan);
    testCompareOperators(s1, s1, EqualTo);
    testCompareOperators(s3, s1, LessThan);
    testCompareOperators(s1, s1, EqualTo);

    Set<Stack<int> > ss {s1, s2};
    EXPECT_EQUAL("{{1, 2, 1, 4, 7}, {1, 2, 1, 5}}", ss.toString());
}

PROVIDED_TEST("Stack peek/pop") {
    Stack<int> s;

    EXPECT(s.isEmpty());
    for (int i = 1; i <= 100; i++) {
        s.push(i);
    }
    while (!s.isEmpty()) {
        int top = s.peek();
        EXPECT_EQUAL(top, s.size());
        EXPECT_EQUAL(top, s.pop());
    }
}

PROVIDED_TEST("Stack hashcode") {
    Stack<int> stack;
    stack.push(69);
    stack.push(42);
    EXPECT_EQUAL(hashCode(stack), hashCode(stack));

    Stack<int> copy = stack;
    EXPECT_EQUAL(hashCode(stack), hashCode(copy));

    Stack<int> empty;
    HashSet<Stack<int> > hashstack {stack, copy, empty, empty};
    EXPECT_EQUAL(2, hashstack.size());
}

PROVIDED_TEST("Stack initializer_list") {
    Stack<int> stack {10, 20, 30};
    EXPECT_EQUAL( "{10, 20, 30}", stack.toString());
}

