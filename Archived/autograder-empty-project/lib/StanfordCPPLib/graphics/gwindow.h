/*
 * File: gwindow.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2019/05/05
 * - added static method for isDarkMode checking support
 * - added static methods to ask for system default widget bg/fg color
 * @version 2019/04/09
 * - added toolbar support
 * @version 2018/10/20
 * - added high-density screen features
 * @version 2018/09/09
 * - added doc comments for new documentation generation
 * @version 2018/09/05
 * - refactored to use a border layout GContainer "content pane" for storing all interactors
 * @version 2018/08/23
 * - renamed to gwindow.h to replace Java version
 * @version 2018/07/29
 * - menu bars
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gwindow_h
#define _gwindow_h

#include <string>
#include <QWindow>
#include <QCloseEvent>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>
#include <QRect>

#define INTERNAL_INCLUDE 1
#include "gcanvas.h"
#define INTERNAL_INCLUDE 1
#include "gcontainer.h"
#define INTERNAL_INCLUDE 1
#include "gdrawingsurface.h"
#define INTERNAL_INCLUDE 1
#include "geventqueue.h"
#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "grid.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "point.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#undef INTERNAL_INCLUDE

class _Internal_QMainWindow;

/**
 * This class represents a graphics window that supports simple graphics.
 * A GWindow is a first-class citizen in our GUI subsystem; all graphical
 * programs will create at least one GWindow to hold other interactors and
 * graphical objects for display on the screen.
 *
 * A GWindow simultaneously serves two major graphical purposes:
 *
 * 1) A top-level container for interactors.
 * You can call the addToRegion and add methods to add interactors to the north,
 * south, west, east, and center regions of the window.
 * The center region holds at most one interactor that expands in both dimensions
 * to fill pixels not occupied by the other four regions.
 * This is analogous to Java AWT/Swing's BorderLayout system.
 * The window uses an internal GContainer that we call its "content pane" to
 * layout the positions and sizes of these interactors.
 * See gcontainer.h for more detail about layout and containers.
 *
 * 2) A graphical canvas for drawing shapes, lines, and colors.
 * A GWindow contains a central graphical canvas that is implemented as an
 * object of type GCanvas.  The canvas will appear on the window the moment you
 * call any drawing method on the window.
 *
 * The graphical canvas consists of two layers.
 * The background layer provides a surface for drawing static pictures that
 * involve no animation, or for 2D pixel-based drawing algorithms.
 * See gcanvas.h and gobjects.h for more detail about drawing shapes, objects,
 * and pixels.
 *
 * The GWindow class includes several drawXxx and fillXxx methods that draw
 * lines, rectangles, and ovals on the background layer without the client
 * needing to directly create objects from the gobjects.h hierarchy.
 *
 * The foreground layer provides an abstraction for adding stateful shapes and
 * graphical objects onto the canvas.  The add() methods that accept GObject
 * parameters place these objects onto the foreground layer.  The advantage of
 * the foreground layer is that you can manipulate the object over time, such as
 * moving it, changing its color, size, or other properties, and see these
 * changes immediately on the screen.  This makes the foreground layer most
 * appropriate for animations or moving sprites.
 *
 * You can use the two GWindow paradigms together in the same window.
 * For example, you can place a row of buttons in the north or south while
 * drawing shapes onto the canvas in the center of the window.
 *
 * If you add() a GInteractor to the center region of the window, we will assume
 * that you do not want the graphical canvas and will replace it with the added
 * interactor.
 */
class GWindow : public GObservable, public virtual GForwardDrawingSurface {
public:
    /**
     * The five regions of window border layouts.
     */
    enum Region {
        REGION_CENTER = GContainer::REGION_CENTER,
        REGION_EAST = GContainer::REGION_EAST,
        REGION_NORTH = GContainer::REGION_NORTH,
        REGION_SOUTH = GContainer::REGION_SOUTH,
        REGION_WEST = GContainer::REGION_WEST
    };

    /**
     * The various actions that can occur when a window closes.
     */
    enum CloseOperation {
        CLOSE_DO_NOTHING,
        CLOSE_HIDE,
        CLOSE_DISPOSE,
        CLOSE_EXIT
    };

    /**
     * The default width of a newly created window in pixels if
     * its width is not explicitly specified.
     */
    static const int DEFAULT_WIDTH;

