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

_Internal_QLineEdit::_Internal_QLineEdit(QGTextField* qgtextField, QWidget* parent)
        : QLineEdit(parent),
          _qgtextfield(qgtextField) {
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(handleTextChange(const QString&)));
}

void _Internal_QLineEdit::handleTextChange(const QString&) {
    QGEvent textChangeEvent(
                /* class  */ QGEvent::KEY_EVENT,
                /* type   */ QGEvent::KEY_TYPED,
                /* name   */ "textchange");
    _qgtextfield->fireEvent(textChangeEvent);
}

QGTextField::QGTextField(const std::string& text, QWidget* parent) {
    _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
    setText(text);
}

QGTextField::QGTextField(const std::string& text, int charsWide, QWidget* parent) {
    _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
    setText(text);
    setCharsWide(charsWide);
}

QGTextField::QGTextField(int charsWide, QWidget* parent) {
    _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
    setCharsWide(charsWide);
}

QGTextField::~QGTextField() {
    // TODO: delete _iqlineedit;
    _iqlineedit = nullptr;
}

int QGTextField::getCharsWide() const {
    QFontMetrics fm(_iqlineedit->font());
    int mWidth = fm.width(QString::fromStdString("m"));
    return (int) (getWidth() / mWidth);
}

int QGTextField::getMaxLength() const {
    return _iqlineedit->maxLength();
}

std::string QGTextField::getPlaceholder() const {
    return _iqlineedit->placeholderText().toStdString();
}

std::string QGTextField::getText() const {
    return _iqlineedit->text().toStdString();
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
    return static_cast<QWidget*>(_iqlineedit);
}

bool QGTextField::isAutocompleteEnabled() const {
    return _iqlineedit->completer() != nullptr;
}

bool QGTextField::isEditable() const {
    return !_iqlineedit->isReadOnly();
}

void QGTextField::removeTextChangeHandler() {
    removeEventHandler("textchange");
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
    QStringListModel* model = new QStringListModel(stringList, _iqlineedit);   // TODO: MEMORY LEAK
    QCompleter* completer = new QCompleter(model, _iqlineedit);                // TODO: MEMORY LEAK
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    _iqlineedit->setCompleter(completer);
}

void QGTextField::setAutocompleteEnabled(bool enabled) {
    if (!enabled) {
        _iqlineedit->setCompleter(nullptr);
    }
    // TODO
}

void QGTextField::setCharsWide(int charsWide) {
    QFontMetrics fm(_iqlineedit->font());
    int mWidth = fm.width(QString::fromStdString("m"));
    _iqlineedit->setFixedWidth(mWidth * charsWide);
    _iqlineedit->updateGeometry();
}

void QGTextField::setEditable(bool value) {
    _iqlineedit->setReadOnly(!value);
}

void QGTextField::setMaxLength(int maxLength) {
    _iqlineedit->setMaxLength(maxLength);
}

void QGTextField::setPlaceholder(const std::string& text) {
    _iqlineedit->setPlaceholderText(QString::fromStdString(text));
}

void QGTextField::setText(const std::string& text) {
    _iqlineedit->setText(QString::fromStdString(text));
}

void QGTextField::setTextChangeHandler(QGEventHandler func) {
    setEventHandler("textchange", func);
}

void QGTextField::setTextChangeHandler(QGEventHandlerVoid func) {
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
