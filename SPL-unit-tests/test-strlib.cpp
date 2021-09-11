/*
 * File: test-strlib.cpp
 * -------------------
 * Unit tests for public functions of strlib.h
 *
 * @author Cathy Zhang Spring 2020
 */

#include "testing/SimpleTest.h"
#include <string>
#include "strlib.h"
using namespace std;


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Conversions between bool and string") {
    /* Conversion from bool to string */
    EXPECT_EQUAL("true", boolToString(true));
    EXPECT_EQUAL("true", boolToString(1));
    EXPECT_EQUAL("false", boolToString(false));
    EXPECT_EQUAL("false", boolToString(0));

    /* Conversion from string to bool */
    EXPECT_EQUAL(true, stringToBool("true"));
    EXPECT_EQUAL(true, stringToBool("   true   "));
    EXPECT_EQUAL(true, stringToBool("1"));
//    EXPECT_EQUAL(true, stringToBool("  1 ")); // this test will cause an error

    EXPECT_EQUAL(false, stringToBool("false"));
    EXPECT_EQUAL(false, stringToBool("0"));

    EXPECT_ERROR(stringToBool("t r u e"));
    EXPECT_ERROR(stringToBool("falsey"));
    EXPECT_ERROR(stringToBool("001"));
}

PROVIDED_TEST("Conversions between char and string") {
    Vector<char> inA = {'A', 'a', '0', '?', '\t', '\n'};
    Vector<string> outA = {"A", "a", "0", "?", "\t", "\n"};

    for (int i = 0; i < inA.size(); i++) {
        EXPECT_EQUAL(charToString(inA[i]), outA[i]);
    }

    Vector<string> inB = {"A", "  A  ", "a", "0", "?"}; // "\t" and "\n" will cause an error
    Vector<char> outB = {'A', 'A', 'a', '0', '?'};

    for (int i = 0; i < inB.size(); i++) {
        EXPECT_EQUAL(stringToChar(inB[i]), outB[i]);
    }

    Vector<string> inError = {"", "ABC", "123"};
    for (auto& e: inError) {
        EXPECT_ERROR(stringToChar(e));
    }
}

PROVIDED_TEST("Checking whether specified string ends with specified suffix") {
    /* Suffix passed in as a string */
    EXPECT(endsWith("", ""));
    EXPECT(endsWith("cpp is cool", ""));
    EXPECT(endsWith("cpp is cool", "cpp is cool"));
    EXPECT(endsWith("cpp is cool", "cool"));
    EXPECT(endsWith("cpp is cool", "l"));
    EXPECT(endsWith("cpp is cool\n", "\n"));

    EXPECT(!endsWith("", "arbitrary"));
    EXPECT(!endsWith("string", "longer string"));
    EXPECT(!endsWith("cpp is cool", "cpp"));
    EXPECT(!endsWith("cpp is cool", "is"));

    /* Suffix passed in as a char */
    EXPECT(endsWith("cpp is cool", 'l'));
    EXPECT(endsWith("a", 'a'));
    EXPECT(endsWith("cpp is cool\t", '\t'));

    EXPECT(!endsWith("", 'a'));
    EXPECT(!endsWith("cpp is cool", 'c'));
    EXPECT(!endsWith("cpp is cool", 'i'));
}

PROVIDED_TEST("Checking equality ignoring case") {
    EXPECT(equalsIgnoreCase("", ""));
    EXPECT(equalsIgnoreCase("equ@lity", "equ@lity"));
    EXPECT(equalsIgnoreCase("equality", "EQUALITY"));
    EXPECT(equalsIgnoreCase("equal1TY", "eQuaL1ty"));

    EXPECT(!equalsIgnoreCase("", "\t \n"));
    EXPECT(!equalsIgnoreCase("equality", "ytilauqe"));
    EXPECT(!equalsIgnoreCase("equality", "inequality"));
}

PROVIDED_TEST("Conversions between string and integer") {
    Vector<int> inA = {0, 1, 106, -55};
    Vector<string> outA = {"0", "1", "106", "-55"};

    for (int i = 0; i < inA.size(); i++) {
        EXPECT_EQUAL(integerToString(inA[i]), outA[i]);
    }

    Vector<string> inB = {"0", "+1", "77  ", "  333", "-4", "-59", "0123", "-045"};
    Vector<int> outB = {0, 1, 77, 333, -4, -59, 123, -45};

    for (int i = 0; i < inB.size(); i++) {
        EXPECT_EQUAL(stringToInteger(inB[i]), outB[i]);
    }

    Vector<string> inError = {"apple", "", "-", "- 59", "123a", "--5", "2.5", "#1", "5 9"};
    for (auto& e: inError) {
        EXPECT_ERROR(stringToInteger(e));
    }

    /* With optional second parameter for radix */
    EXPECT_EQUAL(stringToInteger("234", 8), 156);
    EXPECT_ERROR(stringToInteger("234", -8));
}

