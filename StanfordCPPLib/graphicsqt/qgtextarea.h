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
#include <QSize>
#include <QTextEdit>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGTextArea;

// Internal class; not to be used by clients.
class _Internal_QTextEdit : public QTextEdit, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTextEdit(QGTextArea* qgtextArea, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

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
    QGTextArea(int rows, int columns, QWidget* parent = nullptr);
    QGTextArea(const std::string& text = "", QWidget* parent = nullptr);
    virtual ~QGTextArea();
    virtual void appendFormattedText(const std::string& text, const std::string& color = "", const std::string& font = "");
    virtual void appendHtml(const std::string& html);
    virtual void appendText(const std::string& text);
    virtual void clearText();
    virtual int getColumns() const;
    virtual std::string getHtml() const;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getPlaceholder() const;
    virtual int getRows() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isEditable() const;
    virtual void removeTextChangeHandler();
    virtual void setColumns(int columns);
    virtual void setEditable(bool value);
    virtual void setHtml(const std::string& html);
    virtual void setPlaceholder(const std::string& text);
    virtual void setRows(int rows);
    virtual void setRowsColumns(int rows, int columns);
    virtual void setText(const std::string& text);
    virtual void setTextChangeHandler(QGEventHandler func);
    virtual void setTextChangeHandler(QGEventHandlerVoid func);

private:
    _Internal_QTextEdit* _iqtextedit;

    GDimension getRowColumnSize() const;

    friend class _Internal_QTextEdit;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextarea_h
