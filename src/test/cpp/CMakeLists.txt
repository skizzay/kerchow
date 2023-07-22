conan_cmake_configure(REQUIRES catch2/3.3.2
   SETTINGS cppstd=20
   GENERATORS CMakeDeps CMakeToolchain)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
   BUILD missing
   REMOTE conancenter
   SETTINGS ${settings})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/src/test/cpp)

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

add_executable(kerchow_unit_tests)
target_sources(kerchow_unit_tests PRIVATE
   skizzay/kerchow/fuzzy_container.t.cpp
   skizzay/kerchow/fuzzy_number.t.cpp
   skizzay/kerchow/picker.t.cpp
)
target_link_libraries(kerchow_unit_tests PRIVATE kerchow Catch2::Catch2 Catch2::Catch2WithMain)
set_property(TARGET kerchow_unit_tests PROPERTY CXX_STANDARD 20)
catch_discover_tests(kerchow_unit_tests)

if(CMAKE_COMPILER_IS_GNUCXX)
   list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/src/test/cmake")
   include(CodeCoverage)
   APPEND_COVERAGE_COMPILER_FLAGS()
endif()