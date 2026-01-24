cmake_minimum_required(VERSION 3.14)
project(lazy_sequence LANGUAGES CXX)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT for GoogleTest" FORCE)

file(GLOB HEADERS "*.hpp")
file(GLOB TEMPLATES "*.tpp")

option(BUILD_GMOCK "Build gmock" OFF)
add_subdirectory(googletest EXCLUDE_FROM_ALL)

add_executable(tests
    main_tests.cpp
    hash.hpp
)

target_compile_features(tests PRIVATE cxx_std_20)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
)

target_link_libraries(tests PRIVATE gtest gtest_main)

enable_testing()
add_test(NAME AllTests COMMAND tests)