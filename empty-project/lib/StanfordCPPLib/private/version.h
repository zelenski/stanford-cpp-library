/*
 * File: version.h
 * ---------------
 * This file contains declarations of the version number of the
 * Stanford C++ library.
 *
 * @author Marty Stepp
 * @version 2016/10/13
 */

#ifndef _version_h
#define _version_h

#include <string>

/*
 * Current version of the Stanford C++ library.
 * NOTE: This value and the other date values here all
 *       *MUST* be zero-padded to YYYY/MM/DD format;
 *       if month or day is < 10, insert a preceding 0
 */
#define STANFORD_CPP_LIB_VERSION "2016/10/13"

/*
 * Minimum version of your IDE's project that is supported.
 * platform.cpp checks this and exits if project .pro is older than
 * this to avoid incompatible versions.
 *
 * NOTE: This value and the other date values here all
 *       *MUST* be zero-padded to YYYY/MM/DD format;
 *       if month or day is < 10, insert a preceding 0
 */
#define STANFORD_CPP_PROJECT_MINIMUM_VERSION "2016/10/13"

/*
 * Minimum version of Java back-end spl.jar that is supported.
 * platform.cpp checks this and exits if spl.jar is older than
 * this to avoid incompatible versions.
 *
 * NOTE: This value and the other date values here all
 *       *MUST* be zero-padded to YYYY/MM/DD format;
 *       if month or day is < 10, insert a preceding 0
 */
#define STANFORD_JAVA_BACKEND_MINIMUM_VERSION "2016/10/12"

namespace version {
void ensureJavaBackEndVersion(std::string minVersion = "");
void ensureProjectVersion(std::string minVersion = "");
std::string getCppLibraryVersion();
std::string getJavaBackEndVersion();
std::string getProjectVersion();
} // namespace version

#endif
