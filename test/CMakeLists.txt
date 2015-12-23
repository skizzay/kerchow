PROJECT(find_kerchow_tests)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

ADD_EXECUTABLE(find_kerchow_tests main.cpp)
TARGET_LINK_LIBRARIES(find_kerchow_tests ${CONAN_LIBS})

# TODO: Find better way for supporting c++11
IF(APPLE)
    TARGET_COMPILE_OPTIONS(find_kerchow_tests PUBLIC -std=c++11 -stdlib=libc++ -Wall -Wextra -Werror)
ELSEIF (WIN32 OR UNIX)
    TARGET_COMPILE_OPTIONS(find_kerchow_tests PUBLIC -std=c++11 -Wall -Wextra -Werror)
ENDIF()
