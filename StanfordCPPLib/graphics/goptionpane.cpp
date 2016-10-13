/*
 * File: goptionpane.cpp
 * ---------------------
 * This file implements the operations declared in the goptionpane.h file.
 * See goptionpane.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/26
 * - added showTextFileDialog
 * @version 2014/10/09
 * - finished implementation of showOptionDialog; added dialog types and other fixes
 * - enum rearrangement for clearer client code and better error messages
 * - removed 'using namespace' statement
 * @since 2014/07/09
 */

#include "goptionpane.h"
#include "private/platform.h"

GOptionPane::GOptionPane() {
    // empty
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(std::string message, std::string title,
                                                          ConfirmType type) {
    if (type != GOptionPane::ConfirmType::YES_NO
            && type != GOptionPane::ConfirmType::YES_NO_CANCEL
            && type != GOptionPane::ConfirmType::OK_CANCEL) {
        error("GOptionPane::showConfirmDialog: Illegal dialog type");
    }
    if (title.empty()) {
        title = "Select an option";
    }
    
    int result = stanfordcpplib::getPlatform()->goptionpane_showConfirmDialog(message, title, type);
    if (result == OK_OPTION || result == YES_OPTION) {
        // this is weird code because JOptionPane thinks of OK and Yes as the same,
        // and differentiates based on context of whether this is an OK/Cancel or Yes/No dialog
        return (type == GOptionPane::ConfirmType::OK_CANCEL)
                ? GOptionPane::ConfirmResult::OK : GOptionPane::ConfirmResult::YES;
    } else if (result == NO_OPTION) {
        return GOptionPane::ConfirmResult::NO;
    } else {
        return GOptionPane::ConfirmResult::CANCEL;
    }
}

std::string GOptionPane::showInputDialog(std::string message, std::string title) {
    return stanfordcpplib::getPlatform()->goptionpane_showInputDialog(message, title);
}

void GOptionPane::showMessageDialog(std::string message, std::string title, MessageType type) {
    if (type != GOptionPane::MessageType::PLAIN
            && type != GOptionPane::MessageType::INFORMATION
            && type != GOptionPane::MessageType::ERROR
            && type != GOptionPane::MessageType::WARNING
            && type != GOptionPane::MessageType::QUESTION) {
        error("GOptionPane::showMessageDialog: Illegal dialog type");
    }
    if (title.empty()) {
        title = "Message";
    }
    stanfordcpplib::getPlatform()->goptionpane_showMessageDialog(message, title, type);
}

std::string GOptionPane::showOptionDialog(std::string message, const Vector<std::string>& options,
                                          std::string title, std::string initiallySelected) {
    if (title.empty()) {
        title = "Select an option";
    }
    int index = stanfordcpplib::getPlatform()->goptionpane_showOptionDialog(message, title, options.toStlVector(), initiallySelected);
    if (index == GOptionPane::InternalResult::CLOSED_OPTION
            || index < 0 || index >= options.size()) {
        return "";
    } else {
        return options[index];
    }
}

void GOptionPane::showTextFileDialog(std::string message, std::string title, int rows, int cols) {
    if (title.empty()) {
        title = "Text file contents";
    }
    stanfordcpplib::getPlatform()->goptionpane_showTextFileDialog(message, title, rows, cols);
}
