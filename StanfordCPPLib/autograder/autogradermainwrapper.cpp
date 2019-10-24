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

#define INTERNAL_INCLUDE 1
#include "autograder.h"
#define INTERNAL_INCLUDE 1
#include "consoleautograder.h"
#define INTERNAL_INCLUDE 1
#include "guiautograder.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#include <string>
#undef INTERNAL_INCLUDE

#ifdef SPL_AUTOGRADER_MODE
// to be written by TA/instructor for each assignment
extern void autograderMain();

int qMain(int argc, char** argv);

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
namespace stanfordcpplib {
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)());
extern void shutdownLibrary();
}

#ifndef QT_NEEDS_QMAIN
#undef main
int main(int argc, char** argv) {
    return qMain(argc, argv);
}
// keep in sync with definition in .pro file
#ifdef SPL_REPLACE_MAIN_FUNCTION
#define main qMain
#endif // SPL_REPLACE_MAIN_FUNCTION
#endif // QT_NEEDS_QMAIN

// initializes the Qt GUI library subsystems and Qt graphical console as needed
// (autograders will insert their own main wrapper)
int qMain(int argc, char** argv) {
    extern int main();

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
#endif // SPL_AUTOGRADER_MODE
