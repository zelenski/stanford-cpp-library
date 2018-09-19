#include "private/init.h"

#undef main

// initializes the Qt GUI library subsystems and Qt graphical console as needed
int main(int argc, char** argv) {
    extern int studentMain();
    stanfordcpplib::initializeLibrary(argc, argv);

    //////////////////////////////////////////////////////////////////////////
    /// NOTE TO STUDENT!                                                   ///
    /// If you are directed here by a compiler error,                      ///
    /// it means that you have not written a main function or that it has  ///
    /// the wrong parameters.                                              ///
    /// The heading of your main function must be:                         ///
    ///                                                                    ///
    /// int main() { ... }                                                 ///
    ///                                                                    ///
    /// (Our library secretly renames your main function to "studentMain"  ///
    ///  so that we can actually control the main flow of execution.)      ///
    //////////////////////////////////////////////////////////////////////////
    stanfordcpplib::runMainInThread(studentMain);

    stanfordcpplib::shutdownLibrary();
    return 0;
}

// keep in sync with definition in private/init.h
#define main studentMain

