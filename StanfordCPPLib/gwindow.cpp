/*
 * File: gwindow.cpp
 * -----------------
 * This file implements the GWindow class, passing most calls directly
 * to the appropriate methods in the Platform class, which is implemented
 * separately for each architecture.
 * 
 * @version 2014/11/20
 * - added clearCanvas method
 * @version 2014/11/18
 * - added setResizable method
 * @version 2014/10/13
 * - added gwindowSetExitGraphicsEnabled function for autograders
 * - removed 'using namespace' statement
 */

#include "gwindow.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gtypes.h"
#include "map.h"
#include "platform.h"
#include "strlib.h"
#include "vector.h"

/* Constants */

// variables and functions for auditing calls to pause();
// used to facilitate creation of autograde programs
namespace autograder {
static int gwindow_pauses = 0;
static double gwindow_last_pauseMS = 0.0;
static bool gwindow_exitGraphics_enabled = true;
static bool gwindow_pause_enabled = true;

int gwindowGetNumPauses() {
    return gwindow_pauses;
}

void gwindowResetNumPauses() {
    gwindow_pauses = 0;
}

double gwindowGetLastPauseMS() {
    return gwindow_last_pauseMS;
}

void gwindowResetLastPauseMS() {
    gwindow_last_pauseMS = 0.0;
}

void gwindowSetExitGraphicsEnabled(bool value) {
    gwindow_exitGraphics_enabled = value;
}

void gwindowSetPauseEnabled(bool value) {
    gwindow_pause_enabled = value;
}
}

/* Private function prototypes */

static void initColorTable();
static std::string canonicalColorName(std::string str);

/*
 * Global variable: pp
 * -------------------
 * This variable points to a singleton of the Platform class.
 */

static Platform *pp = getPlatform();

/*
 * Global variable: colorTable
 * ---------------------------
 * This variable holds the translation table that maps colors into
 * their RGB values.  This color table is shared throughout the
 * application and cannot be manipulated by any clients.  If you
 * need to define color names for application specific colors, you
 * should do so by defining string constants with the appropriate
 * hexadecimal values, as in
 *
 *     const string MAGENTA = "0xFF00FF";
 */

static Map<std::string,int> colorTable;

GWindow::GWindow() {
    initGWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, true);
}

GWindow::GWindow(bool visible) {
    initGWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, visible);
}

GWindow::GWindow(double width, double height) {
    initGWindow(width, height, true);
}

GWindow::GWindow(double width, double height, bool visible) {
    initGWindow(width, height, visible);
}

void GWindow::initGWindow(double width, double height, bool visible) {
    gwd = new GWindowData();
    gwd->windowWidth = width;
    gwd->windowHeight = height;
    gwd->windowX = 0;
    gwd->windowY = 0;
    gwd->top = new GCompound();
    gwd->closed = false;
    gwd->exitOnClose = false;
    gwd->repaintImmediately = true;
    pp->gwindow_constructor(*this, width, height, gwd->top, visible);
    setColor("BLACK");
    setVisible(visible);
    pause(1000); // Temporary fix for race condition in back-end.
}

GWindow::~GWindow() {
    // commented out because for some reason it crashes to free this memory
    //   if (gwd) {
    //      if (gwd->top) {
    //         delete gwd->top;
    //         gwd->top = NULL;
    //      }
    //      delete gwd;
    //   }
}

void GWindow::close() {
    if (gwd) {
        gwd->visible = false;
        gwd->closed = true;
    }
    pp->gwindow_close(*this);
    pp->gwindow_delete(*this);
    if (gwd && gwd->exitOnClose) {
        // I was closed by the student's program.
        // I need to inform JBE so that it will shut down.
        exitGraphics();   // calls exit(0);
    }
}

void GWindow::notifyOfClose() {
    if (gwd) {
        gwd->visible = false;
        gwd->closed = true;
        if (gwd->exitOnClose) {
            // JBE notified me that I was closed by the user.
            // JBE is already going to shut itself down.
            // I just have to shut down the C++ process.
            exit(0);
        }
    }
}

