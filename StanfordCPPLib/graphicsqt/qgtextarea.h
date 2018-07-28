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
    virtual void contextMenuEvent(QContextMenuEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
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
    virtual int getCursorPosition() const;
    virtual std::string getHtml() const;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getPlaceholder() const;
    virtual int getRows() const;
    virtual std::string getSelectedText() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isContextMenuEnabled() const;
    virtual bool isEditable() const;
    virtual bool isLineWrap() const;
    virtual void moveCursorToEnd();
    virtual void moveCursorToStart();
    virtual void removeKeyListener();
    virtual void removeMouseListener();
    virtual void removeTextChangeListener();
    virtual void scrollToBottom();
    virtual void scrollToTop();
    virtual void setColumns(int columns);
    virtual void setContextMenuEnabled(bool enabled);
    virtual void setCursorPosition(int index);
    virtual void setEditable(bool value);
    virtual void setHtml(const std::string& html);
    virtual void setKeyListener(QGEventListener func);
    virtual void setKeyListener(QGEventListenerVoid func);
    virtual void setMouseListener(QGEventListener func);
    virtual void setMouseListener(QGEventListenerVoid func);
    virtual void setPlaceholder(const std::string& text);
    virtual void setRows(int rows);
    virtual void setRowsColumns(int rows, int columns);
    virtual void setText(const std::string& text);
    virtual void setLineWrap(bool wrap);
    virtual void setTextChangeListener(QGEventListener func);
    virtual void setTextChangeListener(QGEventListenerVoid func);

private:
    _Internal_QTextEdit* _iqtextedit;
    bool _contextMenuEnabled;

    GDimension getRowColumnSize() const;

    friend class _Internal_QTextEdit;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtextarea_h
