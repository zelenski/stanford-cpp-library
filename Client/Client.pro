###############################################################################
# Project file for CS106B/X student program
#
# @version Fall Quarter 08/28/2020
# @author Julie Zelenski
#   build client program using installed static library
###############################################################################

SPL_VERSION = 2020.1

TEMPLATE  = app
QT += core gui widgets multimedia network
#CONFIG  += silent               # quieter progress during build
CONFIG  -= depend_includepath   # library headers not changing, don't add depend

###############################################################################
#       Find/use installed version of cs106 lib and headers                   #
###############################################################################

# Library installed into per-user writable data location from QtStandardPaths
win32|win64 { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR = $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)

SPL_DIR = $${USER_DATA_DIR}/cs106
STATIC_LIB = $$system_path($${SPL_DIR}/lib/libcs106.a)

# Extra target used as prereq, confirm presence of lib before build
error_no_lib.target = "$${STATIC_LIB}"
error_no_lib.commands = $(error CS106 library not found. See http://cs106b.stanford.edu/qt for library install instructions)
QMAKE_EXTRA_TARGETS += error_no_lib
PRE_TARGETDEPS += $${error_no_lib.target}

# link project against cs106, add library headers to search path
LIBS += -lcs106
QMAKE_LFLAGS = -L$$shell_quote($${SPL_DIR}/lib)
# put PWD first in search list to allow local copy to shadow if needed
INCLUDEPATH += $$PWD "$${SPL_DIR}/include"

###############################################################################
#       Configure project with custom settings                                #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET = $$replace(TARGET, " ", _)

# set DESTDIR to project root dir, this is where executable/app will deploy and run
DESTDIR = $$PWD

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
# Ask Julie if you are curious why main->qMain->studentMain
DEFINES += main=qMain qMain=studentMain

###############################################################################
#       Gather files to list in Qt Creator project browser                    #
###############################################################################

# honeypot to trick Qt Creator into glob while also allowing add files within IDE;
# Qt looks for first 'SOURCES *=' line and adds newly added .cpp/h files there.
# Later in this pro file we glob-add files to SOURCES to extend entries. Note
# we use operator *=  which uniques entries, so no worries about duplicates
SOURCES *= ""
HEADERS *= ""

# Gather any .cpp or .h files within the project folder (student/starter code).
# Second argument true makes search recursive
SOURCES *= $$files(*.cpp, true)
HEADERS *= $$files(*.h, true)

# Gather resource files (image/sound/etc) from res dir, list under "Other files"
OTHER_FILES *= $$files(res/*, true)
# Gather text files from root dir or anywhere recursively
OTHER_FILES *= $$files(*.txt, true)

###############################################################################
#       Configure compile/link flags, use of system libraries         #
###############################################################################

# Configure flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
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
LIBS += -lpthread


###############################################################################
#       Detect/report errors in project structure                             #
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
