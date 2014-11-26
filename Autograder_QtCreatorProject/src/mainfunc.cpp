/*
 * Test file for verifying the Stanford C++ autograder lib functionality.
 */

#include <iostream>
#include <string>
#include "autograder.h"
#include "console.h"
#include "gtest-marty.h"
#include "platform.h"
#include "simpio.h"
#include "vector.h"
#include "private/version.h"
using namespace std;

static const int TIMEOUT_DEFAULT = 3000;


//TEST_CATEGORY(PassFailTests, "standard pass/fail tests");

//TIMED_TEST(PassFailTests, test_ints_PASS, TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 42);
//}

//TIMED_TEST(PassFailTests, test_ints_FAIL, TIMEOUT_DEFAULT) {
//    assertEquals("compare ints", 42, 69);
//}

//TIMED_TEST(PassFailTests, test_doubles_near_PASS, TIMEOUT_DEFAULT) {
//    assertDoubleNear("hi there", 3.14, 3.1415, 0.1);
//}

//TIMED_TEST(PassFailTests, test_doubles_near_FAIL, TIMEOUT_DEFAULT) {
//    assertDoubleNear("hi there", 3.14, 2.718, 0.1);
//}

//TIMED_TEST(PassFailTests, test_string_PASS, TIMEOUT_DEFAULT) {
//    assertEquals("hi there", string("a"), string("a"));
//}

//TIMED_TEST(PassFailTests, test_string_FAIL, TIMEOUT_DEFAULT) {
//    assertEquals("hi there", string("a"), string("b"));
//}

//TIMED_TEST(PassFailTests, test_cstring_PASS, TIMEOUT_DEFAULT) {
//    assertEqualsCString("hi there", "a", "a");
//}

//TIMED_TEST(PassFailTests, test_cstring_FAIL, TIMEOUT_DEFAULT) {
//    assertEqualsCString("hi there", "xxx", "yy");
//}

//TIMED_TEST(PassFailTests, test_char_PASS, TIMEOUT_DEFAULT) {
//    assertEquals("hi there", 'a', 'a');
//}

//TIMED_TEST(PassFailTests, test_char_FAIL, TIMEOUT_DEFAULT) {
//    assertEquals("hi there", 'a', 'b');
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_PASS, TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc2"), string("abc2"));
//    assertEquals("some test 3", 42, 42);
//    assertEquals("some test 4", 123, 123);
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_FAIL1, TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc"), string("def"));   // <-- should show this
//    assertEquals("some test 3", string("abc2"), string("def2"));
//}

//TIMED_TEST(PassFailTests, test_multiple_assertions_FAIL2, TIMEOUT_DEFAULT) {
//    assertEquals("some test 1", string("abc"), string("abc"));
//    assertEquals("some test 2", string("abc"), string("def"));   // <-- should show this
//    assertEquals("some test 3", string("abc2"), string("def2"));
//    assertEquals("some test 4", string("abc"), string("abc"));
//    assertEquals("some test 5", 42, 42);
//}

//TIMED_TEST(PassFailTests, test_reeeeeeeeeeeeeeally_long_name_FAIL, TIMEOUT_DEFAULT) {
//    assertEquals("some failed test 1", string("abc"), string("oops"));
//    assertEquals("some failed test 2", string("abc2"), string("oops2"));
//    assertEquals("some later test that passes 1", string("same1"), string("same1"));
//    assertEquals("some later test that passes 2", string("same2"), string("same2"));
//}


//TEST_CATEGORY(InfiniteLoops, "infinite loops");

//TIMED_TEST(InfiniteLoops, test_infinite_loop1, TIMEOUT_DEFAULT/2) {
//    while (1) { /* lolol */ }
//}


//TIMED_TEST(InfiniteLoops, test_infinite_loop2, TIMEOUT_DEFAULT) {
//    while (1) { sleep(1); }
//}

//TIMED_TEST(InfiniteLoops, test_notInfiniteButExceedsTimeout, TIMEOUT_DEFAULT) {
//    string exp = "a\nb\nc";
//    string stu = "a\nb\nc";
//    assertDiff("some diffs", exp, stu);
//    sleep(5);
//}


TEST_CATEGORY(Crashes, "crashes and exceptions");

TIMED_TEST(Crashes, test_exception, TIMEOUT_DEFAULT) {
    Vector<int> v;
    v[29] = 42;   // boom
    assertDiff("a diff", 42, 42);
}

TIMED_TEST(Crashes, test_sigfpe, TIMEOUT_DEFAULT) {
    int y = 0 + 0;
    int x = 1 / y;  // boom
    assertEquals("a diff", 42, x);
}

TIMED_TEST(Crashes, test_segfault, TIMEOUT_DEFAULT) {
    int* p = NULL;
    int x = *p;  // boom
    assertEquals("a diff", 42, x);
}

TEST_CATEGORY(PassFailTestsEnd, "standard pass/fail tests at end");

TIMED_TEST(PassFailTestsEnd, test_ints_again_PASS, TIMEOUT_DEFAULT) {
    assertEquals("compare ints", 42, 42);
}

TIMED_TEST(PassFailTestsEnd, test_ints_again_2_PASS, TIMEOUT_DEFAULT) {
    sleep(1);
    assertEquals("compare doubles", 42.0, 42.0);
}

TIMED_TEST(PassFailTestsEnd, test_ints_again_3_PASS, TIMEOUT_DEFAULT) {
    assertEquals("compare ints", 42, 42);
}

TIMED_TEST(PassFailTestsEnd, test_ints_again_4_FAIL, TIMEOUT_DEFAULT) {
    sleep(1);
    assertEquals("compare ints", 42, 43);
}

TIMED_TEST(PassFailTestsEnd, test_ints_again_5_PASS, TIMEOUT_DEFAULT) {
    assertEqualsCString("compare strings", "42", "42");
}


/*
 * Code that autograderMain() runs before it starts the autograding test cases.
 */
void before() {
    cout << "THIS IS BEFORE!" << endl;
    cout << "THIS IS BEFORE!" << endl;
    cout << "THIS IS BEFORE!" << endl;
}

/*
 * Code that autograderMain() runs after it ends the autograding test cases.
 */
void after() {
    cout << "THIS IS AFTER!" << endl;
    cout << "THIS IS AFTER!" << endl;
    cout << "THIS IS AFTER!" << endl;
}

void showMyInputTxt() {
    // empty
    autograder::showStudentTextFile("myinput.txt", /* maxWidth */ 75, /* maxHeight */ 30);
}

// this just needs to be here so that it will become studentMain()
int main() {
    cout << "Hello, world!  This is main." << endl;
    string line = getLine("Please type something: ");
    cout << "You typed: " << line << endl;
    return 0;
}

void autograderMain() {
    // autograder::setShowLateDays(true);    // this is the default
    version::ensureProjectVersion("2014/10/31");
    autograder::setAssignmentName("CS 106B Test");
    autograder::setAboutMessage("TEST 1 2 3 \nHWX assignment auto-grader by Marty Stepp");
    // autograder::setStudentProgramFileName("recursionproblems.cpp");
    autograder::setShowInputPanel(true, "inputpanel-mainfunc.xml");
    autograder::setGraphicalUI(true);
    autograder::setTestNameWidth(std::string("test02_abc_oops_real_long_name_gonnaFail").length());
    autograder::setStartMessage("my start message");
    autograder::styleCheckAddFile("mainfunc.cpp");

    autograder::addCallbackButton(showMyInputTxt, "View\nmyinput.txt", "textfile.gif");
    // autograder::setCallbackStart(before);
    // autograder::setCallbackEnd(after);
}
