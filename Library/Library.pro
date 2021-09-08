###############################################################################
# Project file for CS106B/X Library
#
# @author Julie Zelenski
# @version Fall Quarter 09/12/2020
#    build static lib and install into user data
###############################################################################

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
    QMAKE_CXXFLAGS  +=  -Wall -Wextra
    QMAKE_CXXFLAGS  +=  -Wno-inconsistent-missing-override
    DEFINES         +=  QT_DEPRECATED_WARNINGS
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
QMAKE_EXTRA_INCLUDES += $$relative_path($$absolute_path(assume_install.mk), $$OUT_PWD)

win32|win64 { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR = $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)
SPL_DIR = $${USER_DATA_DIR}/cs106

target.path         =   "$${SPL_DIR}/lib"
headers.files       =   $$PUBLIC_HEADERS
headers.path        =   "$${SPL_DIR}/include"
INSTALLS            +=  target headers debughelper

debughelper.files   =   personaltypes.py
mac         { debughelper.path = "$$(HOME)/Qt/Qt Creator.app/Contents/Resources/debugger" }
win32|win64 { debughelper.path = "C:\Qt\Tools\QtCreator\share\qtcreator\debugger" }
unix:!mac   { debughelper.path = "$$(HOME)/Qt/Tools/QtCreator/share/qtcreator/debugger" }
!build_pass:!exists($$debughelper.path) {
    warning("Debug helper: no such path $$debughelper.path")
    debughelper.path = "$${SPL_DIR}"
}

###############################################################################
#       Requirements                                                          #
###############################################################################

!versionAtLeast(QT_VERSION, $$REQUIRES_QT_VERSION) {
    error(The CS106 library for $$SPL_VERSION requires Qt $$REQUIRES_QT_VERSION or newer;\
         $$[QT_VERSION] was detected on your computer. Please upgrade/re-install.)
}
