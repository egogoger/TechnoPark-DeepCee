cmake_minimum_required(VERSION 3.8)
project(subsequences)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(DynArray)

# set(LIB_SOURCE_FILES)

# add_library(forecast_lib STATIC ${LIB_SOURCE_FILES})

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# adds the gtest include directory
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(linear
        linear.c
        DynArray/DynArray.h
        DynArray/DynArray.c)

add_executable(threads
        threads.c
        DynArray/DynArray.h
        DynArray/DynArray.c)

add_executable(processes
        processes.c
        DynArray/DynArray.h
        DynArray/DynArray.c)

# DynArray
add_executable(g_test_dyn_array.out
        DynArray/GTestDynArray.cpp
        DynArray/DynArray.c
        DynArray/DynArray.h)
# Link GTest
target_link_libraries(g_test_dyn_array.out ${GTEST_LIBRARIES})
#target_link_libraries(g_test_dyn_array.out pthread)

add_executable(test
        test.c)