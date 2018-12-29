/*
 * Test file for verifying the Stanford C++ lib GUI functionality.
 */

#include "testcases.h"
#include "filelib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "gfilechooser.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "goptionpane.h"
#include "gtable.h"
#include "gtypes.h"
#include "gwindow.h"
#include "simpio.h"

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

void fileDialogTest() {
    // std::cout << "current dir should be \"" << getCurrentDirectory() << "\"" << std::endl;
    std::ifstream input;
    std::string filename = openFileDialog(input, "booyahtitle", "/usr/lib/eclipse");
    std::cout << "you chose \"" << filename << "\"" << std::endl;
}

void goptionpaneTest() {
    GOptionPane::showMessageDialog("Hello, world");
    GOptionPane::showMessageDialog("Here's one with title and type", "booyah", GOptionPane::MESSAGE_ERROR);
    GOptionPane::showMessageDialog("special chars [*+*&}{] && || !)@(*&#)(&^%!{ \" \" \" \"}) C:\\Program Files\\Test ;,;:\", ';\"\\//\\// ( ) test 1 2 3 $a $b %a %b %1 %2 http://foo.com/! end");
    
    bool result = GOptionPane::showConfirmDialog("Are you sure?");
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    result = GOptionPane::showConfirmDialog("Here's one with title and type", "booyah", GOptionPane::CONFIRM_OK_CANCEL);
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    result = GOptionPane::showConfirmDialog("Here's a second one with title and type", "booyah", GOptionPane::CONFIRM_YES_NO_CANCEL);
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    
    std::string input = GOptionPane::showInputDialog("Type something:");
    std::cout << "you typed: \"" << input << "\"" << std::endl;
    input = GOptionPane::showInputDialog("Here's one with a title", "booyah");
    std::cout << "you typed: \"" << input << "\"" << std::endl;
    
    Vector<std::string> choices;
    choices += "a", "bb", "ccc", "dd";
    std::string option = GOptionPane::showOptionDialog("choose!", choices, "booyah", "ccc");
    std::cout << "you chose: \"" << option << "\"" << std::endl;
}

static void border(GBufferedImage* img) {
    int color = 0x0;
    int w = img->getWidth();
    int h = img->getHeight();
    // top/bottom
    for (int x = 0; x < w; x++) {
        img->setRGB(x, 0, color);
        img->setRGB(x, h-1, color);
    }
    // left/right
    for (int y = 0; y < h; y++) {
        img->setRGB(0, y, color);
        img->setRGB(w-1, y, color);
    }
}

void gbufferedImageTest() {
    GWindow* gw = new GWindow;
    gw->setSize(900, 700);
    gw->setTitle("Test");

    GButton* button1 = new GButton("Click Me 1");
    gw->add(button1, 250, 80);
    //GButton* button2 = new GButton("Click Me 2");
    //gw->addToRegion(button2, "NORTH");

    GLabel* label = new GLabel("test!");
    gw->add(label, 10, 60);
    
    std::cout << "About to construct GBufferedImage." << std::endl;
    GBufferedImage* img = new GBufferedImage(200, 250);
    img->setLocation(10, 80);
    std::cout << "Done constructing GBufferedImage." << std::endl;
    gw->add(img, 50, 50);
    // gw->addToRegion(img, "SOUTH");
    gw->setVisible(true);
    
//    GBufferedImage* img2 = new GBufferedImage(20, 20);
//    img2->fill(GBufferedImage::createRgbPixel(255, 0, 255));
//    Grid<int> grid = img2->toGrid();
//    cout << "grid of pixels before: " << grid << endl;
//    for (int y = 4; y <= 18; y++) {
//        for (int x = 2; x <= 9; x++) {
//            grid[y][x] = GBufferedImage::createRgbPixel(0, 255, 0);
//        }
//    }
//    cout << "grid of pixels after: " << grid << endl;
//    img2->fromGrid(grid);
//    gw->add(img2, 350, 20);
    
    GBufferedImage* img3 = new GBufferedImage();
    img3->load("rainbow.png");
    cout << "adding the image!" << endl;
    gw->add(img3, 10, 20);
    // pause(2000);
    
    cout << "start toGrid" << endl;
    Grid<int> grid3 = img3->toGrid();
    cout << "end toGrid, start rgb shit" << endl;
    for (int y = 0; y < grid3.height(); y++) {
        for (int x = 0; x < grid3.width(); x++) {
            int red, green, blue;
            GBufferedImage::getRedGreenBlue(grid3[y][x], red, green, blue);
            grid3[y][x] = GBufferedImage::createRgbPixel(green, red, blue);
        }
    }
    cout << "end rgb shit, start fromGrid" << endl;
    img3->fromGrid(grid3);
    cout << "end fromGrid" << endl;
    
    pause(2000);

    // fill
    img->fill(0xff00ff);  // purple

    std::cout << "About to setRGB on GBufferedImage." << std::endl;
    for (int y = 2; y < img->getHeight() - 2; y++) {
        for (int x = 5; x <= img->getWidth() - 5; x++) {
            img->setRGB(x, y, 0xffcc33);
        }
    }
    std::cout << "Done setting RGB on GBufferedImage." << std::endl;
    border(img);
    pause(500);
    
    std::cout << "About to resize on GBufferedImage." << std::endl;
    img->resize(100, 50);
    border(img);
    pause(500);
    
    std::cout << "About to resize on GBufferedImage." << std::endl;
    img->resize(200, 80);
    border(img);
    pause(500);
    
    std::cout << "About to setRGB on GBufferedImage." << std::endl;
    for (int y = 10; y < img->getHeight() - 10; y++) {
        for (int x = 10; x <= img->getWidth() - 10; x++) {
            img->setRGB(x, y, 0xff33cc);
        }
    }
    border(img);
    
    std::cout << "About to remove other shit." << std::endl;
    pause(200);
    gw->remove(label);
    gw->remove(button1);
    //gw->removeFromRegion(button2, "NORTH");
    pause(200);
    
    std::cout << "About to remove GBufferedImage." << std::endl;
    pause(200);
    gw->remove(img);
    // gw->removeFromRegion(img, "SOUTH");
    pause(200);
    std::cout << "Test complete." << std::endl;
    std::cout << std::endl;
}

