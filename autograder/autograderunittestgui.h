/*
 * File: autograderunittestgui.h
 * -----------------------------
 * 
 * @author Marty Stepp
 * @version 2018/09/03
 * - basic implementation completed; runnable tests with clickable interactors
 * @version 2018/08/31
 * - added containers for each unit test
 * @version 2018/08/27
 * - initial version, to replace old Java back-end version
 */

#ifndef _autograderunittestgui_h
#define _autograderunittestgui_h

#include <string>
#include "gcheckbox.h"
#include "gcontainer.h"
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

    void addCategory(const std::string& categoryName);
    void addTest(const std::string& testName, const std::string& categoryName = "");
    bool catchExceptions() const;
    void clearTestResults();
    void clearTests();
    TestResult getTestResult(const std::string& testFullName) const;
    bool isChecked(const std::string& testFullName) const;
    bool isStyleCheck() const;
    bool runTestsInSeparateThreads() const;
    void setCatchExceptions(bool catchExceptions);
    void setRunTestsInSeparateThreads(bool runInSeparateThreads);
    void setTestCounts(int passCount, int testCount);
    void setTestDetails(const std::string& testFullName, ::autograder::UnitTestDetails deets);
    void setTestingCompleted(bool visible);
    bool setTestResult(const std::string& testFullName, TestResult result);
    bool setTestRuntime(const std::string& testFullName, long ms);
    void setVisible(bool visible);
    void setWindowDescription(const std::string& description);

private:
    struct TestInfo {
        std::string name;
        std::string category;
        bool completed;
        GCheckBox* checkbox;
        GLabel* descriptionLabel;
        GLabel* resultLabel;
        ::autograder::UnitTestDetails details;

        std::string getFullName() const {
            return category.empty() ? name : (category + "_" + name);
        }
    };

    AutograderUnitTestGui();   // forbid construction

    int getCheckedTestCount() const;
    void minimize(GContainer* category);
    void minimize(GContainer* category, bool minimized);
    void selectAll(GContainer* category, bool selected);
    void showTestDetails(const std::string& testFullName);
    void updateSouthText();

    static AutograderUnitTestGui* _instance;
    static AutograderUnitTestGui* _instanceStyleCheck;
    static const int DEFAULT_WINDOW_WIDTH;
    static const int DEFAULT_WINDOW_HEIGHT;
    static int MAX_WINDOW_HEIGHT;
    static const std::string COLOR_PASS;
    static const std::string COLOR_FAIL;
    static const std::string COLOR_WARN;
    static std::string COLOR_NORMAL;
    static const std::string COLOR_ZEBRA_STRIPE_1;
    static const std::string COLOR_ZEBRA_STRIPE_2;
    static const std::string ICON_DESELECT_ALL_FILENAME;
    static const std::string ICON_FAIL_FILENAME;
    static const std::string ICON_MINIMIZE_FILENAME;
    static const std::string ICON_PASS_FILENAME;
    static const std::string ICON_PROGRESS_FILENAME;
    static const std::string ICON_RUNNING_FILENAME;
    static const std::string ICON_SELECT_ALL_FILENAME;
    static const std::string ICON_UNKNOWN_FILENAME;
    static const std::string ICON_WARNING_FILENAME;

    GWindow* _window;
    GContainer* _center;
    GCheckBox* _catchErrorsBox;
    GCheckBox* _runTestsInThreadsBox;
    // GLabel* _descriptionLabel;
    GLabel* _southLabel;
    GContainer* _currentCategory;

    Map<std::string, GContainer*> _allCategories;
    Map<std::string, TestInfo*> _allTestInfo;

    int _passCount;
    int _testCount;
    // int _testPanelHeight = -1;
    // bool _allCategoriesHidden = false;
    // bool _checkboxesShown = true;
    bool _catchExceptions;
    bool _checkboxesShown;
    bool _runInSeparateThreads;
    bool _testingIsInProgress;
};

} // namespace autograder
} // namespace stanfordcpplib

#endif // _autograderunittestgui_h
