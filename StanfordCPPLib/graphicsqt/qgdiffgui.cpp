/*
 * File: qgdiffgui.cpp
 * -------------------
 * 
 * @author Marty Stepp
 * @version 2018/07/31
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgdiffgui.h"
#include <iostream>
#include <string>
#include "diff.h"
#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "qconsole.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

void QGDiffGui::showDialog(const std::string& name1,
                           const std::string& text1,
                           const std::string& name2,
                           const std::string& text2,
                           bool showCheckBoxes) {
    QGDiffGui diffGui(name1, text1, name2, text2, showCheckBoxes);
}

QGDiffGui::QGDiffGui(const std::string& name1,
                     const std::string& text1,
                     const std::string& name2,
                     const std::string& text2,
                     bool /*showCheckBoxes*/) {

    int flags = diff::DIFF_DEFAULT_FLAGS;
    std::string diffs = diff::diff(text1, text2, flags);

    _window = new QGWindow(800, 600);
    _window->setTitle("Compare Output");
    _hsplitter = new QSplitter(/* orientation */ Qt::Horizontal, /* parent */ _window->getWidget());

    std::string font = stanfordcpplib::qtgui::getConsoleFont();
    _textAreaLeft  = new QGTextArea(name1 + ":\n" + text1);
    _textAreaLeft->setEditable(false);
    _textAreaLeft->setFont(font);
    _textAreaLeft->setLineWrap(false);
    _textAreaRight = new QGTextArea(name2 + ":\n" + text2);
    _textAreaRight->setEditable(false);
    _textAreaRight->setFont(font);
    _textAreaRight->setLineWrap(false);
    _textAreaBottom = new QGTextArea("Differences:\n" + diffs);
    _textAreaBottom->setEditable(false);
    _textAreaBottom->setFont(font);
    _textAreaBottom->setLineWrap(false);
    _hsplitter->addWidget(_textAreaLeft->getWidget());
    _hsplitter->addWidget(_textAreaRight->getWidget());
    _hsplitter->setStretchFactor(1, 1);
    _hsplitterInteractor = new QGGenericInteractor<QSplitter>(_hsplitter);

    _vsplitter = new QSplitter(/* orientation */ Qt::Vertical, /* parent */ _window->getWidget());
    _vsplitter->addWidget(_hsplitter);
    _vsplitter->addWidget(_textAreaBottom->getWidget());
    _vsplitter->setStretchFactor(1, 1);
    _vsplitterInteractor = new QGGenericInteractor<QSplitter>(_vsplitter);

    _window->addToRegion(_vsplitterInteractor, "Center");
    _window->center();
    _window->show();
}

#endif // SPL_QT_GUI