std::string GWindow::getWindowData() const {
    std::ostringstream os;
    os << gwd;
    return os.str();
}

void GWindow::setExitOnClose(bool value) {
    if (!autograder::gwindow_exitGraphics_enabled) {
        return;
    }
    if (gwd) {
        gwd->exitOnClose = value;
    }
    pp->gwindow_setExitOnClose(*this, value);
}

bool GWindow::isRepaintImmediately() const {
    return !gwd || gwd->repaintImmediately;
}

void GWindow::setRepaintImmediately(bool value) {
    if (gwd) {
        gwd->repaintImmediately = value;
    }
}

bool GWindow::isOpen() const {
    return !gwd || !gwd->closed;
}

void GWindow::requestFocus() {
    if (isOpen()) {
        pp->gwindow_requestFocus(*this);
    }
}

void GWindow::clear() {
    if (isOpen()) {
        if (gwd && gwd->top) {
            gwd->top->removeAll();
        }
        pp->gwindow_clear(*this);
    }
}

void GWindow::clearCanvas() {
    if (isOpen()) {
        pp->gwindow_clearCanvas(*this);
    }
}

void GWindow::repaint() {
    if (isOpen()) {
        pp->gwindow_repaint(*this);
    }
}

void GWindow::setVisible(bool flag) {
    if (isOpen()) {
        if (gwd) {
            gwd->visible = flag;
        }
        // *** BUGBUG; commented out
        // pp->gwindow_setVisible(*this, flag);
    }
}

bool GWindow::isVisible() const {
    return gwd && gwd->visible;
}

void GWindow::drawLine(const GPoint & p0, const GPoint & p1) {
    if (isOpen()) {
        drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
    }
}

void GWindow::drawLine(double x0, double y0, double x1, double y1) {
    if (isOpen()) {
        GLine line(x0, y0, x1, y1);
        if (gwd) {
            line.setColor(gwd->color);
        }
        draw(line);
    }
}

GPoint GWindow::drawPolarLine(const GPoint & p0, double r, double theta) {
    return drawPolarLine(p0.getX(), p0.getY(), r, theta);
}

GPoint GWindow::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return GPoint(x1, y1);
}

void GWindow::drawRect(const GRectangle & bounds) {
    if (isOpen()) {
        drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(),
                 bounds.getHeight());
    }
}

void GWindow::drawRect(double x, double y, double width, double height) {
    if (isOpen()) {
        GRect rect(x, y, width, height);
        if (gwd) {
            rect.setColor(gwd->color);
        }
        draw(rect);
    }
}

void GWindow::fillRect(const GRectangle & bounds) {
    if (isOpen()) {
        fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(),
                 bounds.getHeight());
    }
}

void GWindow::fillRect(double x, double y, double width, double height) {
    if (isOpen()) {
        GRect rect(x, y, width, height);
        if (gwd) {
            rect.setColor(gwd->color);
        }
        rect.setFilled(true);
        draw(rect);
    }
}

void GWindow::drawOval(const GRectangle & bounds) {
    if (isOpen()) {
        drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(),
                 bounds.getHeight());
    }
}

void GWindow::drawOval(double x, double y, double width, double height) {
    if (isOpen()) {
        GOval oval(x, y, width, height);
        if (gwd) {
            oval.setColor(gwd->color);
        }
        draw(oval);
    }
}

void GWindow::fillOval(const GRectangle & bounds) {
    if (isOpen()) {
        fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(),
                 bounds.getHeight());
    }
}

void GWindow::fillOval(double x, double y, double width, double height) {
    if (isOpen()) {
        GOval oval(x, y, width, height);
        if (gwd) {
            oval.setColor(gwd->color);
        }
        oval.setFilled(true);
        draw(oval);
    }
}

