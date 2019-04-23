/*
 * File: autograder.h
 * ------------------
 * This file contains declarations of utility code used to help implement
 * autograder programs for grading student assignments.
 * 
 * @author Marty Stepp
 * @version 2019/04/23
 * - reset std::cout/cerr flags on every test run
 * @version 2018/10/03
 * - refactored to Autograder virtual base class
 * @version 2016/12/01
 * - removed most "current test case" logic and replaced with testcase-specific logic
 * @version 2016/08/01
 * - added setExitEnabled(bool) function
 * @version 2014/10/31
 * - added graphical autograder support (setGraphicalUI)
 * @since 2014/10/14
 */

#ifndef _autograder_h
#define _autograder_h

#include <iomanip>
#include <iostream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"

#define INTERNAL_INCLUDE 1
#include "autogradertest.h"
#define INTERNAL_INCLUDE 1
#include "gtest-marty.h"
#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#undef INTERNAL_INCLUDE

#define AUTOGRADER_DEFAULT_ABOUT_TEXT (std::string("CS 106 B/X Autograder Framework\nDeveloped by Marty Stepp (stepp@cs.stanford.edu)"))
#define EXCEPTION_ERROR_MESSAGE (std::string("test threw an exception!"))
#define TIMEOUT_ERROR_MESSAGE (std::string("test timed out! possible infinite loop"))

/*
 * Default file name where late-day information is stored
 */
#define LATE_DAYS_FILENAME "lateDays.txt"

/*
 * Default file where XML information for a console input panel is stored
 */
#define INPUT_PANE_FILENAME "inputpanel.xml"

/*
 * A string printed between output sections for separation.
 */
#define AUTOGRADER_OUTPUT_SEPARATOR "===================================================================="

/*
 * max # of chars student's program can print before halted by autograder
 */
#define MAX_STUDENT_OUTPUT 262144

namespace stanfordcpplib {
namespace autograder {


/*
 * A structure containing information about custom callback buttons in the graphical UI.
 */
struct CallbackButtonInfo {
    void (* func)();    // function to call when button is clicked
    std::string text;   // button text
    std::string icon;   // button icon
};

/*
 * A global structure containing state and flags and configuration for the autograder.
 */
struct AutograderFlags {
public:
    std::string assignmentName;
    std::string studentProgramFileName;
    std::string currentCategoryName;
    std::string currentTestCaseName;
    std::string inputPanelFilename;
    std::string startMessage;
    std::string aboutText;
    bool currentTestShouldRun;
    Map<std::string, Set<std::string> > testsAdded;
    Map<std::string, Timer> testTimers;
    Set<std::string> testsThatShouldRun;

    int failsToPrintPerTest;
    int testNameWidth;
    bool showInputPanel;
    bool showLateDays;
    bool graphicalUI;
    Vector<std::string> styleCheckFiles;
    Map<std::string, std::string> styleCheckFileMap;
    void(* callbackStart)();
    void(* callbackEnd)();
    Vector<CallbackButtonInfo> callbackButtons;
    
    AutograderFlags();
};

/*
 * The main function called by the autograder to run the program.
 * Your autograder file should call this to initiate the grading process.
 */
int autograderMain(int argc = 0, char** argv = nullptr);

/*
 * e.g. date = "13/Oct/2014 10:31:15"
 */
std::string formatDate(const std::string& dateStr);

/**
 * ...
 */
/*abstract*/ class Autograder {
public:
    virtual ~Autograder();

    static Autograder* instance();

    static void setInstance(Autograder* autograder);

    virtual void addCategory(const std::string& categoryName, const std::string& categoryDescription = "") = 0;

    virtual void addTest(const std::string& testName, const std::string& categoryName = "") = 0;

    virtual bool autograderYesOrNo(std::string prompt, std::string reprompt = "", std::string defaultValue = "") = 0;

    virtual bool containsCategory(const std::string& categoryName) = 0;

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual bool currentTestShouldRun() const;

    /**
     * Displays a 'diff' betwen the given expected output and the given student output.
     * Also optionally saves the diffs to a file and/or chops the number of lines shown.
     */
    virtual void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                              const std::string& diffs, const std::string& diffFile = "",
                              int truncateHeight = -1) = 0;

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual std::string getCurrentCategoryName() const;

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual std::string getCurrentTestCaseName() const;

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual AutograderFlags& getFlags();

    /**
     * Returns true if setGraphicalUI was called with value true.  Initially false.
     */
    virtual bool isGraphicalUI() const = 0;

    virtual int mainFunc() = 0;

    virtual void resetStandardInputStreams();

    virtual int runAllTestCases() = 0;

    /**
     * Runs a 'main' function, possibly feeding it cin input, and capture its cout output.
     * Dump the output to a file if filename passed, and return output as a string.
     */
    virtual std::string runAndCapture(
            int (* mainFunc)(),
            const std::string& cinInput = "",
            const std::string& outputFileName = "");

