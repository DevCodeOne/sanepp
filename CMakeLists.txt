cmake_minimum_required(VERSION 3.2.2)
project(sanepp C CXX)
add_compile_options(-Wall -Wextra -Wpedantic)
add_definitions(-std=c++1z)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(include)
file(GLOB SOURCES "src/*cpp" "include/*h")

find_package(Sane REQUIRED)

include_directories(${SANE_INCLUDE_DIR})

add_library(sanepp ${SOURCES})
add_executable(example_sanepp examples/example_sanepp.cpp)

target_link_libraries(sanepp ${SANE_LIBRARY})
target_link_libraries(example_sanepp sanepp)

enable_testing()
file(GLOB TEST_FILES "tests/catch.hpp" "tests/*cpp")
add_executable(sanepp_tests ${SOURCES} ${TEST_FILES})
target_link_libraries(sanepp_tests ${SANE_LIBRARY})
add_test(CatchTests sanepp_tests)
