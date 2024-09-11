/*
 * File: welcome.cpp
 * -----------------
 * Sample program used to confirm Qt/CS106 install.
 * @author Julie Zelenski
 * #version 2024/09
 */

#include <iostream>
#include "console.h"
#include "queue.h"
#include "simpio.h"
#include "gbutton.h"
#include "glabel.h"
#include "goptionpane.h"
#include "gwindow.h"
#include "SimpleTest.h"
#include "sound.h"
#include "splversion.h"
#include "urlstream.h"
using namespace std;

string readUrl(string urlString)
{
    iurlstream ustream;
    ustream.open(urlString);
    return string(std::istreambuf_iterator(ustream), {});
}

void welcomeAlert(string name)
{
    GWindow* window = new GWindow;
    window->setTitle("Welcome");
    window->setLocation(300, 100);
    window->setExitOnClose(true);
    window->setResizable(false);
    window->setBackground("White");
    window->setCanvasSize(240, 220);
    GCanvas *canvas = window->getCanvas();
    canvas->clear();
    canvas->setColor("black");
    canvas->setFont("SansSerif-24");
    canvas->drawString("Welcome " + name + "!", 30, 150);
    canvas->drawImage("res/stanford.png", 60, 0);
    canvas->setColor("#008F00"); // green
    double x = 120, y = 25, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        canvas->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    GLabel *l = new GLabel("Installed Stanford library v " + getLibraryVersion());
    window->addToRegion(l, GWindow::REGION_NORTH);
    GButton *b = new GButton("Play sound clip");
    b->setActionListener([]() { Sound::playFile("res/AllRightNow.mp3"); });
    window->addToRegion(b, GWindow::REGION_SOUTH);
    window->addToRegion(b, GWindow::REGION_SOUTH);
    window->setVisible(true);
}

int main()
{
    Queue<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Copyright 2024 " << names << endl;
    string expected_version = readUrl("https://web.stanford.edu/dept/cs_edu/qt/version.txt"); // network access
    if (!expected_version.empty()) {
        cout << "Able to read from network. Expected library version is " << expected_version << endl;
    } else {
        cout << "Not able to read version from network." << endl;
    }
    string name = getLine("What is your name?");
    welcomeAlert(name);
    getLine("Ready to exit?");
    return 0;
}

// Confirm SimpleTest macros are available and compile
PROVIDED_TEST("SimpleTest of addition") {
    EXPECT_EQUAL(1 + 2, 3);
}
