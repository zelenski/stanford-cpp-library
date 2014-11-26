/*
 * File: platform.h
 * ----------------
 * This file defines the <code>Platform</code> class, which encapsulates
 * the platform-specific parts of the StanfordCPPLib package.  This file is
 * logically part of the implementation and is not interesting to clients.
 *
 * @version 2014/11/20
 * - added gwindow clearCanvas method
 * @version 2014/11/15
 * - improvements to autograder unit test GUI
 * @version 2014/11/14
 * - added method to set unit test runtime in MS
 * @version 2014/10/31
 * - added functions for graphical autograder unit tests
 * - added setVisible for autograder input panel
 * @version 2014/10/08
 * - removed dependency on using namespace statement
 * - filelib_getTempDirectory
 * - gbufferedimage_constructor, gbufferedimage_resize fixed
 * - GOptionPane functions implemented
 * - url_download implemented
 */

#ifndef _platform_h
#define _platform_h

#include <string>
#include <vector>
#include "gevents.h"
#include "gwindow.h"
#include "point.h"
#include "sound.h"

class Platform {
private:
    Platform();
    friend Platform *getPlatform();

public:
    virtual ~Platform();
    void autograderinput_addButton(std::string text, std::string input = "");
    void autograderinput_addCategory(std::string name);
    void autograderinput_removeButton(std::string text);
    void autograderinput_removeCategory(std::string name);
    void autograderinput_setVisible(bool value = true);
    void autograderunittest_addTest(const std::string& testName, const std::string& category = "", bool styleCheck = false);
    bool autograderunittest_isChecked(const std::string& testName);
    void autograderunittest_clearTests(bool styleCheck = false);
    void autograderunittest_clearTestResults(bool styleCheck = false);
    void autograderunittest_setChecked(const std::string& testName, bool checked = true);
    void autograderunittest_setTestCounts(int passCount, int testCount, bool styleCheck = false);
    void autograderunittest_setTestDetails(const std::string& testName, const std::string& details, bool styleCheck = false);
    void autograderunittest_setTestingCompleted(bool completed = true, bool styleCheck = false);
    void autograderunittest_setTestResult(const std::string& testName, const std::string& result, bool styleCheck = false);
    void autograderunittest_setTestRuntime(const std::string& testName, int runtimeMS);
    void autograderunittest_setVisible(bool visible = true, bool styleCheck = false);
    void autograderunittest_setWindowDescriptionText(const std::string& text, bool styleCheck = false);
    std::string cpplib_getCppLibraryVersion();
    std::string cpplib_getJavaBackEndVersion();
    void cpplib_setCppLibraryVersion();
    std::string file_openFileDialog(std::string title, std::string mode, std::string path);
    void filelib_createDirectory(std::string path);
    std::string filelib_expandPathname(std::string filename);
    bool filelib_fileExists(std::string filename);
    std::string filelib_getCurrentDirectory();
    std::string filelib_getDirectoryPathSeparator();
    std::string filelib_getSearchPathSeparator();
    std::string filelib_getTempDirectory();
    bool filelib_isDirectory(std::string filename);
    bool filelib_isFile(std::string filename);
    bool filelib_isSymbolicLink(std::string filename);
    void filelib_listDirectory(std::string path, std::vector<std::string>& list);
    void filelib_setCurrentDirectory(std::string path);
    void g3drect_constructor(GObject* gobj, double width, double height, bool raised);
    void g3drect_setRaised(GObject* gobj, bool raised);
    void garc_constructor(GObject* gobj, double width, double height, double start, double sweep);
    void garc_setFrameRectangle(GObject* gobj, double x, double y, double width, double height);
    void garc_setStartAngle(GObject* gobj, double angle);
    void garc_setSweepAngle(GObject* gobj, double angle);
    void gbufferedimage_constructor(GObject* gobj, double x, double y, double width, double height, int rgb);
    void gbufferedimage_fill(GObject* gobj, int rgb);
    void gbufferedimage_fillRegion(GObject* gobj, double x, double y, double width, double height, int rgb);
    std::string gbufferedimage_load(GObject* gobj, const std::string& filename);
    void gbufferedimage_resize(GObject* gobj, double width, double height, bool retain = true);
    std::string gbufferedimage_save(const GObject* const gobj, const std::string& filename);
    void gbufferedimage_setRGB(GObject* gobj, double x, double y, int rgb);
    void gbutton_constructor(GObject* gobj, std::string label);
    void gcheckbox_constructor(GObject* gobj, std::string label);
    bool gcheckbox_isSelected(GObject* gobj);
    void gcheckbox_setSelected(GObject* gobj, bool state);
    void gchooser_addItem(GObject* gobj, std::string item);
    void gchooser_constructor(GObject* gobj);
    std::string gchooser_getSelectedItem(GObject* gobj);
    void gchooser_setSelectedItem(GObject* gobj, std::string item);
    void gcompound_add(GObject* compound, GObject* gobj);
    void gcompound_constructor(GObject* gobj);
    GEvent gevent_getNextEvent(int mask);
    GEvent gevent_waitForEvent(int mask);
    std::string gfilechooser_showOpenDialog(std::string currentDir);
    std::string gfilechooser_showSaveDialog(std::string currentDir);
    GDimension gimage_constructor(GObject* gobj, std::string filename);
    GDimension ginteractor_getSize(GObject* gobj);
    bool ginteractor_isEnabled(GObject* gint);
    void ginteractor_setActionCommand(GObject* gobj, std::string cmd);
    void ginteractor_setEnabled(GObject* gint, bool value);
    void ginteractor_setIcon(GObject* gobj, std::string filename);
    void ginteractor_setTextPosition(GObject* gobj, int horizontal, int vertical);
    void glabel_constructor(GObject* gobj, std::string label);
    double glabel_getFontAscent(const GObject* gobj);
    double glabel_getFontDescent(const GObject* gobj);
    GDimension glabel_getSize(const GObject* gobj);
    void glabel_setFont(GObject* gobj, std::string font);
    void glabel_setLabel(GObject* gobj, std::string str);
    void gline_constructor(GObject* gobj, double x1, double y1, double x2, double y2);
    void gline_setEndPoint(GObject* gobj, double x, double y);
    void gline_setStartPoint(GObject* gobj, double x, double y);
    bool gobject_contains(const GObject* gobj, double x, double y);
    void gobject_delete(GObject* gobj);
    GRectangle gobject_getBounds(const GObject* gobj);
    void gobject_remove(GObject* gobj);
    void gobject_rotate(GObject* gobj, double theta);
    void gobject_scale(GObject* gobj, double sx, double sy);
    void gobject_sendBackward(GObject* gobj);
    void gobject_sendForward(GObject* gobj);
    void gobject_sendToBack(GObject* gobj);
    void gobject_sendToFront(GObject* gobj);
    void gobject_setAntialiasing(bool value);
    void gobject_setColor(GObject* gobj, std::string color);
    void gobject_setFillColor(GObject* gobj, std::string color);
    void gobject_setFilled(GObject* gobj, bool flag);
    void gobject_setLineWidth(GObject* gobj, double lineWidth);
    void gobject_setLocation(GObject* gobj, double x, double y);
    void gobject_setSize(GObject* gobj, double width, double height);
    void gobject_setVisible(GObject* gobj, bool flag);
    int goptionpane_showConfirmDialog(std::string message, std::string title, int type);
    std::string goptionpane_showInputDialog(std::string message, std::string title);
    void goptionpane_showMessageDialog(std::string message, std::string title, int type);
    int goptionpane_showOptionDialog(std::string message,
                                     std::string title,
                                     const std::vector<std::string>& options,
                                     std::string initiallySelected);
    void goptionpane_showTextFileDialog(std::string message, std::string title, int rows, int cols);
    void goval_constructor(GObject* gobj, double width, double height);
    void gpolygon_addVertex(GObject* gobj, double x, double y);
    void gpolygon_constructor(GObject* gobj);
    void grect_constructor(GObject* gobj, double width, double height);
    void groundrect_constructor(GObject* gobj, double width, double height, double corner);
    void gslider_constructor(GObject* gobj, int min, int max, int value);
    int gslider_getMajorTickSpacing(const GObject* gobj);
    int gslider_getMinorTickSpacing(const GObject* gobj);
    bool gslider_getPaintLabels(const GObject* gobj);
    bool gslider_getPaintTicks(const GObject* gobj);
    bool gslider_getSnapToTicks(const GObject* gobj);
    int gslider_getValue(const GObject* gobj);
    void gslider_setMajorTickSpacing(GObject* gobj, int value);
    void gslider_setMinorTickSpacing(GObject* gobj, int value);
    void gslider_setPaintLabels(GObject* gobj, bool value);
    void gslider_setPaintTicks(GObject* gobj, bool value);
    void gslider_setSnapToTicks(GObject* gobj, bool value);
    void gslider_setValue(GObject* gobj, int value);
    void gtextfield_constructor(GObject* gobj, int nChars);
    std::string gtextfield_getText(GObject* gobj);
    bool gtextfield_isEditable(const GObject* gobj);
    void gtextfield_setEditable(GObject* gobj, bool value);
    void gtextfield_setText(GObject* gobj, std::string str);
    void gtimer_constructor(const GTimer& timer, double delay);
    void gtimer_delete(const GTimer& timer);
    void gtimer_pause(double milliseconds);
    void gtimer_start(const GTimer& timer);
    void gtimer_stop(const GTimer& timer);
    void gwindow_addToRegion(const GWindow& gw, GObject* gobj, std::string region);
    void gwindow_clear(const GWindow& gw);
    void gwindow_clearCanvas(const GWindow& gw);
    void gwindow_close(const GWindow& gw);
    void gwindow_constructor(const GWindow& gw, double width, double height,
                             GObject* topCompound, bool visible = true);
    void gwindow_delete(const GWindow& gw);
    void gwindow_draw(const GWindow& gw, const GObject* gobj);
    void gwindow_drawInBackground(const GWindow& gw, const GObject* gobj);
    void gwindow_exitGraphics(bool abortBlockedConsoleIO = true);
    GDimension gwindow_getCanvasSize(const GWindow& gw);
    Point gwindow_getLocation(const GWindow& gw);
    GDimension gwindow_getRegionSize(const GWindow& gw, std::string region);
    double gwindow_getScreenHeight();
    GDimension gwindow_getScreenSize();
    double gwindow_getScreenWidth();
    GDimension gwindow_getSize(const GWindow& gw);
    void gwindow_minimize(const GWindow& gw);
    void gwindow_pack(const GWindow& gw);
    void gwindow_removeFromRegion(const GWindow& gw, GObject* gobj, std::string region);
    void gwindow_repaint(const GWindow& gw);
    void gwindow_requestFocus(const GWindow& gw);
    void gwindow_setCanvasSize(const GWindow& gw, int width, int height);
    void gwindow_setExitOnClose(const GWindow& gw, bool value);
    void gwindow_setLocation(const GWindow& gw, int x, int y);
    void gwindow_setLocationSaved(const GWindow& gw, bool value);
    void gwindow_setRegionAlignment(const GWindow& gw, std::string region, std::string align);
    void gwindow_setResizable(const GWindow& gw, bool value);
    void gwindow_setSize(const GWindow& gw, int width, int height);
    void gwindow_setTitle(const GWindow& gw, std::string title);
    void gwindow_setVisible(const GWindow& gw, bool flag);
    void gwindow_toFront(const GWindow& gw);
    void jbeconsole_clear();
    bool jbeconsole_isBlocked();
    void jbeconsole_minimize();
    void jbeconsole_setCloseOperation(int value);
    void jbeconsole_setExitProgramOnClose(bool value);
    void jbeconsole_setLocationSaved(bool value);
    void jbeconsole_setFont(const std::string& font);
    void jbeconsole_setLocation(int x, int y);
    void jbeconsole_setSize(double width, double height);
    void jbeconsole_setTitle(const std::string& title);
    void jbeconsole_setVisible(bool value);
    void jbeconsole_toFront();
    std::string os_getLastError();
    bool regex_match(std::string s, std::string regexp);
    int regex_matchCount(std::string s, std::string regexp);
    int regex_matchCountWithLines(std::string s, std::string regexp, std::string& linesOut);
    std::string regex_replace(std::string s, std::string regexp, std::string replacement, int limit = -1);
    void setStackSize(unsigned int stackSize);
    void sound_constructor(Sound *sound, std::string filename);
    void sound_delete(Sound *sound);
    void sound_play(Sound *sound);
    int url_download(std::string url, std::string filename);
};

Platform *getPlatform();

#endif
