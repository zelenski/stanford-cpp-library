/*
 * File: gbrowserpane.h
 * --------------------
 * This file declares the <code>GBrowserPane</code> class, which is a
 * graphical interactor that displays a web page.
 *
 * @version 2018/12/28
 * - added methods for text selection, scrolling, cursor position, key/mouse listeners
 * @version 2018/09/17
 * - fixed thread safety bugs
 * - added link listener events
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gbrowserpane.h to replace Java version
 * @version 2018/07/15
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gbrowserpane_h
#define _gbrowserpane_h

#include <string>
#include <QSize>
#include <QTextBrowser>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

class _Internal_QTextBrowser;

/**
 * A GBrowserPane is a graphical interactor that displays a web page.
 * This interactor is a wrapping around the Qt QTextBrowser widget, which is
 * able to display rich content such as HTML pages.
 *
 * You can use GBrowserPane to implement the core rendering engine of a basic
 * web browser, though it does not support all web browser features such as
 * JavaScript content, secure sessions, or cookies.
 */
class GBrowserPane : public GInteractor {
public:
    /**
     * Constructs a new browser pane.
     * If a URL string is passed, loads the data from that URL.
     * Otherwise, the pane is initially blank.
     */
    GBrowserPane(const std::string& url = "", QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the browser pane.
     */
    virtual ~GBrowserPane() Q_DECL_OVERRIDE;

    /**
     * Deselects any text that is currently selected in the pane.
     */
    virtual void clearSelection();

    /**
     * Sets the text in the pane to be empty.
     */
    virtual void clearText();

    /**
     * Returns the MIME content type for the current page.
     * The default content type is "text/html".
     * (If you need to look up the content type for a given file/page extension,
     * consider using the HttpServer::getContentType(extension) function.)
     */
    virtual std::string getContentType() const;

    /**
     * Returns the keyboard cursor's current position in the text pane as a
     * 0-based character index within the overall text string.
     */
    virtual int getCursorPosition() const;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the URL of the web page or file name being currently viewed.
     * If no page or file has been loaded, returns an empty string.
     */
    virtual std::string getPageUrl() const;

    /**
     * Returns the text that is currently selected in the text pane.
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
     * Returns the full text of the current page or file being displayed in the pane.
     * This could be a fairly long string, depending on the page.
     * Initially an empty string if no page or file has yet been loaded.
     */
    virtual std::string getText() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns whether the text pane allows the user to modify its text.
     * Default false.
     */
    virtual bool isEditable() const;

    /**
     * Returns whether the text pane wraps its text when a line becomes too long.
     * Default true.
     */
    virtual bool isLineWrap() const;

    /**
     * Sets the text pane's keyboard cursor position to the end of the
     * current text.
     */
    virtual void moveCursorToEnd();

    /**
     * Sets the text pane's keyboard cursor position to the start of the
     * current text.
     */
    virtual void moveCursorToStart();

    /**
     * Reads text from the given file and displays the entire file's text as the
     * contents of this formatted pane.
     * The pane will try to display the content in the best appropriate format,
     * such as rendering basic HTML content with formatting intact.
     * If the file cannot be read, sets the pane's text to be empty.
     */
    virtual void readTextFromFile(std::istream& file);

    /**
     * Reads text from the given file and displays the entire file's text as the
     * contents of this formatted pane.
     * The pane will try to display the content in the best appropriate format,
     * such as rendering basic HTML content with formatting intact.
     * If the file does not exist or cannot be read, sets the pane's text to be empty.
     */
    virtual void readTextFromFile(const std::string& filename);

    /**
     * Reads text from the given web page URL and displays the entire page's
     * text as the contents of this formatted pane.
     * If the page does not exist or cannot be read, sets the pane's text to be empty.
     */
    virtual void readTextFromUrl(const std::string& url);

    /**
     * Removes the key listener from this text pane so that it will no longer
     * call it when the user types keys.
     */
    virtual void removeKeyListener();

    /**
     * Removes the link listener from the canvas so that it will no longer
     * call it when hyperlink events occur.
     */
    virtual void removeLinkListener();

    /**
     * Removes the mouse listener from this text pane so that it will no longer
     * call it when the user moves/clicks the mouse.
     */
    virtual void removeMouseListener();

    /**
     * Removes the text change listener from this text pane so that it will no longer
     * call it when the user modifies the text.
     */
    virtual void removeTextChangeListener();

    /**
     * Moves the visible scroll region of the text pane so that the bottom part
     * of the text is visible.
     */
    virtual void scrollToBottom();

    /**
     * Moves the visible scroll region of the text pane so that the top part
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
     * Selects the entire text of the text pane.
     */
    virtual void selectAll();

    /**
     * Sets the MIME content type being used to display the current/future pages.
     * The default content type is "text/html".
     * The suggested use of this function would be to call it just before calling
     * loadTextFromFile or loadTextFromUrl.
     * (If you need to look up the content type for a given file/page extension,
     * consider using the HttpServer::getContentType(extension) function.)
     */
    virtual void setContentType(const std::string& contentType);

    /**
     * Moves the keyboard cursor to the given 0-based character index within
     * the text.
     * @throw ErrorException if index is negative
     */
    virtual void setCursorPosition(int index, bool keepAnchor = false);

    /**
     * Sets whether the text pane allows the user to modify its text.
     * Default true.
     */
    virtual void setEditable(bool value);

    /**
     * Sets a key listener on this text pane so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListener func);

    /**
     * Sets a key listener on this text pane so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListenerVoid func);

    /**
     * Sets whether the text pane wraps its text when a line becomes too long.
     * Default true.
     */
    virtual void setLineWrap(bool wrap);

    /**
     * Sets a link listener on this canvas so that it will be called
     * when the user clicks on hyperlinks on the pane.
     * Any existing mouse listener will be replaced.
     */
    virtual void setLinkListener(GEventListener func);

    /**
     * Sets a link listener on this canvas so that it will be called
     * when the user clicks on hyperlinks on the pane.
     * Any existing mouse listener will be replaced.
     */
    virtual void setLinkListener(GEventListenerVoid func);

    /**
     * Sets a mouse listener on this text pane so that it will be called
     * when the user moves or clicks the mouse.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListener func);

    /**
     * Sets a mouse listener on this text pane so that it will be called
     * when the user moves or clicks the mouse.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListenerVoid func);

    /**
     * Sets the pane to display to the given contents using its current content type.
     * For example, if you build your own string of HTML or text content and want
     * to display it in the pane without saving it to a file, this is the method to use.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets a text change listener on this text pane so that it will be called
     * when the user modifies the current text.
     * Any existing text change listener will be replaced.
     *
     * A text change listener is similar to a key listener in that each will
     * fire an event when the user types a character in the text pane.
     * But a key listener will fire when any key is pressed, even one that does
     * not modify the text itself, such as when the user presses an arrow key
     * or the Ctrl key or Esc or any other special character.
     *
     * A text change listener will fire only when the actual text changes, such
     * as when the user types a new character into the area.
     */
    virtual void setTextChangeListener(GEventListener func);

    /**
     * Sets a text change listener on this text pane so that it will be called
     * when the user modifies the current text.
     * Any existing text change listener will be replaced.
     *
     * A text change listener is similar to a key listener in that each will
     * fire an event when the user types a character in the text pane.
     * But a key listener will fire when any key is pressed, even one that does
     * not modify the text itself, such as when the user presses an arrow key
     * or the Ctrl key or Esc or any other special character.
     *
     * A text change listener will fire only when the actual text changes, such
     * as when the user types a new character into the area.
     */
    virtual void setTextChangeListener(GEventListenerVoid func);

private:
    Q_DISABLE_COPY(GBrowserPane)

    std::string _pageUrl;   // url/filename of the most recently loaded page
    std::string _contentType;
    _Internal_QTextBrowser* _iqtextbrowser;

    friend class _Internal_QTextBrowser;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QTextBrowser : public QTextBrowser, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTextBrowser(GBrowserPane* gbrowserpane, QWidget* parent = nullptr);
    QVariant loadResource(int type, const QUrl &url) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    GBrowserPane* _gbrowserpane;
    QString _clickedLink;
};

#endif // _gbrowserpane_h
