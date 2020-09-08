###############################################################################
# Project file for CS106B/X Library
#
# @author Julie Zelenski
# @version 2020/04/02
#   beta version to build and install static lib
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
TARGET = cs106
CONFIG += staticlib

REQUIRES_QT_VERSION = 5.11
SPL_VERSION = 2020.1

###############################################################################
#       Gather files                                                          #
###############################################################################

LIB_SUBDIRS = collections console graphics io system util

for(dir, LIB_SUBDIRS): PUBLIC_HEADERS *= $$files($${dir}/*.h)
PRIVATE_HEADERS *= $$files(private/*.h)
HEADERS *= $$PUBLIC_HEADERS $$PRIVATE_HEADERS

for(dir, LIB_SUBDIRS): SOURCES *= $$files($${dir}/*.cpp)
SOURCES *= $$files(private/*.cpp)

RESOURCES = images.qrc

INCLUDEPATH += $$LIB_SUBDIRS
QT += core gui widgets network multimedia

###############################################################################
#       Build settings                                                        #
###############################################################################

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
CONFIG += c++11

# CONFIG += develop_mode
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
    CONFIG += silent
}

# JDZ to throw exceptions when a collection iterator is used after it has
# been invalidated (e.g. if you remove from a Map while iterating over it)
DEFINES += SPL_THROW_ON_INVALID_ITERATOR

# should we attempt to precompile the Qt moc_*.cpp files for speed?
DEFINES += SPL_PRECOMPILE_QT_MOC_FILES

# Installation
# ------------
# make install is kind of what is wanted here (copy headers+lib
# to install location). However make install always copies (no skip if up-to-date
# and nothing was built), install target also requires add make step in Qt Creator,
# both ugh.
# Below are manual steps to copy lib+headers to install dir after each re-compile
#
# Install location is in user's home directory (near location required
# for wizard). This should be writable even on cluster computer.
# Note strategy is to specify all paths using forward-slash separator
# which requires goopy rework of APPDATA environment variable
QMAKE_SUBSTITUTES = private/build.h.in

###############################################################################
#       Make install                                                          #
###############################################################################
>>>>>>> 248b5ffc... Version info written to library, shown in info panel

# Use makefile include to set default goal to install target
QMAKE_EXTRA_INCLUDES += $${PWD}/assume_install.mk

# Library installed into per-user location. (writable even on shared/cluster)
# Specify all paths using forward-slash as separator, let qmake rewrite as needed
win32|win64 {
    USER_STORAGE = $$(APPDATA)
} else {
    USER_STORAGE = $$(HOME)/.config
}
INSTALL_PATH = $${USER_STORAGE}/QtProject/qtcreator/cs106

for(h, PUBLIC_HEADERS): TO_COPY *= $$relative_path($$PWD, $$OUT_PWD)/$$h
QMAKE_POST_LINK = @echo "Installing into "$${INSTALL_PATH} && $(COPY_FILE) $$TO_COPY $${INSTALL_PATH}/include

HEADER_DEST = $${INSTALL_PATH}/include
# provide string for info panel without any funky/unescaped chars
INSTALLED_LOCATION = "QtProject/qtcreator/cs106 of user's home config"

target.path += $${INSTALL_PATH}/lib

headers.files = $$PUBLIC_HEADERS
headers.path = $${INSTALL_PATH}/include


INSTALLS += target headers

###############################################################################
#       Requirements                                                          #
###############################################################################

!versionAtLeast(QT_VERSION, $$REQUIRES_QT_VERSION) {
    error(The CS106 library for $$SPL_VERSION requires Qt $$REQUIRES_QT_VERSION or newer; Qt $$[QT_VERSION] was detected on your computer. Please upgrade/re-install.)
}
