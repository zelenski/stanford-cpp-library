/*
 * File: gtest-marty.h
 * -------------------
 * This file contains declarations of utility code to augment the functionality
 * of the Google Test C++ unit testing framework.
 * 
 * @author Marty Stepp
 * @version 2014/10/31
 * - added code for graphical test runner UI
 * @version 2014/10/14
 * - moved some code out to diff.h and stylecheck.h
 */

#ifndef _gtest_marty_h
#define _gtest_marty_h

#include <string>
#include "gtest.h"
#include "map.h"
#include "set.h"
#include "strlib.h"
#include "timer.h"

namespace autograder {

class AutograderTest : public testing::Test {
public:
    static const int DEFAULT_TIMEOUT_MS;
    
    virtual int getTestTimeout() const;
    virtual void setTestTimeout(int ms);
    virtual void SetUp();
    virtual void TearDown();
    virtual void TestBody();   // override me

private:
    int timeoutMS;
};

// NOTE: must keep in sync with UNIT_TEST_TYPE_NAMES in gtest-marty.cpp
enum UnitTestType {
    TEST_ASSERT_EQUALS = 0,
    TEST_ASSERT_NOT_EQUALS,
    TEST_ASSERT_NEAR,
    TEST_ASSERT_DIFF,
    TEST_ASSERT_TRUE,
    TEST_ASSERT_FALSE,
    TEST_EXCEPTION,
    TEST_NOT_EXCEPTION,
    TEST_PASS,
    TEST_FAIL,
    TEST_STYLE_CHECK,
};

struct UnitTestDetails {
public:
    UnitTestType testType;
    std::string message;
    std::string expected;
    std::string student;
    std::string valueType;
    bool passed;
    
    UnitTestDetails();
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg = "",
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    const std::string& exp,
                    const std::string& stu,
                    const std::string& vtype,
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    bool exp,
                    bool stu,
                    const std::string& vtype,
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    char exp,
                    char stu,
                    const std::string& vtype,
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    double exp,
                    double stu,
                    const std::string& vtype,
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    int exp,
                    int stu,
                    const std::string& vtype,
                    bool pass = false);
    
    UnitTestDetails(autograder::UnitTestType tp,
                    const std::string& msg,
                    const char* const exp,
                    const char* const stu,
                    const std::string& vtype,
                    bool pass = false);
    
    std::string toString() const;
};
std::ostream& operator <<(std::ostream& out, const UnitTestDetails& deets);

/*
 * A class to customize the Google Test output, mostly to make it more concise.
 */
class MartyTestResultPrinter : public ::testing::EmptyTestEventListener {
public:
    static void setFailDetails(const UnitTestDetails& deets);

    MartyTestResultPrinter();

    /*
     * Called before a test starts.
     */
    virtual void OnTestStart(const ::testing::TestInfo& test_info);

    /*
     * Called after a failed assertion or a SUCCEED() invocation.
     */
    virtual void OnTestPartResult(const ::testing::TestPartResult& test_part_result);

    /*
     * Called after a test ends.
     */
    virtual void OnTestEnd(const ::testing::TestInfo& test_info);

    /*
     * Called after all test cases are done.
     */
    virtual void OnTestProgramEnd(const ::testing::UnitTest& unit_test);

    /*
     * Sets the number of assertion failures that will be printed per test case.
     * Used to avoid massively long output for repeated failures.
     * If a value <= 0 is passed, there is no limit.
     */
    void setFailsToPrintPerTest(int fails);
    
    int getTestNameWidth() const;
    void setTestNameWidth(int width);

private:
    static std::string failMessage;
    bool testInProgress;
    int failCountThisTest;
    int testCount;
    int failCountToPrintPerTest;
    int testNameWidth;
    Map<std::string, bool> testsReported;
};

/*
 * A class that shows the Google Test output in a pretty graphical window.
 */
class MartyGraphicalTestResultPrinter : public ::testing::EmptyTestEventListener {
public:
    static const int TEST_RUNTIME_MIN_TO_DISPLAY_MS;
    static void setFailDetails(const UnitTestDetails& deets);

    MartyGraphicalTestResultPrinter();

    /*
     * Called before a test starts.
     */
    virtual void OnTestStart(const ::testing::TestInfo& test_info);

    /*
     * Called after a failed assertion or a SUCCEED() invocation.
     */
    virtual void OnTestPartResult(const ::testing::TestPartResult& test_part_result);

    /*
     * Called after a test ends.
     */
    virtual void OnTestEnd(const ::testing::TestInfo& test_info);

    /*
     * Called after all test cases are done.
     */
    virtual void OnTestProgramEnd(const ::testing::UnitTest& unit_test);

private:
    void ensureCurrentTestAdded();
};

} // namespace autograder

#define TEST_FAIL_PREFIX std::string("Test case failed: ")
#define FAIL_PREFIX      std::string("Assertion failed: ")

/*
 * A series of JUnit-like assert macros.
 */

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
    GTEST_FATAL_FAILURE_(msg)

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
#define assertCStringEquals(msg, a, b)       EXPECT_STREQ(a, b)
#define assertCStringNotEquals(msg, a, b)    EXPECT_STRNE(a, b)

#endif
