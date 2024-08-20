cmake_minimum_required(VERSION 3.17)
project(Welcome)

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
set(CMAKE_CXX_STANDARD 11)

# Need latest version of MinGW.
# You may need to specify the direct path.
# You can get MinGW from here: https://winlibs.com/
#set(CMAKE_PREFIX_PATH "C:/PATH_TO_MINGW")
set(CMAKE_PREFIX_PATH "C:/Qt6/6.2.4/mingw_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Special sauce to get qt resources to work
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick QuickControls2 Widgets Qt6QuickCompiler REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 Widgets REQUIRED)


# Include all the cs106 library folders
include_directories("Library")
include_directories("Library/collections")
include_directories("Library/console")
include_directories("Library/graphics")
include_directories("Library/io")
include_directories("Library/private")
include_directories("Library/resources")
include_directories("Library/system")
include_directories("Library/util")
include_directories("Library/testing")
include_directories("Welcome")

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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Library/private/build.h.in 
        ${CMAKE_CURRENT_SOURCE_DIR}/Library/private/build.h @ONLY)

# Create list of source files
file(GLOB LIBRARY_SRC CONFIGURE_DEPENDS
        "Library/collections/*.cpp"
        "Library/console/*.cpp"
        "Library/graphics/*.cpp"
        "Library/io/*.cpp"
        "Library/system/*.cpp"
        "Library/util/*.cpp"
        "Library/testing/*.cpp"
        "Library/private/*.cpp"
        )

# Remove to avoid extra redefinitions
list(REMOVE_ITEM LIBRARY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Library/private/filelibwindows.cpp")
list(REMOVE_ITEM LIBRARY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Library/private/filelibunix.cpp")

file(GLOB QT_RESOURCES Library/images.qrc)

# We need to say what libraries we need from Qt6
# Qt is very picky about the compiler version.
# Make sure you check to make sure your compiler
# is up to date.  You have been warned!
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

qt_add_resources(RCC_SOURCES ${QT_RESOURCES})

# https://doc.qt.io/qt-6/cmake-get-started.html
## Create static library
qt_add_library(CS106_library STATIC
        ${LIBRARY_SRC}
        ${PRIVATE_SRC}
        ${LIBRARY_HEADER}
        ${PRIVATE_HEADER}
        ${RCC_SOURCES}
)

# Link CS106 library to required Qt libraries
target_link_libraries(CS106_library PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

add_executable(${PROJECT_NAME}
        Welcome/Welcome.cpp
)

# link exe to CS106 library
target_link_libraries(${PROJECT_NAME} CS106_library)

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
# Ask Julie if you are curious why main->qMain->studentMain
target_compile_definitions(${PROJECT_NAME} PRIVATE main=qMain)
target_compile_definitions(${PROJECT_NAME} PRIVATE qMain=studentMain)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

if (WIN32)
    set(DEBUG_SUFFIX)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB ${REQUIRED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
