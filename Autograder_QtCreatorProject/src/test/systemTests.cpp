/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */

#include "testcases.h"
#include "assertions.h"
#include "gtest-marty.h"
#include "ioutils.h"
#include <iomanip>
#include <iostream>
#include <string>
//using namespace std;

#define TEST_TIMEOUT_DEFAULT 3000

TEST_CATEGORY(SystemTests, "system tests");

TIMED_TEST(SystemTests, callExitTest, TEST_TIMEOUT_DEFAULT) {
    // lolol, I exit
    // std::exit(0);
}


TEST_CATEGORY(AssertionTests, "assertion tests");

TIMED_TEST(AssertionTests, failFirstPassSecond, TEST_TIMEOUT_DEFAULT * 100) {
    assertEqualsInt("1st test - should fail", 42, 17);
    assertEqualsInt("2nd test - should pass", 42, 42);
}

TIMED_TEST(AssertionTests, passFirstFailSecond, TEST_TIMEOUT_DEFAULT) {
    assertEqualsInt("1st test - should pass", 42, 42);
    assertEqualsInt("2nd test - should fail", 42, 17);
}

TIMED_TEST(AssertionTests, failFirstPassSecondFailThird, TEST_TIMEOUT_DEFAULT) {
    assertEqualsInt("1st test - should fail", 42, 17);
    assertEqualsInt("2nd test - should pass", 42, 42);
    assertEqualsInt("3rd test - should fail", 49, 19);
}

TIMED_TEST(AssertionTests, passFirstFailSecondPassThird, TEST_TIMEOUT_DEFAULT) {
    assertEqualsInt("1st test - should pass", 42, 42);
    assertEqualsInt("2nd test - should fail", 42, 17);
    assertEqualsInt("3rd test - should pass", 49, 49);
}

TIMED_TEST(AssertionTests, coutFlagsTest1, TEST_TIMEOUT_DEFAULT) {
    ioutils::captureStdoutBegin();
    std::cout << std::fixed << std::setprecision(2) << 12.3456789 << std::endl;
    std::string output = ioutils::captureStdoutEnd();
    assertEqualsString("console output #1", "12.35\n", output);
}

TIMED_TEST(AssertionTests, coutFlagsTest2, TEST_TIMEOUT_DEFAULT) {
    ioutils::captureStdoutBegin();
    std::cout << 12.3456789 << std::endl;
    std::string output = ioutils::captureStdoutEnd();
    assertEqualsString("console output #2", "12.3457\n", output);
}

TIMED_TEST(AssertionTests, assertPointerPass, TEST_TIMEOUT_DEFAULT) {
    int* p1 = new int(42);
    int* p2 = p1;
    assertEqualsPointer("1st test - should pass", p1, p2);
}

TIMED_TEST(AssertionTests, assertPointerFail, TEST_TIMEOUT_DEFAULT) {
    int* p1 = new int(42);
    int* p2 = new int(42);
    assertEqualsPointer("2nd test - should fail", p1, p2);
}
