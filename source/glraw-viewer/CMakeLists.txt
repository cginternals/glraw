
set(target glraw-viewer)
message(STATUS "App ${target}")


# External libraries

find_package(Qt5OpenGL  5.1 REQUIRED)
find_package(Qt5Core    5.1 REQUIRED)
find_package(Qt5Gui     5.1 REQUIRED)
find_package(Qt5Widgets 5.1 REQUIRED)


# Includes

include_directories(
    # e.g., ${TRDP_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glraw/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)


# Libraries

set(libs
    # e.g., ${TRDP_LIBRARIES}
    glraw
)


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-D${META_PROJECT_NAME_UPPER}_STATIC")
endif()


# Sources

set(sources
    main.cpp

    Application.cpp
    Application.h
    Canvas.cpp
    Canvas.h
    DropFix.cpp
    DropFix.h
    Viewer.cpp
    Viewer.h
)

set(uis
    Viewer.ui
)


# UI Files

qt5_wrap_ui(compiled_ui_files ${uis})


# Build executable

add_executable(${target} ${sources} ${compiled_ui_files})

target_link_libraries(${target} ${libs})
qt5_use_modules(${target} OpenGL Core Gui Widgets)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")


# Deployment

if(WIN32)
    install_qt(runtime ${INSTALL_BIN} Widgets)
endif()

install(TARGETS ${target} COMPONENT runtime
    RUNTIME DESTINATION ${INSTALL_BIN}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
