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

#ifdef SPL_QT_GUI
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
                /* name   */ "textchange",
                /* source */ _qgtextfield);
    textChangeEvent.setActionCommand(_qgtextfield->getActionCommand());
    _qgtextfield->fireEvent(textChangeEvent);
}

void _Internal_QLineEdit::keyPressEvent(QKeyEvent* event) {
    QLineEdit::keyPressEvent(event);   // call super
    if (!_qgtextfield->hasEventListener("action")) return;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        QGEvent actionEvent(
                    /* class  */ QGEvent::ACTION_EVENT,
                    /* type   */ QGEvent::ACTION_PERFORMED,
                    /* name   */ "action",
                    /* source */ _qgtextfield);
        actionEvent.setActionCommand(_qgtextfield->getActionCommand());
        _qgtextfield->fireEvent(actionEvent);
    }
}

QSize _Internal_QLineEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QLineEdit::sizeHint();
    }
}

_Internal_QSpinBox::_Internal_QSpinBox(QGTextField* qgtextField, int min, int max, int step, QWidget* parent)
        : QSpinBox(parent),
          _qgtextfield(qgtextField) {
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QSpinBox::handleTextChange(const QString&) {
    QGEvent textChangeEvent(
                /* class  */ QGEvent::KEY_EVENT,
                /* type   */ QGEvent::KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _qgtextfield);
    textChangeEvent.setActionCommand(_qgtextfield->getActionCommand());
    _qgtextfield->fireEvent(textChangeEvent);
}

QLineEdit* _Internal_QSpinBox::lineEdit() const {
    return QSpinBox::lineEdit();
}

QSize _Internal_QSpinBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSpinBox::sizeHint();
    }
}

_Internal_QDoubleSpinBox::_Internal_QDoubleSpinBox(QGTextField* qgtextField, double min, double max, double step, QWidget* parent)
        : QDoubleSpinBox(parent),
          _qgtextfield(qgtextField) {
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QDoubleSpinBox::handleTextChange(const QString&) {
    QGEvent textChangeEvent(
                /* class  */ QGEvent::KEY_EVENT,
                /* type   */ QGEvent::KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _qgtextfield);
    textChangeEvent.setActionCommand(_qgtextfield->getActionCommand());
    _qgtextfield->fireEvent(textChangeEvent);
}

QLineEdit* _Internal_QDoubleSpinBox::lineEdit() const {
    return QDoubleSpinBox::lineEdit();
}

QSize _Internal_QDoubleSpinBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QDoubleSpinBox::sizeHint();
    }
}


QGTextField::QGTextField(const std::string& text, int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
    _inputType = QGTextField::INPUT_TYPE_TEXT;
    if (!text.empty()) {
        setText(text);
    }
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
}

QGTextField::QGTextField(int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
    _inputType = QGTextField::INPUT_TYPE_TEXT;
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
}

QGTextField::QGTextField(int value, int min, int max, int step, QWidget* parent) {
    _iqspinbox = new _Internal_QSpinBox(this, min, max, step, getInternalParent(parent));
    _iqspinbox->setValue(value);
    _inputType = QGTextField::INPUT_TYPE_INTEGER;
}

QGTextField::QGTextField(double value, double min, double max, double step, QWidget* parent) {
    _iqdoublespinbox = new _Internal_QDoubleSpinBox(this, min, max, step, getInternalParent(parent));
    _iqdoublespinbox->setValue(value);
    _inputType = QGTextField::INPUT_TYPE_REAL;
}

QGTextField::~QGTextField() {
    // TODO: delete _iqlineedit;
    _iqlineedit = nullptr;
    _iqspinbox = nullptr;
    _iqdoublespinbox = nullptr;
}

int QGTextField::getCharsWide() const {
    QFontMetrics fm(getWidget()->font());
    int mWidth = fm.width(QString::fromStdString("m"));
    return (int) (getWidth() / mWidth);
}

QGTextField::InputType QGTextField::getInputType() const {
    return _inputType;
}

