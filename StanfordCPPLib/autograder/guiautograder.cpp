/*
 * File: guiautograder.cpp
 * -----------------------
 * This file contains the implementation of the AutograderUnitTestGui class.
 * See autograderunittestgui.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2019/05/05
 * - dark mode support
 * @version 2019/04/23
 * - reset std::cout/cerr flags on every test run
 * @version 2019/04/22
 * - now uses image strip file for icons
 * @version 2019/04/20
 * - display expected/actual values using toPrintable to show non-printable characters better
 * @version 2018/12/01
 * - added printf messages to indicate each test as it runs/finishes
 * @version 2018/10/07
 * - icon constants and path fixes; start message word wrap
 * @version 2018/10/06
 * - initial version for 18au; clickable test cases with expanding details
 * @version 2018/10/03
 * - renamed to AutograderGUI; various windows/UI merged into one window
 * @version 2018/09/03
 * - basic implementation completed; runnable tests with clickable interactors
 * @version 2018/08/27
 * - initial incomplete version
 * @since 2018/08/27
 */

#define INTERNAL_INCLUDE 1
#include "guiautograder.h"
#define INTERNAL_INCLUDE 1
#include "autogradertest.h"
#define INTERNAL_INCLUDE 1
#include "bitstream.h"
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gconsolewindow.h"
#define INTERNAL_INCLUDE 1
#include "gdiffgui.h"
#define INTERNAL_INCLUDE 1
#include "gdiffimage.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "ginputpanel.h"
#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "goptionpane.h"
#define INTERNAL_INCLUDE 1
#include "gscrollpane.h"
#define INTERNAL_INCLUDE 1
#include "gspacer.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "ioutils.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "regexpr.h"
#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "stylecheck.h"
#define INTERNAL_INCLUDE 1
#include "testresultprinter.h"
#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#undef INTERNAL_INCLUDE
#include <sstream>

// student's main function
extern int main();

