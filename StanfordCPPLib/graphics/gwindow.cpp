/*
 * File: gwindow.cpp
 * -----------------
 * This file implements the GWindow class, passing most calls directly
 * to the appropriate methods in the Platform class, which is implemented
 * separately for each architecture.
 * 
 * @version 2016/10/18
 * - fixed alpha bugs in get/setPixels
 * - added get/setPixel[s]ARGB to retain transparency channel
 * @version 2016/10/16
 * - added get/setPixel[s], get/setWidth/Height
 * - alphabetized methods
 * @version 2016/10/08
 * - added toBack/Front
 * @version 2016/10/07
 * - added getCanvasWidth, getCanvasHeight methods
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/08/02
 * - added saveCanvasPixels method
 * - re-enabled setVisible(bool) method
 * @version 2016/07/22
 * - fixed autograder bug with exit() call in notifyOnClose function
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
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
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gtypes.h"
#include "map.h"
#include "strlib.h"
#include "vector.h"
#include "private/platform.h"
#include "private/static.h"

/* Constants */

// variables and functions for auditing calls to pause();
// used to facilitate creation of autograde programs
STATIC_VARIABLE_DECLARE(int, gwindowPauses, 0)
STATIC_VARIABLE_DECLARE(double, gwindowLastPauseMS, 0.0)
STATIC_VARIABLE_DECLARE(bool, gwindowExitGraphicsEnabled, true)
STATIC_VARIABLE_DECLARE(bool, gwindowPauseEnabled, true)

/* Private function prototypes */

static std::string canonicalColorName(const std::string& str);
static int fixAlpha(int argb);

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

static Map<std::string, int>& colorTable() {
    static Map<std::string, int> __colorTable;
    if (__colorTable.isEmpty()) {
        __colorTable["black"] = 0x000000;
        __colorTable["blue"] = 0x0000FF;
        __colorTable["brown"] = 0x926239;
        __colorTable["cyan"] = 0x00FFFF;
        __colorTable["darkgray"] = 0x595959;
        __colorTable["gray"] = 0x999999;
        __colorTable["green"] = 0x00FF00;
        __colorTable["lightgray"] = 0xBFBFBF;
        __colorTable["magenta"] = 0xFF00FF;
        __colorTable["orange"] = 0xFFC800;
        __colorTable["pink"] = 0xFFAFAF;
        __colorTable["purple"] = 0xFF00FF;
        __colorTable["red"] = 0xFF0000;
        __colorTable["white"] = 0xFFFFFF;
        __colorTable["yellow"] = 0xFFFF00;
    }
    return __colorTable;
}

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

GWindow::GWindow(GWindowData* gwd) {
    this->gwd = gwd;
}

void GWindow::initGWindow(double width, double height, bool visible) {
    gwd = new GWindowData();
    gwd->windowWidth = width;
    gwd->windowHeight = height;
    gwd->windowX = 0;
    gwd->windowY = 0;
    gwd->top = new GCompound();
    gwd->colorInt = 0;
    gwd->closed = false;
    gwd->visible = visible;
    gwd->resizable = false;
    gwd->exitOnClose = false;
    gwd->repaintImmediately = true;
    stanfordcpplib::getPlatform()->gwindow_constructor(*this, width, height, gwd->top, visible);
    setColor("BLACK");
    setVisible(visible);
    // pause(1000); // Temporary fix for race condition in back-end.
}

GWindow::~GWindow() {
    // commented out because for some reason it crashes to free this memory
    //   if (gwd) {
    //      if (gwd->top) {
    //         delete gwd->top;
    //         gwd->top = nullptr;
    //      }
    //      delete gwd;
    //      gwd = nullptr;
    //   }
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

void GWindow::addToRegion(GInteractor* gobj, const std::string& region) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_addToRegion(*this, (GObject *) gobj, region);
    }
}

void GWindow::addToRegion(GLabel* gobj, const std::string& region) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_addToRegion(*this, (GObject *) gobj, region);
    }
}

void GWindow::center() {
    setLocation(CENTER_MAGIC_VALUE, CENTER_MAGIC_VALUE);
}

void GWindow::clear() {
    if (isOpen()) {
        if (gwd && gwd->top) {
            gwd->top->removeAll();
        }
        stanfordcpplib::getPlatform()->gwindow_clear(*this);
    }
}

void GWindow::clearCanvas() {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_clearCanvas(*this);
    }
}

