#####################################################################
## Stanford CS 106 B/X Qt Creator project file                     ##
## (now compatible with Qt-based GUI system!)                      ##
#####################################################################
#
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
#####################################################################
## If you need to add files or folders to your project, we suggest ##
## that you "re-initialize" your project by doing the following:   ##
##                                                                 ##
## 1- close Qt Creator.                                            ##
## 2- delete your ".pro.user" file and "build_xxxxxxx" directory.  ##
## 3- place the new files/folders into your project directory.     ##
## 4- re-open and "Configure" your project again.                  ##
#####################################################################
#
# @author Marty Stepp
#     (past authors/support by Reid Watson, Rasmus Rygaard, Jess Fisher, etc.)
# @version 2018/10/23
# - added Qt multimedia flag for sound and video playback
# @version 2018/10/20
# - flag for console scaling on high-DPI systems
# @version 2018/10/18
# - C++14 support added on some systems
# @version 2018/10/06
# - revised autograder folder structure
# @version 2018/10/04
# - variables for GUI vs console-based autograders
# @version 2018/09/29
# - simplify/improve resource-file-copying logic (with Windows fixes)
# @version 2018/09/25
# - flags to rename main function and other linker fixes
# @version 2018/09/23
# - fixed some compiler flags to fix build on Win/Mac
# @version 2018/09/20
# - fixed static linking for release builds
# @version 2018/09/18
# - added flags for precompilation of Qt MOC resources
# @version 2018/09/16
# - removed stack-size increase code on Windows that broke thread creation
# @version 2018/09/06
# - removed references to old Java back-end spl.jar
# @version 2018/07/01
# - enable Qt in configuration to support Qt-based GUI functionality
# - remove some compiler warnings (long-long, useless-cast) because they trigger in Qt
# @version 2018/02/28
# - flag to disable some BasicGraph Vertex/Edge members
# @version 2018/01/23
# - modify a couple of clang compiler flags for OSX
# @version 2017/11/15
# - turn on collection iterator version checking
# @version 2017/11/02
# - exclude Qt lib inclusion on Mac
# - fixed unknown compiler warning option on Mac
# @version 2017/10/24
# - added pthread library on Linux/Mac
# @version 2017/10/20
# - fixed autograder source/header inclusion wrt project filter
# - fixed autograder compiler warning flags
# @version 2017/10/18
# - re-enabled compiler warning flags that were mistakenly disabled (oops!)
# - fixed bug with omitting directories under res/ for other-files copying
# @version 2017/10/06
# - added some compiler warning flags
# - added $$files wrappings around source/header file inclusion
# - added cache() call to all OSes to get rid of Qmake warning
# @version 2017/10/05
# - bug fixes for PROJECT_FILTER option (used for multiple .pro in same folder)
# @version 2017/10/01
# - added check for being inside .zip archive (common mistake on Windows)
# - added check for intl chars in path (common issue for intl students)
# - added kludge to trick Qt Creator into allowing you to add files to your project (yay)
# - tweaked behavior for xcopy of files on Windows
# - improved quoting/escaping of paths and filenames for compatibility
# @version 2016/12/09
# - added SPL_THROW_ON_INVALID_ITERATOR flag (default disabled)
# @version 2016/12/01
# - slight tweaks to compiler flags to improve stack trace line generation
# @version 2016/11/07
# - better C++11 compiler flag compatibility (courtesy Kevin Miller)
# @version 2016/10/30
# - added FONTSIZE back in, but commented out by default
# @version 2016/10/19
# - added PROJECT_FILTER option to make it easier to have multiple .pro in same dir (e.g. ADTs HW)
# @version 2016/10/18
# - re-disabled Mac stack increase flag
# @version 2016/10/15
# - new stricter compiler warning flags (float equality comparison; null pointers; undefined macros; etc.)
# @version 2016/10/13
# - split StanfordCPPLib into subfolders: collections, graphics, io, private, system, util
# @version 2016/09/24
# - added private/*.cpp to sources
# - added verification of .pro version by default
# @version 2016/08/12
# - fixed Windows release build problems
# @version 2016/08/04
# - added flag for throwing errors on op >> parsing (default off)
# @version 2016/07/22
# - added support for src/autograder/ directory
# @version 2016/06/28
# - fixed bugs with 'copydata' on Windows systems
# @version 2016/06/24
# - removed FONTSIZE setting; left to default and app configuration
# - made output/ folder copy itself to subdir of build directory, if present
# - improved code flow, structure, and comments
# @version 2015/04/09
# - decreased Mac stack size to avoid sporatic crashes on Mac systems
# @version 2014/11/29
# - added pthread library on Mac/Linux for running each test in its own thread
# @version 2014/11/13
# - fixes related to generating stack traces
# - support for putting testing files in a src/test/ folder (used in development)
# @version 2014/11/05
# - improved/fixed flags for exception-handling
# @version 2014/10/31
# - standard autograder-compatible version; should work with all assignments and graders.

