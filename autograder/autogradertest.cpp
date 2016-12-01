/*
 * File: autogradertest.cpp
 * ------------------------
 * This file contains the implementation of the AutograderTest class.
 * See autogradertest.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2016/10/04
 * - slight refactor of static variables
 * @version 2014/11/24
 * - initial version
 * @since 2014/11/24
 */

#include "autogradertest.h"
#include "autograder.h"
#include "private/platform.h"

namespace autograder {

int AutograderTest::TIMEOUT_MS_DEFAULT = 10000;
int AutograderTest::TIMEOUT_MS_MIN = 100;

static std::vector<std::string>& allCategories() {
    static std::vector<std::string> v;   // static OK
    return v;
}
std::map<std::string, std::vector<std::string> >& allTests() {
    static std::map<std::string, std::vector<std::string> > __allTests;   // static OK
    return __allTests;
}

void AutograderTest::addTestToList(const std::string& categoryName, const std::string& testName) {
    if (allTests().find(categoryName) == allTests().end()) {
        allCategories().push_back(categoryName);
    }
    allTests()[categoryName].push_back(testName);
}

const std::vector<std::string>& AutograderTest::getAllCategories() {
    return allCategories();
}

const std::vector<std::string>& AutograderTest::getAllTests(const std::string& categoryName) {
    return allTests()[categoryName];
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

std::string AutograderTest::getName() const {
    return this->name;
}

std::string AutograderTest::getCategory() const {
    return this->category;
}

std::string AutograderTest::getFullName() const {
    return this->category.empty() ? this->name : (this->category + "_" + this->name);
}

bool AutograderTest::shouldRun() {
    if (autograder::isGraphicalUI()) {
        return stanfordcpplib::getPlatform()->autograderunittest_isChecked(this->getFullName());
    } else {
        return true;
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
