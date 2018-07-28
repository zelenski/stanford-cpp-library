/*
 * File: qgchooser.cpp
 * -------------------
 *
 * @version 2018/06/28
 * - initial version
 */

#include "qgchooser.h"
#include "error.h"
#include "qgwindow.h"
#include "strlib.h"

_Internal_QComboBox::_Internal_QComboBox(QGChooser* qgchooser, QWidget* parent)
        : QComboBox(parent),
          _qgchooser(qgchooser) {
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChange()));
}

void _Internal_QComboBox::handleChange() {
    QGEvent changeEvent(
                /* class  */ QGEvent::CHANGE_EVENT,
                /* type   */ QGEvent::STATE_CHANGED,
                /* name   */ "change",
                /* source */ _qgchooser);
    changeEvent.setActionCommand(_qgchooser->getActionCommand());
    _qgchooser->fireEvent(changeEvent);
}

QSize _Internal_QComboBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QComboBox::sizeHint();
    }
}


QGChooser::QGChooser(QWidget* parent) {
    _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
}

QGChooser::QGChooser(const std::initializer_list<std::string>& items, QWidget* parent) {
    _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
    addItems(items);
}

QGChooser::QGChooser(const Vector<std::string>& items, QWidget* parent) {
    _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
    addItems(items);
}

QGChooser::~QGChooser() {
    // TODO: delete _iqcomboBox;
    _iqcomboBox = nullptr;
}

void QGChooser::addItem(const std::string& item) {
    _iqcomboBox->addItem(QString::fromStdString(item));
}

void QGChooser::addItems(const std::initializer_list<std::string>& items) {
    for (const std::string& item : items) {
        addItem(item);
    }
}

void QGChooser::addItems(const Vector<std::string>& items) {
    for (const std::string& item : items) {
        addItem(item);
    }
}

void QGChooser::checkIndex(const std::string& member, int index, int min, int max) const {
    if (max < 0) {
        max = size() - 1;
    }
    if (index < min || index > max) {
        error(member + ": index was " + integerToString(index) + " but must be between "
              + integerToString(min) + " and " + integerToString(max) + " inclusive");
    }
}

void QGChooser::clearItems() {
    _iqcomboBox->clear();
}

std::string QGChooser::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getSelectedItem();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* QGChooser::getInternalWidget() const {
    return _iqcomboBox;
}

std::string QGChooser::getItem(int index) const {
    checkIndex("QGChooser::getItem", index);
    return _iqcomboBox->itemText(index).toStdString();
}

int QGChooser::getSelectedIndex() const {
    return _iqcomboBox->currentIndex();
}

std::string QGChooser::getSelectedItem() const {
    return _iqcomboBox->currentText().toStdString();
}

std::string QGChooser::getType() const {
    return "QGChooser";
}

QWidget* QGChooser::getWidget() const {
    return static_cast<QWidget*>(_iqcomboBox);
}

bool QGChooser::isEditable() const {
    return _iqcomboBox->isEditable();
}

void QGChooser::removeActionListener() {
    removeEventListener("change");
}

void QGChooser::setActionListener(QGEventListener func) {
    setEventListener("change", func);
}

void QGChooser::setActionListener(QGEventListenerVoid func) {
    setEventListener("change", func);
}

void QGChooser::setItem(int index, const std::string& item) {
    checkIndex("QGChooser::setItem", index);
    _iqcomboBox->setItemText(index, QString::fromStdString(item));
}

void QGChooser::setSelectedIndex(int index) {
    checkIndex("QGChooser::setSelectedIndex", index);
    _iqcomboBox->setCurrentIndex(index);
}

void QGChooser::setEditable(bool editable) {
    _iqcomboBox->setEditable(editable);
}

void QGChooser::setSelectedItem(const std::string& item) {
    _iqcomboBox->setCurrentText(QString::fromStdString(item));
}

int QGChooser::size() const {
    return _iqcomboBox->count();
}