void GWindow::setColor(std::string color) {
    setColor(convertColorToRGB(color));
}

void GWindow::setColor(int rgb) {
    if (gwd) {
        gwd->color = convertRGBToColor(rgb);
    }
}

std::string GWindow::getColor() const {
    return gwd->color;
}

double GWindow::getWidth() const {
    return gwd->windowWidth;
}

double GWindow::getHeight() const {
    return gwd->windowHeight;
}

GDimension GWindow::getSize() const {
    return pp->gwindow_getSize(*this);
}

GDimension GWindow::getCanvasSize() const {
    return GDimension(getWidth(), getHeight());
}

void GWindow::setSize(int width, int height) {
    if (isOpen()) {
        pp->gwindow_setSize(*this, width, height);
    }
}

void GWindow::pack() {
    if (isOpen()) {
        pp->gwindow_pack(*this);
    }
}

void GWindow::setCanvasSize(int width, int height) {
    if (isOpen()) {
        pp->gwindow_setCanvasSize(*this, width, height);
    }
}

void GWindow::setTitle(std::string title) {
    setWindowTitle(title);
}

void GWindow::setWindowTitle(std::string title) {
    if (isOpen()) {
        if (gwd) {
            gwd->windowTitle = title;
        }
        pp->gwindow_setTitle(*this, title);
    }
}

Point GWindow::getLocation() const {
    return pp->gwindow_getLocation(*this);
}

void GWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setLocation(int x, int y) {
    if (isOpen()) {
        if (gwd) {
            gwd->windowX = x;
            gwd->windowY = y;
        }
        pp->gwindow_setLocation(*this, x, y);
    }
}

void GWindow::setLocationSaved(bool value) {
    if (isOpen()) {
        pp->gwindow_setLocationSaved(*this, value);
    }
}

void GWindow::center() {
    // TODO: doesn't set gwd->windowX or windowY properly
    setLocation(CENTER_MAGIC_VALUE, CENTER_MAGIC_VALUE);
}

std::string GWindow::getWindowTitle() const {
    return gwd->windowTitle;
}

void GWindow::draw(const GObject& gobj) {
    if (isOpen()) {
        draw(&gobj);
    }
}

void GWindow::draw(GObject *gobj) {
    if (isOpen()) {
        if (!gwd || gwd->repaintImmediately) {
            pp->gwindow_draw(*this, gobj);
        } else {
            pp->gwindow_drawInBackground(*this, gobj);
        }
    }
}

void GWindow::draw(const GObject *gobj) {
    if (isOpen()) {
        if (!gwd || gwd->repaintImmediately) {
            pp->gwindow_draw(*this, gobj);
        } else {
            pp->gwindow_drawInBackground(*this, gobj);
        }
    }
}

void GWindow::draw(GObject & gobj, double x, double y) {
    if (isOpen()) {
        draw(&gobj, x, y);
    }
}

void GWindow::draw(GObject *gobj, double x, double y) {
    if (isOpen()) {
        gobj->setLocation(x, y);
        if (!gwd || gwd->repaintImmediately) {
            pp->gwindow_draw(*this, gobj);
        } else {
            pp->gwindow_drawInBackground(*this, gobj);
        }
    }
}

void GWindow::add(GObject *gobj) {
    if (isOpen()) {
        if (gwd) {
            gwd->top->add(gobj);
        }
    }
}

void GWindow::add(GObject *gobj, double x, double y) {
    if (isOpen()) {
        gobj->setLocation(x, y);
        add(gobj);
    }
}

void GWindow::addToRegion(GInteractor *gobj, std::string region) {
    if (isOpen()) {
        pp->gwindow_addToRegion(*this, (GObject *) gobj, region);
    }
}

void GWindow::addToRegion(GLabel *gobj, std::string region) {
    if (isOpen()) {
        pp->gwindow_addToRegion(*this, (GObject *) gobj, region);
    }
}

