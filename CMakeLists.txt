#
# Libreset main CMake file
#
cmake_minimum_required(VERSION 2.8)

project(libreset C)


# Find doxygen
find_program(DOXYGEN doxygen)


#
# Find checkstyle.sh
#
set(CHECKSTYLE ${PROJECT_SOURCE_DIR}/etc/checkstyle.sh)


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


