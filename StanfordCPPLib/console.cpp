/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 */

#include <string>
#include "console.h"
#include "error.h"
#include "platform.h"
using namespace std;

static void sclTerminateHandler();

static Platform* pp = getPlatform();
static bool consoleEcho = false;
static bool consoleEventOnClose = false;
static bool consoleExitProgramOnClose = false;
static bool consoleLocationSaved = false;
static bool consolePrintExceptions = false;
static void (*old_terminate)() = NULL;

void clearConsole() {
    if (autograder::isConsoleAutograderMode()) {
        cout << "==================== (console cleared) ====================" << endl;
    } else {
        pp->jbeconsole_clear();
    }
}

bool getConsoleEcho() {
    return consoleEcho;
}

bool getConsolePrintExceptions() {
    return consolePrintExceptions;
}

void setConsoleEcho(bool echo) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleEcho = echo;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleExitProgramOnClose = exitOnClose;
    pp->jbeconsole_setExitProgramOnClose(exitOnClose);
}

bool getConsoleExitProgramOnClose() {
    return consoleExitProgramOnClose;
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleEventOnClose = eventOnClose;
}

bool getConsoleEventOnClose() {
    return consoleEventOnClose;
}

void setConsoleLocationSaved(bool value) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleLocationSaved = value;
    pp->jbeconsole_setLocationSaved(value);
}

bool getConsoleLocationSaved() {
    return consoleLocationSaved;
}

void setConsoleFont(const string & font) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setFont(font);
}

void setConsolePrintExceptions(bool printExceptions) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    if (printExceptions && !consolePrintExceptions) {
        old_terminate = set_terminate(sclTerminateHandler);
    } else if (!printExceptions && consolePrintExceptions) {
        set_terminate(old_terminate);
    }
    consolePrintExceptions = printExceptions;
}

void setConsoleSize(double width, double height) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setSize(width, height);
}

void setConsoleLocation(int x, int y) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setLocation(x, y);
}

namespace autograder {
static bool consoleEchoUserInput = false;
static bool consoleAutograderMode = false;
static bool consoleLocked = false;
static string consoleLogFile = "";

bool getConsoleEchoUserInput() {
    return consoleEchoUserInput;
}

string getConsoleLogFile() {
    return consoleLogFile;
}

bool isConsoleAutograderMode() {
    return consoleAutograderMode;
}

void setConsoleAutograderMode(bool value) {
    consoleAutograderMode = value;
}

void setConsoleEchoUserInput(bool echo) {
    consoleEchoUserInput = echo;
}

void setConsoleLogFile(const string & filename) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleLogFile = filename;
}

void setConsoleSettingsLocked(bool value) {
    consoleLocked = value;
}

bool isConsoleSettingsLocked() {
    return consoleLocked;
}

void addInputButton(const string& text, const string& input) {
    pp->autograderinput_addButton(text, input);
}

void addInputCategory(const string& name) {
    pp->autograderinput_addCategory(name);
}

void removeInputCategory(const string& name) {
    pp->autograderinput_removeCategory(name);
}

void removeInputButton(const string& text) {
    pp->autograderinput_removeButton(text);
}
}

static void sclTerminateHandler() {
    ostream& out = cerr;
    try {
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An ErrorException occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw ex;
    } catch (const InterruptedIOException& /* iex */) {
        // blocked console I/O was interrupted; just exit program immediately
        // (doesn't close any other JBE-generated GUI windows, but oh well)
        cout.flush();
        exit(0);
    } catch (const std::exception& ex) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An exception occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw ex;
    } catch (std::string str) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        cout.flush();
        out << msg;
        throw str;
    } catch (char const* str) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        cout.flush();
        out << msg;
        throw str;
    } catch (int n) {
        char buf[128];
        snprintf(buf, 128, "%d", n);
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An int exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw n;
    } catch (long l) {
        char buf[128];
        snprintf(buf, 128, "%ld", l);
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A long exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw l;
    } catch (char c) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A char exception occurred during program execution: \n";
        msg += " *** '";
        msg += c;
        msg += "'\n ***\n";
        cout.flush();
        out << msg;
        throw c;
    } catch (bool b) {
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A bool exception occurred during program execution: \n";
        msg += " *** ";
        msg += (b ? "true" : "false");
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw b;
    } catch (double d) {
        char buf[128];
        snprintf(buf, 128, "%lf", d);
        string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A double exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        cout.flush();
        out << msg;
        throw d;
    }
    abort();
}
