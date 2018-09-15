/*
 * File: platform.c
 * ----------------
 * This file implements the platform-dependent aspects of the library,
 * primarily by passing commands to the Java back end.
 */

#ifdef _MSC_VER
#  include <windows.h>
#  include <tchar.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#endif

#if defined(__unix__) || defined(__STDC_HOSTED__)
#  define pause unixpause
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <dirent.h>
#  include <errno.h>
#  include <pwd.h>
#  include <unistd.h>
#  undef pause
#endif

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include "cslib.h"
#include "filelib.h"
#include "gevents.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "hashmap.h"
#include "platform.h"
#include "queue.h"
#include "simpio.h"
#include "strbuf.h"
#include "strlib.h"
#include "tokenscanner.h"

/* Constants */

#define MAX_ID_LENGTH 24

/* Package variables */

static string programName = "Test";
static FILE *pin;
static FILE *pout;
static bool initialized = false;
static Queue eventQueue;
static HashMap sourceTable;
static HashMap timerTable;
static HashMap windowTable;
static StringBuffer psb;

/* Private function prototypes */

static void initPipe(void);
static void putPipe(string format, ...);
static string getResult();
static void getStatus();
static int getInt();
static int getDouble();
static GDimension getGDimension();
static GEvent parseEvent(string line);
static GEvent parseMouseEvent(TokenScanner scanner, EventType type);
static GEvent parseKeyEvent(TokenScanner scanner, EventType type);
static GEvent parseTimerEvent(TokenScanner scanner, EventType type);
static GEvent parseWindowEvent(TokenScanner scanner, EventType type);
static GEvent parseActionEvent(TokenScanner scanner, EventType type);
static string scanString(TokenScanner scanner);
static int scanInt(TokenScanner scanner);
static double scanDouble(TokenScanner scanner);
static bool scanBool(TokenScanner scanner);
static string quotedString(string str);
static string boolString(bool flag);
static void registerSource(GInteractor interactor);

void initPlatform(void) {
   if (!initialized) initPipe();
   sourceTable = newHashMap();
   timerTable = newHashMap();
   windowTable = newHashMap();
   eventQueue = newQueue();
   psb = newStringBuffer();
   initialized = true;
}

string getId(void *ptr) {
   char str[MAX_ID_LENGTH];
   sprintf(str, "0x%lX", (long) ptr);
   return copyString(str);
}

#ifdef _MSC_VER

// Fix these

/* Windows implementation of interface to Java back end */

int startupMain(int argc, char **argv) {
   extern int Main(int argc, char **argv);
   programName = getRoot(getTail(argv[0]));
   initPipe();
   ConsoleStreambuf cbuf;
   cin.rdbuf(&cbuf);
   cout.rdbuf(&cbuf);
   ShowWindow(GetConsoleWindow(), SW_HIDE);
   return Main(argc, argv);
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
   string cmd = "java -jar spl.jar " + programName;
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
   int ok = CreateProcessA(NULL, cmdLine, NULL, NULL, true, 0,
                           NULL, NULL, &sInfo, &pInfo);
   if (!ok) {
      DWORD err = GetLastError();
      cerr << err << endl;
   } else {
      CloseHandle(pInfo.hProcess);
      CloseHandle(pInfo.hThread);
   }
}

#else

/* Linux/Mac implementation of interface to Java back end */

extern string *getMainArgArray(void);

static void initPipe(void) {
   int toJBE[2], fromJBE[2], child;
   string option;

   programName = getRoot(getTail(getMainArgArray()[0]));
   pipe(toJBE);
   pipe(fromJBE);
   child = fork();
   if (child == 0) {
      dup2(toJBE[0], 0);
      close(toJBE[0]);
      close(toJBE[1]);
      dup2(fromJBE[1], 1);
      close(fromJBE[0]);
      close(fromJBE[1]);
#ifdef __APPLE__
      option = concat("-Xdock:name=", programName);
      execlp("java", "java", option, "-jar", "spl.jar", programName, NULL);
#else
      execlp("java", "java", "-jar", "spl.jar", programName, NULL);
#endif
      error("Could not exec spl.jar");
   } else {
      pin = fdopen(fromJBE[0], "r");
      pout = fdopen(toJBE[1], "w");
      close(fromJBE[1]);
      close(toJBE[0]);
   }
}

