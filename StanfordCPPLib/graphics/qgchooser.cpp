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

_Q_Internal_Chooser::_Q_Internal_Chooser(QGChooser* chooser, QWidget* parent)
        : QComboBox(parent),
          _chooser(chooser) {
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChange()));
}

void _Q_Internal_Chooser::handleChange() {
    if (_chooser->_changeHandler) {
        _chooser->_changeHandler();
    }
}


QGChooser::QGChooser(QWidget* parent)
        : _chooser(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _changeHandler(nullptr) {
    // empty
}

QGChooser::QGChooser(const std::initializer_list<std::string>& items, QWidget* parent)
        : _chooser(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _changeHandler(nullptr) {
    addItems(items);
}

QGChooser::QGChooser(const Vector<std::string>& items, QWidget* parent)
        : _chooser(this, parent ? parent : (QWidget*) QGWindow::getLastWindow()),
          _changeHandler(nullptr) {
    addItems(items);
}

void QGChooser::addItem(const std::string& item) {
    _chooser.addItem(QString::fromStdString(item));
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
    _chooser.clear();
}

std::string QGChooser::getItem(int index) const {
    checkIndex("QGChooser::getItem", index);
    return _chooser.itemText(index).toStdString();
}

int QGChooser::getSelectedIndex() const {
    return _chooser.currentIndex();
}

std::string QGChooser::getSelectedItem() const {
    return _chooser.currentText().toStdString();
}

QWidget* QGChooser::getWidget() const {
    return (QWidget*) &_chooser;
}

bool QGChooser::isEditable() const {
    return _chooser.isEditable();
}

bool QGChooser::isEnabled() const {
    return _chooser.isEnabled();
}

void QGChooser::setChangeHandler(void (* func)()) {
    _changeHandler = func;
}

void QGChooser::setItem(int index, const std::string& item) {
    checkIndex("QGChooser::setItem", index);
    _chooser.setItemText(index, QString::fromStdString(item));
}

void QGChooser::setSelectedIndex(int index) {
    checkIndex("QGChooser::setSelectedIndex", index);
    _chooser.setCurrentIndex(index);
}

void QGChooser::setEditable(bool editable) {
    _chooser.setEditable(editable);
}

void QGChooser::setEnabled(bool enabled) {
    _chooser.setEnabled(enabled);
}

void QGChooser::setSelectedItem(const std::string& item) {
    _chooser.setCurrentText(QString::fromStdString(item));
}

int QGChooser::size() const {
    return _chooser.count();
}
