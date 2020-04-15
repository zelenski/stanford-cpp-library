/**
 * File: TestDriver.h
 *
 * @author Keith Schwarz
 * @version 2020/3/22
 *    Keith final revision from end of quarter 19-2
 * @version 20200/4/8
 *    Minor tweaks by Julie & Nick for quarter 19-3
 */
#pragma once

#include "TextUtils.h"
#include "error.h"
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "filelib.h"

/* Type representing a key in the top-level testing map. We use the file basename
 * as the top-level grouping.
 */
using TestKey = std::string;

/* Type representing a test case, including name, owner (student or provided),
 * line number, and the callback function to execute.
 */
struct TestCase {
    std::string name;
    int line;
    std::string owner;
    std::function<void()> callback;
};

/* The top-level tests map. This is defined as a function rather than a static variable
 * to ensure that it plays well with static initializers.
 */
std::map<TestKey, std::multimap<int, TestCase>>& gTestsMap();

/* Exception type signifying that a test case failed. */
class TestFailedException: public std::logic_error {
public:
    TestFailedException(const std::string &message, std::size_t line, const char* filename);
};

/* Object whose sole purpose is to put a test case into a group of tests. */
class TestCaseAdder {
public:
    TestCaseAdder(const TestKey& key, int lineNumber, const std::string& name, const std::string &owner, std::function<void()>);
};

/**** Defines the macro that adds a new test case. ****/

/* First, undefine STUDENT_TEST, since we defined it above as a way of "prototyping" it. */
#undef STUDENT_TEST
#undef PROVIDED_TEST

/* We need several levels of indirection here because of how the preprocessor works.
 * This first layer expands out to the skeleton of what we want.
 */
#define STUDENT_TEST(name) DO_ADD_TEST(_testCase, _adder, name, __LINE__, "student")
#define PROVIDED_TEST(name) DO_ADD_TEST(_testCase, _adder, name, __LINE__, "provided")

/* This level of indirection exists so that line will be expanded to __LINE__ and
 * from there to the true line number. We still can't token-paste it here, since
 * the even further level of indirection.
 */
#define DO_ADD_TEST(fn, adder, name, line, owner)\
    static void JOIN(fn, line)();\
    static TestCaseAdder JOIN(adder, line)(getTail(__FILE__), line, name, owner, JOIN(fn, line));\
    static void JOIN(fn, line)()

#define JOIN(X, Y) X##Y

/***** Macros used to implement testing primitives. *****/
void doFail(const std::string &message, std::size_t line, const char* filename);

#undef EXPECT
#define EXPECT(condition) doExpect(condition, "EXPECT failed: " #condition " is not true.", __LINE__, __FILE__)
void doExpect(bool condition, const std::string& expression, std::size_t line, const char* filename);

#undef EXPECT_ERROR
#define EXPECT_ERROR(condition) do {\
    try {\
        (void)(condition); \
        doFail("EXPECT_ERROR: " #condition " did not call error().", __LINE__, __FILE__); \
    } catch (const ErrorException& ) { \
        /* Do nothing. */ \
    }\
} while(0)

/* * * * Pretty-Printers for EXPECT_EQUAL * * * */
inline std::string debugFriendlyString(char c) {
    return quotedVersionOf(c);
}

/* Print strings in quotes. */
inline std::string debugFriendlyString(const std::string& s) {
    return quotedVersionOf(s);
}
inline std::string debugFriendlyString(const char* s) {
    return debugFriendlyString(std::string(s));
}
template <size_t N> std::string debugFriendlyString(char (&s)[N]) {
    return debugFriendlyString((const char *)s);
}

/* Print bools as strings. */
inline std::string debugFriendlyString(bool value) {
    return value? "true" : "false";
}

/* Print nullptr as such. */
inline std::string debugFriendlyString(std::nullptr_t) {
    return "nullptr";
}

template <typename T>
inline std::string debugFriendlyString(T* ptr) {
    /* Print null pointers as such. */
    if (ptr == nullptr) return "nullptr";

    std::ostringstream converter;
    converter << "Memory address " << std::hex << ptr;
    return converter.str();
}

/* Print everything else using stream insertion. */
template <typename T> std::string debugFriendlyString(const T& value) {
    std::ostringstream result;
    result << value;
    return result.str();
}

/* * * * Equality comparisons * * * */

/* Equality comparisons are fuzzy for real numbers. */
inline bool _areEqual(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 0.001;
}

template <typename T1, typename T2> bool _areEqual(const T1& lhs, const T2& rhs) {
    return lhs == rhs;
}

#undef EXPECT_EQUAL
#define EXPECT_EQUAL(student, reference)                                                    \
    do {                                                                                    \
       auto _studentAnswer   = (student);                                                   \
       auto _referenceAnswer = (reference);                                                 \
                                                                                            \
       std::stringstream _expression;                                                       \
       _expression << std::boolalpha << "EXPECT_EQUAL failed: "                             \
                   << #student << " != " << #reference "\n"                                 \
                   << "                " #student   " = "                                   \
                   << debugFriendlyString(_studentAnswer) << '\n'                           \
                   << "                " #reference " = "                                   \
                   << debugFriendlyString(_referenceAnswer);                                \
       doExpect(_areEqual(_studentAnswer, _referenceAnswer), _expression.str(), __LINE__, __FILE__); \
    } while (0)

#undef REPORT_FAILURE
#define REPORT_FAILURE(message) reportFailure(message, __LINE__, __FILE__)
void reportFailure(const char* message, std::size_t line, const char* filename);

