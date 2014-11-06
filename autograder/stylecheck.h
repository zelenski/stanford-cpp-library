/*
 * File: stylecheck.h
 * ------------------
 * This file contains declarations of code to perform regex-based rough style
 * checking on C++ code.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/10/14
 */

#ifndef _stylecheck_h
#define _stylecheck_h

#include <string>

namespace stylecheck {
void styleCheck(std::string codeFileName, std::string styleXmlFileName = "stylecheck.xml", bool printWarning = true);
} // namespace stylecheck

#endif // _stylecheck_h
