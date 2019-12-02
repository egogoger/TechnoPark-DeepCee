cmake_minimum_required(VERSION 3.8)
project(HW1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -Wextra -Werror -Wpedantic")

set(LIB_SOURCE_FILES
        DayWeather.h
        DynArray.c
        DynArray.h
        stats.c
        stats.h)

add_library(forecast_lib STATIC ${LIB_SOURCE_FILES})

# Set GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# Add GoogleTest include directory
include_directories(${GTEST_INCLUDE_DIRS})

# Tests
add_executable(tests.out tests.cpp)
target_link_libraries(tests.out forecast_lib)
# Linking google tests
target_link_libraries(tests.out ${GTEST_LIBRARIES})
target_link_libraries(tests.out pthread)
