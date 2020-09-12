/*
 * File: gtextarea.cpp
 * -------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - moved some event listener code to GInteractor superclass
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/08/23
 * - renamed to gtextarea.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "gtextarea.h"
#include <QScrollBar>
#include <QTextCursor>
#include "gcolor.h"
#include "gfont.h"
#include "gthread.h"
#include "gwindow.h"
#include "require.h"

GTextArea::GTextArea(int rows, int columns, QWidget* parent)
        : _contextMenuEnabled(true) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    });
    setRowsColumns(rows, columns);
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextArea::GTextArea(const std::string& text, QWidget* parent)
        : _contextMenuEnabled(true) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextArea::~GTextArea() {
    // TODO: delete _iqtextedit;
    _iqtextedit->detach();
    _iqtextedit = nullptr;
}

void GTextArea::appendFormattedText(const std::string& text, const std::string& color, const std::string& font) {
    moveCursorToEnd();

    // create a formatted block with the font and color
    QTextCharFormat format;
    if (!color.empty()) {
        format.setForeground(QBrush(GColor::convertColorToRGB(color)));
    }
    if (!font.empty()) {
        // carry over only the font's weight, not size/face
        QFont qfont = GFont::toQFont(_iqtextedit->font(), font);
        format.setFontWeight(qfont.weight());
    }

    QTextCursor cursor = _iqtextedit->textCursor();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(QString::fromStdString(text), format);
    cursor.endEditBlock();
    GThread::runOnQtGuiThread([this, cursor]() {
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });

    moveCursorToEnd();
}

void GTextArea::appendHtml(const std::string& html) {
    // TODO: use insertHtml for speed?
    setHtml(getHtml() + html);
}

void GTextArea::appendText(const std::string& text) {
    QTextCursor cursor = _iqtextedit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(QString::fromStdString(text));
    moveCursorToEnd();
}

void GTextArea::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.clearSelection();
        _iqtextedit->setTextCursor(cursor);
    });
}

void GTextArea::clearText() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->clear();
    });
}

int GTextArea::getColumns() const {
    return (int) (getHeight() / getRowColumnSize().width);
}

int GTextArea::getCursorPosition() const {
    return _iqtextedit->textCursor().position();
}

std::string GTextArea::getHtml() const {
    return _iqtextedit->toHtml().toStdString();
}

_Internal_QWidget* GTextArea::getInternalWidget() const {
    return _iqtextedit;
}

std::string GTextArea::getPlaceholder() const {
    return _iqtextedit->placeholderText().toStdString();
}

GDimension GTextArea::getRowColumnSize() const {
    QFontMetrics metrics(_iqtextedit->font());
    return GDimension(metrics.horizontalAdvance(QString::fromStdString("mmmmmmmmmm")) / 10.0, metrics.lineSpacing() + 2);
}

int GTextArea::getRows() const {
    return (int) (getHeight() / getRowColumnSize().height);
}

std::string GTextArea::getSelectedText() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return getText().substr(start, end - start);
    } else {
        return "";
    }
}

int GTextArea::getSelectionEnd() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return end;
    } else {
        // no selection; cursor sets selection start/end to be equal
        return -1;
    }
}

int GTextArea::getSelectionLength() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    return end - start;
}

int GTextArea::getSelectionStart() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return start;
    } else {
        return -1;
    }
}

std::string GTextArea::getText() const {
    return _iqtextedit->toPlainText().toStdString();
}

std::string GTextArea::getType() const {
    return "GTextArea";
}

QWidget* GTextArea::getWidget() const {
    return static_cast<QWidget*>(_iqtextedit);
}

bool GTextArea::isContextMenuEnabled() const {
    return _contextMenuEnabled;
}

bool GTextArea::isEditable() const {
    return !_iqtextedit->isReadOnly();
}

bool GTextArea::isLineWrap() const {
    return _iqtextedit->lineWrapMode() != QTextEdit::NoWrap;
}

void GTextArea::moveCursorToEnd() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::moveCursorToStart() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GTextArea::scrollToBottom() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextedit->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
        scrollbar->setSliderPosition(scrollbar->maximum());
    });
}

void GTextArea::scrollToTop() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextedit->verticalScrollBar();
        scrollbar->setValue(0);
        scrollbar->setSliderPosition(0);
    });
}

void GTextArea::select(int startIndex, int length) {
    require::nonNegative(startIndex, 0, "GTextArea::select", "startIndex");
    require::nonNegative(length, 0, "GTextArea::select", "length");
    GThread::runOnQtGuiThread([this, startIndex, length]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.setPosition(startIndex);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
        _iqtextedit->setTextCursor(cursor);
    });
}

void GTextArea::selectAll() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->selectAll();
    });
}

void GTextArea::setColumns(int columns) {
    require::nonNegative(columns, "GTextArea::setColumns");
    double desiredWidth = getRowColumnSize().width * columns;
    setPreferredSize(desiredWidth, getHeight());
    setSize(desiredWidth, getHeight());
}

void GTextArea::setContextMenuEnabled(bool enabled) {
    _contextMenuEnabled = enabled;
}

void GTextArea::setCursorPosition(int index, bool keepAnchor) {
    require::nonNegative(index, "TextArea::setCursorPosition", "index");
    GThread::runOnQtGuiThread([this, index, keepAnchor]() {
        QTextCursor cursor(_iqtextedit->textCursor());
        cursor.setPosition(index, keepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqtextedit->setReadOnly(!value);
    });
}

void GTextArea::setHtml(const std::string& html) {
    GThread::runOnQtGuiThread([this, html]() {
        _iqtextedit->setHtml(QString::fromStdString(html));
    });
}

void GTextArea::setLineWrap(bool wrap) {
    GThread::runOnQtGuiThread([this, wrap]() {
        _iqtextedit->setLineWrapMode(wrap ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    });
}

void GTextArea::setMouseListener(GEventListener func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GTextArea::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GTextArea::setPlaceholder(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextedit->setPlaceholderText(QString::fromStdString(text));
    });
}

void GTextArea::setRows(int rows) {
    require::nonNegative(rows, "GTextArea::setRows");
    double desiredHeight = getRowColumnSize().height * rows;
    setPreferredSize(getWidth(), desiredHeight);
    setSize(getWidth(), desiredHeight);
}

void GTextArea::setRowsColumns(int rows, int columns) {
    require::nonNegative2D(rows, columns, "GTextArea::setRowsColumns", "rows", "columns");
    double desiredWidth = getRowColumnSize().width * columns;
    double desiredHeight = getRowColumnSize().height * rows;
    setPreferredSize(desiredWidth, desiredHeight);
    setSize(desiredWidth, desiredHeight);
}

void GTextArea::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextedit->setText(QString::fromStdString(text));
    });
}

void GTextArea::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GTextArea::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}


_Internal_QTextEdit::_Internal_QTextEdit(GTextArea* gtextArea, QWidget* parent)
        : QTextEdit(parent),
          _gtextarea(gtextArea) {
    require::nonNull(gtextArea, "_Internal_QTextEdit::constructor");
    setObjectName(QString::fromStdString("_Internal_QTextEdit_" + std::to_string(gtextArea->getID())));
    ensureCursorVisible();
    setFocusPolicy(Qt::StrongFocus);
    setTabChangesFocus(false);
    document()->setUndoRedoEnabled(false);
    connect(this, SIGNAL(textChanged()), this, SLOT(handleTextChange()));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(handleScroll(int)));
}

void _Internal_QTextEdit::contextMenuEvent(QContextMenuEvent* event) {
    if (!_gtextarea) {
        QTextEdit::contextMenuEvent(event);   // call super
        return;
    }
    if (_gtextarea->isContextMenuEnabled()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void _Internal_QTextEdit::detach() {
    _gtextarea = nullptr;
}

void _Internal_QTextEdit::handleScroll(int value) {
    if (_gtextarea && _gtextarea->isAcceptingEvent("scroll")) {
        GEvent scrollEvent(
                    /* class  */ SCROLL_EVENT,
                    /* type   */ SCROLL_SCROLLED,
                    /* name   */ "scroll",
                    /* source */ _gtextarea);
        scrollEvent.setActionCommand(_gtextarea->getActionCommand());
        scrollEvent.setY(value);   // approximate
        _gtextarea->fireEvent(scrollEvent);
    }
}

