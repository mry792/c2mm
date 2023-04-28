cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(c2mm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(brokkr REQUIRED)
brokkr_project_version_from_git()

brokkr_library(
    ${PROJECT_NAME}
    LIBRARY
        DEPENDENCIES Catch2::Catch2
    UNIT_TESTS PROFILE Catch2
)

brokkr_package()
