/*
 * File: autogradercompat.cpp
 * --------------------------
 * See autogradercompat.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/10/04
 * - initial version, copied from autograder.cpp
 */

#define INTERNAL_INCLUDE 1
#include "autogradercompat.h"
#define INTERNAL_INCLUDE 1
#include "guiautograder.h"
#undef INTERNAL_INCLUDE

namespace autograder {

void addCallbackButton(void (* func)(),
                       const std::string& text,
                       const std::string& icon) {
    if (isGraphicalUI()) {
        stanfordcpplib::autograder::GuiAutograder::instance()->addCallbackButton(func, text, icon);
    } else {
        // empty
    }
}

void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                  const std::string& diffs, const std::string& diffFile, int truncateHeight) {
    stanfordcpplib::autograder::GuiAutograder::instance()->displayDiffs(
            expectedOutput, studentOutput, diffs, diffFile, truncateHeight);
}

std::string getCurrentCategoryName() {
    return stanfordcpplib::autograder::GuiAutograder::instance()->getCurrentCategoryName();
}

std::string getCurrentTestCaseName() {
    return stanfordcpplib::autograder::GuiAutograder::instance()->getCurrentTestCaseName();
}

bool currentTestShouldRun() {
    return stanfordcpplib::autograder::GuiAutograder::instance()->currentTestShouldRun();
}

stanfordcpplib::autograder::AutograderFlags& getFlags() {
    return stanfordcpplib::autograder::GuiAutograder::instance()->getFlags();
}

bool isGraphicalUI() {
    return stanfordcpplib::autograder::GuiAutograder::instance()->isGraphicalUI();
}

std::string runAndCapture(int (* mainFunc)(),
                          const std::string& cinInput,
                          const std::string& outputFileName) {
    return stanfordcpplib::autograder::GuiAutograder::instance()->runAndCapture(
            mainFunc, cinInput, outputFileName);
}


void setAboutMessage(const std::string& message) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setAboutMessage(message);
}

void setAssignmentName(const std::string& name) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setAssignmentName(name);
}

void setCallbackEnd(void (*func)()) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setCallbackEnd(func);
}

void setCallbackStart(void (*func)()) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setCallbackStart(func);
}

void setCurrentCategoryName(const std::string& categoryName) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setCurrentCategoryName(categoryName);
}

void setCurrentTestCaseName(const std::string& testFullName) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setCurrentTestCaseName(testFullName);
}

void setCurrentTestShouldRun(bool shouldRun) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setCurrentTestShouldRun(shouldRun);
}

bool exitEnabled() {
    return stanfordcpplib::exitEnabled();
}

void setExitEnabled(bool enabled) {
    stanfordcpplib::setExitEnabled(enabled);
}

void setFailDetails(stanfordcpplib::autograder::AutograderTest& test,
                    const stanfordcpplib::autograder::UnitTestDetails& deets) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setFailDetails(test, deets);
}

void setFailDetails(const stanfordcpplib::autograder::UnitTestDetails& deets) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setFailDetails(deets);
}

void setFailsToPrintPerTest(int count) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setFailsToPrintPerTest(count);
}

void setGraphicalUI(bool value) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setGraphicalUI(value);
}

void setShowInputPanel(bool show, const std::string& filename) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setShowInputPanel(show, filename);
}

void setShowLateDays(bool show) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setShowLateDays(show);
}

void setShowTestCaseFailDetails(bool show) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setShowTestCaseFailDetails(show);
}

void setStartMessage(const std::string& startMessage) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setStartMessage(startMessage);
}

void setStudentProgramFileName(const std::string& filename) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setStudentProgramFileName(filename);
}

void setTestCounts(int passCount, int testCount, bool isStyleCheck) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setTestCounts(
            passCount, testCount, isStyleCheck);
}

void setTestNameWidth(int width) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setTestNameWidth(width);
}

void setTestShouldRun(const std::string& testFullName, bool shouldRun) {
    stanfordcpplib::autograder::GuiAutograder::instance()->setTestShouldRun(
            testFullName, shouldRun);
}

/*
 * student_submission_time: 13/Oct/2014 10:31:15
 * assignment_due_time: 13/Oct/2014 23:59:00
 * calendar_days_late: 0
 */
void showLateDays(const std::string& filename) {
    stanfordcpplib::autograder::GuiAutograder::instance()->showLateDays(filename);
}

void showOutput(const std::string& output, bool showIfGraphical, bool showIfConsole) {
    stanfordcpplib::autograder::GuiAutograder::instance()->showOutput(output, showIfGraphical, showIfConsole);
}

void showOutput(std::ostringstream& output, bool showIfGraphical, bool showIfConsole) {
    showOutput(output.str(), showIfGraphical, showIfConsole);
    output.str("");   // clear output
}

void showStudentTextFile(const std::string& filename, int maxWidth, int maxHeight) {
    stanfordcpplib::autograder::GuiAutograder::instance()->showStudentTextFile(
            filename, maxWidth, maxHeight);
}

void styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName) {
    stanfordcpplib::autograder::GuiAutograder::instance()->styleCheckAddFile(
            filename, styleCheckXmlFileName);
}

bool testShouldRun(const std::string& testFullName) {
    return stanfordcpplib::autograder::GuiAutograder::instance()->testShouldRun(testFullName);
}

bool autograderYesOrNo(std::string prompt, std::string reprompt = "", std::string defaultValue = "") {
    return stanfordcpplib::autograder::GuiAutograder::instance()->autograderYesOrNo(
            prompt, reprompt, defaultValue);
}

} // namespace autograder
