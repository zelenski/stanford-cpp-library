/*
 * File: autograderunittestgui.cpp
 * -------------------------------
 * This file contains the implementation of the AutograderUnitTestGui class.
 * See autograderunittestgui.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2018/08/27
 * - initial version
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
const std::string AutograderUnitTestGui::PASS_COLOR = "#006600";
const std::string AutograderUnitTestGui::FAIL_COLOR = "#660000";
const std::string AutograderUnitTestGui::WARN_COLOR = "#999900";
const std::string AutograderUnitTestGui::ZEBRA_STRIPE_COLOR_1 = "#fafafa";
const std::string AutograderUnitTestGui::ZEBRA_STRIPE_COLOR_2 = "#ebebeb";

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
          _descriptionLabel(nullptr),
          _southLabel(nullptr),
          _currentCategory(nullptr),
          _passCount(0),
          _testCount(0),
          _catchExceptions(true),
          _checkboxesShown(true),
          _runInSeparateThreads(true),
          _testingIsInProgress(false) {
    GThread::runOnQtGuiThread([this]() {
        _window = new GWindow(700, 400);   // TODO: fix size
        // _window->setCloseOperation(GWindow::CLOSE_HIDE);
        _window->setExitOnClose(true);
        _window->setTitle("Autograder Tests");
        _window->setVisible(false);

        // _descriptionLabel = new GLabel("Autograder Tests");
        // align center
        // GFont::changeFontSize(_descriptionLabel, -1);

        _catchErrorsBox = new GCheckBox("&Catch exceptions in tests", /* checked */ true);
        _runTestsInThreadsBox = new GCheckBox("Run &tests in separate threads", /* checked */ true);
        _southLabel = new GLabel(" ");
        _southLabel->setIcon("progress.gif");
        // align left

        // layout
        GContainer* north = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        north->add(_catchErrorsBox);
        north->add(_runTestsInThreadsBox);
        _window->addToRegion(north, GWindow::REGION_NORTH);

        _center = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        _center->setBackground("blue");   // TODO: remove
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
        selectAllButton->setIcon("checkbox-checked.gif");
        selectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(selectAllButton, -2);
        selectAllButton->setTooltip("Double-click to select all tests from all categories.");
        selectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ true);
        });

        GButton* deselectAllButton = new GButton("None");
        deselectAllButton->setIcon("checkbox-unchecked.gif");
        deselectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(deselectAllButton, -2);
        deselectAllButton->setTooltip("Double-click to deselect all tests from all categories.");
        deselectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ false);
        });

        GButton* minimizeButton = new GButton("Hide");
        minimizeButton->setIcon("minus.gif");
        GFont::changeFontSize(minimizeButton, -2);
        deselectAllButton->setTooltip("Double-click to minimize all categories.");
        deselectAllButton->setActionListener([this, category]() {
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

        top->setBackground("yellow");   // TODO: remove
        category->add(top);
    }

    // TODO: central vertical container
    _center->add(category);
    GLayout::forceUpdate(category);
    GLayout::forceUpdate(_window->getWidget());
}

