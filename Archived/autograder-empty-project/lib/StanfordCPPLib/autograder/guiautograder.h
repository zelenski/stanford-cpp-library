/*
 * File: guiautograder.h
 * ---------------------
 * 
 * @author Marty Stepp
 * @version 2019/05/05
 * - dark mode support
 * @version 2019/04/23
 * - reset std::cout/cerr flags on every test run
 * @version 2019/04/22
 * - now uses image strip file for icons
 * @version 2018/10/07
 * - icon constants and path fixes
 * @version 2018/10/03
 * - renamed to AutograderGUI; various windows/UI merged into one window
 * @version 2018/09/03
 * - basic implementation completed; runnable tests with clickable interactors
 * @version 2018/08/31
 * - added containers for each unit test
 * @version 2018/08/27
 * - initial version, to replace old Java back-end version
 */

#ifndef _guiautograder_h
#define _guiautograder_h

#include <string>
#define INTERNAL_INCLUDE 1
#include "autograder.h"
#define INTERNAL_INCLUDE 1
#include "gbutton.h"
#define INTERNAL_INCLUDE 1
#include "gcheckbox.h"
#define INTERNAL_INCLUDE 1
#include "gcontainer.h"
#define INTERNAL_INCLUDE 1
#include "glabel.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace autograder {

class GuiAutograder : public Autograder {
public:
    static GuiAutograder* instance();

    virtual ~GuiAutograder() Q_DECL_OVERRIDE;

    virtual void addCallbackButton(void (* func)(),
                                   const std::string& text,
                                   const std::string& icon);
    virtual void addCategory(const std::string& categoryName, const std::string& categoryDescription = "") Q_DECL_OVERRIDE;
    virtual void addTest(const std::string& testName, const std::string& categoryName = "") Q_DECL_OVERRIDE;
    virtual bool autograderYesOrNo(std::string prompt, std::string reprompt = "", std::string defaultValue = "") Q_DECL_OVERRIDE;
    bool catchExceptions() const;
    void clearTestResults();
    void clearTests();
    virtual bool containsCategory(const std::string& categoryName) Q_DECL_OVERRIDE;
    virtual void displayDiffs(const std::string& expectedOutput, const std::string& studentOutput,
                              const std::string& diffs, const std::string& diffFile = "",
                              int truncateHeight = -1) Q_DECL_OVERRIDE;
    TestResult getTestResult(const std::string& testFullName) const;
    bool isChecked(const std::string& testFullName) const;
    virtual bool isGraphicalUI() const Q_DECL_OVERRIDE;
    bool isStyleCheck() const;
    void onClick_autograde();
    void onClick_runManually();
    virtual int mainFunc() Q_DECL_OVERRIDE;
    virtual int runAllTestCases() Q_DECL_OVERRIDE;
    virtual std::string runAndCapture(
            int (* mainFunc)(),
            const std::string& cinInput = "",
            const std::string& outputFileName = "") Q_DECL_OVERRIDE;
    virtual void runStyleChecker() Q_DECL_OVERRIDE;
    virtual void runTest(stanfordcpplib::autograder::AutograderTest* test) Q_DECL_OVERRIDE;
    void setCatchExceptions(bool catchExceptions);
    virtual void setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) Q_DECL_OVERRIDE;
    virtual void setFailDetails(const autograder::UnitTestDetails& deets) Q_DECL_OVERRIDE;
    virtual void setTestCounts(int passCount, int testCount, bool isStyleCheck) Q_DECL_OVERRIDE;
    virtual void setTestDetails(const std::string& testFullName, UnitTestDetails deets) Q_DECL_OVERRIDE;
    void setTestingCompleted(bool visible);
    virtual bool setTestResult(const std::string& testFullName, TestResult result) Q_DECL_OVERRIDE;
    virtual bool setTestRuntime(const std::string& testFullName, long ms) Q_DECL_OVERRIDE;
    void setVisible(bool visible);
    void setWindowDescription(const std::string& description);
    virtual void showLateDays(const std::string& filename = LATE_DAYS_FILENAME) Q_DECL_OVERRIDE;
    virtual void showOutput(const std::string& output, bool showIfGraphical = true, bool showIfConsole = true) Q_DECL_OVERRIDE;
    virtual void showStudentTextFile(const std::string& filename, int maxWidth = 0, int maxHeight = 0) Q_DECL_OVERRIDE;

