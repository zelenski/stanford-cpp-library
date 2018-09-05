/*
 * File: autograderunittestgui.cpp
 * -------------------------------
 * This file contains the implementation of the AutograderUnitTestGui class.
 * See autograderunittestgui.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2018/09/03
 * - basic implementation completed; runnable tests with clickable interactors
 * @version 2018/08/27
 * - initial incomplete version
 * @since 2018/08/27
 */

#include "autograderunittestgui.h"
#include "autograder.h"
#include "autogradertest.h"
#include "gfont.h"
#include "ginteractors.h"
#include "gthread.h"
#include "regexpr.h"
#include "strlib.h"
#include "unittestdetails.h"

namespace stanfordcpplib {
namespace autograder {

AutograderUnitTestGui* AutograderUnitTestGui::_instance = nullptr;
AutograderUnitTestGui* AutograderUnitTestGui::_instanceStyleCheck = nullptr;
const int AutograderUnitTestGui::DEFAULT_WINDOW_WIDTH = 500;
const int AutograderUnitTestGui::DEFAULT_WINDOW_HEIGHT = 700;
int AutograderUnitTestGui::MAX_WINDOW_HEIGHT = -1;
const std::string AutograderUnitTestGui::COLOR_PASS = "#006600";
const std::string AutograderUnitTestGui::COLOR_FAIL = "#660000";
const std::string AutograderUnitTestGui::COLOR_WARN = "#999900";
std::string AutograderUnitTestGui::COLOR_NORMAL = "#eeeeee";
const std::string AutograderUnitTestGui::COLOR_ZEBRA_STRIPE_1 = "#fafafa";
const std::string AutograderUnitTestGui::COLOR_ZEBRA_STRIPE_2 = "#ebebeb";
const std::string AutograderUnitTestGui::ICON_DESELECT_ALL_FILENAME = "checkbox-unchecked.gif";
const std::string AutograderUnitTestGui::ICON_FAIL_FILENAME = "fail.gif";
const std::string AutograderUnitTestGui::ICON_MINIMIZE_FILENAME = "minus.gif";
const std::string AutograderUnitTestGui::ICON_PASS_FILENAME = "pass.gif";
const std::string AutograderUnitTestGui::ICON_PROGRESS_FILENAME = "progress.gif";
const std::string AutograderUnitTestGui::ICON_RUNNING_FILENAME = "running.gif";
const std::string AutograderUnitTestGui::ICON_SELECT_ALL_FILENAME = "checkbox-checked.gif";
const std::string AutograderUnitTestGui::ICON_UNKNOWN_FILENAME = "unknown.gif";
const std::string AutograderUnitTestGui::ICON_WARNING_FILENAME = "warning.gif";

AutograderUnitTestGui* AutograderUnitTestGui::instance(bool isStyleCheck) {
    if (isStyleCheck) {
        if (!_instanceStyleCheck) {
            _instanceStyleCheck = new AutograderUnitTestGui();
        }
        return _instanceStyleCheck;
    } else {
        if (!_instance) {
            _instance = new AutograderUnitTestGui();
        }
        return _instance;
    }
}

AutograderUnitTestGui::AutograderUnitTestGui()
        : _window(nullptr),
          _center(nullptr),
          _catchErrorsBox(nullptr),
          _runTestsInThreadsBox(nullptr),
          // _descriptionLabel(nullptr),
          _southLabel(nullptr),
          _currentCategory(nullptr),
          _passCount(0),
          _testCount(0),
          _catchExceptions(true),
          _checkboxesShown(true),
          _runInSeparateThreads(true),
          _testingIsInProgress(false) {
    GThread::runOnQtGuiThread([this]() {
        _window = new GWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);   // TODO: fix size
        // _window->setCloseOperation(GWindow::CLOSE_HIDE);
        _window->setExitOnClose(true);
        _window->setTitle("Autograder Tests");
        _window->setVisible(false);

        if (MAX_WINDOW_HEIGHT < 0) {
            // make it nearly the entire screen height
            MAX_WINDOW_HEIGHT = (int) (GWindow::getScreenHeight() - 80);
        }

        // _descriptionLabel = new GLabel("Autograder Tests");
        // align center
        // GFont::changeFontSize(_descriptionLabel, -1);

        _catchErrorsBox = new GCheckBox("&Catch exceptions in tests", /* checked */ true);
        _runTestsInThreadsBox = new GCheckBox("Run &tests in separate threads", /* checked */ true);
        _southLabel = new GLabel(" ");
        _southLabel->setIcon(ICON_PROGRESS_FILENAME);
        _southLabel->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        // _southLabel->setBackground("red");   // TODO: remove
        _window->addToRegion(_southLabel, GWindow::REGION_SOUTH);
        // _southLabel->getWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        _window->setRegionHorizontalAlignment(GWindow::REGION_SOUTH, ALIGN_LEFT);

        // align left

        // layout
        GContainer* north = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        north->add(_catchErrorsBox);
        north->add(_runTestsInThreadsBox);
        _window->addToRegion(north, GWindow::REGION_NORTH);
        _window->setRegionHorizontalAlignment(GWindow::REGION_NORTH, ALIGN_LEFT);

        _center = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        _center->setPadding(0);
        _center->setMargin(0);
        _center->setSpacing(1);
        GScrollPane* scroll = new GScrollPane(_center);
        _window->addToRegion(scroll, GWindow::REGION_CENTER);
    });
}

void AutograderUnitTestGui::addCategory(const std::string& categoryName) {
    if (_allCategories.containsKey(categoryName)) {
        return;
    }

    GContainer* category = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
    category->setMargin(0);
    category->setPadding(0);
    category->setSpacing(0);
    _currentCategory = category;
    _allCategories[categoryName] = category;

    if (!isStyleCheck()) {
        // top row of 'select/deselect all' buttons
        GContainer* top = new GContainer(GContainer::LAYOUT_FLOW_HORIZONTAL);
        top->setHorizontalAlignment(ALIGN_LEFT);

        GButton* selectAllButton = new GButton("All");
        selectAllButton->setIcon(ICON_SELECT_ALL_FILENAME);
        selectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(selectAllButton, -2);
        selectAllButton->setTooltip("Double-click to select all tests from all categories.");
        selectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ true);
        });
        selectAllButton->setDoubleClickListener([this]() {
            selectAll(_center, /* selected */ true);
        });

        GButton* deselectAllButton = new GButton("None");
        deselectAllButton->setIcon(ICON_SELECT_ALL_FILENAME);
        deselectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(deselectAllButton, -2);
        deselectAllButton->setTooltip("Double-click to deselect all tests from all categories.");
        deselectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ false);
        });
        deselectAllButton->setDoubleClickListener([this]() {
            selectAll(_center, /* selected */ false);
        });

        GButton* minimizeButton = new GButton("Hide");
        minimizeButton->setIcon(ICON_MINIMIZE_FILENAME);
        GFont::changeFontSize(minimizeButton, -2);
        minimizeButton->setTooltip("Double-click to minimize all categories.");
        minimizeButton->setActionListener([this, category]() {
            minimize(category);
        });

        if (_checkboxesShown) {
            top->add(selectAllButton);
            top->add(deselectAllButton);
        }
        top->add(minimizeButton);

        if (!categoryName.empty()) {
            GLabel* nameLabel = new GLabel(categoryName);
            GFont::changeFontSize(nameLabel, -1);
            top->add(nameLabel);
        }

        category->add(top);
    }

    _center->add(category);
    GLayout::forceUpdate(category);
}

