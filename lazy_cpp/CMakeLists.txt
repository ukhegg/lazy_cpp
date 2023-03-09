cmake_minimum_required(VERSION 3.10)
project(lazy_cpp)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        include/lazy_cpp/lazy_t.hpp
        include/lazy_cpp/lazy_impl_t.hpp)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include)