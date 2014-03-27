/*
 * File: ginteractors.h
 * --------------------
 * This file exports a hierarchy of graphical interactors similar to those
 * provided in the Java Swing libraries.
 * <include src="pictures/ClassHierarchies/GInteractorHierarchy-h.html">
 */

#ifndef _ginteractors_h
#define _ginteractors_h

#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "vector.h"

/*
 * Class: GInteractor
 * ------------------
 * This abstract class is the superclass for all graphical interactors.
 * In most applications, interactors will be added to a control strip
 * along one of the sides of the <code>GWindow</code>, but they can
 * also be placed in specific positions just like any other
 * <code>GObject</code>.
 * <include src="pictures/ClassHierarchies/GInteractorHierarchy.html">
 */

class GInteractor : public GObject {

public:

/*
 * Method: setActionCommand
 * Usage: interactor.setActionCommand(cmd);
 * ----------------------------------------
 * Sets the action command to the indicated string.  If the string is not
 * empty, activating the interactor generates a <code>GActionEvent</code>.
 */

   void setActionCommand(std::string cmd);

/*
 * Method: getActionCommand
 * Usage: string cmd = interactor.getActionCommand();
 * --------------------------------------------------
 * Returns the action command associated with the interactor.
 */

   std::string getActionCommand();

/*
 * Method: setSize
 * Usage: interactor.setSize(size);
 *        interactor.setSize(width, height);
 * -----------------------------------------
 * Changes the size of the interactor to the specified width and height.
 */

   void setSize(const GDimension & size);
   void setSize(double width, double height);

/*
 * Method: setBounds
 * Usage: interactor.setBounds(rect);
 *        interactor.setBounds(x, y, width, height);
 * -------------------------------------------------
 * Changes the bounds of the interactor to the specified values.
 */

   void setBounds(const GRectangle & size);
   void setBounds(double x, double y, double width, double height);

/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;

protected:

   GInteractor();

   std::string actionCommand;

};

/*
 * Class: GButton
 * --------------
 * This interactor subclass represents an onscreen button.  The following
 * program displays a button that, when pressed, generates the message
 * &ldquo;Please do not press this button again&rdquo;
 * (with thanks to Douglas Adams&rsquo;s <i>Hitchhiker&rsquo;s
 * Guide to the Galaxy</i>):
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       GButton *button = new GButton("RED");
 *       gw.addToRegion(button, "SOUTH");
 *       while (true) {
 *          GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Please do not press this button again." << endl;
 *       }
 *       return 0;
 *    }
 *</pre>
 */

class GButton : public GInteractor {

public:

/*
 * Constructor: GButton
 * Usage: GButton *button = new GButton(label);
 * --------------------------------------------
 * Creates a <code>GButton</code> with the specified label.  This
 * constructor also sets the action command for the button to the
 * label string.
 */

   GButton(std::string label);

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   std::string label;

};

/*
 * Class: GCheckBox
 * ----------------
 * This interactor subclass represents an onscreen check box.  Clicking
 * once on the check box selects it; clicking again removes the selection.
 * If a <code>GCheckBox</code> has an action command, clicking on the box
 * generates a <code>GActionEvent</code>.
 * <include src="pictures/GInteractorDiagrams/GCheckBox.html">
 */

class GCheckBox : public GInteractor {

public:

/*
 * Constructor: GCheckBox
 * Usage: GCheckBox *chkbox = new GCheckBox(label);
 * ------------------------------------------------
 * Creates a <code>GCheckBox</code> with the specified label.  In contrast
 * to the <code>GButton</code> constructor, this constructor does not set
 * an action command.
 */

   GCheckBox(std::string label);

/*
 * Method: setSelected
 * Usage: chkbox->setSelected(state);
 * ----------------------------------
 * Sets the state of the check box.
 */

   void setSelected(bool state);

/*
 * Method: isSelected
 * Usage: if (chkbox->isSelected()) ...
 * ------------------------------------
 * Returns <code>true</code> if the check box is selected.
 */

   bool isSelected();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   std::string label;

};

