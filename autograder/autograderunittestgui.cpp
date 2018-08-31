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
#include "ginteractors.h"
#include "gthread.h"
#include "unittestdetails.h"

namespace stanfordcpplib {
namespace autograder {

AutograderUnitTestGui* AutograderUnitTestGui::_instance = nullptr;
AutograderUnitTestGui* AutograderUnitTestGui::_instanceStyleCheck = nullptr;

static const std::string PASS_COLOR = "#006600";
static const std::string FAIL_COLOR = "#660000";
static const std::string WARN_COLOR = "#999900";
static const std::string ZEBRA_STRIPE_COLOR_1 = "#fafafa";
static const std::string ZEBRA_STRIPE_COLOR_2 = "#ebebeb";

struct TestInfo {
    std::string name;
    std::string category;
    GCheckBox* checkbox;
    GLabel* description;
    GLabel* result;
    UnitTestDetails details;

    std::string getFullName() const {
        return category.empty() ? name : (category + "_" + name);
    }
};

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
          _catchErrorsBox(nullptr),
          _runTestsInThreadsBox(nullptr),
          _descriptionLabel(nullptr),
          _southLabel(nullptr),
          _passCount(0),
          _testCount(0),
          _catchExceptions(true),
          _runInSeparateThreads(true),
          _testingIsInProgress(false) {
    GThread::runOnQtGuiThread([this]() {
        _window = new GWindow(700, 400);   // TODO: fix size
        _window->setCloseOperation(GWindow::CLOSE_HIDE);
        _window->setTitle("Autograder");
        _window->setVisible(false);

        _descriptionLabel = new GLabel("Autograder Tests");
        // align center
        // shrink font by 1?

        _catchErrorsBox = new GCheckBox("&Catch exceptions in tests", /* checked */ true);
        _runTestsInThreadsBox = new GCheckBox("Run &tests in separate threads", /* checked */ true);
        _southLabel = new GLabel(" ");
        _southLabel->setIcon("progress.gif");
        // align left

        // layout
        _window->addToRegion(_catchErrorsBox, GWindow::REGION_NORTH);
        _window->addToRegion(_runTestsInThreadsBox, GWindow::REGION_NORTH);
    });
}

void AutograderUnitTestGui::addCategory(const std::string& category) {
    if (_allCategories.containsKey(category)) {
        return;
    }
    // TODO
    // - set as current category
    // - make box container
    //       - titled border showing name
    //       -
}

void AutograderUnitTestGui::addTest(const std::string& /*test*/, const std::string& category) {
    addCategory(category);
    // TODO
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

bool AutograderUnitTestGui::runTestsInSeparateThreads() const {
    return _runInSeparateThreads;
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

void AutograderUnitTestGui::setVisible(bool /*visible*/) {
    // TODO
}

void AutograderUnitTestGui::setWindowDescription(const std::string& /*description*/) {
    // TODO
}

void AutograderUnitTestGui::updateSouthText() {
    std::string text = "passed " + _passCount + " / " + getCheckedTestCount() + " tests";
    if (_testingIsInProgress) {
        text += " (running ...)";
        if (_southLabel.getIcon() == "") {
            _southLabel.setIcon("progress.gif");
        }
    } else {
        text += " (complete)";
        _southLabel.setIcon("");
    }
    _southLabel.setText(text);
}

} // namespace autograder
} // namespace stanfordcpplib
