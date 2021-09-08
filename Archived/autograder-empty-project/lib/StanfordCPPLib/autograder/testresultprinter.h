/*
 * File: testresultprinter.h
 * -------------------------
 * This file contains declarations of classes used by GoogleTest to print and
 * format test case results differently than GTest's default format.
 * 
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#ifndef _testresultprinter_h
#define _testresultprinter_h

#define INTERNAL_INCLUDE 1
#include "autogradertest.h"
#define INTERNAL_INCLUDE 1
#include "gtest.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace autograder {

/*
 * A class to customize the Google Test output, mostly to make it more concise.
 */
class MartyTestResultPrinter : public ::testing::EmptyTestEventListener {
public:
    static void setFailDetails(AutograderTest& test, const UnitTestDetails& deets);
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
    
    /*
     * Number of characters in the test case's name.
     */
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
    static void setFailDetails(AutograderTest& test, const UnitTestDetails& deets);
    static void setFailDetails(const UnitTestDetails& deets);

    /*
     * Called before a test starts.
     */
    virtual void OnTestStart(const ::testing::TestInfo& test_info);

    /*
     * Called after a test ends.
     */
    virtual void OnTestEnd(const ::testing::TestInfo& test_info);
};

} // namespace autograder
} // namespace stanfordcpplib

#endif // _testresultprinter_h
