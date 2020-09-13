/*
 * File: filelibwindows.cpp
 * ------------------------
 * This file contains Windows implementations of filelib.h primitives.
 * This code used to live in platform.cpp before the Java back-end was retired.
 *
 * @version 2018/10/23
 * - added getAbsolutePath
 */

#include "filelib.h"

// define all of the following only on Windows OS
// (see filelibunix.cpp for non-Windows versions)
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#undef MOUSE_EVENT
#undef KEY_EVENT
#undef MOUSE_MOVED
#undef HELP_KEY
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <string>
#include "error.h"
#include "strlib.h"

namespace platform {

void filelib_createDirectory(const std::string& path) {
    std::string pathStr = path;
    if (endsWith(path, "\\")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (CreateDirectoryA(path.c_str(), nullptr) == 0) {
        if (GetLastError() == ERROR_ALREADY_EXISTS && filelib_isDirectory(pathStr)) {
            return;
        }
        error("createDirectory: Can't create " + path);
    }
}

void filelib_deleteFile(const std::string& path) {
    if(filelib_isDirectory(path)) {
        RemoveDirectoryA(path.c_str());
    } else {
        DeleteFileA(path.c_str());
    }
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename.empty()) {
        return "";
    }
    std::string filenameStr = filename;
    for (int i = 0, len = filename.length(); i < len; i++) {
        if (filenameStr[i] == '/') {
            filenameStr[i] = '\\';
        }
    }
    return filenameStr;
}

bool filelib_fileExists(const std::string& filename) {
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    if (_fullpath(realpathOut, path.c_str(), 4095)) {
        std::string absPath(realpathOut);
        return absPath;
    } else {
        return path;
    }
}

std::string filelib_getCurrentDirectory() {
    char path[MAX_PATH + 1];
    int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
    return std::string(path, n);
}

std::string filelib_getDirectoryPathSeparator() {
    return "\\";
}

std::string filelib_getSearchPathSeparator() {
    return ";";
}

std::string filelib_getTempDirectory() {
    char path[MAX_PATH + 1];
    int n = GetTempPathA(MAX_PATH + 1, path);
    return std::string(path, n);
}

bool filelib_isDirectory(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/gg258117(v=vs.85).aspx
bool filelib_isFile(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool filelib_isSymbolicLink(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES
            && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

void filelib_listDirectory(const std::string& path, Vector<std::string> & list) {
    std::string pathStr = path;
    if (pathStr == "") {
        pathStr = ".";
    }
    std::string pattern = pathStr + "\\*.*";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        error("listDirectory: Can't list directory \"" + pathStr + "\"");
    }
    list.clear();
    while (true) {
        std::string name = std::string(fd.cFileName);
        if (name != "." && name != "..") {
            list.add(name);
        }
        if (!FindNextFileA(h, &fd)) {
            break;
        }
    }
    FindClose(h);
    sort(list.begin(), list.end());
}

std::string file_openFileDialog(const std::string& /*title*/,
                                const std::string& /*mode*/,
                                const std::string& /*path*/) {
    // TODO
    return "";
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (!filelib_isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
        error("setCurrentDirectory: Can't change to " + path);
    }
}

} // namespace platform

#endif // _WIN32
