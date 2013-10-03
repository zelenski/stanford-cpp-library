/*
 * File: gwindow.cpp
 * -----------------
 * This file implements the GWindow class, passing most calls directly
 * to the appropriate methods in the Platform class, which is implemented
 * separately for each architecture.
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gevents.h"
#include "gobjects.h"
#include "gmath.h"
#include "gtypes.h"
#include "gwindow.h"
#include "map.h"
#include "strlib.h"
#include "vector.h"
#include "platform.h"
using namespace std;

/* Constants */

static const int DEFAULT_WIDTH = 500;
static const int DEFAULT_HEIGHT = 300;

/* Private function prototypes */

static void initColorTable();
static string canonicalColorName(string str);

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

static Map<string,int> colorTable;

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
   gwd->top = new GCompound();
   pp->createGWindow(*this, width, height, gwd->top);
   setColor("BLACK");
   setVisible(visible);
   pause(1000); // Temporary fix for race condition in back-end.
}

GWindow::~GWindow() {
   /* Empty */
}

void GWindow::close() {
   pp->close(*this);
   pp->deleteGWindow(*this);
}

void GWindow::requestFocus() {
   pp->requestFocus(*this);
}

void GWindow::clear() {
   gwd->top->removeAll();
   pp->clear(*this);
}

void GWindow::repaint() {
   pp->repaint(*this);
}

void GWindow::setVisible(bool flag) {
   gwd->visible = flag;
   pp->setVisible(*this, flag);
}

bool GWindow::isVisible() {
   return gwd->visible;
}

void GWindow::drawLine(const GPoint & p0, const GPoint & p1) {
   drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
}

void GWindow::drawLine(double x0, double y0, double x1, double y1) {
   GLine line(x0, y0, x1, y1);
   line.setColor(gwd->color);
   draw(line);
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
   drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                          bounds.getHeight());
}

void GWindow::drawRect(double x, double y, double width, double height) {
   GRect rect(x, y, width, height);
   rect.setColor(gwd->color);
   draw(rect);
}

void GWindow::fillRect(const GRectangle & bounds) {
   fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                          bounds.getHeight());
}

void GWindow::fillRect(double x, double y, double width, double height) {
   GRect rect(x, y, width, height);
   rect.setColor(gwd->color);
   rect.setFilled(true);
   draw(rect);
}

void GWindow::drawOval(const GRectangle & bounds) {
   drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                          bounds.getHeight());
}

void GWindow::drawOval(double x, double y, double width, double height) {
   GOval oval(x, y, width, height);
   oval.setColor(gwd->color);
   draw(oval);
}

void GWindow::fillOval(const GRectangle & bounds) {
   fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                          bounds.getHeight());
}

void GWindow::fillOval(double x, double y, double width, double height) {
   GOval oval(x, y, width, height);
   oval.setColor(gwd->color);
   oval.setFilled(true);
   draw(oval);
}

void GWindow::setColor(string color) {
   setColor(convertColorToRGB(color));
}

void GWindow::setColor(int rgb) {
   gwd->color = convertRGBToColor(rgb);
}

string GWindow::getColor() {
   return gwd->color;
}

double GWindow::getWidth() {
   return gwd->windowWidth;
}

double GWindow::getHeight() {
   return gwd->windowHeight;
}

void GWindow::setWindowTitle(string title) {
   gwd->windowTitle = title;
   pp->setWindowTitle(*this, title);
}

string GWindow::getWindowTitle() {
   return gwd->windowTitle;
}

void GWindow::draw(const GObject & gobj) {
   draw(&gobj);
}

void GWindow::draw(GObject *gobj) {
   pp->draw(*this, gobj);
}

void GWindow::draw(const GObject *gobj) {
   pp->draw(*this, gobj);
}

void GWindow::draw(GObject & gobj, double x, double y) {
   draw(&gobj, x, y);
}

void GWindow::draw(GObject *gobj, double x, double y) {
   gobj->setLocation(x, y);
   pp->draw(*this, gobj);
}

void GWindow::add(GObject *gobj) {
   gwd->top->add(gobj);
}

void GWindow::add(GObject *gobj, double x, double y) {
   gobj->setLocation(x, y);
   add(gobj);
}

void GWindow::addToRegion(GInteractor *gobj, string region) {
   pp->addToRegion(*this, (GObject *) gobj, region);
}

void GWindow::addToRegion(GLabel *gobj, string region) {
   pp->addToRegion(*this, (GObject *) gobj, region);
}

void GWindow::removeFromRegion(GInteractor *gobj, string region) {
   pp->removeFromRegion(*this, (GObject *) gobj, region);
}

void GWindow::removeFromRegion(GLabel *gobj, string region) {
   pp->removeFromRegion(*this, (GObject *) gobj, region);
}

void GWindow::remove(GObject *gobj) {
   gwd->top->remove(gobj);
}

GObject *GWindow::getGObjectAt(double x, double y) {
   int n = gwd->top->getElementCount();
   for (int i = n - 1; i >= 0; i--) {
      GObject *gobj = gwd->top->getElement(i);
      if (gobj->contains(x, y)) return gobj;
   }
   return NULL;
}

void GWindow::setRegionAlignment(string region, string align) {
   pp->setRegionAlignment(*this, region, align);
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
   pp->pause(milliseconds);
}

double getScreenWidth() {
   return pp->getScreenWidth();
}

double getScreenHeight() {
   return pp->getScreenWidth();
}

int convertColorToRGB(string colorName) {
   if (colorName == "") return -1;
   if (colorName[0] == '#') {
      istringstream is(colorName.substr(1) + "@");
      int rgb;
      char terminator = '\0';
      is >> hex >> rgb >> terminator;
      if (terminator != '@') error("setColor: Illegal color - " + colorName);
      return rgb;
   }
   string name = canonicalColorName(colorName);
   if (colorTable.size() == 0) initColorTable();
   if (!colorTable.containsKey(name)) {
      error("setColor: Undefined color - " + colorName);
   }
   return colorTable[name];
}

string convertRGBToColor(int rgb) {
   if (rgb == -1) return "";
   ostringstream os;
   os << hex << setfill('0') << uppercase << "#";
   os << setw(2) << (rgb >> 16 & 0xFF);
   os << setw(2) << (rgb >> 8 & 0xFF);
   os << setw(2) << (rgb & 0xFF);
   return os.str();
}

void exitGraphics() {
   pp->exitGraphics();
   exit(0);
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

static string canonicalColorName(string str) {
   string result = "";
   int nChars = str.length();
   for (int i = 0; i < nChars; i++) {
      char ch = str[i];
      if (!isspace(ch) && ch != '_') result += tolower(ch);
   }
   return result;
}
