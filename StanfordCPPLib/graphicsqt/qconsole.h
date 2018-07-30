/*
 * File: qconsole.h
 * ----------------
 * This file contains functions related to the library's graphical console window,
 * implemented using the Qt graphics library in native C++ instead of Java.
 * In general if you #include this file, it will implicitly enable the graphical
 * console.  If you don't want to do that, you should #define a flag named
 * __DONT_ENABLE_QT_GRAPHICAL_CONSOLE right before #include'ing this header.
 * Once the graphical console has been enabled, it cannot easily be turned off
 * again for that program.
 * 
 * @version 2018/07/29
 * - menu, icons, hotkeys
 * @version 2018/07/26
 * - refactored QGConsoleWindow class
 * @version 2018/07/15
 * - initial version, based on io/console.h
 */

#ifdef SPL_QT_GUI
#ifndef _qconsole_h
#define _qconsole_h

#include <iostream>
#include <sstream>
#include <string>
#include <Qt>
#include <QMutex>
#include <QWindow>
#include <QKeyEvent>
#include <QTextEdit>
#include <QTextFrame>
#include "qgevent.h"
#include "qgtextarea.h"
#include "qgtypes.h"
#include "qgui.h"
#include "qgwindow.h"
#include "queue.h"
#include "vector.h"
#include "private/consolestreambuf.h"
#include "private/forwardingstreambuf.h"

namespace stanfordcpplib {
namespace qtgui {
class QGConsoleWindow : public QGWindow {
public:
    static bool consoleEnabled();
    static QGConsoleWindow* instance();
    static void setConsoleEnabled(bool enabled);

    virtual void clearConsole();
    virtual void clipboardCopy();
    virtual void clipboardCut();
    virtual void clipboardPaste();
    virtual void compareOutput();
    virtual std::string getBackground() const Q_DECL_OVERRIDE;
    virtual int getBackgroundInt() const Q_DECL_OVERRIDE;
    virtual std::string getColor() const Q_DECL_OVERRIDE;
    virtual int getColorInt() const Q_DECL_OVERRIDE;
    virtual std::string getErrorColor() const;
    virtual std::string getForeground() const Q_DECL_OVERRIDE;
    virtual int getForegroundInt() const Q_DECL_OVERRIDE;
    virtual std::string getOutputColor() const;
    virtual bool isClearEnabled() const;
    virtual bool isEcho() const;
    virtual bool isLocationSaved() const;
    virtual bool isLocked() const;
    virtual void loadConfiguration();
    virtual void loadInputScript(const std::string& filename);
    virtual void print(const std::string& str, bool isStdErr = false);
    virtual void println(bool isStdErr = false);
    virtual void println(const std::string& str, bool isStdErr = false);
    virtual std::string readLine();
    virtual void save();
    virtual void saveAs(const std::string& filename = "");
    virtual void saveConfiguration(bool prompt = true);
    virtual void showAboutDialog();
    virtual void showColorDialog(bool background = false);
    virtual void showFontDialog();
    virtual void showInputScriptDialog();
    virtual void showPrintDialog();
    virtual void selectAll();
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setClearEnabled(bool clearEnabled);
    virtual void setConsoleSize(double width, double height);
    virtual void setColor(int color) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setEcho(bool echo);
    virtual void setErrorColor(const std::string& errorColor);
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setForeground(int color) Q_DECL_OVERRIDE;
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setLocationSaved(bool locationSaved);
    virtual void setLocked(bool locked);
    virtual void setOutputColor(int rgb);
    virtual void setOutputColor(const std::string& outputColor);
    virtual void shutdown();

private:
    static const bool ALLOW_RICH_INPUT_EDITING;
    static const double DEFAULT_WIDTH;
    static const double DEFAULT_HEIGHT;
    static const double DEFAULT_X;
    static const double DEFAULT_Y;
    static const std::string CONFIG_FILE_NAME;
    static const std::string DEFAULT_WINDOW_TITLE;
    static const std::string DEFAULT_FONT_FAMILY;
    static const int DEFAULT_FONT_SIZE;
    static const int MIN_FONT_SIZE;
    static const int MAX_FONT_SIZE;
    static const std::string DEFAULT_BACKGROUND_COLOR;
    static const std::string DEFAULT_ERROR_COLOR;
    static const std::string DEFAULT_OUTPUT_COLOR;
    static const std::string USER_INPUT_COLOR;
    static QGConsoleWindow* _instance;
    static bool _consoleEnabled;

