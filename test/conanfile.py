from conans import ConanFile, CMake
from os.path import join

class KerchowPackageTest(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = "kerchow/1.0@skizzay/testing"
    generators = "cmake"
    default_options = "kerchow:build_tests=True"

    def imports(self):
        self.copy("kerchow_tests", src="bin", dst="bin")

    def build(self):
        cmake = CMake(self.settings)
        self.run("cmake . %s" % cmake.command_line)
        self.run("cmake --build . %s" % cmake.build_config)

    def test(self):
        self.run(join(".", "bin", "find_kerchow_tests"))
