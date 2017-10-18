/*
 * File: autograder.h
 * ------------------
 * This file contains declarations of utility code used to help implement
 * autograder programs for grading student assignments.
 * 
 * @author Marty Stepp
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

#include <string>
#include "map.h"
#include "set.h"
#include "vector.h"

#include "autogradertest.h"
#include "gtest-marty.h"
#include "unittestdetails.h"

/*
 * forward declarations of needed code from Stanford C++ lib's private/main.h
 * (so that the autograder can call the real main() etc.)
 */
extern int mainWrapper(int argc, char **argv);
extern int startupMain(int argc, char **argv);
extern void startupMainDontRunMain(int argc, char **argv);   // added to platform.cpp by Marty
extern int studentMain();
extern int _mainFlags;

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

void addCallbackButton(void (* func)(), const std::string& text, const std::string& icon = "");

/*
 * The main function called by the autograder to run the program.
 * Your autograder file should call this to initiate the grading process.
 */
int autograderMain(int argc = 0, char** argv = nullptr);

/*
 * Displays a 'diff' betwen the given expected output and the given student output.
 * Also optionally saves the diffs to a file and/or chops the number of lines shown.
 */
void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                  const std::string& diffs, const std::string& diffFile = "",
                  int truncateHeight = -1);

/*
 * Called internally by autograder; do not use.
 */
std::string getCurrentCategoryName();

/*
 * Called internally by autograder; do not use.
 */
std::string getCurrentTestCaseName();

/*
 * Called internally by autograder; do not use.
 */
bool currentTestShouldRun();

/*
 * Called internally by autograder; do not use.
 */
AutograderFlags& getFlags();

/*
 * Returns true if setGraphicalUI was called with value true.  Initially false.
 */
bool isGraphicalUI();

/*
 * Runs a 'main' function, possibly feeding it cin input, and capture its cout output.
 * Dump the output to a file if filename passed, and return output as a string.
 */
std::string runAndCapture(
        int (* mainFunc)(),
        const std::string& cinInput = "",
        const std::string& outputFileName = "");

/*
 * ...
 */
void setAboutMessage(const std::string& message);

/*
 * Sets the assignment name to display in the console header.
 */
void setAssignmentName(const std::string& name);

/*
 * Specifies a function to call once the automated tests are finished running.
 * If null is passed, no function will be called.
 */
void setCallbackEnd(void(* func)());

/*
 * Specifies a function to call before the automated tests start running.
 * If null is passed, no function will be called.
 */
void setCallbackStart(void(* func)());

/*
 * Called internally by autograder; do not use.
 */
void setCurrentCategoryName(const std::string& categoryName);

/*
 * Called internally by autograder; do not use.
 */
void setCurrentTestCaseName(const std::string& testFullName);

/*
 * Called internally by autograder; do not use.
 */
void setCurrentTestShouldRun(bool shouldRun);

/*
 * Sets whether the system function exit() should be allowed.
 * Default true, but can be turned off to stop student programs from
 * calling exit() as a form of return.
 * These are here for backward compatibility.
 */
void setExitEnabled(bool enabled);
bool exitEnabled();

/*
 * Specifies details to show for a failed test case.
 * Called internally by assertXxxx() macros.
 */
void setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets);
void setFailDetails(const autograder::UnitTestDetails& deets);

/*
 * Sets the number of assertion failures to print in each individual test;
 * defaults to 1 to avoid printing tons of hard-to-read output.
 * Turn up if you want to see more/all of the assertion failures.
 * Set it to <= 0 for no limit.
 */
void setFailsToPrintPerTest(int count);

/*
 * Sets whether to show a GUI rather than printing test results to cout.
 */
void setGraphicalUI(bool value = true);
bool isGraphicalUI();

/*
 * Enables/disables the helper window panel that pops up with
 * buttons you can press to enter common user inputs.
 * If no file name is passed, defaults to reading data from "inputpanel.xml".
 */
void setShowInputPanel(bool show, const std::string& filename = "");

/*
 * Toggles whether to show the late_days.txt info for each student.
 */
void setShowLateDays(bool show);

/*
 * Sets whether to show full details for each failed test.
 */
void setShowTestCaseFailDetails(bool show);

void setStartMessage(const std::string& startMessage);

/*
 * Sets the name of the student's program file.
 * Useful if the assignment has only a single primary file of code.
 */
void setStudentProgramFileName(const std::string& filename);

/*
 * Sets the number of tests that have been run and passed.
 * This is kept track of automatically, but in certain cases,
 * (most notably when the 'omit on pass' flag is set on a style checker),
 * we want to override the currently visible counts.
 */
void setTestCounts(int passCount, int testCount, bool isStyleCheck);

/*
 * Sets the number of characters in the longest test case name's width;
 * used to align the test case output in the console.
 */
void setTestNameWidth(int width);

/*
 * Called internally by autograder; do not use.
 */
void setTestShouldRun(const std::string& testFullName, bool shouldRun);

/*
 * Sets whether the autograder should display the contents of the lateDays.txt
 * file to see whether the student turned in their assignment on time.
 */
void showLateDays(const std::string& filename = LATE_DAYS_FILENAME);

/*
 * Shows output either as cout (if non-graphical UI) or on a pop-up dialog box (if graphical).
 */
void showOutput(const std::string& output, bool showIfGraphical = true, bool showIfConsole = true);
void showOutput(std::ostringstream& output, bool showIfGraphical = true, bool showIfConsole = true);

/*
 * Displays the contents of the given text file on the screen.
 * Useful for cases when the student must submit a creative output file
 * like mycolony.txt or mygrammar.txt.
 */
void showStudentTextFile(const std::string& filename, int maxWidth = 0, int maxHeight = 0);

/*
 * Adds the given filename to the list of files to be style-checked.
 * Uses the given style check XML rules for style checking.
 * If no style check XML file name is passed, uses a default name
 * based on the filename, by the formula:
 * "Foo.cpp"   -> "stylecheck-foo-cpp.xml"
 * "Foo.bar.h" -> "stylecheck-foo-bar-h.xml"
 */
void styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName = "");

/*
 * Called internally by autograder; do not use.
 */
bool testShouldRun(const std::string& testFullName);
} // namespace autograder

#endif // _autograder_h