void AutograderUnitTestGui::addTest(const std::string& testName, const std::string& categoryName) {
    if (!_allCategories.containsKey(categoryName)) {
        addCategory(categoryName);
    }

    _testCount++;
    GContainer* category = _allCategories[categoryName];
    category->setName("category");
    _currentCategory = category;

    GContainer* test = new GContainer(GContainer::LAYOUT_BORDER);
    test->setName("testPanel");
    test->setPadding(0);
    std::string bgColor = _testCount % 2 == 0 ? COLOR_ZEBRA_STRIPE_1 : COLOR_ZEBRA_STRIPE_2;
    test->setBackground(bgColor);
    GContainer* testWest = new GContainer;
    testWest->setBackground(bgColor);
    testWest->setPadding(0);

    TestInfo* testInfo = new TestInfo;
    testInfo->name = testName;
    testInfo->category = categoryName;
    testInfo->completed = false;
    testInfo->checkbox = new GCheckBox("", /* checked */ true);
    if (!isStyleCheck() && _checkboxesShown) {
        testWest->add(testInfo->checkbox);
    }
    std::string testFullName = testInfo->getFullName();
    _allTestInfo[testFullName] = testInfo;

    GLabel* testNumberLabel = new GLabel(padLeft(integerToString(_testCount), /* digits */ 3, /* fill */ '0') + ". ");
    // TODO: put into map?
    // TODO: align test number right?
    testNumberLabel->setFont(GFont::deriveQFont(testNumberLabel->getFont(), QFont::Bold));
    testWest->add(testNumberLabel);
    test->addToRegion(testWest, GContainer::REGION_WEST);

    std::string testNameShort = regexReplace(testName, "Test_(?:[0-9]{1,5}_)?", "");
    testInfo->descriptionLabel = new GLabel(testNameShort);
    testInfo->descriptionLabel->setTooltip("Click to see detailed results from this test.");
    GFont::changeFontSize(testInfo->descriptionLabel, -1);
    testInfo->descriptionLabel->setFont(GFont::deriveQFont(testInfo->descriptionLabel->getFont(), QFont::Bold));
    testInfo->descriptionLabel->setActionListener([this, testFullName]() {
        showTestDetails(testFullName);
    });
    test->addToRegion(testInfo->descriptionLabel, GContainer::REGION_CENTER);
    // TODO: set mouse/click listener
    // testInfo->descriptionLabel->set
    // TODO: put into map?
    // _allTestInfo.put(testInfo->descriptionLabel, testInfo);

    testInfo->resultLabel = new GLabel;
    testInfo->resultLabel->setText("        ");
    testInfo->resultLabel->setIcon(ICON_RUNNING_FILENAME);
    GFont::changeFontSize(testInfo->resultLabel, -2);
    // TODO: setHorizontalTextPosition LEFT
    testInfo->resultLabel->setTooltip("Click to see detailed results from this test.");
    // TODO: set mouse/click listener
    // testInfo->resultLabel->addMouseListener(this);
    test->addToRegion(testInfo->resultLabel, GContainer::REGION_EAST);
    // TODO: put into map?
    // _allTestInfo.put(testInfo->resultLabel, testInfo);

    _currentCategory->add(test);

    // TODO: check height, visibility of tests/categories, scroll pane (validate/repaint)
    // set height to MAX_...
    // checkVisibility();
}

