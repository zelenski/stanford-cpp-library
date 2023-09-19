/*
 * File: version.cpp
 * -----------------
 * @author Julie Zelenski
 * updated for installed static lib
 */

#include "private/build.h"
#include "version.h"
#include "filelib.h"


#include <QString>
#include <QStandardPaths>
#include <QLibraryInfo>

static std::string installedLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toStdString() +  "/cs106";
}

namespace version {



    std::string getLibraryInfoPanelMessage()
    {
        return "<html><p>" "Stanford C++ Library version " SPL_VERSION "<br>"
            "<br>"
            "<small>Library built on " SPL_BUILD_DATE " by " SPL_BUILD_USER "<br>"
            "Installed into " + installedLocation() + "<br>"
            "Compiled on QT version " + QT_VERSION_STR + "<br>"
            "Running on QT version " + QLibraryInfo::version().toString().toStdString() + "<br>"
            "<br>"
            "Libraries originally authored by Eric Roberts.<br>"
            "Updates from Marty Stepp, Keith Schwarz, and Julie Zelenski.<br>"
            "&copy; Stanford University &bull; 2003-2023.";
    }

    std::string getLibraryVersion()
    {
        return std::string(SPL_VERSION);
    }

    std::string getLibraryPathForResource(const std::string& name)
    {
        std::string path1 = installedLocation() + "/" + name;
        std::string path2 = installedLocation() + "/resources/" + name;

        if (fileExists(path1)) {
            return path1;
        } else if (fileExists(path2)) {
            return path2;
        }
        return "";
    }

}