namespace stanfordcpplib {
namespace autograder {

/*static*/ GuiAutograder* GuiAutograder::_instance = nullptr;
/*static*/ GuiAutograder* GuiAutograder::_instanceStyleCheck = nullptr;
/*static*/ const bool GuiAutograder::AUTO_EXPAND_FAILED_TESTS = false;
/*static*/ const int GuiAutograder::DEFAULT_WINDOW_WIDTH  = 500;
/*static*/ const int GuiAutograder::DEFAULT_WINDOW_HEIGHT = 700;
/*static*/ const int GuiAutograder::MAX_VALUE_DISPLAY_LENGTH = 150;
/*static*/ int GuiAutograder::MAX_WINDOW_HEIGHT = -1;
/*static*/ const std::string GuiAutograder::COLOR_PASS                     = "#006600";
/*static*/ const std::string GuiAutograder::COLOR_PASS_DARK_MODE           = "#55ff44";
/*static*/ const std::string GuiAutograder::COLOR_FAIL                     = "#660000";
/*static*/ const std::string GuiAutograder::COLOR_FAIL_DARK_MODE           = "#f47862";
/*static*/ const std::string GuiAutograder::COLOR_WARN                     = "#999900";
/*static*/ const std::string GuiAutograder::COLOR_WARN_DARK_MODE           = "#eeee22";
/*static*/ const std::string GuiAutograder::COLOR_ZEBRA_STRIPE_1           = "#fafafa";
/*static*/ const std::string GuiAutograder::COLOR_ZEBRA_STRIPE_1_DARK_MODE = "#1e1e1e";
/*static*/ const std::string GuiAutograder::COLOR_ZEBRA_STRIPE_2           = "#ebebeb";
/*static*/ const std::string GuiAutograder::COLOR_ZEBRA_STRIPE_2_DARK_MODE = "#2d2d2d";
/*static*/ const std::string GuiAutograder::ICON_FOLDER = "";
/*static*/ const std::string GuiAutograder::ICON_ABOUT_FILENAME        = GuiAutograder::ICON_FOLDER + "helpbig.gif";
/*static*/ const std::string GuiAutograder::ICON_AUTOGRADE_FILENAME    = GuiAutograder::ICON_FOLDER + "check.gif";
/*static*/ const std::string GuiAutograder::ICON_DESELECT_ALL_FILENAME = GuiAutograder::ICON_FOLDER + "checkbox-unchecked.gif";
/*static*/ const std::string GuiAutograder::ICON_EXIT_FILENAME         = GuiAutograder::ICON_FOLDER + "stop.gif";
/*static*/ const std::string GuiAutograder::ICON_FAIL_FILENAME         = GuiAutograder::ICON_FOLDER + "fail.gif";
/*static*/ const std::string GuiAutograder::ICON_LATE_DAYS_FILENAME    = GuiAutograder::ICON_FOLDER + "calendar.gif";
/*static*/ const std::string GuiAutograder::ICON_MANUAL_FILENAME       = GuiAutograder::ICON_FOLDER + "play.gif";
/*static*/ const std::string GuiAutograder::ICON_MINIMIZE_FILENAME     = GuiAutograder::ICON_FOLDER + "minus.gif";
/*static*/ const std::string GuiAutograder::ICON_PASS_FILENAME         = GuiAutograder::ICON_FOLDER + "pass.gif";
/*static*/ const std::string GuiAutograder::ICON_PROGRESS_FILENAME     = GuiAutograder::ICON_FOLDER + "progress.gif";
/*static*/ const std::string GuiAutograder::ICON_RUNNING_FILENAME      = GuiAutograder::ICON_FOLDER + "running.gif";
/*static*/ const std::string GuiAutograder::ICON_SELECT_ALL_FILENAME   = GuiAutograder::ICON_FOLDER + "checkbox-checked.gif";
/*static*/ const std::string GuiAutograder::ICON_STYLE_CHECK_FILENAME  = GuiAutograder::ICON_FOLDER + "magnifier.gif";
/*static*/ const std::string GuiAutograder::ICON_UNKNOWN_FILENAME      = GuiAutograder::ICON_FOLDER + "unknown.gif";
/*static*/ const std::string GuiAutograder::ICON_WARNING_FILENAME      = GuiAutograder::ICON_FOLDER + "warn.gif";

GuiAutograder* GuiAutograder::instance() {
    if (!_instance) {
        _instance = new GuiAutograder();
    }
    return _instance;
}

GuiAutograder::GuiAutograder()
        : Autograder(),
          _window(nullptr),
          _autogradeButton(nullptr),
          _manualButton(nullptr),
          _styleCheckButton(nullptr),
          _lateDayButton(nullptr),
          _aboutButton(nullptr),
          _exitButton(nullptr),
          _center(nullptr),
          _catchErrorsBox(nullptr),
          // _descriptionLabel(nullptr),
          _startLabel(nullptr),
          _southLabel(nullptr),
          _bigButtons(nullptr),
          _currentCategory(nullptr),
          _passCount(0),
          _testCount(0),
          _catchExceptions(true),
          _checkboxesShown(true),
          _testingIsInProgress(false) {
    GThread::runOnQtGuiThread([this]() {
        _window = new GWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);   // TODO: fix size
        // _window->setCloseOperation(GWindow::CLOSE_HIDE);
        _window->setVisible(false);
        _window->setExitOnClose(true);
        _window->setTitle("Autograder Tests");

        if (MAX_WINDOW_HEIGHT < 0) {
            // make it nearly the entire screen height
            MAX_WINDOW_HEIGHT = static_cast<int>(GWindow::getScreenHeight() - 80);
        }

        // read and unpack autograder icon image strip
        const std::string ICON_STRIP_FILE = "iconstrip-autograder.png";
        const Vector<std::string> IMAGES {
            "calendar.gif",
            "check.gif",
            "helpbig.gif",
            "magnifier.gif",
            "play.gif",
            "pause.gif",
            "stop.gif",
            "textfile.gif",

            "fail.gif",
            "help.gif",
            "pass.gif",
            "running.gif",
            "warn.gif",
            "checkbox-checked.gif",
            "checkbox-unchecked.gif",
            "minus.gif"
        };
        const GDimension LARGE_SIZE(32, 32);
        const GDimension SMALL_SIZE(16, 16);
        Vector<GDimension> IMAGE_SIZES = {
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,
            LARGE_SIZE,

            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE,
            SMALL_SIZE
        };
        _iconStrip = GConsoleWindow::unpackImageStrip(ICON_STRIP_FILE, IMAGES, IMAGE_SIZES);
        if (_iconStrip.isEmpty()) {
            error("GuiAutograder::constructor: could not find autograder image strip file " + ICON_STRIP_FILE);
        }

        _catchErrorsBox = new GCheckBox("&Catch exceptions in tests", /* checked */ true);
        _catchErrorsBox->setActionListener([this]() {
            setCatchExceptions(_catchErrorsBox->isChecked());
        });

        _startLabel = new GLabel(" ");
        // _startLabel->setWidth(DEFAULT_WINDOW_WIDTH - 20);
        _startLabel->setWordWrap(true);

        _southLabel = new GLabel(" ", _iconStrip[ICON_PROGRESS_FILENAME]);
        _southLabel->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        _window->addToRegion(_southLabel, GWindow::REGION_SOUTH);
        // _southLabel->getWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        _window->setRegionHorizontalAlignment(GWindow::REGION_SOUTH, ALIGN_LEFT);

        // container of big buttons to click to run the autograder
        _bigButtons = new GContainer();

        _autogradeButton = new GButton("Automated\ntests", _iconStrip[ICON_AUTOGRADE_FILENAME]);
        _autogradeButton->setActionListener([this]() {
            onClick_autograde();
        });
        _bigButtons->add(_autogradeButton);

        _manualButton = new GButton("Run\nmanually", _iconStrip[ICON_MANUAL_FILENAME]);
        _manualButton->setActionListener([this]() {
            onClick_runManually();
        });
        _bigButtons->add(_manualButton);

        _styleCheckButton = new GButton("Style\nchecker", _iconStrip[ICON_STYLE_CHECK_FILENAME]);
        _styleCheckButton->setActionListener([this]() {
            runStyleChecker();
        });

        for (int i = 0; i < _flags.callbackButtons.size(); i++) {
            GButton* callbackButton = new GButton(_flags.callbackButtons[i].text, _flags.callbackButtons[i].icon);
            callbackButton->setActionListener([this, i]() {
                _flags.callbackButtons[i].func();
            });
            _callbackButtons.add(callbackButton);
            _bigButtons->add(callbackButton);
        }

        _lateDayButton = new GButton("Late days\ninfo", _iconStrip[ICON_LATE_DAYS_FILENAME]);
        _lateDayButton->setActionListener([this]() {
            showLateDays();
        });
        _bigButtons->add(_lateDayButton);

        _aboutButton = new GButton("About\nGrader", _iconStrip[ICON_ABOUT_FILENAME]);
        _aboutButton->setActionListener([this]() {
            GOptionPane::showMessageDialog(_flags.aboutText, "About Autograder",
                                   GOptionPane::MessageType::MESSAGE_INFORMATION);
        });
        _bigButtons->add(_aboutButton);

        _exitButton = new GButton("Exit\nGrader", _iconStrip[ICON_EXIT_FILENAME]);
        _exitButton->setActionListener([this]() {
            stanfordcpplib::setExitEnabled(true);   // don't block exit() call

            // TODO: free up memory used by graphical interactors
            _window->close();   // exits program; will not return
        });
        _bigButtons->add(_exitButton);

        // layout
        GContainer* north = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        north->add(_startLabel);
        north->add(_bigButtons);
        north->add(_catchErrorsBox);
        _window->addToRegion(north, GWindow::REGION_NORTH);
        // _window->setRegionHorizontalAlignment(GWindow::REGION_NORTH, ALIGN_LEFT);

        // center container will hold the test cases themselves
        _center = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        _center->setPadding(0);
        _center->setMargin(0);
        _center->setSpacing(1);
        GScrollPane* scroll = new GScrollPane(_center);
        _window->addToRegion(scroll, GWindow::REGION_CENTER);

        // _window->center();
        _window->toFront();
    });
}

GuiAutograder::~GuiAutograder() {
    // TODO: delete
    _window = nullptr;
    _autogradeButton = nullptr;
    _manualButton = nullptr;
    _styleCheckButton = nullptr;
    _lateDayButton = nullptr;
    _aboutButton = nullptr;
    _exitButton = nullptr;
    _center = nullptr;
    _catchErrorsBox = nullptr;
    _startLabel = nullptr;
    _southLabel = nullptr;
    _currentCategory = nullptr;
}

void GuiAutograder::addCallbackButton(void (* func)(),
                       const std::string& text,
                       const std::string& icon) {
    GButton* button = new GButton(text);
    if (_iconStrip.containsKey(getTail(icon))) {
        button->setIcon(_iconStrip[getTail(icon)]);
    } else {
        button->setIcon(icon);
    }
    button->setActionListener(func);
    _bigButtons->add(button);
}

void GuiAutograder::addCategory(const std::string& categoryName, const std::string& categoryDescription) {
    if (_allCategories.containsKey(categoryName)) {
        return;
    }

    GContainer* category = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
    category->setName("category");
    category->setMargin(0);
    category->setPadding(0);
    category->setSpacing(0);
    _currentCategory = category;
    _allCategories[categoryName] = category;

    if (!isStyleCheck()) {
        // top row of 'select/deselect all' buttons
        GContainer* top = new GContainer(GContainer::LAYOUT_FLOW_HORIZONTAL);
        top->setHorizontalAlignment(ALIGN_LEFT);

        GButton* selectAllButton = new GButton("All", _iconStrip[ICON_SELECT_ALL_FILENAME]);
        selectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(selectAllButton, -2);
        selectAllButton->setTooltip("Double-click to select all tests from all categories.");
        selectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ true);
        });
        selectAllButton->setDoubleClickListener([this]() {
            selectAll(_center, /* selected */ true);
        });

        GButton* deselectAllButton = new GButton("None", _iconStrip[ICON_DESELECT_ALL_FILENAME]);
        deselectAllButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(deselectAllButton, -2);
        deselectAllButton->setTooltip("Double-click to deselect all tests from all categories.");
        deselectAllButton->setActionListener([this, category]() {
            selectAll(category, /* selected */ false);
        });
        deselectAllButton->setDoubleClickListener([this]() {
            selectAll(_center, /* selected */ false);
        });

        GButton* minimizeButton = new GButton("Hide", _iconStrip[ICON_MINIMIZE_FILENAME]);
        minimizeButton->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
        GFont::changeFontSize(minimizeButton, -2);
        minimizeButton->setTooltip("Double-click to minimize all categories.");
        minimizeButton->setActionListener([this, category]() {
            minimize(category);
        });
        minimizeButton->setDoubleClickListener([this]() {
            GThread::runOnQtGuiThreadAsync([this]() {
                minimizeAll(true);
            });
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

        if (!categoryDescription.empty()) {
            GLabel* descriptionLabel = new GLabel(categoryDescription);
            descriptionLabel->setName("description");
            descriptionLabel->setWordWrap(true);
            GFont::changeFontSize(descriptionLabel, -1);
            category->add(descriptionLabel);
        }
    }

    _center->add(category);
    GLayout::forceUpdate(category);
}

