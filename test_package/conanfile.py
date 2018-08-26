from conans import ConanFile, CMake, tools

class KerchowTestConan(ConanFile):
    build_requires = "gtest/1.8.0@bincrafters/stable", "cmake_installer/3.11.2@conan/stable"
    settings = "cppstd", "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._cmake = None

    def build(self):
        self._cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        self._cmake.configure()
        self._cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        if not tools.cross_building(self.settings):
            self._cmake.test()