PROVIDED_TEST("Conversions between string and real") {
    Vector<double> inA = {0, 0.0, 13.7, -81.9};
    Vector<string> outA = {"0", "0", "13.7", "-81.9"};

    for (int i = 0; i < inA.size(); i++) {
        EXPECT_EQUAL(realToString(inA[i]), outA[i]);
    }

    Vector<string> inB = {"0.0", "+13.7", "-81.9", "77  ", "  333", "0123.0", "-045"};
    Vector<double> outB = {0, 13.7, -81.9, 77, 333, 123.0, -45};

    for (int i = 0; i < inB.size(); i++) {
        EXPECT_EQUAL(stringToReal(inB[i]), outB[i]);
    }

    Vector<string> inError = {"apple", "", "\n", "- 59", "123a", "#1", "5 9"};
    for (auto& e: inError) {
        EXPECT_ERROR(stringToReal(e));
    }
}

PROVIDED_TEST("Checking whether specified string starts with specified prefix") {
    /* Prefix passed in as a string */
    EXPECT(startsWith("", ""));
    EXPECT(startsWith("cpp is cool", ""));
    EXPECT(startsWith("cpp is cool", "cpp is cool"));
    EXPECT(startsWith("cpp is cool", "cpp"));
    EXPECT(startsWith("cpp is cool", "c"));
    EXPECT(startsWith("\ncpp is cool", "\n"));

    EXPECT(!startsWith("", "arbitrary"));
    EXPECT(!startsWith("string", "longer string"));
    EXPECT(!startsWith("cpp is cool", "cool"));
    EXPECT(!startsWith("cpp is cool", "is"));

    /* Prefix passed in as a char */
    EXPECT(startsWith("cpp is cool", 'c'));
    EXPECT(startsWith("a", 'a'));
    EXPECT(startsWith("\tcpp is cool", '\t'));

    EXPECT(!startsWith("", 'a'));
    EXPECT(!startsWith("cpp is cool", 'l'));
    EXPECT(!startsWith("cpp is cool", 'i'));
}

PROVIDED_TEST("Checking whether specified string contains specified substring") {
    EXPECT(stringContains("", ""));
    EXPECT(stringContains("cpp is cool", ""));
    EXPECT(stringContains("cpp is cool", "cpp is cool"));
    EXPECT(stringContains("cpp is cool", "cpp"));
    EXPECT(stringContains("cpp is cool", "c"));
    EXPECT(stringContains("\ncpp is cool", "\n"));

    EXPECT(!stringContains("", "arbitrary"));
    EXPECT(!stringContains("string", "longer string"));
    EXPECT(!stringContains("cpp is cool", "cppcool"));
    EXPECT(!stringContains("cpp is cool", "looc si ppc"));
    EXPECT(!stringContains("cpp is cool", "c p p  i s  c o o l"));
}

PROVIDED_TEST("Getting the first and last indicies in specified string of specified substring ") {
    /* Finding the first occurrence */
    EXPECT_EQUAL(stringIndexOf("", ""), 0);
    EXPECT_EQUAL(stringIndexOf("cpp is cool", ""), 0);
    EXPECT_EQUAL(stringIndexOf("cpp is cool", "cpp"), 0);
    EXPECT_EQUAL(stringIndexOf("cpp is cool", "cool"), 7);
    EXPECT_EQUAL(stringIndexOf("cpp is cool", "cpp is cool"), 0);
    EXPECT_EQUAL(stringIndexOf("cpp is cool and cpp is cool", "cpp is cool"), 0);

    EXPECT_EQUAL(stringIndexOf("", "arbitrary"), -1);
    EXPECT_EQUAL(stringIndexOf("cpp is cool", "c is cooler"), -1);
    EXPECT_EQUAL(stringIndexOf("string", "longer string"), -1);

    /* Finding the last occurrence */
    EXPECT_EQUAL(stringLastIndexOf("", ""), 0);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool", ""), 11);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool", "cpp"), 0);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool", "cool"), 7);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool", "cpp is cool"), 0);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool and cpp is cool", "cpp is cool"), 16);

    EXPECT_EQUAL(stringLastIndexOf("", "arbitrary"), -1);
    EXPECT_EQUAL(stringLastIndexOf("cpp is cool", "c is cooler"), -1);
    EXPECT_EQUAL(stringLastIndexOf("string", "longer string"), -1);
}

PROVIDED_TEST("Checking valid conversion from string to integer") {
    Vector<string> inTrue = {"0", "+1", "77  ", "  333", "-59", "0123", "-045"};

    for (int i = 0; i < inTrue.size(); i++) {
        EXPECT(stringIsInteger(inTrue[i]));
    }

    Vector<string> inFalse = {"0.0", "13.7", "-81.9", "apple", "", "\n", "- 59", "123a", "#1", "5 9"};

    for (int i = 0; i < inFalse.size(); i++) {
        EXPECT(!stringIsInteger(inFalse[i]));
    }

    /* With optional second parameter for radix */
    EXPECT(stringIsInteger("234", 8));
    EXPECT_ERROR(stringIsInteger("234", -8));
}