void GWindow::close() {
    if (gwd) {
        gwd->visible = false;
        gwd->closed = true;
    }
    stanfordcpplib::getPlatform()->gwindow_close(*this);
    stanfordcpplib::getPlatform()->gwindow_delete(*this);
    if (gwd && gwd->exitOnClose) {
        // I was closed by the student's program.
        // I need to inform JBE so that it will shut down.
        exitGraphics();   // calls exit(0);
    }
}

void GWindow::compareToImage(const std::string& filename, bool ignoreWindowSize) const {
    stanfordcpplib::getPlatform()->diffimage_compareWindowToImage(*this, filename, ignoreWindowSize);
}

void GWindow::draw(const GObject& gobj) {
    if (isOpen()) {
        draw(&gobj);
    }
}

void GWindow::draw(GObject* gobj) {
    if (isOpen()) {
        if (!gwd || gwd->repaintImmediately) {
            stanfordcpplib::getPlatform()->gwindow_draw(*this, gobj);
        } else {
            stanfordcpplib::getPlatform()->gwindow_drawInBackground(*this, gobj);
        }
    }
}

void GWindow::draw(const GObject* gobj) {
    if (isOpen()) {
        if (!gwd || gwd->repaintImmediately) {
            stanfordcpplib::getPlatform()->gwindow_draw(*this, gobj);
        } else {
            stanfordcpplib::getPlatform()->gwindow_drawInBackground(*this, gobj);
        }
    }
}

void GWindow::draw(GObject& gobj, double x, double y) {
    if (isOpen()) {
        draw(&gobj, x, y);
    }
}

