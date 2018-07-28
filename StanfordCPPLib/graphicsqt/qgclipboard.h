/*
 * File: qgclipboard.h
 * -------------------
 *
 * @version 2018/07/19
 * - initial version
 */

#ifndef _qgclipboard_h
#define _qgclipboard_h

#include <QWindow>   // needed for QEvent
#include <QEvent>
#include <QKeyEvent>
#include <string>

class QGClipboard {
public:
    static void set(const std::string& text);
    static std::string get();
    static bool isCopy(QKeyEvent* event);
    static bool isCut(QKeyEvent* event);
    static bool isPaste(QKeyEvent* event);
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgclipboard_h