private:
    struct TestInfo {
        std::string name;
        std::string category;
        bool completed;
        GCheckBox* checkbox;
        GLabel* descriptionLabel;
        GLabel* runtimeLabel;
        GLabel* resultIconLabel;
        GLabel* detailsLabel;
        UnitTestDetails details;

        std::string getFullName() const {
            return category.empty() ? name : (category + "_" + name);
        }
    };

    GuiAutograder();   // forbid construction

    /*
     * Called when test thread crashes with an exception.
     */
    void failWithException(const std::string& testFullName,
                           const std::string& kind,
                           const std::string& desc,
                           const std::string& stackTrace = "");

    int getCheckedTestCount() const;
    void minimize(GContainer* category);
    void minimize(GContainer* category, bool minimized);
    void minimizeAll(bool minimized = true);
    void selectAll(GContainer* category, bool selected);
    void showTestDetails(const std::string& testFullName, bool force = false);
    bool showTestDetailsInSameWindow(const std::string& testFullName) const;
    void updateSouthText();

    static GuiAutograder* _instance;
    static GuiAutograder* _instanceStyleCheck;
    static const bool AUTO_EXPAND_FAILED_TESTS;
    static const int DEFAULT_WINDOW_WIDTH;
    static const int DEFAULT_WINDOW_HEIGHT;
    static const int MAX_VALUE_DISPLAY_LENGTH;
    static int MAX_WINDOW_HEIGHT;

    static const std::string COLOR_PASS;
    static const std::string COLOR_PASS_DARK_MODE;
    static const std::string COLOR_FAIL;
    static const std::string COLOR_FAIL_DARK_MODE;
    static const std::string COLOR_WARN;
    static const std::string COLOR_WARN_DARK_MODE;
    static const std::string COLOR_ZEBRA_STRIPE_1;
    static const std::string COLOR_ZEBRA_STRIPE_1_DARK_MODE;
    static const std::string COLOR_ZEBRA_STRIPE_2;
    static const std::string COLOR_ZEBRA_STRIPE_2_DARK_MODE;

    static const std::string ICON_FOLDER;
    static const std::string ICON_ABOUT_FILENAME;
    static const std::string ICON_AUTOGRADE_FILENAME;
    static const std::string ICON_DESELECT_ALL_FILENAME;
    static const std::string ICON_EXIT_FILENAME;
    static const std::string ICON_FAIL_FILENAME;
    static const std::string ICON_LATE_DAYS_FILENAME;
    static const std::string ICON_MANUAL_FILENAME;
    static const std::string ICON_MINIMIZE_FILENAME;
    static const std::string ICON_PASS_FILENAME;
    static const std::string ICON_PROGRESS_FILENAME;
    static const std::string ICON_RUNNING_FILENAME;
    static const std::string ICON_SELECT_ALL_FILENAME;
    static const std::string ICON_STYLE_CHECK_FILENAME;
    static const std::string ICON_UNKNOWN_FILENAME;
    static const std::string ICON_WARNING_FILENAME;

    GWindow* _window;
    GButton* _autogradeButton;
    GButton* _manualButton;
    GButton* _styleCheckButton;
    GButton* _lateDayButton;
    GButton* _aboutButton;
    GButton* _exitButton;
    Vector<GButton*> _callbackButtons;
    GContainer* _center;
    GCheckBox* _catchErrorsBox;
    // GLabel* _descriptionLabel;
    GLabel* _startLabel;     // TODO: null on construct/destruct
    GLabel* _southLabel;
    GContainer* _bigButtons;
    GContainer* _currentCategory;

    Map<std::string, GContainer*> _allCategories;
    Map<std::string, TestInfo*> _allTestInfo;
    Map<std::string, QPixmap> _iconStrip;

    int _passCount;
    int _testCount;
    // int _testPanelHeight = -1;
    // bool _allCategoriesHidden = false;
    // bool _checkboxesShown = true;
    bool _catchExceptions;
    bool _checkboxesShown;
    bool _testingIsInProgress;
};

} // namespace autograder
} // namespace stanfordcpplib

#endif // _guiautograder_h
