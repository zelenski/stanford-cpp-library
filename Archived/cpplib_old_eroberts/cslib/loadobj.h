/*
 * File: loadobj.h
 * ---------------
 * This interface supports dynamic loading of functions from object files.
 * Any references in the object file to previously defined objects are
 * allowed, but no additional searching or unresolved references are
 * supported.
 */

#ifndef _loadobj_h
#define _loadobj_h

#include "cslib.h"

/*
 * Function: loadObject
 * Usage: loadObject(pathname);
 * ----------------------------
 * Loads the object file into the current executable.
 */

void loadObject(string pathname);

/*
 * Function: loadSymbols
 * Usage: loadSymbols(progname);
 * -----------------------------
 * Loads the symbols from the executable program.
 */

void loadSymbols(string progname);

/*
 * Function: findFunction
 * Usage: fn = findFunction(fnname);
 * ---------------------------------
 * Looks up the function in the symbol table and returns a pointer to the
 * code.
 */

proc findFunction(string fnname);

#endif
