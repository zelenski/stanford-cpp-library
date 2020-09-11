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
#include "exceptions.h"
#include "qtgui.h"
#include "strlib.h"
#include "private/static.h"
#include <QStandardPaths>
#include "private/version.h"


#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

inline void initResourcesOutsideNamespace() { Q_INIT_RESOURCE(images); }


namespace stanfordcpplib {

namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void shutdownConsole();
}

static void parseArgsQt(int argc, char** argv);

STATIC_VARIABLE_DECLARE(bool, isExitEnabled, true)

bool exitEnabled() {
    return STATIC_VARIABLE(isExitEnabled);
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

#ifndef SPL_HEADLESS_MODE
    GThread::setGuiThread();
#endif // SPL_HEADLESS_MODE

    parseArgsQt(argc, argv);

#ifndef SPL_HEADLESS_MODE
    // initialize the main Qt graphics subsystem
    QtGui::instance()->setArgs(argc, argv);
    QtGui::instance()->initializeQt();
    initResourcesOutsideNamespace();

    // initialize Qt graphical console (if student #included it)
    initializeQtGraphicalConsole();
#endif // SPL_HEADLESS_MODE
}

void initializeStudentThread() {
    exceptions::setTopLevelExceptionHandlerEnabled(true);
}

// this should be roughly the same code as platform.cpp's parseArgs function
static void parseArgsQt(int argc, char** argv) {
    if (argc <= 0) {
        return;
    }
    std::string arg0 = argv[0];
    exceptions::setProgramName(argv[0]);
    // programName() = getRoot(getTail(arg0));

#ifndef _WIN32
    // on Mac only, may need to change wd because of app's nested dir structure
    size_t ax = arg0.find(".app/Contents/");
    if (ax != std::string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            std::string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str()); // wd is folder containing .app
        }
    }
#endif // _WIN32

    char* noConsoleFlag = getenv("NOCONSOLE");
    if (noConsoleFlag && startsWith(std::string(noConsoleFlag), "t")) {
        return;
    }
}

void setExitEnabled(bool enabled) {
    STATIC_VARIABLE(isExitEnabled) = enabled;
    // TODO: notify GConsoleWindow?
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

namespace std {
void __stanfordcpplib__exitLibrary(int status) {
    if (stanfordcpplib::exitEnabled()) {
        // call std::exit (has been renamed)

#ifdef exit
#undef exit
        std::exit(status);
#define exit __stanfordcpplib__exitLibrary
#endif // exit

    } else {
        // not allowed to call exit(); produce error message
        std::ostringstream out;
        out << "Program tried to call exit(" << status << ") to quit. " << std::endl;
        out << "*** This function has been disabled; main should end through " << std::endl;
        out << "*** normal program control flow." << std::endl;
        error(out.str());
    }
}

} // namespace std
