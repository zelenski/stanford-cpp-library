/*
 * File: consoletext.h
 * -------------------
 * This file provides a way to use the functions from console.h
 * without mandating the use of the Stanford C++ library's
 * graphical console window.
 * 
 * @author Marty Stepp
 * @version 2018/08/31
 * - initial version
 */

#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "console.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
