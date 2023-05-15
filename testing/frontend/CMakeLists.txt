#add_executable(tokenizer_test tokenizer_test.cc)

#target_link_libraries(tokenizer_test frontend GTest::gtest_main)

add_executable(parser_test parser_test.cpp)

target_link_libraries(parser_test frontend GTest::gtest_main)

include(GoogleTest)
#gtest_discover_tests(tokenizer_test parser_test)
gtest_discover_tests(parser_test)