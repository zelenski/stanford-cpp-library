///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include <iomanip>
//#include "error.h"
//#include "qconsole.h"
//#include "timer.h"
//using namespace std;

//void testQtConsole();

//int mainQtConsole() {
//    testQtConsole();
//    return 0;
//}

//void testQtConsole() {
//    // QGui::instance()->runOnQtGuiThread([]() {
//        //    cout << "Hello, world!" << endl;
//        //    cout << "This is a test of the Qt graphical console!" << endl;
//        //    cout << endl;
//        cout << "Hello, cout 1!" << endl;
//        cerr << "Hello, cerr 1!" << endl;
//        cout << "Hello, cout 2!" << endl;
//        cerr << "Hello, cerr 2!" << endl;
//        cout << endl;
//        cerr << "This is stderr!" << endl;
//        cerr << "So is this." << endl;
//        cout << "COUT IN THIS PART ";
//        // cout.flush();
//        cerr << "... BUT IT ALTERNATES TO CERR! ";
//        // cerr.flush();
//        cout << endl;

//        Timer tim;
//        tim.start();
//        for (int i = 0; i < 1001; i++) {
//            cout << setw(5) << i;
//            if (i > 0 && i % 10 == 0) {
//                cout << endl;
//                // stanfordcpplib::qtgui::getConsoleWindow()->pause(10);
//            }
//        }
//        tim.stop();
//        cout << "Took " << tim.elapsed() << "ms" << endl;
//        cout << "What about me? :-(";   // no endl

//        cout << "How about a nasty error?!" << endl;
//        error("oooooooooops");

//    // });

////    cout << "How about a nasty error?!" << endl;
////    error("oooooooooops");
////    cout << "That was not cool." << endl;
//}
