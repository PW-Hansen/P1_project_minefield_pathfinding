add_executable(test_function test_function.c)
target_link_libraries(test_function mtest)

discover_tests(test_function)
