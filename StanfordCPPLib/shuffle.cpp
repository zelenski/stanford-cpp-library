#include "shuffle.h"
using namespace std;

/*
 * Randomly rearranges the characters of the given string and returns the
 * rearranged version.
 */
string shuffle(string s) {
    for (int i = 0, length = s.length(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            string::value_type temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
    }
    return s;
}
