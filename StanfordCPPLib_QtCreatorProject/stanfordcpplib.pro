# Stanford CS 106 B/X Qt Creator project file
#
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
# If you need to add files or folders to your project, we recommend the following:
# - close Qt Creator.
# - delete your ".pro.user" file and "build_xxxxxxx" directory.
# - place the new files/folders into your project directory.
# - re-open and "Configure" your project again.
#
# @author Marty Stepp, Reid Watson, Rasmus Rygaard, Jess Fisher, etc.
# @version 2015/04/09
# - decreased Mac stack size to avoid sporatic crashes on Mac systems
# @version 2014/11/29
# - added pthread library on Mac/Linux for running each test in its own thread
# @version 2014/11/13
# - fixes related to generating stack traces
# - support for putting testing files in a src/test/ folder (used in development)
# @version 2014/11/05
# - improved/fixed flags for exception-handling
# @version 2014/10/31
# - standard autograder-compatible version; should work with all assignments and graders.

TEMPLATE = app

# make sure we do not accidentally #include files placed in 'resources'
CONFIG += no_include_pwd

# checks to ensure that the Stanford C++ library and its associated
# Java back-end are both present in this project
!exists($$PWD/lib/StanfordCPPLib/private/version.h) {
    message(*** Stanford C++ library not found!)
    message(*** This project cannot run without the folder lib/StanfordCPPLib/.)
    message(*** Place that folder into your project and try again.)
    error(Exiting.)
}
!exists($$PWD/lib/spl.jar) {
    message(*** Stanford Java back-end library 'spl.jar' not found!)
    message(*** This project cannot run without spl.jar present.)
    message(*** Place that file into your lib/ folder and try again.)
    error(Exiting.)
}

win32 {
    !exists($$PWD/lib/addr2line.exe) {
        message(*** Stanford C++ library support file 'addr2line.exe' not found!)
        message(*** Our library needs this file present to produce stack traces.)
        message(*** Place that file into your lib/ folder and try again.)
        error(Exiting.)
    }
}

