/*
 * File: autogradercompat.h
 * ------------------------
 * This file contains functions to make the autograder library more
 * backward-compatible with old autograder code.
 * 
 * @author Marty Stepp
 * @version 2018/10/04
 * - initial version, copied from autograder.h
 */

#ifndef _autogradercompat_h
#define _autogradercompat_h

#include <string>
#define INTERNAL_INCLUDE 1
#include "autograder.h"
#undef INTERNAL_INCLUDE

namespace autograder {

void addCallbackButton(void (* func)(), const std::string& text, const std::string& icon = "");
void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                  const std::string& diffs, const std::string& diffFile = "",
                  int truncateHeight = -1);
std::string getCurrentCategoryName();
std::string getCurrentTestCaseName();
bool currentTestShouldRun();
stanfordcpplib::autograder::AutograderFlags& getFlags();
bool isGraphicalUI();
std::string runAndCapture(
        int (* mainFunc)(),
        const std::string& cinInput = "",
        const std::string& outputFileName = "");
void setAboutMessage(const std::string& message);
void setAssignmentName(const std::string& name);
void setCallbackEnd(void(* func)());
void setCallbackStart(void(* func)());
void setCurrentCategoryName(const std::string& categoryName);
void setCurrentTestCaseName(const std::string& testFullName);
void setCurrentTestShouldRun(bool shouldRun);
void setExitEnabled(bool enabled);
bool exitEnabled();
void setFailDetails(stanfordcpplib::autograder::AutograderTest& test,
                    const stanfordcpplib::autograder::UnitTestDetails& deets);
void setFailDetails(const stanfordcpplib::autograder::UnitTestDetails& deets);
void setFailsToPrintPerTest(int count);
void setGraphicalUI(bool value = true);
void setShowInputPanel(bool show, const std::string& filename = "");
void setShowLateDays(bool show);
void setShowTestCaseFailDetails(bool show);
void setStartMessage(const std::string& startMessage);
void setStudentProgramFileName(const std::string& filename);
void setTestCounts(int passCount, int testCount, bool isStyleCheck);
void setTestNameWidth(int width);
void setTestShouldRun(const std::string& testFullName, bool shouldRun);
void showLateDays(const std::string& filename = LATE_DAYS_FILENAME);
void showOutput(const std::string& output, bool showIfGraphical = true, bool showIfConsole = true);
void showOutput(std::ostringstream& output, bool showIfGraphical = true, bool showIfConsole = true);
void showStudentTextFile(const std::string& filename, int maxWidth = 0, int maxHeight = 0);
void styleCheckAddFile(const std::string& filename, const std::string& styleCheckXmlFileName = "");
bool testShouldRun(const std::string& testFullName);
} // namespace autograder

#endif // _autograder_h
