///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include "gcolor.h"
//#include "gobjects.h"
//#include "ginteractors.h"
//#include "timer.h"
//using namespace std;

//void testQtable();

//int mainQtTable() {
//    testQtable();
//    return 0;
//}

//void testQtable() {
//    static GWindow* window = new GWindow(900, 300);
//    window->setTitle("QtGui Window");
//    window->setResizable(true);
//    window->setExitOnClose(true);
//    window->center();


//    GButton* button = new GButton("Triforce");
//    button->setColor(GColor::RED);
//    button->setBackground(GColor::YELLOW);
//    // button->setIcon("triangle-icon.png");
//    button->setTextPosition(GInteractor::TEXT_BESIDE_ICON);
//    button->setActionListener([](GEvent event) {
//        cout << "button click! event = " << event << endl;
//    });

//    button->setDoubleClickListener([](GEvent event) {
//        cout << "button double-click! event = " << event << endl;
//    });

//    button->setAccelerator("Ctrl-T");
//    window->addToRegion(button, "South");
//    cout << "button:    " << button->toString() << endl;
//    cout << "button accelerator: " << button->getAccelerator() << endl;
//    cout << "button font: " << button->getFont() << endl;
//    button->setFont("Monospaced-Bold-14");

//    int rows = 7;
//    int cols = 5;
//    static GTable* table = new GTable(rows, cols);
//    table->setColumnHeaderStyle(GTable::COLUMN_HEADER_EXCEL);
//    for (int r = 0; r < rows; r++) {
//        for (int c = 0; c < cols; c++) {
//            table->set(r, c, "hello");
//        }
//    }

//    // styles
//    // 1) global
//    table->setColor("magenta");

//    // 2) row
//    table->setRowBackground(1, "green");
//    table->setRowFont(1, "Monospaced-12-Bold");
//    table->setRowForeground(3, "Blue");

//    // 3) column
//    table->setColumnAlignment(1, ALIGN_RIGHT);
//    table->setColumnBackground(1, "Cyan");
//    table->setColumnFont(2, "*-*-Italic");
//    table->setColumnForeground(3, "Red");

//    // 4) cell
//    table->setCellForeground(0, 0, "Red");
//    table->setCellAlignment(1, 1, ALIGN_CENTER);
//    table->setCellBackground(2, 2, "Blue");

//    // 1) global (again) - should override 2)-4)
//    table->setBackground("yellow");

//    // 2) row (again) - should set cell (1,1) to bold, not italic
//    // table->setRowFont(1, "Monospaced-12-Bold");

//    table->setTableListener([](GEvent event) {
//        cout << "table event: " << event.toString() << endl;
//    });
//    window->addToRegion(table, "Center");
//    // window->pack();

//    GButton* button2 = new GButton("Resize");
//    button2->setActionListener([]() {
//        table->resize(table->numRows() + 1, table->numCols() + 1);
//    });
//    window->addToRegion(button2, "South");

//    GButton* button3 = new GButton("ClearF");
//    button3->setActionListener([]() {
//        // table->clearCellFormatting(1, 1);
//        table->clearFormatting();
//    });
//    window->addToRegion(button3, "South");
//}