void GuiAutograder::addTest(const std::string& testName, const std::string& categoryName) {
    if (!containsCategory(categoryName)) {
        addCategory(categoryName);
    }

    _testCount++;
    GContainer* category = _allCategories[categoryName];
    category->setName("category");
    _currentCategory = category;

    GContainer* test = new GContainer(GContainer::LAYOUT_BORDER);
    test->setName("testPanel");
    test->setMargin(0);
    test->setPadding(0);
    test->setSpacing(0);
    std::string bgColor = _testCount % 2 == 0
            ? GWindow::chooseLightDarkModeColor(COLOR_ZEBRA_STRIPE_1, COLOR_ZEBRA_STRIPE_1_DARK_MODE)
            : GWindow::chooseLightDarkModeColor(COLOR_ZEBRA_STRIPE_2, COLOR_ZEBRA_STRIPE_2_DARK_MODE);
    test->setBackground(bgColor);
    GContainer* testWest = new GContainer;
    testWest->setBackground(bgColor);
    testWest->setPadding(0);

    TestInfo* testInfo = new TestInfo;
    testInfo->name = testName;
    testInfo->category = categoryName;
    testInfo->completed = false;
    testInfo->checkbox = new GCheckBox("", /* checked */ true);
    testInfo->checkbox->setActionListener([this, testInfo]() {
        this->setTestShouldRun(testInfo->getFullName(), testInfo->checkbox->isChecked());
    });
    if (!isStyleCheck() && _checkboxesShown) {
        testWest->add(testInfo->checkbox);
    }
    std::string testFullName = testInfo->getFullName();
    _allTestInfo[testFullName] = testInfo;

    GLabel* testNumberLabel = new GLabel(padLeft(std::to_string(_testCount), /* digits */ 3, /* fill */ '0') + ". ");
    // TODO: align test number right?
    testNumberLabel->setFont(GFont::deriveQFont(testNumberLabel->getFont(), QFont::Bold));
    testNumberLabel->setActionListener([testInfo]() {
        testInfo->checkbox->toggle();
    });
    testWest->add(testNumberLabel);
    test->addToRegion(testWest, GContainer::REGION_WEST);

    std::string testNameShort = regexReplace(testName, "Test_(?:[0-9]{1,5}_)?", "");
    testInfo->descriptionLabel = new GLabel(testNameShort);
    testInfo->descriptionLabel->setTooltip("Click to see detailed results from this test.");
    GFont::changeFontSize(testInfo->descriptionLabel, -1);
    testInfo->descriptionLabel->setFont(GFont::deriveQFont(testInfo->descriptionLabel->getFont(), QFont::Bold));
    auto showDetailsFunc = [this, testFullName]() {
        showTestDetails(testFullName);
    };
    testInfo->descriptionLabel->setActionListener(showDetailsFunc);
    test->addToRegion(testInfo->descriptionLabel, GContainer::REGION_CENTER);

    // have to separate runtime from result icon because the Qt GLabel doesn't
    // support a label that has both an icon and text (oops)
    testInfo->runtimeLabel = new GLabel;
    testInfo->runtimeLabel->setText(" ");
    GFont::changeFontSize(testInfo->runtimeLabel, -2);
    testInfo->runtimeLabel->setTooltip("Click to see detailed results from this test.");
    testInfo->runtimeLabel->setActionListener(showDetailsFunc);
    testInfo->resultIconLabel = new GLabel;
    testInfo->resultIconLabel->setIcon(_iconStrip[ICON_RUNNING_FILENAME]);
    testInfo->resultIconLabel->setTooltip("Click to see detailed results from this test.");
    testInfo->resultIconLabel->setActionListener(showDetailsFunc);

    GContainer* east = new GContainer();
    east->add(testInfo->runtimeLabel);
    east->add(testInfo->resultIconLabel);
    test->addToRegion(east, GContainer::REGION_EAST);

    testInfo->detailsLabel = new GLabel;
    testInfo->detailsLabel->setText("        ");
    // GFont::changeFontSize(testInfo->detailsLabel, -1);
    testInfo->detailsLabel->setTooltip("Detailed results of the test");
    // testInfo->detailsLabel->setWidth(DEFAULT_WINDOW_WIDTH - 20);
    testInfo->detailsLabel->setHeight(0);
    testInfo->detailsLabel->setVisible(false);   // hidden until clicked
    testInfo->detailsLabel->setActionListener(showDetailsFunc);
    test->addToRegion(new GSpacer(20, 1), GContainer::REGION_SOUTH);
    test->addToRegion(testInfo->detailsLabel, GContainer::REGION_SOUTH);
    test->setRegionHorizontalAlignment(GContainer::REGION_SOUTH, ALIGN_LEFT);

    _currentCategory->add(test);

    // initially every test should be run unless later unchecked
    setTestShouldRun(testFullName, true);
}

