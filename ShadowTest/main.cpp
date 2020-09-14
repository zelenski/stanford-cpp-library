#include <iostream>
#include "console.h"
#include "strlib.h"
#include "strlib.h"
#include "vector.h"
#include "extra/mine.h"
#include "again.h"

using namespace std;

int main()
{
    Vector<int> v = {1, 2, 3, 4, 5};
    cout << "Hello, world!" << endl;
    cout << boolToString(false) << endl;
    cout << boolToString(1) << endl;
    cout << jzelenski(true) << endl;
    cout << missing(1) << endl;

    v.shuffle();
    cout << v << endl;
    cout << "All done, exiting" << endl;
    return 0;
}

