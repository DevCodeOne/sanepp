cmake_minimum_required(VERSION 3.6.2)
project(sanepp C CXX)
set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(include)

find_package(Sane)
include_directories(${SANE_INCLUDE_DIR})

file(GLOB SOURCES "src/*cpp" "include/*h")
add_library(sanepp ${SOURCES})
target_link_libraries(sanepp ${SANE_LIBRARY})
add_executable(example_sanepp examples/example_sanepp.cpp)
target_link_libraries(example_sanepp sanepp)

file(GLOB TEST_FILES "tests/catch.hpp" "tests/*cpp")
add_executable(tests_catch ${SOURCES} ${TEST_FILES})
target_link_libraries(tests_catch ${SANE_LIBRARY})

