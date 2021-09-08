/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include <string>
#include "functional.h"
#include "vector.h"
using namespace std;

void testFunctional();

int multimain(functional) {
    testFunctional();
    return 0;
}

bool is_even(int n) {
    return n % 2 == 0;
}

int add(int a, int b) {
    return a + b;
}

int squared(int n) {
    return n * n;
}

int len(string s) {
    return (int) s.length();
}

bool strLenLess(const string& s1, const string& s2) {
    return s1.length() < s2.length() ||
            (s1.length() == s2.length() && s1 < s2);
}

void testFunctional() {
    // TODO: fix (broken by recent PR)

    Vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2 = functional::filter(v, is_even);
    cout << "v  : " << v << endl;
    cout << "filter by is_even()" << endl;
    cout << "v2 : " << v2 << endl << endl;

    Vector<int> v3 = functional::map(v, squared);
    cout << "map squared()" << endl;
    cout << "v3 : " << v3 << endl << endl;

    Vector<string> names {"Cynthia", "Marty", "Ed", "", "Bob"};
    Vector<int> lengths;
    functional::map(names, len, lengths);
    cout << "names  : " << names << endl;
    cout << "map len()" << endl;
    cout << "lengths: " << lengths << endl << endl;

    int result = functional::reduce(v, add);
    cout << "reduce by add()" << endl;
    cout << "sum: " << result << endl;
}
