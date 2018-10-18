/*
 * File: initstudent.h
 * -------------------
 * This file helps initialize the Stanford C++ library if necessary.
 * The library needs an explicit initialization step if either or both
 * of the following are true:
 *
 * - The student #includes console.h to use the graphical console.
 * - The student uses any of the graphical classes based on Qt.
 *
 * We initialize the library by inserting a main function that wraps around
 * the student's main (renaming theirs to qMain).
 *
 * @version 2018/09/25
 * - initial version
 */

#include "private/init.h"

// only define initstudent if needed by SPL QT GUI or graphical console
#if !defined(_initstudent_h) && (defined(SPL_QT_GUI_IN_USE) || defined(SPL_GRAPHICAL_CONSOLE_IN_USE))
#define _initstudent_h
#ifndef QT_NEEDS_QMAIN
#ifdef REPLACE_MAIN_FUNCTION
#define main qMain
#endif // REPLACE_MAIN_FUNCTION
#endif // QT_NEEDS_QMAIN

#endif // !defined(_initstudent_h) && (defined(SPL_QT_GUI_IN_USE) || defined(SPL_GRAPHICAL_CONSOLE_IN_USE))
