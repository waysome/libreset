#
# Libreset main CMake file
#
cmake_minimum_required(VERSION 2.8)

project(libreset C)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)


# Find doxygen
find_program(DOXYGEN doxygen)


#
# Project options
#
option(HARD_MODE "Enables extra checks for use during development" OFF)
option(DEBUG_OUTPUT "Enables debug output, if the built type is \"Debug\"" OFF)


#
# Enable testing
#
enable_testing()

#
# Libreset's sources reside in ./src/.
#
add_subdirectory(src)
add_subdirectory(paper)
add_subdirectory(doc)
add_subdirectory(tests)


