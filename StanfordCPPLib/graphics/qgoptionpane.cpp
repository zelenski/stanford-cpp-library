/*
 * File: qgoptionpane.cpp
 * ----------------------
 *
 * This code is largely copied from goptionpane.cpp and modified to use
 * Qt's QMessageBox and QInputDialog classes.
 *
 * @version 2018/06/28
 * - initial version
 */

#include "qgoptionpane.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QWidget>
#include "error.h"
#include "strlib.h"

QGOptionPane::QGOptionPane() {
    // empty
}

QGOptionPane::ConfirmResult QGOptionPane::showConfirmDialog(const std::string& message,
                                                            const std::string& title,
                                                            ConfirmType type) {
    return showConfirmDialog(/* parent */ nullptr, message, title, type);
}

QGOptionPane::ConfirmResult QGOptionPane::showConfirmDialog(QWidget* parent,
                                                            const std::string& message,
                                                            const std::string& title,
                                                            ConfirmType type) {
    if (type != QGOptionPane::ConfirmType::YES_NO
            && type != QGOptionPane::ConfirmType::YES_NO_CANCEL
            && type != QGOptionPane::ConfirmType::OK_CANCEL) {
        error("QGOptionPane::showConfirmDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Select an option") : title;

    // convert our enum types to Qt's button enum type
    QMessageBox::StandardButtons buttons;
    QMessageBox::StandardButton defaultButton = QMessageBox::Cancel;
    if (type == QGOptionPane::ConfirmType::YES_NO) {
        buttons = QMessageBox::Yes | QMessageBox::No;
        defaultButton = QMessageBox::No;
    } else if (type == QGOptionPane::ConfirmType::YES_NO_CANCEL) {
        buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    } else if (type == QGOptionPane::ConfirmType::OK_CANCEL) {
        buttons = QMessageBox::Ok | QMessageBox::Cancel;
    }

    int result = QMessageBox::question(parent,
            QString::fromStdString(titleToUse),
            QString::fromStdString(message),
            buttons,
            defaultButton);
    switch (result) {
        case QMessageBox::Yes:
            return QGOptionPane::ConfirmResult::YES;
        case QMessageBox::No:
            return QGOptionPane::ConfirmResult::NO;
        case QMessageBox::Cancel:
        default:
            return QGOptionPane::ConfirmResult::CANCEL;
    }
}

std::string QGOptionPane::showInputDialog(const std::string& message,
                                          const std::string& title,
                                          const std::string& initialValue) {
    return showInputDialog(/* parent */ nullptr, message, title, initialValue);
}

std::string QGOptionPane::showInputDialog(QWidget* parent,
                                          const std::string& message,
                                          const std::string& title,
                                          const std::string& initialValue) {
    std::string titleToUse = title.empty() ? std::string("Type a value") : title;
    return QInputDialog::getText(parent,
            QString::fromStdString(titleToUse),
            QString::fromStdString(message),
            QLineEdit::Normal,
            QString::fromStdString(initialValue)).toStdString();
}

void QGOptionPane::showMessageDialog(const std::string& message,
                                     const std::string& title,
                                     MessageType type) {
    showMessageDialog(/* parent */ nullptr, message, title, type);
}

void QGOptionPane::showMessageDialog(QWidget* parent,
                                     const std::string& message,
                                     const std::string& title,
                                     MessageType type) {
    if (type != QGOptionPane::MessageType::PLAIN
            && type != QGOptionPane::MessageType::INFORMATION
            && type != QGOptionPane::MessageType::ERROR
            && type != QGOptionPane::MessageType::WARNING
            && type != QGOptionPane::MessageType::QUESTION) {
        error("QGOptionPane::showMessageDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Message") : title;

    if (type == QGOptionPane::MessageType::PLAIN
            || type == QGOptionPane::MessageType::INFORMATION
            || type == QGOptionPane::MessageType::QUESTION) {
        QMessageBox::information(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
    } else if (type == QGOptionPane::MessageType::WARNING) {
        QMessageBox::warning(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
    } else if (type == QGOptionPane::MessageType::ERROR) {
        QMessageBox::critical(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
    }
}

std::string QGOptionPane::showOptionDialog(const std::string& message,
                                           const Vector<std::string>& options,
                                           const std::string& title,
                                           const std::string& initiallySelected) {
    return showOptionDialog(/* parent */ nullptr, message, options, title, initiallySelected);
}

std::string QGOptionPane::showOptionDialog(QWidget* parent,
                                           const std::string& message,
                                           const Vector<std::string>& options,
                                           const std::string& title,
                                           const std::string& initiallySelected) {
    std::string titleToUse = title.empty() ? std::string("Select an option") : title;
    QMessageBox box;
    if (parent) {
        box.setParent(parent);
    }
    box.setText(QString::fromStdString(message));
    box.setWindowTitle(QString::fromStdString(title));

    for (std::string option : options) {
        box.addButton(QString::fromStdString(option), QMessageBox::ActionRole);
    }
    if (!initiallySelected.empty()) {
        // TODO: dunno how to set initially selected button properly
        // box.setDefaultButton(QString::fromStdString(initiallySelected));
    }
    int index = box.exec();

    if (index == QGOptionPane::InternalResult::CLOSED_OPTION
            || index < 0 || index >= options.size()) {
        return "";
    } else {
        return options[index];
    }
}

void QGOptionPane::showTextFileDialog(const std::string& message,
                                      const std::string& title,
                                      int /* rows */,
                                      int /* cols */) {
    std::string titleToUse = title.empty() ? std::string("Text file contents") : title;
    showMessageDialog(message, titleToUse);
    // TODO
    error("QGOptionPane::showTextFileDialog: not implemented");
}
void QGOptionPane::showTextFileDialog(QWidget* parent,
                                      const std::string& message,
                                      const std::string& title,
                                      int /* rows */,
                                      int /* cols */) {
    std::string titleToUse = title.empty() ? std::string("Text file contents") : title;
    showMessageDialog(parent, message, titleToUse);
    // TODO
    error("QGOptionPane::showTextFileDialog: not implemented");
}
