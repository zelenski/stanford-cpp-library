/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include "console.h"

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))


int multimain(3) {
    std::cout << "Yay this is multimain #3!!!" << std::endl;

    int nums[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::cout << "Array length is " << ARRAY_LENGTH(nums) << std::endl;

    return 0;
}
