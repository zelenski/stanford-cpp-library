/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

//#include "console.h"
//#include "error.h"
//#include "exceptions.h"
//#include "map.h"
//#include "note.h"
//#include "vector.h"
//#include "queue.h"
//#include "random.h"
//#include "strlib.h"
//// #include "testcases.h"
//#include "private/version.h"
//#include "types.h"
//#include <exception>
//#include <iostream>

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

int main() {
//    cout << "&main = " << (void*) Main << endl;
//    cout << "&foo  = " << (void*) foo << endl;
//    cout << "&bar  = " << (void*) bar << endl;
//    getLine("?");

//    foo();

    cout << "hi" + 40000 << endl;
    cout << "hi" + 'x' << endl;
    int n = 4000;
    char c = 'x';
    cout << "hi" + n << endl;
    cout << "hi" + c << endl;

    /*HashMap<int, string> map1;
    map1[1] = "10";
    map1[6] = "60";
    map1[7] = "70";
    map1[4] = "40";
    map1[5] = "50";
    map1[2] = "20";
    map1[3] = "30";
    map1[8] = "80";
    HashMap<int, string> map2 = map1;
    test(map2);
    cout << boolalpha << map1.containsKey(1) << endl;
    cout << boolalpha << map2.containsKey(10) << endl;
    for (int k : map1) {
        if (k % 2 == 0) {
            cout << "remove " << k << endl;
            //map1.remove(k);
            map1.clear();
        } else {
            cout << "modify " << k << endl;
            map1[k] += "!";
        }
    }
    cout << "Done" << endl;*/

//    Vector<int> v {1, 2, 3, 4, 5};
//    for (int n : v) {
//        if (n % 2 != 0) {
//            cout << "Vector remove " << n << endl;
//            v.remove(n);
//        }
//    }
//    cout << "Done" << endl;

//    BasicGraph graph;
//    graph.addVertex("v1");
//    graph.removeVertex("v1");
//    cout << graph << endl;
//    cout << boolalpha << graph.containsVertex("v1") << endl;


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
    
//    std::cout << "Complete." << std::endl;

    // Note note(1.5, Note::G, 5, Note::NATURAL, false);
//    Note note("1.5 G 5 NATURAL false");
//    Note rest("1 R false");
//    // std::cout << "NOTE: " << note << std::endl;
//    std::cout << "about to play ... " << std::endl;
//    note.play();
//    rest.play();
//    std::cout << "about to play 2 ... " << std::endl;
//    note.play();
//    rest.play();



    std::cout << "done." << std::endl;
    return 0;
}
