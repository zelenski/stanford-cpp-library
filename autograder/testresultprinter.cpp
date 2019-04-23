/*
 * File: testresultprinter.cpp
 * ---------------------------
 * This file contains implementation of classes used by GoogleTest to print and
 * format test case results differently than GTest's default format.
 * See testresultprinter.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2018/08/27
 * - refactored to use AutograderUnitTestGui cpp class
 * @version 2018/01/23
 * - fixed bug with not overwriting bad/vague test output on diffs ("expected true, got false" etc.)
 * @version 2016/12/01
 * - fixed bugs with badly displayed / wrong test results
 * - clarified assertion output
 * @version 2016/10/22
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2015/10/08
 * - fixes for printing of failed non-timeout tests
 * @version 2014/11/24
 * @since 2014/11/24
 */

#define INTERNAL_INCLUDE 1
#include "testresultprinter.h"
#define INTERNAL_INCLUDE 1
#include "autograder.h"
#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace autograder {
std::string MartyTestResultPrinter::failMessage = "";

STATIC_CONST_VARIABLE_DECLARE(int, TEST_OUTPUT_INDENT, 8)
STATIC_CONST_VARIABLE_DECLARE(std::string, TEST_OUTPUT_INDENT_SPACES, "        ")

static std::string gtest_result_reformat(std::string msg) {
    bool hasActualLine = stringContains(msg, "Actual:");
    bool hasWhichIsLine = stringContains(msg, "Which is:");
    Vector<std::string> lines = stringutils::explodeLines(msg);
    for (int i = 0; i < lines.size(); i++) {
        if ((hasActualLine && startsWith(trim(lines[i]), "Value of:")) ||
            (hasWhichIsLine && startsWith(trim(lines[i]), "Expected:"))) {
            lines.remove(i);
            i--;
        }
    }
    msg = stringutils::implode(lines);
    msg = stringReplace(msg, "  Actual:", "Student :");
    msg = stringReplace(msg, "Which is:", "Expected:");
    return msg;
}

MartyTestResultPrinter::MartyTestResultPrinter() {
    testInProgress = false;
    failCountThisTest = 0;
    failCountToPrintPerTest = 0;  // no limit
    testCount = 0;
    failMessage = "";
    testNameWidth = 64;  // default 64 chars wide
}

int MartyTestResultPrinter::getTestNameWidth() const {
    return testNameWidth;
}

void MartyTestResultPrinter::OnTestStart(const ::testing::TestInfo& test_info) {
    testInProgress = true;
    testCount++;
    std::string testFullName = test_info.full_name();
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->getFlags().currentTestCaseName = testFullName;
    std::cout << std::setw(4) << std::right << testCount << ") "
         << std::setw(testNameWidth) << std::left << test_info.name() << " ... ";
    std::cout.flush();
}

void MartyTestResultPrinter::OnTestPartResult(const ::testing::TestPartResult& test_part_result) {
    if (test_part_result.failed()) {
        failCountThisTest++;
        if (failCountThisTest == 1) {
            if (!failMessage.empty()) {
                std::cout << "FAIL!" << std::endl;
            } else {
                std::string msg = test_part_result.message();
                if (msg.find("C++ exception with description ") != std::string::npos) {
                    std::cout << "EXCEPTION!" << std::endl;
                }
            }
        }
        if (failCountToPrintPerTest > 0 && failCountThisTest <= failCountToPrintPerTest) {
            if (!failMessage.empty()) {
                std::cout << STATIC_VARIABLE(TEST_OUTPUT_INDENT_SPACES) << failMessage << std::endl;
            }
            std::cout << stringutils::indent(gtest_result_reformat(test_part_result.summary()), STATIC_VARIABLE(TEST_OUTPUT_INDENT)) << std::endl;
        }
    }
}

void MartyTestResultPrinter::OnTestEnd(const ::testing::TestInfo& test_info) {
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    if (autograder->getFlags().testTimers[test_info.full_name()].isStarted()) {
        autograder->getFlags().testTimers[test_info.full_name()].stop();
    }
    if (test_info.result()->Failed()) {
        if (failCountToPrintPerTest > 0 && failCountThisTest > failCountToPrintPerTest) {
            int extraFails = failCountThisTest - failCountToPrintPerTest;
            std::cout << STATIC_VARIABLE(TEST_OUTPUT_INDENT_SPACES) << "(" << extraFails << " additional assertion failure(s) not printed)" << std::endl;
        }
    } else {
        std::cout << "pass, " << std::setw(5) << std::right
                  << autograder->getFlags().testTimers[test_info.full_name()].elapsed()
                  << "ms" << std::endl;
    }
    testInProgress = false;
    failCountThisTest = 0;
}

