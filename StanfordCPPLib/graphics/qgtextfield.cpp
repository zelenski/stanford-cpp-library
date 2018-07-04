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
#include <QFontMetrics>
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
    _qgtextfield->fireEvent("textchange");
}

QGTextField::QGTextField(const std::string& text, QWidget* parent)
        : _qtextfield(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    ensureThreadSafety();
    setText(text);
}

QGTextField::QGTextField(const std::string& text, int charsWide, QWidget* parent)
        : _qtextfield(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    ensureThreadSafety();
    setText(text);
    setCharsWide(charsWide);
}

QGTextField::QGTextField(int charsWide, QWidget* parent)
        : _qtextfield(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()) {
    ensureThreadSafety();
    setCharsWide(charsWide);
}

int QGTextField::getCharsWide() const {
    QFontMetrics fm(_qtextfield.font());
    int mWidth = fm.width(QString::fromStdString("m"));
    return (int) (getWidth() / mWidth);
}

int QGTextField::getMaxLength() const {
    return _qtextfield.maxLength();
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

std::string QGTextField::getValue() const {
    return getText();
}

bool QGTextField::getValueAsBool() const {
    std::string text = trim(getText());
    return stringToBool(text);
}

char QGTextField::getValueAsChar() const {
    std::string text = getText();
    if (text.empty()) {
        return '\0';
    } else {
        return text[0];
    }
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

void QGTextField::setCharsWide(int charsWide) {
    QFontMetrics fm(_qtextfield.font());
    int mWidth = fm.width(QString::fromStdString("m"));
    _qtextfield.setFixedWidth(mWidth * charsWide);
    _qtextfield.updateGeometry();
}

void QGTextField::setEditable(bool value) {
    _qtextfield.setReadOnly(!value);
}

void QGTextField::setMaxLength(int maxLength) {
    _qtextfield.setMaxLength(maxLength);
}

void QGTextField::setPlaceholder(const std::string& text) {
    _qtextfield.setPlaceholderText(QString::fromStdString(text));
}

void QGTextField::setText(const std::string& text) {
    _qtextfield.setText(QString::fromStdString(text));
}

void QGTextField::setTextChangeHandler(std::function<void()> func) {
    setEventHandler("textchange", func);
}

void QGTextField::setValue(bool value) {
    setText(boolToString(value));
}

void QGTextField::setValue(char value) {
    setText(charToString(value));
}

void QGTextField::setValue(double value) {
    setText(doubleToString(value));
}

void QGTextField::setValue(int value) {
    setText(integerToString(value));
}

void QGTextField::setValue(const std::string& value) {
    setText(value);
}

bool QGTextField::valueIsBool() const {
    return stringIsBool(trim(getText()));
}

bool QGTextField::valueIsChar() const {
    return (int) getText().length() == 1;
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
