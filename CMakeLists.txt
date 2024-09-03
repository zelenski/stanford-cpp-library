cmake_minimum_required(VERSION 3.17)
project(stanford-cpp-library)

# This CMake provides an example of how to use CMake to build the
# Stanford cpp library rather than a pro file.
# This allows the use of other IDE's such as CLion, VSC, etc. for
# development.

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
set(CMAKE_CXX_STANDARD 11)

set(CS106_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Library")

# We try to make a best guess based on the OS, but you may
# need to manually specify the path to the qt CMake.
if (WIN32)
    # MinGW is recommended for Windows.  Please note that
    # Qt is very picky about the MinGW compiler.  Be sure
    # you are using the most up to date version of MinGW.
    set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64")

    # Alternatively MSVC 2019 compiler can be used (Not tested)
    # set(CMAKE_PREFIX_PATH "C:/Qt6/6.7.2/msvc2019_64")
elseif (APPLE)
    set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.7.2/macos")
else ()
    # UNIX/Linux case
    set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.7.2/gcc_64")
endif ()

# Verify CMAKE_PREFIX_PATH exists
if (NOT EXISTS ${CMAKE_PREFIX_PATH})
    message(WARNING "The path ${CMAKE_PREFIX_PATH} does not exist"
            "please verify the path to your Qt installation")
endif ()

set(CMAKE_AUTOMOC ON)  # Automatic handling of the Qt Meta-Object Compiler (MOC)
set(CMAKE_AUTORCC ON)  # Automatic handling of the Qt Resource compiler
set(CMAKE_AUTOUIC ON)  # Automatic handling of the Qt UI code generator

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

# Include all the cs106 library folders
include_directories("${CS106_LIB_PATH}")
include_directories("${CS106_LIB_PATH}/collections")
include_directories("${CS106_LIB_PATH}/console")
include_directories("${CS106_LIB_PATH}/graphics")
include_directories("${CS106_LIB_PATH}/io")
include_directories("${CS106_LIB_PATH}/private")
include_directories("${CS106_LIB_PATH}/resources")
include_directories("${CS106_LIB_PATH}/system")
include_directories("${CS106_LIB_PATH}/util")
include_directories("${CS106_LIB_PATH}/testing")


# Copy resources folder into per-user writable data location from QtStandardPaths
if (WIN32)
    set(QTP_EXE "qtpaths6.exe")
else()
    set(QTP_EXE "qtpaths6")
endif ()

set (QT_INSTALL_BINS "${CMAKE_PREFIX_PATH}/bin")

# Find writable data directory
execute_process(COMMAND "${QT_INSTALL_BINS}/${QTP_EXE}" "--writable-path" "GenericDataLocation" 
        OUTPUT_VARIABLE USER_DATA_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

# Copy resources folder into data directory
set (SPL_DIR "${USER_DATA_DIR}/cs106")
file(COPY "${CS106_LIB_PATH}/resources" DESTINATION "${SPL_DIR}")

# Used in build.h.in file to generate build.h file
set (SPL_VERSION "2023.1")
string(TIMESTAMP TODAY "%d/%m/%Y")
set (SPL_BUILD_DATE ${TODAY})

# Find user from environment variable
# Environment variable is either USERNAME or USER
if ("$ENV{USER}" STREQUAL "")
    set (SPL_BUILD_USER $ENV{USERNAME})
else ()
    set (SPL_BUILD_USER "$ENV{USER}")
endif ()

# Generate build.h file
add_definitions(-DSPL_CMAKE_BUILD)
configure_file(
        ${CS106_LIB_PATH}/private/build.h.in
        ${CS106_LIB_PATH}/private/build.h 
        @ONLY
)

# Create list of source files
file(GLOB LIBRARY_SRC CONFIGURE_DEPENDS
        "${CS106_LIB_PATH}/collections/*.cpp"
        "${CS106_LIB_PATH}/console/*.cpp"
        "${CS106_LIB_PATH}/graphics/*.cpp"
        "${CS106_LIB_PATH}/io/*.cpp"
        "${CS106_LIB_PATH}/system/*.cpp"
        "${CS106_LIB_PATH}/util/*.cpp"
        "${CS106_LIB_PATH}/testing/*.cpp"
        "${CS106_LIB_PATH}/private/*.cpp"
)

# Create list of header files
file(GLOB LIBRARY_HEADER CONFIGURE_DEPENDS
        "${CS106_LIB_PATH}/collections/*.h"
        "${CS106_LIB_PATH}/console/*.h"
        "${CS106_LIB_PATH}/graphics/*.h"
        "${CS106_LIB_PATH}/io/*.h"
        "${CS106_LIB_PATH}/system/*.h"
        "${CS106_LIB_PATH}/util/*.h"
        "${CS106_LIB_PATH}/testing/*.h"
        "${CS106_LIB_PATH}/private/*.h"
)

file(GLOB QT_RESOURCES ${CS106_LIB_PATH}/images.qrc)

# We need to say what libraries we need from Qt6
# Qt is very picky about the compiler version.
# Make sure you check to make sure your compiler
# is up to date.  You have been warned!
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

qt_add_resources(RCC_SOURCES ${QT_RESOURCES})

# https://doc.qt.io/qt-6/cmake-get-started.html
# Create static library
qt_add_library(CS106_library STATIC
        ${LIBRARY_SRC}
        ${LIBRARY_HEADER}
        ${RCC_SOURCES}
)

# Link CS106 library to required Qt libraries
target_link_libraries(CS106_library PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

# Call CMakeLists.txt files of subdirectories
add_subdirectory(Welcome)
add_subdirectory(SPL-unit-tests)
add_subdirectory(SimpleTestGuide)

# TODO: These need to be fixed/tested
# add_subdirectory(RandomClientTests/BugFixes)
# add_subdirectory(RandomClientTests/CompileFlags)
# add_subdirectory(RandomClientTests/ShadowTest)
