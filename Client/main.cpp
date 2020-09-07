#include <iostream>
#include "console.h"
#include "vector.h"
using namespace std;

int main()
{
    Vector<string> names = {"Leland", "Stanford", "Junior", "University"};
    cout << "Hello, world!" << endl;
    cout << "Copyright 2020 " << names << endl;
    return 0;
}