TEMPLATE = app
PROJECT_FILTER =
QT       += core gui multimedia network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

###############################################################################
# BEGIN SECTION FOR SPECIFYING SOURCE/LIBRARY/RESOURCE FILES OF PROJECT       #
###############################################################################

# checks to make sure we haven't accidentally opened the project
# from within a ZIP archive (common mistake on Windows)

win32 {
    contains(PWD, .*\.zip.*) | contains(PWD, .*\.ZIP.*) {
        message("")
        message(*******************************************************************)
        message(*** ERROR: You are trying to open this project from within a .ZIP archive:)
        message(*** $$PWD)
        message(*** You need to extract the files out of the ZIP file first.)
        message(*** Open the ZIP in your file explorer and press the Extract button.)
        message(*******************************************************************)
        message("")
        error(Exiting.)
    }
}

# check for intl chars in path (common issue for intl students)
PWD_WITHOUT_BAD_CHARS = $$PWD
PWD_WITHOUT_BAD_CHARS ~= s|[^a-zA-Z0-9_ ().\/:;+-]+|???
BAD_CHARS = $$PWD
BAD_CHARS ~= s|[a-zA-Z0-9_ ().\/:;-]+|
!isEmpty(BAD_CHARS) {
    message("")
    message(*******************************************************************)
    message(*** ERROR: Your project directory contains invalid characters:)
    message(*** $$PWD)
    message(***)
    message(*** The characters that are invalid are: $$BAD_CHARS)
    message(***)
    message(*** You need to store your project in a directory without any punctuation)
    message(*** marks such as commas, or international symbols such as)
    message(*** Chinese or Korean symbols.)
    message(*** If you keep seeing this error try creating a simple directory)
    message(*** name such as "C:\Programs\Homework1.")
    message(*******************************************************************)
    message("")
    error(Exiting.)
}

# checks to ensure that the Stanford C++ library is present in this project
!exists($$PWD/lib/StanfordCPPLib/private/version.h) {
    message("")
    message(*******************************************************************)
    message(*** ERROR: Stanford C++ library not found!)
    message(*** This project cannot run without the folder lib/StanfordCPPLib/.)
    message(*** Place that folder into your project and try again.)
    message(*******************************************************************)
    message("")
    error(Exiting.)
}

win32 {
    !exists($$PWD/lib/addr2line.exe) {
        message("")
        message(*******************************************************************)
        message(*** ERROR: Stanford C++ library support file 'addr2line.exe' not found!)
        message(*** Our library needs this file present to produce stack traces.)
        message(*** Place that file into your lib/ folder and try again.)
        message(*******************************************************************)
        message("")
        warning(Exiting.)
    }
}

# precompiled header speeds up build times
!win32 {
PRECOMPILED_HEADER = $$files($$PWD/lib/StanfordCPPLib/private/precompiled.h)
}

# honeypot to trick Qt Creator so that adding files works from within IDE;
# Qt looks for first 'SOURCES *=' line and adds newly added .cpp/h files there.
# But then that causes the files to be added twice because of *.cpp/h rules below.
# To get around this, we follow the first 'SOURCES *=' line by a line that clears
# out SOURCES, so that the Qt Creator .pro modification is ineffectual.
DISTFILES *= ""
DISTFILES = ""
HEADERS *= ""
HEADERS = ""
SOURCES *= ""
SOURCES = ""