# include various source .cpp files and header .h files in the build process
# (student's source code can be put into project root, or src/ subfolder)
SOURCES += $$PWD/lib/StanfordCPPLib/*.cpp
SOURCES += $$PWD/lib/StanfordCPPLib/stacktrace/*.cpp
exists($$PWD/src/*.cpp) {
    SOURCES += $$PWD/src/*.cpp
}
exists($$PWD/src/test/*.cpp) {
    SOURCES += $$PWD/src/test/*.cpp
}
exists($$PWD/*.cpp) {
    SOURCES += $$PWD/*.cpp
}

HEADERS += $$PWD/lib/StanfordCPPLib/*.h
HEADERS += $$PWD/lib/StanfordCPPLib/private/*.h
HEADERS += $$PWD/lib/StanfordCPPLib/stacktrace/*.h
exists($$PWD/src/*.h) {
    HEADERS += $$PWD/src/*.h
}
exists($$PWD/src/test/*.h) {
    HEADERS += $$PWD/src/test/*.h
}
exists($$PWD/*.h) {
    HEADERS += $$PWD/*.h
}

# set up flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Wreturn-type
QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS += -Wunreachable-code
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-write-strings

unix:!macx {
    QMAKE_CXXFLAGS += -rdynamic
    QMAKE_LFLAGS += -rdynamic
    QMAKE_LFLAGS += -Wl,--export-dynamic
    QMAKE_CXXFLAGS += -Wl,--export-dynamic
}
!win32 {
    QMAKE_CXXFLAGS += -Wno-dangling-field
    QMAKE_CXXFLAGS += -Wno-unused-const-variable
    LIBS += -ldl
}

# increase system stack size (helpful for recursive programs)
win32 {
    QMAKE_LFLAGS += -Wl,--stack,536870912
    LIBS += -lDbghelp
    LIBS += -lbfd
    #LIBS += -liberty
    LIBS += -limagehlp
}
macx {
    #QMAKE_LFLAGS += -Wl,-stack_size,0x2000000
}

# set up flags used internally by the Stanford C++ libraries
DEFINES += SPL_CONSOLE_X=999999
DEFINES += SPL_CONSOLE_Y=999999
DEFINES += SPL_CONSOLE_WIDTH=750
DEFINES += SPL_CONSOLE_HEIGHT=500
DEFINES += SPL_CONSOLE_FONTSIZE=14
DEFINES += SPL_CONSOLE_ECHO
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE
DEFINES += SPL_VERIFY_JAVA_BACKEND_VERSION
DEFINES += SPL_PROJECT_VERSION=20141113

# directories examined by Qt Creator when student writes an #include statement
INCLUDEPATH += $$PWD/lib/StanfordCPPLib/
INCLUDEPATH += $$PWD/lib/StanfordCPPLib/private/
INCLUDEPATH += $$PWD/lib/StanfordCPPLib/stacktrace/
INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/
exists($$PWD/src/test/*.h) {
    INCLUDEPATH += $$PWD/src/test/
}

# build-specific options (debug vs release)
CONFIG(release, debug|release) {
    # make 'release' target be statically linked so it is a stand-alone executable
    # (this code comes from Rasmus Rygaard)
    QMAKE_CXXFLAGS += -O2
    macx {
        QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app && rm $${OUT_PWD}/*.o && rm $${OUT_PWD}/Makefile'
    }
    unix:!macx {
        QMAKE_POST_LINK += 'rm $${OUT_PWD}/*.o && rm $${OUT_PWD}/Makefile'
        QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
    }
    win32 {
        TARGET_PATH = $${OUT_PWD}/release/$${TARGET}.exe
        TARGET_PATH ~= s,/,\\,g

        OUT_PATH = $${OUT_PWD}/
        OUT_PATH ~= s,/,\\,g

        REMOVE_DIRS += $${OUT_PWD}/release
        REMOVE_DIRS += $${OUT_PWD}/debug
        REMOVE_FILES += $${OUT_PWD}/Makefile
        REMOVE_FILES += $${OUT_PWD}/Makefile.Debug
        REMOVE_FILES += $${OUT_PWD}/Makefile.Release
        REMOVE_FILES += $${OUT_PWD}/object_script.$${TARGET}.Release
        REMOVE_FILES += $${OUT_PWD}/object_script.$${TARGET}.Debug
        REMOVE_DIRS ~= s,/,\\,g
        REMOVE_FILES ~= s,/,\\,g

        QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
        QMAKE_POST_LINK += 'move $${TARGET_PATH} $${OUT_PWD} \
            && rmdir /s /q $${REMOVE_DIRS} \
            && del $${REMOVE_FILES}'
    }
}
CONFIG(debug, debug|release) {
    # make 'debug' target use no optimization, generate debugger symbols,
    # and catch/print any uncaught exceptions thrown by the program
    QMAKE_CXXFLAGS += -O0
    QMAKE_CXXFLAGS += -g3
    QMAKE_CXXFLAGS += -ggdb3
    DEFINES += SPL_CONSOLE_PRINT_EXCEPTIONS
}

# This function copies the given files to the destination directory.
# Used to place important resources from res/ and spl.jar into build/ folder.
defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$OUT_PWD

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        !win32 {
            copyResources.commands += cp -r '"'$$FILE'"' '"'$$DDIR'"' $$escape_expand(\\n\\t)
        }
        win32 {
            copyResources.commands += xcopy '"'$$FILE'"' '"'$$DDIR'"' /e /y $$escape_expand(\\n\\t)
        }
    }
    export(copyResources.commands)
}

!win32 {
    copyToDestdir($$files($$PWD/res/*))
    copyToDestdir($$files($$PWD/lib/*.jar))
    exists($$PWD/*.txt) {
        copyToDestdir($$files($$PWD/*.txt))
    }
}
win32 {
    copyToDestdir($$PWD/res)
    copyToDestdir($$PWD/lib/*.jar)
    copyToDestdir($$PWD/lib/addr2line.exe)
    exists($$PWD/*.txt) {
        copyToDestdir($$PWD/*.txt)
    }
}

copyResources.input += $$files($$PWD/lib/*.jar)
win32 {
    copyResources.input += $$files($$PWD/lib/addr2line.exe)
}
copyResources.input += $$files($$PWD/res/*)
exists($$PWD/*.txt) {
    copyResources.input += $$files($$PWD/*.txt)
}

OTHER_FILES += $$files(res/*)
exists($$PWD/*.txt) {
    OTHER_FILES += $$files($$PWD/*.txt)
}

QMAKE_EXTRA_TARGETS += copyResources
POST_TARGETDEPS += copyResources

# Platform-specific project settings to reduce warnings on Mac OS X systems
macx {
    cache()
    QMAKE_MAC_SDK = macosx
}

# ================== END GENERAL PROJECT SETTINGS ==================

# settings specific to CS 106 B/X auto-grading programs; do not modify
exists($$PWD/lib/autograder/*.cpp) {
    # include the various autograder source code and libraries in the build process
    SOURCES += $$PWD/lib/autograder/*.cpp
    HEADERS += $$PWD/lib/autograder/*.h
    INCLUDEPATH += $$PWD/lib/StanfordCPPLib/private/
    INCLUDEPATH += $$PWD/lib/autograder/
    DEFINES += SPL_AUTOGRADER_MODE

    # a check to ensure that required autograder resources are present in this project
    !exists($$PWD/res/autograder/pass.gif) {
        message(*** Stanford library cannot find its image files pass.gif, fail.gif, etc.!)
        message(*** This project cannot run without those images present.)
        message(*** Place those files into your res/autograder/ folder and try again.)
        error(Exiting.)
    }

    # copy autograder resource files into build folder
    copyResources.input += $$files($$PWD/res/autograder/*)
    OTHER_FILES += $$files(res/autograder/*)

    !win32 {
        LIBS += -lpthread
        copyToDestdir($$files($$PWD/res/autograder/*))
    }
    win32 {
        copyToDestdir($$PWD/res/autograder)
    }

    # copy source code into build folder so it can be analyzed by style checker
    exists($$PWD/src/*.cpp) {
        copyResources.input += $$files($$PWD/src/*.cpp)
        copyToDestdir($$files($$PWD/src/*.cpp))
    }
    exists($$PWD/*.cpp) {
        copyResources.input += $$files($$PWD/*.cpp)
        copyToDestdir($$files($$PWD/*.cpp))
    }
    exists($$PWD/src/*.h) {
        copyResources.input += $$files($$PWD/src/*.h)
        copyToDestdir($$files($$PWD/src/*.h))
    }
    exists($$PWD/*.h) {
        copyResources.input += $$files($$PWD/*.h)
        copyToDestdir($$files($$PWD/*.h))
    }
}
