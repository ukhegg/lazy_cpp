from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
import os


class LazyCppRecipe(ConanFile):
    name = 'lazy_cpp'
    version = '0.1'
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

    def test(self):
        pass
