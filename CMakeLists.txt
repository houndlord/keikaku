cmake_minimum_required(VERSION 3.14)
project(keikaku LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(lib)
#include_directories(${CMAKE_SOURCE_DIR}/lib)

option(BUILD_TESTS "Build tests" OFF)

if (BUIILD_TESTS) 
    enable_testing()
    add_subdirectory(testing)
endif()

add_executable(
  tokenizer ${CMAKE_SOURCE_DIR}/main.cpp
)
