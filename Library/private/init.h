/*
 * File: init.h
 * ------------
 * These functions setup/teardown the Stanford C++ library.
 *
 * Originally, necessary setup was initiated via a static initializer. This required
 * careful arrangement include+guard, this is no longer used as pre/post work is
 * is inserted into the wrapper "main" function which surrounds student main.
 *
 * @version 2018/08/28
 * - refactor to use stanfordcpplib namespace and init.cpp
 * @version 2018/07/03
 * - add code to handle Qt GUI library initialization
 * @version 2017/04/25
 * - wrap library initializer in an #ifndef to avoid multiple declaration
 *
 */

#ifndef _init_h
#define _init_h

#include <string>

namespace stanfordcpplib {

/**
 * Returns true if the std::exit function is enabled.
 * This will be true unless disabled by, say, an autograder.
 */
bool exitEnabled();

/**
 * Initializes the Stanford C++ library.
 * A call to this function is inserted before the student's main() runs.
 * This should be run from the Qt GUI (main) thread.
 */
void initializeLibrary(int argc, char** argv);

/**
 * This is for any initialization that needs to be done in the student's thread
 * rather than on the Qt GUI main thread.
 * Currently this is used primarily to set up exception handlers for the
 * student's thread to catch and report errors.
 */
void initializeStudentThread();

/**
 * Sets whether the std::exit function will be enabled or not.
 * If disabled, an error() will be thrown if student tries to exit().
 */
void setExitEnabled(bool enabled);

/**
 * Shuts down the Stanford C++ library.
 * A call to this function is inserted after the student's main().
 */
void shutdownLibrary();



} // namespace stanfordcpplib

// bypass std::exit function
namespace std {
void __stanfordcpplib__exitLibrary(int status);
} // namespace std

#define STD_EXIT __std_exit_function_
#define exit __stanfordcpplib__exitLibrary

#ifdef SPL_HEADLESS_MODE
#include "headless.h"
#endif // SPL_HEADLESS_MODE

#endif // _init_h
