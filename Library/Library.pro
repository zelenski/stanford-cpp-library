###############################################################################
# Project file for CS106B/X Library
#
# @author Julie Zelenski
# @version Winter Quarter 2025.2 for Qt 6
#    build static lib and install into user data
###############################################################################

TEMPLATE    =   lib
TARGET      =   cs106
CONFIG      +=  staticlib nostrip

SPL_VERSION         =   2025.2
REQUIRES_QT_VERSION =   6.9

###############################################################################
#       Gather files                                                          #
###############################################################################

LIB_SUBDIRS         =   collections console graphics io system util testing

for(dir, LIB_SUBDIRS) {
    PUBLIC_HEADERS  +=  $$files($${dir}/*.h)
    SOURCES         +=  $$files($${dir}/*.cpp)
}

PRIVATE_HEADERS     +=  $$files(private/*.h)
SOURCES             +=  $$files(private/*.cpp)
HEADERS             +=  $$PUBLIC_HEADERS $$PRIVATE_HEADERS

RESOURCES           =   images.qrc
OTHER_FILES         =   stanfordtypes.py
RESOURCE_FILES      =   $$files(resources/*)
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

win32|win64 {
        # disable g++ interprocedural ra shenanigans (root cause of Lexicon crash 2025/04)
    QMAKE_CXXFLAGS  += -fno-ipa-ra
}


###############################################################################
#       Make install                                                          #
###############################################################################

# Use makefile include to set default goal to install target
QMAKE_EXTRA_INCLUDES += $$relative_path($$absolute_path(assume_install.mk), $$OUT_PWD)

win32|win64 { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR = $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)
SPL_DIR = $${USER_DATA_DIR}/cs106

INSTALLS            +=  target headers resources versionfile debughelper

target.path         =   "$${SPL_DIR}/lib"
headers.files       =   $$PUBLIC_HEADERS
headers.path        =   "$${SPL_DIR}/include"
resources.files     =   $$RESOURCE_FILES
resources.path      =   "$${SPL_DIR}/resources"
versionfile.files   =   spl_version
versionfile.path    =   "$${SPL_DIR}/lib"
QMAKE_SUBSTITUTES   +=  spl_version.in
debughelper.files   =   stanfordtypes.py
mac         { debughelper.path = "$$(HOME)/Qt/Qt Creator.app/Contents/Resources/debugger" }
win32|win64 { debughelper.path = "C:\Qt\Tools\QtCreator\share\qtcreator\debugger" }
unix:!mac   { debughelper.path = "$$(HOME)/Qt/Tools/QtCreator/share/qtcreator/debugger" }
!build_pass:!exists($$debughelper.path) {
    warning("Debug helper: no such path $$debughelper.path")
    debughelper.path = "$${SPL_DIR}"
}

# Add dependency before install to remove any existing SPL_DIR
unix {
    rmexisting.commands = rm -rf \"$$SPL_DIR\"
}
win32 {
    rmexisting.commands = if exist \"$$shell_path($$SPL_DIR)\" rd /s /q \"$$shell_path($$SPL_DIR)\"
}
install.depends = rmexisting
QMAKE_EXTRA_TARGETS += install rmexisting

###############################################################################
#       Requirements                                                          #
###############################################################################

!versionAtLeast(QT_VERSION, $$REQUIRES_QT_VERSION) {
    error(The CS106 library $$SPL_VERSION requires Qt version $$REQUIRES_QT_VERSION and\
         your computer has older Qt version $$[QT_VERSION]. You must uninstall Qt and install version $$REQUIRES_QT_VERSION or newer.)
}
