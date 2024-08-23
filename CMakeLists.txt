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

# Make sure you have an updated version of MinGW
# set(CMAKE_PREFIX_PATH "C:/PATH_TO_COMPILER")
# Alternatively it can also be MSVC compiler (not tested)
# set(CMAKE_PREFIX_PATH "C:/Qt6/6.2.4/msvc2019_64")
set(CMAKE_PREFIX_PATH "C:/Qt6/6.2.4/mingw_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Special sauce to get qt resources to work
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick QuickControls2 Widgets Qt6QuickCompiler REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 Widgets REQUIRED)

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
        ${PRIVATE_SRC}
        ${LIBRARY_HEADER}
        ${PRIVATE_HEADER}
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
