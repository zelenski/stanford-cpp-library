#include <string>
#include "SimpleTest.h"
#include "vector.h"
using namespace std;

// reversed(str) returns copy of str with characters in reverse order.
string reversed(string s) {
    string result;
    for (int i = s.length() - 1; i >= 0; i--) {
        result += s[i];
    }
    return result;
}

bool isPalindrome(string s) {
    return s == reversed(s);
}

bool isEven(int n) {
    return n % 2 == 0;
}

bool inSortedOrder(Vector<int>& v)
{
    for (int i = 1; i < v.size(); i++)
        if (v[i] < v[i-1]) return false;
    return true;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Demonstrate different SimpleTest use case") {
    EXPECT_EQUAL(reversed("but"), "tub");
    EXPECT_EQUAL(reversed("stanford"), "drofnats");
}

STUDENT_TEST("test additional cases not covered by the provided tests") {
    EXPECT_EQUAL(reversed("racecar"), "racecar");
    EXPECT_EQUAL(reversed(""), "");
    EXPECT_EQUAL(reversed("123456789"), "987654321");
}

STUDENT_TEST("test my isEven function") {
    EXPECT(isPalindrome("racecar"));
    EXPECT(!isPalindrome("stanford"));
}

STUDENT_TEST("test my isPalindrome function") {
    EXPECT(isPalindrome("racecar"));
    EXPECT(!isPalindrome("stanford"));
}

STUDENT_TEST("Time operation vector sort on tiny input")
{
    Vector<int> v = {3, 7, 2, 45, 2, 6, 3, 56, 12};
    TIME_OPERATION(v.size(), v.sort());
}

STUDENT_TEST("Time operation vector sort over a range of input sizes")
{
    for (int size = 25000; size < 20000000; size *= 4) {
        Vector<int> v;
        for (int i = 0; i < size; i++) {
            v.add(randomInteger(1, 1000)); // fill vector with random values
        }
        TIME_OPERATION(v.size(), v.sort());
    }
}

STUDENT_TEST("Time operation vector sort on tiny input and verify is sorted")
{
    Vector<int> v = {3, 7, 2, 45, 2, 6, 3, 56, 12};
    TIME_OPERATION(v.size(), v.sort());
    EXPECT(inSortedOrder(v));
}

STUDENT_TEST("confirm isEven() works correctly for even number 4") {
    EXPECT(isEven(4));
}

STUDENT_TEST("confirm isEven() works correctly for odd number 3") {
    EXPECT(isEven(3)); // this "bogus" test will fail
}

STUDENT_TEST("confirm isEven() works correctly for odd number 3") {
    EXPECT(!isEven(3));
}
