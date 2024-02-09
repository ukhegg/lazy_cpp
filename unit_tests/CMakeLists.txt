cmake_minimum_required(VERSION 3.0)
project(lazy_cpp_tests)
set(CMAKE_CXX_STANDARD 17)


find_package(Catch2 REQUIRED)

SET(SOURCE_FILES
        src/lazy_t_tests.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}
        PUBLIC ${lazy_cpp_SOURCE_DIR}/include
        ${Catch2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} lazy_cpp Catch2::Catch2WithMain)