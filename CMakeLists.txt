cmake_minimum_required(VERSION 3.2.2)
project(sanepp C CXX)
set(EXECUTABLE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_definitions(-std=c++17)
add_compile_options(-Wall -Wextra -Wpedantic)

option(SANEPP_BUILD_EXAMPLES "Build examples for sanepp" ON)
option(SANEPP_BUILD_TESTS "Build tests for sanepp" ON)

file(GLOB SOURCES "src/*cpp" "include/*h")
add_library(sanepp ${SOURCES})

find_package(Sane REQUIRED)

target_include_directories(sanepp PUBLIC include)
target_include_directories(sanepp PRIVATE ${SANE_INCLUDE_DIR})
target_link_libraries(sanepp PRIVATE ${SANE_LIBRARY})

IF (SANEPP_BUILD_EXAMPLES)
    add_executable(example_sanepp examples/example_sanepp.cpp)
    add_executable(example_sanepp_options examples/example_sanepp_options.cpp)
    target_link_libraries(example_sanepp PRIVATE sanepp)
    target_link_libraries(example_sanepp_options PRIVATE sanepp)
ENDIF()

IF (SANEPP_BUILD_TESTS)
    enable_testing()
    file(GLOB TEST_FILES "tests/catch.hpp" "tests/*cpp")
    add_executable(sanepp_tests ${TEST_FILES})
    target_link_libraries(sanepp_tests PRIVATE sanepp)
    add_test(CatchTests sanepp_tests)
ENDIF()
