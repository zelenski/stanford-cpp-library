#ifndef _testcases_h
#define _testcases_h

// collection tests
void compareTest();
void foreachTest();
void hashCodeTest();

// exception tests
void exceptionTest();
void segfaultTest(int sig = 0);
void stackOverflowTest(int n = 0);

// gui tests
void fileDialogTest();
void gbufferedImageTest();
void goptionpaneTest();

// pipe tests
void cinOutTest();
void coutCerrMixTest();
void longStringTest();

// url tests
void urlstreamTest();

#endif