#endif

static void putPipe(string format, ...) {
   string cmd, jbetrace;
   va_list args;
   int capacity;

   clearStringBuffer(psb);
   va_start(args, format);
   capacity = printfCapacity(format, args);
   va_end(args);
   va_start(args, format);
   sbFormat(psb, capacity, format, args);
   va_end(args);
   cmd = getString(psb);
   jbetrace = getenv("JBETRACE");
   if (jbetrace != NULL && (jbetrace[0] == 'T' || jbetrace[0] == 't')) {
      fprintf(stderr, "-> %s\n", cmd);
   }
   fprintf(pout, "%s\n", cmd);
   fflush(pout);
}

static string getResult() {
   string line, result, jbetrace;

   jbetrace = getenv("JBETRACE");
   while (true) {
      line = readLine(pin);
      if (line == NULL) exit(1);
      if (jbetrace != NULL && (jbetrace[0] == 'T' || jbetrace[0] == 't')) {
         fprintf(stderr, "<- %s\n", line);
      }
      if (startsWith(line, "result:")) {
         result = substring(line, 7, stringLength(line));
         freeBlock(line);
         return result;
      } else if (startsWith(line, "event:")) {
         enqueue(eventQueue, parseEvent(line + 6));
         freeBlock(line);
      }
   }
}

static void getStatus() {
   string result;

   result = getResult();
   if (!stringEqual(result, "ok")) error(result);
   freeBlock(result);
}

static int getInt() {
   string str;
   int result;

   str = getResult();
   result = stringToInteger(str);
   freeBlock(str);
   return result;
}

static int getDouble() {
   string str;
   double result;

   str = getResult();
   result = stringToReal(str);
   freeBlock(str);
   return result;
}

static GDimension getGDimension() {
   string str;
   double width, height;

   str = getResult();
   sscanf(str, "GDimension(%lg,%lg)", &width, &height);
   freeBlock(str);
   return createGDimension(width, height);
}

static int getBool() {
   string str;
   bool result;

   str = getResult();
   result = (str[0] == 'T' || str[0] == 't');
   freeBlock(str);
   return result;
}

static GEvent parseEvent(string line) {
   TokenScanner scanner;
   string name;
   GEvent e;

   scanner = newTokenScanner();
   ignoreWhitespace(scanner);
   scanNumbers(scanner);
   scanStrings(scanner);
   setInputString(scanner, line);
   name = nextToken(scanner);
   e = NULL;
   if (stringEqual(name, "mousePressed")) {
      e = parseMouseEvent(scanner, MOUSE_PRESSED);
   } else if (stringEqual(name, "mouseReleased")) {
      e = parseMouseEvent(scanner, MOUSE_RELEASED);
   } else if (stringEqual(name, "mouseClicked")) {
      e = parseMouseEvent(scanner, MOUSE_CLICKED);
   } else if (stringEqual(name, "mouseMoved")) {
      e = parseMouseEvent(scanner, MOUSE_MOVED);
   } else if (stringEqual(name, "mouseDragged")) {
      e = parseMouseEvent(scanner, MOUSE_DRAGGED);
   } else if (stringEqual(name, "keyPressed")) {
      e = parseKeyEvent(scanner, KEY_PRESSED);
   } else if (stringEqual(name, "keyReleased")) {
      e = parseKeyEvent(scanner, KEY_RELEASED);
   } else if (stringEqual(name, "keyTyped")) {
      e = parseKeyEvent(scanner, KEY_TYPED);
   } else if (stringEqual(name, "actionPerformed")) {
      e = parseActionEvent(scanner, ACTION_PERFORMED);
   } else if (stringEqual(name, "timerTicked")) {
      e = parseTimerEvent(scanner, TIMER_TICKED);
   } else if (stringEqual(name, "windowClosed")) {
      e = parseWindowEvent(scanner, WINDOW_CLOSED);
      closeGWindow(getGWindow(e));
   } else if (stringEqual(name, "windowResized")) {
      e = parseWindowEvent(scanner, WINDOW_RESIZED);
   } else if (stringEqual(name, "lastWindowClosed")) {
      exit(0);
   } else {
      /* Ignore for now */
   }
   freeTokenScanner(scanner);
   freeBlock(name);
   return e;
}

