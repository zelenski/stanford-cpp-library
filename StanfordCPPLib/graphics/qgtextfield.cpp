/*
 * File: qgtextfield.cpp
 * ---------------------
 *
 * @version 2018/06/29
 * - added textChange event
 * - added autocompletion
 * @version 2018/06/25
 * - initial version
 */

#include "qgtextfield.h"
#include <QCompleter>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include "qgwindow.h"
#include "strlib.h"

_Q_Internal_TextField::_Q_Internal_TextField(QGTextField* textField, QWidget* parent)
        : QLineEdit(parent),
          _qgtextfield(textField) {
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(handleTextChange(const QString&)));
}

void _Q_Internal_TextField::handleTextChange(const QString&) {
    if (_qgtextfield->_textChangeHandler) {
        _qgtextfield->_textChangeHandler();
    }
}

QGTextField::QGTextField(const std::string& text, QWidget* parent)
        : _qtextfield(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _textChangeHandler(nullptr) {
    setText(text);
}

std::string QGTextField::getPlaceholder() const {
    return _qtextfield.placeholderText().toStdString();
}

std::string QGTextField::getText() const {
    return _qtextfield.text().toStdString();
}

std::string QGTextField::getType() const {
    return "QGTextField";
}

double QGTextField::getValueAsDouble() const {
    std::string text = trim(getText());
    return stringToDouble(text);
}

int QGTextField::getValueAsInt() const {
    return getValueAsInteger();
}

int QGTextField::getValueAsInteger() const {
    std::string text = trim(getText());
    return stringToInteger(text);
}

QWidget* QGTextField::getWidget() const {
    return (QWidget*) &_qtextfield;
}

bool QGTextField::isAutocompleteEnabled() const {
    return _qtextfield.completer() != nullptr;
}

bool QGTextField::isEditable() const {
    return !_qtextfield.isReadOnly();
}

void QGTextField::setAutocompleteList(std::initializer_list<std::string> strings) {
    Vector<std::string> v(strings);
    setAutocompleteList(v);
}

void QGTextField::setAutocompleteList(const Vector<std::string>& strings) {
    QStringList stringList;
    for (std::string s : strings) {
        stringList.push_back(QString::fromStdString(s));
    }
    QStringListModel* model = new QStringListModel(stringList, &_qtextfield);   // MEMORY LEAK
    QCompleter* completer = new QCompleter(model, &_qtextfield);                // MEMORY LEAK
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    _qtextfield.setCompleter(completer);
}

void QGTextField::setAutocompleteEnabled(bool enabled) {
    if (!enabled) {
        _qtextfield.setCompleter(nullptr);
    }
    // TODO
}

void QGTextField::setEditable(bool value) {
    _qtextfield.setReadOnly(!value);
}

void QGTextField::setPlaceholder(const std::string& text) {
    _qtextfield.setPlaceholderText(QString::fromStdString(text));
}

void QGTextField::setText(const std::string& text) {
    _qtextfield.setText(QString::fromStdString(text));
}

void QGTextField::setTextChangeHandler(void (* func)()) {
    _textChangeHandler = func;
}

bool QGTextField::valueIsDouble() const {
    return stringIsDouble(trim(getText()));
}

bool QGTextField::valueIsInt() const {
    return stringIsInteger(trim(getText()));
}

bool QGTextField::valueIsInteger() const {
    return stringIsInteger(trim(getText()));
}

bool QGTextField::valueIsReal() const {
    return stringIsReal(trim(getText()));
}
