/*
 * File: gdiffgui.h
 * ----------------
 * 
 * @author Marty Stepp
 * @version 2018/10/06
 * - allow passing diff flags
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

#include "diff.h"
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
    static const std::string COLOR_EXPECTED;
    static const std::string COLOR_EXPECTED_DARK_MODE;
    static const std::string COLOR_LINE_NUMBERS;
    static const std::string COLOR_LINE_NUMBERS_DARK_MODE;
    static const std::string COLOR_STUDENT;
    static const std::string COLOR_STUDENT_DARK_MODE;

    /**
     * Pops up a diff GUI window to display
     */
    static void showDialog(const std::string& name1,
                           const std::string& text1,
                           const std::string& name2,
                           const std::string& text2,
                           int diffFlags = diff::DIFF_DEFAULT_FLAGS,
                           bool showCheckBoxes = false);

private:
    static const bool LINE_NUMBERS;

    GDiffGui(const std::string& name1,
             const std::string& text1,
             const std::string& name2,
             const std::string& text2,
             int diffFlags = diff::DIFF_DEFAULT_FLAGS,
             bool showCheckBoxes = false);   // forbid construction
    virtual ~GDiffGui();

    Q_DISABLE_COPY(GDiffGui)

    void setupDiffText(const std::string& diffs);
    void setupLeftRightText(GTextArea* textArea, const std::string& text);
    void syncScrollBars(bool left);

    GWindow* _window;
    QSplitter* _hsplitter;
    QSplitter* _vsplitter;
    GTextArea* _textAreaLeft;
    GTextArea* _textAreaRight;
    GTextArea* _textAreaBottom;
    GGenericInteractor<QSplitter>* _hsplitterInteractor;
    GGenericInteractor<QSplitter>* _vsplitterInteractor;
};

#endif // _gdiffgui_h
