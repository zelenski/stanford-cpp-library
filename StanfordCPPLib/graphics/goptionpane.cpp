/*
 * File: goptionpane.cpp
 * ---------------------
 *
 * This code is largely copied from goptionpane.cpp and modified to use
 * Qt's QMessageBox and QInputDialog classes.
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to goptionpane.cpp to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#include "goptionpane.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QWidget>
#include "error.h"
#include "gbutton.h"
#include "gtextarea.h"
#include "gthread.h"
#include "gwindow.h"
#include "strlib.h"

GOptionPane::GOptionPane() {
    // empty
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    return showConfirmDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, type);
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(GWindow* parent,
                                                          const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    return showConfirmDialog(parent ? parent->getWidget() : nullptr, message, title, type);
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(QWidget* parent,
                                                          const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    if (type != GOptionPane::ConfirmType::YES_NO
            && type != GOptionPane::ConfirmType::YES_NO_CANCEL
            && type != GOptionPane::ConfirmType::OK_CANCEL) {
        error("GOptionPane::showConfirmDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Select an option") : title;

    // convert our enum types to Qt's button enum type
    QMessageBox::StandardButtons buttons;
    QMessageBox::StandardButton defaultButton = QMessageBox::Cancel;
    if (type == GOptionPane::ConfirmType::YES_NO) {
        buttons = QMessageBox::Yes | QMessageBox::No;
        defaultButton = QMessageBox::No;
    } else if (type == GOptionPane::ConfirmType::YES_NO_CANCEL) {
        buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    } else if (type == GOptionPane::ConfirmType::OK_CANCEL) {
        buttons = QMessageBox::Ok | QMessageBox::Cancel;
    }

    GOptionPane::ConfirmResult confirmResult = GOptionPane::CANCEL;
    GThread::runOnQtGuiThread([parent, titleToUse, message, buttons, defaultButton, &confirmResult]() {
        int dialogResult = QMessageBox::question(parent,
                QString::fromStdString(titleToUse),
                QString::fromStdString(message),
                buttons,
                defaultButton);
        switch (dialogResult) {
            case QMessageBox::Yes:
                confirmResult = GOptionPane::ConfirmResult::YES;
                break;
            case QMessageBox::No:
                confirmResult = GOptionPane::ConfirmResult::NO;
                break;
            case QMessageBox::Cancel:
            default:
                confirmResult = GOptionPane::ConfirmResult::CANCEL;
                break;
        }
    });
    return confirmResult;
}

std::string GOptionPane::showInputDialog(const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    return showInputDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, initialValue);
}

std::string GOptionPane::showInputDialog(GWindow* parent,
                                         const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    return showInputDialog(parent ? parent->getWidget() : nullptr, message, title, initialValue);
}

std::string GOptionPane::showInputDialog(QWidget* parent,
                                         const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    std::string titleToUse = title.empty() ? std::string("Type a value") : title;
    std::string result = "";
    GThread::runOnQtGuiThread([parent, titleToUse, message, initialValue, &result]() {
        result = QInputDialog::getText(parent,
                QString::fromStdString(titleToUse),
                QString::fromStdString(message),
                QLineEdit::Normal,
                QString::fromStdString(initialValue)).toStdString();
    });
    return result;
}

void GOptionPane::showMessageDialog(const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    showMessageDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, type);
}

void GOptionPane::showMessageDialog(GWindow* parent,
                                    const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    showMessageDialog(parent ? parent->getWidget() : nullptr, message, title, type);
}

void GOptionPane::showMessageDialog(QWidget* parent,
                                    const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    if (type != GOptionPane::MessageType::PLAIN
            && type != GOptionPane::MessageType::INFORMATION
            && type != GOptionPane::MessageType::ERROR
            && type != GOptionPane::MessageType::WARNING
            && type != GOptionPane::MessageType::QUESTION
            && type != GOptionPane::MessageType::ABOUT) {
        error("GOptionPane::showMessageDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Message") : title;

    GThread::runOnQtGuiThread([parent, message, titleToUse, type]() {
        if (type == GOptionPane::MessageType::PLAIN
                || type == GOptionPane::MessageType::INFORMATION
                || type == GOptionPane::MessageType::QUESTION) {
            QMessageBox::information(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::WARNING) {
            QMessageBox::warning(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::ERROR) {
            QMessageBox::critical(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::ABOUT) {
            QMessageBox::about(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        }
    });
}

std::string GOptionPane::showOptionDialog(const std::string& message,
                                          const Vector<std::string>& options,
                                          const std::string& title,
                                          const std::string& initiallySelected) {
    return showOptionDialog(/* parent */ static_cast<QWidget*>(nullptr), message, options, title, initiallySelected);
}

std::string GOptionPane::showOptionDialog(GWindow* parent,
                                          const std::string& message,
                                          const Vector<std::string>& options,
                                          const std::string& title,
                                          const std::string& initiallySelected) {
    return showOptionDialog(parent ? parent->getWidget() : nullptr, message, options, title, initiallySelected);
}

std::string GOptionPane::showOptionDialog(QWidget* parent,
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
    box.setWindowTitle(QString::fromStdString(titleToUse));
    box.setAttribute(Qt::WA_QuitOnClose, false);

    for (std::string option : options) {
        box.addButton(QString::fromStdString(option), QMessageBox::ActionRole);
    }
    if (!initiallySelected.empty()) {
        // TODO: dunno how to set initially selected button properly
        // box.setDefaultButton(QString::fromStdString(initiallySelected));
    }

    std::string result = "";
    GThread::runOnQtGuiThread([&box, &options, &result]() {
        int index = box.exec();
        if (index == GOptionPane::InternalResult::CLOSED_OPTION
                || index < 0 || index >= options.size()) {
            result = "";
        } else {
            result = options[index];
        }
    });
    return result;
}

void GOptionPane::showTextFileDialog(const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    showTextFileDialog(static_cast<QWidget*>(nullptr), fileText, title, rows, cols);
}

void GOptionPane::showTextFileDialog(GWindow* parent,
                                     const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    showTextFileDialog(parent ? parent->getWidget() : nullptr, fileText, title, rows, cols);
}

void GOptionPane::showTextFileDialog(QWidget* /*parent*/,
                                     const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    static const std::string DEFAULT_FONT = "Monospaced-*-*";
    static const int DEFAULT_ROWS    = 20;
    static const int DEFAULT_COLUMNS = 80;
    if (rows <= 0) {
        rows = DEFAULT_ROWS;
    }
    if (cols <= 0) {
        cols = DEFAULT_COLUMNS;
    }

    std::string titleToUse = title.empty() ? std::string("Text file contents") : title;
    GWindow* window = new GWindow;
    window->setTitle(title);

    GTextArea* textArea = new GTextArea(fileText);
    textArea->setFont(DEFAULT_FONT);
    textArea->setRowsColumns(DEFAULT_ROWS, DEFAULT_COLUMNS);
    window->addToRegion(textArea, GWindow::REGION_CENTER);

    GButton* button = new GButton("&OK");
    button->setActionListener([window]() {
        window->close();
    });
    window->addToRegion(button, GWindow::REGION_SOUTH);

    window->pack();
    window->center();
    window->show();
}
