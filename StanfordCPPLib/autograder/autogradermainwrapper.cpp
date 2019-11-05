/*
 * File: autogradermainwrapper.cpp
 * -------------------------------
 * This file contains the autograderMain function to be called from
 * main() in an autograder-enabled project. The autograder does not
 * have to play any symbol-renaming games as we control all of the code
 * in that path.
 *
 * @author Marty Stepp
 * @version 2018/10/07
 * - initial version; refactored from autograder.cpp
 */
#include "private/init.h"
#include "consoleautograder.h"
#include "guiautograder.h"
#include "qtgui.h"
#include <string>

int autograderMain(int argc, char** argv, bool useGraphicalAutograder, void (*setupfn)())
{
    // initialize Stanford libraries and graphical console
    stanfordcpplib::initializeLibrary(argc, argv);

//    setConsoleLocationSaved(true);
//    setConsoleCloseOperation(GWindow::CLOSE_HIDE);

    // create requested kind of autograder (gui or console)
    stanfordcpplib::autograder::Autograder* autograder = nullptr;
    if (useGraphicalAutograder) {
        autograder = stanfordcpplib::autograder::GuiAutograder::instance();
    } else {
       autograder =  stanfordcpplib::autograder::ConsoleAutograder::instance();
    }
    stanfordcpplib::autograder::Autograder::setInstance(autograder);

    // tell autograder names of all tests
    for (const std::string& category : stanfordcpplib::autograder::AutograderTest::getAllCategories()) {
        for (const std::string& test : stanfordcpplib::autograder::AutograderTest::getAllTests(category)) {
            autograder->addTest(test, category);
        }
    }

    if (setupfn) setupfn();  // run any assignment-specific setup function if given
    autograder->mainFunc();

    // start GUI event processing
    QtGui::instance()->startEventLoop();

    stanfordcpplib::shutdownLibrary();
    return 0;
}
