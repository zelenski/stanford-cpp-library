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
#include "thread.h"
#include "biginteger.h"
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
//    cout << "You're about to be prompted to enter some text." << endl;
//    cout << "When you do, close the window." << endl;
//    cout << "Then, look down at stdout." << endl;
//    cout << "You should notice that some input was entered even though" << endl;
//    cout << "nothing was echoed to the graphical console." << endl;
//    cout << "This desyncs the graphics console from stdin/stdout," << endl;
//    cout << "though I don't know why." << endl;

    GWindow gw(400, 300);
    gw.setColor("blue");
    gw.fillPolygon({30, 20, 90, 50, 70, 120, 150, 100, 180, 10});

    for (int i = 0; ; i++) {
        string line = getLine("Enter line " + to_string(i) + ": ");
        cout << "You entered: " << line << endl;
        cout << "Now waiting for a click event:" << endl;
        GMouseEvent event = waitForClick();
        cout << "event = " << event << endl;
    }
}

/*
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
*/

int f() {
    if (2 % 2 == 1) {
        return 46;
    } else {
        return 45;
    }
}

void voidf() {
//    if (rand() && 2 % 2 == 1) {
//        cout << "f says 42!" << endl;
//    } else {
        cout << "f says 43!" << endl;
//    }
}

bigint fact(int n) {
    bigint result("1");
    for (int i = 2; i <= n; i++) {
        result *= BigInteger(integerToString(i));
    }
    return result;
}

void testBigInteger() {
    {
        BigInteger bi("-24");
        cout << "abs = " << bi.abs() << endl;
        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
        bi = bi.abs();
        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
        cout << "pos: " << boolalpha << BigInteger::ZERO.isPositive() << ", neg: " << BigInteger::ZERO.isNegative() << ", nonneg: " << BigInteger::ZERO.isNonNegative() << endl;

        BigInteger bi2("16");
        cout << "gcd = " << bi.gcd(bi2) << endl << endl;

        do {
            cout << "int bi2 = " << bi2 << ", hex = " << bi2.toString(16) << ", oct = " << bi2.toString(8) << ", bin = " << bi2.toString(2) << endl;
            bi2 *= BigInteger::TWO;
        } while (bi2.isInt());

        do {
            cout << "long bi2 = " << bi2 << endl;
            bi2 *= BigInteger::TEN;
        } while (bi2.isLong());

        cout << "max = " << bi.max(bi2) << ", min = " << bi.min(bi2) << endl;

        cout << "modPow = " << BigInteger::TWO.modPow(10, 100) << endl;

        cout << "pow = " << endl;
        for (int i = 0; i < 20; i++) {
            cout << "2 ^ " << i << " = " << BigInteger::TWO.pow(i) << endl;
        }

        // radix stuff
        BigInteger biHex("0xff00", 16);
        cout << "bihex = " << biHex << ", hex = " << biHex.toString(16) << endl;
        BigInteger biBin("0b101010", 2);
        cout << "bibin = " << biBin << ", bin = " << biBin.toString(2) << endl;

        BigInteger biUnary("1111111", 1);
        cout << "biunary = " << biUnary << ", unary " << biUnary.toString(1) << endl;

        for (BigInteger bi = 1; bi <= 100; bi++) {
            cout << "int bi = " << bi << ", hex = " << bi.toString(16) << ", oct = " << bi.toString(8) << ", bin = " << bi.toString(2) << endl;
        }

        // various int/long/double x BigInteger binary operations
        cout << "bitwise stuff:" << endl;
        BigInteger binary1("10100011", 2);
        BigInteger binary2("01110110", 2);
        cout << "a     = " << binary1.toString(2) << endl;
        cout << "b     = " << setw(8) << binary2.toString(2) << endl;
        cout << "a & b = " << setw(8) << (binary1 & binary2).toString(2) << endl;
        cout << "a | b = " << setw(8) << (binary1 | binary2).toString(2) << endl;
        cout << "a ^ b = " << setw(8) << (binary1 ^ binary2).toString(2) << endl;
        cout << "~a    = " << setw(8) << (~binary1).toString(2) << endl;
        cout << "!a    = " << setw(8) << (!binary1).toString(2) << endl;
        cout << endl;

        cout << "int +: " << (42 + bi) << endl;
        cout << "int -: " << (42 - bi) << endl;
        cout << "int *: " << (42 * bi) << endl;
        cout << "int /: " << (42 / bi) << endl;
        cout << "int %: " << (42 % bi) << endl;

        cout << "long +: " << ((long)42 + bi) << endl;
        cout << "long -: " << ((long)42 - bi) << endl;
        cout << "long *: " << ((long)42 * bi) << endl;
        cout << "long /: " << ((long)42 / bi) << endl;
        cout << "long %: " << ((long)42 % bi) << endl;

//        cout << "double +: " << (42.5 + bi) << endl;
//        cout << "double -: " << (42.5 - bi) << endl;
//        cout << "double *: " << (42.5 * bi) << endl;
//        cout << "double /: " << (42.5 / bi) << endl;
//        cout << "double %: " << (42.5 % bi) << endl;
    }

    try {
        BigInteger bad("-845lol098");
        cout << bad << endl;
    } catch (ErrorException& ex) {
        cout << "correctly threw exception when creating bad integer: " << ex.getMessage() << endl;
    }

    // big division
    BigInteger realbig(1000);
    realbig *= realbig;
    realbig *= realbig;
    realbig *= realbig;
    realbig *= realbig;
    BigInteger almostAsBig(realbig);
    realbig *= realbig;
    cout << "realbig: " << realbig << endl;
    cout << "almost : " << almostAsBig << endl;

    try {
        BigInteger quotient = realbig / almostAsBig;
        cout << "quotient: " << quotient << endl;
    } catch (ErrorException& ex) {
        cout << "correctly threw exception when dividing by large integer" << endl;
    }


//    cout << "Fib:" << endl;
//    BigInteger a(1);
//    BigInteger b(1);
//    for (int i = 0; i < 100; i++) {
//        BigInteger c = a + b;
//        cout << "#" << i << ": " << c << endl;
//        a = b;
//        b = c;
//    }

//    cout << "Fact:" << endl;
//    bigint f;
//    for (int i = 0; i < 50; i++) {
//        f = fact(i);
//        cout << "fact(" << i << ") = " << f << endl;
//    }

//    for (int i = 0; i < 19; i++) {
//        bigint g = fact(i);
//        bigint quotient = f / g;
//        bigint mod = f / g;

//        cout << "i = " << i << ", fact = " << g << ", quot = " << quotient << ", mod = " << mod << endl;
//    }
}

