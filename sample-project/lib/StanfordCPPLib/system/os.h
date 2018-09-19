/*
 * File: os.h
 * ----------
 * This file exports a class with static member functions related to the
 * current operating system.
 *
 * @version 2018/09/16
 * - initial version
 */

#ifndef _os_h
#define _os_h

#include <string>

class OS {
public:
    static std::string getName();
    static std::string getVersion();
    static bool isLinux();
    static bool isMac();
    static bool isWindows();

private:
    OS();   // prevent construction
};

#endif // _os_h
