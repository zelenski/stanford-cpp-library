/*
 * File: welcome.cpp
 * -----------------
 * Sample program used to confirm Qt/CS106 install.
 * @author Julie Zelenski
 */

#include <iostream>
#include "console.h"
#include "queue.h"
#include "simpio.h"
#include "gbutton.h"
#include "gfont.h"
#include "glabel.h"
#include "goptionpane.h"
#include "gwindow.h"
#include "SimpleTest.h"
#include "sound.h"
#include "splversion.h"
#include "urlstream.h"
using namespace std;

void welcomeAlert(string name)
{
    GWindow* window = new GWindow;
    window->setTitle("Welcome");
    window->setLocation(300, 100);
    window->setExitOnClose(true);
    window->setResizable(false);
    window->setCanvasSize(240, 220);
    GCanvas *canvas = window->getCanvas();
    canvas->setBackground("white");
    canvas->clear();
    canvas->setColor("black");
    QFont f = GFont::toQFont("SansSerif-24");
    canvas->setFont(f);
    string message = "Hello, " + name + "!";
    int m_width = QFontMetrics(f).horizontalAdvance(QString::fromStdString(message));
    canvas->drawString(message, (canvas->getWidth()-m_width)/2.0, 150);
    canvas->drawImage("res/stanford.png", 60, 0);
    canvas->setColor("#008F00"); // green
    double x = 120, y = 25, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        canvas->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    GLabel *l = new GLabel("Installed Stanford library " + getLibraryVersion());
    window->addToRegion(l, GWindow::REGION_NORTH);
    GButton *b = new GButton("Play sound clip");
    b->setActionListener([]() { Sound::playFile("res/AllRightNow.wav"); });
    window->addToRegion(b, GWindow::REGION_SOUTH);
    window->setVisible(true);
}

void checkLibraryVersion()
{
    // Confirm installed library matches version of our Qt install instructions
    // This also confirms availability of network functionality
    string url = "https://web.stanford.edu/dept/cs_edu/qt/version.txt";
    iurlstream ustream;
    ustream.open(url);
    string from_csedu;
    getline(ustream, from_csedu);
    string installed_version = getLibraryVersion();
    cout << "Installed version of library is " << installed_version;

    if (from_csedu.empty()) {
        cerr << " (unable to read version spec from network to compare)" << endl;
    } else if (from_csedu != installed_version) {
        cerr << " (version spec published on cs_edu is " << from_csedu << ")" << endl;
    } else {
        cout << " (matches version spec from cs_edu)" << endl;
    }
}

int main()
{
    Queue<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Copyright 2025-26 " << names << endl;
    checkLibraryVersion();
    string name = getLine("What is your name?");
    welcomeAlert(name);
    cout << "All done!" << endl;
    return 0;
}

// Confirm SimpleTest macros are available and compile
PROVIDED_TEST("SimpleTest of addition") {
    EXPECT_EQUAL(1 + 2, 3);
}
