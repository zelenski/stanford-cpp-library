/*
 * Various support classes and types used in practice problems.
 *
 * @version 2015/08/10
 */

#ifndef _types_h
#define _types_h

#include <iostream>
using namespace std;

struct Domino {
    int first;
    int second;
    
    Domino(int f = 0, int s = 0);
};

ostream& operator <<(ostream& out, const Domino& d);
istream& operator >>(istream& input, Domino& d);

#endif // _types_h
