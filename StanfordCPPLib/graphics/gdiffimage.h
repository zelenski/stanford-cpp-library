/*
 * File: gdiffimage.h
 * ------------------
 * 
 * @author Marty Stepp
 * @version 2018/09/15
 * - initial version, converted from Java back-end DiffImage class
 */

#ifndef _gdiffimage_h
#define _gdiffimage_h

// signal that GUI system is in use (so it will be initialized)
#ifndef INTERNAL_INCLUDE
#define SPL_QT_GUI_IN_USE 1
#endif // INTERNAL_INCLUDE

#include <string>
#include <QWidget>
#include <QSplitter>
#include "gbutton.h"
#include "gcanvas.h"
#include "gcheckbox.h"
#include "ginteractor.h"
#include "glabel.h"
#include "gslider.h"
#include "gwindow.h"

/**
 * A GDiffImage is a graphical window that displays differences between two
 * images.  This class is used to implement the "Compare Graphical Output"
 * feature of graphical programs, as well as various autograder comparisons.
 *
 * @private
 * TODO: document
 */
class GDiffImage {
public:
    static const int HIGHLIGHT_COLOR_DEFAULT;

    static void showDialog(const std::string& name1,
                           GCanvas* image1,
                           const std::string& name2,
                           GCanvas* image2);

    static void showDialog(const std::string& name1,
                           const std::string& imageFile1,
                           const std::string& name2,
                           const std::string& imageFile2);

private:
    GDiffImage(const std::string& name1,
               GCanvas* image1,
               const std::string& name2,
               GCanvas* image2);   // forbid construction
    virtual ~GDiffImage();

    Q_DISABLE_COPY(GDiffImage)

    std::string getPixelString(GImage* image, int x, int y) const;

    GWindow* _window;
    GSlider* _slider;
    GCheckBox* _highlightDiffsBox;
    GButton* _colorButton;
    GLabel* _diffPixelsLabel;
    GLabel* _imageLabel1;
    GLabel* _imageLabel2;
    GLabel* _southPixelLabel;
    GImage* _image1;
    GImage* _image2;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gdiffimage_h

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE
