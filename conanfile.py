from conans import ConanFile, CMake


class ChainConan(ConanFile):
    name = "chain"
    version = "0.1"
    homepage = "https://github.com/KaoCC/chain"
    author = "Chih-Chen Kao"
    description = "A collection of utilities."
    license = "GPL-3.0+"

    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "chain/*.hpp", "CMakeLists.txt", "tests/*.cpp", "LICENSE"
    no_copy_source = True

    requires = ["catch2/3.0.1"]
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy(pattern="LICENSE*", dst="licenses")
        self.copy("*.hpp", dst="include")

    def package_id(self):
        self.info.clear()