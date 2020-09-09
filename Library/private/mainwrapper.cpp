/*
 * File: mainwrapper.cpp
 * ---------------------
 * This file contains a 'main' function definition that initializes the
 * Stanford C++ library, then runs the student's main function
 * in its own thread.  This is necessary for the Qt version of the library to
 * function properly.
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

// Moved the main wrapper function into module by itself so that
// linking will pull in this module if and only if final link of
// executable is missing definition of main/qMain. This will generally
// happen via some preprocessor sleight-of-hand where what was originally
// called main() by student will be renamed to studentMain(), thus causing
// main to be undefined and this version to be linked that will call to
// libraryMain() which does setup and then calls studentMain.

int libraryMain(int, char **);

int main(int argc, char **argv)
{
    return libraryMain(argc, argv);
}

// In Windows startup, entry point is called qMain()
// supply both, the other is fine unused
int qMain(int argc, char **argv)
{
    return libraryMain(argc, argv);
}
