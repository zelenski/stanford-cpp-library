/*
 * File: autograder.cpp
 * --------------------
 * This file contains implementations of utility code used to help implement
 * autograder programs for grading student assignments.
 * See autograder.h for documentation of each member.
 * 
 * @author Marty Stepp
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

#include "autograder.h"
#include <cstdio>
#include "console.h"
#include "exceptions.h"
#include "filelib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "ginteractors.h"
#include "goptionpane.h"
#include "gwindow.h"
#include "map.h"
#include "simpio.h"
#include "version.h"
#include "private/platform.h"
#include "private/static.h"

#include "date.h"
#include "gtest-marty.h"
#include "inputpanel.h"
#include "ioutils.h"
#include "stringutils.h"
#include "stylecheck.h"
#include "testresultprinter.h"

// to be written by TA/instructor for each assignment
extern void autograderMain();

namespace autograder {
STATIC_CONST_VARIABLE_DECLARE(std::string, DEFAULT_ABOUT_TEXT,
                              "CS 106 B/X Autograder Framework\nDeveloped by Marty Stepp (stepp@cs.stanford.edu)")
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(Set<GButton*>, AUTOGRADER_BUTTONS)

AutograderFlags::AutograderFlags() {
    assignmentName = "";
    studentProgramFileName = "";
    currentCategoryName = "";
    currentTestCaseName = "";
    startMessage = "";
    aboutText = STATIC_VARIABLE(DEFAULT_ABOUT_TEXT);
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

STATIC_VARIABLE_DECLARE_BLANK(AutograderFlags, FLAGS)

void addCallbackButton(void (* func)(), const std::string& text, const std::string& icon) {
    CallbackButtonInfo buttonInfo;
    buttonInfo.func = func;
    buttonInfo.text = text;
    buttonInfo.icon = icon;
    STATIC_VARIABLE(FLAGS).callbackButtons.add(buttonInfo);
}

void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                  const std::string& diffs, const std::string& diffFile, int truncateHeight) {
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        
    } else {
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
}

std::string getCurrentCategoryName() {
    return STATIC_VARIABLE(FLAGS).currentCategoryName;
}

std::string getCurrentTestCaseName() {
    return STATIC_VARIABLE(FLAGS).currentTestCaseName;
}

bool currentTestShouldRun() {
    return STATIC_VARIABLE(FLAGS).currentTestShouldRun;
}

AutograderFlags& getFlags() {
    return STATIC_VARIABLE(FLAGS);
}

bool isGraphicalUI() {
    return STATIC_VARIABLE(FLAGS).graphicalUI;
}

std::string runAndCapture(int (* mainFunc)(),
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


void setAboutMessage(const std::string& message) {
    STATIC_VARIABLE(FLAGS).aboutText = STATIC_VARIABLE(DEFAULT_ABOUT_TEXT) + "\n"
            + "Version: " + AUTOGRADER_LIB_VERSION
            + "\n==================================\n"
            + message;
}

void setAssignmentName(const std::string& name) {
    STATIC_VARIABLE(FLAGS).assignmentName = name;
}

void setCallbackEnd(void (*func)()) {
    STATIC_VARIABLE(FLAGS).callbackEnd = func;
}

void setCallbackStart(void (*func)()) {
    STATIC_VARIABLE(FLAGS).callbackStart = func;
}

void setCurrentCategoryName(const std::string& categoryName) {
    STATIC_VARIABLE(FLAGS).currentCategoryName = categoryName;
}

void setCurrentTestCaseName(const std::string& testFullName) {
    STATIC_VARIABLE(FLAGS).currentTestCaseName = testFullName;
}

void setCurrentTestShouldRun(bool shouldRun) {
    STATIC_VARIABLE(FLAGS).currentTestShouldRun = shouldRun;
}

bool exitEnabled() {
    return stanfordcpplib::exitEnabled();
}

void setExitEnabled(bool enabled) {
    stanfordcpplib::setExitEnabled(enabled);
}

void setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) {
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        MartyGraphicalTestResultPrinter::setFailDetails(test, deets);
    } else {
        MartyTestResultPrinter::setFailDetails(test, deets);
    }
}

void setFailDetails(const autograder::UnitTestDetails& deets) {
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        MartyGraphicalTestResultPrinter::setFailDetails(deets);
    } else {
        MartyTestResultPrinter::setFailDetails(deets);
    }
}

void setFailsToPrintPerTest(int count) {
    STATIC_VARIABLE(FLAGS).failsToPrintPerTest = count;
}

void setGraphicalUI(bool value) {
    STATIC_VARIABLE(FLAGS).graphicalUI = value;
}

void setShowInputPanel(bool show, const std::string& filename) {
    STATIC_VARIABLE(FLAGS).showInputPanel = show;
    if (!filename.empty()) {
        STATIC_VARIABLE(FLAGS).inputPanelFilename = filename;
    }
}

/*
 * Toggles whether to show the late_days.txt info for each student.
 */
