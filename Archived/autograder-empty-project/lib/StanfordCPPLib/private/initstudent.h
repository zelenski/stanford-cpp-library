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
 * @version 2018/10/18
 * - added multi-main support macros
 * @version 2018/09/25
 * - initial version
 */

#include "multimain.h"
