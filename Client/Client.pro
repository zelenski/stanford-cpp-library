###############################################################################
# @version 2020/04/02
# @author Julie Zelenski
#   beta of client project that builds on installed library
###############################################################################

QUARTER_ID = 20-1

TEMPLATE  = app
QT += core gui widgets multimedia network
CONFIG   += silent
CONFIG   -= console

###############################################################################
# Section 1:  Confirm libcs106.a installed locally                            #
###############################################################################

# Library installed in user's home directory. This will be
# writable even on cluster computer
# Strategy is to specify all paths using forward-slash as separator.
# qmake will rewrite
win32|win64 {
    USER_STORAGE = $$(APPDATA)
    USER_STORAGE = $$replace(USER_STORAGE, \\\\, /)
} else {
    USER_STORAGE = $$(HOME)/.config
}
SPL_PATH = $${USER_STORAGE}/QtProject/qtcreator/cs106

# Library as target, attempt to generate if missing will raise error
library.target = $${SPL_PATH}/lib/libcs106.a
library.commands = $(error CS106 library not installed. See http://cs106b.stanford.edu/qt/library for install instructions)
QMAKE_EXTRA_TARGETS += library
PRE_TARGETDEPS += $${library.target}

# JDZ not a good way to do it.
# Verify cs106 library installed.  JDZ: ideally this should check version too
#!exists($${SPL_PATH}/lib/libcs106.a) {
#    message("[Building "$$TARGET"] No cs106 library installed. See http://cs106b.stanford.edu/qt/library for install instructions.")
#}

###############################################################################
# Section 2:  Configure project to link with libcs106                         #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET = $$replace(TARGET, " ", _)

# set DESTDIR to project root dir, this is where executable/app will deploy and run
DESTDIR = $$PWD

# link against cs106, add library headers to search path
LIBS += -lcs106
QMAKE_LFLAGS = -L$${SPL_PATH}/lib
INCLUDEPATH *= $${SPL_PATH}/include

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
DEFINES += main=studentMain

###############################################################################
# Section 3:   Gather other files to list in Qt Creator project browser       #
###############################################################################

# honeypot to trick Qt Creator into glob while also allowing add files within IDE;
# Qt looks for first 'SOURCES *=' line and adds newly added .cpp/h files there.
# Later in this pro file we glob-add files to SOURCES ourselves but we use
# operator *=  which will unique entries, so no worries about listing things twice
SOURCES *= ""
HEADERS *= ""

# use a recursive search to pick up any .cpp or .h files within the
# project folder (student code, library code, assignment code)
SOURCES *= $$files(*.cpp, true)
HEADERS *= $$files(*.h, true)
INCLUDEPATH *= $$PWD/ src/
# JDZ cwd is later removed with no_include so check this

# Gather resource files (image/sound/etc), list under "Other files"
OTHER_SUBDIRS = res input output
for(dir, OTHER_SUBDIRS): OTHER_FILES *= $$files($${dir}/*, true)
# Also grab text files from root dir
OTHER_FILES *= $$files(*.txt, true)

###############################################################################
#  Section 4:   Configure compile/link flags, use of system libraries         #
###############################################################################

# Configure flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
CONFIG += no_include_pwd         # make sure we do not accidentally #include files placed in 'resources'
CONFIG += sdk_no_version_check   # removes spurious warnings on Mac OS X

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
CONFIG += c++11

# enable extra warnings
QMAKE_CXXFLAGS_WARN_ON -= -Wall -Wextra -W

QMAKE_CXXFLAGS += -Wcast-align
QMAKE_CXXFLAGS += -Wformat=2
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
QMAKE_CXXFLAGS += -Wno-unused-const-variable

*-clang { # warning flags specific to clang
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
    QMAKE_CXXFLAGS += -Wno-useless-cast
}
*-g++ {   # warning flags specific to g++
    QMAKE_CXXFLAGS += -Wlogical-op
}

# LIBRARIES
# thread used on all, other additions per-platform
# on Windows, Dbghelp needed for crash-backtrace
# on Mac and linux, -ldl allows dynamic symbol lookup
LIBS += -lpthread
win32 : LIBS += -lDbghelp
macx|linux : LIBS += -ldl

# build-specific options (debug vs release)

# options for the 'debug' target (default):
# use no optimization, generate debugger symbols,
# and catch/print to console any uncaught exceptions thrown by the program
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g3
    QMAKE_CXXFLAGS += -fno-inline
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer

    linux-g++ {
         # on Linux g++, these flags help us gather line numbers for stack traces
        QMAKE_CXXFLAGS += -export-dynamic
        QMAKE_CXXFLAGS += -Wl,--export-dynamic
        QMAKE_LFLAGS += -export-dynamic
        QMAKE_LFLAGS += -rdynamic
    }
    *-clang {
        QMAKE_LFLAGS += -rdynamic
        QMAKE_LFLAGS += -Wl,-no_pie
    }
}


###############################################################################
#  Section 5:   Detect/report errors in project structure                     #
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
    error(Exiting. Project directory name contains invalid characters $$BAD_CHARS)
}

# END OF FILE (this should be line #195; if not, your .pro has been changed!)
