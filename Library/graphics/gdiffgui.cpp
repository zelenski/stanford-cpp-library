/*
 * File: gdiffgui.cpp
 * ------------------
 * 
 * @author Marty Stepp
 * @version 2019/04/20
 * - display expected/actual values using toPrintable to show non-printable characters better
 * @version 2018/10/06
 * - allow passing diff flags
 * @version 2018/09/27
 * - bug fix for allocating on heap rather than stack
 * @version 2018/08/23
 * - renamed to gdiffgui.cpp to replace Java version
 * @version 2018/07/31
 * - initial version
 */

#include "gdiffgui.h"
#include <iostream>
#include <QScrollBar>
#include <string>
#include "consoletext.h"
#include "gthread.h"
#include "stringutils.h"

/*static*/ const std::string GDiffGui::COLOR_EXPECTED = "#009900";
/*static*/ const std::string GDiffGui::COLOR_EXPECTED_DARK_MODE = "#55ff44";
/*static*/ const std::string GDiffGui::COLOR_LINE_NUMBERS = "#888888";
/*static*/ const std::string GDiffGui::COLOR_LINE_NUMBERS_DARK_MODE = "#aaaaaa";
/*static*/ const std::string GDiffGui::COLOR_STUDENT  = "#bb0000";
/*static*/ const std::string GDiffGui::COLOR_STUDENT_DARK_MODE = "#f47862";
/*static*/ const bool GDiffGui::LINE_NUMBERS = true;

void GDiffGui::showDialog(const std::string& name1,
                          const std::string& text1,
                          const std::string& name2,
                          const std::string& text2,
                          int diffFlags,
                          bool showCheckBoxes) {
    /* GDiffGui* diffGui = */ new GDiffGui(name1, text1, name2, text2, diffFlags, showCheckBoxes);
    // TODO: delete diffGui;
    // but don't do it too quickly or the lambda functions won't be able to
    // work on the window
}

GDiffGui::GDiffGui(const std::string& name1,
                   const std::string& text1,
                   const std::string& name2,
                   const std::string& text2,
                   int diffFlags,
                   bool /*showCheckBoxes*/) {
    GThread::runOnQtGuiThread([this, name1, text1, name2, text2, diffFlags]() {
        std::string diffs = diff::diff(text1, text2, diffFlags);

        _window = new GWindow(800, 600);
        _window->setTitle("Compare Output");
        _hsplitter = new QSplitter(/* orientation */ Qt::Horizontal, /* parent */ _window->getWidget());
        _window->setCloseOperation(GWindow::CLOSE_HIDE);

        // function to close the window when Escape is pressed
        // (similar to code in gdiffimage.cpp)
        auto windowCloseLambda = [this](GEvent event) {
            if (event.getType() == KEY_PRESSED && event.getKeyChar() == GEvent::ESCAPE_KEY) {
                _window->close();
            }
        };

        std::string font = getConsoleFont();
        _textAreaLeft  = new GTextArea();
        setupLeftRightText(_textAreaLeft, name1 + ":\n" + text1);
        _textAreaLeft->setEditable(false);
        _textAreaLeft->setFont(font);
        _textAreaLeft->setLineWrap(false);
        _textAreaLeft->setKeyListener(windowCloseLambda);
        _textAreaLeft->setEventListener("scroll", [this]() {
            syncScrollBars(/* left */ true);
        });

        _textAreaRight = new GTextArea();
        setupLeftRightText(_textAreaRight, name2 + ":\n" + text2);
        _textAreaRight->setEditable(false);
        _textAreaRight->setFont(font);
        _textAreaRight->setLineWrap(false);
        _textAreaRight->setKeyListener(windowCloseLambda);
        _textAreaRight->setEventListener("scroll", [this]() {
            syncScrollBars(/* left */ false);
        });

        _textAreaBottom = new GTextArea("Differences:\n");
        _textAreaBottom->setEditable(false);
        _textAreaBottom->setFont(font);
        _textAreaBottom->setLineWrap(false);
        _textAreaBottom->setKeyListener(windowCloseLambda);
        setupDiffText(diffs);
        _hsplitter->addWidget(_textAreaLeft->getWidget());
        _hsplitter->addWidget(_textAreaRight->getWidget());
        _hsplitter->setStretchFactor(0, 1);
        _hsplitter->setStretchFactor(1, 1);
        _hsplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));   // evenly size the two halves
        _hsplitterInteractor = new GGenericInteractor<QSplitter>(_hsplitter);

        _vsplitter = new QSplitter(/* orientation */ Qt::Vertical, /* parent */ _window->getWidget());
        _vsplitter->addWidget(_hsplitter);
        _vsplitter->addWidget(_textAreaBottom->getWidget());
        _vsplitter->setStretchFactor(0, 1);
        _vsplitter->setStretchFactor(1, 1);
        _vsplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));   // evenly size the two halves
        _vsplitterInteractor = new GGenericInteractor<QSplitter>(_vsplitter);

        _window->addToRegion(_vsplitterInteractor, GWindow::REGION_CENTER);
        _window->setKeyListener(windowCloseLambda);
        _window->center();
        _window->show();
    });
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

