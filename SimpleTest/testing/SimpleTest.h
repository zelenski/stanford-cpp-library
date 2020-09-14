/**
 * File: SimpleTest.h
 *
 * @author Keith Schwarz
 * @version 2020/3/22
 *    Keith final revision from end of quarter 19-2
 * @version 20200/4/8
 *    Minor tweaks by Julie for quarter 19-3
 */
#pragma once


/*** Utility functions to express your test cases. ***/

/* Checks whether the given condition is true. If so, nothing happens. If the condition
 * is false, then the test fails. For example:
 *
 *     EXPECT(!vec.isEmpty());
 *     EXPECT(vec.size() < 10);
 */
#define EXPECT(condition) /* Use to confirm truth of a boolean expression. */

/* Checks whether two objects are equal. If so, nothing happens. If not, then the test fails
 * and reports what the two values were. The expectation is that the first value should be
 * the student's value and the second should be the correct answer. For example:
 *
 *     EXPECT_EQUAL(studentValue, referenceValue)
 */
#define EXPECT_EQUAL(student, reference) /* Use to confirm value is equal to expected. */

/* Checks whether the given expression called the error() handler. If so, nothing happens.
 * If not, then the test fails. You can use this to confirm that an expression that is
 * supposed to trigger an exception indeed does so. For example:
 *
 *     Vector<int> myVec;
 *     EXPECT_ERROR(myVec.get(0));    // Nothing happens, since myVec.get(0) does trigger error().
 *     EXPECT_ERROR(myVec.isEmpty()); // The test fails because no error would be generated here.
 */
#define EXPECT_ERROR(expression) /* Use to confirm an expected error is raised. */

/* Fails the given test, printing out the specified error message. For example:
 *
 *     REPORT_FAILURE("Something went wrong.");
 */
#define REPORT_FAILURE(message) /* Use message to explain the failure */

/* Time the evaluation of an expression report the time in seconds in test results.
 * The argument size is the size of the input.
 *
 * This syntax for adding a timed operation in a test case is:
 *
 *    TIME_OPERATION(myVector.size(), myVector.sort());
 */
 #define TIME_OPERATION(size, expression) /* Time the evaluation of expression */

/* Defines a new test case. You can write whatever code you want inside of the test case,
 * but you'll likely want to use EXPECT and EXPECT_EQUAL in your test cases, as they're
 * what actually perform tests.
 *
 * The syntax for defining a new test case is
 *
 *    ADD_TEST("Description of Test Case") {
 *       ... whatever code you'd like! ...
 *    }
 */
#define STUDENT_TEST(name) /* Add a new test case. */

/* We use this to define the test cases we provide to you.
 * You should not use this macro yourself.
 */
#define PROVIDED_TEST(name) /* For our use. */


/* Enumerated type for the different options when running tests. Your options are:
 *     NO_TESTS (and thus just continue to normal main function)
 *     ALL_TESTS (all tests from all files) or
 *     SELECTED_TESTS (to select which tests to run from a menu)
 */
enum Choice { NO_TESTS, ALL_TESTS, SELECTED_TESTS };

/* Enumerated type for where to report test results. Your options are:
 *     CONSOLE_ONLY (text output on console)
 *     WINDOW_ONLY (results displayed in separate window)
 *     CONSOLE_AND_WINDOW (both, this is the default)
 */
enum Where { CONSOLE_ONLY, WINDOW_ONLY, CONSOLE_AND_WINDOW };

/* Call this function from your main to run the desired tests.
 */
bool runSimpleTests(Choice ch, Where where = CONSOLE_AND_WINDOW);


#include "TestDriver.h"
