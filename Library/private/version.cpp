/*
 * File: version.cpp
 * -----------------
 * @author Julie Zelenski
 * updated for installed static lib
 */

#include "private/build.h"
#include "version.h"

namespace version {

    std::string getLibraryInfoPanelMessage()
    {
        return "<html><p>" "Stanford C++ Library version " SPL_VERSION "<br>"
            "<br>"
            "<small>Build on " SPL_BUILD_DATE " by " SPL_BUILD_USER "<br>"
            "Installed into " SPL_INSTALL_LOCATION "<br>"
            "<br>"
            "Libraries originally authored by Eric Roberts.<br>"
            "Updates from Marty Stepp, Keith Schwarz, and Julie Zelenski.<br>"
            "&copy; Stanford University &bull; 2003-2020.";
    }

    std::string getLibraryVersion()
    {
        return std::string(SPL_VERSION);
    }
}