static GEvent parseMouseEvent(TokenScanner scanner, EventType type) {
   string id;
   int modifiers;
   double time, x, y;
   GEvent e;

   verifyToken(scanner, "(");
   id = scanString(scanner);
   verifyToken(scanner, ",");
   time = scanDouble(scanner);
   verifyToken(scanner, ",");
   modifiers = scanInt(scanner);
   verifyToken(scanner, ",");
   x = scanDouble(scanner);
   verifyToken(scanner, ",");
   y = scanDouble(scanner);
   verifyToken(scanner, ")");
   e = newGMouseEvent(type, get(windowTable, id), x, y);
   setEventTime(e, time);
   setModifiers(e, modifiers);
   freeBlock(id);
   return e;
}

static GEvent parseKeyEvent(TokenScanner scanner, EventType type) {
   string id;
   int modifiers, keyChar, keyCode;
   double time;
   GEvent e;

   verifyToken(scanner, "(");
   id = scanString(scanner);
   verifyToken(scanner, ",");
   time = scanDouble(scanner);
   verifyToken(scanner, ",");
   modifiers = scanInt(scanner);
   verifyToken(scanner, ",");
   keyChar = scanInt(scanner);
   verifyToken(scanner, ",");
   keyCode = scanInt(scanner);
   verifyToken(scanner, ")");
   e = newGKeyEvent(type, get(windowTable, id), keyChar, keyCode);
   setEventTime(e, time);
   setModifiers(e, modifiers);
   freeBlock(id);
   return e;
}

static GEvent parseTimerEvent(TokenScanner scanner, EventType type) {
   string id;
   double time;
   GEvent e;

   verifyToken(scanner, "(");
   id = scanString(scanner);
   verifyToken(scanner, ",");
   time = scanDouble(scanner);
   verifyToken(scanner, ")");
   e = newGTimerEvent(type, get(timerTable, id));
   setEventTime(e, time);
   freeBlock(id);
   return e;
}

static GEvent parseWindowEvent(TokenScanner scanner, EventType type) {
   string id;
   double time;
   GEvent e;

   verifyToken(scanner, "(");
   id = scanString(scanner);
   verifyToken(scanner, ",");
   time = scanDouble(scanner);
   verifyToken(scanner, ")");
   e = newGWindowEvent(type, get(windowTable, id));
   setEventTime(e, time);
   freeBlock(id);
   return e;
}

static GEvent parseActionEvent(TokenScanner scanner, EventType type) {
   string id, action;
   double time;
   GEvent e;

   verifyToken(scanner, "(");
   id = scanString(scanner);
   verifyToken(scanner, ",");
   action = scanString(scanner);
   verifyToken(scanner, ",");
   time = scanDouble(scanner);
   verifyToken(scanner, ")");
   e = newGActionEvent(type, get(sourceTable, id), action);
   setEventTime(e, time);
   freeBlock(id);
   freeBlock(action);
   return e;
}

static string scanString(TokenScanner scanner) {
   string token, result;

   token = nextToken(scanner);
   result = getStringValue(token);
   freeBlock(token);
   return result;
}

static int scanInt(TokenScanner scanner) {
   string token;
   int sign, result;

   token = nextToken(scanner);
   sign = 1;
   if (stringEqual(token, "-")) {
      sign = -1;
      freeBlock(token);
      token = nextToken(scanner);
   }
   result = stringToInteger(token);
   freeBlock(token);
   return sign * result;
}