bool GuiAutograder::autograderYesOrNo(std::string prompt, std::string /*reprompt*/, std::string /*defaultValue*/) {
    prompt = stringReplace(prompt, " (Y/n)", "");
    prompt = stringReplace(prompt, " (y/N)", "");
    prompt = stringReplace(prompt, " (y/n)", "");
    prompt = stringReplace(prompt, " (Y/N)", "");
    return GOptionPane::showConfirmDialog(prompt) == GOptionPane::ConfirmResult::CONFIRM_YES;
}

bool GuiAutograder::catchExceptions() const {
    return _catchExceptions;
}

void GuiAutograder::clearTestResults() {
    _passCount = 0;
    for (TestInfo* testInfo : _allTestInfo.values()) {
        if (!testInfo) {
            continue;
        }
        UnitTestDetails deets;   // clear it out
        testInfo->details = deets;
        testInfo->completed = false;   // JDZ: must reset! if not, default/empty result treated as valid
        testInfo->descriptionLabel->setForeground(GWindow::getDefaultInteractorTextColorInt());
        testInfo->resultIconLabel->setText("");
        testInfo->resultIconLabel->setIcon(_iconStrip[ICON_RUNNING_FILENAME]);
    }
    updateSouthText();
}

void GuiAutograder::clearTests() {
    // TODO
}

bool GuiAutograder::containsCategory(const std::string& categoryName) {
    return _allCategories.containsKey(categoryName);
}

void GuiAutograder::displayDiffs(const std::string& /*expectedOutput*/,
                                 const std::string& /*studentOutput*/,
                                 const std::string& /*diffs*/,
                                 const std::string& /*diffFile*/,
                                 int /*truncateHeight*/) {
    // empty
}