    QGConsoleWindow();
    void _initMenuBar();
    void _initWidgets();
    void _initStreams();
    virtual void checkForUpdates();
    QTextFragment getUserInputFragment() const;
    int getUserInputStart() const;
    int getUserInputEnd() const;
    bool isCursorInUserInputArea() const;
    void processBackspace(int key);
    void processEof();
    void processKeyPress(QGEvent event);
    void processUserInputEnterKey();
    void processUserInputKey(int key);
    virtual ~QGConsoleWindow();

    QGTextArea* _textArea;
    bool _clearEnabled;
    bool _echo;
    bool _locationSaved;
    bool _locked;
    bool _promptActive;
    bool _shutdown;
    std::string _errorColor;
    std::string _outputColor;
    std::string _inputBuffer;
    std::string _lastSaveFileName;
    Queue<std::string> _inputLines;
    Queue<std::string> _inputScript;
    stanfordcpplib::qtgui::ConsoleStreambufQt* _cinout_new_buf;
    stanfordcpplib::qtgui::ConsoleStreambufQt* _cerr_new_buf;
    QReadWriteLock _cinMutex;
    QReadWriteLock _cinQueueMutex;
    QMutex _coutMutex;
};

/*
 * Function: clearConsole
 * Usage: clearConsole();
 * ----------------------
 * Erases the contents of the console window.
 */
void clearConsole();

/*
 * Function: getConsoleClearEnabled
 * Usage: bool mode = getConsoleClearEnabled();
 * --------------------------------------------
 * Returns whether the console's "clear" function should be enabled.
 * Default true.  If false, clear() will just print a message
 * rather than actually clearing the console.
 */
bool getConsoleClearEnabled();

/*
 * Function: setConsoleCloseOperation();
 * Usage: if (getConsoleCloseOperation() == ...)
 * ---------------------------------------------
 * Returns what the console will do when the user hits its "close" button.
 * By default, this is CONSOLE_HIDE_ON_CLOSE unless set otherwise.
 */
QGWindow::CloseOperation getConsoleCloseOperation();

/*
 * Function: getConsoleEcho
 * Usage: bool echo = getConsoleEcho();
 * ------------------------------------
 * Returns whether or not the input/output from the Stanford graphical
 * console window is being echoed onto the standard operating system terminal
 * window. Initially this is false unless set to true by a previous call to
 * setConsoleEcho(true).
 */
bool getConsoleEcho();

/*
 * Returns whether the graphical console window is enabled.
 * This is true if you have included "qconsole.h" in your program.
 */
bool getConsoleEnabled();

/*
 * Returns whether the overall C++ program will terminate if the console
 * window is closed.  By default this is false initially.
 */
bool getConsoleExitProgramOnClose();

std::string getConsoleFont();

double getConsoleHeight();

QGPoint getConsoleLocation();

/*
 * Returns whether the location of the console window should be remembered and
 * restored between runs of the program.  Initially false.
 */
bool getConsoleLocationSaved();

/*
 * Function: getConsolePrintExceptions
 * Usage: bool ex = getConsolePrintExceptions();
 * ---------------------------------------------
 * Returns whether or not a feature is enabled that causes exceptions to be
 * echoed to the Stanford graphical console window when they are thrown.
 * Disabled (false) by default.
 */
bool getConsolePrintExceptions();

/*
 * Returns whether functions like setConsoleFont or setConsoleSize
 * are currently ignored.  See setConsoleSettingsLocked.
 */
bool getConsoleSettingsLocked();

QGDimension getConsoleSize();

double getConsoleWidth();

QGConsoleWindow* getConsoleWindow();

/*
 * Function: getConsoleWindowTitle
 * Usage: string title = getConsoleWindowTitle();
 * ----------------------------------------------
 * Returns the title bar text of the console window.
 */
std::string getConsoleWindowTitle();

void initializeQtGraphicalConsole();

/*
 * Function: setConsoleClearEnabled
 * Usage: setConsoleClearEnabled(true);
 * ------------------------------------
 * Returns whether the console's "clear" function should be enabled.
 * Default true.  If false, clear() will just print a message
 * rather than actually clearing the console.
 * This is sometimes used to facilitate creation of auto-grading scripts.
 */
void setConsoleClearEnabled(bool value);

/*
 * Function: setConsoleCloseOperation();
 * Usage: setConsoleCloseOperation(op);
 * ------------------------------------
 * Sets what the console should do when the user hits its "close" button.
 */
void setConsoleCloseOperation(QGWindow::CloseOperation op);

/*
 * Function: setConsoleEcho
 * Usage: setConsoleEcho(true);
 * ----------------------------
 * Enables or disables echoing the input/output from the Stanford graphical
 * console window onto the standard operating system terminal window.
 * Normally you don't need this echoing, but if you want to be able to copy
 * and paste your console interaction into another window, it is useful.
 */
void setConsoleEcho(bool echo);   // added by Marty

/*
 * Sets the color used to display text in the console window.
 * The color string passed should be in a hex format such as "#ffa32f";
 */
void setConsoleErrorColor(const std::string& color);

/*
 * Sets whether the overall C++ program should terminate if the console
 * window is closed.  By default this is false initially.
 */
void setConsoleExitProgramOnClose(bool exitOnClose);

/*
 * Function: setConsoleFont
 * Usage: setConsoleFont(font);
 * ----------------------------
 * Changes the font used for the console.  The <code>font</code> parameter
 * is typically a string in the form <code>family-style-size</code>.
 * In this string, <code>family</code> is the name of the font family;
 * <code>style</code> is either missing (indicating a plain font) or one
 * of the strings <code>Bold</code>, <code>Italic</code>, or
 * <code>BoldItalic</code>; and <code>size</code> is an integer
 * indicating the point size.  If any of these components is
 * specified as an asterisk, the existing value is retained.
 * The <code>font</code> parameter can also be a sequence of
 * such specifications separated by semicolons, in which case the
 * first available font on the system is used.
 */
void setConsoleFont(const std::string& font);

/*
 * Function: setConsoleLocation
 * Usage: setConsoleLocation(x, y);
 * --------------------------------
 * Changes the (x, y) pixel position of the console to the specified
 * coordinates, relative to the top/left of the screen.
 */
void setConsoleLocation(double x, double y);

/*
 * Sets whether the location of the console window should be remembered and
 * restored between runs of the program.  Default false.
 */
void setConsoleLocationSaved(bool value);

/*
 * Sets the color used to display text in the console window.
 * The color string passed should be in a hex format such as "#ffa32f";
 */
void setConsoleOutputColor(const std::string& color);

/*
 * Function: setConsolePrintExceptions
 * Usage: setConsolePrintExceptions(true);
 * ----------------------------
 * Enables or disables a feature that causes exceptions to be echoed to the
 * Stanford graphical console window when they are thrown.
 * Disabled (false) by default.
 * Note that using this feature may make it harder to get a stack trace in the
 * debugger if you are debugging the cause of an exception.
 */
void setConsolePrintExceptions(bool printExceptions);

/*
 * If set to true, disables functions like setConsoleFont or setConsoleSize,
 * to facilitate autograder construction by negating any such calls in the
 * student's main program.  Initially false.
 */
void setConsoleSettingsLocked(bool value);

/*
 * Function: setConsoleSize
 * Usage: setConsoleSize(width, height);
 * -------------------------------------
 * Changes the size of the console to the specified dimensions, measured
 * in pixels.
 */
void setConsoleSize(double width, double height);

/*
 * Function: setConsoleWindowTitle
 * Usage: setConsoleWindowTitle(title);
 * ------------------------------------
 * Changes the title bar text of the console window to the specified text.
 */
void setConsoleWindowTitle(const std::string& title);

/*
 * Function: shutdownConsole
 * Usage: shutdownConsole();
 * -------------------------
 * Closes the graphical console window and turns it off.
 */
void shutdownConsole();

} // namespace qtgui
} // namespace stanfordcpplib

