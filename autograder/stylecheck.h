/*
 * File: stylecheck.h
 * ------------------
 * This file contains declarations of code to perform regex-based rough style
 * checking on C++ code.
 * 
 * @author Marty Stepp
 * @version 2016/10/08
 * - added setStyleCheckMergedWithUnitTests;
 *   ability to merge style checks with regular unit tests
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#ifndef _stylecheck_h
#define _stylecheck_h

#include <string>

namespace stylecheck {
bool isStyleCheckMergedWithUnitTests();
void setStyleCheckMergedWithUnitTests(bool merged = true);
void styleCheck(const std::string& codeFileName, const std::string& styleXmlFileName = "stylecheck.xml", bool printWarning = true);
} // namespace stylecheck

#endif // _stylecheck_h
