/*
 * File: qgtextarea.cpp
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgtextarea.h"
#include "qgwindow.h"
#include "strlib.h"

_Internal_QTextEdit::_Internal_QTextEdit(QGTextArea* qgtextArea, QWidget* parent)
        : QTextEdit(parent),
          _qgtextarea(qgtextArea) {
    connect(this, SIGNAL(textChanged()), this, SLOT(handleTextChange()));
}

void _Internal_QTextEdit::handleTextChange() {
    QGEvent textChangeEvent(
                /* class  */ QGEvent::KEY_EVENT,
                /* type   */ QGEvent::KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _qgtextarea);
    textChangeEvent.setActionCommand(_qgtextarea->getActionCommand());
    _qgtextarea->fireEvent(textChangeEvent);
}

QSize _Internal_QTextEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextEdit::sizeHint();
    }
}


QGTextArea::QGTextArea(int rows, int columns, QWidget* parent) {
    _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    setRowsColumns(rows, columns);
}

QGTextArea::QGTextArea(const std::string& text, QWidget* parent) {
    _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    setText(text);
}

QGTextArea::~QGTextArea() {
    // TODO: delete _iqtextedit;
    _iqtextedit = nullptr;
}

int QGTextArea::getColumns() const {
    return (int) (getHeight() / getRowColumnSize().getWidth());
}

_Internal_QWidget* QGTextArea::getInternalWidget() const {
    return _iqtextedit;
}

std::string QGTextArea::getPlaceholder() const {
    return _iqtextedit->placeholderText().toStdString();
}

GDimension QGTextArea::getRowColumnSize() const {
    QFontMetrics m(_iqtextedit->font());
    return GDimension(m.width(QString::fromStdString("m")), m.lineSpacing());
}

int QGTextArea::getRows() const {
    return (int) (getHeight() / getRowColumnSize().getHeight());
}

std::string QGTextArea::getText() const {
    return _iqtextedit->toPlainText().toStdString();
}

std::string QGTextArea::getType() const {
    return "QGTextArea";
}

QWidget* QGTextArea::getWidget() const {
    return static_cast<QWidget*>(_iqtextedit);
}

bool QGTextArea::isEditable() const {
    return !_iqtextedit->isReadOnly();
}

void QGTextArea::removeTextChangeHandler() {
    removeEventHandler("textchange");
}

void QGTextArea::setColumns(int columns) {
    double desiredWidth = getRowColumnSize().getWidth() * columns;
    setSize(desiredWidth, getHeight());
}

void QGTextArea::setEditable(bool value) {
    _iqtextedit->setReadOnly(!value);
}

void QGTextArea::setPlaceholder(const std::string& text) {
    _iqtextedit->setPlaceholderText(QString::fromStdString(text));
}

void QGTextArea::setRows(int rows) {
    double desiredHeight = getRowColumnSize().getHeight() * rows;
    setSize(getWidth(), desiredHeight);
}

void QGTextArea::setRowsColumns(int rows, int columns) {
    double desiredWidth = getRowColumnSize().getWidth() * columns;
    double desiredHeight = getRowColumnSize().getHeight() * rows;
    setSize(desiredWidth, desiredHeight);
}

void QGTextArea::setText(const std::string& text) {
    _iqtextedit->setText(QString::fromStdString(text));
}

void QGTextArea::setTextChangeHandler(QGEventHandler func) {
    setEventHandler("textchange", func);
}

void QGTextArea::setTextChangeHandler(QGEventHandlerVoid func) {
    setEventHandler("textchange", func);
}
