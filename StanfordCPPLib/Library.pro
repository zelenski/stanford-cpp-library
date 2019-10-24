###############################################################################
# @author Julie Zelenski
# @version 2019/10/21
#   first attempt at building 106 code into static library
#   make install will publish lib in QT plugins dir and can be used by student
#   application without having to rebuild entire library within each project
#   only operates in autograder mode for now
###############################################################################

DEFINES += SPL_PROJECT_VERSION=20191021   # kludgy YYYYMMDD constant used by lib to know its version

# Versioning
# ----------
QUARTER_ID = 19-1
REQUIRES_QT_VERSION = 5.11

# Top-level configuration
# -----------------------
# This project manages source/headers for CS106B/X library
# It builds into a static library (libcs106.a)
# which is installed into directory qt_plugins/cs106
TARGET = cs106
TEMPLATE = lib
CONFIG += staticlib
INSTALL_PATH = $$[QT_INSTALL_PLUGINS]/$${TARGET}

# Below we include one-line into makefile to set
#     .DEFAULT_GOAL := install
# this means make will install by default
# (yes, this is a cheezy hack, is there a cleaner way?)
QMAKE_EXTRA_INCLUDES += $$relative_path($$PWD/default-goal-install.mk, $$OUT_PWD)

CONFIG += c++11
CONFIG += develop_mode
CONFIG += autograder
#CONFIG += silent


LIB_SUBDIRS = collections graphics io system util
autograder: LIB_SUBDIRS += autograder

# Glob source and header files
# (track public/private headers separately for install)
for(dir, LIB_SUBDIRS): PUBLIC_HEADERS *= $$files($${dir}/*.h)
PRIVATE_HEADERS *= $$files(private/*.h)
for(dir, LIB_SUBDIRS): SOURCES *= $$files($${dir}/*.cpp)
SOURCES *= $$files(private/*cpp)

HEADERS *= $$PUBLIC_HEADERS $$PRIVATE_HEADERS

# Glob other files from resources
OTHER_FILES *= $$files(resources/*)

# set include path for all library folders, also QT headers
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
    CONFIG += silent
} 

#JDZ to try on Windows
#QMAKE_CXXFLAGS += -O2 -finline

# JDZ: current always builds library in autograder mode, needs rework!
autograder {
    DEFINES += SPL_AUTOGRADER_MODE
    DEFINES += SPL_GRAPHICAL_AUTOGRADER
}

# wrapper name for 'main' function (needed so student can write 'int main'
# but our library can grab the actual main function to initialize itself)
DEFINES += SPL_REPLACE_MAIN_FUNCTION=1
DEFINES += main=qMain


# x/y location and w/h of the graphical console window; set to -1 to center
DEFINES += SPL_CONSOLE_X=-1
DEFINES += SPL_CONSOLE_Y=-1
DEFINES += SPL_CONSOLE_WIDTH=900
DEFINES += SPL_CONSOLE_HEIGHT=550

# echo graphical console onto the plain text console as well?
DEFINES += SPL_CONSOLE_ECHO

# quit the C++ program when the graphical console is closed?
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE

# print details about uncaught exceptions with red error text / stack trace
DEFINES += SPL_CONSOLE_PRINT_EXCEPTIONS

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


# make install target
# -------------------
# install target will install files as QT plug-ins
# files copied are static library (libcs106.a) and
# public headers (flattened), private headers (in subdir)
target.path = $${INSTALL_PATH}
private_headers.path = $${target.path}/private
public_headers.path = $$target.path
public_headers.files = $$PUBLIC_HEADERS
private_headers.files = $$PRIVATE_HEADERS
INSTALLS += target public_headers private_headers



# Requirements
# ------------
# Error if installed version of QT is insufficient

requirements = $$split(REQUIRES_QT_VERSION, ".")
lessThan(QT_MAJOR_VERSION, $$member(requirements, 0)) | equals(QT_MAJOR_VERSION, $$member(requirements, 0)):lessThan(QT_MINOR_VERSION, $$member(requirements, 1)) {
    error("The CS106 library for quarter $$QUARTER_ID requires Qt $$REQUIRES_QT_VERSION or newer; Qt $$[QT_VERSION] was detected. Please upgrade/re-instal.l")
} else {
    message("The CS106 library for quarter $$QUARTER_ID requires Qt $$REQUIRES_QT_VERSION or newer; Qt $$[QT_VERSION] was detected. Thumbs up!")
}