    /**
     * The default height of a newly created window in pixels if
     * its height is not explicitly specified.
     */
    static const int DEFAULT_HEIGHT;

    /**
     * The minimum number of dots per inch before a screen is considered
     * to be high-density or high-DPI.
     */
    static const int HIGH_DPI_SCREEN_THRESHOLD;

    /**
     * The minimum number of dots per inch on a "normal" low-DPI screen.
     * Used to figure out how much to scale up on high-DPI screens.
     */
    static const int STANDARD_SCREEN_DPI;

    /**
     * The default file name used to load a GWindow's initial
     * title bar icon.
     */
    static const std::string DEFAULT_ICON_FILENAME;

    /**
     * Creates a new window of a default width and height.
     */
    GWindow(bool visible = true);

    /**
     * Creates a new window of the given width and height.
     */
    GWindow(double width, double height, bool visible = true);

    /**
     * Creates a new window of the given location and size.
     */
    GWindow(double x, double y, double width, double height, bool visible = true);

    /**
     * Frees memory allocated internally by the window.
     */
    virtual ~GWindow() Q_DECL_OVERRIDE;

    /**
     * Adds the given interactor to the center region of the window.
     * This replaces the graphical canvas and causes the canvas to be hidden.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GInteractor* interactor);

    /**
     * Adds the given interactor to the center region of the window
     * and moves it to the given x/y location.
     * This replaces the graphical canvas and causes the canvas to be hidden.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GInteractor* interactor, double x, double y);

    /**
     * Adds the given interactor to the center region of the window.
     * This replaces the graphical canvas and causes the canvas to be hidden.
     */
    virtual void add(GInteractor& interactor);

    /**
     * Adds the given interactor to the center region of the window
     * and moves it to the given x/y location.
     * This replaces the graphical canvas and causes the canvas to be hidden.
     */
    virtual void add(GInteractor& interactor, double x, double y);

    /**
     * Adds the given graphical object to the window's canvas.
     * This causes the graphical canvas to appear if it was not already showing.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GObject* obj);

    /**
     * Adds the given graphical object to the window's canvas
     * and moves it to the given x/y location.
     * This causes the graphical canvas to appear if it was not already showing.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GObject* obj, double x, double y);

    /**
     * Adds the given graphical object to the window's canvas.
     * This causes the graphical canvas to appear if it was not already showing.
     */
    virtual void add(GObject& obj);

    /**
     * Adds the given graphical object to the window's canvas
     * and moves it to the given x/y location.
     * This causes the graphical canvas to appear if it was not already showing.
     */
    virtual void add(GObject& obj, double x, double y);

    /**
     * Adds a menu with the given text to the window's top menu bar.
     * If the given menu already exists, returns it without adding it again.
     */
    virtual QMenu* addMenu(const std::string& text);

    /**
     * Adds a new menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, an ACTION_MENU action event will occur.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon = "");

    /**
     * Adds a new menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon, GEventListenerVoid func);

    /**
     * Adds a new menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const QIcon& icon, GEventListenerVoid func);

    /**
     * Adds a new menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const QPixmap& icon, GEventListenerVoid func);

    /**
     * Adds a new checkbox menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, an ACTION_MENU action event will occur.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked = false,
                                         const std::string& icon = "");

    /**
     * Adds a new checkbox menu item to the given menu.
     * If the given menu item already exists in this menu, returns it without
     * adding it again.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked,
                                         const std::string& icon, GEventListenerVoid func);

    /**
     * Adds a horizontal line separator to the end of the given menu.
     * @throw ErrorException if the given menu does not exist
     */
    virtual QAction* addMenuSeparator(const std::string& menu);

    /**
     * Adds a sub-menu within an existing menu.
     * You can later add items to this sub-menu using:
     *
     * myWindow->addMenuItem(menu + "/" + submenu, item);
     * @throw ErrorException if the given menu does not exist
     */
    virtual QMenu* addSubMenu(const std::string& menu, const std::string& submenu);

    /**
     * Adds the given interactor to the given region in this window.
     * @throw ErrorException if the interactor is null
     */
    virtual void addToRegion(GInteractor* interactor, Region region);

    /**
     * Adds the given interactor to the given region in this window.
     * @throw ErrorException if the interactor is null
     */
    virtual void addToRegion(GInteractor* interactor, const std::string& region = "Center");

