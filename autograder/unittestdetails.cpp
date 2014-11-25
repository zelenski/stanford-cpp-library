/*
 * File: unittestdetails.cpp
 * -------------------------
 * This file contains implementation of a structure of details about a test
 * that is passed through the Java back-end to the autograder GUI to display.
 * See unittestdetails.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2014/11/24
 * @since 2014/11/24
 */

#include "unittestdetails.h"

static std::string UNIT_TEST_TYPE_NAMES[11] = {
    "ASSERT_EQUALS",
    "ASSERT_NOT_EQUALS",
    "ASSERT_NEAR",
    "ASSERT_DIFF",
    "ASSERT_TRUE",
    "ASSERT_FALSE",
    "EXCEPTION",
    "NOT_EXCEPTION",
    "PASS",
    "FAIL",
    "STYLE_CHECK",
};

namespace autograder {

UnitTestDetails::UnitTestDetails()
    : testType(autograder::UnitTestType::TEST_ASSERT_EQUALS),
      message(""),
      expected(""),
      student(""),
      passed(false) {
    // empty
}

UnitTestDetails::UnitTestDetails(
        autograder::UnitTestType tp,
        const std::string& msg,
        bool pass)
    : testType(tp),
      message(msg),
      expected(""),
      student(""),
      passed(pass) {
    // empty
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 const std::string& exp,
                                 const std::string& stu,
                                 const std::string& vtype,
                                 bool pass)
    : testType(tp),
      message(msg),
      expected(exp),
      student(stu),
      valueType(vtype),
      passed(pass) {
    // empty
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 bool exp,
                                 bool stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      valueType("bool"),
      passed(pass) {
    expected = boolToString(exp);
    student = boolToString(stu);
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 char exp,
                                 char stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      valueType("char"),
      passed(pass) {
    expected = charToString(exp);
    student = charToString(stu);
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 double exp,
                                 double stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      valueType("double"),
      passed(pass) {
    expected = realToString(exp);
    student = realToString(stu);
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 int exp,
                                 int stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      valueType("int"),
      passed(pass) {
    expected = integerToString(exp);
    student = integerToString(stu);
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 const char* const exp,
                                 const char* const stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      expected(exp),
      student(stu),
      valueType("string"),
      passed(pass) {
    // empty
}

std::string UnitTestDetails::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const UnitTestDetails& deets) {
    out << "UnitTestDetails{"
        << "testType=" << UNIT_TEST_TYPE_NAMES[deets.testType]
        << ",message=" << urlEncode(deets.message)
        << ",expected=" << urlEncode(deets.expected)
        << ",student=" << urlEncode(deets.student)
        << ",valueType=" << deets.valueType
        << ",passed=" << std::boolalpha << deets.passed
        << "}";
    return out;
}

} // namespace autograder
