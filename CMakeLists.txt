cmake_minimum_required(VERSION 3.0)
project(lazy_cpp_proj)
set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

add_subdirectory(lazy_cpp)
add_subdirectory(unit_tests)
