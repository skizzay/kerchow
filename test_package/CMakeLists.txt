cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(kerchow_tests
   test_main.cpp
)
target_link_libraries(kerchow_tests ${CONAN_LIBS})
target_compile_options(kerchow_tests PUBLIC
   -Wall -Wextra -Werror -pedantic -fstrict-aliasing)

# CTest is a testing tool that can be used to test your project.
enable_testing()
add_test(NAME kerchow_tests
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND kerchow_tests)
