/*
 * File: autograderunittestgui.h
 * -----------------------------
 * 
 * @author Marty Stepp
 * @version 2018/08/27
 * - initial version, to replace old Java back-end version
 */

#ifndef _autograderunittestgui_h
#define _autograderunittestgui_h

#include <string>
#include "gcheckbox.h"
#include "glabel.h"
#include "gwindow.h"
#include "map.h"
#include "unittestdetails.h"

namespace stanfordcpplib {
namespace autograder {

class AutograderUnitTestGui {
public:
    enum TestResult {
        TEST_RESULT_UNKNOWN,
        TEST_RESULT_FAIL,
        TEST_RESULT_WARN,
        TEST_RESULT_PASS
    };

    static AutograderUnitTestGui* instance(bool isStyleCheck = false);

    void addCategory(const std::string& category);
    void addTest(const std::string& test, const std::string& category);
    bool catchExceptions() const;
    void clearTestResults();
    void clearTests();
    bool isChecked(const std::string& testFullName) const;
    bool runTestsInSeparateThreads() const;
    void setCatchExceptions(bool catchExceptions);
    void setRunTestsInSeparateThreads(bool runInSeparateThreads);
    void setTestCounts(int passCount, int testCount);
    void setTestDetails(const std::string& testName, ::autograder::UnitTestDetails deets);
    void setTestingCompleted(bool visible);
    void setTestResult(const std::string& test, TestResult result);
    void setTestRuntime(const std::string& testName, long ms);
    void setVisible(bool visible);
    void setWindowDescription(const std::string& description);

private:
    AutograderUnitTestGui();   // forbid construction

    int getCheckedTestCount() const;
    void updateSouthText();

    static AutograderUnitTestGui* _instance;
    static AutograderUnitTestGui* _instanceStyleCheck;

    GWindow* _window;
    GCheckBox* _catchErrorsBox;
    GCheckBox* _runTestsInThreadsBox;
    GLabel* _descriptionLabel;
    GLabel* _southLabel;

    Map<std::string, std::string> _allCategories;

    int _passCount;
    int _testCount;
    int _testPanelHeight = -1;
    // bool _allCategoriesHidden = false;
    // bool _checkboxesShown = true;
    bool _catchExceptions;
    bool _runInSeparateThreads;
    bool _testingIsInProgress;
};

} // namespace autograder
} // namespace stanfordcpplib

#endif // _autograderunittestgui_h
