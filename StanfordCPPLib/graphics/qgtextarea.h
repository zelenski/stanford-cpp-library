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
class _Internal_QTextEdit : public QTextEdit {
    Q_OBJECT

public:
    _Internal_QTextEdit(QGTextArea* qgtextArea, QWidget* parent = nullptr);

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
    virtual int getColumns() const;
    virtual std::string getPlaceholder() const;
    virtual int getRows() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isEditable() const;
    virtual void setColumns(int columns);
    virtual void setEditable(bool value);
    virtual void setPlaceholder(const std::string& text);
    virtual void setRows(int rows);
    virtual void setRowsColumns(int rows, int columns);
    virtual void setText(const std::string& text);
    virtual void setTextChangeHandler(std::function<void()> func);

private:
    _Internal_QTextEdit* _iqtextedit;

    GDimension getRowColumnSize() const;

    friend class _Internal_QTextEdit;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextarea_h