void gtableTest() {
    GWindow* gw = new GWindow;
    gw->setTitle("GTable Test");
    
    GTable* table = new GTable(5, 3);
    table->select(0, 1);
    table->setTableListener([](GTableEvent tableEvent) {
        std::cout << "cell updated: " << tableEvent.toString() << std::endl;
    });
    gw->addToRegion(table, "NORTH");
    
    GButton* buttget = new GButton("Get All");
    buttget->setActionListener([=]() {
        for (int row = 0; row < table->numRows(); row++) {
            for (int col = 0; col < table->numCols(); col++) {
                cout << "R" << row << "C" << col << "=\"" << table->get(row, col) << "\" ";
            }
            cout << endl;
        }
        int row, col;
        table->getSelectedCell(row, col);
        cout << "selected: R" << row << "C" << col << endl;
    });
    gw->addToRegion(buttget, "SOUTH");
    
    GButton* buttset = new GButton("Set All");
    buttset->setActionListener([=]() {
        for (int row = 0; row < table->numRows(); row++) {
            for (int col = 0; col < table->numCols(); col++) {
                std::string value = "R" + std::to_string(row) + "C" + std::to_string(col);
                table->set(row, col, value);
            }
        }
        table->select(1, 2);
    });
    gw->addToRegion(buttset, "SOUTH");
    
    GButton* buttclear = new GButton("Clear");
    buttclear->setActionListener([=]() {
        table->clear();
    });
    gw->addToRegion(buttclear, "SOUTH");
    
    GButton* buttrowadd = new GButton("+R");
    buttrowadd->setActionListener([=]() {
        table->resize(table->numRows() + 1, table->numCols());
    });
    gw->addToRegion(buttrowadd, "SOUTH");
    
    GButton* buttrowrem = new GButton("-R");
    buttrowrem->setActionListener([=]() {
        table->resize(table->numRows() - 1, table->numCols());
    });
    gw->addToRegion(buttrowrem, "SOUTH");
    
    GButton* buttcoladd = new GButton("+C");
    buttcoladd->setActionListener([=]() {
        table->resize(table->numRows(), table->numCols() + 1);
    });
    gw->addToRegion(buttcoladd, "SOUTH");
    
    GButton* buttcolrem = new GButton("-C");
    buttcolrem->setActionListener([=]() {
        table->resize(table->numRows(), table->numCols() - 1);
    });
    gw->addToRegion(buttcolrem, "SOUTH");
    
    GButton* buttwidthadd = new GButton("+W");
    buttwidthadd->setActionListener([=]() {
        table->setColumnWidth(1, table->getColumnWidth(1) + 20);
    });
    gw->addToRegion(buttwidthadd, "SOUTH");
    
    GButton* buttwidthrem = new GButton("-W");
    buttwidthrem->setActionListener([=]() {
        table->setColumnWidth(1, table->getColumnWidth(1) - 20);
    });
    gw->addToRegion(buttwidthrem, "SOUTH");
    
    gw->setVisible(true);
}

void radioButtonTest() {
    GWindow* gw = new GWindow;
    gw->setTitle("Radio Button Test");
    GRadioButton* rb1 = new GRadioButton("Red");
    GRadioButton* rb2 = new GRadioButton("Green", "", true);
    GRadioButton* rb3 = new GRadioButton("Blue");
    gw->addToRegion(rb1, "SOUTH");
    gw->addToRegion(rb2, "SOUTH");
    gw->addToRegion(rb3, "SOUTH");

    // a second group of buttons
    GRadioButton* rb4 = new GRadioButton("Black", "N", true);
    GRadioButton* rb5 = new GRadioButton("White", "N");
    gw->addToRegion(rb4, "NORTH");
    gw->addToRegion(rb5, "NORTH");
    
    gw->setVisible(true);
}
