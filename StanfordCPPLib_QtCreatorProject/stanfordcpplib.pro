# Qt Creator project file (standard version)
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
# @author Marty Stepp, Reid Watson, Rasmus Rygaard, Jess Fisher, etc.
# @version 2014/10/29

TEMPLATE = app

# Make sure we do not accidentally #include files placed in 'resources'
CONFIG += no_include_pwd

SOURCES += $$PWD/../StanfordCPPLib/*.cpp
exists($$PWD/src/*.cpp) {
    SOURCES += $$PWD/src/*.cpp
}
exists($$PWD/*.cpp) {
    SOURCES += $$PWD/*.cpp
}

HEADERS += $$PWD/../StanfordCPPLib/*.h
HEADERS += $$PWD/../StanfordCPPLib/private/*.h
exists($$PWD/src/*.h) {
    HEADERS += $$PWD/src/*.h
}
exists($$PWD/*.h) {
    HEADERS += $$PWD/*.h
}

# set up flags for the C++ compiler
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Wreturn-type
QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS += -Wunreachable-code
QMAKE_CXXFLAGS += -Wno-dangling-field
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-unused-const-variable
QMAKE_CXXFLAGS += -Wno-write-strings

# set up flags used by the Stanford C++ libraries
DEFINES += SPL_CONSOLE_X=999999
DEFINES += SPL_CONSOLE_Y=999999
DEFINES += SPL_CONSOLE_WIDTH=750
DEFINES += SPL_CONSOLE_HEIGHT=500
DEFINES += SPL_CONSOLE_FONTSIZE=14
DEFINES += SPL_CONSOLE_ECHO
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE
DEFINES += SPL_VERIFY_JAVA_BACKEND_VERSION

INCLUDEPATH += $$PWD/../StanfordCPPLib/
INCLUDEPATH += $$PWD/../StanfordCPPLib/private/
INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/

# build-specific options (debug vs release)
CONFIG(release, debug|release) {
    # make 'release' target be statically linked so it is a stand-alone executable
    # (this code comes from Rasmus Rygaard)
    QMAKE_LFLAGS += -O2
    !macx {
    }
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
    QMAKE_LFLAGS += -O0
    QMAKE_LFLAGS += -g
    QMAKE_LFLAGS += -DSPL_CONSOLE_PRINT_EXCEPTIONS 
}

# a check to ensure that the Java back-end is present in this project
exists($$PWD/lib/spl.jar) {
    # good; do nothing
} else {
    message(*** Stanford Java back-end library 'spl.jar' not found!)
    message(*** This project cannot run without spl.jar present.)
    message(*** Place that file into your lib/ folder and try again.)
    error(Exiting.)
}

# Copies the given files to the destination directory
# The rest of this file defines how to copy the resources folder
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
    exists($$PWD/*.txt) {
        copyToDestdir($$PWD/*.txt)
    }
}

copyResources.input += $$files($$PWD/lib/*.jar)
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

macx {
    cache()
    QMAKE_MAC_SDK = macosx
}
