################################################################################
### Selecting CMake minimum version

CMAKE_MINIMUM_REQUIRED (VERSION 3.10)

################################################################################
### Setting Global Parameters

SET(PROJECT_NAME "packetio_app")
PROJECT("${PROJECT_NAME}")

INCLUDE(GNUInstallDirs)
add_executable(main main.c )

find_package(libconfig CONFIG REQUIRED)

set(CMAKE_PREFIX_PATH "/usr/local/lib/" ${CMAKE_PREFIX_PATH})

LINK_DIRECTORIES(
	"/usr/local/lib/"
)


find_package(Threads REQUIRED)

#find_package(dcisink)
target_link_libraries(main packetio config pthread)
