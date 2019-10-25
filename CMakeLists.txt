cmake_minimum_required(VERSION 3.8)
project(SubSequences)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -Wextra -Werror --pedantic")


include_directories(project/DynArray)
include_directories(project/linear)
include_directories(project/threads)
include_directories(project/processes)


set(LIB_DYNARRAY
        project/DynArray/DynArray.c)
add_library(_dynarray STATIC ${LIB_DYNARRAY})

set(LIB_LINEAR
        project/linear/linear.c
        project/linear/test_linear.cpp
        project/DynArray/DynArray.c)
add_library(_linear STATIC ${LIB_LINEAR})

set(LIB_THREADS
        project/threads/threads.c
        project/threads/test_threads.cpp
        project/DynArray/DynArray.c)
add_library(_threads STATIC ${LIB_THREADS})

set(LIB_PROCESSES
        project/processes/processes.c
        project/processes/test_processes.cpp
        project/DynArray/DynArray.c)
add_library(_processes STATIC ${LIB_PROCESSES})


# Set GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# Add GoogleTest include directory
include_directories(${GTEST_INCLUDE_DIRS})


# DynArray Lib
add_executable(test_DynArray.out project/DynArray/test_DynArray.cpp)
target_link_libraries(test_DynArray.out _dynarray)
# Link GoogleTest
target_link_libraries(test_DynArray.out ${GTEST_LIBRARIES})
target_link_libraries(test_DynArray.out pthread)

# Linear Lib
add_executable(test_linear.out project/linear/test_linear.cpp)
target_link_libraries(test_linear.out _linear)
# Link GoogleTest
target_link_libraries(test_linear.out ${GTEST_LIBRARIES})
target_link_libraries(test_linear.out pthread)

# Threads Lib
add_executable(test_threads.out project/threads/test_threads.cpp)
target_link_libraries(test_threads.out _threads)
# Link GoogleTest
target_link_libraries(test_threads.out ${GTEST_LIBRARIES})
target_link_libraries(test_threads.out pthread)

# Processes Lib
add_executable(test_processes.out project/processes/test_processes.cpp)
target_link_libraries(test_processes.out _processes)
# Link GoogleTest
target_link_libraries(test_processes.out ${GTEST_LIBRARIES})
target_link_libraries(test_processes.out pthread)