void setShowLateDays(bool show) {
    STATIC_VARIABLE(FLAGS).showLateDays = show;
}

void setShowTestCaseFailDetails(bool /*show*/) {
    // showTestCaseFailDetails = show;
}

void setStartMessage(const std::string& startMessage) {
    STATIC_VARIABLE(FLAGS).startMessage = startMessage;
}

void setStudentProgramFileName(const std::string& filename) {
    STATIC_VARIABLE(FLAGS).studentProgramFileName = filename;
}

void setTestCounts(int passCount, int testCount, bool isStyleCheck) {
    stanfordcpplib::getPlatform()->autograderunittest_setTestCounts(passCount, testCount, isStyleCheck);
}

void setTestNameWidth(int width) {
    STATIC_VARIABLE(FLAGS).testNameWidth = width;
}

void setTestShouldRun(const std::string& testFullName, bool shouldRun) {
    if (shouldRun) {
        STATIC_VARIABLE(FLAGS).testsThatShouldRun.add(testFullName);
    } else {
        STATIC_VARIABLE(FLAGS).testsThatShouldRun.remove(testFullName);
    }
}

static std::string formatDate(const std::string& dateStr) {
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

/*
 * student_submission_time: 13/Oct/2014 10:31:15
 * assignment_due_time: 13/Oct/2014 23:59:00
 * calendar_days_late: 0
 */
void showLateDays(const std::string& filename) {
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
    
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        for (std::string line : stringSplit(text, "\n")) {
            if (stringContains(line, ": ")) {
                std::string key = line.substr(0, line.find(": "));
                std::string value = line.substr(line.find(": ") + 2);
                lineMap.put(key, value);
            }
        }
        
        std::string message;
        std::string dueTime = lineMap["assignment_due_time"];
        std::string submitTime = lineMap["student_submission_time"];
        std::string daysLate = lineMap["calendar_days_late"];
        std::string details = lineMap["details"];
        if (dueTime != "unknown") {
            dueTime = formatDate(dueTime);
        }
        if (submitTime != "unknown") {
            submitTime = formatDate(submitTime);
        }
        message += "<html><table>";
        if (!details.empty()) {
            message += "<tr><td><b>NOTE:</b></td><td>" + details + "</td></tr>";
        }
        message += "<tr><td><b>due</b></td><td>" + dueTime + "</td></tr>";
        message += "<tr><td><b>submitted</b></td><td>" + submitTime + "</td></tr>";
        message += "<tr><td><b>cal.days late</b></td><td>" + daysLate + "</td></tr>";
        message += "</table></html>";
        GOptionPane::showMessageDialog(message, filename,
                                       GOptionPane::PLAIN);
    } else {
        std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
        std::cout << "Contents of " << filename << ":" << std::endl;
        std::cout << text;
        if (!endsWith(text, '\n')) {
            std::cout << std::endl;
        }
        std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    }
}

void showOutput(const std::string& output, bool showIfGraphical, bool showIfConsole) {
    if (isGraphicalUI()) {
        if (showIfGraphical) {
            GOptionPane::showMessageDialog(output);
        }
    } else {
        // not graphical UI
        if (showIfConsole) {
            std::cout << output << std::endl;
        }
    }
}

void showOutput(std::ostringstream& output, bool showIfGraphical, bool showIfConsole) {
    showOutput(output.str(), showIfGraphical, showIfConsole);
    output.str("");   // clear output
}

