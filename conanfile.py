#! /usr/bin/env python3

from pathlib import Path

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import update_conandata
from conan.tools.scm import Git


class C2MMRecipe (ConanFile):
    name = 'c2mm'
    # version = (computed from local repo)

    author = 'M. Emery Goss <m.goss792@gmail.com>'
    url = 'https://github.com/mry792/c2mm.git'
    description = 'Mocking library built with and for Catch2.'

    settings = 'os', 'arch', 'compiler', 'build_type',

    @property
    def git (self):
        return Git(self, self.recipe_folder)

    @property
    def _has_git_repo (self):
        try:
            recipe_folder = Path(self.recipe_folder).resolve()
            repo_root = Path(self.git.get_repo_root()).resolve()
            return recipe_folder = repo_root
        except Exception:
            return False

    def set_version (self):
        if not self.version:
            tag = self.git.run('describe --tags --match v*.*.*')
            self.version = tag[1:]

    ###
    # RECIPE STEPS
    ###

    def export (self):
        if self._has_git_repo:
            scm_url, scm_commit = self.git.get_url_and_commit()
            update_conandata(self, {
                'source': {
                    'commit': scm_commit,
                    'url': scm_url,
                }
            })
            return

        update_conandata(self, {
            'source': { 'url': '(local)' }
        })

    def export_sources (self):
        if not self._has_git_repo:
            self.copy('*.hpp')
            self.copy('*.cpp')
            self.copy('CMakeLists.txt')
            self.copy('LICENSE')
            self.copy('README.md')

    def layout (self):
        cmake_layout(self)

    requires = 'catch2/3.4.0'
    tool_requires = 'brokkr/0.2.3-3-gb04df13@egoss/testing'

    def package_id (self):
        self.info.clear()

    def validate (self):
        if self.settings.get_safe('compiler.cppstd'):
            check_min_cppstd(self, 20)

    def source (self):
        source = self.conan_data['source']
        if source['url'] != '(local)':
            git = self.git
            git.clone(source['url'], target = '.')
            git.checkout(source['commit'])

    def generate (self):
        CMakeDeps(self).generate()

        tc = CMakeToolchain(self)
        if self.conan_data['source']['url'] == '(local)':
            tc.cache_variables['BROKKR_THIS_PROJECT_VERSION:STRING'] = self.version
        tc.generate()

    def build (self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package (self):
        cmake = CMake(self)
        cmake.install()
