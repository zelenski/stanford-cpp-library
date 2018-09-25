/*
 * File: gdiffgui.cpp
 * ------------------
 * 
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gdiffgui.cpp to replace Java version
 * @version 2018/07/31
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gdiffgui.h"
#include <iostream>
#include <string>
#include "consoletext.h"
#include "diff.h"
#undef INTERNAL_INCLUDE

void GDiffGui::showDialog(const std::string& name1,
                          const std::string& text1,
                          const std::string& name2,
                          const std::string& text2,
                          bool showCheckBoxes) {
    GDiffGui diffGui(name1, text1, name2, text2, showCheckBoxes);
}

GDiffGui::GDiffGui(const std::string& name1,
                   const std::string& text1,
                   const std::string& name2,
                   const std::string& text2,
                   bool /*showCheckBoxes*/) {

    int flags = diff::DIFF_DEFAULT_FLAGS;
    std::string diffs = diff::diff(text1, text2, flags);

    _window = new GWindow(800, 600);
    _window->setTitle("Compare Output");
    _hsplitter = new QSplitter(/* orientation */ Qt::Horizontal, /* parent */ _window->getWidget());

    std::string font = getConsoleFont();
    _textAreaLeft  = new GTextArea(name1 + ":\n" + text1);
    _textAreaLeft->setEditable(false);
    _textAreaLeft->setFont(font);
    _textAreaLeft->setLineWrap(false);
    _textAreaRight = new GTextArea(name2 + ":\n" + text2);
    _textAreaRight->setEditable(false);
    _textAreaRight->setFont(font);
    _textAreaRight->setLineWrap(false);
    _textAreaBottom = new GTextArea("Differences:\n" + diffs);
    _textAreaBottom->setEditable(false);
    _textAreaBottom->setFont(font);
    _textAreaBottom->setLineWrap(false);
    _hsplitter->addWidget(_textAreaLeft->getWidget());
    _hsplitter->addWidget(_textAreaRight->getWidget());
    _hsplitter->setStretchFactor(1, 1);
    _hsplitterInteractor = new GGenericInteractor<QSplitter>(_hsplitter);

    _vsplitter = new QSplitter(/* orientation */ Qt::Vertical, /* parent */ _window->getWidget());
    _vsplitter->addWidget(_hsplitter);
    _vsplitter->addWidget(_textAreaBottom->getWidget());
    _vsplitter->setStretchFactor(1, 1);
    _vsplitterInteractor = new GGenericInteractor<QSplitter>(_vsplitter);

    _window->addToRegion(_vsplitterInteractor, "Center");
    _window->center();
    _window->show();
}

GDiffGui::~GDiffGui() {
    // TODO: delete
    _window = nullptr;
    _hsplitter = nullptr;
    _vsplitter = nullptr;
    _textAreaLeft = nullptr;
    _textAreaRight = nullptr;
    _textAreaBottom = nullptr;
    _hsplitterInteractor = nullptr;
    _vsplitterInteractor = nullptr;
}
