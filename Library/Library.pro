###############################################################################
# Project file for CS106B/X Library
#
# @author Julie Zelenski
# @version Fall Quarter 08/28/2020
#    build static lib and install into user data
###############################################################################

# Top-level configuration
# -----------------------
# CS106B/X library project
# Builds static library libcs106.a to be installed in fixed location
# along with library headers and shared resources (icons, binaries, etc.)
# Student client program accesses library+resources from fixed location

TEMPLATE    =   lib
TARGET      =   cs106
CONFIG      +=  staticlib

SPL_VERSION         =   2020.1
REQUIRES_QT_VERSION =   5.15

###############################################################################
#       Gather files                                                          #
###############################################################################

LIB_SUBDIRS         =   collections console graphics io system util

for(dir, LIB_SUBDIRS) {
    PUBLIC_HEADERS  +=  $$files($${dir}/*.h)
    SOURCES         +=  $$files($${dir}/*.cpp)
}

PRIVATE_HEADERS     +=  $$files(private/*.h)
SOURCES             +=  $$files(private/*.cpp)
HEADERS             +=  $$PUBLIC_HEADERS $$PRIVATE_HEADERS

RESOURCES           =   images.qrc
OTHER_FILES         =   personaltypes.py
QMAKE_SUBSTITUTES   =   private/build.h.in

INCLUDEPATH         +=  $$LIB_SUBDIRS
QT                  +=  core gui widgets network multimedia

###############################################################################
#       Build settings                                                        #
###############################################################################

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
CONFIG              +=  c++11

# Set develop_mode to enable warnings, deprecated, nit-picks, all of it.
# Pay attention and fix! Library should compile cleanly.
# Disable mode when publish to quiet build for student.

develop_mode {
    CONFIG          +=  debug
    CONFIG          -=  silent
    CONFIG          +=  warn_on
    QMAKE_CXXFLAGS  += -Wall -Wextra
    QMAKE_CXXFLAGS  += -Wno-inconsistent-missing-override
    DEFINES         += QT_DEPRECATED_WARNINGS
} else {
    CONFIG          +=  warn_off
    CONFIG          +=  sdk_no_version_check
    CONFIG          +=  silent
    CONFIG          +=  release
}

###############################################################################
#       Make install                                                          #
###############################################################################

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
win32|win64 { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR = $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)
SPL_DIR = $${USER_DATA_DIR}/cs106

target.path         =   "$${SPL_DIR}/lib"
headers.files       =   $$PUBLIC_HEADERS
headers.path        =   "$${SPL_DIR}/include"
INSTALLS            +=  target headers

debughelper.files   =   personaltypes.py
mac         { debughelper.path = "$$(HOME)/Qt/Qt Creator.app/Contents/Resources/debugger" }
win32|win64 { debughelper.path = "C:/Qt/Tools/QtCreator/share/qtcreator/debugger" }
unix:!mac   { debughelper.path = "$$(HOME)/Qt/Tools/QtCreator/share/qtcreator/debugger" }
!build_pass:exists($$debughelper.path) {
    INSTALLS        +=  debughelper
} else {
    warning("Skipping debug helper, no such path $$debughelper.path")
}

###############################################################################
#       Requirements                                                          #
###############################################################################

!versionAtLeast(QT_VERSION, $$REQUIRES_QT_VERSION) {
    error(The CS106 library for $$SPL_VERSION requires Qt $$REQUIRES_QT_VERSION or newer;\
         $$[QT_VERSION] was detected on your computer. Please upgrade/re-install.)
}