void MartyTestResultPrinter::OnTestProgramEnd(const ::testing::UnitTest& unit_test) {
    std::cout << "====================================================================" << std::endl;
    int testCount = unit_test.total_test_count();
    int failCount = unit_test.failed_test_count();
    int passCount = testCount - failCount;
    std::cout << "Total tests: " << testCount << ", passed: " << passCount << ", failed: " << failCount << std::endl;
    if (failCount == 0) {
        std::cout << "The student passed all functionality tests. Hooray!" << std::endl;
    } else {
        std::cout << "The student failed " << failCount << " functionality tests. Please investigate." << std::endl;
    }
}

void MartyTestResultPrinter::setFailDetails(AutograderTest& /* test */, const UnitTestDetails& deets) {
    setFailDetails(deets);
}

void MartyTestResultPrinter::setFailDetails(const UnitTestDetails& deets) {
    failMessage = FAIL_PREFIX + deets.message;
    switch (deets.testType) {
    case autograder::UnitTestType::TEST_ASSERT_EQUALS:
        failMessage += "  (must be equal)";  break;
    case autograder::UnitTestType::TEST_ASSERT_NOT_EQUALS:
        failMessage += "  (must be non-equal)";  break;
    case autograder::UnitTestType::TEST_ASSERT_NEAR:
        failMessage += "  (must be nearly equal)";  break;
    case autograder::UnitTestType::TEST_ASSERT_TRUE:
        failMessage += "  (must be true)";  break;
    case autograder::UnitTestType::TEST_ASSERT_FALSE:
        failMessage += "  (must be false)";  break;
    default:  break;
    }
}

void MartyTestResultPrinter::setFailsToPrintPerTest(int fails) {
    failCountToPrintPerTest = fails;
}

void MartyTestResultPrinter::setTestNameWidth(int width) {
    testNameWidth = width;
}

// ===========================================================================

const int MartyGraphicalTestResultPrinter::TEST_RUNTIME_MIN_TO_DISPLAY_MS = 100;

void MartyGraphicalTestResultPrinter::setFailDetails(AutograderTest& test, const UnitTestDetails& deets) {
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->setTestDetails(test.getFullName(), deets);
}

void MartyGraphicalTestResultPrinter::setFailDetails(const UnitTestDetails& deets) {
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    std::string curTest = autograder->getCurrentTestCaseName();
    autograder->setTestDetails(curTest, deets);
}

void MartyGraphicalTestResultPrinter::OnTestStart(const ::testing::TestInfo& test_info) {
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    std::string testFullName = test_info.full_name();
    autograder->getFlags().testTimers[testFullName].start();   // starts timer
    if (autograder->testShouldRun(testFullName)) {
        autograder->setCurrentTestCaseName(testFullName);
    }
}

void MartyGraphicalTestResultPrinter::OnTestEnd(const ::testing::TestInfo& test_info) {
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    std::string testFullName = test_info.full_name();
    if (!autograder->testShouldRun(testFullName)) {
        return;
    }

    if (test_info.result()->Failed()) {
        autograder->setTestResult(testFullName, stanfordcpplib::autograder::TEST_RESULT_FAIL);
        for (int i = 0; i < test_info.result()->total_part_count(); i++) {
            testing::TestPartResult part = test_info.result()->GetTestPartResult(i);
            if (part.failed()) {
                UnitTestDetails deets(autograder::UnitTestType::TEST_FAIL, part.message());
                deets.overwrite = false;   // BUGFIX for bad test details on Windows
                autograder->setTestDetails(testFullName, deets);
                break;
            }
        }
    } else {
        autograder->setTestResult(testFullName, stanfordcpplib::autograder::TEST_RESULT_PASS);
    }

    if (autograder->getFlags().testTimers.containsKey(testFullName)) {
        autograder->getFlags().testTimers[testFullName].stop();
        long runtimeMS = autograder->getFlags().testTimers[testFullName].elapsed();
        if (runtimeMS >= TEST_RUNTIME_MIN_TO_DISPLAY_MS) {
            autograder->setTestRuntime(testFullName, runtimeMS);
        }
    }
}

} // namespace autograder
} // namespace stanfordcpplib
