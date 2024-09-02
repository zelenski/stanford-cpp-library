cmake_minimum_required(VERSION 3.17)
project(SimpleTestGuide)

# Need the relative path to Library
set(CS106_LIB_PATH "../Library")

# Create the project executable
add_executable(${PROJECT_NAME}
        main.cpp
        simpletestguide.cpp
        unittest.cpp
)

# link exe to CS106 library
target_link_libraries(${PROJECT_NAME} CS106_library)

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
# Ask Julie if you are curious why main->qMain->studentMain
target_compile_definitions(${PROJECT_NAME} PRIVATE main=qMain)
target_compile_definitions(${PROJECT_NAME} PRIVATE qMain=studentMain)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

if (WIN32)

    # If the debug library versions do not exist, 
    # consider using the dll library version instead   
    set(EXTENSION "dll")
    #    set(EXTENSION "debug")
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB ${REQUIRED_LIBS})
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}.${EXTENSION}")
            message("${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}.${EXTENSION} does not exist")
        endif ()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}.${EXTENSION}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()