GDimension GWindow::getRegionSize(std::string region) const {
    return pp->gwindow_getRegionSize(*this, region);
}

void GWindow::removeFromRegion(GInteractor *gobj, std::string region) {
    if (isOpen()) {
        pp->gwindow_removeFromRegion(*this, (GObject *) gobj, region);
    }
}

void GWindow::removeFromRegion(GLabel *gobj, std::string region) {
    if (isOpen()) {
        pp->gwindow_removeFromRegion(*this, (GObject *) gobj, region);
    }
}

void GWindow::remove(GObject *gobj) {
    if (isOpen()) {
        if (gwd && gwd->top) {
            gwd->top->remove(gobj);
        }
    }
}

GObject *GWindow::getGObjectAt(double x, double y) const {
    if (gwd && gwd->top) {
        int n = gwd->top->getElementCount();
        for (int i = n - 1; i >= 0; i--) {
            GObject *gobj = gwd->top->getElement(i);
            if (gobj->contains(x, y)) return gobj;
        }
    }
    return NULL;
}

void GWindow::setRegionAlignment(std::string region, std::string align) {
    if (isOpen()) {
        pp->gwindow_setRegionAlignment(*this, region, align);
    }
}

void GWindow::setResizable(bool resizable) {
    if (isOpen()) {
        pp->gwindow_setResizable(*this, resizable);
    }
}

bool GWindow::operator==(GWindow w2) {
    return gwd == w2.gwd;
}

bool GWindow::operator!=(GWindow w2) {
    return gwd != w2.gwd;
}

GWindow::GWindow(GWindowData *gwd) {
    this->gwd = gwd;
}

void pause(double milliseconds) {
    if (autograder::gwindow_pause_enabled) {
        pp->gtimer_pause(milliseconds);
    }
    autograder::gwindow_pauses++;
    autograder::gwindow_last_pauseMS = milliseconds;
}

double getScreenWidth() {
    return pp->gwindow_getScreenWidth();
}

double getScreenHeight() {
    return pp->gwindow_getScreenHeight();   // BUGBUG: was returning getScreenWidth
}

GDimension getScreenSize() {
    return pp->gwindow_getScreenSize();
}

int convertColorToRGB(std::string colorName) {
    if (colorName == "") return -1;
    if (colorName[0] == '#') {
        std::istringstream is(colorName.substr(1) + "@");
        int rgb;
        char terminator = '\0';
        is >> std::hex >> rgb >> terminator;
        if (terminator != '@') error("convertColorToRGB: Illegal color - " + colorName);
        return rgb;
    }
    std::string name = canonicalColorName(colorName);
    if (colorTable.size() == 0) initColorTable();
    if (!colorTable.containsKey(name)) {
        error("convertColorToRGB: Undefined color - " + colorName);
    }
    return colorTable[name];
}

std::string convertRGBToColor(int rgb) {
    if (rgb == -1) return "";
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    return os.str();
}

void exitGraphics() {
    if (autograder::gwindow_exitGraphics_enabled) {
        pp->gwindow_exitGraphics();   // calls exit(0);
    }
}

static void initColorTable() {
    colorTable["black"] = 0x000000;
    colorTable["darkgray"] = 0x595959;
    colorTable["gray"] = 0x999999;
    colorTable["lightgray"] = 0xBFBFBF;
    colorTable["white"] = 0xFFFFFF;
    colorTable["red"] = 0xFF0000;
    colorTable["yellow"] = 0xFFFF00;
    colorTable["green"] = 0x00FF00;
    colorTable["cyan"] = 0x00FFFF;
    colorTable["blue"] = 0x0000FF;
    colorTable["magenta"] = 0xFF00FF;
    colorTable["orange"] = 0xFFC800;
    colorTable["pink"] = 0xFFAFAF;
}

static std::string canonicalColorName(std::string str) {
    std::string result = "";
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        char ch = str[i];
        if (!isspace(ch) && ch != '_') result += tolower(ch);
    }
    return result;
}
