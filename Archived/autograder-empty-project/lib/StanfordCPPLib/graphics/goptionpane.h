/*
 * File: goptionpane.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/23
 * - renamed enum constants to avoid name collisions (may break some client code)
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to goptionpane.h to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _goptionpane_h
#define _goptionpane_h

#include <string>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

// forward declaration
class GWindow;
class GOptionPane;

/**
 * This class provides static methods that pop up graphical input/output dialog
 * boxes on the screen.
 */
class GOptionPane {
public:
    /**
     * Types used by showConfirmDialog, representing the three kinds of
     * confirmation dialogs: Yes/No, Yes/No/Cancel, or OK/Cancel.
     */
    enum ConfirmType {
        CONFIRM_YES_NO = 0,
        CONFIRM_YES_NO_CANCEL = 1,
        CONFIRM_OK_CANCEL = 2
    };

    /**
     * The various results that can be returned from some option dialogs.
     * Note that NO has the value 0 and YES/OK have nonzero values, so you
     * can use a ConfirmResult in a boolean context.
     */
    enum ConfirmResult {
        CONFIRM_CANCEL = -1,   // for yes/no/cancel dialogs
        CONFIRM_NO = 0,        // 0 so that 'no' is 'falsey'
        CONFIRM_YES = 1,       // 1 so that 'yes' is 'truthy'
        CONFIRM_OK = 2         // for ok/cancel dialogs
    };

    /**
     * Types used by showMessageDialog, representing the various kinds of
     * message dialogs.  The type often slightly varies the dialog's appearance,
     * such as changing its icons or font.
     */
    enum MessageType {
        MESSAGE_ERROR = 0,
        MESSAGE_INFORMATION = 1,
        MESSAGE_PLAIN = -1,
        MESSAGE_WARNING = 2,
        MESSAGE_QUESTION = 3,
        MESSAGE_ABOUT = 4
    };

    /**
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     */
    static ConfirmResult showConfirmDialog(const std::string& message,
                                           const std::string& title = "",
                                           ConfirmType type = CONFIRM_YES_NO);

    /**
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     */
    static ConfirmResult showConfirmDialog(GWindow* parent,
                                           const std::string& message,
                                           const std::string& title = "",
                                           ConfirmType type = CONFIRM_YES_NO);

    /**
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     */
    static ConfirmResult showConfirmDialog(QWidget* parent,
                                           const std::string& message,
                                           const std::string& title = "",
                                           ConfirmType type = CONFIRM_YES_NO);

    /**
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     */
    static std::string showInputDialog(const std::string& message,
                                       const std::string& title = "",
                                       const std::string& initialValue = "");

    /**
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     */
    static std::string showInputDialog(GWindow* parent,
                                       const std::string& message,
                                       const std::string& title = "",
                                       const std::string& initialValue = "");

    /**
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     */
    static std::string showInputDialog(QWidget* parent,
                                       const std::string& message,
                                       const std::string& title = "",
                                       const std::string& initialValue = "");

    /**
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of PLAIN_MESSAGE, INFORMATION_MESSAGE,
     * WARNING_MESSAGE, or QUESTION_MESSAGE; this slightly affects the dialog's
     * appearance.  The default is PLAIN_MESSAGE.
     */
    static void showMessageDialog(const std::string& message,
                                  const std::string& title = "",
                                  MessageType type = MESSAGE_PLAIN);

    /**
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of PLAIN_MESSAGE, INFORMATION_MESSAGE,
     * WARNING_MESSAGE, or QUESTION_MESSAGE; this slightly affects the dialog's
     * appearance.  The default is PLAIN_MESSAGE.
     */
    static void showMessageDialog(GWindow* parent,
                                  const std::string& message,
                                  const std::string& title = "",
                                  MessageType type = MESSAGE_PLAIN);

    /**
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of PLAIN_MESSAGE, INFORMATION_MESSAGE,
     * WARNING_MESSAGE, or QUESTION_MESSAGE; this slightly affects the dialog's
     * appearance.  The default is PLAIN_MESSAGE.
     */
    static void showMessageDialog(QWidget* parent,
                                  const std::string& message,
                                  const std::string& title = "",
                                  MessageType type = MESSAGE_PLAIN);

    /**
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     */
    static std::string showOptionDialog(const std::string& message,
                                        const Vector<std::string>& options,
                                        const std::string& title = "",
                                        const std::string& initiallySelected = "");

    /**
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     */
    static std::string showOptionDialog(GWindow* parent,
                                        const std::string& message,
                                        const Vector<std::string>& options,
                                        const std::string& title = "",
                                        const std::string& initiallySelected = "");

    /**
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     */
    static std::string showOptionDialog(QWidget* parent,
                                        const std::string& message,
                                        const Vector<std::string>& options,
                                        const std::string& title = "",
                                        const std::string& initiallySelected = "");

    /**
     * Displays the given text in a scrolling monospaced text area.
     * rows/cols parameters control size to show; set to <= 0 for a default limit.
     */
    static void showTextFileDialog(const std::string& fileText,
                                   const std::string& title = "",
                                   int rows = -1, int cols = -1);

    /**
     * Displays the given text in a scrolling monospaced text area.
     * rows/cols parameters control size to show; set to <= 0 for a default limit.
     */
    static void showTextFileDialog(GWindow* parent,
                                   const std::string& fileText,
                                   const std::string& title = "",
                                   int rows = -1, int cols = -1);

    /**
     * Displays the given text in a scrolling monospaced text area.
     * rows/cols parameters control size to show; set to <= 0 for a default limit.
     */
    static void showTextFileDialog(QWidget* parent,
                                   const std::string& fileText,
                                   const std::string& title = "",
                                   int rows = -1, int cols = -1);

private:
    /**
     * Private constructor so that clients don't try to construct
     * GOptionPane objects.  The class is meant to be used via its
     * static methods, not by instantiation.
     */
    GOptionPane();

    /**
     * The results that can come back from showConfirmDialog.
     * These are converted into Result enum values.
     */
    enum InternalResult {
        INTERNAL_CANCEL_OPTION = 2,
        INTERNAL_CLOSED_OPTION = -1,
        INTERNAL_NO_OPTION = 1,
        INTERNAL_OK_OPTION = 0,
        INTERNAL_YES_OPTION = 0
    };
};

#endif // _goptionpane_h
