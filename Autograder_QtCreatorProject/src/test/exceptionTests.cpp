/*
 * Test file for verifying the Stanford C++ lib exception functionality.
 */

#include "testcases.h"
#include "error.h"
#include "exceptions.h"
#include "recursion.h"
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
    MyNS::LoL::cCcCc_cCcCc(0, nullptr, 0, 0, false);
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

int rihelper(int k) {
    std::cout << recursionIndent() << "riHelper(" << k << ")" << endl;
    if (k <= 0) {
        return k;
    } else {
        int a = rihelper(k - 1);
        int b = rihelper(k - 2);
        return a + b;
    }
}

void recursionIndentTest() {
    rihelper(3);
}

// commenting out because of compiler warning about infinite recursion
void stackOverflowTest(int n) {
    int a[100] = {0};
    if (a[0] || n % 1000 == 0) {
        cout << "stack overflow n=" << n << endl;
    }
    if (!a[0]) {
        stackOverflowTest(n+1);
    }
}

void segC_theOneThatActuallyThrows(int sig) {
    if (sig == SIGFPE) {
        // divide by 0 (generate SIGFPE)
        int a = 1;
        int b = 0;
        cout << (a/b) << endl;
    } else if (sig == SIGSEGV) {
        // dereference a NULL pointer (generate SIGSEGV)
        int* foo = nullptr;
        cout << *foo << endl;
    } else if (sig == SIGABRT) {
        // cause program to "abort" (generate SIGABRT)
        abort();
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


TEST_CATEGORY(ExceptionTests, "Exception tests");

// test raising various kinds of exceptions
TIMED_TEST(ExceptionTests, ErrorExceptionTest, TEST_TIMEOUT_DEFAULT) {
    exceptionTest();
}

void excepB() {
    int x = 42;
    string s = "hi";
    s += x;
    throw 42;
}

void excepA() {
    int x = 42;
    string s = "hi";
    s += x;
    excepB();
}

TIMED_TEST(ExceptionTests, IntExceptionTest, TEST_TIMEOUT_DEFAULT * 1000) {
    excepA();
}

TIMED_TEST(ExceptionTests, DoubleExceptionTest, TEST_TIMEOUT_DEFAULT) {
    throw 3.14;
}

TIMED_TEST(ExceptionTests, StringExceptionTest, TEST_TIMEOUT_DEFAULT) {
    throw string("oh crap");
}

TIMED_TEST(ExceptionTests, CharStarExceptionTest, TEST_TIMEOUT_DEFAULT) {
    throw "oh crap!!";
}

TIMED_TEST(ExceptionTests, StdExceptionTest, TEST_TIMEOUT_DEFAULT) {
    throw std::exception();
}

// test raising various kinds of signals
TIMED_TEST(ExceptionTests, SegfaultTest, TEST_TIMEOUT_DEFAULT) {
    segfaultTest(SIGSEGV);
}

TIMED_TEST(ExceptionTests, SIGFPETest, TEST_TIMEOUT_DEFAULT) {
    segfaultTest(SIGFPE);
}

// can't test SIGABRT because Google Test framework needs to capture it
//TIMED_TEST(ExceptionTests, AbortTest, TEST_TIMEOUT_DEFAULT) {
//    segfaultTest(SIGABRT);
//}

// test stack overflow
// (commenting out because we currently can't gracefully catch stack overflows)
//TIMED_TEST(ExceptionTests, StackOverflowTest, TEST_TIMEOUT_DEFAULT) {
//    stackOverflowTest(42);
//}
