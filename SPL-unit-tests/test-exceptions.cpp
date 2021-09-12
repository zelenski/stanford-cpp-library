#include <iostream>
#include "console.h"
#include "simpio.h"
#include "error.h"
//#include <QDebug>
#include "vector.h"
#include "set.h"
#include "filelib.h"
#include "strlib.h"
#include "gthread.h"
#include "exceptions.h"
#include "testing/SimpleTest.h"

using namespace std;

int gZero, *gPtr;

void stack_overflow() { static int count; count++; stack_overflow(); }

struct  {
    const char *explain;
    std::function<void()> fn;
} faults[] = {
    {"exit normally",   ([] { cout << "Goodbye!" << endl; } ) },
    {"infinite loop",   ([] { for (int i = 0; ; i++); } ) },
    {"segv",            ([] { *gPtr = 106; } ) },
    {"sigabort",        ([] { abort(); } ) },
    {"sigfpe",          ([] { gZero = 106/gZero; } ) },
    {"endless print",   ([] { for (int i = 0;  ; i++) cout << i << endl; } ) },
    {"stack overflow",  ([] { stack_overflow(); } ) },
    {"call error",      ([] { error("Qt is my best friend (not)"); } ) },
    {"call qFatal",     ([] { qFatal("Fatal error being reported via qFatal"); } ) },
    {"throw int",       ([] { throw 106; } ) },
    {"throw float",     ([] { throw 3.14159; } ) },
    {"throw str",       ([] { throw "Hyperjump!"; } ) },
    {"throw Vector",    ([] { throw Vector<int>{3, 4}; } ) },
    {"Vector bounds",   ([] { Vector<int> v; v[106]; } ) },
    {"Vector modify",   ([] { Vector<int> v {4, 7}; for (int m : v) v.remove(m); } ) },
    {"Set modify",      ([] { Set<int> set {4, 7}; for (int m : set) set.add(m*2); } ) },
    {"stringToReal",    ([] { stringToReal("3.14Pi.59"); } ) },
    {"readFile",        ([] { readEntireFile("nonexistent"); } ) },
    {"std::vector at",  ([] { vector<int> v; v.at(106); } ) },
    {"std::string at",  ([] { string s; s.at(106); } ) },
    {"gui segv",        ([] { GThread::runOnQtGuiThread([] { *gPtr = 106; } ); } ) },
    {"gui fpe",         ([] { GThread::runOnQtGuiThread([] { gZero = 106/gZero; } ); } ) },
    {"gui throw",       ([] { GThread::runOnQtGuiThread([] { throw 106; } ); } ) },
    {"gui error",       ([] { GThread::runOnQtGuiThread([] {  Vector<int> v; v[106]; } ); } ) },
    {"gui stack",       ([] { GThread::runOnQtGuiThread([] { stack_overflow(); } ); } ) },
};


PROVIDED_TEST("Create exception and see what happens")
{
  //  exceptions::setTopLevelExceptionHandlerEnabled(false);
   // exceptions::interruptIfDebug();

    int n = sizeof(faults)/sizeof(*faults);
    cout << " MENU " << endl;
    for (int i = 0; i < n; i++) {
        cout << i << " : " << faults[i].explain << endl;
    }
    int i = getIntegerBetween("Enter number: ", 0, n-1);
    cout << "Choice was: " << faults[i].explain << endl;
    faults[i].fn();
    cout << "This is after the fault, and mostly won't appear" << endl;
}
