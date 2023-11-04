################################################################################
### Selecting CMake minimum version

CMAKE_MINIMUM_REQUIRED (VERSION 3.10)

################################################################################
### Setting Global Parameters

SET(PROJECT_NAME "packetio_all")
PROJECT("${PROJECT_NAME}")


add_subdirectory(lib)
add_subdirectory(example)