void GWindow::draw(GObject* gobj, double x, double y) {
    if (isOpen()) {
        gobj->setLocation(x, y);
        if (!gwd || gwd->repaintImmediately) {
            stanfordcpplib::getPlatform()->gwindow_draw(*this, gobj);
        } else {
            stanfordcpplib::getPlatform()->gwindow_drawInBackground(*this, gobj);
        }
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

void GWindow::drawLine(const GPoint& p0, const GPoint& p1) {
    if (isOpen()) {
        drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
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

void GWindow::drawOval(const GRectangle& bounds) {
    if (isOpen()) {
        drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    }
}

void GWindow::drawPixel(double x, double y) {
    setPixel(x, y, getColorInt());
}

void GWindow::drawPixel(double x, double y, int color) {
    setPixel(x, y, color);
}

void GWindow::drawPixel(double x, double y, const std::string& color) {
    setPixel(x, y, convertColorToRGB(color));
}

GPoint GWindow::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return GPoint(x1, y1);
}

GPoint GWindow::drawPolarLine(const GPoint& p0, double r, double theta) {
    return drawPolarLine(p0.getX(), p0.getY(), r, theta);
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

void GWindow::drawRect(const GRectangle& bounds) {
    if (isOpen()) {
        drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
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

void GWindow::fillOval(const GRectangle & bounds) {
    if (isOpen()) {
        fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
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

void GWindow::fillRect(const GRectangle & bounds) {
    if (isOpen()) {
        fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    }
}

double GWindow::getCanvasHeight() const {
    return getCanvasSize().getHeight();
}

GDimension GWindow::getCanvasSize() const {
    GDimension size = stanfordcpplib::getPlatform()->gwindow_getContentPaneSize(*this);
    return GDimension(size.getWidth() - getRegionWidth("West") - getRegionWidth("East"),
                      size.getHeight() - getRegionHeight("North") - getRegionHeight("South"));
}

double GWindow::getCanvasWidth() const {
    return getCanvasSize().getWidth();
}

std::string GWindow::getColor() const {
    return gwd->color;
}

int GWindow::getColorInt() const {
    return gwd->colorInt;
}

GObject* GWindow::getGObjectAt(double x, double y) const {
    if (gwd && gwd->top) {
        int n = gwd->top->getElementCount();
        for (int i = n - 1; i >= 0; i--) {
            GObject* gobj = gwd->top->getElement(i);
            if (gobj->contains(x, y)) {
                return gobj;
            }
        }
    }
    return nullptr;
}

double GWindow::getHeight() const {
    if (!gwd || isResizable()) {
        // have to ask the back-end for height because the user might have
        // resized the window since its size was last set
        return getSize().getHeight();
    } else {
        return gwd->windowHeight;
    }
}

Point GWindow::getLocation() const {
    Point loc = stanfordcpplib::getPlatform()->gwindow_getLocation(*this);
    if (gwd) {
        gwd->windowX = loc.getX();
        gwd->windowY = loc.getY();
    }
    return loc;
}

// TODO: bounds-checking?
int GWindow::getPixel(double x, double y) const {
    int rgb = stanfordcpplib::getPlatform()->gwindow_getPixel(*this, (int) x, (int) y);
    rgb = rgb & 0x00ffffff;   // strip alpha component
    return rgb;
}

// TODO: bounds-checking?
int GWindow::getPixelARGB(double x, double y) const {
    int rgb = stanfordcpplib::getPlatform()->gwindow_getPixel(*this, (int) x, (int) y);
    rgb = fixAlpha(rgb);
    return rgb;
}

Grid<int> GWindow::getPixels() const {
    Grid<int> pixels = stanfordcpplib::getPlatform()->gwindow_getPixels(*this);
    for (int row = 0, rows = pixels.numRows(); row < rows; row++) {
        for (int col = 0, cols = pixels.numCols(); col < cols; col++) {
            pixels[row][col] = pixels[row][col] & 0x00ffffff;   // strip alpha component
        }
    }
    return pixels;
}

Grid<int> GWindow::getPixelsARGB() const {
    Grid<int> pixels = stanfordcpplib::getPlatform()->gwindow_getPixels(*this);
    for (int row = 0, rows = pixels.numRows(); row < rows; row++) {
        for (int col = 0, cols = pixels.numCols(); col < cols; col++) {
            pixels[row][col] = fixAlpha(pixels[row][col]);
        }
    }
    return pixels;
}

double GWindow::getRegionHeight(const std::string& region) const {
    return getRegionSize(region).getHeight();   // inefficient but oh well
}

GDimension GWindow::getRegionSize(const std::string& region) const {
    return stanfordcpplib::getPlatform()->gwindow_getRegionSize(*this, region);
}

double GWindow::getRegionWidth(const std::string& region) const {
    return getRegionSize(region).getWidth();   // inefficient but oh well
}

GDimension GWindow::getSize() const {
    if (!gwd || isResizable()) {
        // have to ask the back-end for size because the user might have
        // resized the window since its size was last set
        GDimension size = stanfordcpplib::getPlatform()->gwindow_getSize(*this);
        if (gwd) {
            gwd->windowWidth = size.getWidth();
            gwd->windowHeight = size.getHeight();
        }
        return size;
    } else {
        // can just use our locally cached width/height
        return GDimension(gwd->windowWidth, gwd->windowHeight);
    }
}

std::string GWindow::getTitle() const {
    return gwd->windowTitle;
}

double GWindow::getWidth() const {
    if (!gwd || isResizable()) {
        // have to ask the back-end for width because the user might have
        // resized the window since its size was last set
        return getSize().getWidth();
    } else {
        return gwd->windowWidth;
    }
}

std::string GWindow::getWindowData() const {
    std::ostringstream os;
    os << gwd;
    return os.str();
}

std::string GWindow::getWindowTitle() const {
    return gwd->windowTitle;
}

double GWindow::getX() const {
    return getLocation().getX();
}

double GWindow::getY() const {
    return getLocation().getX();
}

bool GWindow::inBounds(double x, double y) const {
    GDimension contentPaneSize = stanfordcpplib::getPlatform()->gwindow_getContentPaneSize(*this);
    return x >= 0 && x < contentPaneSize.getWidth()
            && y >= 0 && y < contentPaneSize.getHeight();
}

bool GWindow::inCanvasBounds(double x, double y) const {
    GDimension canvasSize = getCanvasSize();
    return x >= 0 && x < canvasSize.getWidth()
            && y >= 0 && y < canvasSize.getHeight();
}

bool GWindow::isOpen() const {
    return !gwd || !gwd->closed;
}

bool GWindow::isRepaintImmediately() const {
    return !gwd || gwd->repaintImmediately;
}

bool GWindow::isResizable() const {
    return gwd && gwd->resizable;
}

bool GWindow::isVisible() const {
    return gwd && gwd->visible;
}

void GWindow::notifyOfClose() {
    if (gwd) {
        gwd->visible = false;
        gwd->closed = true;
        if (gwd->exitOnClose) {
            // JBE notified me that I was closed by the user.
            // JBE is already going to shut itself down.
            // I just have to shut down the C++ process.
            std::exit(0);
        }
    }
}

void GWindow::pack() {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_pack(*this);
    }
}

void GWindow::remove(GObject* gobj) {
    if (isOpen()) {
        if (gwd && gwd->top) {
            gwd->top->remove(gobj);
        }
    }
}

void GWindow::removeFromRegion(GInteractor* gobj, const std::string& region) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_removeFromRegion(*this, (GObject*) gobj, region);
    }
}

void GWindow::removeFromRegion(GLabel* gobj, const std::string& region) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_removeFromRegion(*this, (GObject*) gobj, region);
    }
}

void GWindow::repaint() {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_repaint(*this);
    }
}

void GWindow::requestFocus() {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_requestFocus(*this);
    }
}

void GWindow::saveCanvasPixels(const std::string& filename) {
    stanfordcpplib::getPlatform()->gwindow_saveCanvasPixels(*this, filename);
}

void GWindow::setCanvasHeight(double height) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setCanvasSize(*this, (int) getCanvasWidth(), (int) height);
    }
}

