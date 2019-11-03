/*
 * File: gdiffimage.cpp
 * --------------------
 * 
 * @author Marty Stepp
 * @version 2019/04/23
 * - can press Escape key to close window
 * @version 2018/10/12
 * - added "highlight diffs in color" checkbox and functionality
 * @version 2018/09/15
 * - initial version, converted from Java back-end DiffImage class
 */

// TODO: free memory

#include "gdiffimage.h"
#include <iostream>
#include <string>
#include "gcolor.h"
#include "gcolorchooser.h"
#include "gfont.h"
#include "gspacer.h"
#include "gthread.h"
#include "require.h"

/*static*/ const std::string GDiffImage::HIGHLIGHT_COLOR_DEFAULT = "#e000e0";   // 224, 0, 224

/*static*/ void GDiffImage::showDialog(
        const std::string& name1,
        GCanvas* image1,
        const std::string& name2,
        GCanvas* image2) {
    // TODO: free memory on close
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
    _imageDiffs = nullptr;
    int diffPixelsCount = image1->countDiffPixels(image2);
    _image2->setOpacity(0.5);

    // set up window and widgets
    _window = new GWindow(800, 600);
    _window->setTitle("Compare Graphical Output");
    // _window->setResizable(false);
    _window->setAutoRepaint(false);

    // function to close the window when Escape is pressed
    // (similar to code in gdiffgui.cpp)
    auto windowCloseLambda = [this](GEvent event) {
        if (event.getType() == KEY_PRESSED && event.getKeyChar() == GEvent::ESCAPE_KEY) {
            _window->close();
        }
    };
    _window->setKeyListener(windowCloseLambda);

    _slider = new GSlider();
    _slider->setActionListener([this]() {
        _image2->setOpacity(_slider->getValue() / 100.0);
        drawImages();
    });
    _slider->setKeyListener(windowCloseLambda);

    _highlightDiffsBox = new GCheckBox("&Highlight diffs in color: ");
    _highlightDiffsBox->setActionListener([this]() {
        _slider->setEnabled(!_highlightDiffsBox->isChecked());
        drawImages();
    });
    _highlightDiffsBox->setKeyListener(windowCloseLambda);

    _highlightColor = HIGHLIGHT_COLOR_DEFAULT;
    _colorButton = new GButton("&X");
    _colorButton->setBackground(_highlightColor);
    _colorButton->setForeground(_highlightColor);
    _colorButton->setActionListener([this]() {
        chooseHighlightColor();
    });
    _colorButton->setKeyListener(windowCloseLambda);

    _diffPixelsLabel = new GLabel("(" + std::to_string(diffPixelsCount) + " pixels differ)");
    GFont::boldFont(_diffPixelsLabel);

    _imageLabel1 = new GLabel(name1);
    _imageLabel2 = new GLabel(name2);
    _southPixelLabel = new GLabel("");

    // TODO?
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
    south2->add(new GSpacer(20, 1));
    south2->add(_highlightDiffsBox);
    south2->add(_colorButton);
    south->add(south2);

    GContainer* south3 = new GContainer();
    south3->add(_southPixelLabel);
    south->add(south3);

    _window->addToRegion(south, GWindow::REGION_SOUTH);

    // poke the canvas
    double canvasWidth  = std::max(_image1->getWidth(),  _image2->getWidth());
    double canvasHeight = std::max(_image1->getHeight(), _image2->getHeight());
    // _window->add(_image1);
    // _window->add(_image2);

    _window->getCanvas()->setMouseListener([this](GEvent event) {
        if (event.getType() != MOUSE_MOVED) {
            return;
        }
        int x = static_cast<int>(event.getX());
        int y = static_cast<int>(event.getY());
        _southPixelLabel->setText(
                "(x=" + std::to_string(x)
                + ", y=" + std::to_string(y)
                + ") expected: " + getPixelString(_image1, x, y)
                + " actual: " + getPixelString(_image2, x, y));
    });

    // set up events
    _window->setCanvasSize(canvasWidth, canvasHeight);
    _window->drawPixel(0, 0, 0xffffff);
    drawImages();

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

void GDiffImage::chooseHighlightColor() {
    std::string color = GColorChooser::showDialog(
            /* parent */ _window,
            /* title */ "Choose a highlight color",
            /* initial color */ _highlightColor);
    if (color.empty()) {
        return;
    }
    _highlightColor = color;
    _colorButton->setBackground(color);
    _colorButton->setForeground(color);
    drawImages();
}

void GDiffImage::drawImages() {
    _window->clearCanvas();
    if (_highlightDiffsBox->isChecked()) {
        GThread::runOnQtGuiThreadAsync([this]() {
            // draw the highlighted diffs (if so desired)
            int w1 = _image1->getWidth();
            int h1 = _image1->getHeight();
            int w2 = _image2->getWidth();
            int h2 = _image2->getHeight();

            int wmax = std::max(w1, w2);
            int hmax = std::max(h1, h2);

            if (!_imageDiffs) {
                _imageDiffs = new GImage(wmax, hmax);
            }
            QImage* imgDiff = _imageDiffs->getQImage();

            // check each pair of pixels
            // (access raw QImages for speed)
            int highlightColor = GColor::convertColorToRGB(_highlightColor) | 0xff000000;
            QImage* img1 = _image1->getQImage();
            QImage* img2 = _image2->getQImage();
            for (int y = 0; y < hmax; y++) {
                for (int x = 0; x < wmax; x++) {
                    int pixel1 = (x < w1 && y < h1) ? (img1->pixel(x, y) & 0xffffffff) : 0;
                    int pixel2 = (x < w2 && y < h2) ? (img2->pixel(x, y) & 0xffffffff) : 0;
                    imgDiff->setPixel(x, y, (pixel1 == pixel2) ? pixel1 : highlightColor);
                }
            }
            _window->draw(_imageDiffs);
        });
    } else {
        _window->draw(_image1);
        _window->draw(_image2);   // possibly at sub-1 opacity
    }

    _window->repaint();
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