# include various source .cpp files and header .h files in the build process
# (student's source code can be put into project root, or src/ subfolder)
exists($$PWD/lib/StanfordCPPLib/*.cpp) {
    SOURCES *= $$files($$PWD/lib/StanfordCPPLib/*.cpp)
}
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/collections/*.cpp)
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/graphics/*.cpp)
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/io/*.cpp)
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/private/*.cpp)
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/system/*.cpp)
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/util/*.cpp)
exists($$PWD/src/$$PROJECT_FILTER*.cpp) {
    SOURCES *= $$files($$PWD/src/$$PROJECT_FILTER*.cpp)
}
exists($$PWD/src/test/*.cpp) {
    SOURCES *= $$files($$PWD/src/test/*.cpp)
}
exists($$PWD/$$PROJECT_FILTER*.cpp) {
    SOURCES *= $$files($$PWD/$$PROJECT_FILTER*.cpp)
}

exists($$PWD/lib/StanfordCPPLib/*.h) {
    HEADERS *= $$files($$PWD/lib/StanfordCPPLib/*.h)
}
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/collections/*.h)
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/graphics/*.h)
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/io/*.h)
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/private/*.h)
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/system/*.h)
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/util/*.h)
exists($$PWD/src/test/*.h) {
    HEADERS *= $$files($$PWD/src/test/*.h)
}
exists($$PWD/src/$$PROJECT_FILTER*.h) {
    HEADERS *= $$files($$PWD/src/$$PROJECT_FILTER*.h)
}
exists($$PWD/$$PROJECT_FILTER*.h) {
    HEADERS *= $$files($$PWD/$$PROJECT_FILTER*.h)
}

# directories examined by Qt Creator when student writes an #include statement
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/collections/
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/graphics/
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/io/
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/system/
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/util/
exists($$PWD/lib/StanfordCPPLib/autograder/*) {
    INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/autograder/
}
INCLUDEPATH *= $$PWD/src/
INCLUDEPATH *= $$PWD/
exists($$PWD/src/autograder/$$PROJECT_FILTER/*.h) {
    INCLUDEPATH *= $$PWD/src/autograder/$$PROJECT_FILTER/
}
exists($$PWD/src/test/*.h) {
    INCLUDEPATH *= $$PWD/src/test/
}

# directories listed as "Other files" in left Project pane of Qt Creator
exists($$PWD/res/*) {
    OTHER_FILES *= $$files($$PWD/res/*)
}
exists($$PWD/*.txt) {
    OTHER_FILES *= $$files($$PWD/*.txt)
}
exists($$PWD/input/*) {
    OTHER_FILES *= $$files($$PWD/input/*)
}
exists($$PWD/output/*) {
    OTHER_FILES *= $$files($$PWD/output/*)
}

###############################################################################
# END SECTION FOR SPECIFYING SOURCE/LIBRARY/RESOURCE FILES OF PROJECT         #
###############################################################################


###############################################################################
# BEGIN SECTION FOR SPECIFYING COMPILER/LINKER FLAGS AND LIBRARIES            #
###############################################################################

# set up flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
CONFIG += no_include_pwd         # make sure we do not accidentally #include files placed in 'resources'
CONFIG += sdk_no_version_check   # removes spurious warnings on Mac OS X

win32 {
    # some Windows systems have old MinGW compilers, so be safe and use C++11
    CONFIG += c++11
} else {
    # Mac/Linux should support the latest C++ features
    CONFIG += c++14
}

# uncomment this if you want to dump the preprocessor output into the .o files
# (useful when debugging advanced preprocessor hacking)
# QMAKE_CXXFLAGS += -E

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Wcast-align
#QMAKE_CXXFLAGS += -Wfloat-equal
QMAKE_CXXFLAGS += -Wformat=2
QMAKE_CXXFLAGS += -Wlogical-op
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-old-style-cast
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-sign-conversion
QMAKE_CXXFLAGS += -Wno-write-strings
QMAKE_CXXFLAGS += -Wreturn-type
QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS += -Werror=uninitialized
QMAKE_CXXFLAGS += -Wunreachable-code
exists($$PWD/lib/autograder/*.h) | exists($$PWD/lib/StanfordCPPLib/autograder/$$PROJECT_FILTER/*.h) | exists($$PWD/lib/autograder/$$PROJECT_FILTER/*.cpp) {
    # omit some warnings/errors in autograder projects
    # (largely because the Google Test framework violates them a ton of times)
    QMAKE_CXXFLAGS += -Wno-deprecation
    QMAKE_CXXFLAGS += -Wno-reorder
    QMAKE_CXXFLAGS += -Wno-unused-function
    QMAKE_CXXFLAGS += -Wno-useless-cast
} else {
    #QMAKE_CXXFLAGS += -Wuseless-cast
    #QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant   # TODO: re-enable for student code?
    #QMAKE_CXXFLAGS += -Werror=zero-as-null-pointer-constant
}

win32 {
    # additional flags for Windows
    LIBS += -lDbghelp
    LIBS += -lbfd
    LIBS += -limagehlp
    cache()
} else {
    # flags that don't work on Windows MinGW compiler
    # QMAKE_CXXFLAGS += -Wno-keyword-macro
    # QMAKE_CXXFLAGS += -Wno-reserved-id-macro
    QMAKE_CXXFLAGS += -Wno-unused-const-variable
    LIBS += -ldl
}

# additional flags for Mac OS X
macx {
    # calling cache() reduces warnings on Mac OS X systems
    cache()
    QMAKE_MAC_SDK = macosx
}

# additional flags for Linux
unix:!macx {
    cache()
}

# libraries for all OSes
LIBS += -lpthread

# additional flags for clang compiler (default on Mac)
COMPILERNAME = $$QMAKE_CXX
COMPILERNAME ~= s|.*/|
equals(COMPILERNAME, clang++) {
    QMAKE_CXXFLAGS += -Wno-format-nonliteral
    QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
    QMAKE_CXXFLAGS += -Wno-overloaded-virtual
    QMAKE_CXXFLAGS += -Wno-unknown-warning-option
}

# set up configuration flags used internally by the Stanford C++ libraries
# These flags are all optional but can simplify project configuration.
# (setting x/y to 999999 centers the window)
# (see platform.cpp/h for descriptions of some of these flags)

# what version of the Stanford .pro is this? (kludgy integer YYYYMMDD format)
DEFINES += SPL_PROJECT_VERSION=20181023

# wrapper name for 'main' function (needed so student can write 'int main'
# but our library can grab the actual main function to initialize itself)
DEFINES += REPLACE_MAIN_FUNCTION=1
DEFINES += main=qMain

# x/y location and w/h of the graphical console window; set to -1 to center
DEFINES += SPL_CONSOLE_X=-1
DEFINES += SPL_CONSOLE_Y=-1
DEFINES += SPL_CONSOLE_WIDTH=800
DEFINES += SPL_CONSOLE_HEIGHT=500

# font size of the font in the graphical console window; can also be set via window menu
#DEFINES += SPL_CONSOLE_FONTSIZE=18

# echo graphical console onto the plain text console as well?
DEFINES += SPL_CONSOLE_ECHO

# quit the C++ program when the graphical console is closed?
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE

# crash if the .pro is older than the minimum specified in version.h? (SPL_PROJECT_VERSION)
DEFINES += SPL_VERIFY_PROJECT_VERSION

# allow clients to access the internal data inside the heap of PriorityQueue?
# (used for some practice exam exercises/demos)
DEFINES += PQUEUE_ALLOW_HEAP_ACCESS

# should toString / << of a PriorityQueue display the elements in sorted order,
# or in heap internal order? the former is more expected by client; the latter
# is faster and avoids a deep-copy
DEFINES += PQUEUE_PRINT_IN_HEAP_ORDER

# flag to throw exceptions when a collection iterator is used after it has
# been invalidated (e.g. if you remove from a Map while iterating over it)
DEFINES += SPL_THROW_ON_INVALID_ITERATOR

# flag to add members like 'cost', 'visited', etc. to BasicGraph Vertex/Edge
# (we are going to disable these to force more interesting implementations)
# DEFINES += SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

# should we throw an error() when operator >> fails on a collection?
# for years this was true, but the C++ standard says you should just silently
# set the fail bit on the stream and exit, so that has been made the default.
# DEFINES += SPL_ERROR_ON_STREAM_EXTRACT

# scale up the console window on high-DPI screens?
# DEFINES += SPL_SCALE_HIGH_DPI_SCREEN

# is the .cpp portion of the library merged into a single .cpp file
# to speed up compilation?
DEFINES += SPL_MERGED_LIBRARY_SINGLE_FILE

# should we attempt to precompile the Qt moc_*.cpp files for speed?
DEFINES += SPL_PRECOMPILE_QT_MOC_FILES

# build-specific options (debug vs release)

# make 'debug' target (default) use no optimization, generate debugger symbols,
# and catch/print to console any uncaught exceptions thrown by the program
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g3
    QMAKE_CXXFLAGS += -fno-inline
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer

    unix:!macx {
        equals(COMPILERNAME, g++) {
            # on Linux g++, these flags help us gather line numbers for stack traces
            QMAKE_CXXFLAGS += -rdynamic
            QMAKE_CXXFLAGS += -export-dynamic
            QMAKE_CXXFLAGS += -Wl,--export-dynamic
            QMAKE_LFLAGS += -export-dynamic
            QMAKE_LFLAGS += -rdynamic
        }
    }
    unix:macx {
        equals(COMPILERNAME, clang++) {
            QMAKE_LFLAGS += -rdynamic
        }
    }

    # print details about uncaught exceptions with red error text / stack trace
    DEFINES += SPL_CONSOLE_PRINT_EXCEPTIONS
}

