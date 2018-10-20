/*
 * File: gbrowserpane.h
 * --------------------
 *
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
     * Returns the MIME content type for the current page.
     * The default content type is "text/html".
     * (If you need to look up the content type for a given file/page extension,
     * consider using the HttpServer::getContentType(extension) function.)
     */
    virtual std::string getContentType() const;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the URL of the web page or file name being currently viewed.
     * If no page or file has been loaded, returns an empty string.
     */
    virtual std::string getPageUrl() const;

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
     * Removes the link listener from the canvas so that it will no longer
     * call it when hyperlink events occur.
     */
    virtual void removeLinkListener();

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
     * Sets the pane to display to the given contents using its current content type.
     * For example, if you build your own string of HTML or text content and want
     * to display it in the pane without saving it to a file, this is the method to use.
     */
    virtual void setText(const std::string& text);

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
