
set(target glraw)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)

find_package(Qt5OpenGL  5.1 REQUIRED)
find_package(Qt5Core    5.1 REQUIRED)
find_package(Qt5Gui     5.1 REQUIRED)
#find_package(Qt5Widgets 5.1 REQUIRED)


#
# Includes
#

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-D${META_PROJECT_NAME_UPPER}_STATIC")
else()
    add_definitions("-DGLRAW_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
    add_definitions("-DGLRAW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/glraw_api.h

    ${include_path}/AbstractConverter.h
    ${include_path}/AssetInformation.h
    ${include_path}/Canvas.h
    ${include_path}/CompressionConverter.h
    ${include_path}/Converter.h
    ${include_path}/ConvertManager.h
    ${include_path}/FileWriter.h
    ${include_path}/ImageEditorInterface.h
    ${include_path}/MirrorEditor.h
    ${include_path}/RawFile.h
    ${include_path}/ScaleEditor.h
    ${include_path}/S3TCExtensions.h
)

set(sources
    ${source_path}/AbstractConverter.cpp
    ${source_path}/AssetInformation.cpp
    ${source_path}/Canvas.cpp
    ${source_path}/CompressionConverter.cpp
    ${source_path}/Converter.cpp
    ${source_path}/ConvertManager.cpp
    ${source_path}/FileWriter.cpp
    ${source_path}/MirrorEditor.cpp
    ${source_path}/RawFile.cpp
    ${source_path}/ScaleEditor.cpp
    ${source_path}/UniformParser.cpp
    ${source_path}/UniformParser.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

add_library(${target} ${api_includes} ${sources})

target_link_libraries(${target} ${libs} ${EXTRA_LIBS})
qt5_use_modules(${target} OpenGL Core Gui)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                "${GLRAW_SOURCE_DIR}/include")

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# deploy qt binaries (glraw debug target should not be deployed)
if(WIN32)
    install_qt(${INSTALL_BIN} Core Gui Widgets OpenGL)
endif()

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