void GDiffGui::setupDiffText(const std::string& diffs) {
    Vector<std::string> lines = stringSplit(diffs, "\n");
    const std::string COLOR_NORMAL = _textAreaBottom->getColor();
    for (std::string line : lines) {
        std::string color;
        if (startsWith(line, "EXPECTED <")) {
            color = GWindow::chooseLightDarkModeColor(COLOR_EXPECTED, COLOR_EXPECTED_DARK_MODE);
        } else if (startsWith(line, "STUDENT  >")) {
            color = GWindow::chooseLightDarkModeColor(COLOR_STUDENT, COLOR_STUDENT_DARK_MODE);
        } else {
            color = COLOR_NORMAL;
        }

        // BUGFIX: display special characters with extra printable character info
        line = stringutils::toPrintable(line);

        _textAreaBottom->appendFormattedText(line + "\n", color);
    }
}

void GDiffGui::setupLeftRightText(GTextArea* textArea, const std::string& text) {
    if (!LINE_NUMBERS) {
        textArea->setText(text);
        return;
    }

    Vector<std::string> lines = stringSplit(text, "\n");

    const std::string COLOR_NORMAL = textArea->getColor();
    for (int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];

        // insert a gray line number at start of each line
        int digits = static_cast<int>(std::to_string(lines.size()).length());
        std::string lineNumberString =
                padLeft(i == 0 ? std::string("") : std::to_string(i), digits) + "  ";
        textArea->appendFormattedText(lineNumberString, GWindow::chooseLightDarkModeColor(COLOR_LINE_NUMBERS, COLOR_LINE_NUMBERS_DARK_MODE));
        textArea->appendFormattedText(stringutils::toPrintable(line) + "\n", COLOR_NORMAL);
    }
}

void GDiffGui::syncScrollBars(bool left) {
    QTextEdit* qleft  = static_cast<QTextEdit*>(_textAreaLeft->getWidget());
    QTextEdit* qright = static_cast<QTextEdit*>(_textAreaRight->getWidget());
    if (left) {
        // adjust the right scrollbar to match the left one
        GThread::runOnQtGuiThreadAsync([qleft, qright]() {
            int lval = qleft->verticalScrollBar()->value();
            int rmax = qright->verticalScrollBar()->maximum();
            int rval = std::max(0, std::min(lval, rmax));
            qright->verticalScrollBar()->setValue(rval);
        });
    } else {
        // adjust the left scrollbar to match the right one
        GThread::runOnQtGuiThreadAsync([qleft, qright]() {
            int rval = qright->verticalScrollBar()->value();
            int lmax = qleft->verticalScrollBar()->maximum();
            int lval = std::max(0, std::min(rval, lmax));
            qleft->verticalScrollBar()->setValue(lval);
        });
    }
}
