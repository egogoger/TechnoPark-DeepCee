cmake_minimum_required(VERSION 3.8)
project(SubSequences)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -Wextra -Werror -Wpedantic")


include_directories(lib_subseq/dyn_array)
include_directories(lib_subseq/linear)
include_directories(lib_subseq/threads)
include_directories(lib_subseq/processes)


set(LIB_SUBSEQ
        lib_subseq/dyn_array/dyn_array.c
        lib_subseq/linear/linear.c
        lib_subseq/processes/processes.c
        lib_subseq/threads/threads.c)
add_library(_subseq MODULE ${LIB_SUBSEQ})


# Set GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# Add GoogleTest include directory
include_directories(${GTEST_INCLUDE_DIRS})

# Comparative tests
add_executable(main_test.out lib_subseq/tests/main_test.cpp)
target_link_libraries(main_test.out ${GTEST_LIBRARIES})
target_link_libraries(main_test.out pthread)
target_link_libraries(main_test.out ${CMAKE_DL_LIBS})
