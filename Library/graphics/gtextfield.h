/*
 * File: gtextfield.h
 * ------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key events
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gtextfield.h to replace Java version
 * @version 2018/06/29
 * - added textChange event
 * @version 2018/06/25
 * - initial version
 */


#ifndef _gtextfield_h
#define _gtextfield_h

#include <initializer_list>
#include <string>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QString>

#include "ginteractor.h"
#include "vector.h"

class _Internal_QLineEdit;
class _Internal_QSpinBox;
class _Internal_QDoubleSpinBox;

/**
 * This interactor subclass represents a text field for entering short text strings.
 * Pressing Enter in a text field generates an action event.
 */
class GTextField : public GInteractor {
public:
    /**
     * Constants for the valid types of text field input.
     */
    enum InputType {
        INPUT_TYPE_TEXT,
        INPUT_TYPE_INTEGER,
        INPUT_TYPE_REAL
    };

    /**
     * Creates a text field with the given initial text.
     * If the optional charsWide parameter is passed, sizes the text field
     * wide enough to display the given number of characters.
     */
    GTextField(const std::string& text = "", int charsWide = 0, QWidget* parent = nullptr);

    /**
     * Creates a text field wide enough to display the given number of characters.
     */
    GTextField(int charsWide, QWidget* parent = nullptr);

    /**
     * Creates a text field for entering integer values, with the given initial value.
     * The value is constrained to the given minimum and maximum, incrementing by
     * the given step amount.
     * @throw ErrorException if min > max or value is not between min and max
     */
    GTextField(int value, int min, int max, int step = 1, QWidget* parent = nullptr);

