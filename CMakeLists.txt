CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)

IF(BIICODE)
    MESSAGE(Building with biicode)

    INCLUDE(biicode/cmake/tools)
    ADD_BII_TARGETS()

    SET(KERCHOW_TARGET ${BII_BLOCK_TARGET})

    ACTIVATE_CPP11(INTERFACE ${BII_BLOCK_TARGET})
ELSEIF (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    MESSAGE(Building with conan)

    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()

    INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

    ADD_LIBRARY(kerchow src/kerchow/picker.cpp)

    SET(KERCHOW_TARGET kerchow)

    IF (BUILD_TESTS)
        ENABLE_TESTING()
        ADD_SUBDIRECTORY(tests)
        ADD_TEST(kerchow_tests bin/kerchow_tests)
    ENDIF()
ELSE()
    MESSAGE(FATAL_ERROR "Kerchow must be built with either biicode or conan.  The preferred method is conan.")
ENDIF()

IF(APPLE)
    TARGET_COMPILE_OPTIONS(${KERCHOW_TARGET} PUBLIC "-std=c++11 -stdlib=libc++")
ELSEIF (WIN32 OR UNIX)
    TARGET_COMPILE_OPTIONS(${KERCHOW_TARGET} PUBLIC "-std=c++11")
ENDIF()

# Try to use something that generates fairly decent entropy
IF (EXISTS /dev/urandom)
    TARGET_COMPILE_DEFINITIONS(${KERCHOW_TARGET} PRIVATE KERCHOW_USE_DEV_URANDOM)
ELSEIF (EXISTS /dev/random)
    TARGET_COMPILE_DEFINITIONS(${KERCHOW_TARGET} PRIVATE KERCHOW_USE_DEV_RANDOM)
ENDIF()
