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

TEST_CATEGORY(StackTests, "Stack tests");

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
    HashSet<Stack<int> > hashstack;
    Stack<int> s;
    s.add(69);
    s.add(42);
    hashstack.add(s);
    std::cout << "hashset of stack: " << hashstack << std::endl;
}

TIMED_TEST(StackTests, initializerListTest_Stack, TEST_TIMEOUT_DEFAULT) {
    Stack<int> stack {10, 20, 30};
    std::cout << "init list Stack = " << stack << std::endl;
}
