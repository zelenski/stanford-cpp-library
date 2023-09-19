/*
 * Test file for verifying the Stanford C++ lib system functionality.
 */


#include "SimpleTest.h"
#include "ioutils.h"
#include <iomanip>
#include <iostream>
#include <string>


PROVIDED_TEST("expect equal, integer compare") {
    EXPECT_EQUAL(42, 42);
    EXPECT_EQUAL(0, 0);
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
    EXPECT(f != g); // values are not exact/bitwise equal
    EXPECT_EQUAL(f, g); // but are within fp epsilon
}

static double zero = 0;

PROVIDED_TEST("expect equal, inf should equal self ") {
    EXPECT_EQUAL(1/zero, 1/zero);
    EXPECT_EQUAL(-1/zero, 1/zero);
    EXPECT_EQUAL(-1/zero, -1/zero);
}

PROVIDED_TEST("nan is not equal to anything even itself (HEADS UP: test expected to fail)") {
    EXPECT(zero/zero != zero/zero);
    EXPECT_EQUAL(zero/zero, zero/zero);
}

PROVIDED_TEST("capture output, compare equal") {
    ioutils::captureStdoutBegin();
    std::cout << std::setprecision(4) << 12.3456789;
    std::string output = ioutils::captureStdoutEnd();
    EXPECT_EQUAL(output, "12.35");
}

PROVIDED_TEST("capture output, compare equal") {
    ioutils::captureStdoutBegin();
    std::cout << std::fixed << std::setprecision(2) << 12.3456789 << std::endl;
    std::string output = ioutils::captureStdoutEnd();
    EXPECT_EQUAL(output, "12.35\n");
}
