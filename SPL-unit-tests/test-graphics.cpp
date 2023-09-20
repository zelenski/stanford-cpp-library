/*
 * Test file for verifying the Stanford C++ lib GUI functionality.
 */


#include "filelib.h"
#include "gcanvas.h"
#include "gevent.h"
#include "goptionpane.h"
#include "gtable.h"
#include "gwindow.h"
#include "gbutton.h"
#include "glabel.h"
#include "gradiobutton.h"

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "SimpleTest.h"
using namespace std;

MANUAL_TEST("chooseFilenameDialog") {
    std::cout << "Opening file chooser in directory \"" << getCurrentDirectory() << "\"" << std::endl;
    std::string filename = chooseFilenameDialog("booyahtitle", "", "*.cpp");
    std::cout << "You selected file:  \"" << filename << "\"" << std::endl;
}

MANUAL_TEST("GOptionPane messages, alerts, and confirm dialogs") {
    GOptionPane::showMessageDialog("Just saying hi");
    GOptionPane::showMessageDialog("This would be an error alert", "Oh no", GOptionPane::MESSAGE_ERROR);
    GOptionPane::showMessageDialog("Testing special chars [*+*&}{] && || !)@(*&#)(&^%!{ \" \" \" \"}) C:\\Program Files\\Test ;,;:\", ';\"\\//\\// ( ) test 1 2 3 $a $b %a %b %1 %2 http://foo.com/! end");

    auto result = GOptionPane::showConfirmDialog("Are you sure? (say Yes)");
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_YES);
    result = GOptionPane::showConfirmDialog("All is cool? (say OK)", "I have a title", GOptionPane::CONFIRM_OK_CANCEL);
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_OK);
    result = GOptionPane::showConfirmDialog("This one allows Cancel (do that)", "I have a title", GOptionPane::CONFIRM_YES_NO_CANCEL);
    EXPECT_EQUAL(result, GOptionPane::CONFIRM_CANCEL);
}

MANUAL_TEST("GOptionPane input dialogs") {
    std::string input = GOptionPane::showInputDialog("Enter b in text field:", "I have a title");
    EXPECT_EQUAL(input, "b");
    input = GOptionPane::showInputDialog("Enter Karel in text field:", "I have a title", "Karel");
    EXPECT_EQUAL(input, "Karel");
}

MANUAL_TEST("GOptionPane option dialogs") {
    Vector<std::string> choices = {"Apple", "Banana", "Cherry", "Default"};
    std::string option = GOptionPane::showOptionDialog("Please pick Banana", choices, "I have a title", "Default");
    EXPECT_EQUAL(option, "Banana");
}

MANUAL_TEST("GRadioButton to select from option group") {
    GWindow* gw = new GWindow;
    GLabel* label = new GLabel("Hello, world!");
    label->setColor("#000000");
    label->setBackground("#ffffff");
    label->setFont("Helvetica-64-Bold");
    gw->addToRegion(label, "CENTER");

    gw->setTitle("Radio Button (close window to end test)");
    GRadioButton* rb1 = new GRadioButton("Red", "color");
    rb1->setActionListener([label](GEvent) { label->setBackground("#ff0000");});
    GRadioButton* rb2 = new GRadioButton("Green", "color", true);
    rb2->setActionListener([label](GEvent) { label->setBackground("#0000ff");});
    GRadioButton* rb3 = new GRadioButton("Blue", "color");
    rb3->setActionListener([label](GEvent) { label->setBackground("#00ff00");});
    gw->addToRegion(rb1, "SOUTH");
    gw->addToRegion(rb2, "SOUTH");
    gw->addToRegion(rb3, "SOUTH");

    // a second group of buttons
    GRadioButton* rb4 = new GRadioButton("Black", "N", true);
    rb4->setActionListener([label](GEvent) { label->setColor("#0");});
    GRadioButton* rb5 = new GRadioButton("White", "N");
    rb5->setActionListener([label](GEvent) { label->setColor("#ffffff");});
    gw->addToRegion(rb4, "NORTH");
    gw->addToRegion(rb5, "NORTH");

    gw->setVisible(true);
    while (gw->isVisible()) {}
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

MANUAL_TEST("GCanvas with image") {
    GWindow* gw = new GWindow;
    gw->setSize(300, 300);
    gw->setTitle("Testing GCanvas...");
    gw->setCloseOperation(GWindow::CLOSE_DO_NOTHING); // disallow close while test is running

    GLabel* label = new GLabel("Label");
    gw->addToRegion(label, "SOUTH");

    label->setText("Display image Stanford S");
    GCanvas* img = new GCanvas();
    img->load("res/stanford.png");
    gw->add(img, 10, 20);
    pause(2000);

    label->setText("Retrieve pixels via toGrid, compare to individual pixel retrieved one by one");
    Grid<int> grid = img->toGrid();
    for (int y = 0; y < grid.numRows(); y++) {
        for (int x = 0; x < grid.numCols(); x++) {
            EXPECT_EQUAL(img->getARGB(x, y), grid[y][x]);
        }
    }
    label->setText("Replace image with pixel copy, should not change");
    img->fromGrid(grid);
    pause(1000);

    label->setText("Resize, fill magenta");
    img->resize(120, 120);
    img->fill(0xff00ff);  // magenta
    border(img);

    label->setText("setRGB on inner pixels (one at a time, this is slow!)");
    for (int y = 0; y < img->getHeight()/2; y++) {
        for (int x = 0; x < img->getWidth()/2; x++) {
            img->setRGB(x + img->getWidth()/4, y + img->getHeight()/4, 0xffcc33);
        }
    }
    pause(500);

    label->setText("Resize, fill red");
    img->resize(150, 300);
    img->fill(0xff0000);
    pause(500);

    label->setText("Use setGrid to change inner pixels (fast)");
    grid = img->toGrid();

    for (int y = 0; y < img->getHeight()/2; y++) {
        for (int x = 0; x < img->getWidth()/2; x++) {
            grid[y + img->getHeight()/4][x + img->getWidth()/4] = 0xffcc33;
        }
    }

    img->setPixels(grid);
    pause(1000);
    gw->setCloseOperation(GWindow::CLOSE_DISPOSE);
    label->setText("Done, close window to exit this test");
    while (gw->isVisible()) {}
}

MANUAL_TEST("GTable with editing controls") {
    GWindow* gw = new GWindow;
    gw->setTitle("Testing GTable (close window when finished)");

    GTable* table = new GTable(5, 3);
    table->select(0, 1);
    table->setTableListener([](GTableEvent tableEvent) {
        //std::cout << "cell updated: " << tableEvent.toString() << std::endl;
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
    while (gw->isVisible()) {}
}
