/*
 * File: assertions.h
 * ------------------
 * This file declares a series of JUnit-like assert macros.
 * These are meant to work similarly to the ones built into GoogleTest
 * but with more JUnit-like syntax.
 * These macros also alert our graphical unit test runner UI so that it
 * can show the results in the GUI for the user.
 * 
 * @author Marty Stepp
 * @version 2016/08/02
 * - added assertEqualsImage
 * @version 2014/11/24
 * - initial version
 * @since 2014/11/24
 */

#ifndef _assertions_h
#define _assertions_h

#include "gtest.h"

#define TEST_FAIL_PREFIX std::string("Test case failed: ")
#define FAIL_PREFIX      std::string("Assertion failed: ")

#define assertDiff(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_DIFF, \
        msg, a, b, "diff", a == b)); \
    EXPECT_EQ(a, b)

#define assertDiffPass(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_DIFF, \
        msg, a, b, "diff", true));

// general assertEquals macro (better to use assertEqualsFoo for various types)
#define assertEquals(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, a, b, "T", a == b)); \
    EXPECT_EQ(a, b)

// 'quiet' version, only sends to backend/GUI if failed for efficiency
// (needed when doing a ton of assertions in one program)
#define assertEqualsQ(msg, a, b) \
    if (a != b) { \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, a, b, "T", a == b)); \
    } \
    EXPECT_EQ(a, b)

// hard-failure on assertion fail
#define assertEqualsF(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, a, b, "T", a == b)); \
    ASSERT_EQ(a, b)

#define assertEqualsFQ(msg, a, b) \
    if (a != b) { \
        autograder::setFailDetails(autograder::UnitTestDetails( \
            autograder::UnitTestType::TEST_ASSERT_EQUALS, \
            msg, a, b, "T", a == b)); \
    } \
    ASSERT_EQ(a, b)

#define assertEqualsString(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, a, b, "string", a == b)); \
    EXPECT_EQ(a, b)

#define assertEqualsCString(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, a, b, "string", std::string(a) == std::string(b))); \
    EXPECT_EQ(a, b)

#define assertEqualsInt(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, integerToString(a), integerToString(b), "int", a == b)); \
    EXPECT_EQ(a, b)

#define assertEqualsBool(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_EQUALS, \
        msg, boolToString(a), boolToString(b), "bool", a == b)); \
    EXPECT_EQ(a, b)

#define assertEqualsDouble(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NEAR, \
        msg, realToString(a), realToString(b), "double", \
        (a-b) <= 0.001 && (a-b) >= -0.001)); \
    EXPECT_NEAR(a, b, 0.001)

#define assertNotEquals(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NOT_EQUALS, \
        msg, a, b, "T", a != b)); \
    EXPECT_NE(a, b)

#define assertNotEqualsString(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NOT_EQUALS, \
        msg, a, b, "string", a != b)); \
    EXPECT_NE(a, b)

#define assertNotEqualsInt(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NOT_EQUALS, \
        msg, integerToString(a), integerToString(b), "int", a != b)); \
    EXPECT_NE(a, b)

#define assertNotEqualsBool(msg, a, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NOT_EQUALS, \
        msg, boolToString(a), boolToString(b), "bool", a != b)); \
    EXPECT_NE(a, b)

#define assertDoubleNear(msg, a, b, t) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_NEAR, \
        msg, realToString(a), realToString(b), "double", \
        (a-b) <= t && (a-b) >= -t)); \
    EXPECT_NEAR(a, b, t)
#define assertNear assertDoubleNear

#define assertTrue(msg, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_TRUE, \
        msg, boolToString(true), boolToString(b), "bool", \
        b)); \
    EXPECT_TRUE(b)

#define assertFalse(msg, b) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_ASSERT_FALSE, \
        msg, boolToString(false), boolToString(b), "bool", \
        !b)); \
    EXPECT_FALSE(b)

#define assertFail(msg) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_FAIL, \
        msg)); \
    GTEST_FATAL_FAILURE_(msg.c_str())

#define assertFailQuiet() \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_FAIL, \
        "")); \
    GTEST_FATAL_FAILURE_("")

#define assertPass(msg) \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_PASS, \
        msg, true)); \
    GTEST_SUCCESS_(msg)

#define assertPassQuiet() \
    autograder::setFailDetails(autograder::UnitTestDetails( \
        autograder::UnitTestType::TEST_PASS, \
        "", true)); \
    GTEST_SUCCESS_("")

#define assertThrowsAny(msg, stmt)           EXPECT_ANY_THROW(stmt)
#define assertThrows(msg, stmt, ex)          EXPECT_THROW(stmt, ex)
#define assertNotThrows(msg, stmt, ex)       EXPECT_NO_THROW(stmt)

void assertEqualsImage(const std::string& msg,
                       const std::string& imagefile1,
                       const std::string& imagefile2);

#endif // _assertions_h
