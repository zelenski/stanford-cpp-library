/**
 * TestDriver.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 */
#include "TestDriver.h"
#include <string>
using namespace std;

/* Why this type? Because we want to be able to
 *
 * 1. find all the tests in a certain group,
 * 2. have them be sorted by line number, and
 * 3. then have a name associated with each.
 */
map<TestKey, multimap<int, TestCase>>& gTestsMap() {
    static map<TestKey, multimap<int, TestCase>> result;
    return result;
}

void doFail(const string& message, size_t line, const char* filename) {
    throw TestFailedException(message, line, filename);
}
void doExpect(bool condition, const string &expression, size_t line, const char* filename) {
    if (!condition) {
        doFail(expression, line, filename);
    }
}

void reportFailure(const char* message, size_t line, const char* filename) {
    doFail(message, line, filename);
}

/* TestFailedException implementation. */
TestFailedException::TestFailedException(const string& message, std::size_t line, const char*)
    : logic_error("Line " + to_string(line) + ": " + string(message)) {

}

/* TestCaseAdder implementation. */
TestCaseAdder::TestCaseAdder(const TestKey& key, int line, const std::string& name, const std::string &owner, std::function<void()> callback) {
    gTestsMap()[key].insert(make_pair(line, (TestCase){name,line,owner,callback}));

}