static double scanDouble(TokenScanner scanner) {
   string token;
   double result;
   int sign;

   token = nextToken(scanner);
   sign = 1;
   if (stringEqual(token, "-")) {
      sign = -1;
      freeBlock(token);
      token = nextToken(scanner);
   }
   result = stringToReal(token);
   freeBlock(token);
   return sign * result;
}

static bool scanBool(TokenScanner scanner) {
   string token;
   bool result;

   token = nextToken(scanner);
   result = startsWith(token, "t");
   freeBlock(token);
   return result;
}

static string quotedString(string str) {
   StringBuffer sb;
   char ch;
   int i;

   sb = newStringBuffer();
   pushChar(sb, '"');
   for (i = 0; str[i] != '\0'; i++) {
      ch = str[i];
      switch (ch) {
       case '\a': sbprintf(sb, "\\a"); break;
       case '\b': sbprintf(sb, "\\b"); break;
       case '\f': sbprintf(sb, "\\f"); break;
       case '\n': sbprintf(sb, "\\n"); break;
       case '\r': sbprintf(sb, "\\r"); break;
       case '\t': sbprintf(sb, "\\t"); break;
       case '\v': sbprintf(sb, "\\v"); break;
       case '"':  sbprintf(sb, "\\%03o", ch); break;
       case '\\': sbprintf(sb, "\\\\"); break;
       default:
         if (isprint(ch)) {
            pushChar(sb, ch);
         } else {
            sbprintf(sb, "\\%03o", ch & 0xFF);
         }
      }
   }
   pushChar(sb, '"');
   str = copyString(getString(sb));
   freeStringBuffer(sb);
   return str;
}

static string boolString(bool flag) {
   return (flag) ? "true" : "false";
}

/* GWindow operations */

void createGWindowOp(GWindow gw, double width, double height, GObject top) {
   string id;

   initPlatform();
   id = getId(gw);
   put(windowTable, id, gw);
   freeBlock(id);
   putPipe("GWindow.create(\"0x%lX\", %g, %g, \"0x%lX\")",
           (long) gw, width, height, (long) top);
}

void deleteGWindowOp(GWindow gw) {
   string id;

   id = getId(gw);
   remove(windowTable, id);
   freeBlock(id);
   putPipe("GWindow.delete(\"0x%lX\")", (long) gw);
}

void closeGWindowOp(GWindow gw) {
   putPipe("GWindow.close(\"0x%lX\")", (long) gw);
}

void requestFocusOp(GWindow gw) {
   putPipe("GWindow.requestFocus(\"0x%lX\")", (long) gw);
}

void clearOp(GWindow gw) {
   putPipe("GWindow.clear(\"0x%lX\")", (long) gw);
}

void repaintOp(GWindow gw) {
   putPipe("GWindow.repaint(\"0x%lX\")", (long) gw);
}

void setVisibleGWindowOp(GWindow gw, bool flag) {
   putPipe("GWindow.setVisible(\"0x%lX\", %s)", (long) gw, boolString(flag));
}

void setResizableOp(GWindow gw, bool flag) {
   putPipe("GWindow.setResizable(\"0x%lX\", %s)", (long) gw, boolString(flag));
}

void setWindowTitleOp(GWindow gw, string title) {
   title = quotedString(title);
   putPipe("GWindow.setWindowTitle(\"0x%lX\", %s)", (long) gw, title);
   freeBlock(title);
}

void setRegionAlignmentOp(GWindow gw, string region, string align) {
   putPipe("GWindow.setRegionAlignment(\"0x%lX\", \"%s\", \"%s\")",
           (long) gw, region, align);
}

void addToRegionOp(GWindow gw, GObject gobj, string region) {
   putPipe("GWindow.addToRegion(\"0x%lX\", \"0x%lX\", \"%s\")",
           (long) gw, (long) gobj, region);
}

double getScreenWidthOp(void) {
   putPipe("GWindow.getScreenWidth()");
   return getDouble();
}

