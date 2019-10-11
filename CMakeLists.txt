cmake_minimum_required(VERSION 3.8)
project(IS1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES
        DynArray.c
        DayWeather.c)

add_library(forecast_lib STATIC ${LIB_SOURCE_FILES})

#add_executable(IS1
#        main.c
#        DynArray.c
#        DynArray.h
#        DayWeather.c
#        DayWeather.h)

add_executable(tests.out
        tests.c
        DynArray.c
        DynArray.h
        DayWeather.c
        DayWeather.h)

target_link_libraries(tests.out forecast_lib)

# My own tests
enable_testing()

add_test(NAME MainTest
        COMMAND tests.out)

set_tests_properties(MainTest PROPERTIES
        PASS_REGULAR_EXPRESSION "OK")
