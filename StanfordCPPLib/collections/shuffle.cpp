/*
 * File: shuffle.cpp
 * -----------------
 * Implementation of the functions in shuffle.h.
 * See shuffle.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/02/01
 */

#include "shuffle.h"

/*
 * Randomly rearranges the characters of the given string and returns the
 * rearranged version.
 */
std::string shuffle(std::string s) {
    for (int i = 0, length = s.length(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            std::string::value_type temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
    }
    return s;
}
