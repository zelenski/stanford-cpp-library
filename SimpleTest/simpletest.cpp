#include "testing/SimpleTest.h"
#include "strlib.h"
#include "set.h"
using namespace std;

// Testing Keith's cool feature to allow use of literal
// in second argument to EXPECT_EQUAL

PROVIDED_TEST("Literal string/char* as argument to EXPECT_EQUAL") {
    EXPECT_EQUAL("A", charToString('A'));
    EXPECT_EQUAL(charToString('A'), "A");
    //EXPECT_EQUAL_K("A", charToString('A')); // this direction doesn't work
    EXPECT_EQUAL_K(charToString('A'), "A");
}

PROVIDED_TEST("Literal Set as argument to EXPECT_EQUAL") {
    Set<string> one = {"A", "C"};
    Set<string> two = {"B", "C"};
    Set<string> sum = {"A", "B", "C"};

    EXPECT_EQUAL(one + two, sum);
    EXPECT_EQUAL(sum, one + two);

    // Previously literal did not parse at all in either argument
    // EXPECT_EQUAL(one + two, {"A", "B", "C"});
    //EXPECT_EQUAL({"A", "B", "C"}, one + two);

    // Now literal is ok in second argument, but not first
    EXPECT_EQUAL_K(one + two, {"A", "B", "C"});
    //EXPECT_EQUAL_K({"A", "B", "C"}, one + two);
}

template <typename T> T sum(T val, int n)
{
    T result = 0;
    for (int i = 0; i < n; i++) result += val;
    return result;
}

PROVIDED_TEST("float/double equality") {
    float fsum = sum(0.1f, 10);
    double dsum = sum(0.1, 10);

    // literals: float double
    EXPECT(0.1 != 0.1f); // not exact
    EXPECT(0.1f != 0.1); // not exact
    //EXPECT(floatingPointEqual(0.1f, 0.1)); // ambiguous
    //EXPECT(floatingPointEqual(0.1, 0.1f)); // ambiguous
    //EXPECT_EQUAL(0.1f, 0.1); // no fpe, raw==
    EXPECT_EQUAL_K(0.1f, 0.1); // yes fpe, both forced to float
    //EXPECT_EQUAL(0.1, 0.1f);  // no fpe, raw==
    //EXPECT_EQUAL_K(0.1, 0.1f);  // yes fpe, both forced to double, val mismatch

    // float == float
    EXPECT(fsum != 1.0f); // not exact
    EXPECT(floatingPointEqual(fsum, 1.0f)); // within epsilon
    EXPECT_EQUAL(fsum, 1.0f); // yes fpe, both are float
    EXPECT_EQUAL_K(fsum, 1.0f); // yes fpe, both forced to float

    // float == double
    EXPECT(fsum != 1.0); // not exact
    //EXPECT(floatingPointEqual(fsum, 1.0)); // ambiguous
    //EXPECT_EQUAL(fsum, 1.0); // no fpe, raw==
    EXPECT_EQUAL_K(fsum, 1.0); // yes fpe, both forced to float
    EXPECT(fsum != dsum); // not exact
    //EXPECT(floatingPointEqual(fsum, dsum)); // ambiguous
    //EXPECT_EQUAL(fsum, dsum); // no fpe, raw==
    EXPECT_EQUAL_K(fsum, dsum); // yes fpe, both forced to float

    // double == double
    EXPECT(dsum != 1.0); // not exact
    EXPECT(floatingPointEqual(dsum, 1.0)); // within epsilon
    EXPECT_EQUAL(dsum, 1.0); // yes fpe, both are double
    EXPECT_EQUAL_K(dsum, 1.0); // yes fpe, both forced to double

    // double == float
    EXPECT(dsum != 1.0f); // not exact
    //EXPECT(floatingPointEqual(dsum, 1.0f)); // ambiguous
    //EXPECT_EQUAL(dsum, 1.0f); // no fpe, raw==
    EXPECT_EQUAL_K(dsum, 1.0f); // yes fpe, both forced to double
    EXPECT(dsum != fsum); // not exact
    //EXPECT(floatingPointEqual(dsum, fsum)); // ambiguous
    //EXPECT_EQUAL(dsum, fsum); // no fpe, raw==
    //EXPECT_EQUAL_K(dsum, fsum); // yes fpe, both forced to double, val mismatch

    // float == int
    EXPECT(fsum != 1); // not exact
    EXPECT(floatingPointEqual(fsum, 1));
    //EXPECT_EQUAL(fsum, 1); // no fpe, raw==
    EXPECT_EQUAL_K(fsum, 1); //yes fpe, both forced to float

    // double == int
    EXPECT(dsum != 1); // not exact
    EXPECT(floatingPointEqual(dsum, 1)); // ambiguous
    //EXPECT_EQUAL(dsum, 1); // no fpe, raw==
    EXPECT_EQUAL_K(dsum, 1); // yes fpe, both forced to double
}

