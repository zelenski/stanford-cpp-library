/*
 * File: stylecheck.cpp
 * --------------------
 * This file contains declarations of code to perform regex-based rough style
 * checking on C++ code.
 * See sylecheck.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2018/08/27
 * - refactored to use AutograderUnitTestGui cpp class
 * @version 2016/12/01
 * - fixed bugs with full test names to match 12/01 versions
 * @version 2016/10/28
 * - improved category merging on stylecheck when merged with autograder tests
 * @version 2016/10/22
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * - changed styleCheck function to return rather than crash on file-not-found
 * - fixed colons on merged style check category prefixes
 * @version 2016/10/08
 * - added setStyleCheckMergedWithUnitTests;
 *   ability to merge style checks with regular unit tests
 * @version 2014/11/15
 * - added warning description to top of window if 'omitOnPass' is turned on
 * @version 2014/10/31
 * - added support for graphical style checker output
 * @since 2014/10/14
 */

#define INTERNAL_INCLUDE 1
#include "stylecheck.h"
#include <cstring>
#define INTERNAL_INCLUDE 1
#include "autograder.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gtest-marty.h"
#define INTERNAL_INCLUDE 1
#include "rapidxml.h"
#define INTERNAL_INCLUDE 1
#include "regexpr.h"
#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "unittestdetails.h"
#define INTERNAL_INCLUDE 1
#include "xmlutils.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace stylecheck {

STATIC_CONST_VARIABLE_DECLARE(int, DEFAULT_MIN_COUNT, 0)
STATIC_CONST_VARIABLE_DECLARE(int, DEFAULT_MAX_COUNT, 999999999)
STATIC_VARIABLE_DECLARE(bool, styleChecksMerged, false)