void GuiAutograder::failWithException(const std::string& testFullName,
                                      const std::string& kind,
                                      const std::string& desc,
                                      const std::string& stackTrace) {
    std::ostringstream out;
    out << kind << " was thrown during test execution:" << std::endl;
    out << desc << std::endl << std::endl;

    if (stackTrace.empty()) {
        out << "<small>(Sorry; stack trace cannot be shown in this view." << std::endl;
        out << " To see a stack trace, Run/Debug again" << std::endl;
        out << " with the 'Catch exceptions' box unchecked.)</small>" << std::endl;
    } else {
        out << "Stack trace:" << std::endl;
        out << "<pre><font color='"
               + GWindow::chooseLightDarkModeColor(GDiffGui::COLOR_STUDENT, GDiffGui::COLOR_STUDENT_DARK_MODE)
               + "'>" << stackTrace << "</font></pre>" << std::endl;
    }
    // TODO: can I get this stack trace?

    // can't really get a stack trace of an exception here; would need to get it
    // at the moment that the exception was generated
    // exceptions::printStackTrace(out);


    std::string errorMessage = out.str();
    setTestDetails(testFullName, UnitTestDetails(
        TEST_EXCEPTION,
        errorMessage));
    setTestResult(testFullName, TEST_RESULT_FAIL);
}

int GuiAutograder::getCheckedTestCount() const {
    int checkedCount = 0;
    Vector<GInteractor*> checkboxes = _center->getDescendents("GCheckBox");
    for (GInteractor* interactor : checkboxes) {
        GCheckBox* checkbox = static_cast<GCheckBox*>(interactor);
        if (checkbox->isChecked()) {
            checkedCount++;
        }
    }
    return checkedCount;
}

stanfordcpplib::autograder::TestResult GuiAutograder::getTestResult(const std::string& testFullName) const {
    if (!_allTestInfo.containsKey(testFullName)) {
        return TEST_RESULT_UNKNOWN;
    }
    TestInfo* testInfo = _allTestInfo[testFullName];
    std::string fg = testInfo->descriptionLabel->getForeground();
    if (fg == COLOR_FAIL || fg == COLOR_FAIL_DARK_MODE) {
        return TEST_RESULT_FAIL;
    } else if (fg == COLOR_WARN || fg == COLOR_WARN_DARK_MODE) {
        return TEST_RESULT_WARN;
    } else if (fg == COLOR_PASS || fg == COLOR_PASS_DARK_MODE) {
        return TEST_RESULT_PASS;
    } else if (testInfo->completed) {
        return testInfo->details.passed ? TEST_RESULT_PASS : TEST_RESULT_FAIL;
    } else {
        return TEST_RESULT_UNKNOWN;
    }
}

bool GuiAutograder::isChecked(const std::string& testFullName) const {
    return _allTestInfo.containsKey(testFullName)
            && _allTestInfo[testFullName]->checkbox->isChecked();
}

bool GuiAutograder::isGraphicalUI() const {
    return true;
}

bool GuiAutograder::isStyleCheck() const {
    return this == _instanceStyleCheck;
}

int GuiAutograder::mainFunc() {
    // TODO: don't really need this?
    std::string studentName = QtGui::instance()->getApplicationDisplayName();
    _window->setTitle(studentName.empty()? _flags.assignmentName + " Autograder " : studentName);
    _window->_autograder_setIsAutograderWindow(true);
    _window->rememberPosition();
    _window->setVisible(true);
    setConsoleExitProgramOnClose(true);

    if (!_flags.startMessage.empty()) {
        std::string startMessage = _flags.startMessage;
        if (!stringContains(startMessage, "<html>")) {
            startMessage = stringReplace(startMessage, "Note:", "<b>Note:</b>");
            startMessage = stringReplace(startMessage, "NOTE:", "<b>NOTE:</b>");
            startMessage = "<html><body>"
                    + startMessage
                    + "</body></html>";
        }

        _startLabel->setText(startMessage);
    }

    int result = 0;

    // _window->pack();
    _window->show();

    return result;
}

void GuiAutograder::minimize(GContainer* category) {
    minimize(category, category->getName() != "categoryHidden");
}

void GuiAutograder::minimize(GContainer* category, bool minimized) {
    if (minimized) {
        category->setName("categoryHidden");
    } else {
        category->setName("category");
    }

    for (GInteractor* interactor : category->getInteractors()) {
        if ((interactor->getType() == "GContainer" && interactor->getName() == "testPanel")
                || (interactor->getType() == "GLabel" && interactor->getName() == "description")) {
            interactor->setVisible(!minimized);
        }
    }
}

void GuiAutograder::minimizeAll(bool minimized) {
    for (GContainer* category : _allCategories.values()) {
        minimize(category, minimized);
    }
}

void GuiAutograder::onClick_autograde() {
    if (_flags.callbackStart) {
        _flags.callbackStart();
    }

    clearTestResults();
    setTestingCompleted(false);
    setVisible(true);

    // rest must be async so that the GUI event thread goes back into its event
    // loop and doesn't get "stuck" here waiting for test cases to finish
    /*int result =*/
    GThread::runInNewThreadAsync([this]() {
        runAllTestCases();
        setTestingCompleted(true);

        // if style checker is merged, also run it now
        if (stylecheck::isStyleCheckMergedWithUnitTests()) {
            runStyleChecker();
        }

        if (_flags.callbackEnd) {
            _flags.callbackEnd();
        }
    }, "RunAllTestCases");
}

