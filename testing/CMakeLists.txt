option(BUILD_TESTS "Build the tests" ON)

if(BUILD_TESTS)
    # Download and set up Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    # Add subdirectories for tests
    add_subdirectory(frontend)
endif()



#add_subdirectory(frontend)

#target_link_libraries(
#  tokenizer_test
#  GTest::gtest_main
#)

#gtest_discover_tests(tokenizer_test)