add_executable(visitor_test ir_visitor_test.cpp)

target_link_libraries(visitor_test frontend codegen GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(visitor_test)