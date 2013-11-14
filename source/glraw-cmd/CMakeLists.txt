
# Target
set(target glraw-cmd)
message(STATUS "Configuring ${target}")

# Generate meta header
configure_file(glraw-cmd-version.h.in glraw-cmd-version.h)

set(sources
    Application.cpp
    Application.h
    ArgumentsParser.cpp
    ArgumentsParser.h
    main.cpp
)

# Group source files
set(GLRAW_SOURCE_GROUP "Source Files")
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.h$|\\\\.hpp$|\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${GLRAW_SOURCE_GROUP} ${sources})

# wrap ui files and group wrap headers
list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

source_group_by_path(${CMAKE_CURRENT_BINARY_DIR} "\\\\.h$" 
    ${GLRAW_SOURCE_GROUP} ${uihs})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})


# Build library
add_executable(${target} ${sources} ${uihs})

qt5_use_modules(${target} OpenGL Gui Widgets)

target_link_libraries(${target}
	glraw)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
#    LIBRARY DESTINATION ${INSTALL_SHARED}
#    ARCHIVE DESTINATION ${INSTALL_LIB}
)
