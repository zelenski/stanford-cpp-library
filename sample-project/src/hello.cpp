/*
 * CS 106B/X Sample Project
 * last updated: 2018/09/19 by Marty Stepp
 *
 * This project helps test that your Qt Creator system is installed correctly.
 * Compile and run this program to see a console and a graphical window.
 * If you see these windows, your Qt Creator is installed correctly.
 */

#include <iostream>
#include "console.h"
#include "ginteractors.h" // for GWindow
#include "gwindow.h" // for GWindow
#include "simpio.h"  // for getLine
#include "vector.h"  // for Vector
using namespace std;

int main() {
    cout << "This sample project helps you check and make sure that" << endl;
    cout << "you have successfully installed Qt Creator, C++ compiler/libs," << endl;
    cout << "and that the Stanford C++ libraries work on your machine." << endl;
    cout << endl;
    cout << "If this project is working properly on your computer," << endl;
    cout << "You should see a \"Sample Project\" window with a" << endl;
    cout << "smiley face pop up when you build and run it." << endl;
    cout << endl;
    cout << "If it doesn't run, please double-check the Qt Creator" << endl;
    cout << "installation instructions linked from the class web site." << endl;
    cout << endl;

    GWindow* window = new GWindow(300, 200);
    window->setTitle("Sample Project");
    window->setLocation(50, 50);
    window->setExitOnClose(true);
    window->setBackground("White");
    window->clear();
    window->setColor("Black");
    window->drawOval( 10, 10, 180, 180);   // head
    window->setColor("blue");
    window->fillOval( 50, 50,  20,  20);   // eye
    window->fillOval(130, 50,  20,  20);   // eye
    window->setColor("red");
    window->fillOval( 95, 95,  10,  10);   // nose
    window->setColor("black");
    window->drawString("Go Stanford!", 180, 30);

    // draw the mouth using a Vector to make sure the Stanford collections work
    Vector<GPoint> mouth;
    mouth.add(GPoint( 40, 120));
    mouth.add(GPoint( 70, 150));
    mouth.add(GPoint(130, 150));
    mouth.add(GPoint(160, 120));
    for (int i = 0; i < mouth.size() - 1; i++) {
        GPoint p1 = mouth.get(i);
        GPoint p2 = mouth.get(i + 1);
        window->drawLine(p1, p2);   // mouth
    }

    window->setVisible(true);
    window->requestFocus();

    return 0;
}
