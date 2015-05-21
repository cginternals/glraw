
# Generate meta-header
configure_file(glraw-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/glraw-version.h)

# We do NOT want to deploy this header (at least not for binary packages)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/glraw/glraw-version.h DESTINATION ${INSTALL_INCLUDE}/glraw)

find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}

    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
)

set(GLRAW_DEPENDENCY_LIBRARIES
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
)

# Qt5

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)


# Libraries
set(IDE_FOLDER "")
add_subdirectory(glraw)

# Applications
add_subdirectory(glraw-cmd)
