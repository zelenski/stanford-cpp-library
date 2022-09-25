#include "console.h"
#include <string>
#include "SimpleTest.h"
using namespace std;

int main()
{
    if (runSimpleTests(SELECTED_TESTS))
        return 0;
    cout << "All done, exiting." << endl;
    return 0;
}
