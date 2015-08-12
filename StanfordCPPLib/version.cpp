/*
 * File: version.cpp
 * -----------------
 * This file implements functions for checking the version number of the
 * Stanford C++ library, its Java back-end, and the project properties
 * file (.pro).
 *
 * @author Marty Stepp 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/11/13
 * - ifdef'd out macroIntegerToDate function to remove unused function warning on some platforms
 * @since 2014/10/31
 */

#include "private/version.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include "platform.h"

namespace version {
#ifdef SPL_PROJECT_VERSION
/*
 * converts an 8-digit into into a YYYY/MM/DD date,
 * e.g. 20141031 -> "2014/10/31"
 * needed because I can't have a string as a -D command-line defined flag
 */
static std::string macroIntegerToDate(int macroInteger) {
    std::ostringstream out;
    out << (macroInteger / 10000);         // 2014
    out << "/";
    out << (macroInteger % 10000 / 100);   // 10
    out << "/";
    out << (macroInteger % 100);           // 31
    return out.str();
}
#endif // SPL_PROJECT_VERSION

static void ensureJavaBackEndVersionHelper(std::string minVersion) {
    std::string backendVersion = getPlatform()->cpplib_getJavaBackEndVersion();
    if (backendVersion < minVersion) {
        // use stderr directly rather than cerr because graphical console is unreachable
        fputs("\n", stderr);
        fputs("***\n", stderr);
        fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
        fputs("*** The Java back-end library 'spl.jar' detected is too old\n", stderr);
        fputs("*** and is not compatible with this version of the C++ library.\n", stderr);
        fputs("*** Exiting immediately to avoid compatibility errors.\n", stderr);
        fputs("*** Please update your spl.jar to a newer compatible version.\n", stderr);
        fprintf(stderr, "*** Java back-end spl.jar found: %s\n", backendVersion.c_str());
        fprintf(stderr, "*** Minimum version accepted   : %s\n", minVersion.c_str());
        fputs("***\n", stderr);
        fputs("\n", stderr);
        fflush(stderr);
        
        exit(1);
    }
}

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
        
        exit(1);
    }
}

void ensureJavaBackEndVersion(std::string minVersion) {
    if (minVersion.empty()) {
        minVersion = STANFORD_JAVA_BACKEND_MINIMUM_VERSION;
    }
    ensureJavaBackEndVersionHelper(minVersion);
}

void ensureProjectVersion(std::string minVersion) {
    if (minVersion.empty()) {
#ifdef SPL_MINIMUM_PROJECT_VERSION
        minVersion = SPL_MINIMUM_PROJECT_VERSION;
#else
        minVersion = "";
#endif
    }
    ensureProjectVersionHelper(minVersion);
}

std::string getCppLibraryVersion() {
    return STANFORD_CPP_LIB_VERSION;
}

std::string getJavaBackEndVersion() {
    return getPlatform()->cpplib_getJavaBackEndVersion();
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
