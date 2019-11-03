/*
 * File: headless.h
 * ----------------
 * This file contains some code for running in "headless" mode with no GUI.
 *
 * @version 2018/11/22
 * - initial version
 */


#ifndef _headless_h
#define _headless_h

#ifdef SPL_HEADLESS_MODE

// make up a Qt version
#define QT_VERSION 5110

// make Qt version macro produce a version less than this
#define QT_VERSION_CHECK(a, b, c) 5090

#endif // SPL_HEADLESS_MODE

#endif // _headless_h
