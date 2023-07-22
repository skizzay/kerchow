conan_cmake_configure(
   SETTINGS cppstd=20
   GENERATORS CMakeDeps CMakeToolchain
)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/src/main/cpp)

add_library(kerchow INTERFACE)
target_include_directories(kerchow INTERFACE
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>
      $<INSTALL_INTERFACE:include>
   )
target_sources(kerchow INTERFACE
   skizzay/kerchow/kerchow.h
   skizzay/kerchow/arithmetic.h
   skizzay/kerchow/coalesce.h
   skizzay/kerchow/distribution.h
   skizzay/kerchow/fuzzy_container.h
   skizzay/kerchow/fuzzy_number.h
   skizzay/kerchow/picker.h
   skizzay/kerchow/uniform_distribution.h
)