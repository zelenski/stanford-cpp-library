/*
 * File: version.cpp
 * -----------------
 * This file implements functions for checking the version number of the
 * Stanford C++ library, its Java back-end, and the project properties
 * file (.pro).
 *
 * @author Marty Stepp 
 * @version 2016/09/24
 * - bug fix for std::quick_exit function
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/11/13
 * - ifdef'd out macroIntegerToDate function to remove unused function warning on some platforms
 * @since 2014/10/31
 */

#define INTERNAL_INCLUDE 1
#include "private/version.h"
#undef INTERNAL_INCLUDE
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace version {
#ifdef SPL_PROJECT_VERSION
/*
 * converts an 8-digit integer into a YYYY/MM/DD date string,
 * e.g. 20140907 -> "2014/09/07"
 * needed because I can't have a string as a -D command-line defined flag
 */
static std::string macroIntegerToDate(int macroInteger) {
    int year = (macroInteger / 10000);          // 2014
    int month = (macroInteger % 10000 / 100);   // 09
    int day = (macroInteger % 100);             // 07
    std::ostringstream out;
    out << std::setw(4) << std::setfill('0') << year
        << "/" << std::setw(2) << std::setfill('0') << month
        << "/" << std::setw(2) << std::setfill('0') << day;
    return out.str();
}
#endif // SPL_PROJECT_VERSION

static void ensureProjectVersionHelper(std::string minVersion) {
#ifdef SPL_PROJECT_VERSION
    std::string projectVersion = macroIntegerToDate(SPL_PROJECT_VERSION);
#else
    std::string projectVersion = "";
#endif

    if (projectVersion < minVersion) {
        fputs("\n", stderr);
        fputs("***\n", stderr);
        fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
        fputs("*** Your project's properties (from your .pro file) are too old\n", stderr);
        fputs("*** and are not compatible with this version of the C++ library.\n", stderr);
        fputs("*** Exiting immediately to avoid compatibility errors.\n", stderr);
        fputs("*** Please update your .pro to a newer compatible version.\n", stderr);
        fprintf(stderr, "*** Your .pro version found : %s\n", projectVersion.c_str());
        fprintf(stderr, "*** Minimum version accepted: %s\n", minVersion.c_str());
        fputs("***\n", stderr);
        fputs("\n", stderr);
        fflush(stderr);
        
        std::exit(1);
    }
}

void ensureJavaBackEndVersion(std::string /*minVersion*/) {
    // empty
}

void ensureProjectVersion(std::string minVersion) {
    if (minVersion.empty()) {
#ifdef SPL_MINIMUM_PROJECT_VERSION
        minVersion = SPL_MINIMUM_PROJECT_VERSION;
#else
        minVersion = STANFORD_CPP_PROJECT_MINIMUM_VERSION;
#endif
    }
    ensureProjectVersionHelper(minVersion);
}

std::string getCppLibraryDocsUrl() {
    return STANFORD_CPP_LIB_DOCS_URL;
}

std::string getCppLibraryVersion() {
    return STANFORD_CPP_LIB_VERSION;
}

std::string getJavaBackEndVersion() {
    return "?";
}

/*
 * Returns version of project properties as defined in .pro file.
 */
std::string getProjectVersion() {
#ifdef SPL_PROJECT_VERSION
    // 20141031 -> "2014/10/31"
    std::ostringstream out;
    out << (SPL_PROJECT_VERSION / 10000);         // 2014
    out << "/";
    out << (SPL_PROJECT_VERSION % 10000 / 100);   // 10
    out << "/";
    out << (SPL_PROJECT_VERSION % 100);           // 31
    return out.str();
#endif
    return "";
}

} // namespace version
