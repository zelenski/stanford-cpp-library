/*
 * File: gtextfield.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key events
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2019/02/01
 * - emit CHANGE_EVENT on key presses when text changes
 * @version 2018/08/23
 * - renamed to gtextfield.cpp to replace Java version
 * @version 2018/06/29
 * - added textChange event
 * - added autocompletion
 * @version 2018/06/25
 * - initial version
 */

#include "gtextfield.h"
#include <QCompleter>
#include <QFontMetrics>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include "gthread.h"
#include "gwindow.h"
#include "require.h"

GTextField::GTextField(const std::string& text, int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
        _inputType = GTextField::INPUT_TYPE_TEXT;
    });
    if (!text.empty()) {
        setText(text);
    }
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
        _inputType = GTextField::INPUT_TYPE_TEXT;
    });
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(int value, int min, int max, int step, QWidget* parent) {
    require::require(min <= max, "GTextField::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GTextField::constructor", "value");
    GThread::runOnQtGuiThread([this, value, min, max, step, parent]() {
        _iqspinbox = new _Internal_QSpinBox(this, min, max, step, getInternalParent(parent));
        _iqspinbox->setValue(value);
        _inputType = GTextField::INPUT_TYPE_INTEGER;
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(double value, double min, double max, double step, QWidget* parent) {
    require::require(min <= max, "GTextField::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GTextField::constructor", "value");
    GThread::runOnQtGuiThread([this, value, min, max, step, parent]() {
        _iqdoublespinbox = new _Internal_QDoubleSpinBox(this, min, max, step, getInternalParent(parent));
        _iqdoublespinbox->setValue(value);
        _inputType = GTextField::INPUT_TYPE_REAL;
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::~GTextField() {
    // TODO: delete _iqlineedit;
    if (_iqlineedit) {
        _iqlineedit->detach();
        _iqlineedit = nullptr;
    }
    if (_iqspinbox) {
        _iqspinbox->detach();
        _iqspinbox = nullptr;
    }
    if (_iqdoublespinbox) {
        _iqdoublespinbox->detach();
        _iqdoublespinbox = nullptr;
    }
}

std::string GTextField::getActionEventType() const {
    return "change";
}

int GTextField::getCharsWide() const {
    QFontMetrics fm(getWidget()->font());
    int mWidth = fm.horizontalAdvance(QString::fromStdString("m"));
    return (int) (getWidth() / mWidth);
}

GTextField::InputType GTextField::getInputType() const {
    return _inputType;
}

_Internal_QWidget* GTextField::getInternalWidget() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit;
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox;
    } else {
        return _iqdoublespinbox;
    }
}

int GTextField::getMaxLength() const {
    int maxLength = _iqlineedit->maxLength();

    // Qt has default max text length of 32767
    maxLength = maxLength == 32767 ? 0 : maxLength;

    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        // empty
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        std::string maxStr = std::to_string(_iqspinbox->maximum());
        maxLength = std::max(maxLength, (int) maxStr.length());
    } else {
        std::string maxStr = std::to_string(_iqdoublespinbox->maximum());
        maxLength = std::max(maxLength, (int) maxStr.length());   // TODO: may be incorrect w/ decimal value
    }

    return maxLength;
}

std::string GTextField::getPlaceholder() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->placeholderText().toStdString();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->placeholderText().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->placeholderText().toStdString();
    }
}

std::string GTextField::getText() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->text().toStdString();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->text().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->text().toStdString();
    }
}

std::string GTextField::getType() const {
    return "GTextField";
}

std::string GTextField::getValue() const {
    return getText();
}

bool GTextField::getValueAsBool() const {
    std::string text = trim(getText());
    return stringToBool(text);
}

char GTextField::getValueAsChar() const {
    std::string text = getText();
    if (text.empty()) {
        return '\0';
    } else {
        return text[0];
    }
}

double GTextField::getValueAsDouble() const {
    std::string text = trim(getText());
    return stringToDouble(text);
}

int GTextField::getValueAsInt() const {
    return getValueAsInteger();
}

int GTextField::getValueAsInteger() const {
    std::string text = trim(getText());
    return stringToInteger(text);
}

QWidget* GTextField::getWidget() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return static_cast<QWidget*>(_iqlineedit);
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return static_cast<QWidget*>(_iqspinbox);
    } else {
        return static_cast<QWidget*>(_iqdoublespinbox);
    }
}

