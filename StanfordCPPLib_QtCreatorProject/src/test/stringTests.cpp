/*
 * Test file for verifying the Stanford C++ lib string lib functionality.
 */

#include "testcases.h"
#include "strlib.h"
#include <iostream>
#include <string>

void stringToIntegerTest() {
    int n1 = stringToInteger("234");
    int n2 = stringToInteger("234", 16);
    std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl;
}
