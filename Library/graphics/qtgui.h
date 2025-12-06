/*
 * File: qtgui.h
 * -------------
 *
 * @author Marty Stepp
 * @version 2018/09/09
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to qtgui.h
 * @version 2018/07/03
 * - initial version
 */


#ifndef _qtgui_h
#define _qtgui_h

#include <string>
#include <QApplication>
#include <QObject>

#include "gthread.h"
#include "gtypes.h"

#define EXITING_DUE_TO_WINDOW_CLOSE 106

class GWindow;

class QSPLApplication : public QApplication {
public:
    QSPLApplication(int& argc, char *argv[]);
    bool notify(QObject* receiver, QEvent* e) override;
};


/**
 * This class contains functionality for initializing and managing the overall
 * Qt-based GUI system of the library.
 * Students and clients generally do not need to interact with this class
 * directly.
 * @private
 */
class QtGui : public QObject {
    Q_OBJECT

public:
    /**
     * Creates and initializes the Qt graphics subsystem.
     * Clients do not need to call this directly because it is called
     * automatically by the library before main() runs.
     */
    static void createInstance(int argc, char **argv);

    /**
     * Shuts down the Qt graphics system.
     */
    void exitGraphics(int exitCode = 0);

    /**
     * Returns the count of arguments passed to the main() function.
     * The string arguments themselves can be examined using getArgv.
     */
    int getArgc() const;

    /**
     * Returns the string arguments passed to the main() function.
     * The number of arguments is found using getArgc.
     */
    char** getArgv() const;

    /**
     * Returns a pointer to the QtGui object for the graphical library.
     */
    static QtGui* instance();

    /**
     * Runs an event loop that calls the given function that returns an int.
     * The event loop will block until the graphical program is done running.
     * If the exitAfter argument is true, calls exitGraphics after the event
     * loop is finished.
     */
    void startBackgroundEventLoop(GThunkInt mainFunc, bool exitAfter = true);

    void startEventLoop(bool exitAfter = true);


public slots:
    /**
     * A slot used by the Qt event queue.
     */
    void processEventFromQueue();

private:
    Q_DISABLE_COPY(QtGui)

    QtGui(int argc, char **argv);   // forbid construction

    int _argc;
    char** _argv;

    static QSPLApplication* _app;
    static QThread* _qtGuiThread;
    static GThread* _studentThread;
    static QtGui* _instance;

    friend class GEventQueue;
    friend class GThread;
    friend class GWindow;
};

namespace stanfordcpplib {
/*
 * Code that runs when student thread shuts down.
 * @private
 */
void studentThreadHasExited(const std::string& reason);
} // namespace stanfordcpplib

#endif // _qtgui_h
