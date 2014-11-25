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
#include "private/version.h"
using namespace std;

static const int TIMEOUT_DEFAULT = 2000;


TEST_CATEGORY(TestTest01, first category);

TIMED_TEST(TestTest01, test00_infinite_loop, TIMEOUT_DEFAULT/2) {
    while (1) { /* lolol */ }
}

TIMED_TEST(TestTest01, test01_ab, TIMEOUT_DEFAULT) {
    assertEquals("hi there", string("a"), string("a"));
}

TIMED_TEST(TestTest01, test02_abc_oops_real_long_name_gonnaFail, TIMEOUT_DEFAULT) {
    assertEquals("some failed test 1", string("abc"), string("oops"));
    assertEquals("some failed test 2", string("abc2"), string("oops2"));
    assertEquals("some later test that passes 1", string("same1"), string("same1"));
    assertEquals("some later test that passes 2", string("same2"), string("same2"));
}

TIMED_TEST(TestTest01, test03_near, TIMEOUT_DEFAULT) {
    // sleep(1);
    assertDoubleNear("hi there", 3.14, 3.1415, 0.1);
}

TIMED_TEST(TestTest01, test03b_infinite_loop, TIMEOUT_DEFAULT/2) {
    while (1) { /* lolol */ }
}


TEST_CATEGORY(TestTest02, "second category");

TIMED_TEST(TestTest02, test04_def, TIMEOUT_DEFAULT) {
    assertEquals("hi there", string("a"), string("a"));
}

TIMED_TEST(TestTest02, test05_infinite_loop, TIMEOUT_DEFAULT) {
    while (1) { /* lolol */ }
}

TIMED_TEST(TestTest02, test06_anotherOne, TIMEOUT_DEFAULT) {
    // sleep(2);
    string exp = "expected 1\nexpected 2\nexpected 3";
    string stu = "expected 1\nstudent 2 oops\nexpected 3";
    assertDiff("some diffs", exp, stu);
}


TEST_CATEGORY(TestTest03, "third category");

TIMED_TEST(TestTest03, test07_also_long_name_gonnaFail, TIMEOUT_DEFAULT) {
    assertEquals("some passing test early, should be replaced by fail later", string("good1"), string("good1"));
    assertEquals("some failed test", string("good1"), string("bad1"));
    assertEquals("some failed test", string("good2"), string("bad2"));
    assertEquals("some failed test", string("good3"), string("good3"));
}

TIMED_TEST(TestTest03, test08_moreTesting, TIMEOUT_DEFAULT) {
    string exp = "exp";
    string stu = "exp";
    assertDiff("a diff", exp, stu);
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
