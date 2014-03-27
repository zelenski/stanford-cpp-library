/*
 * File: platform.cpp
 * ------------------
 * This file implements the platform interface by passing commands to
 * a Java back end that manages the display.
 */

#ifdef _WIN32
#  include <windows.h>
#  include <tchar.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/resource.h>
#  include <dirent.h>
#  include <errno.h>
#  include <pwd.h>
#  include <stdint.h>
#  include <unistd.h>
static bool tracePipe;
static int pin;
static int pout;
#endif

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>
#include <vector>
#include "private/version.h"
#include "error.h"
#include "filelib.h"
#include "gevents.h"
#include "gtimer.h"
#include "gtypes.h"
#include "hashmap.h"
#include "queue.h"
#include "platform.h"
#include "stack.h"
#include "strlib.h"
#include "tokenscanner.h"
#include "vector.h"

using namespace std;

// internal flag to emit a dump of every message sent to the Java back-end;
// used for debugging purposes
static const bool PIPE_DEBUG = false;

static string getLineConsole();
static void putConsole(const string& str);
static void endLineConsole();
static void echoConsole(const string& str);
static void fileLogConsole(const string& str);
static int scanInt(TokenScanner& scanner);
static double scanDouble(TokenScanner& scanner);
static GDimension scanDimension(const string& str);
static Point scanPoint(const string& str);
static GRectangle scanRectangle(const string& str);
static void setConsoleProperties();
void startupMainDontRunMain(int argc, char** argv);
//static void abortAllConsoleIO();

class ConsoleStreambuf : public streambuf {

private:

    /* Constants */

    static const int BUFFER_SIZE = 1024;

    /* Instance variables */

    char inBuffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];
    int blocked;

public:

    ConsoleStreambuf() {
        setg(inBuffer, inBuffer, inBuffer);
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        blocked = 0;
    }

    ~ConsoleStreambuf() {
        /* Empty */
    }

    bool isBlocked() {
        return blocked > 0;
    }

    virtual int underflow() {
        // Allow long strings at some point
        blocked++;
        string line = getLineConsole();
        blocked--;
        int n = line.length();
        if (n + 1 >= BUFFER_SIZE) {
            error("String too long");
        }
        for (int i = 0; i < n; i++) {
            inBuffer[i] = line[i];
        }
        inBuffer[n++] = '\n';
        inBuffer[n] = '\0';
        setg(inBuffer, inBuffer, inBuffer + n);
        return inBuffer[0];
    }

    virtual int overflow(int ch = EOF) {
        string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                putConsole(line);
                endLineConsole();
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            putConsole(line);
        }
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        if (ch != EOF) {
            outBuffer[0] = ch;
            pbump(1);
        }
        return ch != EOF;
    }

    virtual int sync() {
        return overflow();
    }
};

/* Private data */

static Queue<GEvent> eventQueue;
static HashMap<string, GTimerData*> timerTable;
static HashMap<string, GWindowData*> windowTable;
static HashMap<string, GObject*> sourceTable;
static HashMap<string, string> optionTable;
static string programName;
static ofstream logfile;
static streambuf* cin_old_buf;
static streambuf* cout_old_buf;
static ConsoleStreambuf* cinout_new_buf;


#ifdef _WIN32
static HANDLE rdFromJBE = NULL;
static HANDLE wrFromJBE = NULL;
static HANDLE rdToJBE = NULL;
static HANDLE wrToJBE = NULL;
#endif

/* Prototypes */

static void initPipe();
static void putPipe(string line);
static string getPipe();
static string getResult(bool consumeAcks = false);
static void getStatus();
static GEvent parseEvent(string line);
static GEvent parseMouseEvent(TokenScanner& scanner, EventType type);
static GEvent parseKeyEvent(TokenScanner& scanner, EventType type);
static GEvent parseTimerEvent(TokenScanner& scanner, EventType type);
static GEvent parseWindowEvent(TokenScanner& scanner, EventType type);
static GEvent parseActionEvent(TokenScanner& scanner, EventType type);

/* Implementation of the Platform class */

Platform::Platform() {
    /* Empty */
}

Platform::~Platform() {
    /* Empty */
}

/* Unix implementations of filelib.h primitives */

#ifndef _WIN32

bool Platform::filelib_fileExists(string filename) {
    struct stat fileInfo;
    return stat(filename.c_str(), &fileInfo) == 0;
}

bool Platform::filelib_isFile(string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISREG(fileInfo.st_mode) != 0;
}

bool Platform::filelib_isSymbolicLink(string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISLNK(fileInfo.st_mode) != 0;
}

bool Platform::filelib_isDirectory(string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISDIR(fileInfo.st_mode) != 0;
}

void Platform::filelib_setCurrentDirectory(string path) {
    if (chdir(path.c_str()) == 0) {
        string msg = "setCurrentDirectory: ";
        string err = string(strerror(errno));
        error(msg + err);
    }
}

string Platform::filelib_getCurrentDirectory() {
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
        string msg = "getCurrentDirectory: ";
        string err = string(strerror(errno));
        error(msg + err);
    }
    string result = string(cwd);
    free(cwd);
    return result;
}

void Platform::filelib_createDirectory(string path) {
    if (endsWith(path, "/")) {
        path = path.substr(0, path.length() - 2);
    }
    if (mkdir(path.c_str(), 0777) != 0) {
        if (errno == EEXIST && filelib_isDirectory(path)) return;
        string msg = "createDirectory: ";
        string err = string(strerror(errno));
        error(msg + err);
    }
}

string Platform::filelib_getDirectoryPathSeparator() {
    return "/";
}

string Platform::filelib_getSearchPathSeparator() {
    return ":";
}

string Platform::filelib_expandPathname(string filename) {
    if (filename == "") return "";
    int len = filename.length();
    if (filename[0] == '~') {
        int spos = 1;
        while (spos < len && filename[spos] != '\\' && filename[spos] != '/') {
            spos++;
        }
        char *homedir = NULL;
        if (spos == 1) {
            homedir = getenv("HOME");
            if (homedir == NULL) homedir = getpwuid(getuid())->pw_dir;
        } else {
            struct passwd *pw = getpwnam(filename.substr(1, spos - 1).c_str());
            if (pw == NULL) error("expandPathname: No such user");
            homedir = pw->pw_dir;
        }
        filename = string(homedir) + filename.substr(spos);
        len = filename.length();
    }
    for (int i = 0; i < len; i++) {
        if (filename[i] == '\\') filename[i] = '/';
    }
    return filename;
}

void Platform::filelib_listDirectory(string path, vector<string>& list) {
    if (path == "") path = ".";
    DIR *dir = opendir(path.c_str());
    if (dir == NULL) error(string("listDirectory: Can't open ") + path);
    list.clear();
    while (true) {
        struct dirent *ep = readdir(dir);
        if (ep == NULL) break;
        string name = string(ep->d_name);
        if (name != "." && name != "..") list.push_back(name);
    }
    closedir(dir);
    sort(list.begin(), list.end());
}

#else

bool Platform::filelib_fileExists(string filename) {
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool Platform::filelib_isFile(string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_NORMAL);
}

