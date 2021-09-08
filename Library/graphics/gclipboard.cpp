/*
 * File: gclipboard.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gclipboard.cpp to replace Java version
 * @version 2018/07/19
 * - initial version
 */

#include "gclipboard.h"
#include <QApplication>
#include <QClipboard>
#include <QString>
#include "require.h"

GClipboard::GClipboard() {
    // empty
}

void GClipboard::set(const std::string& text) {
    QClipboard* clipboard = QApplication::clipboard();
    require::nonNull(clipboard, "GClipboard::set", "clipboard");
    clipboard->setText(QString::fromStdString(text));
}

std::string GClipboard::get() {
    QClipboard* clipboard = QApplication::clipboard();
    require::nonNull(clipboard, "GClipboard::get", "clipboard");
    return clipboard->text().toStdString();
}

bool GClipboard::isCopy(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return ctrlOrMeta && (event->key() == Qt::Key_C || event->key() == Qt::Key_Insert);
}

bool GClipboard::isCut(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_X)
            || (shift && event->key() == Qt::Key_Delete);
}

bool GClipboard::isPaste(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_V)
            || (shift && event->key() == Qt::Key_Insert);
}
