/*
 * File: qgtextarea.h
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgtextarea_h
#define _qgtextarea_h

#include <string>
#include <QTextEdit>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGTextArea;

// Internal class; not to be used by clients.
class _Q_Internal_TextArea : public QTextEdit {
    Q_OBJECT

public:
    _Q_Internal_TextArea(QGTextArea* textArea, QWidget* parent = nullptr);

public slots:
    void handleTextChange();

private:
    QGTextArea* _qgtextarea;
};

/*
 * ...
 */
class QGTextArea : public QGInteractor {
public:
    QGTextArea(const std::string& text = "", QWidget* parent = nullptr);
    std::string getPlaceholder() const;
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    bool isEditable() const;
    void setEditable(bool value);
    void setPlaceholder(const std::string& text);
    void setText(const std::string& text);
    void setTextChangeHandler(std::function<void()> func);

private:
    _Q_Internal_TextArea _qtextarea;

    friend class _Q_Internal_TextArea;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextarea_h