    /**
     * Adds the given interactor to the given region in this window.
     */
    virtual void addToRegion(GInteractor& interactor, Region region);

    /**
     * Adds the given interactor to the given region in this window.
     */
    virtual void addToRegion(GInteractor& interactor, const std::string& region = "Center");

    /**
     * Adds a toolbar to this window where action buttons can be placed.
     */
    virtual void addToolbar(const std::string& title = "");

    /**
     * Adds a new item to the window's toolbar.
     * If the window does not have a toolbar, one is added.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, an ACTION_MENU action event will occur.
     */
    virtual QAction* addToolbarItem(const std::string& item,
                                    const std::string& icon = "");

    /**
     * Adds a new item to the window's toolbar.
     * If the window does not have a toolbar, one is added.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     */
    virtual QAction* addToolbarItem(const std::string& item,
                                    const std::string& icon,
                                    GEventListenerVoid func);

    /**
     * Adds a new item to the window's toolbar.
     * If the window does not have a toolbar, one is added.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     */
    virtual QAction* addToolbarItem(const std::string& item,
                                    const QIcon& icon,
                                    GEventListenerVoid func);

    /**
     * Adds a new item to the window's toolbar.
     * If the window does not have a toolbar, one is added.
     * You can supply an optional icon to show next to the menu item.
     * When the menu item is clicked, the given listener function will be called.
     */
    virtual QAction* addToolbarItem(const std::string& item,
                                    const QPixmap& icon,
                                    GEventListenerVoid func);

    /**
     * Adds a separator to the window's toolbar.
     * If the window does not have a toolbar, one is added.
     */
    virtual QAction* addToolbarSeparator();

    /**
     * Removes all interactors from all regionss of the window.
     */
    virtual void clear() Q_DECL_OVERRIDE;

    /**
     * Removes all graphical objects from the graphical canvas in this window
     * and resets the background layer to the window's background color.
     */
    virtual void clearCanvas();

    /**
     * Removes all graphical objects from the graphical canvas in this window.
     * This means that any shapes added using the add() methods, such as GRect,
     * GOval, etc. will be removed, while any shapes drawn directly onto the
     * window's background pixel layer by calling the drawXxx() methods will be
     * retained.  To clear the background layer as well, call clearCanvasPixels
     * or clearCanvas instead.
     */
    virtual void clearCanvasObjects();

    /**
     * Resets the background layer of pixels in the window's canvas to the
     * window's background color.
     * This means that any shapes added using the add() methods, such as GRect,
     * GOval, etc. will remain, while any shapes drawn directly onto the
     * window's background pixel layer by calling the drawXxx() methods will be
     * wiped out.  To clear the shapes added to the foreground layer as well,
     * call clearCanvasObjects or clearCanvas instead.
     */
    virtual void clearCanvasPixels();

    /**
     * Removes all interactors from the given region of this window.
     */
    virtual void clearRegion(Region region);

    /**
     * Removes all interactors from the given region of this window.
     */
    virtual void clearRegion(const std::string& region);

    /**
     * Removes all items from the window's toolbar, if present.
     */
    virtual void clearToolbarItems();

    /**
     * Relocates the window to the exact center of the current screen.
     */
    virtual void center();

    /**
     * Returns which color to use depending on whether the user's computer is
     * in light or dark mode.
     * If in light mode, returns lightColor; else returns darkColor.
     */
    static std::string chooseLightDarkModeColor(const std::string& lightColor,
                                                const std::string& darkColor);

    /**
     * Returns which color to use depending on whether the user's computer is
     * in light or dark mode.
     * If in light mode, returns lightColor; else returns darkColor.
     */
    static int chooseLightDarkModeColorInt(int lightColor, int darkColor);

    /**
     * Closes the window.
     * If a window listener has been set, a WINDOW_CLOSING and then
     * WINDOW_CLOSED event is sent to it.
     */
    virtual void close();

    /**
     * Compares the pixels of this window to the contents of the image in the
     * given file.
     * The differences are displayed in a "diff image" window that highlights
     * any differing pixels.
     *
     * TODO: implement
     * @private
     */
    virtual void compareToImage(const std::string& filename, bool ignoreWindowSize = true) const;