bool Platform::filelib_isSymbolicLink(string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES
            && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

bool Platform::filelib_isDirectory(string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

void Platform::filelib_setCurrentDirectory(string path) {
    if (!filelib_isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
        error("setCurrentDirectory: Can't change to " + path);
    }
}

string Platform::filelib_getCurrentDirectory() {
    char path[MAX_PATH + 1];
    int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
    return string(path, n);
}

void Platform::filelib_createDirectory(string path) {
    if (!CreateDirectoryA(path.c_str(), NULL)) {
        error("createDirectory: Can't create " + path);
    }
}

string Platform::filelib_getDirectoryPathSeparator() {
    return "\\";
}

string Platform::filelib_getSearchPathSeparator() {
    return ";";
}

string Platform::filelib_expandPathname(string filename) {
    if (filename == "") return "";
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        if (filename[i] == '/') filename[i] = '\\';
    }
    return filename;
}

void Platform::filelib_listDirectory(string path, vector<string> & list) {
    if (path == "") path = ".";
    string pattern = path + "\\*.*";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        error("listDirectory: Can't list directory");
    }
    list.clear();
    while (true) {
        string name = string(fd.cFileName);
        if (name != "." && name != "..") list.push_back(name);
        if (!FindNextFileA(h, &fd)) break;
    }
    FindClose(h);
    sort(list.begin(), list.end());
}

#endif

/*
 * This function increases the system stack size on Unixy platforms (Linux+Mac),
 * making it possible to make many nested calls of depth-first search without
 * crashing the system.
 * Windows MinGW systems set their stack size through compiler/linker flags
 * that are set in the project's .pro file.
 */
#ifdef _WIN32
    // Windows doesn't support this operation