void GuiAutograder::onClick_runManually() {
    // set up buttons to automatically enter user input
    if (_flags.showInputPanel) {
        GInputPanel::instance()->load(_flags.inputPanelFilename);
    }

    // actually run the student's program
    // (While program is running, if we close console, exit entire
    // autograder program because we might be blocked on console I/O.
    // But after it's done running, set behavior to just hide the
    // console, since the grader will probably try to close it and then
    // proceed with more grading and tests afterward.
    // A little wonky, but it avoids most of the surprise cases of
    // "I closed the student's console and it killed the autograder".
    GConsoleWindow* console = getConsoleWindow();
    console->clearConsole();
    console->setVisible(true);
    console->toFront();
    console->setCloseOperation(GWindow::CLOSE_HIDE);
    stanfordcpplib::setExitEnabled(false);   // block exit() call

    GThread::runInNewThreadAsync([]() {
        main();   // run student's main function
    }, "Main (student)");
}

int GuiAutograder::runAllTestCases() {
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
        MartyGraphicalTestResultPrinter* graphicalPrinter = new autograder::MartyGraphicalTestResultPrinter();
        listeners.Append(graphicalPrinter);
    }

    // run Google Test framework now;
    // this call burrows deep down into gtest-all.cpp and does crazy stuff with
    // Google Test macros and stuff that I don't really understand, so have fun
    int result = RUN_ALL_TESTS();

    // un-lock-down
    setConsoleSettingsLocked(false);
    ioutils::setConsoleEchoUserInput(false);
    setConsoleClearEnabled(true);
    GWindow::_autograder_setPauseEnabled(true);
    GWindow::_autograder_setExitGraphicsEnabled(true);

    return result;
}

std::string GuiAutograder::runAndCapture(int (* mainFunc)(),
                                         const std::string& cinInput,
                                         const std::string& outputFileName) {
    // run the 'main' function, possibly feeding it cin, input, and capture its cout output
    if (!cinInput.empty()) {
        ioutils::redirectStdinBegin(cinInput);
    }

    ioutils::setConsoleOutputLimit(MAX_STUDENT_OUTPUT);   // prevent infinite output by student
    ioutils::captureStdoutBegin(/* alsoStdErr */ true);

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

void GuiAutograder::runStyleChecker() {
    setTestingCompleted(false);
    int styleCheckCount = 0;
    for (std::string filename : _flags.styleCheckFiles) {
        stylecheck::styleCheck(
                    filename,
                    _flags.styleCheckFileMap[filename],
                     /* printWarning */ styleCheckCount == 0);
        styleCheckCount++;
    }
    setTestingCompleted(true);
}

void GuiAutograder::runTest(stanfordcpplib::autograder::AutograderTest* test) {
    resetStandardInputStreams();
    int timeoutMS = test->getTestTimeout();
    std::string testName = test->getName();
    std::string testFullName = test->getFullName();
    GThread* thread = GThread::runInNewThreadAsync([this, test, testFullName]() {
        setCurrentTestCaseName(testFullName);
        if (catchExceptions()) {
            try {
                // echo test's name to plain-text console for logging purposes
                printf("running test \"%s\"\n", testFullName.c_str()); fflush(stdout);

                // run and catch exceptions/errors thrown during test execution
                test->TestRealBody();

                printf("  complete.\n"); fflush(stdout);
            } catch (const ErrorException& ex) {
                printf("  threw ErrorException: \"%s\"\n", ex.what()); fflush(stdout);
                failWithException(testFullName, "An ErrorException", ex.what(), ex.getStackTrace());
            }

//            catch (const std::exception& ex) {
//                failWithException(testFullName, "A C++ exception", ex.what());
//            } catch (std::string str) {
//                failWithException(testFullName, "A string exception", str);
//            } catch (char const* str) {
//                failWithException(testFullName, "A string exception", str);
//            } catch (int n) {
//                failWithException(testFullName, "An int exception", std::to_string(n));
//            } catch (long l) {
//                failWithException(testFullName, "A long exception", longToString(l));
//            } catch (char c) {
//                failWithException(testFullName, "A char exception", charToString(c));
//            } catch (bool b) {
//                failWithException(testFullName, "A bool exception", boolToString(b));
//            } catch (double d) {
//                failWithException(testFullName, "A double exception", realToString(d));
//            }

            // BUGFIX: don't catch '...' because it blocks necessary error
            // propagation on thread timeout/shutdown
            // catch (...) {
            //     failWithException(testFullName, "An unknown exception", "...");
            // }

        } else {
            test->TestRealBody();
            printf("  complete.\n"); fflush(stdout);
        }
    }, /* threadName */ testName);

    resetStandardInputStreams();
    if (GThread::wait(thread, timeoutMS)) {
        // thread is still running; timed out
        printf("  timed out after %d ms.\n", timeoutMS); fflush(stdout);
        thread->stop();
        setFailDetails(*test, autograder::UnitTestDetails(
            autograder::UnitTestType::TEST_FAIL,
            TIMEOUT_ERROR_MESSAGE));
        error(TIMEOUT_ERROR_MESSAGE);
    }
}

void GuiAutograder::selectAll(GContainer* category, bool selected) {
    for (GInteractor* interactor : category->getDescendents("GCheckBox")) {
        GCheckBox* checkbox = static_cast<GCheckBox*>(interactor);
        checkbox->setChecked(selected);
    }
}

void GuiAutograder::setCatchExceptions(bool catchExceptions) {
    _catchExceptions = catchExceptions;
}

void GuiAutograder::setTestCounts(int passCount, int testCount, bool /*isStyleCheck*/) {
    _passCount = passCount;
    _testCount = testCount;
    updateSouthText();
}

void GuiAutograder::setTestDetails(const std::string& testFullName, UnitTestDetails deets) {
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
    testInfo->completed = true;

    // also open up
    if (AUTO_EXPAND_FAILED_TESTS
            && !deets.passed
            && showTestDetailsInSameWindow(testFullName)) {
        showTestDetails(testFullName, /* force */ true);
    }
}

void GuiAutograder::setTestingCompleted(bool completed) {
    _testingIsInProgress = !completed;
    updateSouthText();
}

bool GuiAutograder::setTestResult(const std::string& testFullName, TestResult result) {
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
        labelForegroundColor = GWindow::chooseLightDarkModeColor(COLOR_FAIL, COLOR_FAIL_DARK_MODE);
        break;
    case TEST_RESULT_PASS:
        iconFile = ICON_PASS_FILENAME;
        labelForegroundColor = GWindow::chooseLightDarkModeColor(COLOR_PASS, COLOR_PASS_DARK_MODE);
        _passCount++;
        break;
    case TEST_RESULT_WARN:
        iconFile = ICON_WARNING_FILENAME;
        labelForegroundColor = GWindow::chooseLightDarkModeColor(COLOR_WARN, COLOR_WARN_DARK_MODE);
        break;
    case TEST_RESULT_UNKNOWN:
        iconFile = ICON_UNKNOWN_FILENAME;
        labelForegroundColor = GWindow::getDefaultInteractorTextColor();
        break;
    }

    testInfo->resultIconLabel->setIcon(_iconStrip[iconFile]);
    testInfo->descriptionLabel->setForeground(labelForegroundColor);
    updateSouthText();
    return true;
}