    /**
     * Returns true if events can occur on the window.
     * This will be true if the window has been initialized and is visible.
     * @private
     */
    virtual bool eventsEnabled() const Q_DECL_OVERRIDE;

    /**
     * Returns a direct pointer to the window's internal graphical canvas
     * on which shapes and objects are drawn.
     * Use with care!
     */
    virtual GCanvas* getCanvas() const;

    /**
     * Returns the height of the window's central canvas area in pixels.
     */
    virtual double getCanvasHeight() const;

    /**
     * Returns the width and height of the window's central canvas area in pixels.
     */
    virtual GDimension getCanvasSize() const;

    /**
     * Returns the width of the window's central canvas area in pixels.
     */
    virtual double getCanvasWidth() const;

    /**
     * Returns a constant representing the action that will be taken when the
     * user closes the window.
     */
    virtual CloseOperation getCloseOperation() const;

    /**
     * Returns the default color for backgrounds of interactors as a string.
     * This is normally a light-grayish color, depending on the user's
     * system settings.
     * On some systems that are in "dark mode" this may be a color closer to black.
     */
    static std::string getDefaultInteractorBackgroundColor();

    /**
     * Returns the default color for text on interactors as an RGB integer.
     * This is normally a light-grayish color, depending on the user's
     * system settings.
     * On some systems that are in "dark mode" this may be a color closer to black.
     */
    static int getDefaultInteractorBackgroundColorInt();

    /**
     * Returns the default color for text on interactors as a string.
     * This is normally black or a nearly-black color, depending on the user's
     * system settings.
     * On some systems that are in "dark mode" this may be a color closer to white.
     */
    static std::string getDefaultInteractorTextColor();

    /**
     * Returns the default color for text on interactors as an RGB integer.
     * This is normally black or a nearly-black color, depending on the user's
     * system settings.
     * On some systems that are in "dark mode" this may be a color closer to white.
     */
    static int getDefaultInteractorTextColorInt();

    /**
     * Returns the graphical object at the given 0-based index in the window's
     * graphical canvas.
     * @throw ErrorException if the index is out of bounds
     */
    virtual GObject* getGObject(int index) const;

    /**
     * Returns the top-most graphical object in the z-ordering in the window's
     * graphical canvas that touches the given x/y pixel location.
     * If no object touches the given location, returns nullptr.
     */
    virtual GObject* getGObjectAt(double x, double y) const;

    /**
     * Returns the total number of graphical objects in the window's canvas.
     */
    virtual int getGObjectCount() const;

    /**
     * Returns a pointer to the most recently created Qt window object.
     * Not to be called by students.
     * @private
     */
    static QMainWindow* getLastWindow();

    /**
     * Returns the x/y location of the top-left corner of the interior of the window on screen,
     * excluding any onscreen window title bar and frame.
     */
    virtual GPoint getLocation() const;

    /**
     * Returns the total height of the window in pixels, excluding its title
     * bar and borders.
     */
    virtual double getHeight() const;

    /**
     * Returns the size that the window would prefer to be.
     * The window prefers to be exactly the right size to fit the interactors
     * placed inside it at their own preferred sizes without stretching.
     * This is the size that the window will be set to if you call pack().
     */
    virtual GDimension getPreferredSize() const;

    /**
     * Returns the height of the given region of the window in pixels.
     */
    virtual double getRegionHeight(Region region) const;

    /**
     * Returns the height of the given region of the window in pixels.
     */
    virtual double getRegionHeight(const std::string& region) const;

    /**
     * Returns the width and height of the given region of the window in pixels.
     */
    virtual GDimension getRegionSize(Region region) const;

    /**
     * Returns the width and height of the given region of the window in pixels.
     */
    virtual GDimension getRegionSize(const std::string& region) const;

    /**
     * Returns the width of the given region of the window in pixels.
     */
    virtual double getRegionWidth(Region region) const;

    /**
     * Returns the width of the given region of the window in pixels.
     */
    virtual double getRegionWidth(const std::string& region) const;

    /**
     * Returns the dots-per-inch of the screen.
     * This is used when accounting for high-density screens.
     */
    static int getScreenDpi();

    /**
     * Returns the ratio of this screen's DPI compared to a normal low-DPI screen.
     * This can be used to scale up graphics on high-density screens.
     */
    static double getScreenDpiScaleRatio();

