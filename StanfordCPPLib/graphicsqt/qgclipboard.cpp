/*
 * File: qgclipboard.cpp
 * ---------------------
 *
 * @version 2018/07/19
 * - initial version
 */

#include "qgclipboard.h"
#include <QApplication>
#include <QClipboard>
#include <QString>

void QGClipboard::set(const std::string& text) {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromStdString(text));
}

std::string QGClipboard::get() {
    QClipboard* clipboard = QApplication::clipboard();
    return clipboard->text().toStdString();
}

bool QGClipboard::isCopy(QKeyEvent* event) {
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return ctrlOrMeta && (event->key() == Qt::Key_C || event->key() == Qt::Key_Insert);
}

bool QGClipboard::isCut(QKeyEvent* event) {
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_X)
            || (shift && event->key() == Qt::Key_Delete);
}

bool QGClipboard::isPaste(QKeyEvent* event) {
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_V)
            || (shift && event->key() == Qt::Key_Insert);
}
