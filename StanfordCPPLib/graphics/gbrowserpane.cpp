/*
 * File: gbrowserpane.cpp
 * ----------------------
 *
 * @version 2018/09/17
 * - fixed thread safety bugs
 * - added link listener events
 * @version 2018/08/23
 * - renamed to gbrowserpane.h to replace Java version
 * @version 2018/07/15
 * - initial version
 */

#include "gbrowserpane.h"
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "gthread.h"
#include "require.h"
#include "server.h"
#include "strlib.h"

GBrowserPane::GBrowserPane(const std::string& url, QWidget* parent) {
    GThread::runOnQtGuiThread([this, url, parent]() {
        _iqtextbrowser = new _Internal_QTextBrowser(this, getInternalParent(parent));
    });
    if (!url.empty()) {
        readTextFromUrl(url);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GBrowserPane::~GBrowserPane() {
    // TODO: delete _iqtextbrowser;
    _iqtextbrowser = nullptr;
}

std::string GBrowserPane::getContentType() const {
    return _contentType;
}

std::string GBrowserPane::getPageUrl() const {
    return _pageUrl;
}

_Internal_QWidget* GBrowserPane::getInternalWidget() const {
    return _iqtextbrowser;
}

std::string GBrowserPane::getText() const {
    return _iqtextbrowser->toHtml().toStdString();
}

std::string GBrowserPane::getType() const {
    return "GBrowserPane";
}

QWidget* GBrowserPane::getWidget() const {
    return static_cast<QWidget*>(_iqtextbrowser);
}

void GBrowserPane::readTextFromFile(std::istream& file) {
    std::string fileText = readEntireStream(file);
    setText(fileText);
}

void GBrowserPane::readTextFromFile(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (!input.fail()) {
        _pageUrl = filename;
        std::string extension = getExtension(filename);
        std::string contentType = HttpServer::getContentType(extension);
        if (!contentType.empty()) {
            setContentType(contentType);
        }
        readTextFromFile(input);
    }
}

void GBrowserPane::readTextFromUrl(const std::string& url) {
    this->_pageUrl = url;
    GThread::runOnQtGuiThread([this, url]() {
        QUrl qurl(QString::fromStdString(url));
        _iqtextbrowser->setSource(qurl);
    });
}

void GBrowserPane::removeLinkListener() {
    removeEventListener("linkclick");
}

void GBrowserPane::setContentType(const std::string& contentType) {
    _contentType = contentType;
}

void GBrowserPane::setLinkListener(GEventListener func) {
    setEventListener("linkclick", func);
}

void GBrowserPane::setLinkListener(GEventListenerVoid func) {
    setEventListener("linkclick", func);
}

void GBrowserPane::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextbrowser->setText(QString::fromStdString(text));
    });
}


_Internal_QTextBrowser::_Internal_QTextBrowser(GBrowserPane* gbrowserpane, QWidget* parent)
        : QTextBrowser(parent),
          _gbrowserpane(gbrowserpane) {
    require::nonNull(gbrowserpane, "_Internal_QTextBrowser::constructor");
    setObjectName(QString::fromStdString("_Internal_QTextBrowser_" + integerToString(gbrowserpane->getID())));
}

QVariant _Internal_QTextBrowser::loadResource(int type, const QUrl &url) {
    // patch to work properly for data:... URLs
    if (type == QTextDocument::ImageResource && url.scheme() == QLatin1String("data")) {
        QRegExp regex("^image/[^;]+;base64,(.+)={0,2}$");
        if (regex.indexIn(url.path()) >= 0) {
            QImage img;
            if (img.loadFromData(QByteArray::fromBase64(regex.cap(1).toLatin1()))) {
                return QVariant::fromValue(img);
            }
        }
    }
    return QTextBrowser::loadResource(type, url);
}

void _Internal_QTextBrowser::mousePressEvent(QMouseEvent* event) {
    QTextBrowser::mousePressEvent(event);
    if (!_gbrowserpane->isAcceptingEvent("linkclick")) return;
    if (!(event->button() & Qt::LeftButton)) {
        return;
    }
    QString clickedAnchor = anchorAt(event->pos());
    if (clickedAnchor.isEmpty()) {
        return;
    }
    _clickedLink = clickedAnchor;
}

void _Internal_QTextBrowser::mouseReleaseEvent(QMouseEvent* event) {
    if (!_gbrowserpane->isAcceptingEvent("linkclick")) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }
    if (!(event->button() & Qt::LeftButton)) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }
    QString clickedAnchor = anchorAt(event->pos());
    if (clickedAnchor.isEmpty() || _clickedLink.isEmpty()
            || clickedAnchor != _clickedLink) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }

    _clickedLink = QString::fromStdString("");   // clear

    GEvent linkEvent(
                /* class  */ HYPERLINK_EVENT,
                /* type   */ HYPERLINK_CLICKED,
                /* name   */ "linkclick",
                /* source */ _gbrowserpane);
    linkEvent.setButton(static_cast<int>(event->button()));
    linkEvent.setX(event->x());
    linkEvent.setY(event->y());
    linkEvent.setModifiers(event->modifiers());
    linkEvent.setRequestURL(clickedAnchor.toStdString());
    linkEvent.setActionCommand(_gbrowserpane->getActionCommand());
    linkEvent.setInternalEvent(event);
    _gbrowserpane->fireEvent(linkEvent);
}

QSize _Internal_QTextBrowser::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextBrowser::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#include "moc_gbrowserpane.cpp"   // speeds up compilation of auto-generated Qt files
#endif // SPL_PRECOMPILE_QT_MOC_FILES