    /**
     * Returns the height of the entire screen in pixels.
     */
    static double getScreenHeight();

    /**
     * Returns the width and height of the entire screen in pixels.
     */
    static GDimension getScreenSize();

    /**
     * Returns the width of the entire screen in pixels.
     */
    static double getScreenWidth();

    /**
     * Returns the total width and height of the window in pixels, excluding
     * its title bar and borders.
     */
    virtual GDimension getSize() const;

    /**
     * Returns the title bar text for the window.
     */
    virtual std::string getTitle() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /**
     * Returns an internal Qt widget representing the window.
     * Clients do not need to use this method directly.
     * @private
     */
    virtual QWidget* getWidget() const;

    /**
     * Returns the total width of the window in pixels, excluding its title
     * bar and borders.
     */
    virtual double getWidth() const;

    /**
     * Returns the x location of the left edge of the interior of the window on screen,
     * excluding any onscreen window title bar and frame.
     */
    virtual double getX() const;

    /**
     * Returns the y location of the top edge of the interior of the window on screen,
     * excluding any onscreen window title bar and frame.
     */
    virtual double getY() const;

    /**
     * Returns true if this window has a toolbar.
     */
    virtual bool hasToolbar() const;

    /**
     * Makes the window be not visible on the screen.
     * Equivalent to setVisible(false).
     */
    virtual void hide();

    /**
     * Returns true if the given x/y location is within the bounds of the entire
     * window.
     * Note that this is based on the entire window size including its title
     * bar, menus, borders, etc.
     * If you are trying to test for bounds for shapes in the window canvas area,
     * use the inCanvasBounds method instead.
     */
    virtual bool inBounds(double x, double y) const;

    /**
     * Returns true if the given x/y location is within the bounds of the central
     * canvas area of the window.
     */
    virtual bool inCanvasBounds(double x, double y) const;

    /**
     * Returns true if the user's computer is in "dark mode."
     * This is a popular dark color scheme mostly used on recent Macs.
     * Our checking is imperfect and basically just creates a dummy widget
     * and checks whether it has bright text and a dark background.
     */
    static bool isDarkMode();

    /**
     * Returns whether the dots-per-inch of the screen are high enough to
     * consider it a "high-density" screen for which scaling should be used.
     * The threshold for this is given by the constant
     */
    static bool isHighDensityScreen();

    /**
     * Returns whether we should scale some windows when run on high-density
     * screens.
     */
    static bool isHighDpiScalingEnabled();

    /**
     * Returns true if the window is in a maximized state, occupying the entire
     * screen.
     */
    virtual bool isMaximized() const;

    /**
     * Returns true if the window is in a minimized (iconified) state, which
     * often displays as the window being hidden except for a task bar icon.
     */
    virtual bool isMinimized() const;

    /**
     * Returns true if the window is currently open and visible on the screen.
     */
    virtual bool isOpen() const;

    /* @inherit */
    virtual bool isRepaintImmediately() const Q_DECL_OVERRIDE;

    /**
     * Returns true if the window allows itself to be resized.
     * This is initially true but can be changed by calling setResizable(false).
     */
    virtual bool isResizable() const;

    /**
     * Returns true if the window is visible on the screen.
     */
    virtual bool isVisible() const;

    /**
     * Reads pixel data from the file with the given name and loads it into the
     * window's canvas area.
     * @throw ErrorException if the file is not found or cannot be loaded as an image
     */
    virtual void loadCanvasPixels(const std::string& filename);

    /**
     * Puts the window in a maximized state, occupying the entire screen.
     */
    virtual void maximize();

    /**
     * Puts the window in a minimized (iconified) state, which
     * often displays as the window being hidden except for a task bar icon.
     */
    virtual void minimize();

    /**
     * Resizes the window to its preferred size.
     * The window prefers to be exactly the right size to fit the interactors
     * placed inside it at their own preferred sizes without stretching.
     * This is the size that the window would return from a call to getPreferredSize.
     */
    virtual void pack();

    /**
     * Causes the current thread to pause itself for the given number of milliseconds.
     * Equivalent to sleep().
     * @throw ErrorException if ms is negative
     */
    virtual void pause(double ms);

