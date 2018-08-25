/*
 * File: gtextarea.h
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gtextarea.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gtextarea_h
#define _gtextarea_h

#include <string>
#include <QSize>
#include <QTextEdit>
#include <QWidget>
#include "ginteractor.h"

// forward declaration
class GTextArea;

// Internal class; not to be used by clients.
class _Internal_QTextEdit : public QTextEdit, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTextEdit(GTextArea* gtextArea, QWidget* parent = nullptr);
    virtual void contextMenuEvent(QContextMenuEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleTextChange();

private:
    GTextArea* _gtextarea;
};

/*
 * ...
 */
class GTextArea : public GInteractor {
public:
    GTextArea(int rows, int columns, QWidget* parent = nullptr);
    GTextArea(const std::string& text = "", QWidget* parent = nullptr);
    virtual ~GTextArea();
    virtual void appendFormattedText(const std::string& text, const std::string& color = "", const std::string& font = "");
    virtual void appendHtml(const std::string& html);
    virtual void appendText(const std::string& text);
    virtual void clearSelection();
    virtual void clearText();
    virtual int getColumns() const;
    virtual int getCursorPosition() const;
    virtual std::string getHtml() const;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getPlaceholder() const;
    virtual int getRows() const;
    virtual std::string getSelectedText() const;
    virtual int getSelectionEnd() const;
    virtual int getSelectionLength() const;
    virtual int getSelectionStart() const;
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
    virtual void select(int startIndex, int length);
    virtual void selectAll();
    virtual void setColumns(int columns);
    virtual void setContextMenuEnabled(bool enabled);
    virtual void setCursorPosition(int index, bool keepAnchor = false);
    virtual void setEditable(bool value);
    virtual void setHtml(const std::string& html);
    virtual void setKeyListener(GEventListener func);
    virtual void setKeyListener(GEventListenerVoid func);
    virtual void setMouseListener(GEventListener func);
    virtual void setMouseListener(GEventListenerVoid func);
    virtual void setPlaceholder(const std::string& text);
    virtual void setRows(int rows);
    virtual void setRowsColumns(int rows, int columns);
    virtual void setText(const std::string& text);
    virtual void setLineWrap(bool wrap);
    virtual void setTextChangeListener(GEventListener func);
    virtual void setTextChangeListener(GEventListenerVoid func);

private:
    _Internal_QTextEdit* _iqtextedit;
    bool _contextMenuEnabled;

    GDimension getRowColumnSize() const;

    friend class _Internal_QTextEdit;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtextarea_h
