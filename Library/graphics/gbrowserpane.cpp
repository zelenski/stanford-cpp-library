/*
 * File: gbrowserpane.cpp
 * ----------------------
 * This file contains the implementation of the <code>GBrowserPane</code> class
 * as declared in gbrowserpane.h.
 *
 * @version 2019/04/23
 * - moved some event-handling code to GInteractor superclass
 * @version 2018/12/28
 * - added methods for text selection, scrolling, cursor position, key/mouse listeners
 * @version 2018/09/17
 * - fixed thread safety bugs
 * - added link listener events
 * @version 2018/08/23
 * - renamed to gbrowserpane.h to replace Java version
 * @version 2018/07/15
 * - initial version
 */

#include "gbrowserpane.h"
#include <QScrollBar>
#include <QTextCursor>
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "gthread.h"
#include "require.h"

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
    _iqtextbrowser->detach();
    _iqtextbrowser = nullptr;
}

void GBrowserPane::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.clearSelection();
        _iqtextbrowser->setTextCursor(cursor);
    });
}

void GBrowserPane::clearText() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->clear();
    });
}

int GBrowserPane::getCursorPosition() const {
    return _iqtextbrowser->textCursor().position();
}

_Internal_QWidget* GBrowserPane::getInternalWidget() const {
    return _iqtextbrowser;
}

std::string GBrowserPane::getPageUrl() const {
    return _pageUrl;
}

std::string GBrowserPane::getSelectedText() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return getText().substr(start, end - start);
    } else {
        return "";
    }
}

int GBrowserPane::getSelectionEnd() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return end;
    } else {
        // no selection; cursor sets selection start/end to be equal
        return -1;
    }
}

int GBrowserPane::getSelectionLength() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    return end - start;
}

int GBrowserPane::getSelectionStart() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return start;
    } else {
        return -1;
    }
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

bool GBrowserPane::isEditable() const {
    return !_iqtextbrowser->isReadOnly();
}

bool GBrowserPane::isLineWrap() const {
    return _iqtextbrowser->lineWrapMode() != QTextEdit::NoWrap;
}

void GBrowserPane::moveCursorToEnd() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
}

void GBrowserPane::moveCursorToStart() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
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

void GBrowserPane::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GBrowserPane::scrollToBottom() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextbrowser->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
        scrollbar->setSliderPosition(scrollbar->maximum());
    });
}

void GBrowserPane::scrollToTop() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextbrowser->verticalScrollBar();
        scrollbar->setValue(0);
        scrollbar->setSliderPosition(0);
    });
}

void GBrowserPane::select(int startIndex, int length) {
    require::nonNegative(startIndex, "GBrowserPane::select", "startIndex");
    require::nonNegative(length, "GBrowserPane::select", "length");
    GThread::runOnQtGuiThread([this, startIndex, length]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.setPosition(startIndex);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
        _iqtextbrowser->setTextCursor(cursor);
    });
}

void GBrowserPane::selectAll() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->selectAll();
    });
}

void GBrowserPane::setCursorPosition(int index, bool keepAnchor) {
    require::nonNegative(index, "TextArea::setCursorPosition", "index");
    GThread::runOnQtGuiThread([this, index, keepAnchor]() {
        QTextCursor cursor(_iqtextbrowser->textCursor());
        cursor.setPosition(index, keepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
}

void GBrowserPane::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqtextbrowser->setReadOnly(!value);
    });
}

void GBrowserPane::setMouseListener(GEventListener func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GBrowserPane::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GBrowserPane::setLineWrap(bool wrap) {
    GThread::runOnQtGuiThread([this, wrap]() {
        _iqtextbrowser->setLineWrapMode(wrap ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    });
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

// JDZ: when replace text, browser pane scrolls to seemingly arbitrary location
// Annoying! this version perserve scroll position (if true) or scrolls to end (if false)
void GBrowserPane::setTextPreserveScroll(const std::string& text, bool preserve) {
    GThread::runOnQtGuiThread([this, text, preserve]() {
        QScrollBar* scrollbar = _iqtextbrowser->verticalScrollBar();
        int oldPos = scrollbar->value();
        _iqtextbrowser->setText(QString::fromStdString(text));
        int newPos = preserve ? oldPos : scrollbar->maximum();
        scrollbar->setValue(newPos);
        scrollbar->setSliderPosition(newPos);
    });
}

void GBrowserPane::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GBrowserPane::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}

_Internal_QTextBrowser::_Internal_QTextBrowser(GBrowserPane* gbrowserpane, QWidget* parent)
        : QTextBrowser(parent),
          _gbrowserpane(gbrowserpane) {
    require::nonNull(gbrowserpane, "_Internal_QTextBrowser::constructor");
    setObjectName(QString::fromStdString("_Internal_QTextBrowser_" + std::to_string(gbrowserpane->getID())));
    setFocusPolicy(Qt::StrongFocus);
}

void _Internal_QTextBrowser::detach() {
    _gbrowserpane = nullptr;
}

void _Internal_QTextBrowser::mousePressEvent(QMouseEvent* event) {
    QTextBrowser::mousePressEvent(event);
    if (!_gbrowserpane->isAcceptingEvent("linkclick")) {
        return;
    }
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
    linkEvent.setX(event->pos().x());
    linkEvent.setY(event->pos().y());
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