void showStudentTextFile(const std::string& filename, int maxWidth, int maxHeight) {
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
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        GOptionPane::showTextFileDialog(
                    myText,
                    ///* title */ "Contents of " + filename + " (" + integerToString(height) + " lines)",
                    /* title */ "Contents of " + filename,
                    maxHeight, maxWidth
                    );
    } else {
        // these begin/end markers are kept to the same width
        // as AUTOGRADER_OUTPUT_SEPARATOR from autograder.h
        std::cout << "Here are the contents of the student's " << filename << " file"
                  << " (" << height << " lines):" << std::endl
                  << "============================== BEGIN ===============================" << std::endl
                  << myText
                  << "=============================== END ================================" << std::endl;
    }
}

void styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName) {
    std::string styleFile = styleCheckXmlFileName;
    if (styleCheckXmlFileName.empty()) {
        // "Foo.cpp" -> "stylecheck-foo-cpp.xml"
        styleFile = toLowerCase(filename);
        styleFile = stringReplace(styleFile, ".", "-");
        styleFile = "stylecheck-" + styleFile + ".xml";
    }
    STATIC_VARIABLE(FLAGS).styleCheckFiles.add(filename);
    STATIC_VARIABLE(FLAGS).styleCheckFileMap.put(filename, styleFile);
}

bool testShouldRun(const std::string& testFullName) {
    return STATIC_VARIABLE(FLAGS).testsThatShouldRun.contains(testFullName);
}

#ifdef SPL_AUTOGRADER_MODE
static bool autograderYesOrNo(std::string prompt, std::string reprompt = "", std::string defaultValue = "") {
    if (STATIC_VARIABLE(FLAGS).graphicalUI) {
        prompt = stringReplace(prompt, " (Y/n)", "");
        prompt = stringReplace(prompt, " (y/N)", "");
        prompt = stringReplace(prompt, " (y/n)", "");
        prompt = stringReplace(prompt, " (Y/N)", "");
        return GOptionPane::showConfirmDialog(prompt) == GOptionPane::ConfirmResult::YES;
    } else {
        return getYesOrNo(prompt, reprompt, defaultValue);
    }
}


static int mainRunAutograderTestCases(int argc, char** argv) {
    static bool gtestInitialized = false;   // static OK
    
    // set up a few initial settings and lock-down the program
    ioutils::setConsoleEchoUserInput(true);
    setConsoleClearEnabled(false);
    gwindowSetPauseEnabled(false);
    gwindowSetExitGraphicsEnabled(false);
    setConsoleSettingsLocked(true);

    if (!gtestInitialized) {
        gtestInitialized = true;
        ::testing::InitGoogleTest(&argc, argv);

        // set up 'test result printer' to better format test results and errors
        ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
        delete listeners.Release(listeners.default_result_printer());
        if (STATIC_VARIABLE(FLAGS).graphicalUI) {
            autograder::MartyGraphicalTestResultPrinter* graphicalPrinter = new autograder::MartyGraphicalTestResultPrinter();
            listeners.Append(graphicalPrinter);
        } else {
            autograder::MartyTestResultPrinter* printer = new autograder::MartyTestResultPrinter();
            if (STATIC_VARIABLE(FLAGS).testNameWidth > 0) {
                printer->setTestNameWidth(STATIC_VARIABLE(FLAGS).testNameWidth);
            }
            if (STATIC_VARIABLE(FLAGS).failsToPrintPerTest > 0) {
                printer->setFailsToPrintPerTest(STATIC_VARIABLE(FLAGS).failsToPrintPerTest);
            }
            listeners.Append(printer);
        }
    }
    
    int result = RUN_ALL_TESTS();   // run Google Test framework now

    // un-lock-down
    setConsoleSettingsLocked(false);
    ioutils::setConsoleEchoUserInput(false);
    setConsoleClearEnabled(true);
    gwindowSetPauseEnabled(true);
    gwindowSetExitGraphicsEnabled(true);

    if (!STATIC_VARIABLE(FLAGS).graphicalUI) {
        std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
        getLine("Press Enter to continue . . .");
    }
    
    return result;
}

