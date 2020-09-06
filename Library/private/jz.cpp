// This is a garbage file, here only to mess with things
// and verify how build handles

#include <string>
#include "jz.h"

int jz_test(std::string str)
{
    // empty
    int unused;
    int implicit_conversion = 4.5;
    int total; // uninitialized
    for (int i = 0; i < str.length(); i++)
        total += str[i];
    // no return
}