    /**
     * Creates a text field for entering real number values, with the given initial value.
     * The value is constrained to the given minimum and maximum, incrementing by
     * the given step amount.
     * @throw ErrorException if min > max or value is not between min and max
     */
    GTextField(double value, double min, double max, double step, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the text field.
     */
    ~GTextField() override;

    /**
     * Returns the number of characters that can fit in the visible area of
     * this text field.
     */
    virtual int getCharsWide() const;

    /**
     * Returns the type of input accepted by this text field.
     * The default is text input (GTextField::INPUT_TYPE_TEXT), but you can create
     * a field that accepts integers (INPUT_TYPE_INTEGER) or real numbers
     * (INPUT_TYPE_REAL).
     */
    virtual InputType getInputType() const;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /**
     * Returns the maximum length of string allowed in the text field.
     * By default no max is set, in which case this method returns 0.
     */
    virtual int getMaxLength() const;

    /**
     * Returns the text field's placeholder text, which is usually displayed
     * as a light gray text in the field when the field is empty.
     * This usually indicates a hint to the user about what value to type.
     */
    virtual std::string getPlaceholder() const;

    /**
     * Returns the text field's current text.
     * Equivalent to getValue.
     */
    virtual std::string getText() const;

    /* @inherit */
    std::string getType() const override;

    /**
     * Returns the text field's current text.
     * Equivalent to getText.
     */
    virtual std::string getValue() const;

    /**
     * Returns the currently typed value in the text field,
     * interpreted as a bool value of true or false.
     * See the stringToBool function in strlib.h for more information about
     * how the string is converted to a bool value.
     * @throw ErrorException if the string cannot be converted to bool type
     */
    virtual bool getValueAsBool() const;

    /**
     * Returns the currently typed value in the text field as a char value.
     * This returns the first character of the string, or '\0' if the text
     * field is empty.
     */
    virtual char getValueAsChar() const;

    /**
     * Returns the currently typed value in the text field,
     * interpreted as a real number value.
     * See the stringToDouble function in strlib.h for more information about
     * how the string is converted to a numeric value.
     * @throw ErrorException if the string cannot be converted to double type
     */
    virtual double getValueAsDouble() const;

    /**
     * Returns the currently typed value in the text field,
     * interpreted as an integer value.
     * See the stringToInteger function in strlib.h for more information about
     * how the string is converted to a numeric value.
     * Equivalent to getValueAsInteger.
     * @throw ErrorException if the string cannot be converted to integer type
     */
    virtual int getValueAsInt() const;

    /**
     * Returns the currently typed value in the text field,
     * interpreted as an integer value.
     * See the stringToInteger function in strlib.h for more information about
     * how the string is converted to a numeric value.
     * Equivalent to getValueAsInteger.
     * @throw ErrorException if the string cannot be converted to integer type
     */
    virtual int getValueAsInteger() const;

    /* @inherit */
    QWidget* getWidget() const override;

    /**
     * Returns true if this text field has an autocompletion list of options
     * that will pop up as the user begins typing.
     * Text fields do not initially have such a list, but you can supply one
     * by calling setAutocompleteList.
     */
    virtual bool isAutocompleteEnabled() const;

    /**
     * Returns true if the text field's value can be edited.
     * Initially this is true but can be changed by calling
     * setEditable(false).
     */
    virtual bool isEditable() const;

    /**
     * Removes the text change listener from this text field so that it will no longer
     * call it when the user types characters.
     */
    virtual void removeTextChangeListener();

    /**
     * Sets the given list of strings to be used as an autocompletion list for
     * this text field.  After calling this, if the user types characters in
     * the text field that form a prefix of any string in the list, those strings
     * will be shown as a drop-down autocompletion list that the user can choose
     * completed values from.
     * To turn this feature back off, call setAutocompleteEnabled(false).
     */
    virtual void setAutocompleteList(std::initializer_list<std::string> strings);

    /**
     * Sets the given list of strings to be used as an autocompletion list for
     * this text field.  After calling this, if the user types characters in
     * the text field that form a prefix of any string in the list, those strings
     * will be shown as a drop-down autocompletion list that the user can choose
     * completed values from.
     * To turn this feature back off, call setAutocompleteEnabled(false).
     */
    virtual void setAutocompleteList(const Vector<std::string>& strings);

    /**
     * Sets whether the autocompletion feature is enabled for this text field.
     * If you call setAutocompleteList, the autocompletion feature is automatically
     * enabled; so the main purpose of this method is to turn it back off later.
     */
    virtual void setAutocompleteEnabled(bool enabled);

    /**
     * Sets the width of this text field to be exactly wide enough to display
     * the given number of characters.
     */
    virtual void setCharsWide(int charsWide);

    /**
     * Sets whether the value in the text box can be edited.
     * Initially true.
     */
    virtual void setEditable(bool value);

    /**
     * Sets the maximum number of characters that can be typed into the field.
     */
    virtual void setMaxLength(int maxLength);

    /**
     * Sets a gray message that is displayed in the background of the text field
     * before the user has typed any value.
     * This is often used as a hint to the user about what kind of value to type.
     */
    virtual void setPlaceholder(const std::string& text);

    /**
     * Sets the current text value in the text field.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets a text-change listener on this text field so that it will be called
     * when the value in the field changes, which will occur on every key press.
     * Any existing text-change listener will be replaced.
     */
    virtual void setTextChangeListener(GEventListener func);

    /**
     * Sets a text-change listener on this text field so that it will be called
     * when the value in the field changes, which will occur on every key press.
     * Any existing text-change listener will be replaced.
     */
    virtual void setTextChangeListener(GEventListenerVoid func);

    /**
     * Sets the current text value in the text field to the string representation
     * of the given value.
     */
    virtual void setValue(bool value);

    /**
     * Sets the current text value in the text field to the string representation
     * of the given value.
     */
    virtual void setValue(char value);

    /**
     * Sets the current text value in the text field to the string representation
     * of the given value.
     */
    virtual void setValue(double value);

    /**
     * Sets the current text value in the text field to the string representation
     * of the given value.
     */
    virtual void setValue(int value);

    /**
     * Sets the current text value in the text field to the string representation
     * of the given value.
     * Equivalent to setText.
     */
    virtual void setValue(const std::string& value);

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as a bool value of true or false.
     * If this is true, a call to getValueAsBool will succeed.
     */
    virtual bool valueIsBool() const;

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as a char value.
     * This will be true if its length is exactly 1.
     * If this is true, a call to getValueAsChar will succeed.
     */
    virtual bool valueIsChar() const;

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as a real number.
     * If this is true, a call to getValueAsDouble will succeed.
     * Equivalent to valueIsReal.
     */
    virtual bool valueIsDouble() const;

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as an integer.
     * If this is true, a call to getValueAsInt / getValueAsInteger will succeed.
     * Equivalent to valueIsInteger.
     */
    virtual bool valueIsInt() const;

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as an integer.
     * If this is true, a call to getValueAsInt / getValueAsInteger will succeed.
     * Equivalent to valueIsInt.
     */
    virtual bool valueIsInteger() const;

    /**
     * Returns true if the currently typed value in the text field can be
     * interpreted as a real number.
     * If this is true, a call to getValueAsDouble will succeed.
     * Equivalent to valueIsDouble.
     */
    virtual bool valueIsReal() const;

protected:
    /**
     * @private
     */
    std::string getActionEventType() const override;

private:
    Q_DISABLE_COPY(GTextField)

    // pointers to the internal Qt text field;
    // at most one of these will be non-null for a given instance
    _Internal_QLineEdit* _iqlineedit;
    _Internal_QSpinBox* _iqspinbox;
    _Internal_QDoubleSpinBox* _iqdoublespinbox;

    // type of text field; helps tell us which of the above internal Qt widgets
    // will be non-null
    InputType _inputType;

    friend class _Internal_QLineEdit;
    friend class _Internal_QSpinBox;
    friend class _Internal_QDoubleSpinBox;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QLineEdit : public QLineEdit, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QLineEdit(GTextField* gtextField, QWidget* parent = nullptr);
    void detach() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    QSize sizeHint() const override;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;

    friend class GTextField;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QSpinBox : public QSpinBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSpinBox(GTextField* qgtextField, int min, int max, int step = 1, QWidget* parent = nullptr);
    void detach() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    virtual QLineEdit* lineEdit() const;
    QSize sizeHint() const override;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;

    friend class GTextField;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QDoubleSpinBox : public QDoubleSpinBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QDoubleSpinBox(GTextField* qgtextField, double min, double max, double step = 0.1, QWidget* parent = nullptr);
    void detach() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    virtual QLineEdit* lineEdit() const;
    QSize sizeHint() const override;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;

    friend class GTextField;
};

#endif // _gtextfield_h