static bool processPatternNode(const std::string& codeFileName,
                               rapidxml::xml_node<>* patternNode,
                               std::string categoryName,
                               const std::string& codeFileText,
                               bool omitOnPass) {
    std::ostringstream out;
    std::string patternRegex = xmlutils::getAttribute(patternNode, "regex");
    patternRegex = stringReplace(patternRegex, "(:IDENTIFIER:)", "(?:[a-zA-Z_$][a-zA-Z0-9_$]{0,255})");
    patternRegex = stringReplace(patternRegex, "(:IDENT:)", "(?:[a-zA-Z_$][a-zA-Z0-9_$]{0,255})");
    patternRegex = stringReplace(patternRegex, "(:SPACES:)", "(?:[ \\t]{0,999})");
    patternRegex = stringReplace(patternRegex, "(:SPACE:)", "(?:[ \\t])");
    patternRegex = stringReplace(patternRegex, "(:TEMPLATE:)", "(?:&lt;[ \t]{0,255}[a-zA-Z_$][a-zA-Z0-9_$]{0,255}[ \t]{0,255}&gt;)");

    std::string patternDescription = xmlutils::getAttribute(patternNode, "description", patternRegex);
    int patternMinCount = xmlutils::getAttributeInt(patternNode, "mincount", STATIC_VARIABLE(DEFAULT_MIN_COUNT));
    int patternMaxCount = xmlutils::getAttributeInt(patternNode, "maxcount", STATIC_VARIABLE(DEFAULT_MAX_COUNT));
    int patternCount = xmlutils::getAttributeInt(patternNode, "count", -1);
    if (patternCount != -1) {
        patternMinCount = patternMaxCount = patternCount;
    }
    bool patternList = xmlutils::getAttributeBool(patternNode, "list", true);
    bool showCounts = xmlutils::getAttributeBool(patternNode, "showcounts", true);
    
    stanfordcpplib::autograder::TestResult failResult =
            stanfordcpplib::autograder::TEST_RESULT_WARN;   // default
    if (xmlutils::hasAttribute(patternNode, "failtype")) {
        std::string failTypeStr = trim(xmlutils::getAttribute(patternNode, "failtype"));
        failResult = failTypeStr == "fail" ? stanfordcpplib::autograder::TEST_RESULT_FAIL
                                           : stanfordcpplib::autograder::TEST_RESULT_WARN;
    }

    // see if student's code text matches the regex
    std::string linesStr;
    int matchCount;
    if (patternList) {
        matchCount = regexMatchCountWithLines(codeFileText, patternRegex, linesStr);
    } else {
        matchCount = regexMatchCount(codeFileText, patternRegex);
    }
    
    std::string rangeStr = "";
    if (patternMinCount == patternMaxCount) {
        rangeStr = "should occur exactly " + std::to_string(patternMinCount) + " times";
    } else if (patternMinCount == 0 && patternMaxCount > 0 && patternMaxCount != STATIC_VARIABLE(DEFAULT_MAX_COUNT)) {
        rangeStr = "should occur <= " + std::to_string(patternMaxCount) + " times";
    } else if (patternMaxCount == STATIC_VARIABLE(DEFAULT_MAX_COUNT) && patternMinCount > 0) {
        rangeStr = "should occur >= " + std::to_string(patternMinCount) + " times";
    } else {
        rangeStr = "should be between " + std::to_string(patternMinCount) + "-" + std::to_string(patternMaxCount) + " times";
    }
    bool pass = true;
    if (patternMinCount > 0 && matchCount <= 0) {
        pass = false;
    } else if (patternMaxCount == 0 && matchCount > 0) {
        pass = false;
    } else if (!(patternMinCount <= matchCount && matchCount <= patternMaxCount)) {
        pass = false;
    }
    
    std::string prefix = "";
    std::string testName = patternDescription;
    if (STATIC_VARIABLE(styleChecksMerged)) {
        // merge categories; one "category" per file
        prefix += "Style Checker: " + codeFileName;
        if (!categoryName.empty()) {
            testName = "[" + categoryName + "] " + testName;
        }
        categoryName = "";
    } else {
        prefix += "[" + codeFileName + "] ";
        testName = prefix + patternDescription;
    }
    std::string testFullName = prefix + categoryName + "_" + testName;

    if (!pass || !omitOnPass) {
        out << "    STYLE CHECK " << (pass ? "PASSED : " : "WARNING: ") << patternDescription << std::endl;
        stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
        stanfordcpplib::autograder::TestResult result = pass
                ? stanfordcpplib::autograder::TEST_RESULT_PASS : failResult;
        if (!autograder->containsCategory(prefix + categoryName)) {
            autograder->addCategory(prefix + categoryName,
                                    "Note: the style checker does not check all aspects of the student's code, and it is merely offering suggestions based on code text patterns. A warning here does not necessarily mean that there should be a deduction, merely that there may be an issue to investigate.");
        }
        autograder->addTest(testName, prefix + categoryName);
        autograder->setTestResult(testFullName, result);
        stanfordcpplib::autograder::UnitTestDetails deets;
        deets.message = patternDescription;
        deets.passed = pass;
        deets.expected = rangeStr;
        deets.student = "actually occurs " + std::to_string(matchCount) + " time(s)";
        if (!linesStr.empty()) {
            deets.student += " on line " + linesStr;
        }
        deets.testType = stanfordcpplib::autograder::UnitTestType::TEST_STYLE_CHECK;
        deets.valueType = "T";
        deets.overwrite = true;
        deets.result = pass ? stanfordcpplib::autograder::TEST_RESULT_PASS
                            : stanfordcpplib::autograder::TEST_RESULT_WARN;
        out.str("");
        out << deets;
        autograder->setTestDetails(testFullName, deets);
        out.str("");

        if (showCounts) {
            out << "         " << rangeStr << std::endl;
            out << "         actually occurs " << matchCount << " time(s)";
            if (!linesStr.empty()) {
                out << " on line " << linesStr;
            }
            autograder->showOutput(out, /* graphical */ false, /* console */ true);
        }
    }
    
    return pass;
}