int main() {
    cout << "Stanford C++ lib tester" << endl;

    testBigInteger();

    // testKeithWindowTest();

//    cout << "f is: " << f() << endl;


//    Thread th = forkThread(voidf);

//    testNotes();

//    testProcess();

//    cout << stanfordcpplib::collections::compareTo(42, 17) << endl;
//    cout << stanfordcpplib::collections::compareTo2(42, 42, 17, 35) << endl;
//    cout << stanfordcpplib::collections::compareTo3(42, 42, "a", "a", 'b', 'a') << endl;
//    cout << stanfordcpplib::collections::compareTo4(42, 42, "a", "a", 'b', 'b', 1.0, 0.5) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 17, 29) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 37, 29) << endl;
//    cout << stanfordcpplib::collections::compareTo5(42, 42, "a", "a", 'b', 'b', 1.0, 1.0, 29, 29) << endl;

    /*
    while (true) {
        cout << "   DATA STRUCTURES::" << endl;
        cout << "c) collections" << endl;
        cout << "cs) collection stream extraction" << endl;
        cout << "ci) collection initializer lists" << endl;
        cout << "csh) collection shuffle" << endl;

        cout << "   STUFF RELATED TO INPUT/OUTPUT:" << endl;
        cout << "e) cout / cerr mix" << endl;
        cout << "i) cin / cout" << endl;
        cout << "an) animation" << endl;
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
        } else if (cmd == "an") {
            animationTest();
        } else if (cmd == "c") {
            randomElementTest();
            compareTest();
            foreachTest();
            hashCodeTest();
        } else if (cmd == "cs") {
            streamExtractTest();
        } else if (cmd == "csh") {
            shuffleTest();
        } else if (cmd == "ci") {
            initializerListTest();
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
    */
    
    cout << "Done." << endl;
    return 0;
}
