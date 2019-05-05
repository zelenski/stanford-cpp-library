/*
 * Test file for verifying the Stanford C++ autograder lib functionality.
 */

#include <iostream>
#include <string>
#include <vector>
#include "autograder.h"
#include "consoletext.h"
#include "gtest-marty.h"
#include "queue.h"
#include "simpio.h"
#include "stack.h"
#include "stl.h"
#include "stylecheck.h"
#include "vector.h"
#include "testcases.h"
#include "private/version.h"
#include "exceptions.h"
using namespace std;

//TEST_CATEGORY(PassFailTests, "standard pass/fail tests");

//void bar() {
//    // error("oooooops");
//    int* n = nullptr;
//    n[28583275] = 17;   // segfault
//}

//void foo() {
//    bar();
//}

//TIMED_TEST(PassFailTests, test_exception_after_easy_assert, TEST_TIMEOUT_DEFAULT) {
//    // assertTrue("Brussel sprouts are yummy", true);
//    foo();
//    // assertTrue("Pie is yummy", true);
//}


//TIMED_TEST(PassFailTests, test_diffs_strict_FAIL, TEST_TIMEOUT_DEFAULT) {
//    string expected = "Hello!\nhow are you?\n";
//    string student  = "hello!\nhow ARE you?\n";
//    assertDiffFlags("message goes here", expected, student, diff::IGNORE_NONE);
//}

//TIMED_TEST(PassFailTests, test_ints_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 42);
//}

//TIMED_TEST(PassFailTests, test_ints_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 69);
//}

//TIMED_TEST(PassFailTests, test_doubles_near_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertDoubleNear("hi there", 3.14, 3.1415, 0.1);
//}

//TIMED_TEST(PassFailTests, test_doubles_near_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertDoubleNear("hi there", 3.14, 2.718, 0.1);
//}

//TIMED_TEST(PassFailTests, test_string_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("hi there", string("a"), string("a"));
//}

//TIMED_TEST(PassFailTests, test_string_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("hi there", string("a"), string("b"));
//}

//TIMED_TEST(PassFailTests, test_cstring_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEqualsCString("hi there", "a", "a");
//}

//TIMED_TEST(PassFailTests, test_cstring_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertEqualsCString("hi there", "xxx", "yy");
//}

//TIMED_TEST(PassFailTests, test_char_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("hi there", 'a', 'a');
//}

//TIMED_TEST(PassFailTests, test_char_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("hi there", 'a', 'b');
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc2"), string("abc2"));
//    assertEquals("some test 3", 42, 42);
//    assertEquals("some test 4", 123, 123);
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_FAIL1, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc"), string("def"));   // <-- should show this
//    assertEquals("some test 3", string("abc2"), string("def2"));
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_FAIL2, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc"), string("def"));   // <-- should show this
//    assertEquals("some test 3", string("abc2"), string("def2"));
//    assertEquals("some test 4", string("abc"), string("abc"));
//    assertEquals("some test 5", 42, 42);
//}

//TIMED_TEST(PassFailTests, test_reeeeeeeeeeeeeeally_long_name_FAIL, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("some failed test 1", string("abc"), string("oops"));
//    assertEquals("some failed test 2", string("abc2"), string("oops2"));
//    assertEquals("some later test that passes 1", string("same1"), string("same1"));
//    assertEquals("some later test that passes 2", string("same2"), string("same2"));
//}


//TEST_CATEGORY(InfiniteLoops, "infinite loops");

//TIMED_TEST(InfiniteLoops, test_infinite_loop1, TEST_TIMEOUT_DEFAULT/2) {
//    while (1) { /* lolol */ }
//}


//TIMED_TEST(InfiniteLoops, test_infinite_loop2, TEST_TIMEOUT_DEFAULT) {
//    while (1) { sleep(1); }
//}

//TIMED_TEST(InfiniteLoops, test_notInfiniteButExceedsTimeout, TEST_TIMEOUT_DEFAULT) {
//    string exp = "a\nb\nc";
//    string stu = "a\nb\nc";
//    assertDiff("some diffs", exp, stu);
//    sleep(5);
//}


//TEST_CATEGORY(Crashes, "crashes and exceptions");

//TIMED_TEST(Crashes, test_exception, TEST_TIMEOUT_DEFAULT) {
//    Vector<int> v;
//    v[29] = 42;   // boom
//    assertDiff("a diff", 42, 42);
//}

//TIMED_TEST(Crashes, test_sigfpe, TEST_TIMEOUT_DEFAULT) {
//    int y = 0 + 0;
//    int x = 1 / y;  // boom
//    assertEquals("a diff", 42, x);
//}

//TIMED_TEST(Crashes, test_segfault, TEST_TIMEOUT_DEFAULT) {
//    int* p = NULL;
//    int x = *p;  // boom
//    assertEquals("a diff", 42, x);
//}

//TEST_CATEGORY(PassFailTestsEnd, "standard pass/fail tests at end");

//TIMED_TEST(PassFailTestsEnd, test_ints_again_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 42);
//}

//TIMED_TEST(PassFailTestsEnd, test_ints_again_2_PASS, TEST_TIMEOUT_DEFAULT) {
//    sleep(1);
//    assertEquals("compare doubles", 42.0, 42.0);
//}

//TIMED_TEST(PassFailTestsEnd, test_ints_again_3_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 42);
//}

//TIMED_TEST(PassFailTestsEnd, test_ints_again_4_FAIL, TEST_TIMEOUT_DEFAULT) {
//    sleep(1);
//    assertEquals("compare ints", 42, 43);
//}

//TIMED_TEST(PassFailTestsEnd, test_ints_again_5_PASS, TEST_TIMEOUT_DEFAULT) {
//    assertEqualsCString("compare strings", "42", "42");
//}

int f() {
    if (2 % 2 == 1) {
        return 42;
    } else {
        return 43;
    }
}

/*
 * A custom autograder UI button to show the student's creative input file.
 */
void showMyInputTxt() {
    // empty
    autograder::showStudentTextFile("myinput.txt", /* maxWidth */ 75, /* maxHeight */ 30);
}

// this just needs to be here so that it will become studentMain()
int main() {
    cout << "Hello, world!" << endl;
    cout << "f is: " << f() << endl;
    int x = getInteger("Type x: ");
    cout << "You typed " << x << endl;
    return 0;
}

void autograderMain() {
    // autograder::setAssignmentName("Stanford C++ Library Test");
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->setAboutMessage("Stanford C++ library tester by Marty Stepp");
    autograder->setGraphicalUI(true);
    autograder->setTestNameWidth(static_cast<int>(std::string("test02_abc_oops_real_long_name_gonnaFail").length()));
    autograder->setStartMessage("my <b>COOL</b> start <i>message</i> yay! "
                                "lorem ipsum lorem ipsum lorem ipsum lorem ipsum lorem ipsum "
                                "lorem ipsum lorem ipsum lorem ipsum lorem ipsum lorem ipsum ");

    //stylecheck::setStyleCheckMergedWithUnitTests(true);
    //autograder::styleCheckAddFile("mainfunc.cpp");
    autograder::addCallbackButton(showMyInputTxt, "View\nmyinput.txt", "textfile.gif");
}
