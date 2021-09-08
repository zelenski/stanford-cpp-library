/*
 * File: gdiffimage.h
 * ------------------
 * 
 * @author Marty Stepp
 * @version 2018/10/12
 * - added "highlight diffs in color" checkbox and functionality
 * @version 2018/09/15
 * - initial version, converted from Java back-end DiffImage class
 */


#ifndef _gdiffimage_h
#define _gdiffimage_h

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
    static const std::string HIGHLIGHT_COLOR_DEFAULT;

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

    void chooseHighlightColor();
    void drawImages();
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
    GImage* _imageDiffs;
    std::string _highlightColor;

    friend class GImage;
};

#endif // _gdiffimage_h
