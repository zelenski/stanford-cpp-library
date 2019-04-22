/*
 * File: mainwrapper.cpp
 * ---------------------
 * This file contains a 'main' function definition that renames the student's
 * main function to 'qMain' and wraps it with a real 'main' function that
 * initializes the Stanford C++ library, then runs the student's main function
 * in its own thread.  This is necessary for the Qt version of the library to
 * function properly.
 *
 * If you receive compiler errors that direct you to this file, you may need to
 * include a .h header from the Stanford C++ library in the file of your project
 * that contains the 'main' function.
 *
 * @version 2018/10/18
 * - multi-main initial implementation
 * @version 2018/10/07
 * - bug fixes for autograder mode
 * @version 2018/09/23
 * - bug fixes for windows Qt main wrapper
 * @version 2018/09/17
 * - initial version
 */

#include "private/init.h"

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef SPL_AUTOGRADER_MODE
int qMain(int argc, char** argv);

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
namespace stanfordcpplib {
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)(void));
extern int selectMainFunction();
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
    stanfordcpplib::initializeLibrary(argc, argv);
    int result = stanfordcpplib::selectMainFunction();
    stanfordcpplib::shutdownLibrary();
    return result;
}
#endif // SPL_AUTOGRADER_MODE
