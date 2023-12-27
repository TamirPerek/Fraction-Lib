cmake_minimum_required(VERSION 3.2.0)

project(
    Fraction-Lib 
    VERSION 0.1.0
    DESCRIPTION "Small library ..."
    HOMEPAGE_URL "https://github.com/TamirPerek/Fraction-Lib"
)

option(FRACTION_INCLUDE_TESTS "Enables unit tests with googletest" OFF)

add_subdirectory(src)
add_subdirectory(lib)

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FRACTION_INCLUDE_TESTS OFF)
endif()


if(FRACTION_INCLUDE_TESTS)
    add_subdirectory(test) 
    enable_testing()
endif()
