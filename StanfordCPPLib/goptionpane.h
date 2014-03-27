/*
 * File: goptionpane.h
 * -------------------
 * This file defines the <code>GOptionPane</code> class which supports
 * popping up graphical dialog boxes for user input.
 */

#ifndef _goptionpane_h
#define _goptionpane_h

#include <string>
#include "vector.h"

enum GOptionPaneResult {
    GOPTIONPANE_YES,
    GOPTIONPANE_NO,
    GOPTIONPANE_CANCEL,
    GOPTIONPANE_OK,
    GOPTIONPANE_ERROR
};

class GOptionPane {
public:
    /*
     * Constants for message types, taken from Java's JOptionPane
     */
    static const int PLAIN_MESSAGE = -1;
    static const int INFORMATION_MESSAGE = 1;
    static const int WARNING_MESSAGE = 2;
    static const int QUESTION_MESSAGE = 3;

    static void showMessageDialog(std::string message, int type = PLAIN_MESSAGE);
    static GOptionPaneResult showConfirmDialog(std::string message);
    static std::string showInputDialog(std::string message);
    static std::string showOptionDialog(const Vector<std::string>& options, std::string initiallySelected = "");

private:
    /*
     * Constants for dialog results, taken from Java's JOptionPane.
     * Converted into GOptionPaneResult before returning to the client.
     */
    static const int YES_OPTION = 0;
    static const int NO_OPTION = 1;
    static const int OK_OPTION = 0;
    static const int CANCEL_OPTION = 2;

    /*
     * The two types of confirm dialogs: Yes/No, and Yes/No/Cancel.
     */
    static const int YES_NO_OPTION = 0;
    static const int YES_NO_CANCEL_OPTION = 1;
};

#endif
