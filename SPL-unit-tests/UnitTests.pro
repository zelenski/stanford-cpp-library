#####################################################################
## Stanford CS 106 B/X Qt Creator project file                     ##
#####################################################################
#
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
# @author Julie Zelenski
# @version 2020/04/02

DEFINES += SPL_PROJECT_VERSION=20200402   # kludgy YYYYMMDD constant used by lib to know its version

# global Qt/project settings
TEMPLATE = app
QT       += core gui widgets multimedia network

###############################################################################
# Section 1:  Configure for CS106-specific project                            #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET = $$replace(TARGET, " ", _)

# set DESTDIR to project root dir, this is where executable/app will deploy and run
DESTDIR = $$PWD


###############################################################################
# Section 2:   Gather other files to list in Qt Creator project browser       #
###############################################################################

# honeypot to trick Qt Creator so that adding files works from within IDE;
# Qt looks for first 'SOURCES *=' line and adds newly added .cpp/h files there.
# Later in this pro file we glob-add files to SOURCES ourselves but we use
# operator *=  which will unique entries, so no worries about listing things twice
HEADERS *= ""
SOURCES *= ""

# use a recursive search to pick up any .cpp or .h files within the
# project folder (student code, library code, assignment code)
SOURCES *= $$files(*.cpp, true)
HEADERS *= $$files(*.h, true)

# add SPL header files compiler search path
INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/
SPL_DIRS = collections graphics io system util
for(d, SPL_DIRS): INCLUDEPATH *= $$PWD/lib/StanfordCPPLib/$${d}
INCLUDEPATH *= .

# Gather resource files (image/sound/etc), list under "Other files"
OTHER_SUBDIRS = res input output
for(d, OTHER_SUBDIRS): OTHER_FILES *= $$files($${d}/*, true)


###############################################################################
#  Section 3:   Configure compile/link flags, use of system libraries         #
###############################################################################

# set up flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
CONFIG += no_include_pwd         # make sure we do not accidentally #include files placed in 'resources'
CONFIG += sdk_no_version_check   # removes spurious warnings on Mac OS X
# CONFIG += warn_off

# gives us a bit more precision about which errors are printed

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
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

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

# scale up the console window on high-DPI screens?
# DEFINES += SPL_SCALE_HIGH_DPI_SCREEN

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

# precompiled header speeds up build times
!win32 {
PRECOMPILED_HEADER = $$files($$PWD/lib/StanfordCPPLib/private/precompiled.h)
}


###############################################################################
#  Section 4:   Detect/report errors in project structure                     #
###############################################################################

# error if project was opened from within a ZIP archive (common mistake on Windows)
win32|win64 {
    contains(PWD, .*\.zip.*) | contains(PWD, .*\.ZIP.*) {
        message(*******************************************************************)
        message(*** ERROR: You are trying to open this project from within a .ZIP archive:)
        message(*** $$PWD)
        message(*** You need to extract the files out of the ZIP file first.)
        message(*** Open the ZIP in your file explorer and press the Extract button.)
        message(*******************************************************************)
        error(Exiting. Need to extract project from ZIP first.)
    }
}

# error if non-standard chars in path (common issue for intl students)
BAD_CHARS = $$PWD
BAD_CHARS ~= s|[a-zA-Z0-9_ .\/:+-]+|
!isEmpty(BAD_CHARS) {
    message(*******************************************************************)
    message(*** ERROR: The name of your project directory contains invalid characters.)
    message(*** $$PWD)
    message(***)
    message(*** The invalid characters are: $$BAD_CHARS)
    message(***)
    message(*** The name cannot contain punctuation or characters from international)
    message(*** alphabets such as Chinese or Korean.)
    message(*** Rename or move your project to a directory with a simple name)
    message(*** such as "C:\Programs\Homework1" and try again.)
    message(*******************************************************************)
    error(Exiting. Project directory name contains invalid characters $$BAD_CHARS (see Compile Output tab))
}

# error if Stanford C++ library files are not present in project
!exists($$PWD/lib/StanfordCPPLib/private/version.h) {
    message(*******************************************************************)
    message(*** ERROR: Stanford C++ library not found!)
    message(*** This project cannot run without the folder lib/StanfordCPPLib/.)
    message(*** Place that folder into your project and try again.)
    message(*******************************************************************)
    error(Exiting. Project is missing Stanford C++ library files (see Compile Output tab))
}

# warn if console tool bar image not available
!exists($$PWD/lib/iconstrip.png) {
    message(*******************************************************************)
    message(*** ERROR: Stanford library cannot find image strip file:)
    message(*** lib/iconstrip.png)
    message(*** Place that file into your lib/ folder and try again.)
    message(*******************************************************************)
    warning(Console tool bar icons unavailable. Project is missing image strip file)
}

# END OF FILE (this should be line #291; if not, your .pro has been changed!)
