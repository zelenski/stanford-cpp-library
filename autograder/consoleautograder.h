/*
 * File: consoleautograder.h
 * -------------------------
 *
 * @author Marty Stepp
 * @version 2018/10/03
 * - initial version, refactored from autograder.h/cpp
 * @since 2014/10/14
 */

#ifndef _consoleautograder_h
#define _consoleautograder_h

#define INTERNAL_INCLUDE 1
#include "autograder.h"
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace autograder {

/**
 * ...
 */
class ConsoleAutograder : public Autograder {
public:
    ConsoleAutograder();
    virtual ~ConsoleAutograder() Q_DECL_OVERRIDE;
    virtual void addCategory(const std::string& categoryName, const std::string& categoryDescription = "") Q_DECL_OVERRIDE;
    virtual void addTest(const std::string& testName, const std::string& categoryName = "") Q_DECL_OVERRIDE;
    virtual bool autograderYesOrNo(std::string prompt, std::string reprompt = "", std::string defaultValue = "") Q_DECL_OVERRIDE;
    virtual bool containsCategory(const std::string& categoryName) Q_DECL_OVERRIDE;
    virtual void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                              const std::string& diffs, const std::string& diffFile = "",
                              int truncateHeight = -1) Q_DECL_OVERRIDE;
    virtual bool isGraphicalUI() const Q_DECL_OVERRIDE;
    virtual int mainFunc() Q_DECL_OVERRIDE;
    virtual int runAllTestCases() Q_DECL_OVERRIDE;
    virtual void runStyleChecker() Q_DECL_OVERRIDE;
    virtual void runTest(stanfordcpplib::autograder::AutograderTest* test) Q_DECL_OVERRIDE;
    virtual void setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) Q_DECL_OVERRIDE;
    virtual void setFailDetails(const autograder::UnitTestDetails& deets) Q_DECL_OVERRIDE;
    virtual void setTestCounts(int passCount, int testCount, bool isStyleCheck) Q_DECL_OVERRIDE;
    virtual void setTestDetails(const std::string& testFullName, UnitTestDetails deets) Q_DECL_OVERRIDE;
    virtual bool setTestResult(const std::string& testFullName, TestResult result) Q_DECL_OVERRIDE;
    virtual bool setTestRuntime(const std::string& testFullName, long ms) Q_DECL_OVERRIDE;
    virtual void showLateDays(const std::string& filename = LATE_DAYS_FILENAME) Q_DECL_OVERRIDE;
    virtual void showOutput(const std::string& output, bool showIfGraphical = true, bool showIfConsole = true) Q_DECL_OVERRIDE;
    virtual void showStudentTextFile(const std::string& filename, int maxWidth = 0, int maxHeight = 0) Q_DECL_OVERRIDE;
}; // class ConsoleAutograder


} // namespace autograder
} // namespace stanfordcpplib

#endif // _consoleautograder_h

