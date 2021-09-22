#include "testing/SimpleTest.h"
#include "strlib.h"
#include "set.h"
using namespace std;


// Testing Keith's cool feature to allow use of literal
// in second argument to EXPECT_EQUAL

PROVIDED_TEST("Literal string/char* as argument to EXPECT_EQUAL") {
    //EXPECT_EQUAL("A", charToString('A')); // this direction no longer works
    EXPECT_EQUAL(charToString('A'), "A");
    EXPECT_EQUAL(charToString('A'), "B");

}

PROVIDED_TEST("Literal Set as argument to EXPECT_EQUAL") {
    Set<string> one = {"A", "C"};
    Set<string> two = {"B", "C"};
    Set<string> sum = {"A", "B", "C"};

    EXPECT_EQUAL(one + two, sum);
    EXPECT_EQUAL(sum, one + two);

    EXPECT_EQUAL(one + two, {"A", "B", "C"});
    EXPECT_EQUAL(one + two, {"A", "B"});
    // Now literal is ok in second argument, but not first
    //EXPECT_EQUAL({"A", "B", "C"}, one + two);
}

template <typename T> T sum(T val, int n)
{
    T result = 0;
    for (int i = 0; i < n; i++) result += val;
    return result;
}

PROVIDED_TEST("float/double literals compared with ==") {
    // literals: float double
    EXPECT(0.1 != 0.1f); // not exact
    EXPECT(0.1f != 0.1); // not exact
}

PROVIDED_TEST("float/double literals compared using EXPECT_EQUAL, will fail") {
    EXPECT_EQUAL(0.1f, 0.1); // yes fpe, both forced to float
    EXPECT_EQUAL(0.1, 0.1f);  // yes fpe, both forced to double, val mismatch
}

PROVIDED_TEST("float literal compared with float op result") {
    float fsum = sum(0.1f, 10);

    EXPECT(fsum != 1.0f); // not exact
    EXPECT_EQUAL(fsum, 1.0f); // yes fpe, both forced to float
}

 PROVIDED_TEST("float op result compared with double literal and double op result") {
    float fsum = sum(0.1f, 10);
    double dsum = sum(0.1, 10);

    EXPECT(fsum != 1.0); // not exact
    EXPECT_EQUAL(fsum, 1.0); // yes fpe, both forced to float
    EXPECT(fsum != dsum); // not exact
    EXPECT_EQUAL(fsum, dsum); // yes fpe, both forced to float
}

PROVIDED_TEST("double literal compared with double op result") {
    double dsum = sum(0.1, 10);

    EXPECT(dsum != 1.0); // not exact
    EXPECT_EQUAL(dsum, 1.0); // yes fpe, both forced to double
}

PROVIDED_TEST("double op result compared with float literal and float op result, will fail") {
    float fsum = sum(0.1f, 10);
    double dsum = sum(0.1, 10);

    // double == float
    EXPECT(dsum != 1.0f); // not exact
    EXPECT_EQUAL(dsum, 1.0f); // yes fpe, both forced to double
    EXPECT(dsum != fsum); // not exact
    EXPECT_EQUAL(dsum, fsum); // yes fpe, both forced to double, val mismatch
}

PROVIDED_TEST("float op result compared with int literal") {
    float fsum = sum(0.1f, 10);
    EXPECT(fsum != 1); // not exact
    EXPECT_EQUAL(fsum, 1); //yes fpe, both forced to float
}

 PROVIDED_TEST("double op result compared with int literal") {
    double dsum = sum(0.1, 10);
    EXPECT(dsum != 1); // not exact
    EXPECT_EQUAL(dsum, 1); // yes fpe, both forced to double
}

// This will all fail
PROVIDED_TEST("Failure report for mismatch literal number") {
    EXPECT_EQUAL(3 + 5, 10);
}

PROVIDED_TEST("Failure report for mismatch literal string") {
    string str = "hello";
    EXPECT_EQUAL(str, "goodbye");
}

PROVIDED_TEST("Failure report for mismatch literal Set") {
    Set<int> s = {3, 4};
    EXPECT_EQUAL(s, {4, 5});
}

PROVIDED_TEST("Failure report for mismatch literal double") {
    double d = 0.1;
    EXPECT_EQUAL(d, 0.2);
}

PROVIDED_TEST("Failure report for mismatch literal number") {
    EXPECT_EQUAL(10, 5+9);
}

PROVIDED_TEST("Failure report for mismatch literal string") {
    EXPECT_EQUAL("hello", "goodbye");
}

PROVIDED_TEST("Failure report for mismatch literal double") {
    EXPECT_EQUAL(0.1, 0.2);
}

PROVIDED_TEST("Failure report for mismatch literal bool") {
    EXPECT_EQUAL(4 % 2 == 0, 0);
}