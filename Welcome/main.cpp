/*
 * File: main.cpp
 * --------------
 * Sample program used to confirm Qt/CS106 install.
 * @author Julie Zelenski
 * #version 2020/09/12
 */

#include <iostream>
#include "console.h"
#include "queue.h"
#include "simpio.h"
#include "gwindow.h"
using namespace std;


void welcomeAlert(string name)
{
    GWindow* window = new GWindow(200, 200);
    window->setTitle("Fall Quarter 2020");
    window->setLocation(200, 200);
    window->setExitOnClose(true);
    window->setBackground("White");
    window->clear();
    window->setColor("black");
    window->drawString("Welcome " + name + "!", 50, 180);
    window->drawImage("res/stanford.png", 35, 20);
    window->setColor("#008F00");
    double x = 100, y = 45, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        window->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    window->setVisible(true);
}

int main()
{
    Queue<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Copyright 2020 " << names << endl;
    string name = getLine("What is your name?");
    welcomeAlert(name);
    return 0;
}

