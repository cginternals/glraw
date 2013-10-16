# Create output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)

# Packages
include(pack-glraw.cmake)
