#include "testing/SimpleTest.h"
#include <string>
#include "strlib.h"
using namespace std;


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Conversions between string and integer") {
    Vector<int> inA = {0, 1, 106, -55};
    Vector<string> outA = {"0", "1", "106", "-55"};

    for (int i = 0; i < inA.size(); i++) {
        EXPECT_EQUAL(integerToString(inA[i]), outA[i]);
    }

    Vector<string> inB = {"0", "77  ", "  333", "-4", "-59", "0123", "-045"};
    Vector<int> outB = {0, 77, 333, -4, -59, 123, -45};

    for (int i = 0; i < inB.size(); i++) {
         EXPECT_EQUAL(stringToInteger(inB[i]), outB[i]);
    }

    Vector<string> inError = {"apple", "", "-", "- 59", "123a", "--5", "2.5", "#1", "5 9"};
    for (auto& e: inError) {
          EXPECT_ERROR(stringToInteger(e));
     }
}