    /**
     * Instructs the library to remember the window's x/y position so that if
     * another window with the same title is opened in the future, it will
     * automatically go back to that location.
     * @private
     */
    virtual void rememberPosition();

    /**
     * Removes the given interactor from the window.
     * This will work regardless of which region you added the interactor to.
     * If the given interactor is not found in this container, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void remove(GInteractor* interactor);

    /**
     * Removes the given interactor from the window.
     * This will work regardless of which region you added the interactor to.
     * If the given interactor is not found in this container, has no effect.
     */
    virtual void remove(GInteractor& interactor);

    /**
     * Removes the given graphical object from the canvas of this window,
     * if it was present.
     * @throw ErrorException if the graphical object is null
     */
    virtual void remove(GObject* obj);

    /**
     * Removes the given graphical object from the canvas of this window,
     * if it was present.
     */
    virtual void remove(GObject& obj);

    /**
     * Removes the click listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeClickListener();

    /**
     * Removes the given interactor from the given region within this window.
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void removeFromRegion(GInteractor* interactor, Region region);

    /**
     * Removes the given interactor from the given region within this window.
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void removeFromRegion(GInteractor* interactor, const std::string& region);

    /**
     * Removes the given interactor from the given region within this window.
     * If the given interactor is not found in the given region, has no effect.
     */
    virtual void removeFromRegion(GInteractor& interactor, Region region);

    /**
     * Removes the given interactor from the given region within this window.
     * If the given interactor is not found in the given region, has no effect.
     */
    virtual void removeFromRegion(GInteractor& interactor, const std::string& region);

    /**
     * Removes the key listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeKeyListener();

    /**
     * Removes the menu listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeMenuListener();

    /**
     * Removes the mouse listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeMouseListener();

    /**
     * Removes the timer listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeTimerListener();

    /**
     * Removes the toolbar from this window, if one was present.
     */
    virtual void removeToolbar();

    /**
     * Removes the window listener from this window so that it will no longer
     * call it when events occur.
     */
    virtual void removeWindowListener();

    /**
     * Asks the system to assign the keyboard focus to the window, which brings
     * it to the top and ensures that key events are delivered to the window.
     * Clicking in the window automatically requests the focus.
     */
    virtual void requestFocus();

    /**
     * Puts the window in a normal state, neither minimized or maximized.
     */
    virtual void restore();

    /**
     * Writes the contents of the window's graphical canvas to the given output
     * filename.  This will write all shapes from the foreground layer as well
     * as all pixels from the background layer.
     * @throw ErrorException if the file cannot be saved
     */
    virtual void saveCanvasPixels(const std::string& filename);

    /* @inherit */
    virtual void setBackground(int color) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Resizes the window so that its central canvas region will occupy exactly
     * the given height in pixels, without changing its width.
     */
    virtual void setCanvasHeight(double height);

    /**
     * Resizes the window so that its central canvas region will occupy exactly
     * the given width and height in pixels.
     */
    virtual void setCanvasSize(double width, double height);

    /**
     * Resizes the window so that its central canvas region will occupy exactly
     * the given width and height in pixels.
     */
    virtual void setCanvasSize(const GDimension& size);

    /**
     * Resizes the window so that its central canvas region will occupy exactly
     * the given width in pixels, without changing its height.
     */
    virtual void setCanvasWidth(double width);

    /**
     * Sets a mouse listener on this window so that it will be called
     * when the mouse is clicked on the window's canvas.
     * Any existing click listener will be replaced.
     * Note that this method is not how you listen to clicks on individual
     * buttons and other interactors inside the window; to do that, call
     * setActionListener and other such methods on those interactors individually.
     */
    virtual void setClickListener(GEventListener func);

    /**
     * Sets a mouse listener on this window so that it will be called
     * when the mouse is clicked on the window's canvas.
     * Any existing click listener will be replaced.
     * Note that this method is not how you listen to clicks on individual
     * buttons and other interactors inside the window; to do that, call
     * setActionListener and other such methods on those interactors individually.
     */
    virtual void setClickListener(GEventListenerVoid func);

    /**
     * Sets what should happen when the window is closed.
     */
    virtual void setCloseOperation(CloseOperation op);

    /**
     * Sets whether the library's GUI system should shut down when the
     * window is closed.
     */
    virtual void setExitOnClose(bool exitOnClose);

    /**
     * Sets the window's total height in pixels.
     */
    virtual void setHeight(double width);