bool GuiAutograder::setTestRuntime(const std::string& testFullName, long ms) {
    if (!_allTestInfo.containsKey(testFullName)) {
        return false;   // test not found
    }

    std::string text = " (" + longToString(ms) + " ms)";
    TestInfo* testInfo = _allTestInfo[testFullName];
    testInfo->runtimeLabel->setText(text);
    return true;
}

void GuiAutograder::setVisible(bool visible) {
    _window->setVisible(visible);
}

void GuiAutograder::setWindowDescription(const std::string& /*description*/) {
    // TODO: set text of _descriptionLabel
}

void GuiAutograder::showLateDays(const std::string& filename) {
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
    GOptionPane::showMessageDialog(message, filename, GOptionPane::MESSAGE_PLAIN);
}

void GuiAutograder::showOutput(const std::string& output, bool showIfGraphical, bool /*showIfConsole*/) {
    if (showIfGraphical) {
        GOptionPane::showMessageDialog(output);
    }
}

void GuiAutograder::showTestDetails(const std::string& testFullName, bool force) {
    TestInfo* testInfo = _allTestInfo[testFullName];
    if (!testInfo || !testInfo->completed) {
        return;
    }

    // toggle between showing and visible
    if (force || static_cast<int>(testInfo->detailsLabel->getHeight()) == 0) {
        const UnitTestDetails& deets = testInfo->details;

        std::string message = deets.message;

        // customize message based on its assertion type
        switch (deets.testType) {
            case TEST_ASSERT_EQUALS:
                message += " (must be equal)";
                break;
            case TEST_ASSERT_NOT_EQUALS:
                message += " (must be non-equal)";
                break;
            case TEST_ASSERT_NEAR:
                message += " (must be nearly equal)";
                break;
            case TEST_ASSERT_DIFF:
                message += " (program output must match)";
                break;
            case TEST_ASSERT_DIFF_IMAGE:
                message += " (images must match)";
                break;
            case TEST_ASSERT_TRUE:
            case TEST_ASSERT_FALSE:
            case TEST_ASSERT_NOT_NULL:
            case TEST_ASSERT_NULL:
                break;
            case TEST_EXCEPTION:
                // message += " (threw exception)";
                break;
            case TEST_NOT_EXCEPTION:
                message += " (didn't throw expected exception)";
                break;
            case TEST_PASS:
                message += " (passed)";
                break;
            case TEST_FAIL:
                message += " (failed)";
                break;
            case TEST_STYLE_CHECK:
                // message += " (style checker warning)";
                break;
        }

        std::string expected = deets.expected;
        std::string student  = deets.student;
        if (deets.valueType == "string") {
            expected = "\"" + expected + "\"";
            student  = "\"" + student + "\"";
        } else if (deets.valueType == "char") {
            expected = "'" + expected + "'";
            student  = "'" + student + "'";
        }
        expected = toPrintable(expected);
        student  = toPrintable(student);

        // insert "expected"/"student" messages for some assert types for clarity
        switch (deets.testType) {
            case TEST_EXCEPTION:
                expected = "do not throw an exception";
                student  = (deets.passed ? "did not throw an exception" : "threw an exception");
                break;
            case TEST_NOT_EXCEPTION:
                expected = "throw an exception";
                student  = (deets.passed ? "threw an exception" : "did not throw an exception");
                break;
            default:
                break;
        }

        // decide whether to show a details label or pop up an external window
        // (external window for full-program text diffs or graphical image diffs)
        std::string htmlMessage = "";
        std::string expectedTruncated;
        std::string studentTruncated;
        switch (deets.testType) {
            case TEST_ASSERT_EQUALS:
            case TEST_ASSERT_NOT_EQUALS:
            case TEST_ASSERT_NEAR:
            case TEST_ASSERT_NULL:
            case TEST_ASSERT_NOT_NULL:
            case TEST_ASSERT_TRUE:
            case TEST_ASSERT_FALSE:
            case TEST_STYLE_CHECK:
            case TEST_EXCEPTION:
            case TEST_NOT_EXCEPTION:
            case TEST_PASS:
            case TEST_FAIL: {
                // details label
                expectedTruncated = stringutils::truncate(expected, MAX_VALUE_DISPLAY_LENGTH, " ...");
                studentTruncated  = stringutils::truncate(student, MAX_VALUE_DISPLAY_LENGTH, " ...");
                message = stringReplace(message, "\n\n", "<br>\n<br>\n");
                htmlMessage += "<p>" + message + "</p>";

                if (!expected.empty() || !student.empty()) {
                    std::string expectedHtml = stringReplace(expectedTruncated, " ", "&nbsp;");
                    std::string studentHtml  = stringReplace(studentTruncated, " ", "&nbsp;");
                    htmlMessage += "<ul>";
                    htmlMessage += "<li><code><font color='" + GWindow::chooseLightDarkModeColor(GDiffGui::COLOR_EXPECTED, GDiffGui::COLOR_EXPECTED_DARK_MODE) + "'>expected:</font></code><code>&nbsp;" + expectedHtml + "</code></li>";
                    htmlMessage += "<li><code><font color='" + GWindow::chooseLightDarkModeColor(GDiffGui::COLOR_STUDENT, GDiffGui::COLOR_STUDENT_DARK_MODE)  + "'>student&nbsp;:</font></code><code>&nbsp;" + studentHtml + "</code></li>";
                    htmlMessage += "</ul>";
                }

                std::string resultColor = GWindow::chooseLightDarkModeColor(COLOR_PASS, COLOR_PASS_DARK_MODE);
                std::string resultText = "PASS";
                if (!deets.passed) {
                    if (deets.result == TEST_RESULT_WARN) {
                        resultColor = GWindow::chooseLightDarkModeColor(COLOR_WARN, COLOR_WARN_DARK_MODE);
                        resultText = "WARNING";
                    } else {
                        resultColor = GWindow::chooseLightDarkModeColor(COLOR_FAIL, COLOR_FAIL_DARK_MODE);
                        resultText = "FAIL";
                    }
                }
                htmlMessage += std::string("<p>result: ")
                        + "<font color='" + resultColor + "'><b>" + resultText
                        + "</b></font></p>";

                // if (!stack.isEmpty()) {
                //     htmlMessage += "<div style='margin-top: 5px;'><b>Stack trace:</b></div><pre>" + stack + "</pre>";
                // }

                message = "<html><body>" + htmlMessage + "</body></html>";
                testInfo->detailsLabel->setText(message);
                testInfo->detailsLabel->setWordWrap(true);
                testInfo->detailsLabel->setVisible(true);
                testInfo->detailsLabel->setHeight(testInfo->detailsLabel->getPreferredHeight());
                // TODO: stretch
                break;
            }
            case TEST_ASSERT_DIFF: {
                // DiffGui external window
                GDiffGui::showDialog("Expected output", deets.expected, "Student output", deets.student, deets.diffFlags);
                break;
            }
            case TEST_ASSERT_DIFF_IMAGE: {
                // DiffImage external window
                GDiffImage::showDialog("Expected output", /* expectedImageFile */ deets.expected,
                                       "Student output", /* studentImageFile */ deets.student);
                break;
            }
        }
    } else {
        // hide
        testInfo->detailsLabel->setHeight(0);
        testInfo->detailsLabel->setVisible(false);
    }
}

