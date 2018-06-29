/*
 * File: qgtextfield.h
 * -------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgtextfield_h
#define _qgtextfield_h

#include <string>
#include <QLineEdit>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGTextField;

// Internal class; not to be used by clients.
class _Q_Internal_TextField : public QLineEdit {
    Q_OBJECT

public:
    _Q_Internal_TextField(QGTextField* textField, QWidget* parent = nullptr);

//public slots:
//    void handleClick();

private:
    QGTextField* _qgtextfield;
};

/*
 * ...
 */
class QGTextField : public QGInteractor {
public:
    QGTextField(const std::string& text = "", QWidget* parent = nullptr);
    std::string getPlaceholder() const;
    std::string getText() const;
    double getValueAsDouble() const;
    int getValueAsInt() const;
    int getValueAsInteger() const;
    virtual QWidget* getWidget() const;
    bool isEditable() const;
    bool isEnabled() const;
    void setEditable(bool value);
    void setEnabled(bool enabled);
    void setPlaceholder(const std::string& text);
    void setText(const std::string& text);

    // get/setInputType

private:
    _Q_Internal_TextField _qtextfield;

    // void (* _clickHandler)();

    friend class _Q_Internal_TextField;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextfield_h
