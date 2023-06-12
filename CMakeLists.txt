cmake_minimum_required(VERSION 3.14)
project(keikaku LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)


add_subdirectory(lib)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(testing)
endif()

add_executable(
  main ${CMAKE_SOURCE_DIR}/main.cpp
)

add_executable(
  kei ${CMAKE_SOURCE_DIR}/kei.cpp
)

target_link_libraries(main frontend codegen util)
target_link_libraries(kei frontend codegen util)