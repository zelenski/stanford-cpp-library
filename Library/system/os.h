/*
 * File: os.h
 * ----------
 * This file exports a class with static member functions related to the
 * current operating system.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2018/09/16
 * - initial version
 */


#ifndef _os_h
#define _os_h

#include <string>

/**
 * This class contains useful static methods for asking about what operating
 * system the program is currently running on.
 */
class OS {
public:
    /**
     * Returns a string representing the name of the current operating system.
     */
    static std::string getName();

    /**
     * Returns a string representing the current version or release of the
     * current operating system.
     */
    static std::string getVersion();

    /**
     * Returns true if the current operating system appears to be Linux.
     */
    static bool isLinux();

    /**
     * Returns true if the current operating system appears to be Mac OS X.
     */
    static bool isMac();

    /**
     * Returns true if the current operating system appears to be Windows.
     */
    static bool isWindows();

private:
    OS();   // prevent construction
};

#endif // _os_h
