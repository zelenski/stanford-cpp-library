/*
 * File: autogradermainwrapper.cpp
 * -------------------------------
 * This file contains the main/qMain function to initiate the program.
 * It overrides/replaces the one from mainwrapper.cpp for the normal
 * Stanford C++ library project.
 *
 * @author Marty Stepp
 * @version 2018/10/07
 * - initial version; refactored from autograder.cpp
 */
#include "private/init.h"
#include "autograder.h"
#include "consoleautograder.h"
#include "guiautograder.h"
#include "qtgui.h"
#include <string>

// JDZ: yucky hack, here until I work out something better
// The library supplies two main() functions (autograder and regular)
// Each main is in separate module; program need to selectively link with
// the desired one. Extra symbol autograder_active defined only in this
// module. Autograder program has undefined ref to that symbol to force
// this module to be linked.
bool autograder_active;

extern void autograderMain();


// initializes the Qt GUI library subsystems and Qt graphical console as needed
int main(int argc, char** argv) {

    // initialize Stanford libraries and graphical console
    stanfordcpplib::initializeLibrary(argc, argv);

//    setConsoleLocationSaved(true);
//    setConsoleCloseOperation(GWindow::CLOSE_HIDE);

    // set which kind of autograder to use (GUI or console) based on .pro flags
#if defined(SPL_GRAPHICAL_AUTOGRADER) || !defined(SPL_CONSOLE_AUTOGRADER)
    stanfordcpplib::autograder::Autograder::setInstance(stanfordcpplib::autograder::GuiAutograder::instance());
#else
    stanfordcpplib::autograder::Autograder::setInstance(stanfordcpplib::autograder::ConsoleAutograder::instance());
#endif
    
    // tell the GUI the names of all tests so that it can display them
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    for (const std::string& category : stanfordcpplib::autograder::AutograderTest::getAllCategories()) {
        for (const std::string& test : stanfordcpplib::autograder::AutograderTest::getAllTests(category)) {
            autograder->addTest(test, category);
        }
    }

    // your assignment-specific autograder main runs here
    ::autograderMain();
    autograder->mainFunc();

    // start GUI event processing
    QtGui::instance()->startEventLoop();

    stanfordcpplib::shutdownLibrary();
    return 0;
}
