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
class _Q_Internal_TextField : public QLineEdit {
    Q_OBJECT

public:
    _Q_Internal_TextField(QGTextField* textField, QWidget* parent = nullptr);

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
    int getCharsWide() const;
    int getMaxLength() const;
    std::string getPlaceholder() const;
    std::string getText() const;
    virtual std::string getType() const;
    std::string getValue() const;
    bool getValueAsBool() const;
    char getValueAsChar() const;
    double getValueAsDouble() const;
    int getValueAsInt() const;
    int getValueAsInteger() const;
    virtual QWidget* getWidget() const;
    bool isAutocompleteEnabled() const;
    bool isEditable() const;
    void setAutocompleteList(std::initializer_list<std::string> strings);
    void setAutocompleteList(const Vector<std::string>& strings);
    void setAutocompleteEnabled(bool enabled);
    void setCharsWide(int charsWide);
    void setEditable(bool value);
    void setMaxLength(int maxLength);
    void setPlaceholder(const std::string& text);
    void setText(const std::string& text);
    void setTextChangeHandler(std::function<void()> func);
    void setValue(bool value);
    void setValue(char value);
    void setValue(double value);
    void setValue(int value);
    void setValue(const std::string& value);
    bool valueIsBool() const;
    bool valueIsChar() const;
    bool valueIsDouble() const;
    bool valueIsInt() const;
    bool valueIsInteger() const;
    bool valueIsReal() const;
    // TODO: get/setInputType?

private:
    _Q_Internal_TextField _qtextfield;

    friend class _Q_Internal_TextField;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextfield_h
