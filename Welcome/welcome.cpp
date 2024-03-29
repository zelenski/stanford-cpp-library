/*
 * File: welcome.cpp
 * -----------------
 * Sample program used to confirm Qt/CS106 install.
 * @author Julie Zelenski
 * #version 2023/09
 */

#include <iostream>
#include "console.h"
#include "queue.h"
#include "simpio.h"
#include "gwindow.h"
#include "SimpleTest.h"
using namespace std;


void welcomeAlert(string name)
{
    GWindow* window = new GWindow(300, 200);
    window->setTitle("Fall Quarter 2023");
    window->setLocation(300, 100);
    window->setExitOnClose(true);
    window->setResizable(false);
    window->setBackground("White");
    window->clear();
    window->setColor("black");
    window->drawString("Welcome " + name + "!", 75, 175);
    window->drawImage("res/stanford.png", 75, 25);
    window->setColor("#008F00"); // green
    double x = 140, y = 50, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        window->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    window->setVisible(true);
}

int main()
{
    Queue<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Copyright 2023 " << names << endl;
    string name = getLine("What is your name?");
    welcomeAlert(name);
    return 0;
}

// Confirm SimpleTest macros are available and compile
PROVIDED_TEST("SimpleTest of addition") {
    EXPECT_EQUAL(1 + 2, 3);
}