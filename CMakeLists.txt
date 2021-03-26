cmake_minimum_required(VERSION 3.11.4 FATAL_ERROR)

project(EndianConverter LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_TESTS "Run tests" ON)

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

    # TODO: fetch gtest
#    if (NOT TARGET GTest::GTest)
#        message(STATUS "Couldn't find googletest. Fetching from git...")
#
#        set(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)
#        include(ExternalProject)
#
#        externalproject_add(fetch_gtest
#                GIT_REPOSITORY
#                    https://github.com/google/googletest.git
#                GIT_TAG
#                    release-1.8.0
#                CMAKE_ARGS
#                    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
#                    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#                    -DCMAKE_CXX_COMPILER_WORKS=1
#                CMAKE_CACHE_ARGS
#                    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
#                BUILD_COMMAND
#                    ""
#                INSTALL_COMMAND
#                    ${CMAKE_COMMAND} --build . --target install --config Release
#                BUILD_ALWAYS
#                    True
#                )
#
#        ExternalProject_Get_Property(fetch_gtest INSTALL_DIR)
#        list(APPEND CMAKE_MODULE_PATH ${INSTALL_DIR})
#
#        add_custom_target(Rescan ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS fetch_gtest)
#
#        find_package(GTest REQUIRED)
#    else()
#        add_custom_target(Rescan)
#    endif()

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