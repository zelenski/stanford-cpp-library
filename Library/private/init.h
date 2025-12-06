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
 * Initializes the Stanford C++ library.
 * A call to this function is inserted before the student's main() runs.
 * This should be run from the Qt GUI (main) thread.
 */
void initializeLibrary(int argc, char** argv);

/**
 * Shuts down the Stanford C++ library.
 * A call to this function is inserted after the student's main().
 */
void shutdownLibrary();


} // namespace stanfordcpplib


#ifdef SPL_HEADLESS_MODE
#include "headless.h"
#endif // SPL_HEADLESS_MODE

#endif // _init_h
