include(GNUInstallDirs)

find_package(Catch2 3 REQUIRED)

###
# Library Definition
###

add_library(${PROJECT_NAME} INTERFACE)
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS ".hpp")
target_sources(${PROJECT_NAME} INTERFACE ${source_files})
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
        Catch2::Catch2
)
target_compile_features(
    ${PROJECT_NAME}
    INTERFACE
        cxx_std_20
)

###
# Unit Tests
###

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        file(GLOB_RECURSE test_files CONFIGURE_DEPENDS "*.test.cpp")
        add_executable(unit_test ${test_files})
        target_compile_options(unit_test
            PRIVATE
                -Werror
                -Wall
                -Wextra
                -Wpedantic
        )
        target_link_libraries(unit_test
            # 3rd-party
            Catch2::Catch2WithMain

            # local
            ${PROJECT_NAME}
        )

        include(Catch)
        catch_discover_tests(unit_test)
    endif()
endif()

###
# Install Library
###

# For when consumed as a subdirectory.
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)
