/*
 * File: platform.cpp
 * ------------------
 * This file implements the platform interface by passing commands to
 * a Java back end that manages the display.
 * 
 * @version 2016/03/16
 * - added functions for HTTP server
 * @version 2015/10/21
 * - moved EchoingStreambuf to plainconsole.h/cpp facilitate in/output capture
 * @version 2015/10/08
 * - bug fixes for sending long strings through Java process pipe
 * @version 2015/10/01
 * - fix to check JAVA_HOME environment variable to force Java executable path
 *   (improved compatibility on Windows systems with many JDKs/JREs installed)
 * @version 2015/08/05
 * - added output limit for EchoingStreamBuf to facilitate trimming of infinite
 *   output by runaway programs / infinite loops
 * @version 2015/08/01
 * - added flag for 'headless' mode for use in non-GUI server environments
 * @version 2015/07/05
 * - added EchoingStreamBuf class for use in unified single-file version of library
 * @version 2014/11/25
 * - added methods for checking and unchecking individual autograder test cases
 * @version 2014/11/20
 * - added gwindow clearCanvas method
 * @version 2014/11/15
 * - improvements to autograder unit test GUI
 * @version 2014/11/14
 * - added method to set unit test runtime in MS
 * @version 2014/11/05
 * - added ability to see cerr messages in red on graphical console window
 * @version 2014/10/31
 * - added functions for graphical style checker
 * - added functions to get/check versions of cpp/Java/project
 * @version 2014/10/27
 * - added functions for graphical autograder unit tests
 * - added setVisible for autograder input panel
 * @version 2014/10/14
 * - removed unused fileLogConsole functionality
 * - added code to verify backend spl.jar version compatibility
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - fixed bug in GBufferedImage.fillRegion back-end call (missing ", " token)
 * - bug fix for filelib openFileDialog call
 * - implemented backend of filelib getTempDirectory
 * - implemented backend of GBufferedImage resize
 * - fixed/completed backend of GOptionPane functions
 * - implemented backend of URL download for iurlstream
 */

#ifdef _WIN32
#  include <windows.h>
#  include <tchar.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#else // _WIN32
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
#endif // _WIN32

#include "platform.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <signal.h>
#include <sstream>
#include <string>
#include <vector>
#include "private/version.h"
#include "error.h"
#include "exceptions.h"
#include "filelib.h"
#include "gevents.h"
#include "gtimer.h"
#include "gtypes.h"
#include "hashmap.h"
#include "plainconsole.h"
#include "queue.h"
#include "stack.h"
#include "strlib.h"
#include "tokenscanner.h"
#include "vector.h"

// internal flag to emit a dump of every message sent to the Java back-end;
// used for debugging purposes
// #define PIPE_DEBUG true

// related: similar constant in Java back-end stanford.spl.SplPipeDecoder.java
static const size_t PIPE_MAX_COMMAND_LENGTH = 2048;

static std::string getLineConsole();
static void putConsole(const std::string& str, bool isStderr = false);
static void endLineConsole(bool isStderr = false);
static void echoConsole(const std::string& str, bool isStderr = false);
static int scanInt(TokenScanner& scanner);
static double scanDouble(TokenScanner& scanner);
static int scanChar(TokenScanner& scanner);
static GDimension scanDimension(const std::string& str);
static Point scanPoint(const std::string& str);
static GRectangle scanRectangle(const std::string& str);
static void setConsoleProperties();
void startupMainDontRunMain(int argc, char** argv);
//static void abortAllConsoleIO();

class ConsoleStreambuf : public std::streambuf {
private:
    /* Constants */
    static const int BUFFER_SIZE = 4096;

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
        std::string line = getLineConsole();
        blocked--;
        int n = line.length();
        if (n + 1 >= BUFFER_SIZE) {
            error("ConsoleStreambuf::underflow: String too long");
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
        return overflow(ch, false);
    }
        
    virtual int overflow(int ch, bool isStderr) {
        std::string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                putConsole(line, isStderr);
                endLineConsole(isStderr);
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            putConsole(line, isStderr);
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
    
    virtual int sync(bool isStderr) {
        return overflow(EOF, isStderr);
    }
};

/*
 * A stream buffer that just "wraps" another stream buffer.
 * This is used here to distinguish cout (black text) from cerr (red text).
 */
class ForwardingStreambuf : public std::streambuf {
private:
    ConsoleStreambuf& delegate;
    bool isStderr;
    
public:
    ForwardingStreambuf(ConsoleStreambuf& del, bool err = false)
            : delegate(del), isStderr(err) {
        // empty
    }
    
    virtual int underflow() {
        return delegate.underflow();
    }
    
    virtual int overflow(int ch = EOF) {
        return delegate.overflow(ch, isStderr);
    }
    
    virtual int sync() {
        return delegate.sync(isStderr);
    }
    
    // functions below are overridden for completeness,
    // but all just delegate to underlying ConsoleStreambuf
    std::streamsize in_avail() {
        return delegate.in_avail();
    }
    
    int snextc() {
        return delegate.snextc();
    }
    
    int sbumpc() {
        return delegate.sbumpc();
    }
    
    int sgetc() {
        return delegate.sgetc();
    }
    
    std::streamsize sgetn(char* s, std::streamsize n) {
        return delegate.sgetn(s, n);
    }
    
    int sputbackc(char c) {
        return delegate.sputbackc(c);
    }
    
    int sungetc() {
        return delegate.sungetc();
    }
    
    int sputc(char c) {
        return delegate.sputc(c);
    }
    
    std::streamsize sputn(const char* s, std::streamsize n) {
        return delegate.sputn(s, n);
    }
};

/* Private data */

static Queue<GEvent> eventQueue;
static HashMap<std::string, GTimerData*> timerTable;
static HashMap<std::string, GWindowData*> windowTable;
static HashMap<std::string, GObject*> sourceTable;
static HashMap<std::string, std::string> optionTable;
static std::string programName;
static std::ofstream logfile;
static ConsoleStreambuf* cinout_new_buf = NULL;

#ifdef _WIN32
static HANDLE rdFromJBE = NULL;
static HANDLE wrFromJBE = NULL;
static HANDLE rdToJBE = NULL;
static HANDLE wrToJBE = NULL;
#else // _WIN32
static pid_t cppLibPid;
static pid_t javaBackEndPid;
#endif // _WIN32

/* Prototypes */

static void initPipe();
static void putPipe(std::string line);
static void putPipeLongString(std::string line);
static std::string getJavaCommand();
static std::string getPipe();
static std::string getResult(bool consumeAcks = false, const std::string& caller = "");
static void getStatus();
static GEvent parseEvent(std::string line);
static GEvent parseMouseEvent(TokenScanner& scanner, EventType type);
static GEvent parseKeyEvent(TokenScanner& scanner, EventType type);
static GEvent parseServerEvent(TokenScanner& scanner, EventType type);
static GEvent parseTableEvent(TokenScanner& scanner, EventType type);
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

// Unix implementation; see Windows implementation elsewhere in this file
bool Platform::filelib_fileExists(std::string filename) {
    struct stat fileInfo;
    return stat(filename.c_str(), &fileInfo) == 0;
}

// Unix implementation; see Windows implementation elsewhere in this file
bool Platform::filelib_isFile(std::string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISREG(fileInfo.st_mode) != 0;
}

// Unix implementation; see Windows implementation elsewhere in this file
bool Platform::filelib_isSymbolicLink(std::string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISLNK(fileInfo.st_mode) != 0;
}

// Unix implementation; see Windows implementation elsewhere in this file
bool Platform::filelib_isDirectory(std::string filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) return false;
    return S_ISDIR(fileInfo.st_mode) != 0;
}