    /**
     * Sets a key listener on this window so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListener func);

    /**
     * Sets a key listener on this window so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListenerVoid func);

    /**
     * Sets the window's top-left x/y location on the screen to the given coordinates.
     */
    virtual void setLocation(double x, double y);

    /**
     * Sets the window's top-left x/y location on the screen to the given point.
     */
    virtual void setLocation(const GPoint& p);

    /**
     * Sets the window's top-left x/y location on the screen to the given point.
     */

    virtual void setLocation(const Point& p);

    /**
     * Sets whether the given item in the given menu is enabled or disabled.
     * @throw ErrorException if the menu and/or item does not exist
     */
    virtual void setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled);

    /**
     * Sets a menu listener on this window so that it will be called
     * when menu items are clicked, sending an ACTION_MENU action event.
     * Any existing menu listener will be replaced.
     */
    virtual void setMenuListener(GEventListener func);

    /**
     * Sets a menu listener on this window so that it will be called
     * when menu items are clicked.
     * Any existing menu listener will be replaced.
     */
    virtual void setMenuListener(GEventListenerVoid func);

    /**
     * Sets a mouse listener on the window's canvas so that it will be called
     * when the user moves or clicks the mouse on the canvas.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListener func);

    /**
     * Sets a mouse listener on the window's canvas so that it will be called
     * when the user moves or clicks the mouse on the canvas.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListenerVoid func);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionAlignment(Region region, VerticalAlignment valign);

    /**
     * Sets the horizontal and vertical alignment of interactors in the given
     * region of the window.
     */
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign);

    /**
     * Sets the horizontal and/or vertical alignment of interactors in the given
     * region of the window.
     */
    virtual void setRegionAlignment(const std::string& region, const std::string& align);

    /**
     * Sets the horizontal and vertical alignment of interactors in the given
     * region of the window.
     */
    virtual void setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionHorizontalAlignment(Region region, HorizontalAlignment halign);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionHorizontalAlignment(const std::string& region, const std::string& halign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionVerticalAlignment(const std::string& region, const std::string& valign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * the window.
     */
    virtual void setRegionVerticalAlignment(Region region, VerticalAlignment valign);

    /**
     * Sets whether the window allows itself to be resized.
     * Initially true.
     */
    virtual void setResizable(bool resizable);

    /**
     * Sets the window's total width and height in pixels.
     * Note that this size includes the window's title bar, border, etc. as added
     * by your operating system.
     * If you actually want to draw shapes over a given width and height of pixels,
     * you should instead use the setCanvasSize method.
     */
    virtual void setSize(double width, double height);

    /**
     * Sets the window's width and height in pixels.
     * Note that this size includes the window's title bar, border, etc. as added
     * by your operating system.
     * If you actually want to draw shapes over a given width and height of pixels,
     * you should instead use the setCanvasSize method.
     */
    virtual void setSize(const GDimension& size);

    /**
     * Sets a menu listener on this window so that it will be called
     * when timer delays elapse, sending a timer event.
     * Any existing timer listener will be replaced.
     */
    virtual void setTimerListener(double ms, GEventListener func);

    /**
     * Sets a menu listener on this window so that it will be called
     * when timer delays elapse, sending a timer event.
     * Any existing timer listener will be replaced.
     */
    virtual void setTimerListener(double ms, GEventListenerVoid func);

    // TODO: setTimerListenerOnce?

    /**
     * Sets the window's title bar text to the given string.
     * Equivalent to setWindowTitle.
     */
    virtual void setTitle(const std::string& title);

    /**
     * Sets whether the window can be seen on the screen.
     * Initially true unless a visible value of false was passed to the window's
     * constructor.
     */
    virtual void setVisible(bool visible);

    /**
     * Sets the window's total width in pixels.
     */
    virtual void setWidth(double width);

    /**
     * Sets the window to use the
     */
    virtual void setWindowIcon(const std::string& iconFile);

    /**
     * Sets a window listener on this window so that it will be called
     * when window events occur, such as resizing or closing the window.
     * Any existing action listener will be replaced.
     */
    virtual void setWindowListener(GEventListener func);

    /**
     * Sets a window listener on this window so that it will be called
     * when window events occur, such as resizing or closing the window.
     * Any existing action listener will be replaced.
     */
    virtual void setWindowListener(GEventListenerVoid func);

    /**
     * Sets the window's title bar text to the given string.
     * Equivalent to setWindowTitle.
     */
    virtual void setWindowTitle(const std::string& title);

    /**
     * Sets the window's left x location on the screen to the given coordinate.
     */
    virtual void setX(double x);

    /**
     * Sets the window's top y location on the screen to the given coordinate.
     */
    virtual void setY(double y);

    /**
     * Sets the window to be visible on the screen.
     * Equivalent to setVisible(true).
     */
    virtual void show();

    /**
     * Causes the current thread to pause itself for the given number of milliseconds.
     * Equivalent to pause().
     * @throw ErrorException if ms is negative
     */
    virtual void sleep(double ms);

    /**
     * Moves the window to the back of the z-ordering in the operating system,
     * underneath any other windows that occupy the same pixels.
     */
    virtual void toBack();

    /**
     * Moves the window to the front of the z-ordering in the operating system,
     * in front of any other windows that occupy the same pixels.
     */
    virtual void toFront();

    /**
     * An internal function that disables the exitGraphics function so that
     * students cannot exit their programs during autograding.
     * This function is effectively private and should not be called by students.
     */
    static void _autograder_setExitGraphicsEnabled(bool enabled);

    /**
     * An internal function that disables the pause function so that
     * students cannot pausetheir programs during autograding.
     * This function is effectively private and should not be called by students.
     */
    static void _autograder_setPauseEnabled(bool enabled);

    /**
     * An internal function that sets whether the current window is part of an
     * autograder program.
     * This function is effectively private and should not be called by students.
     */
    virtual void _autograder_setIsAutograderWindow(bool isAutograderWindow);

protected:
    /**
     * @private
     */
    virtual void processKeyPressEventInternal(QKeyEvent* event);

private:
    Q_DISABLE_COPY(GWindow)

    static _Internal_QMainWindow* _lastWindow;

    virtual void ensureForwardTarget() Q_DECL_OVERRIDE;
    void _init(double width, double height, bool visible);
    static Region stringToRegion(const std::string& regionStr);

    _Internal_QMainWindow* _iqmainwindow;
    GContainer* _contentPane;
    GCanvas* _canvas;
    bool _resizable;
    CloseOperation _closeOperation;
    Map<std::string, QMenu*> _menuMap;
    Map<std::string, QAction*> _menuActionMap;
    QToolBar* _toolbar;

    friend class GInteractor;
    friend class _Internal_QMainWindow;
};


// global functions for compatibility

/**
 * Converts a color name into an integer that encodes the
 * red, green, and blue components of the color.
 * See gcolor.h for more details about colors.
 */
int convertColorToRGB(const std::string& colorName);

/**
 * Converts an <code>rgb</code> value into a color name in the
 * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
 * <code>gg</code>, and <code>bb</code> values are two-digit
 * hexadecimal numbers indicating the intensity of that component.
 * See gcolor.h for more details about colors.
 */
std::string convertRGBToColor(int rgb);

/**
 * Closes all graphics windows and exits from the application without
 * waiting for any additional user interaction.
 */
void exitGraphics();

/**
 * Returns the height of the entire display screen.
 */
double getScreenHeight();

/**
 * Returns the width/height of the entire display screen.
 */
GDimension getScreenSize();

/**
 * Returns the width of the entire display screen.
 */
double getScreenWidth();

/**
 * Pauses for the indicated number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 */
void pause(double milliseconds);

/**
 * Issues a request to update the most recently created graphics window.
 * You can also call the repaint() method on a window directly to repaint that window.
 */
void repaint();


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QMainWindow : public QMainWindow {
    Q_OBJECT

public:
    _Internal_QMainWindow(GWindow* gwindow, QWidget* parent = nullptr);

    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual void timerEvent(QTimerEvent* event) Q_DECL_OVERRIDE;
    virtual bool timerExists(int id = -1);
    virtual int timerStart(double ms);
    virtual void timerStop(int id = -1);

public slots:
    void handleMenuAction(const std::string& menu, const std::string& item);

private:
    GWindow* _gwindow;
    Set<int> _timerIDs;

    void processTimerEvent();

    friend class GWindow;
};

#endif // _gwindow_h
