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

TIMED_TEST(StringTests, diffOutputFailTest, TEST_TIMEOUT_DEFAULT) {
    std::cout << "hi\thow\tare\tyou?" << std::endl;
    std::string exp = "Hi\nyo\nbye\nhow are you\nok\nthe end";
    std::string stu = std::string("Hi\nOOPS\nbye\nhow\t are") + '\0' + std::string(" you\\\nDOH\nthe end");
    assertDiff("program output", exp, stu);
}

TIMED_TEST(StringTests, stringToIntegerTest, TEST_TIMEOUT_DEFAULT) {
    assertEqualsInt("base-10", 234, stringToInteger("234"));
    assertEqualsInt("base-8", 156, stringToInteger("234", /* radix */ 8));
    assertEqualsInt("base-16", 564, stringToInteger("234", /* radix */ 16));
}

TIMED_TEST(StringTests, stringWithWeirdCharsTest, TEST_TIMEOUT_DEFAULT) {
    std::string s1 = "hello  there";
    std::string s2 = "hello\t";
    s2 += '\0';
    s2 += " there";
    s2 += '\377';    // octal for 255
    s2 += "\r\n";
    assertEqualsString("string w/ weird chars", s1, s2);
}

TIMED_TEST(StringTests, stringToIntegerInvalidRadixTest, TEST_TIMEOUT_DEFAULT) {
    assertThrows("expect exception on converting stringToInteger with radix 0", stringToInteger("234", /* radix */ 0);, ErrorException);
}
