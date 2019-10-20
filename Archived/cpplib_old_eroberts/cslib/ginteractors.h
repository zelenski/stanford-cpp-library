/*
 * File: ginteractors.h
 * --------------------
 * This interface exports a hierarchy of graphical interactors similar to
 * those provided in the Java Swing libraries.
 * <include src="pictures/TypeHierarchies/GInteractorHierarchy.html">
 */

#ifndef _ginteractors_h
#define _ginteractors_h

#include "cslib.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "vector.h"

/*
 * Type: GInteractor
 * -----------------
 * This abstract type is the superclass for all graphical interactors.
 * In most applications, interactors will be added to a control strip
 * along one of the sides of the <code>GWindow</code>, but they can
 * also be placed in specific positions just like any other
 * <code>GObject</code>.
 */

typedef GObject GInteractor;

/*
 * Function: setActionCommand
 * Usage: setActionCommand(interactor, cmd);
 * -----------------------------------------
 * Sets the action command to the indicated string.  If the string is not
 * empty, activating the interactor generates a <code>GActionEvent</code>.
 */

void setActionCommand(GInteractor interactor, string cmd);

/*
 * Function: getActionCommand
 * Usage: cmd = getActionCommand(interactor);
 * ------------------------------------------
 * Returns the action command associated with the interactor.
 */

string getActionCommandGInteractor(GInteractor interactor);

/*
 * Type: GButton
 * -------------
 * This interactor subtype represents an onscreen button.  The following
 * program displays a button that, when pressed, generates the message
 * &ldquo;Please do not press this button again&rdquo;
 * (with thanks to Douglas Adams&rsquo;s <i>Hitchhiker&rsquo;s
 * Guide to the Galaxy</i>):
 *
 *<pre>
 *    main() {
 *       GWindow gw = newGWindow(600, 400);
 *       GButton button = newGButton("RED");
 *       addToRegion(gw, button, "SOUTH");
 *       while (true) {
 *          GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
 *          if (getEventType(e) == MOUSE_CLICKED) break;
 *          printf("Please do not press this button again.\n");
 *       }
 *    }
 *</pre>
 */

typedef GInteractor GButton;

/*
 * Function: newGButton
 * Usage: button = newGButton(label);
 * ----------------------------------
 * Creates a <code>GButton</code> with the specified label.  This
 * function also sets the action command for the button to the
 * label string.
 */

GButton newGButton(string label);

/*
 * Type: GCheckBox
 * ---------------
 * This interactor subtype represents an onscreen check box.  Clicking
 * once on the check box selects it; clicking again removes the selection.
 * If a <code>GCheckBox</code> has an action command, clicking on the box
 * generates a <code>GActionEvent</code>.
 * <include src="pictures/GInteractorDiagrams/GCheckBox.html">
 */

typedef GInteractor GCheckBox;

/*
 * Function: GCheckBox
 * Usage: chkbox = new GCheckBox(label);
 * -------------------------------------
 * Creates a <code>GCheckBox</code> with the specified label.  In contrast
 * to the <code>GButton</code>ructor, this function does not automatically
 * set an action command.
 */

GCheckBox newGCheckBox(string label);

/*
 * Function: setSelected
 * Usage: setSelected(chkbox, state);
 * ----------------------------------
 * Sets the state of the check box.
 */

void setSelected(GCheckBox chkbox, bool state);

/*
 * Function: isSelected
 * Usage: if (isSelected(chkbox)) ...
 * ----------------------------------
 * Returns <code>true</code> if the check box is selected.
 */

bool isSelected(GCheckBox chkbox);

/*
 * Type: GSlider
 * -------------
 * This interactor subtype represents an onscreen slider.  Dragging
 * the slider control generates an <code>ActionEvent</code> if the
 * slider has a nonempty action command.
 * <include src="pictures/GInteractorDiagrams/GSlider.html">
 */

typedef GInteractor GSlider;

/*
 * Function: GSlider
 * Usage: slider = new GSlider(min, max, value);
 * ---------------------------------------------
 * Creates a horizontal <code>GSlider</code>.  The parameters are
 * the minimum value, maximum value, and current value of the slider.
 * Assigning an action command to the slider causes the slider to
 * generate an action event whenever the slider value changes.
 */

GSlider newGSlider(int min, int max, int value);

/*
 * Function: setValue
 * Usage: setValue(slider, value);
 * -------------------------------
 * Sets the current value of the slider.
 */

void setValue(GSlider slider, int value);

/*
 * Function: getValue
 * Usage: value = getValue(slider);
 * --------------------------------
 * Returns the current value of the slider.
 */

int getValue(GSlider slider);

/*
 * Type: GTextField
 * ----------------
 * This interactor subtype represents a text field for entering
 * short text strings.  Hitting enter in a text field generates a
 * <code>GActionEvent</code> if the text field has a nonempty
 * action command.
 */

typedef GInteractor GTextField;

/*
 * Function: GTextField
 * Usage: field = newGTextField(nChars);
 * -------------------------------------
 * Creates a text field capable of holding <code>nChars</code> characters.
 * Assigning an action command to the text field causes it to generate an
 * action event whenever the user types the ENTER key.
 */

GTextField newGTextField(int nChars);

/*
 * Function: setText
 * Usage: setText(field, str);
 * ---------------------------
 * Sets the text of the field to the specified string.
 */

void setText(GTextField field, string str);

/*
 * Function: getText
 * Usage: str = getText(field);
 * ----------------------------
 * Returns the contents of the text field.
 */

string getText(GTextField field);

/*
 * Type: GChooser
 * --------------
 * This interactor subtype represents a selectable list.  The
 * <code>newGChooser</code> function creates an empty chooser.
 * Once the chooser has been created, clients can use <code>addItem</code>
 * to add the options.  For example, the following code creates a
 * <code>GChooser</code> containing the four strings
 * <code>"Small"</code>, <code>"Medium"</code>, <code>"Large"</code>,
 * and <code>"X-Large"</code>:
 *
 *<pre>
 *    GChooser sizeChooser = newGChooser();
 *    addItem(sizeChooser, "Small");
 *    addItem(sizeChooser, "Medium");
 *    addItem(sizeChooser, "Large");
 *    addItem(sizeChooser, "X-Large");
 *</pre>
 *<include src="pictures/GInteractorDiagrams/GChooser.html">
 */

typedef GInteractor GChooser;

/*
 * Function: newGChooser
 * Usage: chooser = newGChooser();
 * -------------------------------
 * Creates a chooser that initially contains no items, which are added
 * using the <code>addItem</code> function.  Assigning an action command
 * to the chooser causes it to generate an action event whenever the
 * user selects an item.
 */

GChooser newGChooser(void);

/*
 * Function: addItem
 * Usage: addItem(chooser, item);
 * ------------------------------
 * Adds a new item consisting of the specified string.
 */

void addItem(GChooser chooser, string item);

/*
 * Function: setSelectedItem
 * Usage: setSelectedItem(chooser, item);
 * --------------------------------------
 * Sets the chooser so that it shows the specified item.  If the item
 * does not exist in the chooser, no change occurs.
 */

void setSelectedItem(GChooser chooser, string item);

/*
 * Function: getSelectedItem
 * Usage: item = getSelectedItem(chooser);
 * ---------------------------------------
 * Returns the current item selected in the chooser.
 */

string getSelectedItem(GChooser chooser);

#endif
