/*
 * File: gtextfield.h
 * ------------------
 *
 * @author Marty Stepp
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
#include <QSize>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QString>
#include <QWidget>
#include "ginteractor.h"
#include "vector.h"

// forward declaration
class GTextField;

// Internal class; not to be used by clients.
class _Internal_QLineEdit : public QLineEdit, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QLineEdit(GTextField* gtextField, QWidget* parent = nullptr);
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;
};

// Internal class; not to be used by clients.
class _Internal_QSpinBox : public QSpinBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSpinBox(GTextField* qgtextField, int min, int max, int step = 1, QWidget* parent = nullptr);
    virtual QLineEdit* lineEdit() const;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;
};

// Internal class; not to be used by clients.
class _Internal_QDoubleSpinBox : public QDoubleSpinBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QDoubleSpinBox(GTextField* qgtextField, double min, double max, double step = 0.1, QWidget* parent = nullptr);
    virtual QLineEdit* lineEdit() const;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleTextChange(const QString&);

private:
    GTextField* _gtextfield;
};

/*
 * ...
 */
class GTextField : public GInteractor {
public:
    enum InputType {
        INPUT_TYPE_TEXT,
        INPUT_TYPE_INTEGER,
        INPUT_TYPE_REAL
    };

    GTextField(const std::string& text = "", int charsWide = 0, QWidget* parent = nullptr);
    GTextField(int charsWide, QWidget* parent = nullptr);
    GTextField(int value, int min, int max, int step = 1, QWidget* parent = nullptr);
    GTextField(double value, double min, double max, double step, QWidget* parent = nullptr);
    virtual ~GTextField();
    virtual int getCharsWide() const;
    virtual InputType getInputType() const;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual int getMaxLength() const;
    virtual std::string getPlaceholder() const;
    virtual std::string getText() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual std::string getValue() const;
    virtual bool getValueAsBool() const;
    virtual char getValueAsChar() const;
    virtual double getValueAsDouble() const;
    virtual int getValueAsInt() const;
    virtual int getValueAsInteger() const;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isAutocompleteEnabled() const;
    virtual bool isEditable() const;
    virtual void removeActionListener();
    virtual void removeTextChangeListener();
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setAutocompleteList(std::initializer_list<std::string> strings);
    virtual void setAutocompleteList(const Vector<std::string>& strings);
    virtual void setAutocompleteEnabled(bool enabled);
    virtual void setCharsWide(int charsWide);
    virtual void setEditable(bool value);
    virtual void setMaxLength(int maxLength);
    virtual void setPlaceholder(const std::string& text);
    virtual void setText(const std::string& text);
    virtual void setTextChangeListener(GEventListener func);
    virtual void setTextChangeListener(GEventListenerVoid func);
    virtual void setValue(bool value);
    virtual void setValue(char value);
    virtual void setValue(double value);
    virtual void setValue(int value);
    virtual void setValue(const std::string& value);
    virtual bool valueIsBool() const;
    virtual bool valueIsChar() const;
    virtual bool valueIsDouble() const;
    virtual bool valueIsInt() const;
    virtual bool valueIsInteger() const;
    virtual bool valueIsReal() const;

private:
    _Internal_QLineEdit* _iqlineedit;
    _Internal_QSpinBox* _iqspinbox;
    _Internal_QDoubleSpinBox* _iqdoublespinbox;
    InputType _inputType;

    friend class _Internal_QLineEdit;
    friend class _Internal_QSpinBox;
    friend class _Internal_QDoubleSpinBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtextfield_h
