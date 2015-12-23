from conans import ConanFile, CMake
from os.path import dirname, abspath, join

def _this_directory():
    return dirname(abspath(__file__))

class Kerchow(ConanFile):
    name = "kerchow"
    version = "1.0.1"
    url = "https://github.com/skizzay/kerchow.git"
    settings = "arch", "os", "build_type", "compiler"
    generators = "cmake"
    options = {"build_tests": [True, False], "shared": [True, False]}
    default_options = "build_tests=False", "shared=True"
    exports = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    def requirements(self):
        if self.options.build_tests:
            self.requires("gtest/1.7.0@lasote/stable")

    def config(self):
        if self.options.build_tests:
            self.options["gtest"].shared = self.options.shared

    def build(self):
        cmake = CMake(self.settings)
        self.run("cmake %s %s %s" % (_this_directory(), self._extra_cmake_defines, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)
        if self.options.build_tests:
            self.run("ctest")

    def package(self):
        # Copying headers
        self.copy(pattern="*.h", dst=join("include", "kerchow"), src=join("include", "kerchow"))

        # Copying static and dynamic libs
        self.copy(pattern="*.a", dst="lib", src=".", keep_path=False)
        self.copy(pattern="*.lib", dst="lib", src=".", keep_path=False)
        self.copy(pattern="*.dll", dst="bin", src=".", keep_path=False)
        self.copy(pattern="*.so*", dst="lib", src=".", keep_path=False)
        self.copy(pattern="*.dylib*", dst="lib", src=".", keep_path=False)      

        # Copying tests
        if self.options.build_tests:
            self.copy(pattern="kerchow_tests", dst="bin", src="bin", keep_path=False)      

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libs = ["kerchow"]

    @property
    def _build_tests_flag(self):
        return "-DBUILD_TESTS=TRUE" if self.options.build_tests else ""

    @property
    def _build_shared_flag(self):
        return "-DBUILD_SHARED_LIBS=ON" if self.options.shared else ""

    @property
    def _extra_cmake_defines(self):
        return ' '.join([x for x in [self._build_tests_flag, self._build_shared_flag] if x])
