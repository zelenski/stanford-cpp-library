/*
 * File: autograder.cpp
 * --------------------
 * This file contains implementations of utility code used to help implement
 * autograder programs for grading student assignments.
 * See autograder.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/10/07
 * - moved main/qMain code out to autogradermainwrapper.cpp
 * @version 2018/08/27
 * - refactored to use AutograderUnitTestGui cpp class
 * @version 2017/10/05
 * - avoid error on file-not-found in showStudentTextFile
 * - made autograder window remember position
 * @version 2016/12/01
 * - fixed memory leak from autograder buttons
 * - removed most "current test case" logic and replaced with testcase-specific logic
 * @version 2016/11/09
 * - added mnemonics and accelerators for autograder panel buttons
 * @version 2016/10/28
 * - added assertSimilarImage
 * @version 2016/10/13
 * - refactored exitEnabled/setExitEnabled to use new Platform version
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/09/22
 * - fixed small bugs with exit()
 * @version 2016/08/02
 * - added setExitEnabled(bool) function and exitEnabled flag
 * - added assertEqualsImage
 * @version 2015/10/26
 * - added wrapped version of exit(int) to alert grader when students
 *   try to call exit() in their code to quit the program
 * @version 2014/11/27
 * - bug fix with shutting down program during manual testing
 * - bug fix with style checker not showing and having unwanted checkboxes
 * @version 2014/11/20
 * - bug fix with style checker spinner GIF image remaining at end
 * - bug fix with date/time printing "2:0 PM"
 * @version 2014/11/15
 * - improvements to autograder unit test GUI
 * @version 2014/11/13
 * - fixed bug with "false negative" tests where early assert failed then later assert passed
 * @version 2014/11/04
 * - made showLateDays dialog be 'plain' (no icon)
 * @version 2014/10/31
 * - added graphical autograder support (setGraphicalUI)
 * @since 2014/10/14
 */

#define INTERNAL_INCLUDE 1
#include "autograder.h"
#include <cstdio>
#include "consoleautograder.h"
#include "consoletext.h"
#include "exceptions.h"
#include "filelib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "ginputpanel.h"
#include "ginteractors.h"
#include "goptionpane.h"
#include "guiautograder.h"
#include "gwindow.h"
#include "map.h"
#include "qtgui.h"
#include "simpio.h"
#include "version.h"
#include "private/static.h"
#include "date.h"
#include "gtest-marty.h"
#include "ioutils.h"
#include "stringutils.h"
#include "stylecheck.h"
#include "testresultprinter.h"
#undef INTERNAL_INCLUDE

// to be written by TA/instructor for each assignment
extern void autograderMain();

// student's main function
extern int main();

