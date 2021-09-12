#include <iostream>
#include "console.h"
#include "gridlocation.h"
#include "stack.h"
#include "gwindow.h"
using namespace std;


void graphics_fixes()
{
    GWindow* window = new GWindow(200, 200);
    window->setTitle("Bug test");
    window->setLocation(50, 50);
    window->setBackground("White");
    window->clear();
    window->setColor("#008F00");
    // fix behavior with fill polygon
    double x = 100, y = 45, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        window->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    window->setVisible(true);
}

void gridlocation_fixes()
{
    // GridLocation used to not be able to properly read from a stream
    // It also had one-arg constructor that was bonus
    GridLocation none;
   // GridLocation one = (1,2);
   // GridLocation one = 1;
    GridLocation one(1, 2);

    Stack<GridLocation> s = {none, one, {3, 4}, {5, 6}};
    Stack<GridLocation> t;

    {
        ostringstream output;
        output << s << endl;
        istringstream input(output.str());
        bool flag = (input >> t) ? true : false;
        cout << "read succcess? " << flag << ", same contents? " << (s == t) << endl;
    }
    {
        Stack<GridLocation> empty;
        ostringstream output;
        output << empty << endl;
        istringstream input(output.str());
        bool flag = (input >> t) ? true : false;
        cout << "read succcess? " << flag << ", same contents? " << (empty == t) << endl;
    }
    {
        Stack<GridLocation> bad;
        istringstream input("{r1c2,r2c3,rc,r6c7");
        bool flag = (input >> bad) ? true : false;
        cout << "read succcess? " << flag << ", stack contains " << bad << endl;
    }

}

int main()
{
    graphics_fixes();
    gridlocation_fixes();
    return 0;
}