bool GuiAutograder::showTestDetailsInSameWindow(const std::string& testFullName) const {
    TestInfo* testInfo = _allTestInfo[testFullName];
    if (!testInfo || !testInfo->completed) {
        return false;
    }
    const UnitTestDetails& deets = testInfo->details;
    switch (deets.testType) {
        case TEST_ASSERT_EQUALS:
        case TEST_ASSERT_NOT_EQUALS:
        case TEST_ASSERT_NEAR:
        case TEST_ASSERT_NULL:
        case TEST_ASSERT_NOT_NULL:
        case TEST_ASSERT_TRUE:
        case TEST_ASSERT_FALSE:
        case TEST_STYLE_CHECK:
        case TEST_EXCEPTION:
        case TEST_NOT_EXCEPTION:
        case TEST_PASS:
        case TEST_FAIL:
            return true;
        default:
            return false;
    }
}

void GuiAutograder::updateSouthText() {
    std::string text = "passed " + std::to_string(_passCount)
            + " / " + std::to_string(getCheckedTestCount()) + " tests";
    if (_testingIsInProgress) {
        text += " (running ...)";
        if (_southLabel->getIcon() == "") {
            _southLabel->setIcon(_iconStrip[ICON_PROGRESS_FILENAME]);
        }
    } else {
        text += " (complete)";
        _southLabel->setIcon("");
    }
    _southLabel->setText(text);
}

void GuiAutograder::setFailDetails(AutograderTest& test, const autograder::UnitTestDetails& deets) {
    setTestDetails(test.getFullName(), deets);
}

void GuiAutograder::setFailDetails(const autograder::UnitTestDetails& deets) {
    std::string curTest = getCurrentTestCaseName();
    setTestDetails(curTest, deets);
}

void GuiAutograder::showStudentTextFile(const std::string& filename, int maxWidth, int maxHeight) {
    std::string myText;
    if (fileExists(filename)) {
        myText = readEntireFile(filename);
    } else {
        myText = "file not found: " + filename;
    }
    if (maxWidth > 0) {
        myText = stringutils::trimToWidth(myText, maxWidth);
    }
    if (maxHeight > 0) {
        myText = stringutils::trimToHeight(myText, maxHeight);
    }
    if (!endsWith(myText, "\n")) {
        myText += "\n";
    }
    GOptionPane::showTextFileDialog(
            myText,
            /* title */ "Contents of " + filename,
            maxHeight, maxWidth);
}

} // namespace autograder
} // namespace stanfordcpplib
