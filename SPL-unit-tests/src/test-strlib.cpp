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

PROVIDED_TEST("Trimming whitespace characters") {
    /* Trim from beginning and end */
    EXPECT_EQUAL(trim(""), "");
    EXPECT_EQUAL(trim(" "), "");
    EXPECT_EQUAL(trim("\ttab\t"), "tab");
    EXPECT_EQUAL(trim("newline\n"), "newline");
    EXPECT_EQUAL(trim("\t\n"), "");

    EXPECT_EQUAL(trim(" leading"), "leading");
    EXPECT_EQUAL(trim("trailing "), "trailing");
    EXPECT_EQUAL(trim("   multiple leading and trailing  "), "multiple leading and trailing");
    EXPECT_EQUAL(trim(" inner  space "), "inner  space");

    /* Trim from beginning */
    EXPECT_EQUAL(trimStart(""), "");
    EXPECT_EQUAL(trimStart(" "), "");
    EXPECT_EQUAL(trimStart("\ttab"), "tab");
    EXPECT_EQUAL(trimStart("\nnewline"), "newline");
    EXPECT_EQUAL(trimStart("\t\n"), "");

    EXPECT_EQUAL(trimStart(" leading"), "leading");
    EXPECT_EQUAL(trimStart("trailing "), "trailing ");
    EXPECT_EQUAL(trimStart("   multiple leading and trailing  "), "multiple leading and trailing  ");
    EXPECT_EQUAL(trimStart(" inner  space"), "inner  space");

    /* Trim from end */
    EXPECT_EQUAL(trimEnd(""), "");
    EXPECT_EQUAL(trimEnd(" "), "");
    EXPECT_EQUAL(trimEnd("tab\t"), "tab");
    EXPECT_EQUAL(trimEnd("newline\n"), "newline");
    EXPECT_EQUAL(trimEnd("\t\n"), "");

    EXPECT_EQUAL(trimEnd(" leading"), " leading");
    EXPECT_EQUAL(trimEnd("trailing "), "trailing");
    EXPECT_EQUAL(trimEnd("   multiple leading and trailing  "), "   multiple leading and trailing");
    EXPECT_EQUAL(trimEnd("inner  space "), "inner  space");
}