void GWindow::setCanvasSize(double width, double height) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setCanvasSize(*this, (int) width, (int) height);
    }
}

void GWindow::setCanvasWidth(double width) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setCanvasSize(*this, (int) width, (int) getCanvasHeight());
    }
}

void GWindow::setColor(int rgb) {
    if (gwd) {
        gwd->color = convertRGBToColor(rgb);
        gwd->colorInt = rgb;
    }
}

void GWindow::setColor(const std::string& color) {
    if (gwd) {
        gwd->color = color;
        gwd->colorInt = convertColorToRGB(color);
    }
}

void GWindow::setExitOnClose(bool value) {
    if (!STATIC_VARIABLE(gwindowExitGraphicsEnabled)) {
        return;
    }
    if (gwd) {
        gwd->exitOnClose = value;
    }
    stanfordcpplib::getPlatform()->gwindow_setExitOnClose(*this, value);
}

void GWindow::setHeight(double height) {
    if (isOpen()) {
        GDimension size = getSize();
        stanfordcpplib::getPlatform()->gwindow_setSize(*this, (int) size.getWidth(), (int) height);
        if (gwd) {
            gwd->windowHeight = height;
        }
    }
}

void GWindow::setLocation(double x, double y) {
    if (isOpen()) {
        if (gwd) {
            gwd->windowX = x;
            gwd->windowY = y;
        }
        stanfordcpplib::getPlatform()->gwindow_setLocation(*this, (int) x, (int) y);
    }
}

void GWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setLocationSaved(bool value) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setLocationSaved(*this, value);
    }
}

// TODO: bounds-checking?
void GWindow::setPixel(double x, double y, int rgb) {
    rgb = fixAlpha(rgb);
    stanfordcpplib::getPlatform()->gwindow_setPixel(*this, (int) x, (int) y, rgb, gwd->repaintImmediately);
}

// TODO: bounds-checking?
void GWindow::setPixelARGB(double x, double y, int argb) {
    stanfordcpplib::getPlatform()->gwindow_setPixel(*this, (int) x, (int) y, argb, gwd->repaintImmediately);
}

void GWindow::setPixels(const Grid<int>& pixels) {
    // TODO: fixAlpha on all pixels?
    stanfordcpplib::getPlatform()->gwindow_setPixels(*this, pixels);
}

void GWindow::setPixelsARGB(const Grid<int>& pixelsARGB) {
    stanfordcpplib::getPlatform()->gwindow_setPixels(*this, pixelsARGB);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setRegionAlignment(*this, region, align);
    }
}

void GWindow::setRepaintImmediately(bool value) {
    if (gwd) {
        gwd->repaintImmediately = value;
    }
}

void GWindow::setResizable(bool resizable) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setResizable(*this, resizable);
    }
}

void GWindow::setSize(double width, double height) {
    if (isOpen()) {
        stanfordcpplib::getPlatform()->gwindow_setSize(*this, (int) width, (int) height);
        if (gwd) {
            gwd->windowWidth = width;
            gwd->windowHeight = height;
        }
    }
}

void GWindow::setTitle(const std::string& title) {
    setWindowTitle(title);
}

void GWindow::setVisible(bool flag) {
    if (isOpen()) {
        if (gwd) {
            gwd->visible = flag;
        }
        stanfordcpplib::getPlatform()->gwindow_setVisible(*this, flag);
    }
}

