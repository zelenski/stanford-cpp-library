/*
 * File: autogradertest.cpp
 * ------------------------
 * This file contains the implementation of the AutograderTest class.
 * See autogradertest.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#include "autogradertest.h"
#include "autograder.h"

namespace autograder {

int AutograderTest::TIMEOUT_MS_DEFAULT = 10000;
int AutograderTest::TIMEOUT_MS_MIN = 100;
Vector<std::string> AutograderTest::allCategories;
Map<std::string, Vector<std::string> > AutograderTest::allTests;

void AutograderTest::addTestToList(const std::string& categoryName, const std::string& testName) {
    if (!allTests.containsKey(categoryName)) {
        allCategories.add(categoryName);
    }
    allTests[categoryName] += testName;
}

const Vector<std::string>& AutograderTest::getAllCategories() {
    return allCategories;
}

const Vector<std::string>& AutograderTest::getAllTests(const std::string& categoryName) {
    return allTests[categoryName];
}

int AutograderTest::getDefaultTimeout() {
    return TIMEOUT_MS_DEFAULT;
}

void AutograderTest::setDefaultTimeout(int timeoutMS) {
    AutograderTest::TIMEOUT_MS_DEFAULT = timeoutMS;
}

int AutograderTest::getTestTimeout() const {
    return timeoutMS;
}

void AutograderTest::setTestTimeout(int ms) {
    if (timeoutMS > 0) {
        timeoutMS = std::max(TIMEOUT_MS_MIN, ms);
    } else {
        timeoutMS = ms;
    }
}

void AutograderTest::SetUp() {
    timeoutMS = TIMEOUT_MS_DEFAULT;
}
 
void AutograderTest::TearDown() {
    // empty
}

void AutograderTest::TestBody() {
    // empty; override me
}

void AutograderTest::TestRealBody() {
    // empty; override me
}

} // namespace autograder