bool AutograderUnitTestGui::catchExceptions() const {
    return _catchExceptions;
}

void AutograderUnitTestGui::clearTestResults() {
    _passCount = 0;
    for (TestInfo* testInfo : _allTestInfo.values()) {
        if (!testInfo) {
            continue;
        }
        ::autograder::UnitTestDetails deets;   // clear it out
        testInfo->details = deets;
        testInfo->descriptionLabel->setForeground(COLOR_NORMAL);
        testInfo->resultLabel->setText("");
        testInfo->resultLabel->setIcon(ICON_RUNNING_FILENAME);
    }
    updateSouthText();
}

void AutograderUnitTestGui::clearTests() {
    // TODO
}

int AutograderUnitTestGui::getCheckedTestCount() const {
    // TODO - for each test, if checkbox is checked, count++
    return 0;
}

AutograderUnitTestGui::TestResult AutograderUnitTestGui::getTestResult(const std::string& testFullName) const {
    if (!_allTestInfo.containsKey(testFullName)) {
        return TEST_RESULT_UNKNOWN;
    }
    TestInfo* testInfo = _allTestInfo[testFullName];
    if (testInfo->descriptionLabel->getForeground() == COLOR_FAIL) {
        return TEST_RESULT_FAIL;
    } else if (testInfo->descriptionLabel->getForeground() == COLOR_WARN) {
        return TEST_RESULT_WARN;
    } else if (testInfo->descriptionLabel->getForeground() == COLOR_PASS) {
        return TEST_RESULT_PASS;
    } else if (testInfo->completed) {
        return testInfo->details.passed ? TEST_RESULT_PASS : TEST_RESULT_FAIL;
    } else {
        return TEST_RESULT_UNKNOWN;
    }
}

bool AutograderUnitTestGui::isChecked(const std::string& testFullName) const {
    return _allTestInfo.containsKey(testFullName)
            && _allTestInfo[testFullName]->checkbox->isChecked();
}

bool AutograderUnitTestGui::isStyleCheck() const {
    return this == _instanceStyleCheck;
}

void AutograderUnitTestGui::minimize(GContainer* category) {
    minimize(category, category->getName() != "categoryHidden");
}

void AutograderUnitTestGui::minimize(GContainer* category, bool minimized) {
    if (minimized) {
        category->setName("categoryHidden");
    } else {
        category->setName("category");
    }

    for (GInteractor* interactor : category->getInteractors()) {
        if (interactor->getType() != "GContainer"
                || interactor->getName() != "testPanel") {
            continue;
        }

        // TODO: set height

        interactor->setVisible(!minimized);
    }
}

bool AutograderUnitTestGui::runTestsInSeparateThreads() const {
    return _runInSeparateThreads;
}

