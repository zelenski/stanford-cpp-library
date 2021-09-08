/*
 * File: gclipboard.h
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gclipboard.h to replace Java version
 * @version 2018/07/19
 * - initial version
 */


#ifndef _gclipboard_h
#define _gclipboard_h

#include <QKeyEvent>
#include <string>

/**
 * The GClipboard class contains static methods you can use to get and set
 * the contents of the system clipboard.
 */
class GClipboard {
public:
    /**
     * Returns the current contents of the system clipboard.
     */
    static std::string get();

    /**
     * Returns true if the given event represents a "copy" operation.
     * @throw ErrorException if the event is null
     */
    static bool isCopy(QKeyEvent* event);

    /**
     * Returns true if the given event represents a "cut" operation.
     * @throw ErrorException if the event is null
     */
    static bool isCut(QKeyEvent* event);

    /**
     * Returns true if the given event represents a "paste" operation.
     * @throw ErrorException if the event is null
     */
    static bool isPaste(QKeyEvent* event);

    /**
     * Sets the system clipboard to store the given text.
     * Equivalent to a "copy" operation.
     */
    static void set(const std::string& text);

private:
    GClipboard();   // prevent construction
};

#endif // _gclipboard_h