static void mainRunStyleChecker() {
    stanfordcpplib::getPlatform()->jbeconsole_toFront();
    if (!stylecheck::isStyleCheckMergedWithUnitTests()) {
        stanfordcpplib::getPlatform()->autograderunittest_setVisible(true, /* styleCheck */ true);
    }
    int styleCheckCount = 0;
    for (std::string filename : STATIC_VARIABLE(FLAGS).styleCheckFiles) {
        stylecheck::styleCheck(
                    filename,
                    STATIC_VARIABLE(FLAGS).styleCheckFileMap[filename],
                     /* printWarning */ styleCheckCount == 0);
        if (!STATIC_VARIABLE(FLAGS).graphicalUI) {
            std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
        }
        styleCheckCount++;
    }
    stanfordcpplib::getPlatform()->autograderunittest_setTestingCompleted(true, /* styleCheck */ true);
}

int autograderTextMain(int argc, char** argv) {
    std::cout << STATIC_VARIABLE(FLAGS).assignmentName << " Autograder" << std::endl
              << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    
    // set up buttons to automatically enter user input
    if (STATIC_VARIABLE(FLAGS).showInputPanel) {
        inputpanel::load(STATIC_VARIABLE(FLAGS).inputPanelFilename);
    }
    
    std::string autogradeYesNoMessage = STATIC_VARIABLE(FLAGS).startMessage;
    if (!autogradeYesNoMessage.empty()) {
        autogradeYesNoMessage += "\n\n";
    }
    autogradeYesNoMessage += "Attempt auto-grading (Y/n)? ";
    
    int result = 0;
    bool autograde = autograderYesOrNo(autogradeYesNoMessage, "", "y");
    if (autograde) {
        if (STATIC_VARIABLE(FLAGS).callbackStart) {
            STATIC_VARIABLE(FLAGS).callbackStart();
        }
        
        result = mainRunAutograderTestCases(argc, argv);
        
        // a hook to allow per-assignment code to run at the start
        if (STATIC_VARIABLE(FLAGS).callbackEnd) {
            STATIC_VARIABLE(FLAGS).callbackEnd();
        }
    }
    
    // manual testing
    bool manualGrade = autograderYesOrNo("Run program for manual testing (y/N)? ", "", "n");
    if (manualGrade) {
        gwindowSetExitGraphicsEnabled(false);
        while (manualGrade) {
            studentMain();
            std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
            manualGrade = autograderYesOrNo("Run program again (y/N)? ", "", "n");
        }
        gwindowSetExitGraphicsEnabled(true);
    }
    std::cout << AUTOGRADER_OUTPUT_SEPARATOR << std::endl;
    
    // run any custom callbacks that the assignment's autograder added
    for (int i = 0; i < STATIC_VARIABLE(FLAGS).callbackButtons.size(); i++) {
        std::string buttonText = STATIC_VARIABLE(FLAGS).callbackButtons[i].text;
        buttonText = stringReplace(buttonText, "\n", " ");   // GUI often uses multi-line button text strings; strip
        if (autograderYesOrNo(buttonText + " (Y/n)? ", "", "y")) {
            STATIC_VARIABLE(FLAGS).callbackButtons[i].func();
        }
    }
    
    // run Style Checker to point out common possible style issues
    if (!STATIC_VARIABLE(FLAGS).styleCheckFiles.isEmpty()) {
        if (autograderYesOrNo("Run style checker (Y/n)? ", "", "y")) {
            mainRunStyleChecker();
        }
    }
    
    // display turnin time information to decide late days used
    if (STATIC_VARIABLE(FLAGS).showLateDays) {
        showLateDays();
    }
    
    std::cout << "COMPLETE: Autograder terminated successfully. Exiting." << std::endl;
    return result;
}

static std::string addAutograderButton(GWindow& gui, const std::string& text, const std::string& icon) {
    static Set<char> usedMnemonics;

    std::string html = "<html><center>" + stringReplace(text, "\n", "<br>") + "</center></html>";
    GButton* button = new GButton(html);
    STATIC_VARIABLE(AUTOGRADER_BUTTONS).add(button);

    // set mnemonic shortcut
    char mnemonic = '\0';
    for (int i = 0; i < (int) text.length(); i++) {
        if (isalpha(text[i]) && !usedMnemonics.contains(text[i])) {
            mnemonic = text[i];
            break;
        }
    }
    if (mnemonic) {
        usedMnemonics.add(mnemonic);
        button->setMnemonic(mnemonic);
        button->setAccelerator("ctrl " + charToString(mnemonic));
    }

    if (!icon.empty()) {
        button->setIcon(icon);
        button->setTextPosition(SwingConstants::SWING_CENTER, SwingConstants::SWING_BOTTOM);
    }
    gui.addToRegion(button, "SOUTH");
    return html;
}

