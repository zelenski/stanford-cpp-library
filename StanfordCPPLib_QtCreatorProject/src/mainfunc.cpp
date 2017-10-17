/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

//#include "console.h"
#include "error.h"
#include "collections.h"
#include "exceptions.h"
#include "gwindow.h"
#include "gevents.h"
#include "map.h"
#include "note.h"
#include "process.h"
#include "vector.h"
#include "queue.h"
#include "random.h"
#include "strlib.h"
// #include "testcases.h"
#include "private/version.h"
#include "types.h"
#include <exception>
#include <iostream>

#include <iomanip>
#include <iostream>
#include <string>
#include "basicgraph.h"
#include "call_stack.h"
#include "console.h"
#include "error.h"
#include "exceptions.h"
#include "map.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

void bar() {
    int x = 3;
    int y = 5;
    x += y;

    int a = 3;
    int b = 5;
    a += b;

    Vector<int> v;
    v.add(42);
    error(":-(");

    int c = 2;
    int d = 4;
    c += d;
    cout << "duh duh duh " << a << b << c << d << x << y << endl;
}

void foo() {
    bar();
}

void test(Map<int, string> m) {
    cout << m << endl;
}

void testNotes() {
    // Note note(1.5, Note::G, 5, Note::NATURAL, false);
    Note note("1.5 G 5 NATURAL false");
    Note rest("1 R false");
    // std::cout << "NOTE: " << note << std::endl;
    std::cout << "about to play ... " << std::endl;
    note.play();
    rest.play();
    std::cout << "about to play 2 ... " << std::endl;
    note.play();
    rest.play();
}

void testKeithWindowTest() {
    // std::cout << "Complete." << std::endl;
    cout << "You're about to be prompted to enter some text." << endl;
    cout << "When you do, close the window." << endl;
    cout << "Then, look down at stdout." << endl;
    cout << "You should notice that some input was entered even though" << endl;
    cout << "nothing was echoed to the graphical console." << endl;
    cout << "This desyncs the graphics console from stdin/stdout," << endl;
    cout << "though I don't know why." << endl;

    GWindow sacrificialWindow(400, 300);

    for (int i = 0; ; i++) {
        string line = getLine("Enter line " + to_string(i) + ": ");
        cout << "You entered: " << line << endl;
        cout << "Now waiting for a click event:" << endl;
        GMouseEvent event = waitForClick();
        cout << "event = " << event << endl;
    }
}

void testProcess() {
    Process proc;
    proc.addCommandLineArgs({
//        "/bin/uname",
//        "-a"
        // "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/exitcode42"
        // "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/delayed_output"
        "/home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/test_programs/infinite_loop"
    });
    cout << "my process is: " << proc << endl << endl;
    cout << "About to call startAndWait ..." << endl;

    proc.setTimeout(1000);
    proc.startAndWait();

    cout << "Done with startAndWait" << endl << endl;
    cout << "my process is: " << proc << endl << endl;

    cout << "About to call stop ..." << endl;
    proc.stop();
    cout << "Done with stop" << endl << endl;

    cout << "exit code:" << proc.exitCode() << endl;

    cout << "output (length " << proc.output().length() << "):" << endl
         << proc.output() << endl << endl;
    cout << "my process is: " << proc << endl << endl;
}

int main() {
//    testNotes();

    testProcess();

//    cout << stanfordcpplib::collections::compareTo(42, 17) << endl;
//    cout << stanfordcpplib::collections::compareTo2(42, 42, 17, 35) << endl;
//    cout << stanfordcpplib::collections::compareTo3(42, 42, "a", "a", 'b', 'a') << endl;
//    cout << stanfordcpplib::collections::compareTo4(42, 42, "a", "a", 'b', 'b', 1.0, 0.5) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 17, 29) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 37, 29) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 29, 29) << endl;

//    while (true) {
//        cout << "   DATA STRUCTURES::" << endl;
//        cout << "c) collections" << endl;
//        cout << "cs) collection stream extraction" << endl;
//        cout << "ci) collection initializer lists" << endl;
//        cout << "csh) collection shuffle" << endl;

//        cout << "   STUFF RELATED TO INPUT/OUTPUT:" << endl;
//        cout << "e) cout / cerr mix" << endl;
//        cout << "i) cin / cout" << endl;
//        cout << "an) animation" << endl;
//        cout << "p) pipe" << endl;
//        cout << "s) strings" << endl;
//        cout << "u) urlstream" << endl;
        
//        cout << "   GRAPHICS:" << endl;
//        cout << "fd) file dialog" << endl;
//        cout << "bi) buffered image" << endl;
//        cout << "op) optionpane" << endl;
//        cout << "rb) radio buttons" << endl;
//        cout << "ta) table" << endl;

//        cout << "   HTTP SERVER:" << endl;
//        cout << "ss) start server" << endl;

//        cout << "   ERROR HANDLING AND STACK TRACES:" << endl;
//        cout << "   (Try all 3 to make sure stack trace prints.)" << endl;
//        cout << "   (Current configuration shows no trace on stack overflow.)" << endl;
//        cout << "n) segfault (NULL ptr)" << endl;
//        cout << "t) throw exception" << endl;
//        cout << "o) stack overflow" << endl;
//        cout << "ri) recursion indent" << endl;
//        cout << endl;
//        string cmd = getLine("Command (Enter to quit)?");
//        if (cmd.empty()) {
//            break;
//        } else if (cmd == "an") {
//            animationTest();
//        } else if (cmd == "c") {
//            randomElementTest();
//            compareTest();
//            foreachTest();
//            hashCodeTest();
//        } else if (cmd == "cs") {
//            streamExtractTest();
//        } else if (cmd == "csh") {
//            shuffleTest();
//        } else if (cmd == "ci") {
//            initializerListTest();
//        } else if (cmd == "e") {
//            coutCerrMixTest();
//        } else if (cmd == "fd") {
//            fileDialogTest();
//        } else if (cmd == "bi") {
//            gbufferedImageTest();
//        } else if (cmd == "op") {
//            goptionpaneTest();
//        } else if (cmd == "rb") {
//            radioButtonTest();
//        } else if (cmd == "i") {
//            // cinOutTest();
//            killProcessTest();
//        } else if (cmd == "n") {
//            segfaultTest();
//        } else if (cmd == "p") {
//            outputColorTest();
//            cinOutTest();
//            coutCerrMixTest();
//            getIntegerTest();
//            longStringTest();
//            // killProcessTest();
//        } else if (cmd == "o") {
//            //getPlatform()->setStackSize(1024*1024*128);
//            stackOverflowTest();
//        } else if (cmd == "ri") {
//            recursionIndentTest();
//        } else if (cmd == "s") {
//            stringToIntegerTest();
//        } else if (cmd == "ss") {
//            serverTest();
//        } else if (cmd == "t") {
//            exceptionTest();
//        } else if (cmd == "ta") {
//            gtableTest();
//        } else if (cmd == "u") {
//            urlstreamTest();
//        }
//    }
    
    return 0;
}
