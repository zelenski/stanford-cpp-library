/*
 * File: platform.h
 * ----------------
 * This file defines the <code>Platform</code> class, which encapsulates
 * the platform-specific parts of the StanfordCPPLib package.  This file is
 * logically part of the implementation and is not interesting to clients.
 *
 * @version 2016/11/25
 * - added clipboard_get/set
 * - added gtable_setCell/Column/RowFont
 * - added gwindow_setCloseOperation
 * @version 2016/11/18
 * - added gtable_setEditorValue, gtable_setColumnHeadersVisible
 * @version 2016/10/23
 * - added ginteractor_add/removeActionListener, requestFocus
 * @version 2016/10/16
 * - added gwindow_getPixels
 * @version 2016/10/15
 * - changed all string param to const string&; added gtextfield_setPlaceholder
 * @version 2016/10/13
 * - added exitEnabled / setExitEnabled
 * @version 2016/10/08
 * - added gwindow_toBack/Front
 * @version 2016/09/27
 * - added ginteractor_setText method
 * @version 2016/09/26
 * - added Note playing methods
 * @version 2016/08/02
 * - added DiffImage methods
 * - added GWindow savePixels method
 * @version 2015/11/07
 * - added GTable back-end methods
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

namespace stanfordcpplib {
class Platform {
private:
    Platform();
    friend Platform* getPlatform();

public:
    virtual ~Platform();
    void autograderinput_addButton(const std::string& text, const std::string& input = "");
    void autograderinput_addCategory(const std::string& name);
    void autograderinput_removeButton(const std::string& text);
    void autograderinput_removeCategory(const std::string& name);
    void autograderinput_setVisible(bool value = true);

    void autograderunittest_addTest(const std::string& testName, const std::string& category = "", bool styleCheck = false);
    bool autograderunittest_catchExceptions();
    void autograderunittest_clearTests(bool styleCheck = false);
    void autograderunittest_clearTestResults(bool styleCheck = false);
    bool autograderunittest_isChecked(const std::string& testFullName);
    void autograderunittest_setChecked(const std::string& testFullName, bool checked = true);
    void autograderunittest_setTestCounts(int passCount, int testCount, bool styleCheck = false);
    void autograderunittest_setTestDetails(const std::string& testFullName, const std::string& details, bool styleCheck = false);
    void autograderunittest_setTestingCompleted(bool completed = true, bool styleCheck = false);
    void autograderunittest_setTestResult(const std::string& testFullName, const std::string& result, bool styleCheck = false);
    void autograderunittest_setTestRuntime(const std::string& testFullName, int runtimeMS);
    void autograderunittest_setVisible(bool visible = true, bool styleCheck = false);
    void autograderunittest_setWindowDescriptionText(const std::string& text, bool styleCheck = false);

    std::string clipboard_get();
    void clipboard_set(const std::string& text);

    std::string cpplib_getCppLibraryVersion();
    std::string cpplib_getJavaBackEndVersion();
    void cpplib_setCppLibraryVersion();

    void diffimage_compareImages(const std::string& file1, const std::string& file2, const std::string& outfile);
    void diffimage_compareWindowToImage(const GWindow& gwindow, const std::string& file2, bool ignoreWindowSize);
    void diffimage_show(const std::string& file1, const std::string& file2);

    std::string file_openFileDialog(const std::string& title, const std::string& mode, const std::string& path);
    void filelib_createDirectory(const std::string& path);
    std::string filelib_expandPathname(const std::string& filename);
    bool filelib_fileExists(const std::string& filename);
    std::string filelib_getCurrentDirectory();
    std::string filelib_getDirectoryPathSeparator();
    std::string filelib_getSearchPathSeparator();
    std::string filelib_getTempDirectory();
    bool filelib_isDirectory(const std::string& filename);
    bool filelib_isFile(const std::string& filename);
    bool filelib_isSymbolicLink(const std::string& filename);
    void filelib_listDirectory(const std::string& path, std::vector<std::string>& list);
    void filelib_setCurrentDirectory(const std::string& path);

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
    void gbufferedimage_save(const GObject* const gobj, const std::string& filename);
    void gbufferedimage_setRGB(GObject* gobj, double x, double y, int rgb);
    void gbufferedimage_updateAllPixels(GObject* gobj, const std::string& base64);

    void gbutton_constructor(GObject* gobj, const std::string& label);

    void gcheckbox_constructor(GObject* gobj, const std::string& label);
    bool gcheckbox_isSelected(const GObject* gobj);
    void gcheckbox_setSelected(GObject* gobj, bool state);

    void gchooser_addItem(GObject* gobj, const std::string& item);
    void gchooser_constructor(GObject* gobj);
    std::string gchooser_getSelectedItem(const GObject* gobj);
    void gchooser_setSelectedItem(GObject* gobj, const std::string& item);

    void gcompound_add(GObject* compound, GObject* gobj);
    void gcompound_constructor(GObject* gobj);

    GEvent gevent_getNextEvent(int mask);
    GEvent gevent_waitForEvent(int mask);

    std::string gfilechooser_showOpenDialog(const std::string& currentDir, const std::string& fileFilter);
    std::string gfilechooser_showSaveDialog(const std::string& currentDir, const std::string& fileFilter);

    GDimension gimage_constructor(GObject* gobj, const std::string& filename);

    void ginteractor_addActionListener(GObject* gobj);
    std::string ginteractor_getFont(const GObject* gobj);
    char ginteractor_getMnemonic(const GObject* gobj);
    GDimension ginteractor_getSize(const GObject* gobj);
    bool ginteractor_isEnabled(const GObject* gint);
    void ginteractor_removeActionListener(GObject* gobj);
    void ginteractor_requestFocus(GObject* gobj);
    void ginteractor_setAccelerator(GObject* gobj, const std::string& accelerator);
    void ginteractor_setActionCommand(GObject* gobj, const std::string& cmd);
    void ginteractor_setBackground(GObject* gobj, const std::string& color);
    void ginteractor_setEnabled(GObject* gint, bool value);
    void ginteractor_setFont(GObject* gobj, const std::string& font);
    void ginteractor_setIcon(GObject* gobj, const std::string& filename);
    void ginteractor_setMnemonic(GObject* gobj, char mnemonic);
    void ginteractor_setText(GObject* gobj, const std::string& text);
    void ginteractor_setTextPosition(GObject* gobj, int horizontal, int vertical);
    void ginteractor_setTooltip(GObject* gobj, const std::string& tooltipText);

    void glabel_constructor(GObject* gobj, const std::string& label);
    double glabel_getFontAscent(const GObject* gobj);
    double glabel_getFontDescent(const GObject* gobj);
    GDimension glabel_getSize(const GObject* gobj);
    void glabel_setFont(GObject* gobj, const std::string& font);
    void glabel_setLabel(GObject* gobj, const std::string& str);

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
    void gobject_setColor(GObject* gobj, const std::string& color);
    void gobject_setFillColor(GObject* gobj, const std::string& color);
    void gobject_setFilled(GObject* gobj, bool flag);
    void gobject_setLineWidth(GObject* gobj, double lineWidth);
    void gobject_setLocation(GObject* gobj, double x, double y);
    void gobject_setSize(GObject* gobj, double width, double height);
    void gobject_setVisible(GObject* gobj, bool flag);

    int goptionpane_showConfirmDialog(const std::string& message, const std::string& title, int type);
    std::string goptionpane_showInputDialog(const std::string& message, const std::string& title);
    void goptionpane_showMessageDialog(const std::string& message, const std::string& title, int type);
    int goptionpane_showOptionDialog(const std::string& message,
                                     const std::string& title,
                                     const std::vector<std::string>& options,
                                     const std::string& initiallySelected);
    void goptionpane_showTextFileDialog(const std::string& message, const std::string& title, int rows, int cols);

    void goval_constructor(GObject* gobj, double width, double height);

    void gpolygon_addVertex(GObject* gobj, double x, double y);
    void gpolygon_constructor(GObject* gobj);

    void gradiobutton_constructor(GObject* gobj, const std::string& label, const std::string& group);
    bool gradiobutton_isSelected(const GObject* gobj);
    void gradiobutton_setSelected(GObject* gobj, bool state);

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

    void gtable_autofitColumnWidths(GObject* gobj);
    void gtable_clear(GObject* gobj);
    void gtable_clearFormatting(GObject* gobj);
    void gtable_constructor(GObject* gobj, int numRows, int numCols, double x, double y, double width, double height);
    std::string gtable_get(const GObject* gobj, int row, int column);
    int gtable_getColumnWidth(const GObject* gobj, int column);
    void gtable_getSelection(const GObject* gobj, int& row, int& column);
    void gtable_resize(GObject* gobj, int numRows, int numCols);
    void gtable_select(GObject* gobj, int row, int column);
    void gtable_set(GObject* gobj, int row, int column, const std::string& value);
    void gtable_setCellAlignment(GObject* gobj, int row, int column, int align);
    void gtable_setCellBackground(GObject* gobj, int row, int column, const std::string& color);
    void gtable_setCellFont(GObject* gobj, int row, int column, const std::string& font);
    void gtable_setCellForeground(GObject* gobj, int row, int column, const std::string& color);
    void gtable_setColumnAlignment(GObject* gobj, int column, int align);
    void gtable_setColumnBackground(GObject* gobj, int column, const std::string& color);
    void gtable_setColumnFont(GObject* gobj, int column, const std::string& font);
    void gtable_setColumnForeground(GObject* gobj, int column, const std::string& color);
    void gtable_setColumnHeaderStyle(GObject* gobj, int style);
    void gtable_setColumnWidth(GObject* gobj, int column, int width);
    void gtable_setEditable(GObject* gobj, bool editable);
    void gtable_setEditorValue(GObject* gobj, int row, int column, const std::string& value);
    void gtable_setEventEnabled(GObject* gobj, int type, bool enabled);
    void gtable_setFont(GObject* gobj, const std::string& font);
    void gtable_setHorizontalAlignment(GObject* gobj, const std::string& alignment);
    void gtable_setRowAlignment(GObject* gobj, int row, int align);
    void gtable_setRowBackground(GObject* gobj, int row, const std::string& color);
    void gtable_setRowColumnHeadersVisible(GObject* gobj, bool visible);
    void gtable_setRowFont(GObject* gobj, int row, const std::string& font);
    void gtable_setRowForeground(GObject* gobj, int row, const std::string& color);

    void gtextarea_create(GObject* gobj, double width, double height);
    std::string gtextarea_getText(const GObject *gobj);
    void gtextarea_setEditable(GObject* gobj, bool isEditable);
    void gtextarea_setFont(GObject* gobj, const std::string& font);
    void gtextarea_setText(GObject* gobj, const std::string& text);

    void gtextfield_constructor(GObject* gobj, int nChars);
    std::string gtextfield_getText(const GObject* gobj);
    bool gtextfield_isEditable(const GObject* gobj);
    void gtextfield_setEditable(GObject* gobj, bool value);
    void gtextfield_setPlaceholder(GObject* gobj, const std::string& text);
    void gtextfield_setText(GObject* gobj, const std::string& str);

    void gtimer_constructor(const GTimer& timer, double delay);
    void gtimer_delete(const GTimer& timer);
    void gtimer_pause(double milliseconds);
    void gtimer_start(const GTimer& timer);
    void gtimer_stop(const GTimer& timer);

    void gwindow_addToRegion(const GWindow& gw, GObject* gobj, const std::string& region);
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
    GDimension gwindow_getContentPaneSize(const GWindow& gw);
    Point gwindow_getLocation(const GWindow& gw);
    int gwindow_getPixel(const GWindow& gw, int x, int y);
    Grid<int> gwindow_getPixels(const GWindow& gw);
    GDimension gwindow_getRegionSize(const GWindow& gw, const std::string& region);
    double gwindow_getScreenHeight();
    GDimension gwindow_getScreenSize();
    double gwindow_getScreenWidth();
    GDimension gwindow_getSize(const GWindow& gw);
    void gwindow_minimize(const GWindow& gw);
    void gwindow_pack(const GWindow& gw);
    void gwindow_removeFromRegion(const GWindow& gw, GObject* gobj, const std::string& region);
    void gwindow_repaint(const GWindow& gw);
    void gwindow_requestFocus(const GWindow& gw);
    void gwindow_saveCanvasPixels(const GWindow& gw, const std::string& filename);
    void gwindow_setCanvasSize(const GWindow& gw, int width, int height);
    void gwindow_setCloseOperation(const GWindow& gw, int op);
    void gwindow_setExitOnClose(const GWindow& gw, bool value);
    void gwindow_setLocation(const GWindow& gw, int x, int y);
    void gwindow_setLocationSaved(const GWindow& gw, bool value);
    void gwindow_setPixel(const GWindow& gw, int x, int y, int rgb, bool repaint = true);
    void gwindow_setPixels(const GWindow& gw, const Grid<int>& grid);
    void gwindow_setRegionAlignment(const GWindow& gw, const std::string& region, const std::string& align);
    void gwindow_setResizable(const GWindow& gw, bool value);
    void gwindow_setSize(const GWindow& gw, int width, int height);
    void gwindow_setTitle(const GWindow& gw, const std::string& title);
    void gwindow_setVisible(const GWindow& gw, bool flag);
    void gwindow_toBack(const GWindow& gw);
    void gwindow_toFront(const GWindow& gw);

    void httpserver_sendResponse(int requestID, int httpErrorCode, const std::string& contentType, const std::string& responseText);
    void httpserver_sendResponseFile(int requestID, const std::string& contentType, const std::string& responseFile);
    void httpserver_start(int port);
    void httpserver_stop();

    void jbeconsole_clear();
    bool jbeconsole_isBlocked();
    void jbeconsole_minimize();
    void jbeconsole_setCloseOperation(int value);
    void jbeconsole_setErrorColor(const std::string& color);
    void jbeconsole_setExitProgramOnClose(bool value);
    void jbeconsole_setLocationSaved(bool value);
    void jbeconsole_setOutputColor(const std::string& color);
    void jbeconsole_setFont(const std::string& font);
    void jbeconsole_setLocation(int x, int y);
    void jbeconsole_setSize(double width, double height);
    void jbeconsole_setTitle(const std::string& title);
    void jbeconsole_setVisible(bool value);
    void jbeconsole_toFront();

    void note_play(const std::string& noteString);

    std::string os_getLastError();

    bool regex_match(const std::string& s, const std::string& regexp);
    int regex_matchCount(const std::string& s, const std::string& regexp);
    int regex_matchCountWithLines(const std::string& s, const std::string& regexp, std::string& linesOut);
    std::string regex_replace(const std::string& s, const std::string& regexp, const std::string& replacement, int limit = -1);

    void setStackSize(unsigned int stackSize);

    void sound_constructor(Sound* sound, const std::string& filename);
    void sound_delete(Sound* sound);
    void sound_play(Sound* sound);

    int url_download(const std::string& url, const std::string& filename);
};

/* free function to get a reference to the singleton Platform instance */
Platform* getPlatform();

/* enable / disable use of the std::exit() function */
bool exitEnabled();
void setExitEnabled(bool enabled = true);

// functions to interact with the graphical console through the platform
std::string getLineConsole();
void initializeGraphicalConsole();
void initializeStanfordCppLibrary();
void putConsole(const std::string& str, bool isStderr = false);
void endLineConsole(bool isStderr = false);
void echoConsole(const std::string& str, bool isStderr = false);
void setConsoleProperties();
void parseArgs(int argc, char** argv);

} // namespace stanfordcpplib

// for internal use
void __initializeStanfordCppLibrary(int argc, char** argv);

#endif
