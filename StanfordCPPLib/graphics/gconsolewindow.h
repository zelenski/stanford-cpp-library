/*
 * File: gconsolewindow.h
 * ----------------------
 * 
 * @author Marty Stepp
 * @version 2018/09/23
 * - added getFont
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - initial version, separated out from console .cpp/h
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gconsolewindow_h
#define _gconsolewindow_h

#include <iostream>
#include <sstream>
#include <string>
#include <Qt>
#include <QMutex>
#include <QWindow>
#include <QKeyEvent>
#include <QReadWriteLock>
#include <QTextEdit>
#include <QTextFrame>

#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "gtextarea.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "queue.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#define INTERNAL_INCLUDE 1
#include "private/consolestreambuf.h"
#define INTERNAL_INCLUDE 1
#include "private/forwardingstreambuf.h"
#undef INTERNAL_INCLUDE

/**
 * A GConsoleWindow represents the system console.
 * Most students/clients do not need to utilize this class directly.
 * Output is implicitly printed to the console when you use cout/cerr.
 * But if you want to access the console window directly and interact with it
 * as you would a normal GWindow, you can do so by calling the global function
 * getConsoleWindow.
 *
 * @private
 *
 * TODO: document
 */
class GConsoleWindow : public GWindow {
public:
    static bool consoleEnabled();
    static std::string getDefaultFont();
    static GConsoleWindow* instance();
    static bool isInitialized();
    static void setConsoleEnabled(bool enabled);

    virtual void clearConsole();
    virtual void clipboardCopy();
    virtual void clipboardCut();
    virtual void clipboardPaste();
    virtual void close() Q_DECL_OVERRIDE;
    virtual void compareOutput(const std::string& filename);
    virtual std::string getAllOutput() const;
    virtual std::string getBackground() const Q_DECL_OVERRIDE;
    virtual int getBackgroundInt() const Q_DECL_OVERRIDE;
    virtual std::string getColor() const Q_DECL_OVERRIDE;
    virtual int getColorInt() const Q_DECL_OVERRIDE;
    virtual std::string getErrorColor() const;
    virtual std::string getFont() const Q_DECL_OVERRIDE;
    virtual std::string getForeground() const Q_DECL_OVERRIDE;
    virtual int getForegroundInt() const Q_DECL_OVERRIDE;
    virtual std::string getOutputColor() const;
    virtual bool isClearEnabled() const;
    virtual bool isEcho() const;
    virtual bool isLocationSaved() const;
    virtual bool isLocked() const;
    virtual void loadConfiguration();
    virtual void loadInputScript(int number);
    virtual void loadInputScript(const std::string& filename);
    virtual void print(const std::string& str, bool isStdErr = false);
    virtual void println(bool isStdErr = false);
    virtual void println(const std::string& str, bool isStdErr = false);
    virtual std::string readLine();
    virtual void save();
    virtual void saveAs(const std::string& filename = "");
    virtual void saveConfiguration(bool prompt = true);
    virtual void selectAll();
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setClearEnabled(bool clearEnabled);
    virtual void setConsoleSize(double width, double height);
    virtual void setColor(int color) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setEcho(bool echo);
    virtual void setErrorColor(const std::string& errorColor);
    virtual void setFont(const QFont& font) Q_DECL_OVERRIDE;
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setForeground(int color) Q_DECL_OVERRIDE;
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setLocationSaved(bool locationSaved);
    virtual void setLocked(bool locked);
    virtual void setOutputColor(int rgb);
    virtual void setOutputColor(const std::string& outputColor);
    virtual void setSize(double width, double height) Q_DECL_OVERRIDE;
    virtual void showAboutDialog();
    virtual void showColorDialog(bool background = false);
    virtual void showCompareOutputDialog();
    virtual void showFontDialog();
    virtual void showInputScriptDialog();
    virtual void showPrintDialog();
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
    static const std::string DEFAULT_FONT_WEIGHT;
    static const int DEFAULT_FONT_SIZE;
    static const int MIN_FONT_SIZE;
    static const int MAX_FONT_SIZE;
    static const std::string DEFAULT_BACKGROUND_COLOR;
    static const std::string DEFAULT_ERROR_COLOR;
    static const std::string DEFAULT_OUTPUT_COLOR;
    static const std::string USER_INPUT_COLOR;
    static GConsoleWindow* _instance;
    static bool _consoleEnabled;

    Q_DISABLE_COPY(GConsoleWindow)

    GConsoleWindow();
    virtual ~GConsoleWindow();
    void _initMenuBar();
    void _initWidgets();
    void _initStreams();
    virtual void checkForUpdates();
    QTextFragment getUserInputFragment() const;
    int getUserInputStart() const;
    int getUserInputEnd() const;
    bool isCursorInUserInputArea() const;
    bool isSelectionInUserInputArea() const;
    void processBackspace(int key);
    void processCommandHistory(int delta);
    void processEof();
    void processKeyPress(GEvent event);
    void processUserInputEnterKey();
    void processUserInputKey(int key);
    void setUserInput(const std::string& userInput);

    GTextArea* _textArea;
    bool _clearEnabled;
    bool _echo;
    bool _locationSaved;
    bool _locked;
    bool _promptActive;
    bool _shutdown;
    int _commandHistoryIndex;
    std::string _errorColor;
    std::string _outputColor;
    std::string _inputBuffer;
    std::string _lastSaveFileName;
    Queue<std::string> _inputLines;
    Queue<std::string> _inputScript;
    Vector<std::string> _inputCommandHistory;
    stanfordcpplib::qtgui::ConsoleStreambufQt* _cinout_new_buf;
    stanfordcpplib::qtgui::ConsoleStreambufQt* _cerr_new_buf;
    std::streambuf* _cin_old_buf;
    std::streambuf* _cout_old_buf;
    std::streambuf* _cerr_old_buf;
    std::ostringstream _allOutputBuffer;
    QReadWriteLock _cinMutex;
    QReadWriteLock _cinQueueMutex;
    QMutex _coutMutex;
};

#endif // _gconsolewindow_h
