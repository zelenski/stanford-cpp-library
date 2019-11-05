###############################################################################
# Project file for CS106B/X Library 
#
# @author Julie Zelenski
# @version 2019/11/02
# This version still in development
###############################################################################

# Versioning
# ----------
QUARTER_ID = 19-1
REQUIRES_QT_VERSION = 5.11
VERSION = 19.1
# JDZ: above version gives auto access to qmake major/minor/patch and requires()

#CONFIG += develop_mode


# Top-level configuration
# -----------------------
# CS106B/X library project
# Builds static library libcs106.a to be installed in fixed location
# along with library headers and shared resources (icons, binaries, etc.)
# Student client program accesses library+resources from fixed location

TARGET = cs106
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= depend_includepath
CONFIG += c++11


# Install in user's home directory, along with wizard. This will be
# writable even on cluster computer
# Note strategy is to specify all paths using forward-slash as separator
win32|win64 {
    USER_STORAGE = $$(APPDATA)
    USER_STORAGE = $$replace(USER_STORAGE, \\\\, /)
} else {
    USER_STORAGE = $$(HOME)/.config
}
INSTALL_PATH = $${USER_STORAGE}/QtProject/qtcreator/cs106


LIB_SUBDIRS = autograder collections graphics io system util

# Glob source and header files
# (track public/private headers separately for install)
for(dir, LIB_SUBDIRS): PUBLIC_HEADERS *= $$files($${dir}/*.h)
PRIVATE_HEADERS *= $$files(private/*.h)
for(dir, LIB_SUBDIRS): SOURCES *= $$files($${dir}/*.cpp)
SOURCES *= $$files(private/*cpp)

HEADERS *= $$PUBLIC_HEADERS $$PRIVATE_HEADERS

# Glob other files from resources
RES_FILES = $$files(resources/*)
OTHER_FILES *= $$RES_FILES

# Set include path for all library folders, also QT headers
INCLUDEPATH *= $$LIB_SUBDIRS
QT += core gui widgets network multimedia

# When in develop_mode, enable warnings, deprecated, nit-picks, all of it.
# Pay attention and fix! Published version should trigger no warnings.
# Future update may foil our best-laid plans, so suppress
# warnings when !develop_mode to shield students from these surprises
develop_mode {
    CONFIG += warn_on
    QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra
    DEFINES += QT_DEPRECATED_WARNINGS
} else {
    CONFIG += warn_off
    CONFIG += sdk_no_version_check
    #CONFIG += silent
} 

#JDZ to try on Windows
#QMAKE_CXXFLAGS += -O2 -finline

DEFINES += SPL_INSTALL_DIR=\\\"$${INSTALL_PATH}\\\" QUARTER_ID=\\\"$${QUARTER_ID}\\\"

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

# should we attempt to precompile the Qt moc_*.cpp files for speed?
DEFINES += SPL_PRECOMPILE_QT_MOC_FILES


# Requirements
# ------------
# Error if installed version of QT is insufficient

!versionAtLeast(QT_VERSION, $$REQUIRES_QT_VERSION) {
    error("The CS106 library for quarter $$QUARTER_ID requires Qt $$REQUIRES_QT_VERSION or newer; Qt $$[QT_VERSION] was detected. Please upgrade/re-install.")
} else {
    !buildpass:log("Thumbs up! Qt $$[QT_VERSION] installed (meets quarter $$QUARTER_ID requirement of Qt $$REQUIRES_QT_VERSION or newer)")
}

# JDZ: standard make install target always copies, no check if installed already up-to-date
# so, do it manually, sigh 
# (refer mkspecs/features/file_copies for custom compiler definition to piggyback on)

CONFIG += file_copies
COPIES += resources headers
resources.files = $$RES_FILES
resources.path = $${INSTALL_PATH}/res
headers.files = $$PUBLIC_HEADERS
headers.path = $${INSTALL_PATH}/include

# JDZ: kind of cheezy, why is windows output in different path?
win32 {
    QMAKE_POST_LINK += $$QMAKE_QMAKE -install qinstall debug/libcs106.a $${INSTALL_PATH}/lib/libcs106.a
} else {
    QMAKE_POST_LINK += $$QMAKE_QMAKE -install qinstall libcs106.a $${INSTALL_PATH}/lib/libcs106.a
}
# JDZ: this doesn't work correctly on MacOS, flag lost by qmake
QMAKE_RANLIB +=  -no_warning_for_no_symbols
