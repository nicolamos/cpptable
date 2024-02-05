cmake_minimum_required(VERSION 3.28)

project(cpptable VERSION 0.1.0 LANGUAGES CXX)

message(STATUS "Configuration of ${PROJECT_NAME} package")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(BuildType)
include(FetchContent)
message(STATUS "Building for configuration: ${CMAKE_BUILD_TYPE}")
include(CPM)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPPTABLE_WARNING_FLAGS
    -Werror -Wall -Wextra -pedantic -pedantic-errors
    CACHE STRING "Warning flags")

set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/
CACHE PATH "Installation directory for CMake files")

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

message(STATUS "Configuration of cpptable library")
add_subdirectory(cpptable)

message(STATUS "Configuration of example executable")
add_subdirectory(example)
