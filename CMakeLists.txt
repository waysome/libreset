#
# Libreset main CMake file
#
cmake_minimum_required(VERSION 2.8)

project(libreset C)


#
# Find checkstyle.sh
#
set(CHECKSTYLE ${PROJECT_SOURCE_DIR}/etc/checkstyle.sh)


#
# Libreset's sources reside in ./src/.
#
add_subdirectory(src)
add_subdirectory(paper)


