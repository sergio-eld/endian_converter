cmake_minimum_required(VERSION 3.11.4 FATAL_ERROR)

project(EndianConverter LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

get_directory_property(hasParent PARENT_DIRECTORY)

include(CMakeDependentOption)

# do not build tests if included as a subdirectory
cmake_dependent_option(BUILD_TESTS "Build Tests" ON
        "NOT hasParent" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

add_library(ELD::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# TODO: this must be passed to config file on installation! Endianness must be rediscovered within the target project
# test for endianness
include(TestBigEndian)
TEST_BIG_ENDIAN(ENDIAN)
if (ENDIAN)
    message(STATUS "System uses big endian")
    target_compile_definitions(${PROJECT_NAME} INTERFACE ENDIAN)
else()
    message(STATUS "System uses little endian")
endif()

# testing
if (BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    add_executable(test.static_test tests/static_test.cpp)
    target_link_libraries(test.static_test PRIVATE ${PROJECT_NAME})

    add_executable(test.runtime_test tests/runtime_test.cpp)
    target_link_libraries(test.runtime_test
            PRIVATE
                ELD::${PROJECT_NAME}
                GTest::GTest
            )

    add_test(NAME test.runtime_test
            COMMAND test.runtime_test)
endif()