double getScreenHeightOp(void) {
   putPipe("GWindow.getScreenHeight()");
   return getDouble();
}

void exitGraphicsOp(void) {
   putPipe("GWindow.exitGraphics()");
   exit(0);
}

/* GTimer operations */

void createTimerOp(GTimer timer, double delay) {
   string id;

   id = getId(timer);
   putHashMap(timerTable, id, timer);
   freeBlock(id);
   putPipe("GTimer.create(\"0x%lX\", %g)", (long) timer, delay);
}

void deleteTimerOp(GTimer timer) {
   string id;

   id = getId(timer);
   putHashMap(timerTable, id, timer);
   freeBlock(id);
   putPipe("GTimer.deleteTimer(\"0x%lX\")", (long) timer);
}

void startTimerOp(GTimer timer) {
   putPipe("GTimer.startTimer(\"0x%lX\")", (long) timer);
}

void stopTimerOp(GTimer timer) {
   putPipe("GTimer.stopTimer(\"0x%lX\")", (long) timer);
}

void pauseOp(double milliseconds) {
   putPipe("GTimer.pause(%g)", milliseconds);
   getStatus();
}

/* Sound operations */

void createSoundOp(Sound sound, string filename) {
   filename = quotedString(filename);
   putPipe("Sound.create(\"0x%lX\", %s)", (long) sound, filename);
   freeBlock(filename);
   getStatus();
}

void deleteSoundOp(Sound sound) {
   putPipe("Sound.delete(\"0x%lX\")", (long) sound);
}

void playSoundOp(Sound sound) {
   putPipe("Sound.play(\"0x%lX\")", (long) sound);
}

/* GObject operations */

void deleteGObjectOp(GObject gobj) {
   putPipe("GObject.delete(\"0x%lX\")", (long) gobj);
}

void setVisibleGObjectOp(GObject gobj, bool flag) {
   putPipe("GObject.setVisible(\"0x%lX\", %s)", (long) gobj, boolString(flag));
}

void setColorOp(GObject gobj, string color) {
   putPipe("GObject.setColor(\"0x%lX\", \"%s\")", (long) gobj, color);
}

void setLocationOp(GObject gobj, double x, double y) {
   putPipe("GObject.setLocation(\"0x%lX\", %g, %g)", (long) gobj, x, y);
}

void drawOp(GWindow gw, GObject gobj) {
   putPipe("GWindow.draw(\"0x%lX\", \"0x%lX\")", (long) gw, (long) gobj);
}

void setSizeOp(GObject gobj, double width, double height) {
   putPipe("GObject.setSize(\"0x%lX\", %g, %g)", (long) gobj, width, height);
}

void setFrameRectangleOp(GObject gobj, double x, double y,
                                       double width, double height) {
   putPipe("GArc.setFrameRectangle(\"0x%lX\", %g, %g, %g, %g)",
           (long) gobj, x, y, width, height);
}

void setFilledOp(GObject gobj, bool flag) {
   putPipe("GObject.setFilled(\"0x%lX\", %s)", (long) gobj, boolString(flag));
}

void setFillColorOp(GObject gobj, string color) {
   putPipe("GObject.setFillColor(\"0x%lX\", \"%s\")", (long) gobj, color);
}

/* GRect operations */

void createGRectOp(GRect rect, double width, double height) {
   initPlatform();
   putPipe("GRect.create(\"0x%lX\", %g, %g)", (long) rect, width, height);
}

void createGRoundRectOp(GRoundRect rect, double width, double height,
                                         double corner) {
   initPlatform();
   putPipe("GRoundRect.create(\"0x%lX\", %g, %g, %g)", (long) rect,
                                                       width, height, corner);
}

void createG3DRectOp(G3DRect rect, double width, double height, bool raised) {
   initPlatform();
   putPipe("G3DRect.create(\"0x%lX\", %g, %g, %s)", (long) rect, width, height,
                                                    boolString(raised));
}