_Internal_QWidget* QGTextField::getInternalWidget() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit;
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox;
    } else {
        return _iqdoublespinbox;
    }
}

int QGTextField::getMaxLength() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->maxLength();
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        std::string maxStr = integerToString(_iqspinbox->maximum());
        return (int) maxStr.length();
    } else {
        std::string maxStr = integerToString(_iqdoublespinbox->maximum());
        return (int) maxStr.length();   // TODO: may be incorrect w/ decimal value
    }
}

std::string QGTextField::getPlaceholder() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->placeholderText().toStdString();
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->placeholderText().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->placeholderText().toStdString();
    }
}

std::string QGTextField::getText() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->text().toStdString();
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->text().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->text().toStdString();
    }
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
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return static_cast<QWidget*>(_iqlineedit);
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return static_cast<QWidget*>(_iqspinbox);
    } else {
        return static_cast<QWidget*>(_iqdoublespinbox);
    }
}

bool QGTextField::isAutocompleteEnabled() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->completer() != nullptr;
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->completer() != nullptr;
    } else {
        return _iqdoublespinbox->lineEdit()->completer() != nullptr;
    }
}

bool QGTextField::isEditable() const {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        return !_iqlineedit->isReadOnly();
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        return !_iqspinbox->lineEdit()->isReadOnly();
    } else {
        return !_iqdoublespinbox->lineEdit()->isReadOnly();
    }
}

void QGTextField::removeActionListener() {
    removeEventListener("action");
}

void QGTextField::removeTextChangeListener() {
    removeEventListener("textchange");
}

void QGTextField::setActionListener(QGEventListener func) {
    setEventListener("action", func);
}

void QGTextField::setActionListener(QGEventListenerVoid func) {
    setEventListener("action", func);
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

    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        _iqlineedit->setCompleter(completer);
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        _iqspinbox->lineEdit()->setCompleter(completer);
    } else {
        _iqdoublespinbox->lineEdit()->setCompleter(completer);
    }
}

void QGTextField::setAutocompleteEnabled(bool enabled) {
    if (!enabled) {
        if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setCompleter(nullptr);
        } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setCompleter(nullptr);
        } else {
            _iqdoublespinbox->lineEdit()->setCompleter(nullptr);
        }
    }
    // TODO
}

void QGTextField::setCharsWide(int charsWide) {
    QFontMetrics fm(getWidget()->font());
    int mWidth = fm.width(QString::fromStdString("m"));
    getWidget()->setFixedWidth(mWidth * charsWide);
    getWidget()->updateGeometry();
}

void QGTextField::setEditable(bool value) {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        _iqlineedit->setReadOnly(!value);
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        _iqspinbox->setReadOnly(!value);
    } else {
        _iqdoublespinbox->setReadOnly(!value);
    }
}

void QGTextField::setMaxLength(int maxLength) {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        _iqlineedit->setMaxLength(maxLength);
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        _iqspinbox->lineEdit()->setMaxLength(maxLength);
    } else {
        _iqdoublespinbox->lineEdit()->setMaxLength(maxLength);
    }
}

void QGTextField::setPlaceholder(const std::string& text) {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        _iqlineedit->setPlaceholderText(QString::fromStdString(text));
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        _iqspinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
    } else {
        _iqdoublespinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
    }
}

void QGTextField::setText(const std::string& text) {
    if (_inputType == QGTextField::INPUT_TYPE_TEXT) {
        _iqlineedit->setText(QString::fromStdString(text));
    } else if (_inputType == QGTextField::INPUT_TYPE_INTEGER) {
        _iqspinbox->lineEdit()->setText(QString::fromStdString(text));
    } else {
        _iqdoublespinbox->lineEdit()->setText(QString::fromStdString(text));
    }
}

void QGTextField::setTextChangeListener(QGEventListener func) {
    setEventListener("textchange", func);
}

void QGTextField::setTextChangeListener(QGEventListenerVoid func) {
    setEventListener("textchange", func);
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

#endif // SPL_QT_GUI
