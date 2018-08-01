/*
 * File: qgdiffgui.h
 * -----------------
 * 
 * @author Marty Stepp
 * @version 2018/07/31
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgdiffgui_h
#define _qgdiffgui_h

#include <string>
#include <QWidget>
#include <QSplitter>
#include "qgtextarea.h"
#include "qgwindow.h"

class QGDiffGui {
public:
    static void showDialog(const std::string& name1,
                           const std::string& text1,
                           const std::string& name2,
                           const std::string& text2,
                           bool showCheckBoxes = false);

private:
    QGDiffGui(const std::string& name1,
              const std::string& text1,
              const std::string& name2,
              const std::string& text2,
              bool showCheckBoxes = false);   // forbid construction

    QGWindow* _window;
    QSplitter* _hsplitter;
    QSplitter* _vsplitter;
    QGTextArea* _textAreaLeft;
    QGTextArea* _textAreaRight;
    QGTextArea* _textAreaBottom;
    QGGenericInteractor<QSplitter>* _hsplitterInteractor;
    QGGenericInteractor<QSplitter>* _vsplitterInteractor;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgdiffgui_h
#endif // SPL_QT_GUI
