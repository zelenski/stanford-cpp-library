/*
 * File: main.cpp
 * --------------
 * This file defines a default version of the Main function that takes
 * the argc and argv arguments.  This function must be defined in its
 * own module to ensure that it is loaded only if the client doesn't
 * supply one.
 * 
 * @version 2014/12/02
 * - fixed compiler warning about unused 'argv' parameter
 * @version 2014/10/22
 * - made it work when console.h is not included (uses plain text console)
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unused include statements: exception, iostream, string
 */

#include <iostream>

#ifndef SPL_AUTOGRADER_MODE
int Main(int, char* /*argv*/[]) {
    extern int Main();
    return Main();
}
#endif // SPL_AUTOGRADER_MODE