// Unix implementation; see Windows implementation elsewhere in this file
void Platform::filelib_setCurrentDirectory(std::string path) {
    if (chdir(path.c_str()) == 0) {
        std::string msg = "setCurrentDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
std::string Platform::filelib_getCurrentDirectory() {
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
        std::string msg = "getCurrentDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
        return "";
    } else {
        std::string result = std::string(cwd);
        free(cwd);
        return result;
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
// http://stackoverflow.com/questions/8087805/
// how-to-get-system-or-user-temp-folder-in-unix-and-windows
std::string Platform::filelib_getTempDirectory() {
    char* dir = getenv("TMPDIR");
    if (!dir) dir = getenv("TMP");
    if (!dir) dir = getenv("TEMP");
    if (!dir) dir = getenv("TEMPDIR");
    if (!dir) return "/tmp";
    return dir;
}

// Unix implementation; see Windows implementation elsewhere in this file
void Platform::filelib_createDirectory(std::string path) {
    if (endsWith(path, "/")) {
        path = path.substr(0, path.length() - 2);
    }
    if (mkdir(path.c_str(), 0777) != 0) {
        if (errno == EEXIST && filelib_isDirectory(path)) return;
        std::string msg = "createDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
std::string Platform::filelib_getDirectoryPathSeparator() {
    return "/";
}

// Unix implementation; see Windows implementation elsewhere in this file
std::string Platform::filelib_getSearchPathSeparator() {
    return ":";
}

// Unix implementation; see Windows implementation elsewhere in this file
std::string Platform::filelib_expandPathname(std::string filename) {
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
            if (homedir == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
            }
        } else {
            struct passwd *pw = getpwnam(filename.substr(1, spos - 1).c_str());
            if (pw == NULL) {
                error("expandPathname: No such user");
            } else {
                homedir = pw->pw_dir;
            }
        }
        filename = std::string(homedir) + filename.substr(spos);
        len = filename.length();
    }
    for (int i = 0; i < len; i++) {
        if (filename[i] == '\\') filename[i] = '/';
    }
    return filename;
}

// Unix implementation; see Windows implementation elsewhere in this file
void Platform::filelib_listDirectory(std::string path, std::vector<std::string>& list) {
    if (path == "") path = ".";
    DIR *dir = opendir(path.c_str());
    if (dir == NULL) error(std::string("listDirectory: Can't open ") + path);
    list.clear();
    while (true) {
        struct dirent *ep = readdir(dir);
        if (ep == NULL) break;
        std::string name = std::string(ep->d_name);
        if (name != "." && name != "..") list.push_back(name);
    }
    closedir(dir);
    sort(list.begin(), list.end());
}

#else // _WIN32

// Windows implementation; see Unix implementation elsewhere in this file
bool Platform::filelib_fileExists(std::string filename) {
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

// Windows implementation; see Unix implementation elsewhere in this file
bool Platform::filelib_isFile(std::string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_NORMAL);
}

// Windows implementation; see Unix implementation elsewhere in this file
bool Platform::filelib_isSymbolicLink(std::string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES
            && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

// Windows implementation; see Unix implementation elsewhere in this file
bool Platform::filelib_isDirectory(std::string filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

// Windows implementation; see Unix implementation elsewhere in this file
void Platform::filelib_setCurrentDirectory(std::string path) {
    if (!filelib_isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
        error("setCurrentDirectory: Can't change to " + path);
    }
}

// Windows implementation; see Unix implementation elsewhere in this file
std::string Platform::filelib_getCurrentDirectory() {
    char path[MAX_PATH + 1];
    int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
    return std::string(path, n);
}

// Windows implementation; see Unix implementation elsewhere in this file
std::string Platform::filelib_getTempDirectory() {
    char path[MAX_PATH + 1];
    int n = GetTempPathA(MAX_PATH + 1, path);
    return std::string(path, n);
}

// Windows implementation; see Unix implementation elsewhere in this file
void Platform::filelib_createDirectory(std::string path) {
    if (!CreateDirectoryA(path.c_str(), NULL)) {
        error("createDirectory: Can't create " + path);
    }
}

// Windows implementation; see Unix implementation elsewhere in this file
std::string Platform::filelib_getDirectoryPathSeparator() {
    return "\\";
}

// Windows implementation; see Unix implementation elsewhere in this file
std::string Platform::filelib_getSearchPathSeparator() {
    return ";";
}

// Windows implementation; see Unix implementation elsewhere in this file
std::string Platform::filelib_expandPathname(std::string filename) {
    if (filename == "") return "";
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        if (filename[i] == '/') filename[i] = '\\';
    }
    return filename;
}

// Windows implementation; see Unix implementation elsewhere in this file
void Platform::filelib_listDirectory(std::string path, std::vector<std::string> & list) {
    if (path == "") path = ".";
    std::string pattern = path + "\\*.*";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        error("listDirectory: Can't list directory");
    }
    list.clear();
    while (true) {
        std::string name = std::string(fd.cFileName);
        if (name != "." && name != "..") list.push_back(name);
        if (!FindNextFileA(h, &fd)) break;
    }
    FindClose(h);
    sort(list.begin(), list.end());
}

#endif // _WIN32

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
#else // not _WIN32
void Platform::setStackSize(unsigned int stackSize) {
    // Linux/Mac definition (they DO support stack size changing)
#if defined(__USE_LARGEFILE64)
    // 64-bit version
    rlimit64 rl;
    int result = getrlimit64(RLIMIT_STACK, &rl);
#else // not __USE_LARGEFILE64
    // 32-bit version
    rlimit rl;
    int result = getrlimit(RLIMIT_STACK, &rl);
#endif // __USE_LARGEFILE64
    if (result == 0) {
        if (rl.rlim_cur < stackSize || rl.rlim_max < stackSize) {
            rl.rlim_cur = (rlim_t) stackSize;
            rl.rlim_max = (rlim_t) stackSize;
#if defined(__USE_LARGEFILE64)
            // 64-bit version
            result = setrlimit64(RLIMIT_STACK, &rl);
#else // not __USE_LARGEFILE64
            result = setrlimit(RLIMIT_STACK, &rl);
#endif // __USE_LARGEFILE64
            if (result != 0) {
                std::cerr << std::endl;
                std::cerr << " ***" << std::endl;
                std::cerr << " *** STANFORD C++ LIBRARY" << std::endl;
                std::cerr << " *** Warning: Unable to increase system stack size" << std::endl;
                std::cerr << " ***" << std::endl;
                std::cerr << std::endl;
            }
        }
    }
#endif // _WIN32
}

std::string Platform::os_getLastError() {
#ifdef _WIN32
    // Windows error-reporting code
    DWORD lastErrorCode = ::GetLastError();
    char* errorMsg = NULL;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    ::FormatMessageA(
                   /* dwFlags */ FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   /* lpSource */ NULL,
                   /* dwMessageId */ lastErrorCode,
                   /* dwLanguageId */ LANG_NEUTRAL,
                   /* lpBuffer */ (LPSTR) &errorMsg,
                   /* dwSize */ 0,
                   /* arguments */ NULL);
    if (errorMsg) {
        return std::string(errorMsg);
    } else {
        return "";
    }
#else
    // Linux/Mac error-reporting code
    return std::string(strerror(errno));
#endif // _WIN32
}

bool Platform::regex_match(std::string s, std::string regexp) {
    std::ostringstream os;
    os << "Regex.match(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return (result == "true");
}

int Platform::regex_matchCount(std::string s, std::string regexp) {
    std::ostringstream os;
    os << "Regex.matchCount(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return stringToInteger(result);
}

int Platform::regex_matchCountWithLines(std::string s, std::string regexp, std::string& linesOut) {
    std::ostringstream os;
    os << "Regex.matchCountWithLines(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ")";
    putPipe(os.str());
    linesOut = getResult();       // "count:line,line,line,...,line"
    std::string countStr = "";
    while (!linesOut.empty() && linesOut[0] != ':') {
        countStr += linesOut[0];
        linesOut = linesOut.substr(1);
    }
    if (linesOut.empty() || linesOut[0] != ':' || !stringIsInteger(countStr)) {
        // error, invalid result string from server
        // error("Platform::regex_matchCountWithLines: illegal result string from server");
        return -1;
    }
    linesOut = linesOut.substr(1);    // remove the initial ':'
    return stringToInteger(countStr);
}

std::string Platform::regex_replace(std::string s, std::string regexp, std::string replacement, int /* limit */) {
    std::ostringstream os;
    os << "Regex.replace(";
    writeQuotedString(os, urlEncode(s));
    os << ",";
    writeQuotedString(os, urlEncode(regexp));
    os << ",";
    writeQuotedString(os, urlEncode(replacement));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return urlDecode(result);
}

std::string Platform::file_openFileDialog(std::string title, std::string mode, std::string path) {
    std::ostringstream os;
    os << "File.openFileDialog(";
    writeQuotedString(os, title);
    os << ", \"" << mode << "\", ";
    
    // BUGFIX: (2014/10/09) wasn't working if dirs didn't have slashes at end
    std::string sep = getDirectoryPathSeparator();
    if (isDirectory(path) && !endsWith(path, sep)) {
        path += sep;
    }
    writeQuotedString(os, path);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gwindow_constructor(const GWindow& gw, double width, double height,
                             GObject *topCompound, bool visible) {
    std::ostringstream os;
    os << gw.gwd;
    std::string id = os.str();
    windowTable.put(id, gw.gwd);
    os.str("");
    os << "GWindow.create(\"" << id << "\", " << width << ", " << height
       << ", \"" << topCompound << "\", " << std::boolalpha << visible << ")";
    putPipe(os.str());
    getStatus();
}

void Platform::gwindow_delete(const GWindow& gw) {
    std::ostringstream os;
    os << gw.gwd;
    std::string id = os.str();
    windowTable.remove(id);
    os.str("");
    os << "GWindow.delete(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_close(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.close(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_requestFocus(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.requestFocus(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setExitOnClose(const GWindow& gw, bool value) {
    std::ostringstream os;
    os << "GWindow.setExitOnClose(\"" << gw.gwd << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gwindow_clear(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.clear(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_clearCanvas(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.clearCanvas(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_repaint(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.repaint(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setSize(const GWindow& gw, int width, int height) {
    std::ostringstream os;
    os << "GWindow.setSize(\"" << gw.gwd << "\", " << width << ", " << height
       << ")";
    putPipe(os.str());
}

void Platform::gwindow_setCanvasSize(const GWindow& gw, int width, int height) {
    std::ostringstream os;
    os << "GWindow.setCanvasSize(\"" << gw.gwd << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::gwindow_minimize(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.minimize(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_pack(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.pack(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setTitle(const GWindow& gw, std::string title) {
    std::ostringstream os;
    os << "GWindow.setTitle(\"" << gw.gwd << "\", ";
    writeQuotedString(os, title);
    os << ")";
    putPipe(os.str());
}

void Platform::gwindow_setLocation(const GWindow& gw, int x, int y) {
    std::ostringstream os;
    os << "GWindow.setLocation(\"" << gw.gwd << "\", " << x << ", " << y << ")";
    putPipe(os.str());
}

void Platform::gwindow_setLocationSaved(const GWindow& gw, bool value) {
    std::ostringstream os;
    os << "GWindow.setLocationSaved(\"" << gw.gwd << "\", "
       << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gwindow_toFront(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.toFront(\"" << gw.gwd << "\")";
    putPipe(os.str());
}

double Platform::gwindow_getScreenHeight() {
    putPipe("GWindow.getScreenHeight()");
    return stringToReal(getResult());
}

GDimension Platform::gwindow_getScreenSize() {
    putPipe("GWindow.getScreenSize()");
    std::string result = getResult();
    if (!startsWith(result, "GDimension(")) error("GWindow::getScreenSize: " + result);
    return scanDimension(result);
}

double Platform::gwindow_getScreenWidth() {
    putPipe("GWindow.getScreenWidth()");
    return stringToReal(getResult());
}

void Platform::gtimer_pause(double milliseconds) {
    std::ostringstream os;
    os << "GTimer.pause(" << milliseconds << ")";
    putPipe(os.str());
    getStatus();
}

void Platform::gtimer_constructor(const GTimer& timer, double delay) {
    std::ostringstream os;
    os << timer.gtd;
    std::string id = os.str();
    timerTable.put(id, timer.gtd);
    os.str("");
    os << "GTimer.create(\"" << id << "\", " << delay << ")";
    putPipe(os.str());
}

void Platform::gtimer_delete(const GTimer& timer) {
    std::ostringstream os;
    os << timer.gtd;
    std::string id = os.str();
    timerTable.remove(id);
    os.str("");
    os << "GTimer.deleteTimer(\"" << id << "\")";
    putPipe(os.str());
}

void Platform::gtimer_start(const GTimer& timer) {
    std::ostringstream os;
    os << "GTimer.startTimer(\"" << timer.gtd << "\")";
    putPipe(os.str());
}

void Platform::gtimer_stop(const GTimer& timer) {
    std::ostringstream os;
    os << "GTimer.stopTimer(\"" << timer.gtd << "\")";
    putPipe(os.str());
}

void Platform::httpserver_sendResponse(int requestID, int httpErrorCode, const std::string& contentType, const std::string& responseText) {
    std::ostringstream os;
    os << "HttpServer.sendResponse(" << requestID << "," << httpErrorCode << ",";
    writeQuotedString(os, contentType);
    os << ",";
    writeQuotedString(os, responseText);
    os << ")";
    putPipe(os.str());
}

void Platform::httpserver_sendResponseFile(int requestID, const std::string& contentType, const std::string& responseFilePath) {
    std::ostringstream os;
    os << "HttpServer.sendResponseFile(" << requestID << ",";
    writeQuotedString(os, contentType);
    os << ",";
    writeQuotedString(os, responseFilePath);
    os << ")";
    putPipe(os.str());
}

void Platform::httpserver_start(int port) {
    std::ostringstream os;
    os << "HttpServer.start(" << port << ")";
    putPipe(os.str());
}

void Platform::httpserver_stop() {
    std::ostringstream os;
    os << "HttpServer.stop()";
    putPipe(os.str());
}

void Platform::sound_constructor(Sound *sound, std::string filename) {
    std::ostringstream os;
    os << "Sound.create(\"" << sound << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    getStatus();
}

void Platform::sound_delete(Sound *sound) {
    std::ostringstream os;
    os << "Sound.delete(\"" << sound << "\")";
    putPipe(os.str());
}

void Platform::sound_play(Sound *sound) {
    std::ostringstream os;
    os << "Sound.play(\"" << sound << "\")";
    putPipe(os.str());
}

int Platform::url_download(std::string url, std::string filename) {
    std::ostringstream os;
    os << "URL.download(";
    writeQuotedString(os, urlEncode(url));
    os << ",";
    writeQuotedString(os, urlEncode(filename));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return stringToInteger(result);
}

void Platform::gobject_delete(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.delete(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gcompound_add(GObject *compound, GObject* gobj) {
    std::ostringstream os;
    os << "GCompound.add(\"" << compound << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_remove(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.remove(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setRegionAlignment(const GWindow& gw, std::string region,
                                  std::string align) {
    std::ostringstream os;
    os << "GWindow.setRegionAlignment(\"" << gw.gwd << "\", \"" << region
       << "\", \"" << align << "\")";
    putPipe(os.str());
}

void Platform::gwindow_setResizable(const GWindow& gw, bool value) {
    std::ostringstream os;
    os << "GWindow.setResizable(\"" << gw.gwd << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gwindow_addToRegion(const GWindow& gw, GObject* gobj, const std::string& region) {
    std::ostringstream os;
    os << "GWindow.addToRegion(\"" << gw.gwd << "\", \"" << gobj << "\", \""
       << region << "\")";
    putPipe(os.str());
}

Point Platform::gwindow_getLocation(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.getLocation(\"" << gw.gwd << "\")";
    putPipe(os.str());
    std::string result = getResult();
    if (!startsWith(result, "Point(")) {
        error("GWindow::getLocation: " + result);
    }
    return scanPoint(result);
}

GDimension Platform::gwindow_getRegionSize(const GWindow& gw, std::string region) {
    std::ostringstream os;
    os << "GWindow.getRegionSize(\"" << gw.gwd << "\", \"" << region << "\")";
    putPipe(os.str());
    std::string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error("GWindow::getRegionSize: " + result);
    }
    return scanDimension(result);
}

void Platform::gwindow_removeFromRegion(const GWindow& gw, GObject* gobj,
                                std::string region) {
    std::ostringstream os;
    os << "GWindow.removeFromRegion(\"" << gw.gwd << "\", \""
       << gobj << "\", \"" << region << "\")";
    putPipe(os.str());
}

GDimension Platform::gwindow_getSize(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.getSize(\"" << gw.gwd << "\")";
    putPipe(os.str());
    std::string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error("GWindow::getSize: " + result);
    }
    return scanDimension(result);
}

GDimension Platform::gwindow_getCanvasSize(const GWindow& gw) {
    std::ostringstream os;
    os << "GWindow.getCanvasSize(\"" << gw.gwd << "\")";
    putPipe(os.str());
    std::string result = getResult();
    if (!startsWith(result, "GDimension(")) {
        error("GWindow::getCanvasSize: " + result);
    }
    return scanDimension(result);
}

void Platform::gobject_sendForward(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.sendForward(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendToFront(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.sendToFront(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendBackward(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.sendBackward(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_sendToBack(GObject* gobj) {
    std::ostringstream os;
    os << "GObject.sendToBack(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_setVisible(GObject* gobj, bool flag) {
    std::ostringstream os;
    os << "GObject.setVisible(\"" << gobj << "\", " << std::boolalpha << flag << ")";
    putPipe(os.str());
}

void Platform::gwindow_setVisible(const GWindow& gw, bool flag) {
    std::ostringstream os;
    os << "GWindow.setVisible(\"" << gw.gwd << "\", " << std::boolalpha << flag << ")";
    putPipe(os.str());
}

void Platform::gobject_setColor(GObject* gobj, std::string color) {
    std::ostringstream os;
    os << "GObject.setColor(\"" << gobj << "\", \"" << color << "\")";
    putPipe(os.str());
}

void Platform::gobject_scale(GObject* gobj, double sx, double sy) {
    std::ostringstream os;
    os << "GObject.scale(\"" << gobj << "\", " << sx << ", " << sy << ")";
    putPipe(os.str());
}

void Platform::gobject_rotate(GObject* gobj, double theta) {
    std::ostringstream os;
    os << "GObject.rotate(\"" << gobj << "\", " << theta << ")";
    putPipe(os.str());
}

// Move this computation into gobjects.cpp

bool Platform::gobject_contains(const GObject* gobj, double x, double y) {
    std::ostringstream os;
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
    std::ostringstream os;
    os << "GObject.getBounds(\"" << gobj << "\")";
    putPipe(os.str());
     std::string result = getResult();
    if (!startsWith(result, "GRectangle(")) error(result);
    return scanRectangle(result);
}

void Platform::gobject_setLineWidth(GObject* gobj, double lineWidth) {
    std::ostringstream os;
    os << "GObject.setLineWidth(\"" << gobj << "\", " << lineWidth << ")";
    putPipe(os.str());
}

void Platform::gobject_setLocation(GObject* gobj, double x, double y) {
    std::ostringstream os;
    os << "GObject.setLocation(\"" << gobj << "\", " << x << ", " << y << ")";
    putPipe(os.str());
}

void Platform::gobject_setSize(GObject* gobj, double width, double height) {
    std::ostringstream os;
    os << "GObject.setSize(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

bool Platform::ginteractor_isEnabled(GObject* gint) {
    std::ostringstream os;
    os << "GInteractor.isEnabled(\"" << gint << ")";
    putPipe(os.str());
    return getResult() == "true";
}

void Platform::ginteractor_setEnabled(GObject* gint, bool value) {
    std::ostringstream os;
    os << "GInteractor.setEnabled(\"" << gint << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::ginteractor_setIcon(GObject* gobj, std::string filename) {
    std::ostringstream os;
    os << "GInteractor.setIcon(\"" << gobj << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
}

void Platform::ginteractor_setTextPosition(GObject* gobj, int horizontal, int vertical) {
    std::ostringstream os;
    os << "GInteractor.setTextPosition("
       << "\"" << gobj << "\""
       << ", " << horizontal
       << ", " << vertical << ")";
    putPipe(os.str());
}

void Platform::gobject_setAntialiasing(bool value) {
    std::ostringstream os;
    os << "GObject.setAntialiasing(" << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::garc_setFrameRectangle(GObject* gobj, double x, double y,
                                 double width, double height) {
    std::ostringstream os;
    if (x >= 0 && y >= 0 && width >= 0 && height >= 0) {
        os << "GArc.setFrameRectangle(\"" << gobj << "\", "
           << x << ", " << y << ", "
           << width << ", " << height << ")";
        putPipe(os.str());
    } else {
        error("GArc::setFrameRectangle: x, y, w, h must all be non-negative");
    }
}

void Platform::gwindow_draw(const GWindow& gw, const GObject* gobj) {
    std::ostringstream os;
    os << "GWindow.draw(\"" << gw.gwd << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gwindow_drawInBackground(const GWindow& gw, const GObject* gobj) {
    std::ostringstream os;
    os << "GWindow.drawInBackground(\"" << gw.gwd << "\", \"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gobject_setFilled(GObject* gobj, bool flag) {
    std::ostringstream os;
    os << "GObject.setFilled(\"" << gobj << "\", " << std::boolalpha << flag << ")";
    putPipe(os.str());
}

void Platform::gobject_setFillColor(GObject* gobj, std::string color) {
    std::ostringstream os;
    os << "GObject.setFillColor(\"" << gobj << "\", \"" << color << "\")";
    putPipe(os.str());
}

void Platform::grect_constructor(GObject* gobj, double width, double height) {
    std::ostringstream os;
    os << "GRect.create(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::groundrect_constructor(GObject* gobj, double width, double height,
                                double corner) {
    std::ostringstream os;
    os << "GRoundRect.create(\"" << gobj << "\", " << width << ", " << height
       << ", " << corner << ")";
    putPipe(os.str());
}

void Platform::g3drect_constructor(GObject* gobj, double width, double height,
                             bool raised) {
    std::ostringstream os;
    os << "G3DRect.create(\"" << gobj << "\", "
       << width << ", " << height << ", " << std::boolalpha << raised << ")";
    putPipe(os.str());
}

void Platform::g3drect_setRaised(GObject* gobj, bool raised) {
    std::ostringstream os;
    os << "G3DRect.setRaised(\"" << gobj << "\", "
       << std::boolalpha << raised << ")";
    putPipe(os.str());
}

void Platform::glabel_constructor(GObject* gobj, std::string label) {
    std::ostringstream os;
    // *** BUGBUG: must escape quotation marks in label string (Marty)
    os << "GLabel.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

void Platform::gline_constructor(GObject* gobj, double x1, double y1,
                           double x2, double y2) {
    std::ostringstream os;
    os << "GLine.create(\"" << gobj << "\", " << x1 << ", " << y1
       << ", " << x2 << ", " << y2 << ")";
    putPipe(os.str());
}

void Platform::gline_setStartPoint(GObject* gobj, double x, double y) {
    std::ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GLine.setStartPoint(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("GLine::setStartPoint: x and y must both be non-negative");
    }
}

void Platform::gline_setEndPoint(GObject* gobj, double x, double y) {
    std::ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GLine.setEndPoint(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("GLine::setEndPoint: x and y must both be non-negative");
    }
}

void Platform::garc_constructor(GObject* gobj, double width, double height,
                          double start, double sweep) {
    std::ostringstream os;
    os << "GArc.create(\"" << gobj << "\", " << width << ", " << height
       << ", " << start << ", " << sweep << ")";
    putPipe(os.str());
}

void Platform::garc_setStartAngle(GObject* gobj, double angle) {
    std::ostringstream os;
    os << "GArc.setStartAngle(\"" << gobj << "\", " << angle << ")";
    putPipe(os.str());
}

void Platform::garc_setSweepAngle(GObject* gobj, double angle) {
    std::ostringstream os;
    os << "GArc.setSweepAngle(\"" << gobj << "\", " << angle << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_constructor(GObject* gobj, double x, double y,
                                          double width, double height, int rgb) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GBufferedImage.create(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << (int) width << ", " << (int) height << ", " << rgb << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_fill(GObject* gobj, int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.fill(\"" << gobj << "\", " << rgb << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_fillRegion(GObject* gobj, double x, double y, double width, double height, int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.fillRegion(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << (int) width << ", " << (int) height << ", " << rgb << ")";   // BUGBUG: was missing ", " token
    putPipe(os.str());
}

std::string Platform::gbufferedimage_load(GObject* gobj, const std::string& filename) {
    std::ostringstream os;
    os << "GBufferedImage.load(\"" << gobj << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gbufferedimage_resize(GObject* gobj, double width, double height, bool retain) {
    std::ostringstream os;
    os << "GBufferedImage.resize(\"" << gobj << "\", " << (int) width << ", " << (int) height
       << ", " << std::boolalpha << retain << ")";
    putPipe(os.str());
}

std::string Platform::gbufferedimage_save(const GObject* const gobj, const std::string& filename) {
    std::ostringstream os;
    os << "GBufferedImage.save(\"" << gobj << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gbufferedimage_setRGB(GObject* gobj, double x, double y,
                                     int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.setRGB(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << rgb << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_updateAllPixels(GObject* gobj,
                                              const std::string& base64) {
    std::ostringstream os;
    os << "GBufferedImage.updateAllPixels(\"" << gobj << "\", \"" << base64 << "\")";
    putPipe(os.str());
}

GDimension Platform::gimage_constructor(GObject* gobj, std::string filename) {
    std::ostringstream os;
    os << "GImage.create(\"" << gobj << "\", \"" << filename << "\")";
    putPipe(os.str());
    std::string result = getResult();
    if (!startsWith(result, "GDimension(")) error("GImage::constructor: " + result);
    return scanDimension(result);
}

void Platform::gpolygon_constructor(GObject* gobj) {
    std::ostringstream os;
    os << "GPolygon.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gpolygon_addVertex(GObject* gobj, double x, double y) {
    std::ostringstream os;
    if (x >= 0 && y >= 0) {
        os << "GPolygon.addVertex(\"" << gobj << "\", " << x << ", " << y << ")";
        putPipe(os.str());
    } else {
        error("GPolygon::addVertex: x and y must both be non-negative");
    }
}

void Platform::goval_constructor(GObject* gobj, double width, double height) {
    std::ostringstream os;
    os << "GOval.create(\"" << gobj << "\", " << width << ", "
       << height << ")";
    putPipe(os.str());
}

void Platform::ginteractor_setActionCommand(GObject* gobj, std::string cmd) {
    std::ostringstream os;
    os << "GInteractor.setActionCommand(\"" << gobj << "\", ";
    writeQuotedString(os, cmd);
    os << ")";
    putPipe(os.str());
}

void Platform::ginteractor_setBackground(GObject* gobj, std::string color) {
    std::ostringstream os;
    os << "GInteractor.setBackground(\"" << gobj << "\", \"" << color << "\")";
    putPipe(os.str());
}

GDimension Platform::ginteractor_getSize(GObject* gobj) {
    std::ostringstream os;
    os << "GInteractor.getSize(\"" << gobj << "\")";
    putPipe(os.str());
    return scanDimension(getResult());
}

void Platform::gbutton_constructor(GObject* gobj, std::string label) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GButton.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

void Platform::gcheckbox_constructor(GObject* gobj, std::string label) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GCheckBox.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ")";
    putPipe(os.str());
}

bool Platform::gcheckbox_isSelected(GObject* gobj) {
    std::ostringstream os;
    os << "GCheckBox.isSelected(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult() == "true";
}

void Platform::gcheckbox_setSelected(GObject* gobj, bool state) {
    std::ostringstream os;
    os << "GCheckBox.setSelected(\"" << gobj << "\", "
       << std::boolalpha << state << ")";
    putPipe(os.str());
}

void Platform::gradiobutton_constructor(GObject* gobj, std::string label, std::string group) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GRadioButton.create(\"" << gobj << "\", ";
    writeQuotedString(os, label);
    os << ",";
    writeQuotedString(os, group);
    os << ")";
    putPipe(os.str());
}

bool Platform::gradiobutton_isSelected(GObject* gobj) {
    std::ostringstream os;
    os << "GRadioButton.isSelected(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult() == "true";
}

void Platform::gradiobutton_setSelected(GObject* gobj, bool state) {
    std::ostringstream os;
    os << "GRadioButton.setSelected(\"" << gobj << "\", "
       << std::boolalpha << state << ")";
    putPipe(os.str());
}

void Platform::gslider_constructor(GObject* gobj, int min, int max, int value) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GSlider.create(\"" << gobj << "\", " << min << ", " << max
       << ", " << value << ")";
    putPipe(os.str());
}

int Platform::gslider_getMajorTickSpacing(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getMajorTickSpacing(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

int Platform::gslider_getMinorTickSpacing(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getMinorTickSpacing(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

bool Platform::gslider_getPaintLabels(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getPaintLabels(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

bool Platform::gslider_getPaintTicks(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getPaintTicks(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

bool Platform::gslider_getSnapToTicks(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getSnapToTicks(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

int Platform::gslider_getValue(const GObject* gobj) {
    std::ostringstream os;
    os << "GSlider.getValue(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

void Platform::gslider_setMajorTickSpacing(GObject* gobj, int value) {
    std::ostringstream os;
    os << "GSlider.setMajorTickSpacing(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setMinorTickSpacing(GObject* gobj, int value) {
    std::ostringstream os;
    os << "GSlider.setMinorTickSpacing(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setPaintLabels(GObject* gobj, bool value) {
    std::ostringstream os;
    os << "GSlider.setPaintLabels(\"" << gobj << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setPaintTicks(GObject* gobj, bool value) {
    std::ostringstream os;
    os << "GSlider.setPaintTicks(\"" << gobj << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setSnapToTicks(GObject* gobj, bool value) {
    std::ostringstream os;
    os << "GSlider.setSnapToTicks(\"" << gobj << "\", " << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gslider_setValue(GObject* gobj, int value) {
    std::ostringstream os;
    os << "GSlider.setValue(\"" << gobj << "\", " << value << ")";
    putPipe(os.str());
}

void Platform::gtable_clear(GObject* gobj) {
    std::ostringstream os;
    os << "GTable.clear(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gtable_constructor(GObject* gobj, int numRows, int numCols,
                                  double x, double y, double width, double height) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GTable.create(\"" << gobj << "\", " << numRows << ", " << numCols
       << ", " << x << ", " << y << ", " << width << ", " << height << ")";
    putPipe(os.str());
}

std::string Platform::gtable_get(const GObject * gobj, int row, int column) {
    std::ostringstream os;
    os << "GTable.get(\"" << gobj << "\", " << row << ", " << column << ")";
    putPipe(os.str());
    return getResult();
}

int Platform::gtable_getColumnWidth(const GObject* gobj, int column) {
    std::ostringstream os;
    os << "GTable.getColumnWidth(\"" << gobj << "\", " << column << ")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

void Platform::gtable_getSelection(const GObject* gobj, int& row, int& column) {
    std::ostringstream os;
    os << "GTable.getSelection(\"" << gobj << "\")";
    putPipe(os.str());
    row = stringToInteger(getResult());
    column = stringToInteger(getResult());
}

void Platform::gtable_resize(GObject* gobj, int numRows, int numCols) {
    std::ostringstream os;
    os << "GTable.resize(\"" << gobj << "\", " << numRows << ", " << numCols << ")";
    putPipe(os.str());
}

void Platform::gtable_select(GObject* gobj, int row, int column) {
    std::ostringstream os;
    os << "GTable.select(\"" << gobj << "\", " << row << ", " << column << ")";
    putPipe(os.str());
}

void Platform::gtable_set(GObject* gobj, int row, int column, const std::string& value) {
    std::ostringstream os;
    os << "GTable.set(\"" << gobj << "\", " << row << ", " << column << ", ";
    writeQuotedString(os, value);
    os << ")";
    putPipe(os.str());
}

void Platform::gtable_setColumnWidth(GObject* gobj, int column, int width) {
    std::ostringstream os;
    os << "GTable.setColumnWidth(\"" << gobj << "\", " << column << ", " << width << ")";
    putPipe(os.str());
}

void Platform::gtable_setEditable(GObject* gobj, bool editable) {
    std::ostringstream os;
    os << "GTable.setEditable(\"" << gobj << "\", " << std::boolalpha << editable << ")";
    putPipe(os.str());
}

void Platform::gtable_setEventEnabled(GObject* gobj, int type, bool enabled) {
    std::ostringstream os;
    os << "GTable.setEventEnabled(\"" << gobj << "\", " << type
       << ", " << std::boolalpha << enabled << ")";
    putPipe(os.str());
}

void Platform::gtable_setFont(GObject* gobj, const std::string& font) {
    std::ostringstream os;
    os << "GTable.setFont(\"" << gobj << "\", ";
    writeQuotedString(os, font);
    os << ")";
    putPipe(os.str());
}

void Platform::gtable_setHorizontalAlignment(GObject* gobj, const std::string& alignment) {
    std::ostringstream os;
    os << "GTable.setHorizontalAlignment(\"" << gobj << "\", ";
    writeQuotedString(os, alignment);
    os << ")";
    putPipe(os.str());
}

void Platform::gtextfield_constructor(GObject* gobj, int nChars) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GTextField.create(\"" << gobj << "\", " << nChars << ")";
    putPipe(os.str());
}

std::string Platform::gtextfield_getText(GObject* gobj) {
    std::ostringstream os;
    os << "GTextField.getText(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult();
}

bool Platform::gtextfield_isEditable(const GObject* gobj) {
    std::ostringstream os;
    os << "GTextField.isEditable(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToBool(getResult());
}

void Platform::gtextfield_setEditable(GObject* gobj, bool value) {
    std::ostringstream os;
    os << "GTextField.setEditable(\"" << gobj << "\", "
       << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::gtextfield_setText(GObject* gobj, std::string str) {
    std::ostringstream os;
    os << "GTextField.setText(\"" << gobj << "\", ";
    writeQuotedString(os, str);
    os << ")";
    putPipe(os.str());
}

void Platform::gchooser_constructor(GObject* gobj) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GChooser.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::gchooser_addItem(GObject* gobj, std::string item) {
    std::ostringstream os;
    os << "GChooser.addItem(\"" << gobj << "\", ";
    writeQuotedString(os, item);
    os << ")";
    putPipe(os.str());
}

std::string Platform::gchooser_getSelectedItem(GObject* gobj) {
    std::ostringstream os;
    os << "GChooser.getSelectedItem(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult();
}

void Platform::gchooser_setSelectedItem(GObject* gobj, std::string item) {
    std::ostringstream os;
    os << "GChooser.setSelectedItem(\"" << gobj << "\", ";
    writeQuotedString(os, item);
    os << ")";
    putPipe(os.str());
}

void Platform::gcompound_constructor(GObject* gobj) {
    std::ostringstream os;
    os << "GCompound.create(\"" << gobj << "\")";
    putPipe(os.str());
}

void Platform::glabel_setFont(GObject* gobj, std::string font) {
    std::ostringstream os;
    os << "GLabel.setFont(\"" << gobj << "\", \"" << font << "\")";
    putPipe(os.str());
}

void Platform::glabel_setLabel(GObject* gobj, std::string str) {
    std::ostringstream os;
    os << "GLabel.setLabel(\"" << gobj << "\", ";
    writeQuotedString(os, str);
    os << ")";
    putPipe(os.str());
}

double Platform::glabel_getFontAscent(const GObject* gobj) {
    std::ostringstream os;
    os << "GLabel.getFontAscent(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToReal(getResult());
}

double Platform::glabel_getFontDescent(const GObject* gobj) {
    std::ostringstream os;
    os << "GLabel.getFontDescent(\"" << gobj << "\")";
    putPipe(os.str());
    return stringToReal(getResult());
}

GDimension Platform::glabel_getSize(const GObject* gobj) {
    std::ostringstream os;
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
#ifdef PIPE_DEBUG
    fprintf(stderr, "Platform::waitForEvent returning event \"%s\"\n", event.toString().c_str());  fflush(stderr);
#endif // PIPE_DEBUG
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

std::string Platform::gfilechooser_showOpenDialog(std::string currentDir) {
    std::ostringstream os;
    os << "GFileChooser.showOpenDialog(";
    writeQuotedString(os, currentDir);
    os << ")";
    putPipe(os.str());
    return getResult();
}

std::string Platform::gfilechooser_showSaveDialog(std::string currentDir) {
    std::ostringstream os;
    os << "GFileChooser.showSaveDialog(";
    writeQuotedString(os, currentDir);
    os << ")";
    putPipe(os.str());
    return getResult();
}

int Platform::goptionpane_showConfirmDialog(std::string message, std::string title, int type) {
    std::ostringstream os;
    os << "GOptionPane.showConfirmDialog(";
    writeQuotedString(os, urlEncode(message));
    os << ",";
    writeQuotedString(os, urlEncode(title));
    os << "," << type;
    os << ")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

std::string Platform::goptionpane_showInputDialog(std::string message, std::string title) {
    std::ostringstream os;
    os << "GOptionPane.showInputDialog(";
    writeQuotedString(os, urlEncode(message));
    os << ",";
    writeQuotedString(os, urlEncode(title));
    os << ")";
    putPipe(os.str());
    return urlDecode(getResult());
}

void Platform::goptionpane_showMessageDialog(std::string message, std::string title, int type) {
    std::ostringstream os;
    os << "GOptionPane.showMessageDialog(";
    writeQuotedString(os, urlEncode(message));
    os << ",";
    writeQuotedString(os, urlEncode(title));
    os << "," << type;
    os << ")";
    putPipe(os.str());
    getResult();   // wait for dialog to close
}

int Platform::goptionpane_showOptionDialog(std::string message,
                                           std::string title,
                                           const std::vector<std::string>& options,
                                           std::string initiallySelected) {
    std::ostringstream os;
    os << "GOptionPane.showOptionDialog(";
    writeQuotedString(os, urlEncode(message));
    os << ",";
    writeQuotedString(os, urlEncode(title));
    os << ",";
    os << "{";
    if (!options.empty()) {
        writeQuotedString(os, options[0]);
        for (int i = 1, sz = options.size(); i < sz; ++i) {
            os << ", ";
            writeQuotedString(os, urlEncode(options[i]));
        }
    }
    os << "}";
    os << ",";
    writeQuotedString(os, urlEncode(initiallySelected));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return stringToInteger(result);
}

void Platform::goptionpane_showTextFileDialog(std::string message,
                                              std::string title,
                                              int rows,
                                              int cols) {
    std::ostringstream os;
    os << "GOptionPane.showTextFileDialog(";
    writeQuotedString(os, urlEncode(message));
    os << ",";
    writeQuotedString(os, urlEncode(title));
    os << "," << rows;
    os << "," << cols;
    os << ")";
    putPipe(os.str());
    getResult();   // wait for dialog to close
}

Platform *getPlatform() {
    static Platform gp;
    return &gp;
}

static void putPipeLongString(std::string line) {
    // break into chunks
    // precondition: line does not contain substring "LongCommand.end()"
    putPipe("LongCommand.begin()");
    size_t len = line.length();
    for (size_t i = 0; i < len; i += PIPE_MAX_COMMAND_LENGTH) {
        std::string chunk = line.substr(i, std::min(PIPE_MAX_COMMAND_LENGTH, len - i));
        putPipe(chunk);
    }
    putPipe("LongCommand.end()");
}

#ifdef _WIN32

/* Windows implementation of interface to Java back end */

// formats an error message using Windows lookup of error codes and strings
// Windows implementation; see Unix implementation elsewhere in this file
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
            // use stderr directly rather than cerr because graphical console may be unreachable
            fputs("\n", stderr);
            fputs("***\n", stderr);
            fputs("*** STANFORD C++ LIBRARY\n", stderr);
            fputs("*** Error from Java back-end subprocess:\n", stderr);
            fprintf(stderr, "*** %s\n", errorMsg);
            fputs("***\n", stderr);
            fputs("\n", stderr);
        }
    }
    return result;
}

// Windows implementation; see Unix implementation elsewhere in this file
int startupMain(int argc, char **argv) {
    startupMainDontRunMain(argc, argv);

#ifndef SPL_AUTOGRADER_MODE
    extern int Main(int argc, char **argv);
    return Main(argc, argv);
#else // SPL_AUTOGRADER_MODE
    return 0;
#endif // SPL_AUTOGRADER_MODE
}

/*
 * This is a version of startupMain that does all of the setup but then does
 * not actually run startupMain.
 * This is used to facilitate the creation of autograder programs.
 * Windows implementation; see Unix implementation elsewhere in this file
 */
void startupMainDontRunMain(int /*argc*/, char** argv) {
    exceptions::setProgramNameForStackTrace(argv[0]);
    std::string arg0 = argv[0];
    programName = getRoot(getTail(arg0));
    initPipe();
    cinout_new_buf = new ConsoleStreambuf();
    std::cin.rdbuf(cinout_new_buf);
    std::cout.rdbuf(cinout_new_buf);
    std::cerr.rdbuf(new ForwardingStreambuf(*cinout_new_buf, true));
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    getPlatform()->cpplib_setCppLibraryVersion();
    setConsoleProperties();
}

// Windows implementation; see Unix implementation elsewhere in this file
static void initPipe() {
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = true;
    attr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&rdFromJBE, &wrFromJBE, &attr, 0)) {
        error("Platform::initPipe: Can't create fromJBE");
    }
    if (!SetHandleInformation(rdFromJBE, HANDLE_FLAG_INHERIT, 0)) {
        error("Platform::initPipe: SetHandleInformation failed for fromJBE");
    }
    if (!CreatePipe(&rdToJBE, &wrToJBE, &attr, 0)) {
        error("Platform::initPipe: Can't create toJBE");
    }
    if (!SetHandleInformation(wrToJBE, HANDLE_FLAG_INHERIT, 0)) {
        error("Platform::initPipe: SetHandleInformation failed for toJBE");
    }
    std::string cmd = getJavaCommand();
#ifdef PIPE_DEBUG
    cmd += " -Dstanfordspl.debug=true";
#endif // PIPE_DEBUG
    cmd += " -jar spl.jar";
    cmd += std::string(" ") + programName;
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
        // DWORD err = GetLastError();
        // use stderr directly rather than cerr because graphical console is not connected
        fputs("\n", stderr);
        fputs("***\n", stderr);
        fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
        fputs("*** Unable to connect to Java back-end\n", stderr);
        fputs("*** to launch 'spl.jar' command.\n", stderr);
        fputs("*** Please check your Java installation and make sure\n", stderr);
        fputs("*** that spl.jar is properly attached to your project.\n", stderr);
        fputs("***\n", stderr);
        fflush(stderr);
    } else {
        CloseHandle(pInfo.hProcess);
        CloseHandle(pInfo.hThread);
    }
}

// Windows implementation; see Unix implementation elsewhere in this file
static void putPipe(std::string line) {
    if (line.length() > PIPE_MAX_COMMAND_LENGTH) {
        putPipeLongString(line);
        return;
    }
    
    DWORD nch;
#ifdef PIPE_DEBUG
    fprintf(stderr, "putPipe(\"%s\")\n", line.c_str());  fflush(stderr);
#endif // PIPE_DEBUG
    if (!WinCheck(WriteFile(wrToJBE, line.c_str(), line.length(), &nch, NULL))) return;
    if (!WinCheck(WriteFile(wrToJBE, "\n", 1, &nch, NULL))) return;
    WinCheck(FlushFileBuffers(wrToJBE));
}

// Windows implementation; see Unix implementation elsewhere in this file
static std::string getPipe() {
    std::string line = "";
    DWORD nch;
#ifdef PIPE_DEBUG
    fprintf(stderr, "getPipe(): waiting ...\n");  fflush(stderr);
#endif // PIPE_DEBUG

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

#ifdef PIPE_DEBUG
    fprintf(stderr, "getPipe(): returned \"%s\"\n", line.c_str());  fflush(stderr);
#endif
    return line;
}

#else // not WIN32

/* Linux/Mac implementation of interface to Java back end */

// Unix implementation; see Windows implementation elsewhere in this file
static bool LinCheck(ssize_t result) {
    if (result == EPIPE) {
        // fputs("Error from Java back-end subprocess.\n", stderr);
        // throw InterruptedIOException();
        return false;
    } else {
        return true;
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
static void scanOptions() {
    char *home = getenv("HOME");
    if (home != NULL) {
        std::string filename = std::string() + home + "/.spl";
        std::ifstream infile(filename.c_str());
        if (!infile.fail()) {
            std::string line;
            while (getline(infile, line)) {
                size_t equals = line.find('=');
                if (equals != std::string::npos) {
                    std::string key = line.substr(0, equals);
                    std::string value = line.substr(equals + 1);
                    optionTable.put(key, value);
                }
            }
            infile.close();
        }
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
static std::string getOption(std::string key) {
    char *str = getenv(key.c_str());
    if (str != NULL) return std::string(str);
    return optionTable.get(key);
}

// Unix implementation; see Windows implementation elsewhere in this file
#ifdef SPL_AUTOGRADER_MODE
int startupMain(int /*argc*/, char** argv) {
#else // not SPL_AUTOGRADER_MODE
int startupMain(int argc, char **argv) {
    extern int Main(int argc, char **argv);
#endif // SPL_AUTOGRADER_MODE
    std::string arg0 = argv[0];
    exceptions::setProgramNameForStackTrace(argv[0]);
    programName = getRoot(getTail(arg0));
    size_t ax = arg0.find(".app/Contents/");
    if (ax != std::string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            std::string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
    char *noConsoleFlag = getenv("NOCONSOLE");
    if (noConsoleFlag != NULL && startsWith(std::string(noConsoleFlag), "t")) {
#ifdef SPL_AUTOGRADER_MODE
        return 0;
#else // not SPL_AUTOGRADER_MODE
        return Main(argc, argv);
#endif // SPL_AUTOGRADER_MODE
    }
    scanOptions();
    initPipe();
    cinout_new_buf = new ConsoleStreambuf();
    std::cin.rdbuf(cinout_new_buf);
    std::cout.rdbuf(cinout_new_buf);
    std::cerr.rdbuf(new ForwardingStreambuf(*cinout_new_buf, true));
    std::string font = getOption("CPPFONT");
    if (font != "") {
#ifdef _console_h
        setConsoleFont(font);
#endif // _console_h
    }
    getPlatform()->cpplib_setCppLibraryVersion();
    setConsoleProperties();

#ifndef SPL_AUTOGRADER_MODE
    return Main(argc, argv);
#else // not SPL_AUTOGRADER_MODE
    return 0;
#endif // SPL_AUTOGRADER_MODE
}

/*
 * This is a version of startupMain that does all of the setup but then does
 * not actually run startupMain.
 * This is used to facilitate the creation of autograder programs.
 * Unix implementation; see Windows implementation elsewhere in this file
 */
void startupMainDontRunMain(int /*argc*/, char** argv) {
    std::string arg0 = argv[0];
    exceptions::setProgramNameForStackTrace(argv[0]);
    programName = getRoot(getTail(arg0));
    size_t ax = arg0.find(".app/Contents/");
    if (ax != std::string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            std::string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
    scanOptions();
    initPipe();
    
#ifndef SPL_DISABLE_GRAPHICAL_CONSOLE
    cinout_new_buf = new ConsoleStreambuf();
    std::cin.rdbuf(cinout_new_buf);
    std::cout.rdbuf(cinout_new_buf);
    std::cerr.rdbuf(new ForwardingStreambuf(*cinout_new_buf, true));
    std::string font = getOption("CPPFONT");
    if (font != "") {
        setConsoleFont(font);
    }
    setConsoleProperties();
#endif // SPL_DISABLE_GRAPHICAL_CONSOLE
    
#ifdef SPL_ECHO_PLAIN_CONSOLE
    // echo user input pulled from cin
    plainconsole::setEcho(true);
#ifdef SPL_CONSOLE_OUTPUT_LIMIT
    plainconsole::setOutputLimit(SPL_CONSOLE_OUTPUT_LIMIT);
#endif // SPL_CONSOLE_OUTPUT_LIMIT
#endif // SPL_ECHO_PLAIN_CONSOLE
    
    getPlatform()->cpplib_setCppLibraryVersion();
}

#ifndef SPL_HEADLESS_MODE
// Unix implementation; see Windows implementation elsewhere in this file
static void sigPipeHandler(int /*signum*/) {
    // use stderr directly rather than cerr because graphical console may be unreachable
    fputs("***\n", stderr);
    fputs("*** STANFORD C++ LIBRARY\n", stderr);
    fputs("*** Prematurely exiting program because console window was closed.\n", stderr);
    fputs("***\n", stderr);
    fflush(stderr);
    exit(1);
}
#endif // SPL_HEADLESS_MODE

// Unix implementation; see Windows implementation elsewhere in this file
static void initPipe() {
    char *trace = getenv("JBETRACE");
    logfile.open("/dev/tty");
    tracePipe = trace != NULL && startsWith(toLowerCase(trace), "t");

    std::string splHomeDir = "";
    char *splHome = getenv("SPL_HOME");
    if (splHome != NULL) {
        splHomeDir = splHome;
        // ensure that it ends with a trailing slash
        if (!splHomeDir.empty() && splHomeDir[splHomeDir.length() - 1] != '/') {
            splHomeDir += '/';
        }
    }
    
    // check whether spl.jar file exists (code taken from filelib_fileExists)
    std::string jarName = splHomeDir + "spl.jar";
    struct stat fileInfo;
    if (stat(jarName.c_str(), &fileInfo) != 0) {
        // use stderr directly rather than cerr because graphical console is unreachable
        fputs("\n", stderr);
        fputs("***\n", stderr);
        fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
        fputs("*** Unable to find the file 'spl.jar' for the Stanford\n", stderr);
        fputs("*** C++ library's Java back-end process.\n", stderr);
        fputs("*** Please make sure that spl.jar is properly attached to your project.\n", stderr);
        fputs("*** If you are trying to run a stand-alone executable, place spl.jar\n", stderr);
        fputs("*** in the same directory as your executable, or set the system\n", stderr);
        fputs("*** environment variable SPL_HOME to a directory path containing spl.jar.\n", stderr);
        fputs("***\n", stderr);
        fputs("*** (I looked for it in the following directory:)\n", stderr);
        if (splHomeDir == "") {
            char cwdbuf[1024];
            getcwd(cwdbuf, 1024);
            splHomeDir = cwdbuf;
        }
        fputs(("*** " + splHomeDir + "\n").c_str(), stderr);
        fputs("***\n", stderr);
        fflush(stderr);
        exit(1);
    }
    
    int toJBE[2], fromJBE[2];
    if (pipe(toJBE) != 0) {
        error("Unable to establish pipe to Java back-end; exiting.");
    }
    if (pipe(fromJBE) != 0) {
        error("Unable to establish pipe from Java back-end; exiting.");
    }
    int child = fork();
    if (child == 0) {
        // we are the Java back-end process; launch external Java command
        javaBackEndPid = getpid();
        dup2(toJBE[0], 0);
        close(toJBE[0]);
        close(toJBE[1]);
        dup2(fromJBE[1], 1);
        close(fromJBE[0]);
        close(fromJBE[1]);
        std::string javaCommand = getJavaCommand();
        
#ifdef __APPLE__
        std::string option = "-Xdock:name=" + programName;
        int execlpResult = execlp(javaCommand.c_str(), javaCommand.c_str(), option.c_str(), "-jar", jarName.c_str(),
               programName.c_str(), NULL);
        std::string fullCommand = javaCommand + " " + option + " -jar " + jarName + " " + programName;
#else // !APPLE
#ifdef SPL_HEADLESS_MODE
        int execlpResult = execlp(javaCommand.c_str(), javaCommand.c_str(), "-Djava.awt.headless=true", "-jar", jarName.c_str(), programName.c_str(), NULL);
        std::string fullCommand = javaCommand + " -jar " + jarName + " " + programName;
#else // !SPL_HEADLESS_MODE
        int execlpResult = execlp(javaCommand.c_str(), javaCommand.c_str(), "-jar", jarName.c_str(), programName.c_str(), NULL);
        std::string fullCommand = javaCommand + " -Djava.awt.headless=true -jar " + jarName + " " + programName;
#endif // SPL_HEADLESS_MODE
#endif // APPLE
        
        // if we get here, the execlp call failed, so show error message
        // use stderr directly rather than cerr because graphical console is unreachable
#ifndef SPL_HEADLESS_MODE
        if (execlpResult != 0) {
            char* lastError = strerror(errno);
            std::string workingDir = getCurrentDirectory();
            
            fputs("\n", stderr);
            fputs("***\n", stderr);
            fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
            fputs("*** Unable to launch process to connect to Java back-end\n", stderr);
            fputs("*** using the 'spl.jar' library.\n", stderr);
            fputs("*** Please check your Java installation and make sure\n", stderr);
            fputs("*** that spl.jar is properly attached to your project.\n", stderr);
            fputs("***\n", stderr);
            fputs(("*** Command was: " + fullCommand + "\n").c_str(), stderr);
            fputs(("*** Working dir: " + workingDir + "\n").c_str(), stderr);
            fputs(("***  Result was: " + integerToString(execlpResult) + "\n").c_str(), stderr);
            fputs((std::string("***   Error was: ") + lastError + std::string("\n")).c_str(), stderr);
            fputs("***\n", stderr);
            fflush(stderr);
            exit(1);
        }
#endif // SPL_HEADLESS_MODE
    } else {
        // we are the C++ process; connect pipe input/output
        cppLibPid = getpid();
        pin = fromJBE[0];
        pout = toJBE[1];
        close(fromJBE[1]);
        close(toJBE[0]);
        
        // stop the pipe from generating a SIGPIPE when JBE is closed
#ifndef SPL_HEADLESS_MODE
        signal(SIGPIPE, sigPipeHandler);
#endif // SPL_HEADLESS_MODE
    }
}

// Unix implementation; see Windows implementation elsewhere in this file
static void putPipe(std::string line) {
    if (line.length() > PIPE_MAX_COMMAND_LENGTH) {
        putPipeLongString(line);
        return;
    }
#ifdef PIPE_DEBUG
    fprintf(stderr, "putPipe(\"%s\")\n", line.c_str());  fflush(stderr);
#endif
    LinCheck(write(pout, line.c_str(), line.length()));
    LinCheck(write(pout, "\n", 1));
    if (tracePipe) logfile << "-> " << line << std::endl;
}

// Unix implementation; see Windows implementation elsewhere in this file
static std::string getPipe() {
#ifdef PIPE_DEBUG
    fprintf(stderr, "getPipe(): waiting ...\n");  fflush(stderr);
#endif
    std::string line = "";
    int charsRead = 0;
    int charsReadMax = PIPE_MAX_COMMAND_LENGTH + 100;
    while (charsRead < charsReadMax) {
        char ch;
        ssize_t result = read(pin, &ch, 1);
        if (result <= 0) {
            throw InterruptedIOException();
            // break;   // failed to read from subprocess
        }
        if (ch == '\n') {
            break;
        }
        line += ch;
        charsRead++;
    }
#ifdef PIPE_DEBUG
    fprintf(stderr, "getPipe(): \"%s\"\n", line.c_str());  fflush(stderr);
#endif
    if (tracePipe) logfile << "<- " << line << std::endl;
    return line;
}

#endif // WIN32

static std::string getResult(bool consumeAcks, const std::string& caller) {
    while (true) {
#ifdef PIPE_DEBUG
        fprintf(stderr, "getResult(): calling getPipe() ...\n");  fflush(stderr);
#endif
        std::string line = getPipe();
        
        bool isResult        = startsWith(line, "result:");
        bool isResultLong    = startsWith(line, "result_long:");
        bool isEvent         = startsWith(line, "event:");
        bool isAck           = startsWith(line, "result:___jbe___ack___");
        bool hasACMException = line.find("acm.util.ErrorException") != std::string::npos;
        bool hasException    = line.find("xception") != std::string::npos;
        bool hasError        = line.find("Unexpected error") != std::string::npos;

        if (isResultLong) {
            // read a 'long' result (sent across multiple lines)
            std::ostringstream os;
            std::string nextLine = getPipe();
            while (nextLine != "result_long:end") {
                if (!startsWith(line, "result:___jbe___ack___")) {
                    os << nextLine;
#ifdef PIPE_DEBUG
                    fprintf(stderr, "getResult(): appended line (length so far: %ld)\n", os.str().length());  fflush(stderr);
#endif
                }
                nextLine = getPipe();
            }
            std::string result = os.str();
#ifdef PIPE_DEBUG
            fprintf(stderr, "getResult(): returning long string \"%s ... %s\" (length %ld)\n",
                    result.substr(0, 10).c_str(),
                    result.substr(result.length() - 10, 10).c_str(),
                    result.length());  fflush(stderr);
#endif
            return result;
        } else if (((isResult || isEvent) && hasACMException) ||
                (!isResult && !isEvent && (hasException || hasError))) {
            // an error message from the back-end; throw it here
            std::ostringstream out;
            if (isResult) {
                line = line.substr(7);
            } else if (isEvent) {
                line = line.substr(6);
            }
            out << "ERROR emitted from Stanford Java back-end process:"
                << std::endl << line;
            error(out.str());
        } else if (isResult) {
            // a regular result
            if (!isAck || !consumeAcks) {
                std::string result = line.substr(7);
#ifdef PIPE_DEBUG
        fprintf(stderr, "getResult(): returning regular result (length %ld): \"%s\"\n", result.length(), result.c_str());  fflush(stderr);
#endif
                return result;
            } else {
                // else this is just an acknowledgment of some previous event;
                // not a real result of its own. consume it and keep waiting
#ifdef PIPE_DEBUG
        fprintf(stderr, "getResult(): saw ACK (length %ld): \"%s\"\n", line.length(), line.c_str());  fflush(stderr);
#endif
            }
        } else if (isEvent) {
            // a Java-originated event; enqueue it to process here
            GEvent event = parseEvent(line.substr(6));
            eventQueue.enqueue(event);
            if (event.getEventClass() == WINDOW_EVENT && event.getEventType() == CONSOLE_CLOSED
                    && caller == "getLineConsole") {
                return "";
            }
        } else {
            if (line.find("\tat ") != std::string::npos || line.find("   at ") != std::string::npos) {
                // a line from a back-end Java exception stack trace;
                // shouldn't really be happening, but back end isn't perfect.
                // echo it here to STDERR so C++ user can see it to help diagnose the issue
                fprintf(stderr, "%s\n", line.c_str());
                fflush(stderr);
            }
        }
    }
}

/*
 * Returns the full path to the java (Linux/Mac) or java.exe (Windows)
 * executable to be executed to launch the Java back-end.
 * If the JAVA_HOME environment variable has been set, looks there first
 * for the java executable.
 * If found, returns the one from JAVA_HOME.
 * Otherwise simply returns the string "java" and relies on this being found
 * in the system's execution path.
 * Setting JAVA_HOME can help disambiguate between multiple versions of Java
 * that might be found on the same machine.
 */
static std::string getJavaCommand() {
    static std::string DEFAULT_JAVA_COMMAND = "java";
    char* JAVA_HOME = getenv("JAVA_HOME");
    if (!JAVA_HOME) {
        return DEFAULT_JAVA_COMMAND;
    } else {
        std::string path = JAVA_HOME;
        if (path.empty()) {
            return DEFAULT_JAVA_COMMAND;
        }
        
        std::string pathSep = getDirectoryPathSeparator();
        if (!endsWith(path, pathSep)) {
            path += pathSep;
        }
        path += "bin" + pathSep + "java";
        if (pathSep == "\\") {
            path += ".exe";   // Windows
        }
        if (fileExists(path)) {
            return path;
        } else {
            return DEFAULT_JAVA_COMMAND;   // fallback
        }
    }
}

static void getStatus() {
    std::string result = getResult();
    if (result != "ok") {
        error(result);
    }
}

static GEvent parseEvent(std::string line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    std::string name = scanner.nextToken();
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
    } else if (name == "serverRequest") {
        return parseServerEvent(scanner, SERVER_REQUEST);
    } else if (name == "tableSelected") {
        return parseTableEvent(scanner, TABLE_SELECTED);
    } else if (name == "tableUpdated") {
        return parseTableEvent(scanner, TABLE_UPDATED);
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
#ifndef SPL_DISABLE_GRAPHICAL_CONSOLE
        // Java console window was closed; possibly exit the C++ program now
        extern bool getConsoleExitProgramOnClose();
        extern bool getConsoleEventOnClose();
        if (getConsoleExitProgramOnClose()) {
            // use stderr directly rather than cerr because graphical console is unreachable
            fputs("\n", stderr);
            fputs("***\n", stderr);
            fputs("*** STANFORD C++ LIBRARY\n", stderr);
            fputs("*** Prematurely exiting program because console window was closed\n", stderr);
            fputs("***\n", stderr);
            fputs("\n", stderr);
            fflush(stderr);

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
#endif // SPL_DISABLE_GRAPHICAL_CONSOLE
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
    std::string id = scanner.getStringValue(scanner.nextToken());
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
    std::string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(",");
    int modifiers = scanInt(scanner);
    scanner.verifyToken(",");
    int keyChar = scanChar(scanner);   // BUGFIX 2016/01/27: Thanks to K. Perry
    scanner.verifyToken(",");
    int keyCode = scanInt(scanner);
    scanner.verifyToken(")");
    GKeyEvent e(type, GWindow(windowTable.get(id)), char(keyChar), keyCode);
    e.setEventTime(time);
    e.setModifiers(modifiers);
    return e;
}

static GEvent parseServerEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    double time = scanDouble(scanner);
    scanner.verifyToken(",");
    int requestID = scanInt(scanner);
    scanner.verifyToken(",");
    std::string requestUrl = urlDecode(scanner.getStringValue(scanner.nextToken()));
    scanner.verifyToken(")");

    GServerEvent e(type, requestID, requestUrl);
    e.setEventTime(time);
    return e;
}

static GEvent parseTableEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    std::string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(",");
    int row = scanInt(scanner);
    scanner.verifyToken(",");
    int col = scanInt(scanner);
    std::string value;

    if (type == TABLE_UPDATED) {
        scanner.verifyToken(",");
        value = urlDecode(scanner.getStringValue(scanner.nextToken()));
    }
    scanner.verifyToken(")");
    
    GTableEvent e(type);  //, GTimer(timerTable.get(id)));
    e.setLocation(row, col);
    e.setValue(value);
    e.setEventTime(time);
    return e;
}

static GEvent parseTimerEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    std::string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GTimerEvent e(type, GTimer(timerTable.get(id)));
    e.setEventTime(time);
    return e;
}

static GEvent parseWindowEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    std::string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GWindowEvent e(type, GWindow(windowTable.get(id)));
    e.setEventTime(time);
    return e;
}

static GEvent parseActionEvent(TokenScanner& scanner, EventType type) {
    scanner.verifyToken("(");
    std::string id = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    std::string action = scanner.getStringValue(scanner.nextToken());
    scanner.verifyToken(",");
    double time = scanDouble(scanner);
    scanner.verifyToken(")");
    GActionEvent e(type, sourceTable.get(id), action);
    e.setEventTime(time);
    return e;
}

/* Console code */

void Platform::cpplib_setCppLibraryVersion() {
    std::ostringstream out;
    out << "StanfordCppLib.setCppVersion(";
    writeQuotedString(out, STANFORD_CPP_LIB_VERSION);
    out << ")";
    putPipe(out.str());
}

std::string Platform::cpplib_getJavaBackEndVersion() {
    putPipe("StanfordCppLib.getJbeVersion()");
    std::string result = getResult();
    // BUGFIX 2014/10/14: remove surrounding "" marks (pre-2014/10/16 JBE)
    if (startsWith(result, '"')) {
        result = result.substr(1);
    }
    if (endsWith(result, '"')) {
        result = result.substr(0, result.length() - 1);
    }
    return result;
}

void Platform::jbeconsole_clear() {
    putPipe("JBEConsole.clear()");
}

void Platform::jbeconsole_minimize() {
    putPipe("JBEConsole.minimize()");
}

void Platform::jbeconsole_setFont(const std::string & font) {
    std::ostringstream os;
    os << "JBEConsole.setFont(\"" << font << "\")";
    putPipe(os.str());
}

void Platform::jbeconsole_setSize(double width, double height) {
    std::ostringstream os;
    os << "JBEConsole.setSize(" << width << ", " << height << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setTitle(const std::string& title) {
    std::ostringstream os;
    os << "JBEConsole.setTitle(";
    writeQuotedString(os, title);
    os << ")";
    putPipe(os.str());
}

/*
 * pass (-1, -1) to center
 */
void Platform::jbeconsole_setLocation(int x, int y) {
    std::ostringstream os;
    os << "JBEConsole.setLocation(" << x << ", " << y << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setCloseOperation(int value) {
    std::ostringstream os;
    os << "JBEConsole.setCloseOperation(" << value << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setErrorColor(const std::string& color) {
    std::ostringstream os;
    os << "JBEConsole.setErrorColor(";
    writeQuotedString(os, color);
    os << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setExitProgramOnClose(bool value) {
    std::ostringstream os;
    os << "JBEConsole.setExitOnClose(" << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setLocationSaved(bool value) {
    std::ostringstream os;
    os << "JBEConsole.setLocationSaved(" << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setOutputColor(const std::string& color) {
    std::ostringstream os;
    os << "JBEConsole.setOutputColor(";
    writeQuotedString(os, color);
    os << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_setVisible(bool value) {
    std::ostringstream os;
    os << "JBEConsole.setVisible(" << std::boolalpha << value << ")";
    putPipe(os.str());
}

void Platform::jbeconsole_toFront() {
    putPipe("JBEConsole.toFront()");
}

void Platform::autograderinput_addButton(std::string text, std::string input) {
    if (input.empty()) {
        input = text;
    }
    std::ostringstream out;
    out << "AutograderInput.addButton(";
    writeQuotedString(out, urlEncode(text));
    out << ", ";
    writeQuotedString(out, urlEncode(input));
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_removeButton(std::string text) {
    std::ostringstream out;
    out << "AutograderInput.removeButton(";
    writeQuotedString(out, urlEncode(text));
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_addCategory(std::string name) {
    std::ostringstream out;
    out << "AutograderInput.addCategory(";
    writeQuotedString(out, urlEncode(name));
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_removeCategory(std::string name) {
    std::ostringstream out;
    out << "AutograderInput.removeCategory(";
    writeQuotedString(out, urlEncode(name));
    out << ")";
    putPipe(out.str());
}

void Platform::autograderinput_setVisible(bool value) {
    std::ostringstream out;
    out << "AutograderInput.setVisible(" << std::boolalpha << value << ")";
    putPipe(out.str());
}

void Platform::autograderunittest_addTest(const std::string& testName, const std::string& category, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.addTest(";
    writeQuotedString(os, urlEncode(testName));
    os << ",";
    writeQuotedString(os, urlEncode(category));
    os << "," << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_clearTests(bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.clearTests(" << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_clearTestResults(bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.clearTestResults(" << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

bool Platform::autograderunittest_isChecked(const std::string& testName) {
    std::ostringstream os;
    os << "AutograderUnitTest.isChecked(";
    writeQuotedString(os, urlEncode(testName));
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return (result == "true");
}

void Platform::autograderunittest_setChecked(const std::string& testName, bool checked) {
    std::ostringstream os;
    os << "AutograderUnitTest.setChecked(";
    writeQuotedString(os, urlEncode(testName));
    os << "," << std::boolalpha << checked << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setTestCounts(int passCount, int testCount, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setTestCounts("
       << passCount << "," << testCount
       << "," << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setTestDetails(const std::string& testName, const std::string& details, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setTestDetails(";
    writeQuotedString(os, urlEncode(testName));
    os << ",";
    std::string deets = details;
    stringReplaceInPlace(deets, "\n", "\\n");
    stringReplaceInPlace(deets, "\r", "\\r");
    stringReplaceInPlace(deets, "\t", "\\t");
    writeQuotedString(os, urlEncode(deets));
    os << "," << std::boolalpha << styleCheck << ")";
    std::string str = os.str();
    putPipe(str);
}

void Platform::autograderunittest_setTestingCompleted(bool completed, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setTestingCompleted("
       << std::boolalpha << completed
       << "," << std::boolalpha << styleCheck
       << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setTestResult(const std::string& testName, const std::string& result, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setTestResult(";
    writeQuotedString(os, urlEncode(testName));
    os << ",";
    writeQuotedString(os, urlEncode(result));
    os << "," << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setTestRuntime(const std::string& testName, int runtimeMS) {
    std::ostringstream os;
    os << "AutograderUnitTest.setTestRuntime(";
    writeQuotedString(os, urlEncode(testName));
    os << "," << runtimeMS << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setVisible(bool visible, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setVisible("
       << std::boolalpha << visible
       << "," << std::boolalpha << styleCheck
       << ")";
    putPipe(os.str());
}

void Platform::autograderunittest_setWindowDescriptionText(const std::string& text, bool styleCheck) {
    std::ostringstream os;
    os << "AutograderUnitTest.setWindowDescriptionText(";
    writeQuotedString(os, urlEncode(text));
    os << "," << std::boolalpha << styleCheck << ")";
    putPipe(os.str());
}

static std::string getLineConsole() {
    putPipe("JBEConsole.getLine()");
    std::string result = getResult(/* consumeAcks */ true, /* caller */ "getLineConsole");
    echoConsole(result + "\n");   // wrong for multiple inputs on one line
    return result;
}

static void putConsole(const std::string& str, bool isStderr) {
    std::ostringstream os;
    os << "JBEConsole.print(";

    // BUGFIX: strings that end with \\ don't print because of back-end error;
    //         kludge fix by appending an invisible space after it
    if (!str.empty() && str[str.length() - 1] == '\\') {
        std::string str2 = str + ' ';
        writeQuotedString(os, str2);
    } else {
        writeQuotedString(os, str);
    }
    
    os << "," << std::boolalpha << isStderr << ")";
    putPipe(os.str());
    echoConsole(str, isStderr);
}

#ifdef _console_h
static void echoConsole(const std::string& str, bool isStderr) {
    if (getConsoleEcho()) {
        // write to the standard (non-graphical) console for output copy/pasting
        fputs(str.c_str(), isStderr ? stderr : stdout);
        fflush(stdout);   // flush both stdout and stderr
        fflush(stderr);
    }
}
#else
static void echoConsole(const std::string&, bool) {
    // empty
}
#endif // _console_h

static void endLineConsole(bool isStderr) {
    putPipe("JBEConsole.println()");
    echoConsole("\n", isStderr);
}

static int scanChar(TokenScanner& scanner) {
    std::string token = scanner.nextToken();
    if (token == "-") token += scanner.nextToken();
    return stringToChar(token);
}

static int scanInt(TokenScanner& scanner) {
    std::string token = scanner.nextToken();
    if (token == "-") token += scanner.nextToken();
    return stringToInteger(token);
}

static double scanDouble(TokenScanner& scanner) {
    std::string token = scanner.nextToken();
    if (token == "-") token += scanner.nextToken();
    return stringToReal(token);
}

static GDimension scanDimension(const std::string& str) {
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

static Point scanPoint(const std::string& str) {
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

static GRectangle scanRectangle(const std::string& str) {
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
    std::string fontStr = std::string("Monospaced-Bold-") + integerToString(SPL_CONSOLE_FONTSIZE);
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
    
#if defined(SPL_VERIFY_JAVA_BACKEND_VERSION)
    version::ensureJavaBackEndVersion();
#endif
    
#if defined(SPL_VERIFY_PROJECT_VERSION) && defined(SPL_MINIMUM_PROJECT_VERSION)
    version::ensureProjectVersion();
#endif
}

//static void abortAllConsoleIO() {
//#ifdef _WIN32
//    CancelIo(wrToJBE);
//    CancelIo(rdFromJBE);
//#endif
//    throw InterruptedIOException();
//}
