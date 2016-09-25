/*
 * Test file for verifying the Stanford C++ lib string lib functionality.
 */

#include "testcases.h"
#include "assertions.h"
#include "gtest-marty.h"
#include "strlib.h"
#include <iostream>
#include <string>
//using namespace std;

#define TEST_TIMEOUT_DEFAULT 3000

TEST_CATEGORY(StringTests, "string tests");

TIMED_TEST(StringTests, stringToIntegerTest, TEST_TIMEOUT_DEFAULT) {
    assertEqualsInt("base-10", 234, stringToInteger("234"));
    assertEqualsInt("base-8", 156, stringToInteger("234", /* radix */ 8));
    assertEqualsInt("base-16", 564, stringToInteger("234", /* radix */ 16));
}

TIMED_TEST(StringTests, stringToIntegerInvalidRadixTest, TEST_TIMEOUT_DEFAULT) {
    assertThrows("", stringToInteger("234", /* radix */ 0);, ErrorException);
}