PROVIDED_TEST("Checking valid conversion from string to real") {
    Vector<string> inTrue = {"0.0", "+13.7", "-81.9", "77  ", "  333", "0123.0", "-045"};

    for (int i = 0; i < inTrue.size(); i++) {
        EXPECT(stringIsReal(inTrue[i]));
    }

    Vector<string> inFalse = {"apple", "", "\n", "- 59", "123a", "#1", "5 9"};

    for (int i = 0; i < inFalse.size(); i++) {
        EXPECT(!stringIsReal(inFalse[i]));
    }
}

PROVIDED_TEST("Joining vector of strings with specified delimiter") {
    Vector<string> inA = {};
    Vector<string> inB = {"one"};
    Vector<string> inC = {"me", "myself", "i"};
    Vector<string> inD = {"there are", "7", "words in this", "vector!"};
    Vector<string> inE = {"", "\n", "", "\t", ""};

    EXPECT_EQUAL(stringJoin(inA, " "), "");
    EXPECT_EQUAL(stringJoin(inB, "\n"), "one");
    EXPECT_EQUAL(stringJoin(inC, ""), "memyselfi");
    EXPECT_EQUAL(stringJoin(inC, "AND"), "meANDmyselfANDi");
    EXPECT_EQUAL(stringJoin(inD, "!"), "there are!7!words in this!vector!");
    EXPECT_EQUAL(stringJoin(inE, "8"), "8\n88\t8");
    EXPECT_EQUAL(stringJoin(inE, "\n"), "\n\n\n\n\t\n");
}

PROVIDED_TEST("Replacing instances of a substring with a new substring") {
    EXPECT_EQUAL(stringReplace("", "dogs", "cats"), "");
    EXPECT_EQUAL(stringReplace("dogs", "dogs", "cats"), "cats");
    EXPECT_EQUAL(stringReplace("I love dogs", "dogs", "dogs"), "I love dogs");
    EXPECT_EQUAL(stringReplace("I love dogs", "dogs", "cats"), "I love cats");
    EXPECT_EQUAL(stringReplace("I love dogs", " ", "-"), "I-love-dogs");
    EXPECT_EQUAL(stringReplace("I love dogs\t", "\t", "\n"), "I love dogs\n");
    EXPECT_EQUAL(stringReplace("I love dogs", "I", "You"), "You love dogs");
    EXPECT_EQUAL(stringReplace("I love dogs", "I dislike dogs", "I like dogs"), "I love dogs");
}

PROVIDED_TEST("Splitting a string into a vector of strings given delimiter") {
    Vector<string> outA = {};
    Vector<string> outB = {""};
    Vector<string> outC = {"one"};
    Vector<string> outD = {"", "", ""};
    Vector<string> outE = {"me", "myself", "i"};
    Vector<string> outF = {"there are", "7", "words in this", "vector"};

    EXPECT_EQUAL(stringSplit("", " "), outA);
    EXPECT_EQUAL(stringSplit(" ", " "), outB);
    EXPECT_EQUAL(stringSplit("one\n", "\n"), outC);
    EXPECT_EQUAL(stringSplit("one", "I do not exist"), outC);
    EXPECT_EQUAL(stringSplit("mememe", "me"), outD);
    EXPECT_EQUAL(stringSplit("meANDmyselfANDi", "AND"), outE);
    EXPECT_EQUAL(stringSplit("there are!7!words in this!vector", "!"), outF);
}

PROVIDED_TEST("Conversions between uppercase and lowercase") {
    /* Note: Non-ASCII characters such as "ñ" and "σ" will not be converted to "Ñ" and "Σ" respectively,
     * and vice versa. Calling the functions toLowerCase(str) and toLowerCase(str) on strings containing
     * these characters will return the characters as is. */

    Vector<string> inA = {"a", "A", "scRAMbleD", ".?#@!$%&*^()[]{}-=+", "137", " \t\n"};
    Vector<string> outA = {"A", "A", "SCRAMBLED",".?#@!$%&*^()[]{}-=+", "137", " \t\n"};

    for (int i = 0; i < inA.size(); i++) {
        EXPECT_EQUAL(toUpperCase(inA[i]), outA[i]);
    }

    Vector<string> inB = {"a", "A", "scRAMbleD", ".?#@!$%&*^()[]{}-=+", "137", " \t\n"};
    Vector<string> outB = {"a", "a", "scrambled",".?#@!$%&*^()[]{}-=+", "137", " \t\n"};

    for (int i = 0; i < inB.size(); i++) {
        EXPECT_EQUAL(toLowerCase(inB[i]), outB[i]);
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

PROVIDED_TEST("Encoding and decoding URLs") {
    EXPECT_EQUAL(urlEncode(""), "");
    EXPECT_EQUAL(urlDecode(""), "");

    string toEncode = "Query0~ ?";
    string toDecode = "Query0~+%3F";
    EXPECT_EQUAL(urlEncode(toEncode), toDecode);
    EXPECT_EQUAL(urlDecode(toDecode), toEncode);

    EXPECT_ERROR(urlDecode("%0"));
    EXPECT_ERROR(urlDecode("%GG"));
    EXPECT_ERROR(urlDecode("\n"));
}
