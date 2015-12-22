PROJECT(find_kerchow_tests)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

ADD_EXECUTABLE(find_kerchow_tests main.cpp)
TARGET_LINK_LIBRARIES(find_kerchow_tests ${CONAN_LIBS})