void Platform::setStackSize(unsigned int /* stackSize */) {
#else
void Platform::setStackSize(unsigned int stackSize) {
    // Linux/Mac definition (they DO support stack size changing)
#if defined(__USE_LARGEFILE64)
    // 64-bit version
    rlimit64 rl;
    int result = getrlimit64(RLIMIT_STACK, &rl);
#else
    // 32-bit version
    rlimit rl;
    int result = getrlimit(RLIMIT_STACK, &rl);
#endif
    if (result == 0) {
        if (rl.rlim_cur < stackSize || rl.rlim_max < stackSize) {
            rl.rlim_cur = (rlim_t) stackSize;
            rl.rlim_max = (rlim_t) stackSize;
#if defined(__USE_LARGEFILE64)
            // 64-bit version
            result = setrlimit64(RLIMIT_STACK, &rl);
#else
            result = setrlimit(RLIMIT_STACK, &rl);
#endif
            if (result != 0) {
                cerr << "Warning: Unable to increase system stack size to "
                     << stackSize << endl;
            }
        }
    }
#endif // _WIN32
}


bool Platform::regex_match(string s, string regexp) {
    ostringstream os;
    os << "Regex.match(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    string result = getResult();
    return (result == "true");
}

int Platform::regex_matchCount(string s, string regexp) {
    ostringstream os;
    os << "Regex.matchCount(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    string result = getResult();
    return stringToInteger(result);
}

int Platform::regex_matchCountWithLines(string s, string regexp, string& linesOut) {
    ostringstream os;
    os << "Regex.matchCountWithLines(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    linesOut = getResult();       // "count:line,line,line,...,line"
    string countStr = "";
    while (linesOut[0] != ':') {
        countStr += linesOut[0];
        linesOut = linesOut.substr(1);
    }
    linesOut = linesOut.substr(1);    // remove the initial ':'
    return stringToInteger(countStr);
}

string Platform::regex_replace(string s, string regexp, string replacement, int /* limit */) {
    ostringstream os;
    os << "Regex.replace(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ",";
    writeQuotedString(os, urlEncode(replacement));
    os << ")";
    putPipe(os.str());
    string result = getResult();
    return urlDecode(result);
}

string Platform::file_openFileDialog(string title, string mode, string path) {
    ostringstream os;
    os << "File.openFileDialog(";
    writeQuotedString(os, title);
    os << ", \"" << mode << "\", ";
    writeQuotedString(os, path);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gwindow_constructor(const GWindow& gw, double width, double height,
                             GObject *topCompound) {
    ostringstream os;
    os << gw.gwd;
    string id = os.str();
    windowTable.put(id, gw.gwd);
    os.str("");
    os << "GWindow.create(\"" << id << "\", " << width << ", " << height
       << ", \"" << topCompound << "\")";
    putPipe(os.str());
    getStatus();
}

void Platform::gwindow_delete(const GWindow& gw) {
    ostringstream os;
    os << gw.gwd;
    string id = os.str();
    windowTable.remove(id);
    os.str("");
    os << "GWindow.delete(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_close(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.close(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_requestFocus(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.requestFocus(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setExitOnClose(const GWindow& gw, bool value) {
    ostringstream os;
    os << "GWindow.setExitOnClose(\"" << gw.gwd << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gwindow_clear(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.clear(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_repaint(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.repaint(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setSize(const GWindow& gw, int width, int height) {
    ostringstream os;
    os << "GWindow.setSize(\"" << gw.gwd << "\", " << width << ", " << height
       << ")";
    putPipe(os.str());
}

void Platform::gwindow_setCanvasSize(const GWindow& gw, int width, int height) {
    ostringstream os;
    os << "GWindow.setCanvasSize(\"" << gw.gwd << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::gwindow_pack(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.pack(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setTitle(const GWindow& gw, string title) {
    ostringstream os;
    os << "GWindow.setTitle(\"" << gw.gwd << "\", ";
    writeQuotedString(os, title);
    os << ")";
    putPipe(os.str());
}

void Platform::gwindow_setLocation(const GWindow& gw, int x, int y) {
    ostringstream os;
    os << "GWindow.setLocation(\"" << gw.gwd << "\", " << x << ", " << y << ")";
    putPipe(os.str());
}

void Platform::gwindow_setLocationSaved(const GWindow& gw, bool value) {
    ostringstream os;
    os << "GWindow.setLocationSaved(\"" << gw.gwd << "\", "
       << boolalpha << value << ")";
    putPipe(os.str());
}

double Platform::gwindow_getScreenHeight() {
    putPipe("GWindow.getScreenHeight()");
    return stringToReal(getResult());
}

GDimension Platform::gwindow_getScreenSize() {
    putPipe("GWindow.getScreenSize()");
    string result = getResult();
    if (!startsWith(result, "GDimension(")) error(result);
    return scanDimension(result);
}

double Platform::gwindow_getScreenWidth() {
    putPipe("GWindow.getScreenWidth()");
    return stringToReal(getResult());
}

void Platform::gtimer_pause(double milliseconds) {
    ostringstream os;
    os << "GTimer.pause(" << milliseconds << ")";
    putPipe(os.str());
    getStatus();
}

void Platform::gtimer_constructor(const GTimer& timer, double delay) {
    ostringstream os;
    os << timer.gtd;
    string id = os.str();
    timerTable.put(id, timer.gtd);
    os.str("");
    os << "GTimer.create(\"" << id << "\", " << delay << ")";
    putPipe(os.str());
}

void Platform::gtimer_delete(const GTimer& timer) {
    ostringstream os;
    os << timer.gtd;
    string id = os.str();
    timerTable.remove(id);
    os.str("");
    os << "GTimer.deleteTimer(\"" << id << "\")";
    putPipe(os.str());
}

void Platform::gtimer_start(const GTimer& timer) {
    ostringstream os;
    os << "GTimer.startTimer(\"" << timer.gtd << "\")";
    putPipe(os.str());
}

void Platform::gtimer_stop(const GTimer& timer) {
    ostringstream os;
    os << "GTimer.stopTimer(\"" << timer.gtd << "\")";
    putPipe(os.str());
}

void Platform::sound_constructor(Sound *sound, string filename) {
    ostringstream os;
    os << "Sound.create(\"" << sound << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    getStatus();
}

void Platform::sound_delete(Sound *sound) {
    ostringstream os;
    os << "Sound.delete(\"" << sound << "\")";
    putPipe(os.str());
}

void Platform::sound_play(Sound *sound) {
    ostringstream os;
    os << "Sound.play(\"" << sound << "\")";
    putPipe(os.str());
}

void Platform::gobject_delete(GObject* gobj) {
    ostringstream os;
    os << "GObject.delete(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gcompound_add(GObject *compound, GObject* gobj) {
    ostringstream os;
    os << "GCompound.add(\"" << compound << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_remove(GObject* gobj) {
    ostringstream os;
    os << "GObject.remove(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setRegionAlignment(const GWindow& gw, string region,
                                  string align) {
    ostringstream os;
    os << "GWindow.setRegionAlignment(\"" << gw.gwd << "\", \"" << region
       << "\", \"" << align << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setResizable(const GWindow& gw, bool value) {
    ostringstream os;
    os << "GWindow.setResizable(\"" << gw.gwd << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gwindow_addToRegion(const GWindow& gw, GObject* gobj, string region) {
    ostringstream os;
    os << "GWindow.addToRegion(\"" << gw.gwd << "\", \"" << gobj << "\", \""
       << region << "\")";
    putPipe(os.str());
}

Point Platform::gwindow_getLocation(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.getLocation(\"" << gw.gwd << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "Point(")) {
        error(result);
    }
    return scanPoint(result);
}

GDimension Platform::gwindow_getRegionSize(const GWindow& gw, string region) {
    ostringstream os;
    os << "GWindow.getRegionSize(\"" << gw.gwd << "\", \"" << region << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error(result);
    }
    return scanDimension(result);
}

void Platform::gwindow_removeFromRegion(const GWindow& gw, GObject* gobj,
                                string region) {
    ostringstream os;
    os << "GWindow.removeFromRegion(\"" << gw.gwd << "\", \""
       << gobj << "\", \"" << region << "\")";
    putPipe(os.str());
}

GDimension Platform::gwindow_getSize(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.getSize(\"" << gw.gwd << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error(result);
    }
    return scanDimension(result);
}

GDimension Platform::gwindow_getCanvasSize(const GWindow& gw) {
    ostringstream os;
    os << "GWindow.getCanvasSize(\"" << gw.gwd << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error(result);
    }
    return scanDimension(result);
}

void Platform::gobject_sendForward(GObject* gobj) {
    ostringstream os;
    os << "GObject.sendForward(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendToFront(GObject* gobj) {
    ostringstream os;
    os << "GObject.sendToFront(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendBackward(GObject* gobj) {
    ostringstream os;
    os << "GObject.sendBackward(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendToBack(GObject* gobj) {
    ostringstream os;
    os << "GObject.sendToBack(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_setVisible(GObject* gobj, bool flag) {
    ostringstream os;
    os << boolalpha << "GObject.setVisible(\"" << gobj << "\", " << flag << ")";
    putPipe(os.str());
}

void Platform::gwindow_setVisible(const GWindow& gw, bool flag) {
    ostringstream os;
    os << boolalpha << "GWindow.setVisible(\"" << gw.gwd << "\", " << flag << ")";
    putPipe(os.str());
}

void Platform::gobject_setColor(GObject* gobj, string color) {
    ostringstream os;
    os << "GObject.setColor(\"" << gobj << "\", \"" << color << "\")";
    putPipe(os.str());
}

void Platform::gobject_scale(GObject* gobj, double sx, double sy) {
    ostringstream os;
    os << "GObject.scale(\"" << gobj << "\", " << sx << ", " << sy << ")";
    putPipe(os.str());
}

void Platform::gobject_rotate(GObject* gobj, double theta) {
    ostringstream os;
    os << "GObject.rotate(\"" << gobj << "\", " << theta << ")";
    putPipe(os.str());
}

// Move this computation into gobjects.cpp

bool Platform::gobject_contains(const GObject* gobj, double x, double y) {
    ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GObject.contains(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
        return getResult() == "true";
    } else {
        return false;
    }
}

// Move this computation into gobjects.cpp

GRectangle Platform::gobject_getBounds(const GObject* gobj) {
    ostringstream os;
    os << "GObject.getBounds(\"" << gobj << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "GRectangle(")) error(result);
    return scanRectangle(result);
}

void Platform::gobject_setLineWidth(GObject* gobj, double lineWidth) {
    ostringstream os;
    os << "GObject.setLineWidth(\"" << gobj << "\", " << lineWidth << ")";
    putPipe(os.str());
}

void Platform::gobject_setLocation(GObject* gobj, double x, double y) {
    ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GObject.setLocation(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("Platform::setLocation: x and y must be non-negative");
    }
}

void Platform::gobject_setSize(GObject* gobj, double width, double height) {
    ostringstream os;
    os << "GObject.setSize(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

bool Platform::ginteractor_isEnabled(GObject* gint) {
    ostringstream os;
    os << "GInteractor.isEnabled(\"" << gint << ")";
    putPipe(os.str());
    return getResult() == "true";
}

void Platform::ginteractor_setEnabled(GObject* gint, bool value) {
    ostringstream os;
    os << "GInteractor.setEnabled(\"" << gint << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gobject_setAntialiasing(bool value) {
    ostringstream os;
    os << "GObject.setAntialiasing(" << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::garc_setFrameRectangle(GObject* gobj, double x, double y,
                                 double width, double height) {
    ostringstream os;
    if (x >= 0 && y >= 0 && width >= 0 && height >= 0) {
        os << "GArc.setFrameRectangle(\"" << gobj << "\", "
           << x << ", " << y << ", "
           << width << ", " << height << ")";
        putPipe(os.str());
    } else {
        error("Platform::setFrameRectangle: x, y, w, h must all be non-negative");
    }
}

void Platform::gwindow_draw(const GWindow& gw, const GObject* gobj) {
    ostringstream os;
    os << "GWindow.draw(\"" << gw.gwd << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gwindow_drawInBackground(const GWindow& gw, const GObject* gobj) {
    ostringstream os;
    os << "GWindow.drawInBackground(\"" << gw.gwd << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_setFilled(GObject* gobj, bool flag) {
    ostringstream os;
    os << boolalpha << "GObject.setFilled(\"" << gobj << "\", " << flag << ")";
    putPipe(os.str());
}

void Platform::gobject_setFillColor(GObject* gobj, string color) {
    ostringstream os;
    os << "GObject.setFillColor(\"" << gobj << "\", \"" << color << "\")";
    putPipe(os.str());
}

void Platform::grect_constructor(GObject* gobj, double width, double height) {
    ostringstream os;
    os << "GRect.create(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::groundrect_constructor(GObject* gobj, double width, double height,
                                double corner) {
    ostringstream os;
    os << "GRoundRect.create(\"" << gobj << "\", " << width << ", " << height
       << ", " << corner << ")";
    putPipe(os.str());
}

void Platform::g3drect_constructor(GObject* gobj, double width, double height,
                             bool raised) {
    ostringstream os;
    os << boolalpha << "G3DRect.create(\"" << gobj << "\", "
       << width << ", " << height << ", " << raised << ")";
    putPipe(os.str());
}

void Platform::g3drect_setRaised(GObject* gobj, bool raised) {
    ostringstream os;
    os << boolalpha << "G3DRect.setRaised(\"" << gobj << "\", "
       << raised << ")";
    putPipe(os.str());
}

void Platform::glabel_constructor(GObject* gobj, string label) {
    ostringstream os;
    // *** BUGBUG: must escape quotation marks in label string (Marty)
    os << "GLabel.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

void Platform::gline_constructor(GObject* gobj, double x1, double y1,
                           double x2, double y2) {
    ostringstream os;
    os << "GLine.create(\"" << gobj << "\", " << x1 << ", " << y1
       << ", " << x2 << ", " << y2 << ")";
    putPipe(os.str());
}

void Platform::gline_setStartPoint(GObject* gobj, double x, double y) {
    ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GLine.setStartPoint(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("Platform::setStartPoint: x and y must both be non-negative");
    }
}

void Platform::gline_setEndPoint(GObject* gobj, double x, double y) {
    ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GLine.setEndPoint(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("Platform::setEndPoint: x and y must both be non-negative");
    }
}

void Platform::garc_constructor(GObject* gobj, double width, double height,
                          double start, double sweep) {
    ostringstream os;
    os << "GArc.create(\"" << gobj << "\", " << width << ", " << height
       << ", " << start << ", " << sweep << ")";
    putPipe(os.str());
}

void Platform::garc_setStartAngle(GObject* gobj, double angle) {
    ostringstream os;
    os << "GArc.setStartAngle(\"" << gobj << "\", " << angle << ")";
    putPipe(os.str());
}

void Platform::garc_setSweepAngle(GObject* gobj, double angle) {
    ostringstream os;
    os << "GArc.setSweepAngle(\"" << gobj << "\", " << angle << ")";
    putPipe(os.str());
}

GDimension Platform::gimage_constructor(GObject* gobj, string filename) {
    ostringstream os;
    os << "GImage.create(\"" << gobj << "\", \"" << filename << "\")";
    putPipe(os.str());
    string result = getResult();
    if (!startsWith(result, "GDimension(")) error(result);
    return scanDimension(result);
}

void Platform::gpolygon_constructor(GObject* gobj) {
    ostringstream os;
    os << "GPolygon.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gpolygon_addVertex(GObject* gobj, double x, double y) {
    ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GPolygon.addVertex(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("Platform::addVertex: x and y must both be non-negative");
    }
}

void Platform::goval_constructor(GObject* gobj, double width, double height) {
    ostringstream os;
    os << "GOval.create(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::ginteractor_setActionCommand(GObject* gobj, string cmd) {
    ostringstream os;
    os << "GInteractor.setActionCommand(\"" << gobj << "\", ";
    writeQuotedString(os, cmd);
    os << ")";
    putPipe(os.str());
}

GDimension Platform::ginteractor_getSize(GObject* gobj) {
    ostringstream os;
    os << "GInteractor.getSize(\"" << gobj << "\")";
    putPipe(os.str());
    return scanDimension(getResult());
}

void Platform::gbutton_constructor(GObject* gobj, string label) {
    ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GButton.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

void Platform::gcheckbox_constructor(GObject* gobj, string label) {
    ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GCheckBox.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

bool Platform::gcheckbox_isSelected(GObject* gobj) {
    ostringstream os;
    os << "GCheckBox.isSelected(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult() == "true";
}

void Platform::gcheckbox_setSelected(GObject* gobj, bool state) {
    ostringstream os;
    os << boolalpha << "GCheckBox.setSelected(\"" << gobj << "\", "
       << state << ")";
    putPipe(os.str());
}

void Platform::gslider_constructor(GObject* gobj, int min, int max, int value) {
    ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GSlider.create(\"" << gobj << "\", " << min << ", " << max
       << ", " << value << ")";
    putPipe(os.str());
}

int Platform::gslider_getMajorTickSpacing(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getMajorTickSpacing(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

int Platform::gslider_getMinorTickSpacing(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getMinorTickSpacing(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

bool Platform::gslider_getPaintLabels(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getPaintLabels(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

bool Platform::gslider_getPaintTicks(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getPaintTicks(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

bool Platform::gslider_getSnapToTicks(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getSnapToTicks(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

int Platform::gslider_getValue(const GObject* gobj) {
    ostringstream os;
    os << "GSlider.getValue(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

void Platform::gslider_setMajorTickSpacing(GObject* gobj, int value) {
    ostringstream os;
    os << "GSlider.setMajorTickSpacing(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setMinorTickSpacing(GObject* gobj, int value) {
    ostringstream os;
    os << "GSlider.setMinorTickSpacing(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setPaintLabels(GObject* gobj, bool value) {
    ostringstream os;
    os << "GSlider.setPaintLabels(\"" << gobj << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setPaintTicks(GObject* gobj, bool value) {
    ostringstream os;
    os << "GSlider.setPaintTicks(\"" << gobj << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setSnapToTicks(GObject* gobj, bool value) {
    ostringstream os;
    os << "GSlider.setSnapToTicks(\"" << gobj << "\", " << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setValue(GObject* gobj, int value) {
    ostringstream os;
    os << "GSlider.setValue(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gtextfield_constructor(GObject* gobj, int nChars) {
    ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GTextField.create(\"" << gobj << "\", " << nChars << ")";
    putPipe(os.str());
}

string Platform::gtextfield_getText(GObject* gobj) {
    ostringstream os;
    os << "GTextField.getText(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult();
}

bool Platform::gtextfield_isEditable(const GObject* gobj) {
    ostringstream os;
    os << "GTextField.isEditable(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

void Platform::gtextfield_setEditable(GObject* gobj, bool value) {
    ostringstream os;
    os << "GTextField.setEditable(\"" << gobj << "\", "
       << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gtextfield_setText(GObject* gobj, string str) {
    ostringstream os;
    os << "GTextField.setText(\"" << gobj << "\", ";
    writeQuotedString(os, str);
    os << ")";
    putPipe(os.str());
}

void Platform::gchooser_constructor(GObject* gobj) {
    ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GChooser.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gchooser_addItem(GObject* gobj, string item) {
    ostringstream os;
    os << "GChooser.addItem(\"" << gobj << "\", ";
    writeQuotedString(os, item);
    os << ")";
    putPipe(os.str());
}

string Platform::gchooser_getSelectedItem(GObject* gobj) {
    ostringstream os;
    os << "GChooser.getSelectedItem(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult();
}

void Platform::gchooser_setSelectedItem(GObject* gobj, string item) {
    ostringstream os;
    os << "GChooser.setSelectedItem(\"" << gobj << "\", ";
    writeQuotedString(os, item);
    os << ")";
    putPipe(os.str());
}

void Platform::gcompound_constructor(GObject* gobj) {
    ostringstream os;
    os << "GCompound.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::glabel_setFont(GObject* gobj, string font) {
    ostringstream os;
    os << "GLabel.setFont(\"" << gobj << "\", \"" << font << "\")";
    putPipe(os.str());
}

void Platform::glabel_setLabel(GObject* gobj, string str) {
    ostringstream os;
    os << "GLabel.setLabel(\"" << gobj << "\", ";
    writeQuotedString(os, str);
    os << ")";
    putPipe(os.str());
}

double Platform::glabel_getFontAscent(const GObject* gobj) {
    ostringstream os;
    os << "GLabel.getFontAscent(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToReal(getResult());
}

double Platform::glabel_getFontDescent(const GObject* gobj) {
    ostringstream os;
    os << "GLabel.getFontDescent(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToReal(getResult());
}

GDimension Platform::glabel_getSize(const GObject* gobj) {
    ostringstream os;
    os << "GLabel.getGLabelSize(\"" << gobj << "\")";
    putPipe(os.str());
    return scanDimension(getResult());
}

GEvent Platform::gevent_getNextEvent(int mask) {
    if (eventQueue.isEmpty()) {
        putPipe("GEvent.getNextEvent(" + integerToString(mask) + ")");
        getResult();
        if (eventQueue.isEmpty()) return GEvent();
    }
    return eventQueue.dequeue();
}

GEvent Platform::gevent_waitForEvent(int mask) {
    while (eventQueue.isEmpty()) {
        putPipe("GEvent.waitForEvent(" + integerToString(mask) + ")");

        // BUGBUG: Marty changing to consume ACKs because it was skipping an
        // event on mouse click before
        getResult();
    }

    GEvent event = eventQueue.dequeue();
    if (PIPE_DEBUG) {
        fprintf(stderr, "Platform::waitForEvent returning event \"%s\"\n", event.toString().c_str());  fflush(stderr);
    }
    return event;
}

bool Platform::jbeconsole_isBlocked() {
    return cinout_new_buf && cinout_new_buf->isBlocked();
}

void Platform::gwindow_exitGraphics(bool abortBlockedConsoleIO) {
    if (abortBlockedConsoleIO && jbeconsole_isBlocked()) {
        // graphical console is blocked waiting for an I/O read;
        // won't be able to exit graphics in the JBE anyway; just exit
        exit(0);
    } else {
        putPipe("GWindow.exitGraphics()");
        exit(0);
    }
}

string Platform::gfilechooser_showOpenDialog(string currentDir) {
    ostringstream os;
    os << "GFileChooser.showOpenDialog(";
    writeQuotedString(os, currentDir);
    os << ")";
    putPipe(os.str());
    return getResult();
}

string Platform::gfilechooser_showSaveDialog(string currentDir) {
    ostringstream os;
    os << "GFileChooser.showSaveDialog(";
    writeQuotedString(os, currentDir);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::goptionpane_showMessageDialog(string message) {
    ostringstream os;
    os << "GOptionPane.showMessageDialog(";
    writeQuotedString(os, message);
    os << ")";
    putPipe(os.str());
}

int Platform::goptionpane_showConfirmDialog(string message) {
    ostringstream os;
    os << "GOptionPane.showConfirmDialog(";
    writeQuotedString(os, message);
    os << ")";
    return stringToInteger(getResult());
}

string Platform::goptionpane_showInputDialog(string message) {
    ostringstream os;
    os << "GOptionPane.showInputDialog(";
    writeQuotedString(os, message);
    os << ")";
    return getResult();
}

string Platform::goptionpane_showOptionDialog(const vector<string>& options, string initiallySelected) {
    ostringstream os;
    os << "GOptionPane.showOptionDialog({";
    if (!options.empty()) {
        writeQuotedString(os, options[0]);
        for (int i = 1, sz = options.size(); i < sz; ++i) {
            os << ", ";
            writeQuotedString(os, options[i]);
        }
    }
    os << "}, ";
    writeQuotedString(os, initiallySelected);
    os << ")";
    return getResult();
}

Platform *getPlatform() {
    static Platform gp;
    return &gp;
}

#ifdef _WIN32

/* Windows implementation of interface to Java back end */

// formats an error message using Windows lookup of error codes and strings
static WINBOOL WinCheck(WINBOOL result) {
    if (result == 0 && result != ERROR_IO_PENDING) {
        // failure; Windows error codes: http://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx
        DWORD lastErrorCode = GetLastError();
        char* errorMsg = NULL;
        // Ask Windows to prepare a standard message for a GetLastError() code:
        FormatMessageA(/* dwFlags */ FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       /* lpSource */ NULL,
                       /* dwMessageId */ lastErrorCode,
                       /* dwLanguageId */ LANG_NEUTRAL, // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       /* lpBuffer */ (LPSTR)&errorMsg,
                       /* dwSize */ 0,
                       /* arguments */ NULL);
        if (errorMsg) {
            cerr << "Error from Java back-end subprocess: " << errorMsg << endl;
        }
    }
    return result;
}

int startupMain(int argc, char **argv) {
    extern int Main(int argc, char **argv);
    startupMainDontRunMain(argc, argv);

#ifndef SPL_AUTOGRADER_MODE
    return Main(argc, argv);
#else
    return 0;
#endif // SPL_AUTOGRADER_MODE
}

/*
 * This is a version of startupMain that does all of the setup but then does
 * not actually run startupMain.
 * This is used to facilitate the creation of autograder programs.
 */
void startupMainDontRunMain(int /*argc*/, char** argv) {
    string arg0 = argv[0];
    programName = getRoot(getTail(arg0));
    initPipe();
    cin_old_buf = cin.rdbuf();
    cout_old_buf = cout.rdbuf();
    cinout_new_buf = new ConsoleStreambuf();
    cin.rdbuf(cinout_new_buf);
    cout.rdbuf(cinout_new_buf);
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    getPlatform()->cpplib_setCppLibraryVersion();
    setConsoleProperties();
}

static void initPipe() {
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = true;
    attr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&rdFromJBE, &wrFromJBE, &attr, 0)) {
        error("Can't create fromJBE");
    }
    if (!SetHandleInformation(rdFromJBE, HANDLE_FLAG_INHERIT, 0)) {
        error("SetHandleInformation failed for fromJBE");
    }
    if (!CreatePipe(&rdToJBE, &wrToJBE, &attr, 0)) {
        error("Can't create toJBE");
    }
    if (!SetHandleInformation(wrToJBE, HANDLE_FLAG_INHERIT, 0)) {
        error("SetHandleInformation failed for toJBE");
    }
    string cmd = "java";
    if (PIPE_DEBUG) {
        cmd += " -Dstanfordspl.debug=true";
    }
    cmd += " -jar spl.jar";
    cmd += string(" ") + programName;
    int n = cmd.length();
    char *cmdLine = new char[n + 1];
    for (int i = 0; i < n; i++) {
        cmdLine[i] = cmd[i];
    }
    cmdLine[n] = '\0';
    PROCESS_INFORMATION pInfo;
    memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));
    STARTUPINFOA sInfo;
    memset(&sInfo, 0, sizeof(STARTUPINFOA));
    sInfo.cb = sizeof(STARTUPINFOA);
    sInfo.dwFlags = STARTF_USESTDHANDLES;
    sInfo.hStdInput = rdToJBE;
    sInfo.hStdOutput = wrFromJBE;
    sInfo.hStdError = wrFromJBE;
    int ok = CreateProcessA(NULL, cmdLine, NULL, NULL, true, CREATE_NO_WINDOW,
                            NULL, NULL, &sInfo, &pInfo);
    if (!ok) {
        DWORD err = GetLastError();
        cerr << endl;
        cerr << "ERROR: Stanford C++ library was unable to connect" << endl;
        cerr << "       to its Java back-end to launch 'spl.jar'." << endl;
        cerr << "       Please check your Java installation and make sure" << endl;
        cerr << "       that spl.jar is properly attached to your project." << endl;
        cerr << endl;
        cerr << err << endl;
    } else {
        CloseHandle(pInfo.hProcess);
        CloseHandle(pInfo.hThread);
    }
}

static void putPipe(string line) {
    DWORD nch;
    if (PIPE_DEBUG) {
        fprintf(stderr, "putPipe(\"%s\")\n", line.c_str());  fflush(stderr);
    }
    if (!WinCheck(WriteFile(wrToJBE, line.c_str(), line.length(), &nch, NULL))) return;
    if (!WinCheck(WriteFile(wrToJBE, "\n", 1, &nch, NULL))) return;
    WinCheck(FlushFileBuffers(wrToJBE));
}

static string getPipe() {
    string line = "";
    DWORD nch;
    if (PIPE_DEBUG) {
        fprintf(stderr, "getPipe(): waiting ...\n");  fflush(stderr);
    }

    int charsRead = 0;
    int charsReadMax = 1024*1024;
    while (charsRead < charsReadMax) {
        char ch;
        WINBOOL readFileResult = WinCheck(ReadFile(rdFromJBE, &ch, 1, &nch, NULL));
        if (readFileResult == 0) {
            break;   // failed to read from subprocess
        }
        if (ch == '\n' || ch == '\r') {
            break;
        }
        line += ch;
        charsRead++;
    }

    if (PIPE_DEBUG) {
        fprintf(stderr, "getPipe(): returned \"%s\"\n", line.c_str());  fflush(stderr);
    }
    return line;
}

#else

/* Linux/Mac implementation of interface to Java back end */

static bool LinCheck(ssize_t result) {
    if (result == EPIPE) {
        // cerr << "Error from Java back-end subprocess." << endl;
        // throw InterruptedIOException();
        return false;
    } else {
        return true;
    }
}

static void scanOptions() {
    char *home = getenv("HOME");
    if (home != NULL) {
        string filename = string() + home + "/.spl";
        ifstream infile(filename.c_str());
        if (!infile.fail()) {
            string line;
            while (getline(infile, line)) {
                size_t equals = line.find('=');
                if (equals != string::npos) {
                    string key = line.substr(0, equals);
                    string value = line.substr(equals + 1);
                    optionTable.put(key, value);
                }
            }
            infile.close();
        }
    }
}

static string getOption(string key) {
    char *str = getenv(key.c_str());
    if (str != NULL) return string(str);
    return optionTable.get(key);
}

#ifdef SPL_AUTOGRADER_MODE
int startupMain(int /*argc*/, char** argv) {
#else
int startupMain(int argc, char **argv) {
    extern int Main(int argc, char **argv);
#endif
    string arg0 = argv[0];
    programName = getRoot(getTail(arg0));
    size_t ax = arg0.find(".app/Contents/");
    if (ax != string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
    char *noConsoleFlag = getenv("NOCONSOLE");
    if (noConsoleFlag != NULL && startsWith(string(noConsoleFlag), "t")) {
#ifdef SPL_AUTOGRADER_MODE
        return 0;
#else
        return Main(argc, argv);
#endif
    }
    scanOptions();
    initPipe();
    cin_old_buf = cin.rdbuf();
    cout_old_buf = cout.rdbuf();
    cinout_new_buf = new ConsoleStreambuf();
    cin.rdbuf(cinout_new_buf);
    cout.rdbuf(cinout_new_buf);
    string font = getOption("CPPFONT");
    if (font != "") setConsoleFont(font);
    getPlatform()->cpplib_setCppLibraryVersion();
    setConsoleProperties();

#ifndef SPL_AUTOGRADER_MODE
    return Main(argc, argv);
#else
    return 0;
#endif
}

/*
 * This is a version of startupMain that does all of the setup but then does
 * not actually run startupMain.
 * This is used to facilitate the creation of autograder programs.
 */
void startupMainDontRunMain(int /*argc*/, char** argv) {
    string arg0 = argv[0];
    programName = getRoot(getTail(arg0));
    size_t ax = arg0.find(".app/Contents/");
    if (ax != string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
    scanOptions();
    initPipe();
    cin_old_buf = cin.rdbuf();
    cout_old_buf = cout.rdbuf();
    cinout_new_buf = new ConsoleStreambuf();
    cin.rdbuf(cinout_new_buf);
    cout.rdbuf(cinout_new_buf);
    string font = getOption("CPPFONT");
    if (font != "") setConsoleFont(font);
    getPlatform()->cpplib_setCppLibraryVersion();
    setConsoleProperties();
}

static void sigPipeHandler(int /*signum*/) {
    cerr << "***" << endl;
    cerr << "*** STANFORD C++ LIBRARY" << endl;
    cerr << "*** Prematurely exiting program because console window was closed." << endl;
    cerr << "***" << endl;
    exit(1);
}

static void initPipe() {
    char *trace = getenv("JBETRACE");
    logfile.open("/dev/tty");
    tracePipe = trace != NULL && startsWith(toLowerCase(trace), "t");
    int toJBE[2], fromJBE[2];
    pipe(toJBE);
    pipe(fromJBE);
    int child = fork();
    if (child == 0) {
        dup2(toJBE[0], 0);
        close(toJBE[0]);
        close(toJBE[1]);
        dup2(fromJBE[1], 1);
        close(fromJBE[0]);
        close(fromJBE[1]);
#ifdef __APPLE__
        string option = "-Xdock:name=" + programName;
        execlp("java", "java", option.c_str(), "-jar", "spl.jar",
               programName.c_str(), NULL);
#else
        execlp("java", "java", "-jar", "spl.jar", programName.c_str(), NULL);
#endif
        cerr << endl;
        cerr << "ERROR: Stanford C++ library was unable to connect" << endl;
        cerr << "       to its Java back-end to launch 'spl.jar'." << endl;
        cerr << "       Please check your Java installation and make sure" << endl;
        cerr << "       that spl.jar is properly attached to your project." << endl;
        cerr << endl;
        throw new ErrorException("Could not exec spl.jar");
    } else {
        pin = fromJBE[0];
        pout = toJBE[1];
        close(fromJBE[1]);
        close(toJBE[0]);
        
        // stop the pipe from generating a SIGPIPE when JBE is closed
        signal(SIGPIPE, sigPipeHandler);
    }
}

static void putPipe(string line) {
    if (PIPE_DEBUG) {
        fprintf(stderr, "putPipe(\"%s\")\n", line.c_str());  fflush(stderr);
    }
    LinCheck(write(pout, line.c_str(), line.length()));
    LinCheck(write(pout, "\n", 1));
    if (tracePipe) logfile << "-> " << line << endl;
}

static string getPipe() {
    if (PIPE_DEBUG) {
        fprintf(stderr, "getPipe(): waiting ...\n");  fflush(stderr);
    }
    string line = "";
    int charsRead = 0;
    int charsReadMax = 1024*1024;
    while (charsRead < charsReadMax) {
        char ch;
        ssize_t result = read(pin, &ch, 1);
        if (result <= 0) {
            throw InterruptedIOException();
            // break;   // failed to read from subprocess
        }
        if (ch == '\n') break;
        line += ch;
        charsRead++;
    }
    if (PIPE_DEBUG) {
        fprintf(stderr, "getPipe(): \"%s\"\n", line.c_str());  fflush(stderr);
    }
    if (tracePipe) logfile << "<- " << line << endl;
    return line;
}

#endif

static string getResult(bool consumeAcks) {
    while (true) {
        if (PIPE_DEBUG) {
            fprintf(stderr, "getResult(): calling getPipe() ...\n");  fflush(stderr);
        }
        string line = getPipe();
        bool isResult = startsWith(line, "result:");
        bool isEvent  = startsWith(line, "event:");
        bool isAck    = startsWith(line, "result:___jbe___ack___");
        bool hasACMException = line.find("acm.util.ErrorException") != string::npos;
        bool hasException    = line.find("xception") != string::npos;
        bool hasError        = line.find("Unexpected error") != string::npos;

        // added by Marty: if there is a back-end error, display it
        if (((isResult || isEvent) && hasACMException) ||
                (!isResult && !isEvent && (hasException || hasError))) {
            ostringstream out;
            if (startsWith(line, "result:")) {
                line = line.substr(7);
            } else if (startsWith(line, "event:")) {
                line = line.substr(6);
            }
            out << "ERROR emitted from Stanford Java back-end process:"
                << endl << line;
            error(out.str());
        } else if (isResult) {
            if (!(consumeAcks && isAck)) {
                // this is just an acknowledgment of some previous event;
                // not a real result of its own
                return line.substr(7);
            }
        } else if (isEvent) {
            eventQueue.enqueue(parseEvent(line.substr(6)));
        } else {
            if (line.find("\tat ") != string::npos || line.find("   at ") != string::npos) {
                // part of a Java exception stack trace, so echo it
                cerr << line << endl;
            }
        }
    }
}

static void getStatus() {
    string result = getResult();
    if (result != "ok") {
        error(result);
    }
}

static GEvent parseEvent(string line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    string name = scanner.nextToken();
    if (name == "mousePressed") {
        return parseMouseEvent(scanner, MOUSE_PRESSED);
    } else if (name == "mouseReleased") {
        return parseMouseEvent(scanner, MOUSE_RELEASED);
    } else if (name == "mouseClicked") {
        return parseMouseEvent(scanner, MOUSE_CLICKED);
    } else if (name == "mouseMoved") {
        return parseMouseEvent(scanner, MOUSE_MOVED);
    } else if (name == "mouseDragged") {
        return parseMouseEvent(scanner, MOUSE_DRAGGED);
    } else if (name == "keyPressed") {
        return parseKeyEvent(scanner, KEY_PRESSED);
    } else if (name == "keyReleased") {
        return parseKeyEvent(scanner, KEY_RELEASED);
    } else if (name == "keyTyped") {
        return parseKeyEvent(scanner, KEY_TYPED);
    } else if (name == "actionPerformed") {
        return parseActionEvent(scanner, ACTION_PERFORMED);
    } else if (name == "timerTicked") {
        return parseTimerEvent(scanner, TIMER_TICKED);
    } else if (name == "windowClosed") {
        // BUGBUG: GWindow objects were not maintaining proper state on close
        //         and were doing a circular ring of close() messages to/from JBE
        GWindowEvent e = parseWindowEvent(scanner, WINDOW_CLOSED);
        e.getGWindow().setVisible(false);
        e.getGWindow().notifyOfClose();
        windowTable.remove(e.getGWindow().getWindowData());
        return e;
    } else if (name == "windowResized") {
        return parseWindowEvent(scanner, WINDOW_RESIZED);
    } else if (name == "consoleWindowClosed") {
        // Java console window was closed; possibly exit the C++ program now
        extern bool getConsoleExitProgramOnClose();
        extern bool getConsoleEventOnClose();
        if (getConsoleExitProgramOnClose()) {
            cerr << endl;
            cerr << "***" << endl;
            cerr << "*** STANFORD C++ LIBRARY" << endl;
            cerr << "*** Prematurely exiting program because console window was closed." << endl;
            cerr << "***" << endl;
            cerr << endl;

            // if waiting for keyboard input, abort it
            if (cinout_new_buf && cinout_new_buf->isBlocked()) {
                // abortAllConsoleIO();
                exit(0);
            }

            // close any other graphical windows and exit program
            exitGraphics();
        } else if (getConsoleEventOnClose()) {
            GWindowData* gwd = NULL;
            GWindowEvent e(CONSOLE_CLOSED, GWindow(gwd));
            return e;
        }
    } else if (name == "lastWindowClosed") {
        exit(0);
    } else if (name == "lastWindowGWindow_closed") {
        exit(0);
    } else {
        /* Ignore for now */
    }
    return GEvent();
}

static GEvent parseMouseEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(",");
    int modifiers = scanInt(scanner);
    scanner.verifyToken(",");
    double x = scanDouble(scanner);
    scanner.verifyToken(",");
    double y = scanDouble(scanner);
    scanner.verifyToken(")");
    GMouseEvent e(type, GWindow(windowTable.get(id)), x, y);
    e.setEventTime(time);
    e.setModifiers(modifiers);
    return e;
}

static GEvent parseKeyEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(",");
    int modifiers = scanInt(scanner);
    scanner.verifyToken(",");
    int keyChar = scanInt(scanner);
    scanner.verifyToken(",");
    int keyCode = scanInt(scanner);
    scanner.verifyToken(")");
    GKeyEvent e(type, GWindow(windowTable.get(id)), char(keyChar), keyCode);
    e.setEventTime(time);
    e.setModifiers(modifiers);
    return e;
}

static GEvent parseTimerEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GTimerEvent e(type, GTimer(timerTable.get(id)));
    e.setEventTime(time);
    return e;
}

static GEvent parseWindowEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GWindowEvent e(type, GWindow(windowTable.get(id)));
    e.setEventTime(time);
    return e;
}

static GEvent parseActionEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    string action = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GActionEvent e(type, sourceTable.get(id), action);
    e.setEventTime(time);
    return e;
}

/* Console code */

void Platform::cpplib_setCppLibraryVersion() {
    ostringstream out;
    out << "StanfordCppLib.setCppVersion(";
    writeQuotedString(out, STANFORD_CPP_LIB_VERSION);
    out << ")";
    putPipe(out.str());
}

string Platform::cpplib_getCppLibraryVersion() {
    return STANFORD_CPP_LIB_VERSION;
}

string Platform::cpplib_getJavaBackEndVersion() {
    putPipe("StanfordCppLib.getJbeVersion()");
    string result = getResult();
    return result;
}

void Platform::jbeconsole_clear() {
    putPipe("JBEConsole.clear()");
}

void Platform::jbeconsole_setFont(const string & font) {
    ostringstream os;
    os << "JBEConsole.setFont(\"" << font << "\")";
    putPipe(os.str());
}

void Platform::jbeconsole_setSize(double width, double height) {
    ostringstream os;
    os << "JBEConsole.setSize(" << width << ", " << height << ")";
    putPipe(os.str());
}

/*
 * pass (-1, -1) to center
 */
void Platform::jbeconsole_setLocation(int x, int y) {
    ostringstream os;
    os << "JBEConsole.setLocation(" << x << ", " << y << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setExitProgramOnClose(bool value) {
    ostringstream os;
    os << "JBEConsole.setExitOnClose(" << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setLocationSaved(bool value) {
    ostringstream os;
    os << "JBEConsole.setLocationSaved(" << boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::autograderinput_addButton(string text, string input) {
    if (input.empty()) {
        input = text;
    }
    ostringstream out;
    out << "AutograderInput.addButton(";
    writeQuotedString(out, text);
    out << ", ";
    writeQuotedString(out, input);
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_removeButton(string text) {
    ostringstream out;
    out << "AutograderInput.removeButton(";
    writeQuotedString(out, text);
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_addCategory(string name) {
    ostringstream out;
    out << "AutograderInput.addCategory(";
    writeQuotedString(out, name);
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_removeCategory(string name) {
    ostringstream out;
    out << "AutograderInput.removeCategory(";
    writeQuotedString(out, name);
    out << ")";
    putPipe(out.str());
}

static string getLineConsole() {
    putPipe("JBEConsole.getLine()");
    string result = getResult(true);
    echoConsole(result + "\n");   // wrong for multiple inputs on one line
    return result;
}

static void putConsole(const string& str) {
    ostringstream os;
    os << "JBEConsole.print(";

    // BUGBUG: strings that end with \\ don't print because of back-end error;
    //         kludge fix by appending an invisible space after it
    if (!str.empty() && str[str.length() - 1] == '\\') {
        string str2 = str + ' ';
        writeQuotedString(os, str2);
    } else {
        writeQuotedString(os, str);
    }
    
    os << ")";
    putPipe(os.str());
    echoConsole(str);
}

static void echoConsole(const string& str) {
    if (getConsoleEcho()) {
        // write to the standard cout console for output copy/pasting
        cin.rdbuf(cin_old_buf);
        cout.rdbuf(cout_old_buf);
        cout << str;
        cout.flush();
        cin.rdbuf(cinout_new_buf);
        cout.rdbuf(cinout_new_buf);
    }
    fileLogConsole(str);
}

static void fileLogConsole(const string& str) {
    string consoleLogFile = autograder::getConsoleLogFile();
    if (consoleLogFile.length() > 0) {
        // A bit inefficient; opens/writes/closes the file on each print
        // statement.  But this enables fine-grained control and changing
        // the log file in mid-execution with minimal code base change
        ofstream outfile;
        outfile.open(consoleLogFile.c_str(), fstream::out | fstream::app);
        outfile << str;
        outfile.flush();
        outfile.close();
    }
}

static void endLineConsole() {
    putPipe("JBEConsole.println()");
    echoConsole("\n");
}

static int scanInt(TokenScanner& scanner) {
    string token = scanner.nextToken();
    if (token == "-") token += scanner.nextToken();
    return stringToInteger(token);
}

static double scanDouble(TokenScanner& scanner) {
    string token = scanner.nextToken();
    if (token == "-") token += scanner.nextToken();
    return stringToReal(token);
}

static GDimension scanDimension(const string& str) {
    TokenScanner scanner(str);
    scanner.scanNumbers();
    scanner.ignoreWhitespace();
    scanner.verifyToken("GDimension");
    scanner.verifyToken("(");
    double width = scanDouble(scanner);
    scanner.verifyToken(",");
    double height = scanDouble(scanner);
    scanner.verifyToken(")");
    return GDimension(width, height);
}

static Point scanPoint(const string& str) {
    TokenScanner scanner(str);
    scanner.scanNumbers();
    scanner.ignoreWhitespace();
    scanner.verifyToken("Point");
    scanner.verifyToken("(");
    int x = scanInt(scanner);
    scanner.verifyToken(",");
    int y = scanInt(scanner);
    scanner.verifyToken(")");
    return Point(x, y);
}

static GRectangle scanRectangle(const string& str) {
    TokenScanner scanner(str);
    scanner.scanNumbers();
    scanner.ignoreWhitespace();
    scanner.verifyToken("GRectangle");
    scanner.verifyToken("(");
    double x = scanDouble(scanner);
    scanner.verifyToken(",");
    double y = scanDouble(scanner);
    scanner.verifyToken(",");
    double width = scanDouble(scanner);
    scanner.verifyToken(",");
    double height = scanDouble(scanner);
    scanner.verifyToken(")");
    return GRectangle(x, y, width, height);
}

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * based on compiler options set in the .pro file.
 */
static void setConsoleProperties() {
#if defined(SPL_CONSOLE_FONTSIZE)
    string fontStr = string("Monospaced-Bold-") + integerToString(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

#if defined(SPL_CONSOLE_WIDTH) && defined(SPL_CONSOLE_HEIGHT)
    setConsoleSize(SPL_CONSOLE_WIDTH, SPL_CONSOLE_HEIGHT);
#endif

#if defined(SPL_CONSOLE_X) && defined(SPL_CONSOLE_Y)
    setConsoleLocation(SPL_CONSOLE_X, SPL_CONSOLE_Y);
#endif

#if defined(SPL_CONSOLE_ECHO)
    setConsoleEcho(true);
#endif

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif
    
#if defined(SPL_CONSOLE_EXIT_ON_CLOSE)
    setConsoleExitProgramOnClose(true);
#endif
    
#if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
#endif
}

//static void abortAllConsoleIO() {
//#ifdef _WIN32
//    CancelIo(wrToJBE);
//    CancelIo(rdFromJBE);
//#endif
//    throw InterruptedIOException();
//}
