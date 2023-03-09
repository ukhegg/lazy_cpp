cmake_minimum_required(VERSION 3.0)
project(lazy_cpp_proj)

include(${PROJECT_BINARY_DIR}/conan_toolchain.cmake)

add_subdirectory(lazy_cpp)
add_subdirectory(tests)