void setRaisedOp(G3DRect rect, bool raised) {
   putPipe("G3DRect.setRaised(\"0x%lX\", %s)", (long) rect,
                                               boolString(raised));
}

/* GOval operations */

void createGOvalOp(GOval oval, double width, double height) {
   initPlatform();
   putPipe("GOval.create(\"0x%lX\", %g, %g)", (long) oval, width, height);
}

/* GLine operations */

void createGLineOp(GLine line, double x0, double y0, double x1, double y1) {
   initPlatform();
   putPipe("GLine.create(\"0x%lX\", %g, %g, %g, %g)", (long) line,
                                                      x0, y0, x1, y1);
}

void setStartPointOp(GLine line, double x, double y) {
   putPipe("GLine.setStartPoint(\"0x%lX\", %g, %g)", (long) line, x, y);
}

void setEndPointOp(GLine line, double x, double y) {
   putPipe("GLine.setEndPoint(\"0x%lX\", %g, %g)", (long) line, x, y);
}

/* GArc operations */

void createGArcOp(GArc arc, double width, double height,
                            double start, double sweep) {
   putPipe("GArc.create(\"0x%lX\", %g, %g, %g, %g)", (long) arc, width, height,
                                                     start, sweep);
}

void setStartAngleOp(GArc arc, double angle) {
   putPipe("GArc.setStartAngle(\"0x%lX\", %g)", (long) arc, angle);
}

void setSweepAngleOp(GArc arc, double angle) {
   putPipe("GArc.setSweepAngle(\"0x%lX\", %g)", (long) arc, angle);
}

/* GLabel operations */

void createGLabelOp(GLabel label, string str) {
   str = quotedString(str);
   putPipe("GLabel.create(\"0x%lX\", %s)", (long) label, str);
   freeBlock(str);
}

void setFontOp(GLabel label, string font) {
   putPipe("GLabel.setFont(\"0x%lX\", \"%s\")", (long) label, font);
}

void setLabelOp(GLabel label, string str) {
   str = quotedString(str);
   putPipe("GLabel.setLabel(\"0x%lX\", %s)", (long) label, str);
   freeBlock(str);
}

double getFontAscentOp(GLabel label) {
   putPipe("GLabel.getFontAscent(\"0x%lX\")", (long) label);
   return getDouble();
}

double getFontDescentOp(GLabel label) {
   putPipe("GLabel.getFontDescent(\"0x%lX\")", (long) label);
   return getDouble();
}

GDimension getGLabelSizeOp(GLabel label) {
   putPipe("GLabel.getGLabelSize(\"0x%lX\")", (long) label);
   return getGDimension();
}

/* GImage operations */

GDimension createGImageOp(GObject gobj, string filename) {
   filename = quotedString(filename);
   putPipe("GImage.create(\"0x%lX\", %s)", (long) gobj, filename);
   freeBlock(filename);
   return getGDimension();
}

/* GPolygon operations */

void createGPolygonOp(GPolygon poly) {
   putPipe("GPolygon.create(\"0x%lX\")", (long) poly);
}

void addVertexOp(GPolygon poly, double x, double y) {
   putPipe("GPolygon.addVertex(\"0x%lX\", %g, %g)", (long) poly, x, y);
}

/* GCompound operations */

void createGCompoundOp(GCompound compound) {
   initPlatform();
   putPipe("GCompound.create(\"0x%lX\")", (long) compound);
}

void addOp(GCompound compound, GObject gobj) {
   putPipe("GCompound.add(\"0x%lX\", \"0x%lX\")", (long) compound,
                                                  (long) gobj);
}

void removeOp(GObject gobj) {
   putPipe("GObject.remove(\"0x%lX\")", (long) gobj);
}

void sendForwardOp(GObject gobj) {
   putPipe("GObject.sendForward(\"0x%lX\")", (long) gobj);
}

void sendToFrontOp(GObject gobj) {
   putPipe("GObject.sendToFront(\"0x%lX\")", (long) gobj);
}

