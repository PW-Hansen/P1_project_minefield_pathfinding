
# a (series of) tests is by itself a small program
add_executable(function_tests function_tests.c)
# we need to link with the library we are testing
# target_link_libraries(function_tests functions)

# this is where we actually add the test
# here we could also add some arguments, see https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html
add_test(NAME function_tests
        COMMAND function_tests)

