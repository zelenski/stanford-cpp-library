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

TEST_CATEGORY(StackTests, "Stack tests");

/* Force instantiation of the template on a type to ensure that we don't have anything
 * invidious lurking that just didn't get compiled.
 */
template class Stack<int>;
template class Stack<std::string>;

TIMED_TEST(StackTests, compareTest_Stack, TEST_TIMEOUT_DEFAULT) {
    Stack<int> s1;
    s1.add(1);
    s1.add(2);
    s1.add(1);
    s1.add(5);
    Stack<int> s2;
    s2.add(1);
    s2.add(2);
    s2.add(1);
    s2.add(4);
    s2.add(7);
    compareTestHelper(s1, s2, "Stack", /* compareTo */  1);
    compareTestHelper(s2, s1, "Stack", /* compareTo */ -1);
    compareTestHelper(s1, s1, "Stack", /* compareTo */  0);

    Set<Stack<int> > ss {s1, s2};
    assertEqualsString("ss", "{{1, 2, 1, 4, 7}, {1, 2, 1, 5}}", ss.toString());
}

TIMED_TEST(StackTests, hashCodeTest_Stack, TEST_TIMEOUT_DEFAULT) {
    Stack<int> stack;
    stack.add(69);
    stack.add(42);
    assertEqualsInt("hashcode of self Stack", hashCode(stack), hashCode(stack));

    Stack<int> copy = stack;
    assertEqualsInt("hashcode of copy Stack", hashCode(stack), hashCode(copy));

    Stack<int> empty;
    HashSet<Stack<int> > hashstack {stack, copy, empty, empty};
    assertEqualsInt("hashset of Stack size", 2, hashstack.size());
}

TIMED_TEST(StackTests, initializerListTest_Stack, TEST_TIMEOUT_DEFAULT) {
    Stack<int> stack {10, 20, 30};
    assertEqualsString("init list Stack", "{10, 20, 30}", stack.toString());
}