void _Internal_QTextEdit::handleTextChange() {
    if (_gtextarea && _gtextarea->isAcceptingEvent("textchange")) {
        GEvent textChangeEvent(
                    /* class  */ KEY_EVENT,
                    /* type   */ KEY_TYPED,
                    /* name   */ "textchange",
                    /* source */ _gtextarea);
        textChangeEvent.setActionCommand(_gtextarea->getActionCommand());
        _gtextarea->fireEvent(textChangeEvent);

        // BUGFIX: for backward compatibility, also fire a CHANGE_EVENT
        // (emits only to the old-style waitForEvent function)
        GEvent changeEvent(
                    /* class  */ CHANGE_EVENT,
                    /* type   */ STATE_CHANGED,
                    /* name   */ "statechange",
                    /* source */ _gtextarea);
        changeEvent.setActionCommand(_gtextarea->getActionCommand());
        _gtextarea->fireEvent(changeEvent);
    }
}

void _Internal_QTextEdit::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::keyPressEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("keypress")) {
        event->accept();
        _gtextarea->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QTextEdit::keyPressEvent(event);   // call super
        }
    } else {
        QTextEdit::keyPressEvent(event);   // call super
    }
}

void _Internal_QTextEdit::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::keyReleaseEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gtextarea->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QTextEdit::keyReleaseEvent(event);   // call super
        }
    } else {
        QTextEdit::keyReleaseEvent(event);   // call super
    }
}

void _Internal_QTextEdit::mousePressEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::mousePressEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("mousepress")) {
        event->accept();
        _gtextarea->fireGEvent(event, MOUSE_PRESSED, "mousepress");
        if (event->isAccepted()) {
            QTextEdit::mousePressEvent(event);   // call super
        }
    } else {
        QTextEdit::mousePressEvent(event);   // call super
    }
}

void _Internal_QTextEdit::mouseReleaseEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::mouseReleaseEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("mouserelease")) {
        event->accept();
        _gtextarea->fireGEvent(event, MOUSE_RELEASED, "mouserelease");
        if (event->isAccepted()) {
            QTextEdit::mouseReleaseEvent(event);   // call super
        }
    } else {
        QTextEdit::mouseReleaseEvent(event);   // call super
    }
}

QSize _Internal_QTextEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextEdit::sizeHint();
    }
}


