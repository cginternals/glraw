
# GLRAW_DIR
# GLRAW_FOUND
# GLRAW_LIBRARIES
# GLRAW_INCLUDE_DIRS

# GLRAW_LIBRARY
# GLRAW_LIBRARY_RELEASE
# GLRAW_LIBRARY_DEBUG
# GLRAW_INCLUDE_DIR

# GLRAW_BINARIES (win32 only)
# GLRAW_BINARY_RELEASE (win32 only)
# GLRAW_BINARY_DEBUG (win32 only)

macro (find LIB_NAME HEADER)

    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "glraw")
        set(LIB_NAME_UPPER GLRAW)
        set(LIBNAME glraw)
    else()
        string(TOUPPER GLRAW_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME ${LIB_NAME})
    endif()

    find_path(
	${LIB_NAME_UPPER}_INCLUDE_DIR
	${HEADER}
        ${ENV_GLRAW_DIR}/include
        ${ENV_GLRAW_DIR}/source/${LIB_NAME}/include
        ${GLRAW_DIR}/include
        ${GLRAW_DIR}/source/${LIB_NAME}/include
        ${ENV_PROGRAMFILES}/glraw/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${HEADER} resides"
    )


    find_library(
	${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library"
    )
    find_library(
	${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library"
    )
    

    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()


    set(GLRAW_INCLUDE_DIRS  ${GLRAW_INCLUDE_DIRS}  ${${LIB_NAME_UPPER}_INCLUDE_DIR} PARENT_SCOPE)
    set(GLRAW_LIBRARIES ${GLRAW_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY} PARENT_SCOPE)

    # DEBUG MESSAGES
#    message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
#    message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
#    message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
#    message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()


if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLRAW_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENV_PROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLRAW_DIR}" ENV_GLRAW_DIR)

set(LIB_PATHS   
    ${GLRAW_DIR}/build
    ${GLRAW_DIR}/build/Release
    ${GLRAW_DIR}/build/Debug
    ${GLRAW_DIR}/build-release
    ${GLRAW_DIR}/build-debug
    ${GLRAW_DIR}/lib
    ${ENV_GLRAW_DIR}/lib
    ${ENV_PROGRAMFILES}/glraw/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)


find(glraw glraw/glraw_api.h ${LIB_PATHS})

if (GLRAW_LIBRARY AND WIN32)
    set(GLRAW_BINARIES "")

    find_file(GLRAW_BINARY_RELEASE
        NAMES glraw.dll
        PATHS
        ${GLRAW_DIR}/bin
        ${GLRAW_DIR}/build/Release
        ${GLRAW_DIR}/build-release
        DOC "The glraw binary")

    find_file(GLRAW_BINARY_DEBUG
        NAMES glrawd.dll
        PATHS
        ${GLRAW_DIR}/bin
        ${GLRAW_DIR}/build/Debug
        ${GLRAW_DIR}/build-debug
        DOC "The glraw debug binary")

    if(NOT GLRAW_BINARY_RELEASE STREQUAL "GLRAW_BINARY_RELEASE-NOTFOUND")
        list(APPEND GLRAW_BINARIES ${GLRAW_BINARY_RELEASE})
    endif()

    if(NOT GLRAW_BINARY_DEBUG STREQUAL "GLRAW_BINARY_DEBUG-NOTFOUND")
        list(APPEND GLRAW_BINARIES ${GLRAW_BINARY_DEBUG})
    endif()

    # DEBUG
    # message("${LIB_NAME_UPPER}_BINARIES         = ${${LIB_NAME_UPPER}_BINARIES}")

endif()

# DEBUG
#message("GLRAW_INCLUDE_DIRS  = ${GLRAW_INCLUDE_DIRS}")
#message("GLRAW_LIBRARIES = ${GLRAW_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLRAW DEFAULT_MSG GLRAW_LIBRARIES GLRAW_INCLUDE_DIRS)
mark_as_advanced(GLRAW_FOUND)
