/*
 * Test file for verifying the Stanford C++ autograder lib functionality.
 */

#include "console.h"
#include "simpio.h"
#include "test/testcases.h"
#include "private/version.h"
#include <iostream>
using namespace std;

int main() {
    setConsoleSize(1000, 600);
    setConsoleLocation(-1, -1);
    setConsoleWindowTitle("Marty is great");
    
    while (true) {
        cout << "c) collections" << endl;
        cout << "e) cout / cerr mix" << endl;
        cout << "g) gui" << endl;
        cout << "i) cin / cout" << endl;
        cout << "n) segfault (NULL ptr)" << endl;
        cout << "p) pipe" << endl;
        cout << "s) stack overflow" << endl;
        cout << "t) throw exception" << endl;
        cout << "u) urlstream" << endl;
        string cmd = getLine("Command (Enter to quit)?");
        if (cmd.empty()) {
            break;
        } else if (cmd == "c") {
            compareTest();
            foreachTest();
            hashCodeTest();
        } else if (cmd == "e") {
            coutCerrMixTest();
        } else if (cmd == "g") {
            // fileDialogTest();
            gbufferedImageTest();
            // goptionpaneTest();
        } else if (cmd == "i") {
            cinOutTest();
        } else if (cmd == "n") {
            segfaultTest();
        } else if (cmd == "s") {
            //getPlatform()->setStackSize(1024*1024*128);
            stackOverflowTest();
        } else if (cmd == "t") {
            exceptionTest();
        } else if (cmd == "u") {
            urlstreamTest();
        }
    }
    
    std::cout << "Complete." << std::endl;
    return 0;
}
