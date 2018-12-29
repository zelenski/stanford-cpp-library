/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include <iomanip>
#include "console.h"

int multimain(2) {
    std::cout << "Yay this is main 2!" << std::endl;
    for (double d = 10000; d <= 2000000; ++d) {
        std::cout << std::fixed << d << std::endl;
    }
    return 0;
}
