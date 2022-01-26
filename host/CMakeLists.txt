cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build configuration")


project(JeffHost LANGUAGES CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(../cmake/dependencies.cmake)

file(REAL_PATH ${CMAKE_SOURCE_DIR}/../ PROJECT_ROOT_DIR)

add_subdirectory(../src/common src/common)

enable_testing()
add_subdirectory(../tests/host tests/host)
