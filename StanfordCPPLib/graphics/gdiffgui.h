/*
 * File: gdiffgui.h
 * ----------------
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gdiffgui.h to replace Java version
 * @version 2018/07/31
 * - initial version
 */

#ifndef _gdiffgui_h
#define _gdiffgui_h

#include <string>
#include <QWidget>
#include <QSplitter>
#include "ginteractor.h"
#include "gtextarea.h"
#include "gwindow.h"

/**
 * A GDiffGui is a graphical window that displays differences between two
 * text strings.  This class is used to implement the "Compare Output" feature
 * of the graphical console, as well as various autograder comparisons.
 *
 * @private
 * TODO: document
 */
class GDiffGui {
public:
    static void showDialog(const std::string& name1,
                           const std::string& text1,
                           const std::string& name2,
                           const std::string& text2,
                           bool showCheckBoxes = false);

private:
    GDiffGui(const std::string& name1,
             const std::string& text1,
             const std::string& name2,
             const std::string& text2,
             bool showCheckBoxes = false);   // forbid construction
    virtual ~GDiffGui();

    Q_DISABLE_COPY(GDiffGui)

    GWindow* _window;
    QSplitter* _hsplitter;
    QSplitter* _vsplitter;
    GTextArea* _textAreaLeft;
    GTextArea* _textAreaRight;
    GTextArea* _textAreaBottom;
    GGenericInteractor<QSplitter>* _hsplitterInteractor;
    GGenericInteractor<QSplitter>* _vsplitterInteractor;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gdiffgui_h
