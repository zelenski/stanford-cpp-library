///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include "gcolor.h"
//#include "gobjects.h"
//#include "ginteractors.h"
//using namespace std;

//void testQlayout();

//int mainQtLayout() {
//    testQlayout();
//    return 0;
//}

//void testQlayout() {
//    static GWindow* window = new GWindow(700, 500);
//    window->setTitle("QtLayout Window");
//    window->setBackground("yellow");
//    window->setResizable(true);
//    window->setExitOnClose(true);
//    window->center();

//    // northern area of overall window tests vertical flow layout
//    GContainer* north = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
//    north->setBackground("red");
//    GButton* button1 = new GButton("Button 1");
//    north->add(button1);
//    GButton* button2 = new GButton("Button Number 2");
//    north->add(button2);
//    GButton* button3 = new GButton("B3");
//    north->add(button3);
//    north->setHorizontalAlignment(ALIGN_RIGHT);
//    north->setVerticalAlignment(ALIGN_TOP);
//    window->addToRegion(north, "North");

//    // central area of overall window tests border layout
//    GContainer* center = new GContainer(GContainer::LAYOUT_BORDER);
//    center->setBackground("green");
//    center->addToRegion(new GButton("CN1"), "North");
//    center->addToRegion(new GButton("CN2"), "North");
//    center->addToRegion(new GButton("CN3"), "North");
//    center->addToRegion(new GButton("CW1"), "North");
//    center->setRegionHorizontalAlignment("North", "Left");

//    center->addToRegion(new GButton("CWest1"), "West");
//    center->addToRegion(new GButton("CW2"), "West");
//    center->setRegionAlignment("West", "Right", "Top");

//    center->addToRegion(new GButton("CEast1"), "East");
//    center->addToRegion(new GButton("CE2"), "East");
//    center->addToRegion(new GButton("CEs3"), "East");
//    center->setRegionAlignment("East", "Center", "Bottom");

//    center->addToRegion(new GButton("CS1"), "South");
//    center->addToRegion(new GButton("CSouth2"), "South");
//    center->setRegionHorizontalAlignment("South", "Right");

//    center->addToRegion(new GButton("CCenter"), "Center");
//    window->addToRegion(center, "Center");

//    GContainer* east = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
//    east->setBackground("cyan");
//    GContainer* east1 = new GContainer(GContainer::LAYOUT_FLOW_HORIZONTAL);
//    GContainer* east2 = new GContainer(GContainer::LAYOUT_FLOW_HORIZONTAL);
//    east->add(east1);
//    east->add(east2);
//    window->addToRegion(east, "East");
//    east1->add(new GButton("E11"));
//    east1->add(new GButton("East12"));
//    east1->add(new GButton("E13"));
//    east2->add(new GButton("East11"));
//    east2->add(new GButton("E12"));

//    // southern area of overall window tests grid layout
//    int rows = 4;
//    int cols = 3;
//    GContainer* south = new GContainer(GContainer::LAYOUT_GRID, rows, cols);
//    south->setBackground("blue");
//    south->add(new GButton("r0 long c0"));
//    south->add(new GButton("r0c1"));
//    south->add(new GButton("r0c2"));
//    south->add(new GButton("r1c0"));
//    south->addToGrid(new GButton("r1 c1 row/colspan\ntall BOOYAH\nYAY!!!!!"), 1, 1, 2, 2);
////    south->add(new GButton("r1c2"));
//    south->addToGrid(new GButton("r2c0"), 2, 0);
////    south->add(new GButton("r2 long c1"));
////    south->add(new GButton("r2 ok c2"));
//    south->addToGrid(new GButton("r3 ok c0\ntall\ntall!"), 3, 0);
//    south->add(new GButton("r3 ok c1"));
//    south->add(new GButton("r3 ok c2"));
//    south->setHorizontalAlignment(ALIGN_RIGHT);
//    south->setVerticalAlignment(ALIGN_TOP);
//    window->addToRegion(south, "South");
//    window->setRegionHorizontalAlignment(GWindow::REGION_SOUTH, ALIGN_RIGHT);

//    window->show();
//}