// defined in gwindow.h/cpp
extern void pause(double milliseconds);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qconsole_h

/*
 * qconsole.h is weird in that a student's program must be able to #include it
 * and then magically receive the graphical console instead of the standard one;
 * but we want other lib files to be able to include console.h to get the
 * function prototypes without actually turning the graphical console on.
 * To achieve this, we have the __DONT_ENABLE_QT_GRAPHICAL_CONSOLE flag that lib
 * files can set right before #include'ing qconsole.h.  If they do so, it will
 * declare the prototypes but not initialize the graphical console.
 */
#if (!defined(__DONT_ENABLE_GRAPHICAL_CONSOLE) && !defined(__DONT_ENABLE_QT_GRAPHICAL_CONSOLE))

namespace stanfordcpplib {
void setQtConsoleEnabled(bool enabled);

namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void setConsoleEnabled(bool);
}

#ifndef __QtConsoleInitializer_created
#define __QtConsoleInitializer_created
class __QtConsoleInitializer {
public:
    /*
     * Code to initialize the library.
     * Implemented as a class constructor so that it will run before the
     * student's main function.
     */
    __QtConsoleInitializer() {
        // TODO: re-enable? what about prints during static phase?
        // qtgui::initializeQtGraphicalConsole();
        qtgui::setConsoleEnabled(true);
    }
};
static __QtConsoleInitializer __qt_console_init;
#endif // __QtConsoleInitializer_created
} // namespace stanfordcpplib

#endif // __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#endif // SQL_QT_GUI
