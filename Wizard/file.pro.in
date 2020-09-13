###############################################################################
# Project file for CS106B/X student program
#
# @version Fall Quarter 09/12/2020
# @author Julie Zelenski
#   build client program using installed static library
###############################################################################

SPL_VERSION = 2020.1

TEMPLATE    =   app
QT          +=  core gui widgets multimedia network
CONFIG      +=  silent               # quieter progress during build
CONFIG      -=  depend_includepath   # library headers not changing, don't add depend

###############################################################################
#       Find/use installed version of cs106 lib and headers                   #
###############################################################################

# Library installed into per-user writable data location from QtStandardPaths
win32|win64     { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR   =   $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)

SPL_DIR         =   $${USER_DATA_DIR}/cs106
STATIC_LIB      =   $$system_path($${SPL_DIR}/lib/libcs106.a)

# Confirm presence of lib before build using extra target as prereq
check_lib.target    =  "$${STATIC_LIB}"
check_lib.commands  =  $(error CS106 library not found. See http://cs106b.stanford.edu/qt for library install instructions)
QMAKE_EXTRA_TARGETS +=  check_lib
PRE_TARGETDEP       +=  $${check_lib.target}

# link against libcs106.a, add library headers to search path
# libcs106 requires libpthread, add link here
LIBS            +=  -lcs106 -lpthread
QMAKE_LFLAGS    =   -L$$shell_quote($${SPL_DIR}/lib)
# put PWD first in search list to allow local copy to shadow if needed
INCLUDEPATH     +=  $$PWD "$${SPL_DIR}/include"

###############################################################################
#       Configure project with custom settings                                #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET      =   $$replace(TARGET, " ", _)

# set DESTDIR to project root dir, this is where executable/app will deploy and run
DESTDIR     =   $$PWD

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
# Ask Julie if you are curious why main->qMain->studentMain
DEFINES     +=  main=qMain qMain=studentMain

###############################################################################
#       Gather files to list in Qt Creator project browser                    #
###############################################################################

# honeypot to trick Qt Creator to allow glob-all to coexist with user-added files
# Qt looks for first 'SOURCES *=' line and lists user-added .cpp/h files there.
# Afterward we glob-add files to SOURCES ourselves. Operator *= will unique
# entries, so no worries about duplicates
SOURCES         *=  ""
HEADERS         *=  ""

# Gather any .cpp or .h files within the project folder (student/starter code).
# Second argument true makes search recursive
SOURCES         *=  $$files(*.cpp, true)
HEADERS         *=  $$files(*.h, true)

# Gather resource files (image/sound/etc) from res dir, list under "Other files"
OTHER_FILES     *=  $$files(res/*, true)
# Gather text files from root dir or anywhere recursively
OTHER_FILES     *=  $$files(*.txt, true)

###############################################################################
#       Configure compiler, compile flags                                     #
###############################################################################

# Configure flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off to avoid confusion.)

CONFIG          +=  sdk_no_version_check   # removes spurious warnings on Mac OS X

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
CONFIG          +=  c++11

# WARN_ON has -Wall -Wextra, add/remove a few specific warnings
QMAKE_CXXFLAGS_WARN_ON      +=  -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON      +=  -Werror=uninitialized
QMAKE_CXXFLAGS_WARN_ON      +=  -Wunused-parameter
QMAKE_CXXFLAGS_WARN_ON      +=  -Wmissing-field-initializers
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-old-style-cast
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-sign-compare
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-sign-conversion
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-unused-const-variable

*-clang { # warning flags specific to clang
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wempty-init-stmt
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wignored-qualifiers
}

*-g++ {   # warning flags specific to g++
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wlogical-op
}

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

# END OF FILE (this should be line #147; if not, your .pro has been changed!)
