/*
 * Test file for verifying the Stanford C++ lib GUI functionality.
 */


#include "filelib.h"
#include "gcanvas.h"
#include "gevent.h"
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
#include "testing/SimpleTest.h"
using namespace std;

PROVIDED_TEST("File Dialog") {
    // std::cout << "current dir should be \"" << getCurrentDirectory() << "\"" << std::endl;
    std::ifstream input;
    std::string filename = openFileDialog(input, "booyahtitle", "/usr/lib/eclipse");
    std::cout << "you chose \"" << filename << "\"" << std::endl;
}

PROVIDED_TEST("GOptionPane alerts, dialogs, messages") {
    GOptionPane::showMessageDialog("Just saying hi");
    GOptionPane::showMessageDialog("This would be an error alert", "Oh no", GOptionPane::MESSAGE_ERROR);
    GOptionPane::showMessageDialog("Testing special chars [*+*&}{] && || !)@(*&#)(&^%!{ \" \" \" \"}) C:\\Program Files\\Test ;,;:\", ';\"\\//\\// ( ) test 1 2 3 $a $b %a %b %1 %2 http://foo.com/! end");

    bool result = GOptionPane::showConfirmDialog("Are you sure? (say Yes)");
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_YES);
    result = GOptionPane::showConfirmDialog("All is cool? (say OK)", "I have a title", GOptionPane::CONFIRM_OK_CANCEL);
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_OK);
    result = GOptionPane::showConfirmDialog("This one allows Cancel (do that)", "booyah", GOptionPane::CONFIRM_YES_NO_CANCEL);
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_CANCEL);

    std::string input = GOptionPane::showInputDialog("Enter boo in text field:");
    EXPECT_EQUAL(input, "boo");
    input = GOptionPane::showInputDialog("Enter boo in text field", "I have a title");
    EXPECT_EQUAL(input, "boo");

    Vector<std::string> choices;
    choices += "a", "bb", "ccc", "dd";
    std::string option = GOptionPane::showOptionDialog("Please pick choice bb", choices, "booyah", "ccc");
    EXPECT_EQUAL(option, "bb");
}

PROVIDED_TEST("Gtable in window") {
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

PROVIDED_TEST("Radio buttons") {
    GWindow* gw = new GWindow;
    gw->setTitle("Radio Button Test");
    GRadioButton* rb1 = new GRadioButton("Red", "color");
    GRadioButton* rb2 = new GRadioButton("Green", "color", true);
    GRadioButton* rb3 = new GRadioButton("Blue", "color");
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


static void border(GCanvas* img) {
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

PROVIDED_TEST("Canvas with image") {
    GWindow* gw = new GWindow;
    gw->setSize(300, 300);
    gw->setTitle("Canvas Test");

    GButton* button1 = new GButton("Click Me 1");
    gw->addToRegion(button1, "NORTH");
    GButton* button2 = new GButton("Click Me 2");
    gw->addToRegion(button2, "NORTH");

    GLabel* label = new GLabel("this is a label");
    gw->addToRegion(label, "SOUTH");

    cout << "Show canvas with image of Stanford S" << endl;
    GCanvas* img = new GCanvas();
    img->load("res/stanford.png");
    gw->add(img, 10, 20);
    pause(2000);

    cout << "Retrieve pixels via toGrid, compare to individual pixel retrieved one by one" << endl;
    Grid<int> grid = img->toGrid();
    for (int y = 0; y < grid.numRows(); y++) {
        for (int x = 0; x < grid.numCols(); x++) {
            EXPECT_EQUAL(img->getARGB(x, y), grid[y][x]);
        }
    }
    cout << "Now replace image with its pixel copy, should not change" << endl;
    img->fromGrid(grid);
    pause(1000);

    std::cout << "Will resize GCanvas to 100x50, fill with magenta" << std::endl;
    img->fill(0xff00ff);  // purple
    img->resize(100, 50);
    border(img);

    std::cout << "Now setRGB on inner pixels (one at a time, this is slow!)" << std::endl;
    for (int y = 5; y < img->getHeight() - 5; y++) {
        for (int x = 5; x <= img->getWidth() - 5; x++) {
            img->setRGB(x, y, 0xffcc33);
        }
    }
    std::cout << "Done setting RGB on GCanvas pixels." << std::endl;
    border(img);
    pause(100);

    std::cout << "Will resize GCanvas to 80x200, fill red" << std::endl;
    img->resize(80, 200);
    img->fill(0xff0000);
    border(img);
    pause(300);

    std::cout << "Now setGrid to change on inner pixels" << std::endl;
    grid = img->toGrid();

    for (int y = 5; y < img->getHeight() - 5; y++) {
        for (int x = 5; x <= img->getWidth() - 5; x++) {
            grid[y][x] = 0xffcc33;
        }
    }
    img->setPixels(grid);
    border(img);
    pause(1000);

    std::cout << "About to remove others" << std::endl;
    gw->remove(label);
    gw->remove(button1);
    gw->remove(button2);
    pause(300);

    std::cout << "About to remove GCanvas." << std::endl;
    gw->remove(img);
    std::cout << "Test complete." << std::endl;
    std::cout << std::endl;
}


