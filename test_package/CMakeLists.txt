cmake_minimum_required(VERSION 3.0)
project(lazy_cpp_test_package)
set(CMAKE_CXX_STANDARD 17)

find_package(lazy_cpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/example.cpp)
target_include_directories(${PROJECT_NAME}
        PRIVATE ${lazy_cpp_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} lazy_cpp::lazy_cpp)