int autograderGraphicalMain(int argc, char** argv) {
    GWindow gui(500, 300, /* visible */ false);
    gui.setTitle(STATIC_VARIABLE(FLAGS).assignmentName + " Autograder");
    gui.setCanvasSize(0, 0);
    autograder::gwindowSetIsAutograderWindow(gui, true);
    autograder::gwindowRememberPosition(gui);
    gui.setExitOnClose(true);
    
    GLabel startLabel("");
    if (!STATIC_VARIABLE(FLAGS).startMessage.empty()) {
        std::string startMessage = STATIC_VARIABLE(FLAGS).startMessage;
        if (!stringContains(startMessage, "<html>")) {
            startMessage = stringReplace(startMessage, "Note:", "<b>Note:</b>");
            startMessage = stringReplace(startMessage, "NOTE:", "<b>NOTE:</b>");
            startMessage = "<html><body style='width: 500px; max-width: 500px;'>"
                    + startMessage
                    + "</body></html>";
        }
        
        startLabel.setLabel(startMessage);
        gui.addToRegion(&startLabel, "NORTH");
    }
    
    std::string autogradeText = addAutograderButton(gui, "Automated\ntests", "check.gif");
    std::string manualText = addAutograderButton(gui, "Run\nmanually", "play.gif");
    std::string styleCheckText = addAutograderButton(gui, "Style\nchecker", "magnifier.gif");
    for (int i = 0; i < STATIC_VARIABLE(FLAGS).callbackButtons.size(); i++) {
        STATIC_VARIABLE(FLAGS).callbackButtons[i].text = addAutograderButton(gui, STATIC_VARIABLE(FLAGS).callbackButtons[i].text, STATIC_VARIABLE(FLAGS).callbackButtons[i].icon);
    }
    std::string lateDayText = addAutograderButton(gui, "Late days\ninfo", "calendar.gif");
    std::string aboutText = addAutograderButton(gui, "About\nGrader", "help.gif");
    std::string exitText = addAutograderButton(gui, "Exit\nGrader", "stop.gif");
    gui.pack();
    gui.setVisible(true);
    
    int result = 0;
    while (true) {
        GEvent event = waitForEvent(ACTION_EVENT);
        if (event.getEventClass() == ACTION_EVENT) {
            GActionEvent actionEvent(event);
            std::string cmd = actionEvent.getActionCommand();
            if (cmd == autogradeText) {
                if (STATIC_VARIABLE(FLAGS).callbackStart) {
                    STATIC_VARIABLE(FLAGS).callbackStart();
                }
                
                // stanfordcpplib::getPlatform()->autograderunittest_clearTests();
                stanfordcpplib::getPlatform()->autograderunittest_clearTestResults();
                stanfordcpplib::getPlatform()->autograderunittest_setTestingCompleted(false);
                stanfordcpplib::getPlatform()->autograderunittest_setVisible(true);
                result = mainRunAutograderTestCases(argc, argv);
                stanfordcpplib::getPlatform()->autograderunittest_setTestingCompleted(true);
                
                // if style checker is merged, also run it now
                if (stylecheck::isStyleCheckMergedWithUnitTests()) {
                    mainRunStyleChecker();
                }

                if (STATIC_VARIABLE(FLAGS).callbackEnd) {
                    STATIC_VARIABLE(FLAGS).callbackEnd();
                }
            } else if (cmd == manualText) {
                // set up buttons to automatically enter user input
                if (STATIC_VARIABLE(FLAGS).showInputPanel) {
                    inputpanel::load(STATIC_VARIABLE(FLAGS).inputPanelFilename);
                }
                
                // actually run the student's program
                // (While program is running, if we close console, exit entire
                // autograder program because we might be blocked on console I/O.
                // But after it's done running, set behavior to just hide the
                // console, since the grader will probably try to close it and then
                // proceed with more grading and tests afterward.
                // A little wonky, but it avoids most of the surprise cases of
                // "I closed the student's console and it killed the autograder".
                stanfordcpplib::getPlatform()->jbeconsole_clear();
                stanfordcpplib::getPlatform()->jbeconsole_setVisible(true);
                stanfordcpplib::getPlatform()->jbeconsole_toFront();
                // setConsoleCloseOperation(ConsoleCloseOperation::CONSOLE_EXIT_ON_CLOSE);
                autograder::setExitEnabled(false);   // block exit() call
                setConsoleCloseOperation(ConsoleCloseOperation::CONSOLE_HIDE_ON_CLOSE);

                studentMain();

                // gwindowSetExitGraphicsEnabled(true);
            } else if (cmd == styleCheckText) {
                mainRunStyleChecker();
            } else if (cmd == lateDayText) {
                showLateDays();
            } else if (cmd == aboutText) {
                GOptionPane::showMessageDialog(STATIC_VARIABLE(FLAGS).aboutText, "About Autograder",
                                               GOptionPane::MessageType::INFORMATION);
            } else if (cmd == exitText) {
                autograder::setExitEnabled(true);   // don't block exit() call

                // free up memory used by graphical interactors
                for (GButton* button : STATIC_VARIABLE(AUTOGRADER_BUTTONS)) {
                    delete button;
                }
                STATIC_VARIABLE(AUTOGRADER_BUTTONS).clear();

                gui.close();   // exits program; will not return
                break;
            } else {
                for (CallbackButtonInfo buttonInfo : STATIC_VARIABLE(FLAGS).callbackButtons) {
                    if (cmd == buttonInfo.text) {
                        buttonInfo.func();
                        break;
                    }
                }
            }
        }
    }

    // free up memory used by graphical interactors
    for (GButton* button : STATIC_VARIABLE(AUTOGRADER_BUTTONS)) {
        delete button;
    }
    STATIC_VARIABLE(AUTOGRADER_BUTTONS).clear();
    
    return result;
}
#endif // SPL_AUTOGRADER_MODE
} // namespace autograder

