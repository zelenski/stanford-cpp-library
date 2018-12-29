/*
 * File: unittestdetails.cpp
 * -------------------------
 * This file contains implementation of a structure of details about a test
 * that is passed through the Java back-end to the autograder GUI to display.
 * See unittestdetails.h for declarations and documentation.
 *
 * @author Marty Stepp
 * @version 2016/08/01
 * - added ASSERT_DIFF_IMAGE
 * @version 2014/11/24
 * - initial version
 * @since 2014/11/24
 */

#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#undef INTERNAL_INCLUDE

static std::string UNIT_TEST_TYPE_NAMES[14] = {
    "ASSERT_EQUALS",
    "ASSERT_NOT_EQUALS",
    "ASSERT_NEAR",
    "ASSERT_DIFF",
    "ASSERT_DIFF_IMAGE",
    "ASSERT_TRUE",
    "ASSERT_FALSE",
    "ASSERT_NOT_NULL",
    "ASSERT_NULL",
    "EXCEPTION",
    "NOT_EXCEPTION",
    "PASS",
    "FAIL",
    "STYLE_CHECK",
};

namespace stanfordcpplib {
namespace autograder {

UnitTestDetails::UnitTestDetails()
    : testType(autograder::UnitTestType::TEST_ASSERT_EQUALS),
      message(""),
      expected(""),
      student(""),
      passed(false),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
    // empty
}

UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 const std::string& exp,
                                 const std::string& stu,
                                 const std::string& vtype,
                                 int theDiffFlags,
                                 bool pass)
    : testType(tp),
      message(msg),
      expected(exp),
      student(stu),
      valueType(vtype),
      diffFlags(theDiffFlags),
      passed(pass),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
    expected = std::to_string(exp);
    student = std::to_string(stu);
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
      diffFlags(0),
      passed(pass),
      overwrite(false) {
    // empty
}
UnitTestDetails::UnitTestDetails(autograder::UnitTestType tp,
                                 const std::string& msg,
                                 void* exp,
                                 void* stu,
                                 const std::string& /*vtype*/,
                                 bool pass)
    : testType(tp),
      message(msg),
      valueType("pointer"),
      diffFlags(0),
      passed(pass),
      overwrite(false) {
    expected = std::to_string(exp);
    student = std::to_string(stu);
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
        << ",valueType=" << urlEncode(deets.valueType)
        << ",diffFlags=" << deets.diffFlags
        << ",passed=" << std::boolalpha << deets.passed
        << ",overwrite=" << std::boolalpha << deets.overwrite
        << "}";
    return out;
}

} // namespace autograder
} // namespace stanfordcpplib
