cmake_minimum_required(VERSION 3.13.0)
project(kerchow VERSION 0.1.0 LANGUAGES CXX)

option(KERCHOW_BUILD_TESTS "Build tests" ON)
option(KERCHOW_BUILD_EXAMPLES "Build examples" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -fconcepts-diagnostics-depth=2)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
               "${CMAKE_BINARY_DIR}/conan.cmake"
               TLS_VERIFY ON)
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

include(FetchContent)
add_subdirectory(src/main/cpp)

if (KERCHOW_BUILD_TESTS)
    enable_testing()
    add_subdirectory(src/test/cpp)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)