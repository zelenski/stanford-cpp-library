#include <iostream>
#include "console.h"
#include "gridlocation.h"
#include "stack.h"
#include "gwindow.h"
using namespace std;


#if 0
#include "collections.h"

class Binky {
public:
    map<int, int> map;
};

bool make_error(Binky& set1, Binky& set2) {
    return set1.map < set2.map;
}

// collections.h:1764:31: error: expected greaterthan
void compiler_error()
{
    std::cout << "hello, world" << std::endl;
    Binky a, b;
    make_error(a, b);
}
#endif


void graphics_fixes()
{
    GWindow* window = new GWindow(200, 200);
    window->setTitle("Go Stanford!");
    window->setLocation(50, 50);
    window->setExitOnClose(true);
    window->setBackground("White");
    window->clear();
    window->setColor("black");
    window->drawString("Fall Quarter 2020", 50, 180);
    window->drawImage("res/stanford.png", 35, 20);
    window->setColor("#008F00");
    double x = 100, y = 45, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        window->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    window->setVisible(true);

   //     GPoitns of line should not reorder self
  //  Gline::setPoints(double x0, double y0, double x1, double y1)
}

void grid_location()
{
    GridLocation none;
   // GridLocation one = (1,2);
   // GridLocation one = 1;
    GridLocation one(1, 2);

    Stack<GridLocation> s = {none, one, {3, 4}, {5, 6}};
    Stack<GridLocation> t;

    ostringstream output;
    cout << "WAS " << s << endl;
    output << s << endl;
    istringstream input(output.str());
    bool flag = (input >> t) ? true : false;
    cout << "read succcess? " << flag << ", contents " << t << endl;

    t.clear();
    istringstream in("");
    flag = (in >> t) ? true : false;
    cout << "read empty success? " << flag << ", contents " << t << endl;

    GridLocation center(1, 1);
    cout << "Iterate over neighbors" << endl;
 //   for (GridLocation nbr : center.neighbors())
 //       cout << nbr << endl;
}

int studentMain()
{
    cout << "Hello, world!" << endl;

    graphics_fixes();
    grid_location();

    //timer not sure -- isStarted from stop?

    return 0;
}