bool GTextField::isAutocompleteEnabled() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->completer() != nullptr;
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->completer() != nullptr;
    } else {
        return _iqdoublespinbox->lineEdit()->completer() != nullptr;
    }
}

bool GTextField::isEditable() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return !_iqlineedit->isReadOnly();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return !_iqspinbox->lineEdit()->isReadOnly();
    } else {
        return !_iqdoublespinbox->lineEdit()->isReadOnly();
    }
}

void GTextField::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GTextField::setAutocompleteList(std::initializer_list<std::string> strings) {
    Vector<std::string> v(strings);
    setAutocompleteList(v);
}

void GTextField::setAutocompleteList(const Vector<std::string>& strings) {
    GThread::runOnQtGuiThread([this, &strings]() {
        QStringList stringList;
        for (std::string s : strings) {
            if (!s.empty()) {
                stringList.push_back(QString::fromStdString(s));
            }
        }
        QStringListModel* model = new QStringListModel(stringList, _iqlineedit);   // TODO: MEMORY LEAK
        QCompleter* completer = new QCompleter(model, _iqlineedit);                // TODO: MEMORY LEAK
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);

        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setCompleter(completer);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setCompleter(completer);
        } else {
            _iqdoublespinbox->lineEdit()->setCompleter(completer);
        }
    });
}

void GTextField::setAutocompleteEnabled(bool enabled) {
    GThread::runOnQtGuiThread([this, enabled]() {
        if (!enabled) {
            if (_inputType == GTextField::INPUT_TYPE_TEXT) {
                _iqlineedit->setCompleter(nullptr);
            } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
                _iqspinbox->lineEdit()->setCompleter(nullptr);
            } else {
                _iqdoublespinbox->lineEdit()->setCompleter(nullptr);
            }
        }
    });
    // TODO: ability to set back to false
}

void GTextField::setCharsWide(int charsWide) {
    require::nonNegative(charsWide, "GTextField::setCharsWide");
    GThread::runOnQtGuiThread([this, charsWide]() {
        QFontMetrics fm(getWidget()->font());
        int mWidth = fm.horizontalAdvance(QString::fromStdString("m"));
        getWidget()->setFixedWidth(mWidth * charsWide);
        getWidget()->updateGeometry();
    });
}

void GTextField::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setReadOnly(!value);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->setReadOnly(!value);
        } else {
            _iqdoublespinbox->setReadOnly(!value);
        }
    });
}

void GTextField::setMaxLength(int maxLength) {
    GThread::runOnQtGuiThread([this, maxLength]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setMaxLength(maxLength);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setMaxLength(maxLength);
        } else {
            _iqdoublespinbox->lineEdit()->setMaxLength(maxLength);
        }
    });
}

void GTextField::setPlaceholder(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setPlaceholderText(QString::fromStdString(text));
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
        } else {
            _iqdoublespinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
        }
    });
}

void GTextField::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setText(QString::fromStdString(text));
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setText(QString::fromStdString(text));
        } else {
            _iqdoublespinbox->lineEdit()->setText(QString::fromStdString(text));
        }
    });
}

void GTextField::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GTextField::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}

void GTextField::setValue(bool value) {
    setText(boolToString(value));
}

void GTextField::setValue(char value) {
    setText(charToString(value));
}

void GTextField::setValue(double value) {
    setText(realToString(value));
}

void GTextField::setValue(int value) {
    setText(std::to_string(value));
}

void GTextField::setValue(const std::string& value) {
    setText(value);
}

bool GTextField::valueIsBool() const {
    return stringIsBool(trim(getText()));
}

bool GTextField::valueIsChar() const {
    return (int) getText().length() == 1;
}

bool GTextField::valueIsDouble() const {
    return stringIsDouble(trim(getText()));
}

bool GTextField::valueIsInt() const {
    return stringIsInteger(trim(getText()));
}

bool GTextField::valueIsInteger() const {
    return stringIsInteger(trim(getText()));
}

bool GTextField::valueIsReal() const {
    return stringIsReal(trim(getText()));
}


_Internal_QLineEdit::_Internal_QLineEdit(GTextField* gtextField, QWidget* parent)
        : QLineEdit(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QLineEdit::constructor");
    setObjectName(QString::fromStdString("_Internal_QLineEdit_" + std::to_string(gtextField->getID())));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(handleTextChange(const QString&)));
}

