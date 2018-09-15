/*
 * File: gdiffimage.cpp
 * --------------------
 * 
 * @author Marty Stepp
 * @version 2018/09/15
 * - initial version, converted from Java back-end DiffImage class
 */

// TODO: add checkbox for highlighting diffs in color
// TODO: free memory

#include "gdiffimage.h"
#include <iostream>
#include <string>
#include "gcolor.h"
#include "gfont.h"
#include "require.h"

/*static*/ const int GDiffImage::HIGHLIGHT_COLOR_DEFAULT = 0x8c1414;   // 140, 20, 20

/*static*/ void GDiffImage::showDialog(
        const std::string& name1,
        GCanvas* image1,
        const std::string& name2,
        GCanvas* image2) {
    // TODO: free memory
    new GDiffImage(name1, image1, name2, image2);
}

/*static*/ void GDiffImage::showDialog(
        const std::string& name1,
        const std::string& imageFile1,
        const std::string& name2,
        const std::string& imageFile2) {
    GCanvas* image1 = new GCanvas(imageFile1);
    GCanvas* image2 = new GCanvas(imageFile2);
    showDialog(name1, image1, name2, image2);
}

GDiffImage::GDiffImage(
        const std::string& name1,
        GCanvas* image1,
        const std::string& name2,
        GCanvas* image2) {
    require::nonNull(image1, "GDiffImage::constructor", "image1");
    require::nonNull(image2, "GDiffImage::constructor", "image2");

    _image1 = image1->toGImage();
    _image2 = image2->toGImage();
    int diffPixelsCount = image1->countDiffPixels(image2);
    _image2->setOpacity(0.5);

    // set up window and widgets
    _window = new GWindow(800, 600);
    _window->setTitle("Compare Graphical Output");

    _slider = new GSlider();
    _slider->setActionListener([this]() {
        _image2->setOpacity(_slider->getValue() / 100.0);
    });

    _highlightDiffsBox = new GCheckBox("Highlight diffs in color: ");
    _highlightDiffsBox->setActionListener([this]() {
        _slider->setEnabled(!_highlightDiffsBox->isChecked());
    });

    _colorButton = new GButton("X");
    _colorButton->setBackground(HIGHLIGHT_COLOR_DEFAULT);
    _colorButton->setForeground(HIGHLIGHT_COLOR_DEFAULT);

    _diffPixelsLabel = new GLabel("(" + integerToString(diffPixelsCount) + " pixels differ)");
    GFont::boldFont(_diffPixelsLabel);
    // TODO
    // setDiffPixelsText();

    _imageLabel1 = new GLabel(name1);
    _imageLabel2 = new GLabel(name2);
    _southPixelLabel = new GLabel("");

    // setupMenuBar();

    // do layout
    GContainer* south = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
    GContainer* south1 = new GContainer();
    south1->add(_imageLabel1);
    south1->add(_slider);
    south1->add(_imageLabel2);
    south->add(south1);
    // 20px strut?

    GContainer* south2 = new GContainer();
    south2->add(_diffPixelsLabel);
    south->add(south2);

    GContainer* south3 = new GContainer();
    south3->add(_southPixelLabel);
    south->add(south3);

    _window->addToRegion(south, GWindow::REGION_SOUTH);

    // poke the canvas
    double canvasWidth  = std::max(_image1->getWidth(),  _image2->getWidth());
    double canvasHeight = std::max(_image1->getHeight(), _image2->getHeight());
    _window->add(_image1);
    _window->add(_image2);

    _window->setMouseListener([this](GEvent event) {
        if (event.getType() != MOUSE_MOVED) {
            return;
        }
        int x = static_cast<int>(event.getX());
        int y = static_cast<int>(event.getY());
        _southPixelLabel->setText(
                "(x=" + integerToString(x)
                + ", y=" + integerToString(y)
                + ") expected: " + getPixelString(_image1, x, y)
                + " actual: " + getPixelString(_image2, x, y));
    });

    // set up events
    _window->setCanvasSize(canvasWidth, canvasHeight);
    _window->center();
    _window->show();
}

GDiffImage::~GDiffImage() {
    // TODO: delete
    _window = nullptr;
    _slider = nullptr;
    _highlightDiffsBox = nullptr;
    _colorButton = nullptr;
}

std::string GDiffImage::getPixelString(GImage* image, int x, int y) const {
    if (x >= 0 && x < image->getWidth()
            && y >= 0 && y < image->getHeight()) {
        int rgb = image->getPixel(x, y);
        return GColor::convertRGBToColor(rgb);
    } else {
        return "(none)";
    }
}
