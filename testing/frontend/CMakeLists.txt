add_executable(tokenizer_test tokenizer_test.cc)

target_link_libraries(tokenizer_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)