void setStyleCheckMergedWithUnitTests(bool merged) {
    STATIC_VARIABLE(styleChecksMerged) = merged;
}

bool isStyleCheckMergedWithUnitTests() {
    return STATIC_VARIABLE(styleChecksMerged);
}

/*
 * <stylecheck type="text" filename="life.cpp" omitonpass="true">
 *     <pattern regex="(\/\/.*)|(\/\*([^*]|([*][^\/])\r?\n?)*\*\/)" mincount="18" description="comments" list="false" />
 *      ...
 * </stylecheck>
 */
void styleCheck(const std::string& codeFileName, const std::string& styleXmlFileName, bool printWarning) {
    if (!fileExists(codeFileName)) {
        std::cerr << "Warning: file not found: " << codeFileName << std::endl;
        std::cerr << "Cannot perform style check. Aborting. " << std::endl;
        return;
    }

    std::string codeFileText = readEntireFile(codeFileName);
    rapidxml::xml_node<>* styleCheckNode = xmlutils::openXmlDocument(styleXmlFileName, "stylecheck");
    if (!styleCheckNode) {
        // file was not found or could not be parsed
        return;
    }

    bool omitOnPass = xmlutils::getAttributeBool(styleCheckNode, "omitonpass", true);

    std::ostringstream out;
    out << "STYLE CHECK for " << codeFileName << " based on rules in "
        << styleXmlFileName << ":" << std::endl;
    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->showOutput(out, /* graphical */ false, /* console */ true);
    if (printWarning) {
        out << "  (Note: These are just heuristics and suggestions, not hard rules." << std::endl;
        out << "         The style checker isn't able to check everything." << std::endl;
        out << "         Please look at the student's code and don't penalize them" << std::endl;
        out << "         unless you actually see a problem with their coding style.)" << std::endl;
        out << std::endl;
        if (omitOnPass) {
            out << "<br><br>(Note: Showing only the output of style checks that fail. Passing checks are omitted.)" << std::endl;
            out << std::endl;
        }
    }
    autograder->showOutput(out, /* graphical */ false, /* console */ true);

    if (!fileExists(codeFileName)) {
        out << "*** ERROR: Student code file \"" << codeFileName
            << "\" not found in build folder. Exiting." << std::endl;
        autograder->showOutput(out);
        return;
    }
    if (!fileExists(styleXmlFileName)) {
        out << "*** ERROR: XML style checklist file \"" << styleXmlFileName
            << "\" not found in build folder. Exiting." << std::endl;
        autograder->showOutput(out);
        return;
    }

    // loop over each pattern node
    // handle each pattern node embedded directly within the document element of 'stylecheck'
    int testCount = 0;
    int passCount = 0;
    for (rapidxml::xml_node<>* patternNode : xmlutils::getChildNodes(styleCheckNode, "pattern")) {
        testCount++;
        if (processPatternNode(codeFileName, patternNode, /* categoryName */ "", codeFileText, omitOnPass)) {
            passCount++;
        }
    }

    // handle each pattern node embedded within a 'category' node
    for (rapidxml::xml_node<>* categoryNode : xmlutils::getChildNodes(styleCheckNode, "category")) {
        std::string categoryName = xmlutils::getAttribute(categoryNode, "name");
        for (rapidxml::xml_node<>* patternNode : xmlutils::getChildNodes(categoryNode, "pattern")) {
            testCount++;
            if (processPatternNode(codeFileName, patternNode, categoryName, codeFileText, omitOnPass)) {
                passCount++;
            }
//            if (!STATIC_VARIABLE(styleChecksMerged)) {
//                autograder::setTestCounts(passCount, testCount, /* isStyleCheck */ true);
//            }
        }
    }

    out << "    STYLE CHECK: passed " << passCount << " of " << testCount << " checks." << std::endl;
    autograder->showOutput(out, /* graphical */ false, /* console */ true);
}
} // namespace stylecheck