    virtual void runStyleChecker() = 0;

    virtual void runTest(stanfordcpplib::autograder::AutograderTest* test) = 0;

    /**
     * ...
     */
    virtual void setAboutMessage(const std::string& message);

    /**
     * Sets the assignment name to display in the console header.
     */
    virtual void setAssignmentName(const std::string& name);

    /**
     * Specifies a function to call once the automated tests are finished running.
     * If null is passed, no function will be called.
     */
    virtual void setCallbackEnd(void(* func)());

    /**
     * Specifies a function to call before the automated tests start running.
     * If null is passed, no function will be called.
     */
    virtual void setCallbackStart(void(* func)());

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual void setCurrentCategoryName(const std::string& categoryName);

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual void setCurrentTestCaseName(const std::string& testFullName);

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual void setCurrentTestShouldRun(bool shouldRun);

    /**
     * Specifies details to show for a failed test case.
     * Called internally by assertXxxx() macros.
     */
    virtual void setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) = 0;
    virtual void setFailDetails(const autograder::UnitTestDetails& deets) = 0;

    /**
     * Sets the number of assertion failures to print in each individual test;
     * defaults to 1 to avoid printing tons of hard-to-read output.
     * Turn up if you want to see more/all of the assertion failures.
     * Set it to <= 0 for no limit.
     */
    virtual void setFailsToPrintPerTest(int count);

    /**
     * Sets whether to show a GUI rather than printing test results to cout.
     */
    virtual void setGraphicalUI(bool value = true);

    /**
     * Enables/disables the helper window panel that pops up with
     * buttons you can press to enter common user inputs.
     * If no file name is passed, defaults to reading data from "inputpanel.xml".
     */
    virtual void setShowInputPanel(bool show, const std::string& filename = "");

    /**
     * Toggles whether to show the late_days.txt info for each student.
     */
    virtual void setShowLateDays(bool show);

    /**
     * Sets whether to show full details for each failed test.
     */
    virtual void setShowTestCaseFailDetails(bool show);

    virtual void setStartMessage(const std::string& startMessage);

    /**
     * Sets the name of the student's program file.
     * Useful if the assignment has only a single primary file of code.
     */
    virtual void setStudentProgramFileName(const std::string& filename);

    /**
     * Sets the number of tests that have been run and passed.
     * This is kept track of automatically, but in certain cases,
     * (most notably when the 'omit on pass' flag is set on a style checker),
     * we want to override the currently visible counts.
     */
    virtual void setTestCounts(int passCount, int testCount, bool isStyleCheck) = 0;

    virtual void setTestDetails(const std::string& testFullName, UnitTestDetails deets) = 0;

    /**
     * Sets the number of characters in the longest test case name's width;
     * used to align the test case output in the console.
     */
    virtual void setTestNameWidth(int width);

    virtual bool setTestResult(const std::string& testFullName, TestResult result) = 0;

    virtual bool setTestRuntime(const std::string& testFullName, long ms) = 0;

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual void setTestShouldRun(const std::string& testFullName, bool shouldRun);

    /**
     * Sets whether the autograder should display the contents of the lateDays.txt
     * file to see whether the student turned in their assignment on time.
     */
    virtual void showLateDays(const std::string& filename = LATE_DAYS_FILENAME) = 0;

    /**
     * Shows output either as cout (if non-graphical UI) or on a pop-up dialog box (if graphical).
     */
    virtual void showOutput(const std::string& output, bool showIfGraphical = true, bool showIfConsole = true) = 0;
    virtual void showOutput(std::ostringstream& output, bool showIfGraphical = true, bool showIfConsole = true);

    /**
     * Displays the contents of the given text file on the screen.
     * Useful for cases when the student must submit a creative output file
     * like mycolony.txt or mygrammar.txt.
     */
    virtual void showStudentTextFile(const std::string& filename, int maxWidth = 0, int maxHeight = 0) = 0;

    /**
     * Adds the given filename to the list of files to be style-checked.
     * Uses the given style check XML rules for style checking.
     * If no style check XML file name is passed, uses a default name
     * based on the filename, by the formula:
     * "Foo.cpp"   -> "stylecheck-foo-cpp.xml"
     * "Foo.bar.h" -> "stylecheck-foo-bar-h.xml"
     */
    virtual void styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName = "");

    /**
     * Called internally by autograder; do not use.
     * @private
     */
    virtual bool testShouldRun(const std::string& testFullName) const;

protected:
    static Autograder* _instance;

    Autograder();

    AutograderFlags _flags;
    std::ios _iosCoutBackup;   // for saving cout/cerr state/flags between tests
    std::ios _iosCerrBackup;
}; // class Autograder

} // namespace autograder
} // namespace stanfordcpplib

// for older programs
#define INTERNAL_INCLUDE 1
#include "autogradercompat.h"
#undef INTERNAL_INCLUDE

#endif // _autograder_h