void AutograderUnitTestGui::selectAll(GContainer* category, bool selected) {
    for (GInteractor* interactor : category->getDescendents("GCheckBox")) {
        GCheckBox* checkbox = static_cast<GCheckBox*>(interactor);
        checkbox->setChecked(selected);
    }
}

void AutograderUnitTestGui::setCatchExceptions(bool catchExceptions) {
    _catchExceptions = catchExceptions;
}

void AutograderUnitTestGui::setRunTestsInSeparateThreads(bool runInSeparateThreads) {
    _runInSeparateThreads = runInSeparateThreads;
}

void AutograderUnitTestGui::setTestCounts(int passCount, int testCount) {
    _passCount = passCount;
    _testCount = testCount;
    updateSouthText();
}

void AutograderUnitTestGui::setTestDetails(const std::string& testFullName, ::autograder::UnitTestDetails deets) {
    if (!_allTestInfo.containsKey(testFullName)) {
        return;
    }

    TestInfo* testInfo = _allTestInfo[testFullName];

    // don't replace test details if a test already failed here
    TestResult existingResult = getTestResult(testFullName);
    if (existingResult == TEST_RESULT_FAIL || existingResult == TEST_RESULT_WARN) {
        if (!deets.overwrite) {
            return;
        }
    }

    testInfo->details = deets;
}

void AutograderUnitTestGui::setTestingCompleted(bool completed) {
    _testingIsInProgress = !completed;
    updateSouthText();
}

bool AutograderUnitTestGui::setTestResult(const std::string& testFullName, TestResult result) {
    if (!_allTestInfo.containsKey(testFullName)) {
        return false;   // test not found
    }

    // if test already failed previously, don't set back to passed
    TestResult existingResult = getTestResult(testFullName);
    if ((existingResult == TEST_RESULT_FAIL || existingResult == TEST_RESULT_WARN)
            && result != TEST_RESULT_FAIL) {
        return true;
    }

    TestInfo* testInfo = _allTestInfo[testFullName];
    if (result != TEST_RESULT_UNKNOWN) {
        testInfo->completed = true;
    }

    std::string iconFile;
    std::string labelForegroundColor;
    switch (result) {
    case TEST_RESULT_FAIL:
        iconFile = ICON_FAIL_FILENAME;
        labelForegroundColor = COLOR_FAIL;
        break;
    case TEST_RESULT_PASS:
        iconFile = ICON_PASS_FILENAME;
        labelForegroundColor = COLOR_PASS;
        _passCount++;
        break;
    case TEST_RESULT_WARN:
        iconFile = ICON_WARNING_FILENAME;
        labelForegroundColor = COLOR_WARN;
        break;
    case TEST_RESULT_UNKNOWN:
    default:
        iconFile = ICON_UNKNOWN_FILENAME;
        labelForegroundColor = COLOR_NORMAL;
        break;
    }

    testInfo->resultLabel->setIcon(iconFile);
    testInfo->descriptionLabel->setForeground(labelForegroundColor);
    updateSouthText();
    return true;
}

bool AutograderUnitTestGui::setTestRuntime(const std::string& testFullName, long ms) {
    if (!_allTestInfo.containsKey(testFullName)) {
        return false;   // test not found
    }

    std::string text = " (" + longToString(ms) + " ms)";
    TestInfo* testInfo = _allTestInfo[testFullName];
    testInfo->resultLabel->setText(text);
    return true;
}

void AutograderUnitTestGui::setVisible(bool visible) {
    _window->setVisible(visible);
}

void AutograderUnitTestGui::setWindowDescription(const std::string& /*description*/) {
    // TODO: set text of _descriptionLabel
}

void AutograderUnitTestGui::showTestDetails(const std::string& testFullName) {
    // TODO: proper graphical implementation
    TestInfo* testInfo = _allTestInfo[testFullName];
    if (!testInfo) {
        return;
    }

    std::cout << "details for test " << testFullName << ":" << std::endl
              << testInfo->details << std::endl;
}

void AutograderUnitTestGui::updateSouthText() {
    std::string text = "passed " + integerToString(_passCount)
            + " / " + integerToString(getCheckedTestCount()) + " tests";
    if (_testingIsInProgress) {
        text += " (running ...)";
        if (_southLabel->getIcon() == "") {
            _southLabel->setIcon(ICON_PROGRESS_FILENAME);
        }
    } else {
        text += " (complete)";
        _southLabel->setIcon("");
    }
    _southLabel->setText(text);
}

} // namespace autograder
} // namespace stanfordcpplib
