from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
from conan.tools.build import can_run
from conan.tools.cmake import cmake_layout, CMake
import os
import pathlib


class LazyCppRecipe(ConanFile):
    name = 'lazy_cpp'
    version = '0.8'
    user = 'ukhegg'
    channel = 'stable'
    url = 'https://github.com/ukhegg/lazy_cpp.git'
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = ['CMakeToolchain', 'CMakeDeps']

    def requirements(self):
        pass

    def build_requirements(self):
        self.requires('catch2/3.3.1')

    def source(self):
        git = Git(self)
        git.clone(self.url, target='.')
        git.checkout(self.version)

    def package(self):
        copy(self, pattern="*.hpp",
             src=os.path.join(self.source_folder, 'lazy_cpp/include/'),
             dst=os.path.join(self.package_folder, 'include'))

    def package_info(self):
        self.cpp_info.include_dirs = 'include'

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()
            self.run(os.path.join('unit_tests', 'lazy_cpp_tests'))
