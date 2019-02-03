/*
 * File: gtextarea.h
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/09/10
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gtextarea.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gtextarea_h
#define _gtextarea_h

#include <string>
#include <QSize>
#include <QTextEdit>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

class _Internal_QTextEdit;

/**
 * A GTextArea is a multi-line editable text box.
 * The box allows the user to type arbitrarily long documents.
 * Scroll bars will appear if the text becomes too long to fit in the visible
 * area of the box.
 */
class GTextArea : public GInteractor {
public:
    /**
     * Creates a new text area large enough to display the given number of rows
     * and columns of text.
     * @throw ErrorException if rows or columns value is negative
     */
    GTextArea(int rows, int columns, QWidget* parent = nullptr);

    /**
     * Creates a new text area with the given initial text.
     */
    GTextArea(const std::string& text = "", QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the text area.
     */
    virtual ~GTextArea() Q_DECL_OVERRIDE;

    /**
     * Adds formatted text to the end of the text area.
     * The text will be formatted with the given color and font.
     */
    virtual void appendFormattedText(const std::string& text, const std::string& color = "", const std::string& font = "");

    /**
     * Adds HTML-formatted text to the end of the text area.
     */
    virtual void appendHtml(const std::string& html);

    /**
     * Adds the given plain text to the end of the text area.
     */
    virtual void appendText(const std::string& text);

    /**
     * Deselects any text that is currently selected in the text area.
     */
    virtual void clearSelection();

    /**
     * Sets the text in the text area to be empty.
     */
    virtual void clearText();

    /**
     * Returns the number of visible columns (characters wide) in the text area.
     */
    virtual int getColumns() const;

    /**
     * Returns the keyboard cursor's current position in the text area as a
     * 0-based character index within the overall text string.
     */
    virtual int getCursorPosition() const;

    /**
     * Returns the text area's current text as HTML.
     * This differs from getText in that tags and formatting are not stripped.
     */
    virtual std::string getHtml() const;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the text area's placeholder text, which is usually displayed
     * as a light gray text in the text area when the area is empty.
     * This usually indicates a hint to the user about what value to type.
     * The default initial placeholder is empty.
     */
    virtual std::string getPlaceholder() const;

    /**
     * Returns the number of visible rows (lines tall) in the text area.
     * This is approximate and will be rounded down to the nearest integer if
     * the text area's size is not exactly a multiple of the row/line size.
     */
    virtual int getRows() const;

    /**
     * Returns the text that is currently selected in the text area.
     * If no text is currently selected, returns an empty string.
     */
    virtual std::string getSelectedText() const;

    /**
     * Returns the index just past the end of the current selection of text as a
     * 0-based character index within the overall text string.
     * If no text is currently selected, returns -1.
     */
    virtual int getSelectionEnd() const;

    /**
     * Returns the number of characters that are currently selected.
     * If no text is currently selected, returns 0.
     */
    virtual int getSelectionLength() const;

    /**
     * Returns the index of the start of the current selection of text as a
     * 0-based character index within the overall text string.
     * If no text is currently selected, returns -1.
     */
    virtual int getSelectionStart() const;

    /**
     * Returns the text area's current text.
     */
    virtual std::string getText() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns true if a context menu will pop up when the user right-clicks the
     * text area.
     * By default this is true, but it can be turned off by calling
     * setContextMenuEnabled(false).
     */
    virtual bool isContextMenuEnabled() const;

    /**
     * Returns whether the text area allows the user to modify its text.
     * Default true.
     */
    virtual bool isEditable() const;

    /**
     * Returns whether the text area wraps its text when a line becomes too long.
     * Default true.
     */
    virtual bool isLineWrap() const;

    /**
     * Sets the text area's keyboard cursor position to the end of the
     * current text.
     */
    virtual void moveCursorToEnd();

    /**
     * Sets the text area's keyboard cursor position to the start of the
     * current text.
     */
    virtual void moveCursorToStart();

    /**
     * Removes the key listener from this text area so that it will no longer
     * call it when the user types keys.
     */
    virtual void removeKeyListener();

    /**
     * Removes the mouse listener from this text area so that it will no longer
     * call it when the user moves/clicks the mouse.
     */
    virtual void removeMouseListener();

    /**
     * Removes the text change listener from this text area so that it will no longer
     * call it when the user modifies the text.
     */
    virtual void removeTextChangeListener();

    /**
     * Moves the visible scroll region of the text area so that the bottom part
     * of the text is visible.
     */
    virtual void scrollToBottom();

    /**
     * Moves the visible scroll region of the text area so that the top part
     * of the text is visible.
     */
    virtual void scrollToTop();

    /**
     * Sets the given range of text to be selected, beginning with the given start
     * index as a 0-based character index within the overall text string, and
     * extending for the given length of characters.
     * @throw ErrorException if start index or length is negative
     */
    virtual void select(int startIndex, int length);

    /**
     * Selects the entire text of the text area.
     */
    virtual void selectAll();

    /**
     * Sets the width of the text area to be wide enough to fit the given number
     * of characters (columns) of text.
     * @throw ErrorException if columns value is negative
     */
    virtual void setColumns(int columns);

    /**
     * Sets whether a context menu will pop up when the user right-clicks the
     * text area.
     * Initially true.
     */
    virtual void setContextMenuEnabled(bool enabled);

    /**
     * Moves the keyboard cursor to the given 0-based character index within
     * the text.
     * @throw ErrorException if index is negative
     */
    virtual void setCursorPosition(int index, bool keepAnchor = false);

    /**
     * Sets whether the text area allows the user to modify its text.
     * Default true.
     */
    virtual void setEditable(bool value);

    /**
     * Sets the text area's current text to the given HTML string.
     * This differs from setText in that HTML tags and formatting are applied
     * to the text rather than considered to be regular characters.
     */
    virtual void setHtml(const std::string& html);

    /**
     * Sets a key listener on this text area so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListener func);

    /**
     * Sets a key listener on this text area so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListenerVoid func);

    /**
     * Sets whether the text area wraps its text when a line becomes too long.
     * Default true.
     */
    virtual void setLineWrap(bool wrap);

    /**
     * Sets a mouse listener on this text area so that it will be called
     * when the user moves or clicks the mouse.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListener func);

    /**
     * Sets a mouse listener on this text area so that it will be called
     * when the user moves or clicks the mouse.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListenerVoid func);

    /**
     * Sets the text area's placeholder text, which is usually displayed
     * as a light gray text in the text area when the area is empty.
     * This usually indicates a hint to the user about what value to type.
     * The default initial placeholder is empty.
     */
    virtual void setPlaceholder(const std::string& text);

    /**
     * Sets the height of the text area to be wide enough to fit the given number
     * of lines (rows) of text.
     * @throw ErrorException if rows value is negative
     */
    virtual void setRows(int rows);

    /**
     * Sets the size of the text area to be wide enough to fit the given number
     * of lines (rows) and characters (columns) of text.
     * @throw ErrorException if rows or columns value is negative
     */
    virtual void setRowsColumns(int rows, int columns);

    /**
     * Sets the text area's current text to the given string,
     * replacing any existing text.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets a text change listener on this text area so that it will be called
     * when the user modifies the current text.
     * Any existing text change listener will be replaced.
     *
     * A text change listener is similar to a key listener in that each will
     * fire an event when the user types a character in the text area.
     * But a key listener will fire when any key is pressed, even one that does
     * not modify the text itself, such as when the user presses an arrow key
     * or the Ctrl key or Esc or any other special character.
     *
     * A text change listener will fire only when the actual text changes, such
     * as when the user types a new character into the area.
     */
    virtual void setTextChangeListener(GEventListener func);

    /**
     * Sets a text change listener on this text area so that it will be called
     * when the user modifies the current text.
     * Any existing text change listener will be replaced.
     *
     * A text change listener is similar to a key listener in that each will
     * fire an event when the user types a character in the text area.
     * But a key listener will fire when any key is pressed, even one that does
     * not modify the text itself, such as when the user presses an arrow key
     * or the Ctrl key or Esc or any other special character.
     *
     * A text change listener will fire only when the actual text changes, such
     * as when the user types a new character into the area.
     */
    virtual void setTextChangeListener(GEventListenerVoid func);

private:
    Q_DISABLE_COPY(GTextArea)

    _Internal_QTextEdit* _iqtextedit;
    bool _contextMenuEnabled;

    // helper used by getRows, getColumns, etc.
    GDimension getRowColumnSize() const;

    friend class _Internal_QTextEdit;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
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
    void handleScroll(int value);
    void handleTextChange();

private:
    GTextArea* _gtextarea;

    friend class GTextArea;
};

#endif // _gtextarea_h