# make 'release' target be statically linked so it is a stand-alone executable
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
    macx {
        QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app'
        #QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app && rm $${OUT_PWD}/*.o && rm $${OUT_PWD}/Makefile'
    }
    unix:!macx {
        # commenting out -static because it doesn't link to Qt libraries properly
        #QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
    }
    win32 {
        TARGET_PATH = $${OUT_PWD}/release/$${TARGET}.exe
        TARGET_PATH ~= s,/,\\,g

        OUT_PATH = $${OUT_PWD}/
        OUT_PATH ~= s,/,\\,g

        REMOVE_DIRS += '"'$${OUT_PWD}/release'"'
        REMOVE_DIRS += '"'$${OUT_PWD}/debug'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile.Debug'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile.Release'"'
        REMOVE_FILES += '"'$${OUT_PWD}/object_script.$${TARGET}.Debug'"'
        REMOVE_DIRS ~= s,/,\\,g
        REMOVE_FILES ~= s,/,\\,g

        #QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
        QMAKE_POST_LINK += copy '"'$${TARGET_PATH}'"' '"'$${OUT_PATH}'"'
    }
}

###############################################################################
# END SECTION FOR SPECIFYING COMPILER/LINKER FLAGS AND LIBRARIES              #
###############################################################################


