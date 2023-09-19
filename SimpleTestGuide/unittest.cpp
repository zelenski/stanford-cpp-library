#include "SimpleTest.h"
#include "strlib.h"
#include "set.h"
using namespace std;


// Testing Keith's cool feature to allow use of literal
// in second argument to EXPECT_EQUAL

PROVIDED_TEST("EXPECT_EQUAL, string literal/char* ") {
    EXPECT_EQUAL("A", "A");
    EXPECT_EQUAL(string("A"), "A");
    EXPECT_EQUAL(charToString('A'), "A");
}

PROVIDED_TEST("EXPECT_EQUAL, set literal") {
    Set<string> one = {"A", "C"};
    Set<string> two = {"B", "C"};
    Set<string> sum = {"A", "B", "C"};

    EXPECT_EQUAL(one + two,  {"A", "B", "C"});
    EXPECT_EQUAL(sum, one + two);
}

PROVIDED_TEST("How FAILURE reported for mismatch set literal") {
    Set<string> one = {"A", "C"};
    Set<string> two = {"B", "C"};

    EXPECT_EQUAL(one + two, {"A", "C"}); // mismatch -> FAILURE
}

template <typename T> T sum(T val, int n) {
    T result = 0;
    for (int i = 0; i < n; i++) result += val;
    return result;
}

PROVIDED_TEST("== compare float/double literals, expect to not be exact") {
    // literals: float double
    EXPECT(0.1 != 0.1f); // expect not exact equal
}

PROVIDED_TEST("How FAILURE reported for mismatch type float/double literals") {
    EXPECT_EQUAL(0.1f, 0.1); // yes fpe, both forced to float
    EXPECT_EQUAL(0.1, 0.1f);  // yes fpe, both forced to double, val mismatch -> FAILURE
}

PROVIDED_TEST("float literal compared with float op result") {
    float fsum = sum(0.1f, 10);

    EXPECT(fsum != 1.0f); // expect not exact equal
    EXPECT_EQUAL(fsum, 1.0f); // yes fpe, both forced to float
}

 PROVIDED_TEST("float op result compared with double literal and double op result") {
    float fsum = sum(0.1f, 10);
    double dsum = sum(0.1, 10);

    EXPECT(fsum != 1.0); // expect not exact equal
    EXPECT_EQUAL(fsum, 1.0); // yes fpe, both forced to float
    EXPECT(fsum != dsum); // expect not exact equal
    EXPECT_EQUAL(fsum, dsum); // yes fpe, both forced to float
}

PROVIDED_TEST("double literal compared with double op result") {
    double dsum = sum(0.1, 10);

    EXPECT(dsum != 1.0); // expect not exact equal
    EXPECT_EQUAL(dsum, 1.0); // yes fpe, both forced to double
}

PROVIDED_TEST("How FAILURE reported for double op result compared with float literal and float op result") {
    float fsum = sum(0.1f, 10);
    double dsum = sum(0.1, 10);

    // double == float
    EXPECT(dsum != 1.0f); // expect not exact equal
    EXPECT_EQUAL(dsum, 1.0f); // yes fpe, both forced to double
    EXPECT(dsum != fsum); // expect not exact equal
    EXPECT_EQUAL(dsum, fsum); // yes fpe, both forced to double, val mismatch -> FAILURE
}

PROVIDED_TEST("float op result compared with int literal") {
    float fsum = sum(0.1f, 10);
    EXPECT(fsum != 1); // expect not exact equal
    EXPECT_EQUAL(fsum, 1); //yes fpe, both forced to float
}

 PROVIDED_TEST("double op result compared with int literal") {
    double dsum = sum(0.1, 10);
    EXPECT(dsum != 1); // expect not exact equal
    EXPECT_EQUAL(dsum, 1); // yes fpe, both forced to double
}

// This will all fail
PROVIDED_TEST("How FAILURE reported for mismatch literal number") {
    EXPECT_EQUAL(3 + 5, 10); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal string") {
    string str = "hello";
    EXPECT_EQUAL(str, "goodbye"); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal Set") {
    Set<int> s = {3, 4};
    EXPECT_EQUAL(s, {4, 5});
}

PROVIDED_TEST("How FAILURE reported for mismatch literal double") {
    double d = 0.1;
    EXPECT_EQUAL(d, 0.2); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal number") {
    EXPECT_EQUAL(10, 5+9); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal string") {
    EXPECT_EQUAL("hello", "goodbye"); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal double") {
    EXPECT_EQUAL(0.1, 0.2); // mismatch -> FAILURE
}

PROVIDED_TEST("How FAILURE reported for mismatch literal bool") {
    EXPECT_EQUAL(4 % 2 == 0, 0); // mismatch -> FAILURE
}

