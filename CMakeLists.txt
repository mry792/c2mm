cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
project(c2mm VERSION 0.0.1 LANGUAGES CXX)

find_package(brokkr REQUIRED)

add_subdirectory(src)

###
# Install Package
###

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
)
install(
    EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

brokkr_package(CONFIG_TEMPLATE cmake/config.cmake.in)