// declared in assertions.h
void assertSimilarImage(const std::string& msg,
                        const std::string& imagefile1,
                        const std::string& imagefile2,
                        int diffPixelTolerance,
                        int xmin, int ymin,
                        int xmax, int ymax) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    int diffPixels;
    if (xmin >= 0 && ymin >= 0 && xmax >= xmin && ymax >= ymin) {
        diffPixels = image1.countDiffPixels(image2, xmin, ymin, xmax, ymax);
    } else {
        diffPixels = image1.countDiffPixels(image2);
    }
    bool imagesAreEqual = diffPixels <= diffPixelTolerance;

    autograder::setFailDetails(autograder::UnitTestDetails(
        autograder::UnitTestType::TEST_ASSERT_DIFF_IMAGE,
        msg, imagefile1, imagefile2, "image",
        imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}

// declared in assertions.h
void assertEqualsImage(const std::string& msg,
                       const std::string& imagefile1,
                       const std::string& imagefile2) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    bool imagesAreEqual = image1 == image2;

    autograder::setFailDetails(autograder::UnitTestDetails(
        autograder::UnitTestType::TEST_ASSERT_DIFF_IMAGE,
        msg, imagefile1, imagefile2, "image",
        imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}

#undef main
int main(int argc, char** argv) {
    // initialize Stanford libraries and graphical console
    ::__initializeStanfordCppLibrary(argc, argv);

    setConsoleLocationSaved(true);
    setConsoleCloseOperation(ConsoleCloseOperation::CONSOLE_HIDE_ON_CLOSE);
    
    // tell the GUI the names of all tests so that it can display them
    stanfordcpplib::getPlatform()->autograderunittest_setVisible(false);
    for (std::string category : autograder::AutograderTest::getAllCategories()) {
        for (std::string test : autograder::AutograderTest::getAllTests(category)) {
            stanfordcpplib::getPlatform()->autograderunittest_addTest(test, category);
        }
    }
    stanfordcpplib::getPlatform()->autograderunittest_setVisible(true);

    // your assignment-specific autograder main runs here
    ::autograderMain();
    
    if (autograder::isGraphicalUI()) {
        return autograder::autograderGraphicalMain(argc, argv);
    } else {
        return autograder::autograderTextMain(argc, argv);
    }
}
