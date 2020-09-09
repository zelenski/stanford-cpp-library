#include <iostream>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include <qwindowdefs.h>
using namespace std;

int main()
{
    Vector<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Copyright 2020 " << names << endl;
    string name = getLine("What is your name?");
    cout << "Hello, " << name << "!" << endl;
    return 0;
}

