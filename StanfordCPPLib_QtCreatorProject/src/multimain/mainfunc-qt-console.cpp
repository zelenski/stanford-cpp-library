/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include <iomanip>
#include "error.h"
#include "consoletext.h"
#include "regexpr.h"
#include "set.h"
#include "simpio.h"
#include "timer.h"
#include <QThread>
#include "gthread.h"
using namespace std;

void testQtConsole();

int multimain(console) {
    testQtConsole();
    return 0;
}

void testQtConsole() {
    cout << "Hello, cout 1!" << endl;
    cerr << "Hello, cerr 1!" << endl;
    cout << "Hello, cout 2!" << endl;
    cerr << "Hello, cerr 2!" << endl;
    cout << endl;
    cerr << "This is stderr!" << endl;
    cerr << "So is this." << endl;
    cout << "COUT IN THIS PART ";
    // cout.flush();
    cerr << "... BUT IT ALTERNATES TO CERR! ";
    // cerr.flush();
    cout << endl;
    cout << "COUT ";
    cerr << "CERR ";
    cout << "COUT ";
    cerr << "CERR ";
    cout << "COUT ";
    cerr << "CERR ";
    cout << "COUT ";
    cerr << "CERR ";
    cout << "COUT ";
    cerr << "CERR ";
    cout << endl;

    Timer tim;
    tim.start();
    string s = "hi";
    s = s.substr(1, 1);
    for (int i = 0; i < 1001; i++) {
        cout << setw(5) << i;
        if (i > 0 && i % 10 == 0) {
            cout << endl;
            // stanfordcpplib::qtgui::getConsoleWindow()->pause(10);
        }
    }
    tim.stop();
    cout << "Took " << tim.elapsed() << "ms" << endl;

    cout << "current Qt thread is: " << QThread::currentThread() << endl;
    cout << "current GThread is: " << GThread::getCurrentThread() << endl;
    cout << "student GThread is: " << GThread::getStudentThread() << endl;
    cout << "Qt GUI GThread is: " << GThread::getQtMainThread() << endl;

//    cout << "How about a nasty error?!" << endl;

//    // produce an error
//    Vector<int> v;
//    cout << v[99] << endl;

//    string s;
//    s.erase(999);

//    Set<char> letters {'\n', '\t', '\\'};
//    cout << "letters = " << letters << endl;

//    std::ostringstream out;
//    out << letters;

//    std::istringstream input(out.str());
//    Set<char> set2;
//    input >> set2;
//    cout << "letters after write/read = " << set2 << endl;

//    // try to get Set in a stack trace
////    Set<int> set1 {1, 2, 3};
////    Set<int> set2 {2, 3, 5};
////    Set<int> set3 = set1;
////    set3.unionWith(set2);
////    cout << "union = " << set3 << endl;
////    Set<int> set4 = set1;
////    set4.intersect(set2);
////    cout << "intersect = " << set4 << endl;
////    Set<int> set5 = set1;
////    set5.difference(set2);
////    cout << "difference = " << set5 << endl;

    // user input
    std::string line = getLine("Type: ");
    cout << "You typed: \"" << line << "\"" << endl;

    int num = getInteger("Type an integer: ");
    cout << "You typed: " << num << endl;

    double realnum = getDouble("Type a real number: ");
    cout << "You typed: " << realnum << endl;

//    cout << "Partial line; what about me? :-(";   // no endl
//    // error("oooooooooops");
}
