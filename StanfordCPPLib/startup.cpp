/*
 * File: startup.cpp
 * -----------------
 * This file implements the wrapper for main that allows for
 * system initialization and error handling.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include <cstdlib>
#include <iostream>
#include "error.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#  include <windows.h>
#  define MSC_ERROR_FLAGS (MB_OK | MB_ICONSTOP | MB_TOPMOST)
#endif

/* Global flag word indicating option for main */

int _mainFlags;

#ifdef SPL_AUTOGRADER_MODE
int mainWrapper(int /*argc*/, char** /*argv*/) {
#else
int mainWrapper(int argc, char **argv) {
#endif
    extern int Main(int argc, char **argv);
    try {
#ifndef SPL_AUTOGRADER_MODE
        return Main(argc, argv);
#else
        return 0;
#endif
    } catch (ErrorException & ex) {
        std::string msg = "Error: " + ex.getMessage();
        std::cerr << msg << std::endl;
#ifdef _MSC_VER
        MessageBoxA(NULL, msg.c_str(), "Error!", MSC_ERROR_FLAGS);
#endif
        return EXIT_FAILURE;
    }
}
