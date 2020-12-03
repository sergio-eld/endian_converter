cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)

project(EndianConverter LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

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
add_executable(test.static_test tests/static_test.cpp)
target_link_libraries(test.static_test PRIVATE ${PROJECT_NAME})

add_executable(test.runtime_test tests/runtime_test.cpp)
target_link_libraries(test.runtime_test PRIVATE ${PROJECT_NAME})