void _Internal_QLineEdit::detach() {
    _gtextfield = nullptr;
}

void _Internal_QLineEdit::handleTextChange(const QString&) {
    if (!_gtextfield) {
        return;
    }
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);

    // BUGFIX: for backward compatibility, also fire a CHANGE_EVENT
    // (emits only to the old-style waitForEvent function)
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "statechange",
                /* source */ _gtextfield);
    changeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(changeEvent);
}

void _Internal_QLineEdit::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QLineEdit::keyPressEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("action")) {
        QLineEdit::keyPressEvent(event);   // call super
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            GEvent actionEvent(
                        /* class  */ ACTION_EVENT,
                        /* type   */ ACTION_PERFORMED,
                        /* name   */ "action",
                        /* source */ _gtextfield);
            actionEvent.setActionCommand(_gtextfield->getActionCommand());
            _gtextfield->fireEvent(actionEvent);
        }
    } else if (_gtextfield && _gtextfield->isAcceptingEvent("keypress")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QLineEdit::keyPressEvent(event);   // call super
        }
    } else {
        QLineEdit::keyPressEvent(event);   // call super
    }
}

void _Internal_QLineEdit::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QLineEdit::keyReleaseEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QLineEdit::keyReleaseEvent(event);   // call super
        }
    } else {
        QLineEdit::keyReleaseEvent(event);   // call super
    }
}

QSize _Internal_QLineEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QLineEdit::sizeHint();
    }
}

_Internal_QSpinBox::_Internal_QSpinBox(GTextField* gtextField, int min, int max, int step, QWidget* parent)
        : QSpinBox(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QSpinBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QSpinBox_" + std::to_string(gtextField->getID())));
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QSpinBox::detach() {
    _gtextfield = nullptr;
}

void _Internal_QSpinBox::handleTextChange(const QString&) {
    if (!_gtextfield) {
        return;
    }
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);
}

void _Internal_QSpinBox::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QSpinBox::keyPressEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("action")) {
        QSpinBox::keyPressEvent(event);   // call super
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            GEvent actionEvent(
                        /* class  */ ACTION_EVENT,
                        /* type   */ ACTION_PERFORMED,
                        /* name   */ "action",
                        /* source */ _gtextfield);
            actionEvent.setActionCommand(_gtextfield->getActionCommand());
            _gtextfield->fireEvent(actionEvent);
        }
    } else if (_gtextfield && _gtextfield->isAcceptingEvent("keypress")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QSpinBox::keyPressEvent(event);   // call super
        }
    } else {
        QSpinBox::keyPressEvent(event);   // call super
    }
}

void _Internal_QSpinBox::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QSpinBox::keyReleaseEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QSpinBox::keyReleaseEvent(event);   // call super
        }
    } else {
        QSpinBox::keyReleaseEvent(event);   // call super
    }
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

_Internal_QDoubleSpinBox::_Internal_QDoubleSpinBox(GTextField* gtextField, double min, double max, double step, QWidget* parent)
        : QDoubleSpinBox(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QDoubleSpinBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QDoubleSpinBox_" + std::to_string(gtextField->getID())));
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QDoubleSpinBox::detach() {
    _gtextfield = nullptr;
}

void _Internal_QDoubleSpinBox::handleTextChange(const QString&) {
    if (!_gtextfield) {
        return;
    }
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);
}

void _Internal_QDoubleSpinBox::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QDoubleSpinBox::keyPressEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("action")) {
        QDoubleSpinBox::keyPressEvent(event);   // call super
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            GEvent actionEvent(
                        /* class  */ ACTION_EVENT,
                        /* type   */ ACTION_PERFORMED,
                        /* name   */ "action",
                        /* source */ _gtextfield);
            actionEvent.setActionCommand(_gtextfield->getActionCommand());
            _gtextfield->fireEvent(actionEvent);
        }
    } else if (_gtextfield && _gtextfield->isAcceptingEvent("keypress")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QDoubleSpinBox::keyPressEvent(event);   // call super
        }
    } else {
        QDoubleSpinBox::keyPressEvent(event);   // call super
    }
}

void _Internal_QDoubleSpinBox::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QDoubleSpinBox::keyReleaseEvent", "event");
    if (_gtextfield && _gtextfield->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gtextfield->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QDoubleSpinBox::keyReleaseEvent(event);   // call super
        }
    } else {
        QDoubleSpinBox::keyReleaseEvent(event);   // call super
    }
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


