#####################################################################
## Stanford CS 106 B/X Qt Creator project file                     ##
#####################################################################
#
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
# @author Marty Stepp (past authors/support by Keith Schwarz, Reid Watson, etc.)
# @version 2019/04/23
# - revert copydata code for copying resource files
DEFINES += SPL_PROJECT_VERSION=20190423   # kludgy YYYYMMDD constant used by lib to know its version

# global Qt/project settings
TEMPLATE = app
PROJECT_FILTER =
QT       += core gui multimedia network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

###############################################################################
# BEGIN SECTION FOR SPECIFYING SOURCE/LIBRARY/RESOURCE FILES OF PROJECT       #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET = $$replace(TARGET, " ", _)

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

win64 {
    !exists($$PWD/lib/addr2line64.exe) {
        message("")
        message(*******************************************************************)
        message(*** ERROR: Stanford C++ library support file 'addr2line64.exe' not found!)
        message(*** Our library needs this file present to produce stack traces.)
        message(*** Place that file into your lib/ folder and try again.)
        message(*******************************************************************)
        message("")
        warning(Exiting.)
    }
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

!exists($$PWD/lib/iconstrip.png) {
    message("")
    message(*******************************************************************)
    message(*** ERROR: Stanford library cannot find image strip file:)
    message(*** lib/iconstrip.png)
    message(*** This project cannot run without this file present.)
    message(*** Place the file into your lib/ folder and try again.)
    message(*******************************************************************)
    message("")
    warning(Exiting.)
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
SOURCES *= $$files($$PWD/lib/StanfordCPPLib/*.cpp, true)
SOURCES *= $$files($$PWD/src/*.cpp, true)
exists($$PWD/$$PROJECT_FILTER*.cpp) {
    SOURCES *= $$files($$PWD/$$PROJECT_FILTER*.cpp)
}

exists($$PWD/lib/StanfordCPPLib/*.h) {
    HEADERS *= $$files($$PWD/lib/StanfordCPPLib/*.h)
}
HEADERS *= $$files($$PWD/lib/StanfordCPPLib/*.h, true)
HEADERS *= $$files($$PWD/src/*.h, true)
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
    OTHER_FILES *= $$files($$PWD/res/*, true)
}
exists($$PWD/*.txt) {
    OTHER_FILES *= $$files($$PWD/*.txt)
}
exists($$PWD/input/*) {
    OTHER_FILES *= $$files($$PWD/input/*, true)
}
exists($$PWD/output/*) {
    OTHER_FILES *= $$files($$PWD/output/*, true)
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
# CONFIG += warn_off

# gives us a bit more precision about which errors are printed
QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CFLAGS_WARN_ON -= -W
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -W

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
#QMAKE_CXXFLAGS += -Wextra
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
QMAKE_CXXFLAGS += -Wunused-parameter
exists($$PWD/lib/autograder/*.h) | exists($$PWD/lib/StanfordCPPLib/autograder/$$PROJECT_FILTER/*.h) | exists($$PWD/lib/autograder/$$PROJECT_FILTER/*.cpp) {
    # omit some warnings/errors in autograder projects
    # (largely because the Google Test framework violates them a ton of times)
    QMAKE_CXXFLAGS += -Wno-deprecated
    QMAKE_CXXFLAGS += -Wno-reorder
    QMAKE_CXXFLAGS += -Wno-unused-function
    QMAKE_CXXFLAGS += -Wno-useless-cast
}

# additional flags for Windows
win32 {
    LIBS += -lDbghelp
    LIBS += -lbfd
    LIBS += -limagehlp
    cache()
} else {
    # flags that don't work on Windows MinGW compiler
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
    QMAKE_CXXFLAGS += -Wempty-init-stmt
    QMAKE_CXXFLAGS += -Wignored-qualifiers
    QMAKE_CXXFLAGS += -Winitializer-overrides
    QMAKE_CXXFLAGS += -Wmissing-field-initializers
    QMAKE_CXXFLAGS += -Wmissing-method-return-type
    QMAKE_CXXFLAGS += -Wnull-pointer-arithmetic
    QMAKE_CXXFLAGS += -Wsemicolon-before-method-body
    QMAKE_CXXFLAGS += -Wno-format-nonliteral
    QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
    QMAKE_CXXFLAGS += -Wno-overloaded-virtual
    QMAKE_CXXFLAGS += -Wno-unknown-warning-option
}

# set up configuration flags used internally by the Stanford C++ libraries
# These flags are all optional but can simplify project configuration.
# (setting x/y to 999999 centers the window)
# (see platform.cpp/h for descriptions of some of these flags)

# wrapper name for 'main' function (needed so student can write 'int main'
# but our library can grab the actual main function to initialize itself)
DEFINES += SPL_REPLACE_MAIN_FUNCTION=1
DEFINES += main=qMain

# x/y location and w/h of the graphical console window; set to -1 to center
DEFINES += SPL_CONSOLE_X=-1
DEFINES += SPL_CONSOLE_Y=-1
DEFINES += SPL_CONSOLE_WIDTH=900
DEFINES += SPL_CONSOLE_HEIGHT=550

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
DEFINES += SPL_PQUEUE_ALLOW_HEAP_ACCESS

# should toString / << of a PriorityQueue display the elements in sorted order,
# or in heap internal order? the former is more expected by client; the latter
# is faster and avoids a deep-copy
DEFINES += SPL_PQUEUE_PRINT_IN_HEAP_ORDER

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

# options for the 'debug' target (default):
# use no optimization, generate debugger symbols,
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
            QMAKE_CXXFLAGS += -Wno-unused-command-line-argument
            QMAKE_CXXFLAGS += -Wl,-export_dynamic
            QMAKE_CXXFLAGS += -Wl,-export-dynamic
            QMAKE_CXXFLAGS += -Wl,--export-dynamic
            QMAKE_LFLAGS += -rdynamic
            QMAKE_LFLAGS += -Wl,-no_pie
        }
    }

    # print details about uncaught exceptions with red error text / stack trace
    DEFINES += SPL_CONSOLE_PRINT_EXCEPTIONS
}

# options for the 'release' target:
# enable some compiler optimizations, and
# make it statically linked so it is a stand-alone executable
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
    # commenting out -static because it doesn't link to Qt libraries properly
    #QMAKE_LFLAGS += -static

    macx {
        QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app'
    }
    unix:!macx {
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
    }
    win32 {
        TARGET_PATH = $${OUT_PWD}/release/$${TARGET}.exe
        TARGET_PATH ~= s,/,\\,g

        OUT_PATH = $${OUT_PWD}/
        OUT_PATH ~= s,/,\\,g

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
        SOURCES *= $$files($$PWD/lib/autograder/*.cpp, true)
    }
    exists($$PWD/lib/autograder/*.h) {
        HEADERS *= $$files($$PWD/lib/autograder/*.h, true)
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
    !exists($$PWD/lib/iconstrip-autograder.png) {
        message("")
        message(*******************************************************************)
        message(*** ERROR: Stanford library cannot find autograder image strip file:)
        message(*** lib/iconstrip-autograder.png)
        message(*** This project cannot run without this file present.)
        message(*** Place the file into your lib/ folder and try again.)
        message(*******************************************************************)
        message("")
        error(Exiting.)
    }
}
###############################################################################
# END SECTION FOR CS 106B/X AUTOGRADER PROGRAMS                               #
###############################################################################


###############################################################################
# BEGIN SECTION FOR DEFINING HELPER FUNCTIONS FOR RESOURCE COPYING            #
###############################################################################

COPY_RESOURCE_FILES_INPUT = ""

win32 {
    # copy addr2line exe program(s) to capture stack traces on Windows
    exists($$PWD/lib/*.exe) {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/lib/*.exe)
    }
    # optional .dll files for network features on Windows
    exists($$PWD/lib/*.dll) {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/lib/*.dll)
    }
}
# copy icons and image strips for library GUI/console windows
exists($$PWD/lib/*.png) {
    COPY_RESOURCE_FILES_INPUT += $$files($$PWD/lib/*.png)
}
exists($$PWD/*.txt) {
    COPY_RESOURCE_FILES_INPUT += $$files($$PWD/*.txt)
}
exists($$PWD/res/*) {
    win32 {
        exists($$PWD/res/*/*) {
            # this will preserve the nested res/ directory structure on Windows,
            # at the unfortunate cost of copying res/ to build dir on every run;
            # recommended: avoid res subfolders to avoid this penalty on Windows
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/res)
        } else {
            # res/ with no subdirectories will properly be copied only on modification
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/res/*.*)
        }
    } else {
        # Mac/Linux have proper recursive file-copying semantics in all cases
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/res/*, true)
    }
}
# input/ and output/ have same nested subfolder issue as res/
exists($$PWD/input/*) {
    win32 {
        exists($$PWD/input/*/*) {
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/input)
        } else {
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/input/*.*)
        }
    } else {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/input, true)
    }
}
exists($$PWD/output/*) {
    win32 {
        exists($$PWD/output/*/*) {
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/output)
        } else {
            COPY_RESOURCE_FILES_INPUT += $$files($$PWD/output/*.*)
        }
    } else {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/output, true)
    }
}
exists($$PWD/lib/autograder/*.h) | exists($$PWD/lib/StanfordCPPLib/autograder/*.h) | exists($$PWD/src/autograder/$$PROJECT_FILTER/*.cpp) {
    # in autograder projects, copy over student's .cpp and .h files for possible style checking
    exists($$PWD/*.cpp) {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/*.cpp)
    }
    exists($$PWD/*.h) {
        COPY_RESOURCE_FILES_INPUT += $$files($$PWD/*.h)
    }
}

# platform-specific commands to copy files and folders to build directory
copy_resource_files.name = Copy resource files to the build directory
win32 {
    # https://support.microsoft.com/en-us/help/289483/switches-that-you-can-use-with-xcopy-and-xcopy32-commands
    # /s - copy subfolders
    # /q - quiet (no verbose output)
    # /y - overwrite without prompting
    # /i - if destination does not exist and copying more than one file, assumes destination is a folder
    copy_resource_files.commands = xcopy /s /q /y ${QMAKE_FILE_IN}
} else {
    copy_resource_files.commands = cp -rf ${QMAKE_FILE_IN} .
}
# copy_resource_files.commands = ${COPY_FILE} ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_resource_files.input = COPY_RESOURCE_FILES_INPUT
copy_resource_files.output = $${OUT_PWD}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_resource_files.CONFIG = no_link no_clean target_predeps
QMAKE_EXTRA_COMPILERS += copy_resource_files

###############################################################################
# END SECTION FOR DEFINING HELPER FUNCTIONS FOR RESOURCE COPYING              #
###############################################################################

# END OF FILE (this should be line #560; if not, your .pro has been changed!)
