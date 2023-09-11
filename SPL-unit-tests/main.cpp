#include "console.h"
#include <string>
#include "SimpleTest.h"
using namespace std;

int main()
{
    if (runSimpleTests(SELECTED_TESTS, WINDOW_ONLY))
        return 0;
    cout << "All done, exiting." << endl;
    return 0;
}
