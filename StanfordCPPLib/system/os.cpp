/*
 * File: os.cpp
 * ------------
 * This file implements the OS class declared in os.h.
 * 
 * @author Marty Stepp
 */

#include "os.h"
#include <QString>
#include <QSysInfo>
#include "strlib.h"

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
            || stringContains(kernelType, "mac")
            || stringContains(kernelType, "ios")
            || stringContains(productType, "apple")
            || stringContains(productType, "mac")
            || stringContains(productType, "ios");
}

bool OS::isWindows() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return stringContains(kernelType, "windows")
            || stringContains(productType, "windows");
}

OS::OS() {
    // empty
}
