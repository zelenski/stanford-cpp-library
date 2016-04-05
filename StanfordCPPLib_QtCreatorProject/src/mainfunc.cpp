/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include "console.h"
#include "error.h"
#include "exceptions.h"
#include "map.h"
#include "plainconsole.h"
#include "vector.h"
#include "queue.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "test/testcases.h"
#include "private/version.h"
#include "types.h"
#include <exception>
#include <iostream>
using namespace std;

//int Main() {
//    return 0;
//}

void terminate_handler_func() {
    
}

void unexpected_handler_func() {
    std::cerr << "unexpected called!!" << endl;
    exceptions::printStackTrace();
}

int foo(int n) {
    if (n == 5) {
        error("oh noez");
        return 0;
    } else if (n <= 0) {
        return 1;
    } else {
        return 2 * foo(n - 1);
    }
}

int main() {
#ifdef _console_h
    setConsoleSize(1000, 600);
    setConsoleLocation(-1, -1);
    setConsoleWindowTitle("Marty is great");
#endif // _console_h
    
//    try {
//        foo(8);
//    } catch (ErrorException ex) {
//        cout << "stack trace:" << endl << ex.getStackTrace() << endl;
//    }
    
    while (true) {
        cout << "   DATA STRUCTURES::" << endl;
        cout << "c) collections" << endl;
        
        cout << "   STUFF RELATED TO INPUT/OUTPUT:" << endl;
        cout << "e) cout / cerr mix" << endl;
        cout << "i) cin / cout" << endl;
        cout << "p) pipe" << endl;
        cout << "s) strings" << endl;
        cout << "u) urlstream" << endl;
        
        cout << "   GRAPHICS:" << endl;
        cout << "fd) file dialog" << endl;
        cout << "bi) buffered image" << endl;
        cout << "op) optionpane" << endl;
        cout << "rb) radio buttons" << endl;
        cout << "ta) table" << endl;

        cout << "   HTTP SERVER:" << endl;
        cout << "ss) start server" << endl;

        cout << "   ERROR HANDLING AND STACK TRACES:" << endl;
        cout << "   (Try all 3 to make sure stack trace prints.)" << endl;
        cout << "   (Current configuration shows no trace on stack overflow.)" << endl;
        cout << "n) segfault (NULL ptr)" << endl;
        cout << "t) throw exception" << endl;
        cout << "o) stack overflow" << endl;
        cout << "ri) recursion indent" << endl;
        cout << endl;
        string cmd = getLine("Command (Enter to quit)?");
        if (cmd.empty()) {
            break;
        } else if (cmd == "c") {
            randomElementTest();
            compareTest();
            foreachTest();
            hashCodeTest();
        } else if (cmd == "e") {
            coutCerrMixTest();
        } else if (cmd == "fd") {
            fileDialogTest();
        } else if (cmd == "bi") {
            gbufferedImageTest();
        } else if (cmd == "op") {
            goptionpaneTest();
        } else if (cmd == "rb") {
            radioButtonTest();
        } else if (cmd == "i") {
            // cinOutTest();
            killProcessTest();
        } else if (cmd == "n") {
            segfaultTest();
        } else if (cmd == "p") {
            outputColorTest();
            cinOutTest();
            coutCerrMixTest();
            getIntegerTest();
            longStringTest();
            // killProcessTest();
        } else if (cmd == "o") {
            //getPlatform()->setStackSize(1024*1024*128);
            stackOverflowTest();
        } else if (cmd == "ri") {
            recursionIndentTest();
        } else if (cmd == "s") {
            stringToIntegerTest();
        } else if (cmd == "ss") {
            serverTest();
        } else if (cmd == "t") {
            exceptionTest();
        } else if (cmd == "ta") {
            gtableTest();
        } else if (cmd == "u") {
            urlstreamTest();
        }
    }
    
    std::cout << "Complete." << std::endl;
    return 0;
}