namespace stanfordcpplib {
namespace autograder {

std::string formatDate(const std::string& dateStr) {
    // date = "13/Oct/2014 10:31:15"
    int day;
    char cmonth[16];
    int year;
    int hour;
    int minute;
    int second;
    if (sscanf(dateStr.c_str(), "%2d/%3s/%4d %2d:%2d:%2d", &day, cmonth, &year, &hour, &minute, &second) < 6) {
        return "INVALID DATE STRING";
    }
    std::string monthStr = cmonth;
    std::string amPm = (hour < 12) ? "AM" : "PM";
    if (hour > 12) {
        hour -= 12;
    }
    if (hour == 0) {
        hour = 12;
    }
    Date date(year, Date::parseMonth(monthStr), day);

    std::ostringstream out;
    out << date.getDayOfWeekName().substr(0, 3) << ", "
        << monthStr << " " << day << ", " << year << ", "
        << hour << ":" << std::setw(2) << std::setfill('0') << minute << " " << amPm;
    return out.str();
}


AutograderFlags::AutograderFlags() {
    assignmentName = "";
    studentProgramFileName = "";
    currentCategoryName = "";
    currentTestCaseName = "";
    startMessage = "";
    aboutText = "";
    failsToPrintPerTest = 1;
    testNameWidth = -1;
    showInputPanel = false;
    inputPanelFilename = INPUT_PANE_FILENAME;
    showLateDays = true;
    graphicalUI = false;
    callbackStart = nullptr;
    callbackEnd = nullptr;
    currentTestShouldRun = true;
}


/*static*/ Autograder* Autograder::_instance = nullptr;

/*static*/ Autograder* Autograder::instance() {
    return _instance;
}

/*static*/ void Autograder::setInstance(Autograder* autograder) {
    _instance = autograder;
}

Autograder::Autograder() {
    setAboutMessage(AUTOGRADER_DEFAULT_ABOUT_TEXT);
}

Autograder::~Autograder() {
    // empty
}

bool Autograder::currentTestShouldRun() const {
    return _flags.currentTestShouldRun;
}

std::string Autograder::getCurrentCategoryName() const {
    return _flags.currentCategoryName;
}

std::string Autograder::getCurrentTestCaseName() const {
    return _flags.currentTestCaseName;
}

AutograderFlags& Autograder::getFlags() {
    return _flags;
}

std::string Autograder::runAndCapture(int (* mainFunc)(),
                          const std::string& cinInput,
                          const std::string& outputFileName) {
    // run the 'main' function, possibly feeding it cin, input, and capture its cout output
    if (!cinInput.empty()) {
        ioutils::redirectStdinBegin(cinInput);
    }

    ioutils::setConsoleOutputLimit(MAX_STUDENT_OUTPUT);   // prevent infinite output by student
    ioutils::captureStdoutBegin();

    mainFunc();

    std::string output = ioutils::captureStdoutEnd();
    if (!cinInput.empty()) {
        ioutils::redirectStdinEnd();
    }

    // return the output as a string (and also possibly write it to a file)
    if (!outputFileName.empty()) {
        writeEntireFile(outputFileName, output);
    }
    return output;
}

void Autograder::setAboutMessage(const std::string& message) {
    _flags.aboutText = AUTOGRADER_DEFAULT_ABOUT_TEXT + "\n"
            + "Version: " + AUTOGRADER_LIB_VERSION
            + "\n==================================\n"
            + message;
}

void Autograder::setAssignmentName(const std::string& name) {
    _flags.assignmentName = name;
}

void Autograder::setCallbackEnd(void (*func)()) {
    _flags.callbackEnd = func;
}

void Autograder::setCallbackStart(void (*func)()) {
    _flags.callbackStart = func;
}

void Autograder::setCurrentCategoryName(const std::string& categoryName) {
    _flags.currentCategoryName = categoryName;
}

void Autograder::setCurrentTestCaseName(const std::string& testFullName) {
    _flags.currentTestCaseName = testFullName;
}

void Autograder::setCurrentTestShouldRun(bool shouldRun) {
    _flags.currentTestShouldRun = shouldRun;
}

void Autograder::setFailsToPrintPerTest(int count) {
    _flags.failsToPrintPerTest = count;
}

void Autograder::setGraphicalUI(bool value) {
    _flags.graphicalUI = value;
}

void Autograder::setShowInputPanel(bool show, const std::string& filename) {
    _flags.showInputPanel = show;
    if (!filename.empty()) {
        _flags.inputPanelFilename = filename;
    }
}

void Autograder::setShowLateDays(bool show) {
    _flags.showLateDays = show;
}

void Autograder::setShowTestCaseFailDetails(bool /*show*/) {
    // showTestCaseFailDetails = show;
    // empty
}

void Autograder::setStartMessage(const std::string& startMessage) {
    _flags.startMessage = startMessage;
}

void Autograder::setStudentProgramFileName(const std::string& filename) {
    _flags.studentProgramFileName = filename;
}

void Autograder::setTestNameWidth(int width) {
    _flags.testNameWidth = width;
}

void Autograder::setTestShouldRun(const std::string& testFullName, bool shouldRun) {
    if (shouldRun) {
        _flags.testsThatShouldRun.add(testFullName);
    } else {
        _flags.testsThatShouldRun.remove(testFullName);
    }
}

void Autograder::showOutput(std::ostringstream& output, bool showIfGraphical, bool showIfConsole) {
    showOutput(output.str(), showIfGraphical, showIfConsole);
    output.str("");   // clear output
}

void Autograder::styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName) {
    std::string styleFile = styleCheckXmlFileName;
    if (styleCheckXmlFileName.empty()) {
        // "Foo.cpp" -> "stylecheck-foo-cpp.xml"
        styleFile = toLowerCase(filename);
        styleFile = stringReplace(styleFile, ".", "-");
        styleFile = "stylecheck-" + styleFile + ".xml";
    }
    _flags.styleCheckFiles.add(filename);
    _flags.styleCheckFileMap.put(filename, styleFile);
}

bool Autograder::testShouldRun(const std::string& testFullName) const {
    return _flags.testsThatShouldRun.contains(testFullName);
}

} // namespace autograder
} // namespace stanfordcpplib
