/*
 * File: splversion.h
 * ------------------
 * functions to report version/build info of the
 * Stanford C++ library and where installed.
 *
 */

#pragma once
#include <string>

std::string getLibraryInfoPanelMessage();
std::string getLibraryVersion();
std::string getLibraryPathForResource(const std::string& name);
