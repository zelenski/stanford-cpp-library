/*
 * File: os.cpp
 * ------------
 * This file implements the OS class declared in os.h.
 * 
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless (non-Qt) mode support
 * @version 2018/09/23
 * - bug fix for isMac
 * @version 2018/09/17
 * - initial version
 */

#include "os.h"
#ifndef SPL_HEADLESS_MODE
#include <QString>
#include <QSysInfo>
#endif // SPL_HEADLESS_MODE
#include "strlib.h"

#if !defined(SPL_HEADLESS_MODE) && QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
/*static*/ std::string OS::getName() {
    std::string productName = QSysInfo::prettyProductName().toStdString();
    return productName;
}

/*static*/ std::string OS::getVersion() {
    std::string productVersion = QSysInfo::productVersion().toStdString();
    return productVersion;
}

bool OS::isLinux() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return kernelType == "linux" || productType == "linux";
}

bool OS::isMac() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return stringContains(kernelType, "apple")
            || stringContains(kernelType, "darwin")
            || stringContains(kernelType, "mac")
            || stringContains(kernelType, "ios")
            || stringContains(productType, "apple")
            || stringContains(productType, "ios")
            || stringContains(productType, "mac")
            || stringContains(productType, "osx");
}

bool OS::isWindows() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return stringContains(kernelType, "windows")
            || stringContains(productType, "windows");
}
#else
/*static*/ std::string OS::getName() {
    return "unknown";
}

/*static*/ std::string OS::getVersion() {
    return "unknown";
}

bool OS::isLinux() {
    return false;
}

bool OS::isMac() {
    return false;
}

bool OS::isWindows() {
    return true;   // most old versions of Qt occur on old Windows installs
}
#endif // QT_VERSION

OS::OS() {
    // empty
}
