/*
 * File: consoleautograder.cpp
 * ---------------------------
 * @version 2018/10/03
 * - initial version, refactored from autograder.h/cpp
 */

#include "consoleautograder.h"
#include <cstdio>
#include "consoletext.h"
#include "error.h"
#include "exceptions.h"
#include "filelib.h"
#include "gconsolewindow.h"
#include "gwindow.h"
#include "map.h"
#include "simpio.h"
#include "private/static.h"
#include "date.h"
#include "gtest-marty.h"
#include "ioutils.h"
#include "qtgui.h"
#include "stringutils.h"
#include "stylecheck.h"
#include "testresultprinter.h"

// to be written by TA/instructor for each assignment
extern void autograderMain();

// student's main function
extern int main();

namespace stanfordcpplib {
namespace autograder {

ConsoleAutograder::ConsoleAutograder()
        : Autograder() {
    // TODO
}

ConsoleAutograder::~ConsoleAutograder() {
    // TODO
}

void ConsoleAutograder::addCategory(const std::string& /*categoryName*/, const std::string& /*categoryDescription*/) {
    // empty
}

void ConsoleAutograder::addTest(const std::string& /*testName*/, const std::string& /*categoryName*/) {
    // empty
}

bool ConsoleAutograder::autograderYesOrNo(std::string prompt, std::string reprompt, std::string defaultValue) {
    return getYesOrNo(prompt, reprompt, defaultValue);
}

bool ConsoleAutograder::containsCategory(const std::string& /*categoryName*/) {
    // empty
    return false;
}

void ConsoleAutograder::displayDiffs(const std::string& expectedOutput,
                                     const std::string& studentOutput,
                                     const std::string& diffs,
                                     const std::string& diffFile,
                                     int truncateHeight) {
    std::cout << "FAIL!" << std::endl;
    int expectedHeight = stringutils::height(expectedOutput);
    int studentHeight  = stringutils::height(studentOutput);
    if (expectedHeight != studentHeight) {
        std::cout << "  Expected " << expectedHeight << " lines,"
                  << "found " << studentHeight << " lines." << std::endl;
    }
    std::cout << "      DIFFS:";
    if (!diffFile.empty()) {
        std::cout << " (see complete output in " << diffFile << " in build folder)";
    }
    std::cout << std::endl;
    std::string diffsToShow = diffs;
    if (truncateHeight > 0) {
        diffsToShow = stringutils::trimToHeight(diffsToShow, truncateHeight);
    }
    std::cout << stringutils::indent(diffsToShow, 8) << std::endl;
}

bool ConsoleAutograder::isGraphicalUI() const {
    return false;
}

int ConsoleAutograder::runAllTestCases() {
    static bool gtestInitialized = false;   // static OK

    // set up a few initial settings and lock-down the program
    ioutils::setConsoleEchoUserInput(true);
    setConsoleClearEnabled(false);
    GWindow::_autograder_setPauseEnabled(false);
    GWindow::_autograder_setExitGraphicsEnabled(false);
    setConsoleSettingsLocked(true);

    if (!gtestInitialized) {
        gtestInitialized = true;
        int argc = QtGui::instance()->getArgc();
        char** argv = QtGui::instance()->getArgv();
        ::testing::InitGoogleTest(&argc, argv);

        // set up 'test result printer' to better format test results and errors
        ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
        delete listeners.Release(listeners.default_result_printer());
        MartyTestResultPrinter* printer = new MartyTestResultPrinter();
        if (_flags.testNameWidth > 0) {
            printer->setTestNameWidth(_flags.testNameWidth);
        }
        if (_flags.failsToPrintPerTest > 0) {
            printer->setFailsToPrintPerTest(_flags.failsToPrintPerTest);
        }
        listeners.Append(printer);
    }

    int result = RUN_ALL_TESTS();   // run Google Test framework now

    // un-lock-down
    setConsoleSettingsLocked(false);
    ioutils::setConsoleEchoUserInput(false);
    setConsoleClearEnabled(true);
    GWindow::_autograder_setPauseEnabled(true);
    GWindow::_autograder_setExitGraphicsEnabled(true);

    if (!_flags.graphicalUI) {
        std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
        getLine("Press Enter to continue . . .");
    }

    return result;
}

void ConsoleAutograder::runStyleChecker() {
    getConsoleWindow()->toFront();
    int styleCheckCount = 0;
    for (std::string filename : _flags.styleCheckFiles) {
        stylecheck::styleCheck(
                    filename,
                    _flags.styleCheckFileMap[filename],
                     /* printWarning */ styleCheckCount == 0);
        std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
        styleCheckCount++;
    }
}

void ConsoleAutograder::runTest(stanfordcpplib::autograder::AutograderTest* /*test*/) {
    // TODO
    resetStandardInputStreams();
    error("oops! not implemented");
}

void ConsoleAutograder::setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) {
    MartyTestResultPrinter::setFailDetails(test, deets);
}

void ConsoleAutograder::setFailDetails(const autograder::UnitTestDetails& deets) {
    MartyTestResultPrinter::setFailDetails(deets);
}

void ConsoleAutograder::setTestCounts(int /*passCount*/, int /*testCount*/, bool /*isStyleCheck*/) {
//    stanfordcpplib::autograder::AutograderGui::instance(/*isStyleCheck*/)->setTestCounts(
//                passCount, testCount);
    // TODO
}

void ConsoleAutograder::setTestDetails(const std::string& /*testFullName*/, UnitTestDetails /*deets*/) {
    // empty
}

bool ConsoleAutograder::setTestResult(const std::string& /*testFullName*/, TestResult /*result*/) {
    // empty
    return true;
}

bool ConsoleAutograder::setTestRuntime(const std::string& /*testFullName*/, long /*ms*/) {
    // empty
    return true;
}

/*
 * student_submission_time: 13/Oct/2014 10:31:15
 * assignment_due_time: 13/Oct/2014 23:59:00
 * calendar_days_late: 0
 */
void ConsoleAutograder::showLateDays(const std::string& filename) {
    Map<std::string, std::string> lineMap;
    std::string text;
    if (fileExists(filename)) {
        text = readEntireFile(filename);
    } else {
        text = std::string("student_submission_time: unknown\n")
                + "assignment_due_time: unknown\n"
                + "calendar_days_late: unknown\n"
                + "details: " + filename + " not found!";
    }
    
    std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    std::cout << "Contents of " << filename << ":" << std::endl;
    std::cout << text;
    if (!endsWith(text, '\n')) {
        std::cout << std::endl;
    }
    std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
}

void ConsoleAutograder::showOutput(const std::string& output, bool /*showIfGraphical*/, bool showIfConsole) {
    if (showIfConsole) {
        std::cout << output << std::endl;
    }
}

void ConsoleAutograder::showStudentTextFile(const std::string& filename, int maxWidth, int maxHeight) {
    std::string myText;
    if (fileExists(filename)) {
        myText = readEntireFile(filename);
    } else {
        myText = "file not found: " + filename;
    }
    int height = stringutils::height(myText);
    if (maxWidth > 0) {
        myText = stringutils::trimToWidth(myText, maxWidth);
    }
    if (maxWidth > 0) {
        myText = stringutils::trimToHeight(myText, maxHeight);
    }
    if (!endsWith(myText, "\n")) {
        myText += "\n";
    }
    // these begin/end markers are kept to the same width
    // as AUTOGRADER_OUTPUT_SEPARATOR from autograder.h
    std::cout << "Here are the contents of the student's " << filename << " file"
              << " (" << height << " lines):" << std::endl
              << "============================== BEGIN ===============================" << std::endl
              << myText
              << "=============================== END ================================" << std::endl;
}

int ConsoleAutograder::mainFunc() {
    std::cout << _flags.assignmentName << " Autograder" << std::endl
              << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    
    std::string autogradeYesNoMessage = _flags.startMessage;
    if (!autogradeYesNoMessage.empty()) {
        autogradeYesNoMessage += "\n\n";
    }
    autogradeYesNoMessage += "Attempt auto-grading (Y/n)? ";
    
    int result = 0;
    bool autograde = autograderYesOrNo(autogradeYesNoMessage, "", "y");
    if (autograde) {
        if (_flags.callbackStart) {
            _flags.callbackStart();
        }
        
        // TODO
        // result = mainRunAutograderTestCases();
        
        // a hook to allow per-assignment code to run at the start
        if (_flags.callbackEnd) {
            _flags.callbackEnd();
        }
    }
    
    // manual testing
    bool manualGrade = autograderYesOrNo("Run program for manual testing (y/N)? ", "", "n");
    if (manualGrade) {
        GWindow::_autograder_setExitGraphicsEnabled(false);
        while (manualGrade) {
            main();   // run student's main function
            std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
            manualGrade = autograderYesOrNo("Run program again (y/N)? ", "", "n");
        }
        GWindow::_autograder_setExitGraphicsEnabled(true);
    }
    std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    
    // run any custom callbacks that the assignment's autograder added
    for (int i = 0; i < _flags.callbackButtons.size(); i++) {
        std::string buttonText = _flags.callbackButtons[i].text;
        buttonText = stringReplace(buttonText, "\n", " ");   // GUI often uses multi-line button text strings; strip
        if (autograderYesOrNo(buttonText + " (Y/n)? ", "", "y")) {
            _flags.callbackButtons[i].func();
        }
    }
    
    // run Style Checker to point out common possible style issues
    if (!_flags.styleCheckFiles.isEmpty()) {
        if (autograderYesOrNo("Run style checker (Y/n)? ", "", "y")) {
            // TODO
            // mainRunStyleChecker();
        }
    }
    
    // display turnin time information to decide late days used
    if (_flags.showLateDays) {
        showLateDays();
    }
    
    std::cout << "COMPLETE: Autograder terminated successfully. Exiting." << std::endl;
    return result;
}

} // namespace autograder
} // namespace stanfordcpplib
