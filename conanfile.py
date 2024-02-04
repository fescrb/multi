from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeDeps, CMakeToolchain

class Multi(ConanFile):
    name = 'multi'
    version='0.0.1'

    settings = "os", "compiler", "build_type", "arch"

    requires = 'gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3', 'benchmark/1.8.3#2b95dcd66432d8ea28c5ac4db0be2fb2'

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build_requirements(self):
        self.tool_requires('cmake/3.27.5')

    def layout(self):
        cmake_layout(self)

    def build(self) -> None:
        cmake = CMake(self)
        cmake.configure(variables={
            'CMAKE_EXPORT_COMPILE_COMMANDS': 'ON',
        })
        cmake.build()
