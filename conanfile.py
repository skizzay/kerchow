from conans import ConanFile, CMake
from conans.errors import ConanException


class KerchowConan(ConanFile):
    name = "kerchow"
    version = "2.0.2"
    license = "MIT"
    url = "https://github.com/skizzay/kerchow"
    description = "A library of data structures to simplify fuzz-testing"
    settings = "cppstd", "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    build_requires = "cmake_installer/3.11.2@conan/stable"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "src/*"

    def configure(self):
        if self.settings.cppstd in ["98", "gnu98"]:
            raise ConanException("kerchow is not C++98 compatible")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="src", keep_path=True)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["skizzay_kerchow"]

