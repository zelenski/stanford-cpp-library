/*
 * File: filelibunix.cpp
 * ---------------------
 * This file contains Unix implementations of filelib.h primitives.
 * This code used to live in platform.cpp before the Java back-end was retired.
 *
 * @version 2018/10/23
 * - added getAbsolutePath
 */

#include "filelib.h"

// define all of the following only on non-Windows OS
// (see filelibwindows.cpp for Windows versions)
#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdint.h>
#include <unistd.h>
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
    if (endsWith(path, "/")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (mkdir(pathStr.c_str(), 0777) != 0) {
        if (errno == EEXIST && filelib_isDirectory(pathStr)) {
            return;
        }
        std::string msg = "createDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

void filelib_deleteFile(const std::string& path) {
    remove(path.c_str());
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename == "") {
        return "";
    }
    int len = filename.length();
    std::string expanded = filename;
    if (expanded[0] == '~') {
        int spos = 1;
        while (spos < len && expanded[spos] != '\\' && expanded[spos] != '/') {
            spos++;
        }
        char *homedir = nullptr;
        if (spos == 1) {
            homedir = getenv("HOME");
            if (!homedir) {
                homedir = getpwuid(getuid())->pw_dir;
            }
        } else {
            struct passwd *pw = getpwnam(expanded.substr(1, spos - 1).c_str());
            if (!pw) {
                error("expandPathname: No such user");
            } else {
                homedir = pw->pw_dir;
            }
        }
        expanded = std::string(homedir) + expanded.substr(spos);
        len = expanded.length();
    }
    for (int i = 0; i < len; i++) {
        if (expanded[i] == '\\') {
            expanded[i] = '/';
        }
    }
    return expanded;
}

bool filelib_fileExists(const std::string& filename) {
    struct stat fileInfo;
    return stat(filename.c_str(), &fileInfo) == 0;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    realpath(path.c_str(), realpathOut);
    std::string absPath(realpathOut);
    return absPath;
}

std::string filelib_getCurrentDirectory() {
    char currentDirBuf[4096] = {'\0'};
    char* cwd = getcwd(currentDirBuf, 4096 - 1);
    std::string result;
    if (cwd) {
        result = std::string(cwd);
    } else {
        error("getCurrentDirectory: " + std::string(strerror(errno)));
    }
    return result;
}

std::string filelib_getDirectoryPathSeparator() {
    return "/";
}

std::string filelib_getSearchPathSeparator() {
    return ":";
}

// http://stackoverflow.com/questions/8087805/
// how-to-get-system-or-user-temp-folder-in-unix-and-windows
std::string filelib_getTempDirectory() {
    char* dir = getenv("TMPDIR");
    if (!dir) dir = getenv("TMP");
    if (!dir) dir = getenv("TEMP");
    if (!dir) dir = getenv("TEMPDIR");
    if (!dir) return "/tmp";
    return dir;
}

bool filelib_isDirectory(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISDIR(fileInfo.st_mode) != 0;
}

bool filelib_isFile(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISREG(fileInfo.st_mode) != 0;
}

bool filelib_isSymbolicLink(const std::string& filename) {
    struct stat fileInfo;
    if (lstat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISLNK(fileInfo.st_mode) != 0;
}

void filelib_listDirectory(const std::string& path, Vector<std::string>& list) {
    DIR* dir = opendir(path.empty() ? "." : path.c_str());
    if (!dir) {
        error(std::string("listDirectory: Can't open \"") + path + "\"");
    }
    list.clear();
    while (true) {
        struct dirent* ep = readdir(dir);
        if (!ep) {
            break;
        }
        std::string name = std::string(ep->d_name);
        if (name != "." && name != "..") {
            list.add(name);
        }
    }
    closedir(dir);
    sort(list.begin(), list.end());
}

std::string file_openFileDialog(const std::string& /*title*/,
                                const std::string& /*mode*/,
                                const std::string& /*path*/) {
    // TODO
    return "";
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (chdir(path.c_str()) != 0) {
        std::string msg = "setCurrentDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

} // namespace platform

#endif // _WIN32
