/*
 * File: librarymain.cpp
 * ---------------------
 * This file contains a 'libraryMain' function definition that initializes the
 * Stanford C++ library, then runs the student's main function
 * in its own thread.  This is necessary for the Qt version of the library to
 * function properly.
 */

#include "private/init.h"
#include "qtgui.h"

// student implements what looks like ordinary main() function, but
// will be renamed to studentMain() during build
int studentMain();

// initializes the Qt GUI library subsystems and Qt graphical console as needed
// before calling studentMain

int libraryMain(int argc, char** argv)
{
    stanfordcpplib::initializeLibrary(argc, argv);
#ifdef SPL_HEADLESS_MODE
    studentMain();
#else
    QtGui::instance()->startBackgroundEventLoop(studentMain);
#endif
    stanfordcpplib::shutdownLibrary();
    return 0;
}
