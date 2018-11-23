/*
 * Various support classes and types used in practice problems.
 *
 * @version 2015/08/10
 */

#define INTERNAL_INCLUDE 1
#include "types.h"
#undef INTERNAL_INCLUDE

Domino::Domino(int f, int s) {
    first = f;
    second = s;
}

ostream& operator <<(ostream& out, const Domino& d) {
    return out << "(" << d.first << ":" << d.second << ")";
}

istream& operator >>(istream& input, Domino& d) {
    char ch = '\0';
    input >> ch;   // (
    input >> ch;   // first
    d.first = ch - '0';
    input >> ch;   // :
    input >> ch;   // second
    d.second = ch - '0';
    input >> ch;   // )
    return input;
}
