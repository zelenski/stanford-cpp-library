/*
 * File: gclipboard.h
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gclipboard.h to replace Java version
 * @version 2018/07/19
 * - initial version
 */

#ifndef _gclipboard_h
#define _gclipboard_h

#include <QWindow>   // needed for QEvent
#include <QEvent>
#include <QKeyEvent>
#include <string>

class GClipboard {
public:
    static void set(const std::string& text);
    static std::string get();
    static bool isCopy(QKeyEvent* event);
    static bool isCut(QKeyEvent* event);
    static bool isPaste(QKeyEvent* event);

private:
    GClipboard();   // prevent construction
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gclipboard_h
