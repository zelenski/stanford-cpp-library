/*
 * File: qgtextfield.h
 * -------------------
 *
 * @version 2018/06/29
 * - added textChange event
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgtextfield_h
#define _qgtextfield_h

#include <initializer_list>
#include <string>
#include <QLineEdit>
#include <QString>
#include <QWidget>
#include "qginteractor.h"
#include "vector.h"

// forward declaration
class QGTextField;

// Internal class; not to be used by clients.
class _Internal_QLineEdit : public QLineEdit {
    Q_OBJECT

public:
    _Internal_QLineEdit(QGTextField* qgtextField, QWidget* parent = nullptr);

public slots:
    void handleTextChange(const QString&);

private:
    QGTextField* _qgtextfield;
};

/*
 * ...
 */
class QGTextField : public QGInteractor {
public:
    QGTextField(const std::string& text = "", QWidget* parent = nullptr);
    QGTextField(const std::string& text, int charsWide, QWidget* parent = nullptr);
    QGTextField(int charsWide, QWidget* parent = nullptr);
    virtual ~QGTextField();
    virtual int getCharsWide() const;
    virtual int getMaxLength() const;
    virtual std::string getPlaceholder() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual std::string getValue() const;
    virtual bool getValueAsBool() const;
    virtual char getValueAsChar() const;
    virtual double getValueAsDouble() const;
    virtual int getValueAsInt() const;
    virtual int getValueAsInteger() const;
    virtual QWidget* getWidget() const;
    virtual bool isAutocompleteEnabled() const;
    virtual bool isEditable() const;
    virtual void setAutocompleteList(std::initializer_list<std::string> strings);
    virtual void setAutocompleteList(const Vector<std::string>& strings);
    virtual void setAutocompleteEnabled(bool enabled);
    virtual void setCharsWide(int charsWide);
    virtual void setEditable(bool value);
    virtual void setMaxLength(int maxLength);
    virtual void setPlaceholder(const std::string& text);
    virtual void setText(const std::string& text);
    virtual void setTextChangeHandler(std::function<void()> func);
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

    friend class _Internal_QLineEdit;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextfield_h
