
#include "consoletext.h"
#include "simpio.h"
#include <iostream>
#include <string>
#include "gconsolewindow.h"
#include "SimpleTest.h"

using namespace std;

MANUAL_TEST("console animation") {
    for (int i = 0; i < 10; i++) {
        cout << "Frame #" << i << endl;
        pause(100);
        clearConsole();
    }
    getConsoleWindow()->requestFocus();
    getLine("animation complete, RETURN to continue");
    setConsoleClearEnabled(false);
    for (int i = 0; i < 10; i++) {
        cout << "Frame #" << i << endl;
        pause(100);
        clearConsole();
    }
    getLine("animation 2 complete (no clear), RETURN to continue");
    setConsoleClearEnabled(true);
    for (int i = 0; i < 10; i++) {
        cout << "Frame #" << i << endl;
        pause(100);
        clearConsole();
    }
    getLine("animation 3 complete (w/ clear), RETURN to continue");
}

MANUAL_TEST("console mixed cout/cerr") {
    cerr << "This message comes from cerr" << endl;
    cout << "Another one from cout!" << endl;
    cerr << "This message ALSO comes from cerr" << endl;
    cout << "A third one from cout!" << endl;
    cerr << "HOW ";
    cout << "about ";
    cerr << "A ";
    cout << "mixed ";
    cerr << "LINE??";
    cout << endl;
}

void outputColorTest() {
    cout << "Output color test:" << endl;
    setConsoleOutputColor("#ff00ff");
    cout << "Purple line" << endl;
    setConsoleOutputColor("#0000ff");
    cout << "Blue line" << endl;
    setConsoleOutputColor("#00ff33");
    cout << "GREEN line" << endl;
    setConsoleOutputColor("#666666");
    cout << "GRAY LINE" << endl;
    setConsoleOutputColor("#000000");
    cout << "black line" << endl;
    cout << endl;
}
