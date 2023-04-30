cmake_minimum_required(VERSION 3.14)
project(keikaku LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(lib)
#include_directories(${CMAKE_SOURCE_DIR}/lib)

#option(BUILD_TESTS "Build tests" ON)

#if (BUIILD_TESTS) 
#enable_testing()
#  include(GoogleTest)
#  include(FetchContent)

#  FetchContent_Declare(
#    googletest
#    GIT_REPOSITORY https://github.com/google/googletest.git
#    GIT_TAG        master
#  )
#
#  FetchContent_MakeAvailable(googletest)
#
#  add_subdirectory(testing)
#endif()

add_executable(
  main ${CMAKE_SOURCE_DIR}/main.cpp
)
