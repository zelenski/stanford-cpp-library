/*
 * File: mainwrapper.cpp
 * ---------------------
 * This file contains a 'main' function definition that renames the student's
 * main function to 'studentMain' and wraps it with a real 'main' function that
 * initializes the Stanford C++ library, then runs the student's main function
 * in its own thread.  This is necessary for the Qt version of the library to
 * function properly.
 *
 * If you receive compiler errors that direct you to this file, you may need to
 * include a .h header from the Stanford C++ library in the file of your project
 * that contains the 'main' function.
 *
 * @version 2018/09/23
 * - bug fixes for windows Qt main wrapper
 * @version 2018/09/17
 * - initial version
 */

int qMain(int argc, char** argv);

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
namespace stanfordcpplib {
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)(void));
extern void shutdownLibrary();
}

#ifndef QT_NEEDS_QMAIN
#undef main
int main(int argc, char** argv) {
    return qMain(argc, argv);
}
// keep in sync with definition in .pro file
#ifdef REPLACE_MAIN_FUNCTION
#define main qMain
#endif // REPLACE_MAIN_FUNCTION
#endif // QT_NEEDS_QMAIN

// initializes the Qt GUI library subsystems and Qt graphical console as needed
int qMain(int argc, char** argv) {
    extern int main();
    stanfordcpplib::initializeLibrary(argc, argv);

    //////////////////////////////////////////////////////////////////////////
    /// NOTE TO STUDENT!                                                   ///
    /// If you are directed here by a compiler error,                      ///
    /// it means that you have not written a main function or that it      ///
    /// has the wrong parameters.                                          ///
    /// The heading of your main function must be:                         ///
    ///                                                                    ///
    /// int main() { ... }                                                 ///
    ///                                                                    ///
    /// (Our library secretly renames your main function to "qMain"        ///
    ///  so that we can actually control the main flow of execution.)      ///
    ///                                                                    ///
    /// You may also need to include a .h header from the Stanford         ///
    /// C++ library in the file of your project that contains the          ///
    /// 'main' function.                                                   ///
    //////////////////////////////////////////////////////////////////////////
    stanfordcpplib::runMainInThread(main);

    stanfordcpplib::shutdownLibrary();
    return 0;
}
