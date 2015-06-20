#ifndef _testcases_h
#define _testcases_h

// collection tests
void compareTest();
void foreachTest();
void hashCodeTest();
void randomElementTest();

// exception tests
void exceptionTest();
void recursionIndentTest();
void segfaultTest(int sig = 0);
void stackOverflowTest(int n = 0);

// gui tests
void fileDialogTest();
void gbufferedImageTest();
void goptionpaneTest();
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

#endif