/*
 * Class: GSlider
 * --------------
 * This interactor subclass represents an onscreen slider.  Dragging
 * the slider control generates an <code>ActionEvent</code> if the
 * slider has a nonempty action command.
 * <include src="pictures/GInteractorDiagrams/GSlider.html">
 */

class GSlider : public GInteractor {

public:

/*
 * Constructor: GSlider
 * Usage: GSlider *slider = new GSlider();
 *        GSlider *slider = new GSlider(min, max, value);
 * ------------------------------------------------------
 * Creates a horizontal <code>GSlider</code>.  The second form allows
 * the client to specify the minimum value, maximum value, and current
 * value of the slider.  The first form is equivalent to calling
 * <code>GSlider(0, 100, 50)</code>.  Assigning an action command
 * to the slider causes the slider to generate an action event whenever
 * the slider value changes.
 */

   GSlider();
   GSlider(int min, int max, int value);

/*
 * Method: setValue
 * Usage: slider->setValue(value);
 * -------------------------------
 * Sets the current value of the slider.
 */

   void setValue(int value);

/*
 * Method: getValue
 * Usage: int value = slider->getValue();
 * --------------------------------------
 * Returns the current value of the slider.
 */

   int getValue();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   void create(int min, int max, int value);
   int min;
   int max;

};

/*
 * Class: GTextField
 * -----------------
 * This interactor subclass represents a text field for entering short
 * text strings.  Hitting enter in a text field generates a
 * <code>GActionEvent</code> if the text field has a nonempty action command.

 */

class GTextField : public GInteractor {

public:

/*
 * Constructor: GTextField
 * Usage: GTextField *field = new GTextField();
 *        GTextField *field = new GTextField(nChars);
 * --------------------------------------------------
 * Creates a text field capable of holding <code>nChars</code> characters,
 * which defaults to 10.  Assigning an action command to the text field
 * causes it to generate an action event whenever the user types the
 * ENTER key.
 */

   GTextField();
   GTextField(int nChars);

/*
 * Method: setText
 * Usage: field->setText(str);
 * ---------------------------
 * Sets the text of the field to the specified string.
 */

   void setText(std::string str);

/*
 * Method: getText
 * Usage: string str = field->getText();
 * -------------------------------------
 * Returns the contents of the text field.
 */

   std::string getText();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

};

/*
 * Class: GChooser
 * ---------------
 * This interactor subclass represents a selectable list.  The
 * <code>GChooser</code> constructor creates an empty chooser.
 * Once the chooser has been created, clients can use <code>addItem</code>
 * to add the options.  For example, the following code creates a
 * <code>GChooser</code> containing the four strings
 * <code>"Small"</code>, <code>"Medium"</code>, <code>"Large"</code>,
 * and <code>"X-Large"</code>:
 *
 *<pre>
 *    GChooser *sizeChooser = new GChooser();
 *    sizeChooser->addItem("Small");
 *    sizeChooser->addItem("Medium");
 *    sizeChooser->addItem("Large");
 *    sizeChooser->addItem("X-Large");
 *</pre>
 *<include src="pictures/GInteractorDiagrams/GChooser.html">
 */

class GChooser : public GInteractor {

public:

/*
 * Constructor: GChooser
 * Usage: GChooser *chooser = new GChooser();
 * ------------------------------------------
 * Creates a chooser that initially contains no items, which are added
 * using the <code>addItem</code> method.  Assigning an action command
 * to the chooser causes it to generate an action event whenever the
 * user selects an item.
 */

   GChooser();

/*
 * Method: addItem
 * Usage: chooser->addItem(item);
 * ------------------------------
 * Adds a new item consisting of the specified string.
 */

   void addItem(std::string item);

/*
 * Method: setSelectedItem
 * Usage: chooser->setSelectedItem(item);
 * --------------------------------------
 * Sets the chooser so that it shows the specified item.  If the item
 * does not exist in the chooser, no change occurs.
 */

   void setSelectedItem(std::string item);

/*
 * Method: getSelectedItem
 * Usage: string item = chooser->getSelectedItem();
 * ------------------------------------------------
 * Returns the current item selected in the chooser.
 */

   std::string getSelectedItem();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

};

#endif
