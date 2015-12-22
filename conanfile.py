from conans import ConanFile, CMake

class Kerchow(ConanFile):
    name = "kerchow"
    version = "1.0"
    url = "https://github.com/skizzay/kerchow.git"
    settings = "arch", "os", "build_type", "compiler"
    generators = "cmake"
    options = {"build_tests": [True, False], "shared": [True, False]}
    default_options = "build_tests=True", "shared=True"

    def requirements(self):
        if self.options.build_tests:
            self.requires("gtest/1.7.0@lasote/stable")

    def config(self):
        if self.options.build_tests:
            self.options["gtest"].shared = self.options.shared

    def build(self):
        cmake = CMake(self.settings)
        self.run("cmake . %s %s" % (self._extra_cmake_defines, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    @property
    def _build_tests_flag(self):
        return "-DBUILD_TESTS=TRUE" if self.options.build_tests else ""

    @property
    def _build_shared_flag(self):
        return "-DBUILD_SHARED_LIBS=ON" if self.options.shared else ""

    @property
    def _extra_cmake_defines(self):
        return ' '.join([x for x in [self._build_tests_flag, self._build_shared_flag] if x])
