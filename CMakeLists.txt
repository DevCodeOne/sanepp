cmake_minimum_required(VERSION 3.2.2)
project(sanepp C CXX)
set(EXECUTABLE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_compile_options(-Wall -Wextra -Wpedantic)
add_definitions(-std=c++17)

file(GLOB SOURCES "src/*cpp" "include/*h")
add_library(sanepp ${SOURCES})
add_executable(example_sanepp examples/example_sanepp.cpp)

find_package(Sane REQUIRED)

target_include_directories(sanepp PUBLIC include)
target_include_directories(sanepp PRIVATE ${SANE_INCLUDE_DIR})
target_link_libraries(sanepp PRIVATE ${SANE_LIBRARY})
target_link_libraries(example_sanepp PRIVATE sanepp)

enable_testing()
file(GLOB TEST_FILES "tests/catch.hpp" "tests/*cpp")
add_executable(sanepp_tests ${TEST_FILES})
target_link_libraries(sanepp_tests PRIVATE sanepp)
add_test(CatchTests sanepp_tests)
