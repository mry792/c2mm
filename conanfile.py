#! /usr/bin/env python3

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import update_conandata
from conan.tools.scm import Git
from conans.tools import Git as LegacyGit


class C2MMRecipe (ConanFile):
    name = 'c2mm'
    # version = (computed from local repo)

    author = 'M. Emery Goss <m.goss792@gmail.com>'
    url = 'https://github.com/mry792/c2mm.git'
    description = 'Mocking library built with and for Catch2.'

    settings = 'os', 'compiler', 'build_type', 'arch',
    generators = (
        'CMakeDeps',
        'CMakeToolchain',
    )

    def package_id (self):
        self.info.clear()

    def set_version (self):
        git = LegacyGit(self.recipe_folder)
        tag = git.run('describe --tags')
        self.version = tag[1:]

    requires = 'catch2/3.1.0'
    tool_requires = 'brokkr/0.2.2@egoss/stable'

    def export (self):
        git = Git(self, self.recipe_folder)
        scm_url, scm_commit = git.get_url_and_commit()
        update_conandata(self, {
            'source': {
                'commit': scm_commit,
                'url': scm_url,
            }
        })

    def source (self):
        git = Git(self)
        source = self.conan_data['source']
        git.clone(source['url'], target = '.')
        git.checkout(source['commit'])

    def layout (self):
        cmake_layout(self)

    def build (self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package (self):
        cmake = CMake(self)
        cmake.install()
