/*
 * File: version.h
 * ---------------
 * version message reports the version/build info of the
 * Stanford C++ library.
 *
 */

#ifndef _version_h
#define _version_h
#include <string>

namespace version {

    std::string getLibraryInfoPanelMessage();
    std::string getLibraryVersion();
    std::string getLibraryPathForResource(const std::string& name);
}

#endif // _version_h
