# ---------------------------------------------------------------------------#
# ------------------------------- CMake -------------------------------------#
# ---------------------------------------------------------------------------#

# minimum supported cmake version
cmake_minimum_required(VERSION 2.6)

# identify the project and programming language
project(traceroute C)

# compiler flags passed as arguments 
add_definitions(-Wall -Werror -std=c99)

# set output directories for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# specify the TARGET executable
file(GLOB traceroute_SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(traceroute ${traceroute_SOURCES})

# ---------------------------------------------------------------------------#
