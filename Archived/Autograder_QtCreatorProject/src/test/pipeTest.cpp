#include "testcases.h"
#include "consoletext.h"
#include "error.h"
#include "regexpr.h"
#include "simpio.h"
#include "strlib.h"
#include <iostream>
#include <string>
using namespace std;

void animationTest() {
    for (int i = 0; i < 20; i++) {
        cout << "Frame #" << i << endl;
        pause(200);
        clearConsole();
    }
    getLine("animation complete.");
    setConsoleClearEnabled(false);
    for (int i = 0; i < 20; i++) {
        cout << "Frame #" << i << endl;
        pause(200);
        clearConsole();
    }
    getLine("animation 2 complete (no clear).");
    setConsoleClearEnabled(true);
    for (int i = 0; i < 20; i++) {
        cout << "Frame #" << i << endl;
        pause(200);
        clearConsole();
    }
    getLine("animation 3 complete (w/ clear).");
}

void cinOutTest() {
    cout << "Hello, world! This is main!" << endl;
    for (int i = 0; i < 100; i++) {
        cout << "hello" << endl;
    }
    string input = getLine("How are you doing? ");
    cout << "You said, \"" << input << "\"." << endl;
    cout << "The end." << endl;
}

void coutCerrMixTest() {
    cout << "Hello, world! This is main!" << endl;
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

void getIntegerTest() {
    int n = getInteger("Type an int! ");
    cout << "you typed " << n << endl;
    while (true) {
        std::string line = getLine("Type a string! ");
        if (stringIsInteger(line)) {
            cout << "an integer!" << endl;
        } else {
            cout << "not integer" << endl;
        }
        if (line == "") break;
    }

    //    plainconsole::setEcho(true);
    //    int x = getInteger("Type x: ");
    //    cout << "You typed: " << x << endl;
    //    cout << "Type again: ";
    //    cin >> x;
    //    cout << "You typed: " << x << endl;
    
    // test some int <-> char stuff
    char ch = '3';
    int x = charToInteger(ch);
    cout << ch << " is " << x << endl;
    
    ch = integerToChar(x);
    cout << x << " is " << ch << endl;
    
    // test errors
    // charToInteger('x');
    // integerToChar(42);
        
    //    cout << randomInteger(1, 10) << " "
    //         << randomInteger(1, 10) << " "
    //         << randomInteger(1, 10) << endl;
}

void killProcessTest() {
    std::cout << "Try killing the Java process now. Does it quit gracefully?" << std::endl;
    while (true) {
        try {
           std::string line = getLine();
           std::cout << line << std::endl;
        } catch (ErrorException& ex) {
            std::cout << "Error: " << ex.getMessage() << std::endl;
        }
    }
}

void longStringTest() {
    string slong = "abcbcde";
    while (slong.length() < 1024*64) {
        slong += slong;
    }
    cout << "about to regex replace on long string (length " << slong.length() << ")" << endl;
    slong = regexReplace(slong, "bc", "XX");
    cout << "done with regex replace on long string (length " << slong.length() << ")." << endl;
    cout << slong.substr(0, 60) << " ..." << endl;
    cout << "done." << endl;
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