void sendBackwardOp(GObject gobj) {
   putPipe("GObject.sendBackward(\"0x%lX\")", (long) gobj);
}

void sendToBackOp(GObject gobj) {
   putPipe("GObject.sendToBack(\"0x%lX\")", (long) gobj);
}

/* GEvent operations */

GEvent getNextEventOp(int mask) {
   if (isEmpty(eventQueue)) {
      putPipe("GEvent.getNextEvent(%d)", mask);
      getResult();
      if (isEmpty(eventQueue)) return NULL;
   }
   return dequeue(eventQueue);
}

GEvent waitForEventOp(int mask) {
   while (isEmpty(eventQueue)) {
      putPipe("GEvent.waitForEvent(%d)", mask);
      getResult();
   }
   return dequeue(eventQueue);
}

/* GInteractor operations */

void setActionCommandOp(GInteractor interactor, string cmd) {
   cmd = quotedString(cmd);
   putPipe("GInteractor.setActionCommand(\"0x%lX\", %s)", (long) interactor,
                                                          cmd);
   freeBlock(cmd);
}

GDimension getSizeOp(GInteractor interactor) {
   putPipe("GInteractor.getSize(\"0x%lX\")", (long) interactor);
   return getGDimension();
}

void createGButtonOp(GButton button, string label) {
   registerSource(button);
   label = quotedString(label);
   putPipe("GButton.create(\"0x%lX\", %s)", (long) button, label);
   freeBlock(label);
}

void createGCheckBoxOp(GCheckBox chkbox, string label) {
   registerSource(chkbox);
   label = quotedString(label);
   putPipe("GCheckBox.create(\"0x%lX\", %s)", (long) chkbox, label);
   freeBlock(label);
}

bool isSelectedOp(GCheckBox chkbox) {
   putPipe("GCheckBox.isSelected(\"0x%lX\")", (long) chkbox);
   return getBool();
}

void setSelectedOp(GCheckBox chkbox, bool state) {
   putPipe("GCheckBox.isSelected(\"0x%lX\", %s)", (long) chkbox,
                                                  boolString(state));
}

void createGSliderOp(GSlider slider, int min, int max, int value) {
   registerSource(slider);
   putPipe("GSlider.create(\"0x%lX\", %d, %d, %d)", (long) slider,
                                                    min, max, value);
}

int getValueOp(GSlider slider) {
   putPipe("GSlider.getValue(\"0x%lX\")", (long) slider);
   return getInt();
}

void setValueOp(GSlider slider, int value) {
   putPipe("GSlider.setValue(\"0x%lX\", %d)", (long) slider, value);
}

void createGTextFieldOp(GTextField field, int nChars) {
   registerSource(field);
   putPipe("GTextField.create(\"0x%lX\", %d)", (long) field, nChars);
}

string getTextOp(GTextField field) {
   putPipe("GTextField.getText(\"0x%lX\")", (long) field);
   return getResult();
}

void setTextOp(GTextField field, string str) {
   str = quotedString(str);
   putPipe("GTextField.getText(\"0x%lX\", %s)", (long) field, str);
   freeBlock(str);
}

void createGChooserOp(GChooser chooser) {
   registerSource(chooser);
   putPipe("GChooser.create(\"0x%lX\")", (long) chooser);
}

void addItemOp(GChooser chooser, string item) {
   item = quotedString(item);
   putPipe("GChooser.addItem(\"0x%lX\", %s)", (long) chooser, item);
   freeBlock(item);
}

string getSelectedItemOp(GChooser chooser) {
   putPipe("GChooser.getSelectedItem(\"0x%lX\")", (long) chooser);
   return getResult();
}

void setSelectedItemOp(GChooser chooser, string item) {
   item = quotedString(item);
   putPipe("GChooser.setSelectedItem(\"0x%lX\", %s)", (long) chooser, item);
   freeBlock(item);
}

static void registerSource(GInteractor interactor) {
   string id;

   id = getId(interactor);
   putHashMap(sourceTable, id, interactor);
   freeBlock(id);
}
