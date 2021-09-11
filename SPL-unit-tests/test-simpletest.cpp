/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */


#include "common.h"
#include "testing/SimpleTest.h"
#include "ioutils.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <testing/SimpleTest.h>



PROVIDED_TEST("expect equal, should fail 42 not 17") {
    EXPECT_EQUAL( 42, 17);
    EXPECT_EQUAL( 42, 42);
}

PROVIDED_TEST("expect equal, should fail 42 not 17") {
    EXPECT_EQUAL( 42, 42);
    EXPECT_EQUAL( 42, 17);
}

PROVIDED_TEST("expect equal, pointer compare") {
    int* p1 = new int(42);
    int* p2 = p1;
    EXPECT_EQUAL(p1, p2);
}

PROVIDED_TEST("expect equal, pointer compare") {
    int* p1 = new int(42);
    int* p2 = new int(42);
    EXPECT(p1 != p2);
}

PROVIDED_TEST("expect equal, float") {
    float f = 0.3;
    float g = 0.1;
    g *= 3;
    EXPECT_EQUAL(f, g);
}

PROVIDED_TEST("expect equal, double") {
    double f = 0.3;
    double g = 0.1;
    g *= 3;
    EXPECT_EQUAL(f, g);
}

PROVIDED_TEST("expect equal, mixed float/double") {
    float f = 0.3;
    double g = 0.1;
    g *= 3;
    EXPECT_EQUAL(f, g);
}

PROVIDED_TEST("capture output, compare equal") {
    ioutils::captureStdoutBegin();
    std::cout << 12.3456789 << std::endl;
    std::string output = ioutils::captureStdoutEnd();
    EXPECT_EQUAL( "12.3457\n", output);
}

PROVIDED_TEST("capture output, compare equal") {
    ioutils::captureStdoutBegin();
    std::cout << std::fixed << std::setprecision(2) << 12.3456789 << std::endl;
    std::string output = ioutils::captureStdoutEnd();
    EXPECT_EQUAL( "12.35\n", output);
}