###############################################################################
# BEGIN SECTION FOR CS 106B/X AUTOGRADER PROGRAMS                             #
###############################################################################

# settings specific to CS 106 B/X auto-grading programs; do not modify
exists($$PWD/lib/autograder/*.h) | exists($$PWD/lib/StanfordCPPLib/autograder/*.h) | exists($$PWD/src/autograder/$$PROJECT_FILTER/*.cpp) {
    # include the various autograder source code and libraries in the build process
    exists($$PWD/lib/autograder/*.cpp) {
        SOURCES *= $$files($$PWD/lib/autograder/*.cpp)
    }
    exists($$PWD/lib/StanfordCPPLib/autograder/*.cpp) {
        SOURCES *= $$files($$PWD/lib/autograder/*.cpp)
    }
    exists($$PWD/src/autograder/$$PROJECT_FILTER/*.cpp) {
        SOURCES *= $$files($$PWD/src/autograder/$$PROJECT_FILTER/*.cpp)
    }

    exists($$PWD/lib/autograder/*.h) {
        HEADERS *= $$PWD/lib/autograder/*.h
    }
    exists($$PWD/lib/StanfordCPPLib/autograder/*.h) {
        HEADERS *= $$PWD/lib/StanfordCPPLib/autograder/*.h
    }
    exists($$PWD/src/autograder/$$PROJECT_FILTER/*.h) {
        HEADERS *= $$files($$PWD/src/autograder/$$PROJECT_FILTER/*.h)
    }

    exists($$PWD/lib/autograder/*) {
        INCLUDEPATH *= $$PWD/lib/autograder/
    }
    exists($$PWD/lib/StanfordCPPLib/autograder/*) {
        INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/autograder/
    }
    exists($$PWD/src/autograder/*.h) {
        INCLUDEPATH *= $$PWD/src/autograder/
    }
    exists($$PWD/src/autograder/$$PROJECT_FILTER/*.h) {
        INCLUDEPATH *= $$PWD/src/autograder/$$PROJECT_FILTER/
    }

    DEFINES += SPL_AUTOGRADER_MODE

    # define the style of autograder you want to use (GUI vs console)
    DEFINES += SPL_GRAPHICAL_AUTOGRADER
    # DEFINES += SPL_CONSOLE_AUTOGRADER

    # a check to ensure that required autograder resources are present in this project
    !exists($$PWD/res/autograder/pass.gif) {
        message("")
        message(*******************************************************************)
        message(*** ERROR: Stanford library cannot find image files pass.gif, fail.gif, etc.!)
        message(*** This project cannot run without those images present.)
        message(*** Place those files into your res/autograder/ folder and try again.)
        message(*******************************************************************)
        message("")
        error(Exiting.)
    }

    # copy autograder resource files into build folder
    #exists($$PWD/res/autograder/*) {
    #    COPY_RESOURCE_FILES_INPUT += $$PWD/res/autograder
    #    OTHER_FILES *= $$files(res/autograder)
    #}

    # copy source code into build folder so it can be analyzed by style checker
    #exists($$PWD/src/$$PROJECT_FILTER*.cpp) {
    #    copyResources.input *= $$files($$PWD/src/$$PROJECT_FILTER*.cpp)
    #    copyToDestdir($$files($$PWD/src/$$PROJECT_FILTER*.cpp))
    #}
    #exists($$PWD/$$PROJECT_FILTER*.cpp) {
    #    copyResources.input *= $$files($$PWD/$$PROJECT_FILTER*.cpp)
    #    copyToDestdir($$files($$PWD/$$PROJECT_FILTER*.cpp))
    #}
    #exists($$PWD/src/$$PROJECT_FILTER*.h) {
    #    copyResources.input *= $$files($$PWD/src/$$PROJECT_FILTER*.h)
    #    copyToDestdir($$files($$PWD/src/$$PROJECT_FILTER*.h))
    #}
    #exists($$PWD/$$PROJECT_FILTER*.h) {
    #    copyResources.input *= $$files($$PWD/$$PROJECT_FILTER*.h)
    #    copyToDestdir($$files($$PWD/$$PROJECT_FILTER*.h))
    #}
}
###############################################################################
# END SECTION FOR CS 106B/X AUTOGRADER PROGRAMS                               #
###############################################################################


###############################################################################
# BEGIN SECTION FOR DEFINING HELPER FUNCTIONS FOR RESOURCE COPYING            #
###############################################################################

# copy res/* to root of build directory (input files, etc.)
# COPY_RESOURCE_FILES_INPUT = ""    # defined above so that autograder files can be included
COPY_RESOURCE_FILES_INPUT = ""

win32 {
    exists($$PWD/lib/addr2line.exe) {
        COPY_RESOURCE_FILES_INPUT += $$PWD/lib/addr2line.exe
    }
}
exists($$PWD/*.txt) {
    COPY_RESOURCE_FILES_INPUT += $$PWD/*.txt
}
exists($$PWD/res/*) {
    COPY_RESOURCE_FILES_INPUT += $$PWD/res/*
}
exists($$PWD/input/*) {
    COPY_RESOURCE_FILES_INPUT += $$PWD/input
}
exists($$PWD/output/*) {
    COPY_RESOURCE_FILES_INPUT += $$PWD/output
}

copy_resource_files.name = Copy resource files to the build directory
win32 {
    # https://support.microsoft.com/en-us/help/289483/switches-that-you-can-use-with-xcopy-and-xcopy32-commands
    # /s - copy subfolders
    # /q - quiet (no verbose output)
    # /y - overwrite without prompting
    # /i - if destination does not exist and copying more than one file, assumes destination is a folder
    copy_resource_files.commands = xcopy /s /q /y /i ${QMAKE_FILE_IN}
} else {
    copy_resource_files.commands = cp -rf ${QMAKE_FILE_IN} .
}
copy_resource_files.input = COPY_RESOURCE_FILES_INPUT
copy_resource_files.output = $${OUT_PWD}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_resource_files.CONFIG = no_link no_clean target_predeps
QMAKE_EXTRA_COMPILERS += copy_resource_files

###############################################################################
# END SECTION FOR DEFINING HELPER FUNCTIONS FOR RESOURCE COPYING              #
###############################################################################

# END OF FILE (this should be line #644; if not, your .pro has been changed!)
