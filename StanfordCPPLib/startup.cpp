/*
 * File: startup.cpp
 * -----------------
 * This file implements the wrapper for main that allows for
 * system initialization and error handling.
 */

#include <cstdlib>
#include <iostream>
#include "error.h"
using namespace std;

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#  include <windows.h>
#  define MSC_ERROR_FLAGS (MB_OK | MB_ICONSTOP | MB_TOPMOST)
#endif

/* Global flag word indicating option for main */

int _mainFlags;

int mainWrapper(int argc, char **argv) {
   extern int Main(int argc, char **argv);
   try {
      return Main(argc, argv);
   } catch (ErrorException & ex) {
      string msg = "Error: " + ex.getMessage();
      cerr << msg << endl;
#ifdef _MSC_VER
      MessageBoxA(NULL, msg.c_str(), "Error!", MSC_ERROR_FLAGS);
#endif
      return EXIT_FAILURE;
   }
}
