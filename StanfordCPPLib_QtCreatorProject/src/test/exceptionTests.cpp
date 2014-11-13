/*
 * Test file for verifying the Stanford C++ lib exception functionality.
 */

#include "testcases.h"
#include "error.h"
#include "exceptions.h"
#include "vector.h"
#include <csignal>
#include <iostream>
#include <string>
using namespace std;

namespace MyNS {
    class LoL {
    public:
        static int cCcCc_cCcCc(int, char*, double, char, bool) {
            cout << "function c start" << endl;
            cout << endl;
            // error("blargh!");
            Vector<int> v;
            v.get(42);  // lolol
            cout << "function c end" << endl;
            return 0;
        }
    };
}

void b() {
    cout << "function b start" << endl;
    MyNS::LoL::cCcCc_cCcCc(0, NULL, 0, 0, false);
    cout << "function b end" << endl;
}

void a() {
    cout << "function a start" << endl;
    b();
    cout << "function a end" << endl;
}

void exceptionTest() {
    a();
}

void stackOverflowTest(int n) {
    int a[100] = {0};
    if (a[0] || n % 1000 == 0) {
        cout << "stack overflow n=" << n << endl;
    }
    stackOverflowTest(n+1);
}

void segC_theOneThatActuallyThrows(int sig) {
    if (sig == SIGFPE) {
        // divide by 0 (generate SIGFPE)
        int a = 1;
        int b = 0;
        cout << (a/b) << endl;
    } else if (sig == SIGSEGV) {
        // dereference a NULL pointer (generate SIGSEGV)
        int* foo = 0;
        cout << *foo << endl;
    }
    cout << "will never get here lol" << endl;
}

void segB(int sig) {
    segC_theOneThatActuallyThrows(sig);
}

void segA(int sig) {
    segB(sig);
}

void segfaultTest(int sig) {
    if (sig == 0) {
        sig = SIGSEGV;
    }
    cout << "Hello, world!" << endl;
    segA(sig);
}
