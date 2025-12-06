/*
 * File: init.cpp
 * --------------
 *
 * TODO
 *
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/08/28
 * - refactor to use stanfordcpplib namespace
 * @version 2018/08/27
 * - initial version
 */

#include "private/init.h"
#include "consoletext.h"
#include "qtgui.h"
#include <QLoggingCategory>


#ifdef __APPLE__
#include <unistd.h>   // for chdir
#endif // __APPLE__


namespace stanfordcpplib {

namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void shutdownConsole();
}

static void chdirAppdir(std::string arg0) {
#ifdef __APPLE__
    // on Mac only, may need to change wd because of app's nested dir structure
    size_t pos = arg0.find(".app/Contents/");
    if (pos != std::string::npos) {
        pos = arg0.rfind('/', pos); // find end of parent dir
        if (pos != std::string::npos) {
            std::string cwd = arg0.substr(0, pos);
            chdir(cwd.c_str()); // set wd to folder containing .app
        }
    }
#endif // __APPLE__
}

// called automatically by real main() function;
// to be run in Qt GUI main thread
void initializeLibrary(int argc, char** argv) {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    if (argc > 0) chdirAppdir(argv[0]);

#ifndef SPL_HEADLESS_MODE
    GThread::setCurrentThreadAsGuiThread();

    qSetMessagePattern("[Qt internal] %{category}.%{type}: %{message})\n");
    // suppress harmless warnings (font substitutions, ffmpeg license)
    QLoggingCategory::setFilterRules("qt.qpa.fonts.warning=false\n"
                                     "qt.multimedia.ffmpeg.info=false"
    );

    QtGui::createInstance(argc, argv);      // initialize Qt graphics subsystem

    // initialize Qt graphical console (if student #included it)
    initializeQtGraphicalConsole();
#endif // SPL_HEADLESS_MODE
}

// shut down the Qt graphical console window;
// to be run in Qt main thread
void shutdownLibrary() {
#ifdef SPL_HEADLESS_MODE
    // empty
#else
    shutdownConsole();
#endif // SPL_HEADLESS_MODE
}

} // namespace stanfordcpplib

