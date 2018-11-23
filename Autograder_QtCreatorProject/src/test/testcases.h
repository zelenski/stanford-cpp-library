#ifndef _testcases_h
#define _testcases_h

#include "assertions.h"

// default timeout in ms for test cases
#define TEST_TIMEOUT_DEFAULT 3000

// collection tests
template <typename T>
void compareTestHelper(const T& o1, const T& o2, std::string type = "",
                              int compareTo = 0) {
    assertEqualsBool(type + " " + o1.toString() + " <  " + o2.toString(), compareTo <  0,  o1 <  o2);
    assertEqualsBool(type + " " + o1.toString() + " <= " + o2.toString(), compareTo <= 0,  o1 <= o2);
    assertEqualsBool(type + " " + o1.toString() + " >  " + o2.toString(), compareTo >  0,  o1 >  o2);
    assertEqualsBool(type + " " + o1.toString() + " >= " + o2.toString(), compareTo >= 0,  o1 >= o2);
    assertEqualsBool(type + " " + o1.toString() + " == " + o2.toString(), compareTo == 0,  o1 == o2);
    assertEqualsBool(type + " " + o1.toString() + " != " + o2.toString(), compareTo != 0,  o1 != o2);
}

// exception tests
void exceptionTest();
void recursionIndentTest();
void segfaultTest(int sig = 0);
void stackOverflowTest(int n = 0);

// gui tests
void fileDialogTest();
void gbufferedImageTest();
void goptionpaneTest();
void gtableTest();
void radioButtonTest();

// pipe tests
void cinOutTest();
void coutCerrMixTest();
void getIntegerTest();
void killProcessTest();
void longStringTest();
void outputColorTest();

// string tests
void stringToIntegerTest();

// url tests
void urlstreamTest();

// server tests
void serverTest();

#endif