void GWindow::setWidth(double width) {
    if (isOpen()) {
        GDimension size = getSize();
        stanfordcpplib::getPlatform()->gwindow_setSize(*this, (int) width, (int) size.getHeight());
        if (gwd) {
            gwd->windowWidth = width;
        }
    }
}

void GWindow::setWindowTitle(const std::string& title) {
    if (isOpen()) {
        if (gwd) {
            gwd->windowTitle = title;
        }
        stanfordcpplib::getPlatform()->gwindow_setTitle(*this, title);
    }
}

void GWindow::setX(double x) {
    setLocation(x, getY());
}

void GWindow::setY(double y) {
    setLocation(getX(), y);
}

void GWindow::toBack() {
    stanfordcpplib::getPlatform()->gwindow_toBack(*this);
}

void GWindow::toFront() {
    stanfordcpplib::getPlatform()->gwindow_toFront(*this);
}

bool GWindow::operator ==(const GWindow& w2) {
    return gwd == w2.gwd;
}

bool GWindow::operator !=(const GWindow& w2) {
    return gwd != w2.gwd;
}


// free functions

static std::string canonicalColorName(const std::string& str) {
    std::string result = "";
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        char ch = str[i];
        if (!isspace(ch) && ch != '_') result += tolower(ch);
    }
    return result;
}

int convertColorToRGB(const std::string& colorName) {
    if (colorName == "") return -1;
    if (colorName[0] == '#') {
        std::istringstream is(colorName.substr(1) + "@");
        int rgb;
        char terminator = '\0';
        is >> std::hex >> rgb >> terminator;
        if (terminator != '@') {
            error("convertColorToRGB: Illegal color - " + colorName);
        }
        return rgb;
    }
    std::string name = canonicalColorName(colorName);
    if (!colorTable().containsKey(name)) {
        error("convertColorToRGB: Undefined color - " + colorName);
    }
    return colorTable()[name];
}

std::string convertRGBToColor(int rgb) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    return os.str();
}

void exitGraphics() {
    if (STATIC_VARIABLE(gwindowExitGraphicsEnabled)) {
        stanfordcpplib::getPlatform()->gwindow_exitGraphics();   // calls exit(0);
    }
}

// if RGB is not completely black, but alpha is 0, assume that the
// client meant to use an opaque color and add ff as alpha channel
static int fixAlpha(int argb) {
    int alpha = ((argb & 0xff000000) >> 24) & 0x000000ff;
    if (alpha == 0 && (argb & 0x00ffffff) != 0) {
        argb = argb | 0xff000000;   // set full 255 alpha
    }
    return argb;
}

double getScreenHeight() {
    return stanfordcpplib::getPlatform()->gwindow_getScreenHeight();   // BUGBUG: was returning getScreenWidth
}

GDimension getScreenSize() {
    return stanfordcpplib::getPlatform()->gwindow_getScreenSize();
}

double getScreenWidth() {
    return stanfordcpplib::getPlatform()->gwindow_getScreenWidth();
}

double gwindowGetLastPauseMS() {
    return STATIC_VARIABLE(gwindowLastPauseMS);
}

int gwindowGetNumPauses() {
    return STATIC_VARIABLE(gwindowPauses);
}

void gwindowResetLastPauseMS() {
    STATIC_VARIABLE(gwindowLastPauseMS) = 0.0;
}

void gwindowResetNumPauses() {
    STATIC_VARIABLE(gwindowPauses) = 0;
}

void gwindowSetExitGraphicsEnabled(bool value) {
    STATIC_VARIABLE(gwindowExitGraphicsEnabled) = value;
}

void gwindowSetPauseEnabled(bool value) {
    STATIC_VARIABLE(gwindowPauseEnabled) = value;
}

void pause(double milliseconds) {
    if (STATIC_VARIABLE(gwindowPauseEnabled)) {
        stanfordcpplib::getPlatform()->gtimer_pause(milliseconds);
    }
    STATIC_VARIABLE(gwindowPauses)++;
    STATIC_VARIABLE(gwindowLastPauseMS) = milliseconds;
}

// end free functions


// some flag stuff for error reporting on Windows
#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#  include <windows.h>
#  define MSC_ERROR_FLAGS (MB_OK | MB_ICONSTOP | MB_TOPMOST)
#endif