void AutograderUnitTestGui::addTest(const std::string& testName, const std::string& categoryName) {
    if (!_allCategories.containsKey(categoryName)) {
        addCategory(categoryName);
    }

    _testCount++;
    GContainer* category = _allCategories[categoryName];
    _currentCategory = category;

    GContainer* test = new GContainer(GContainer::LAYOUT_BORDER);
    std::string bgColor = _testCount % 2 == 0 ? ZEBRA_STRIPE_COLOR_1 : ZEBRA_STRIPE_COLOR_2;
    test->setBackground(bgColor);
    GContainer* testWest = new GContainer;
    testWest->setBackground(bgColor);

    TestInfo* testInfo = new TestInfo;
    testInfo->name = testName;
    testInfo->category = categoryName;
    testInfo->checkbox = new GCheckBox("", /* checked */ true);
    if (!isStyleCheck() && _checkboxesShown) {
        testWest->add(testInfo->checkbox);
    }
    _allTestInfo[testInfo->getFullName()] = testInfo;

    GLabel* testCountLabel = new GLabel(integerToString(_testCount) + ". ");
    // TODO: put into map?
    // TODO: align right
    testCountLabel->setFont(GFont::deriveQFont(testCountLabel->getFont(), QFont::Bold));
    testWest->add(testCountLabel);
    test->addToRegion(testWest, GContainer::REGION_WEST);

    std::string testNameShort = regexReplace(testName, "Test_(?:[0-9]{1,5}_)?", "");
    testInfo->descriptionLabel = new GLabel(testNameShort);
    testInfo->descriptionLabel->setTooltip("Click to see detailed results from this test.");
    GFont::changeFontSize(testInfo->descriptionLabel, -1);
    testInfo->descriptionLabel->setFont(GFont::deriveQFont(testInfo->descriptionLabel->getFont(), QFont::Bold));
    test->addToRegion(testInfo->descriptionLabel, GContainer::REGION_CENTER);
    // TODO: set mouse/click listener
    // testInfo->descriptionLabel->set
    // TODO: put into map?
    // _allTestInfo.put(testInfo->descriptionLabel, testInfo);

    testInfo->resultLabel = new GLabel;
    testInfo->resultLabel->setText("        ");
    testInfo->resultLabel->setIcon("running.gif");
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
    // checkVisibility();
}

bool AutograderUnitTestGui::catchExceptions() const {
    return _catchExceptions;
}

void AutograderUnitTestGui::clearTestResults() {
    // TODO
}

void AutograderUnitTestGui::clearTests() {
    // TODO
}

int AutograderUnitTestGui::getCheckedTestCount() const {
    // TODO - for each test, if checkbox is checked, count++
    return 0;
}

bool AutograderUnitTestGui::isChecked(const std::string& /*testFullName*/) const {
    // TODO
    return true;
}

bool AutograderUnitTestGui::isStyleCheck() const {
    return this == _instanceStyleCheck;
}

void AutograderUnitTestGui::minimize(GContainer* /*category*/) {
    // TODO
}

bool AutograderUnitTestGui::runTestsInSeparateThreads() const {
    return _runInSeparateThreads;
}

void AutograderUnitTestGui::selectAll(GContainer* /*category*/, bool /*selected*/) {
    // TODO
}

void AutograderUnitTestGui::setCatchExceptions(bool catchExceptions) {
    _catchExceptions = catchExceptions;
}

void AutograderUnitTestGui::setRunTestsInSeparateThreads(bool runInSeparateThreads) {
    _runInSeparateThreads = runInSeparateThreads;
}

void AutograderUnitTestGui::setTestCounts(int /*passCount*/, int /*testCount*/) {
    // TODO
}

void AutograderUnitTestGui::setTestDetails(const std::string& /*testName*/, ::autograder::UnitTestDetails /*deets*/) {
    // TODO
}

void AutograderUnitTestGui::setTestingCompleted(bool /*visible*/) {
    // TODO
}

void AutograderUnitTestGui::setTestResult(const std::string& /*test*/, TestResult /*result*/) {
    // TODO
}

void AutograderUnitTestGui::setTestRuntime(const std::string& /*testName*/, long /*ms*/) {
    // TODO
}

void AutograderUnitTestGui::setVisible(bool visible) {
    _window->setVisible(visible);
}

void AutograderUnitTestGui::setWindowDescription(const std::string& /*description*/) {
    // TODO
}

void AutograderUnitTestGui::updateSouthText() {
    std::string text = "passed " + integerToString(_passCount)
            + " / " + integerToString(getCheckedTestCount()) + " tests";
    if (_testingIsInProgress) {
        text += " (running ...)";
        if (_southLabel->getIcon() == "") {
            _southLabel->setIcon("progress.gif");
        }
    } else {
        text += " (complete)";
        _southLabel->setIcon("");
    }
    _southLabel->setText(text);
}

} // namespace autograder
} // namespace stanfordcpplib
