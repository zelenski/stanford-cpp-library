#include "testcases.h"
#include "error.h"
#include "regexpr.h"
#include "simpio.h"
#include <iostream>
#include <string>
using namespace std;

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
