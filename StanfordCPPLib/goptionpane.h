/*
 * File: goptionpane.h
 * -------------------
 * This file defines the <code>GOptionPane</code> class which supports
 * popping up graphical dialog boxes for user input.
 * 
 * This class is inspired by, extremely similar to, and implemented on
 * the back-end by, Java's JOptionPane class, so you may wish to consult
 * that class's documentation in the Java API Specification for more
 * information.
 * 
 * @author Marty Stepp
 * @version 2014/10/26
 * - added showTextFileDialog
 * @version 2014/10/09
 * - finished implementation of showOptionDialog; added dialog types and other fixes
 * - enum rearrangement for clearer client code and better error messages
 * @since 2014/07/09
 */

#ifndef _goptionpane_h
#define _goptionpane_h

#include <string>
#include "vector.h"

class GOptionPane {
public:
    /*
     * Constants for showConfirmDialog types, taken from Java's JOptionPane.
     */
    enum ConfirmType {
        /*
         * The three types of confirm dialogs: Yes/No, Yes/No/Cancel, or OK/Cancel.
         */
        OK_CANCEL = 2,
        YES_NO = 0,
        YES_NO_CANCEL = 1
    };

    /*
     * This enumeration type defines the various results that can be returned
     * from some option dialogs.
     */
    enum ConfirmResult {
        CANCEL = -1,   // for yes/no/cancel dialogs
        NO = 0,        // 0 so that 'no' is 'falsey'
        OK = 2,        // for ok/cancel dialogs
        YES = 1        // 1 so that 'yes' is 'truthy'
    };
    
    /*
     * Constants for showMessageDialog types, taken from Java's JOptionPane
     */
    enum MessageType {
        ERROR = 0,
        INFORMATION = 1,
        PLAIN = -1,
        QUESTION = 3,
        WARNING = 2
    };

    /*
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     */
    static ConfirmResult showConfirmDialog(std::string message, std::string title = "",
                                           ConfirmType type = YES_NO);
    
    /*
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     */
    static std::string showInputDialog(std::string message, std::string title = "");
    
    /*
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of PLAIN_MESSAGE, INFORMATION_MESSAGE,
     * WARNING_MESSAGE, or QUESTION_MESSAGE; this slightly affects the dialog's
     * appearance.  The default is PLAIN_MESSAGE.
     */
    static void showMessageDialog(std::string message, std::string title = "",
                                  MessageType type = PLAIN);
    
    /*
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     */
    static std::string showOptionDialog(std::string message, const Vector<std::string>& options,
                                        std::string title = "", std::string initiallySelected = "");

    /*
     * Displays the given text in a scrolling monospaced text area.
     * rows/cols parameters control size to show; set to <= 0 for a default limit.
     */
    static void showTextFileDialog(std::string message, std::string title = "", int rows = -1, int cols = -1);
    
private:
    /*
     * Private constructor so that clients don't try to construct
     * GOptionPane objects.  The class is meant to be used via its
     * static methods, not by instantiation.
     */
    GOptionPane();
    
    enum InternalResult {
        /*
         * The results that can come back from showConfirmDialog.
         * These are converted into Result enum values.
         */
        CANCEL_OPTION = 2,
        CLOSED_OPTION = -1,
        NO_OPTION = 1,
        OK_OPTION = 0,
        YES_OPTION = 0
    };
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _goptionpane_h
