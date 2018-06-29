/*
 * File: qglabel.h
 * ---------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qglabel_h
#define _qglabel_h

#include <string>
#include <QLabel>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGLabel;

// Internal class; not to be used by clients.
class _Q_Internal_Label : public QLabel {
    Q_OBJECT

public:
    _Q_Internal_Label(QGLabel* label, QWidget* parent = nullptr);

private:
    QGLabel* _label;
};

/*
 * ...
 */
class QGLabel : public QGInteractor {
public:
    QGLabel(const std::string& text = "", QWidget* parent = nullptr);
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    void setText(const std::string& text);

private:
    _Q_Internal_Label _label;

    friend class _Q_Internal_Label;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qglabel_h
