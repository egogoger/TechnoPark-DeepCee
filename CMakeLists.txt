cmake_minimum_required(VERSION 3.15)
project(IS1 C)

set(CMAKE_C_STANDARD 99)

add_executable(IS1
        main.c
        DynArray.c
        DynArray.h
        DayWeather.c
        DayWeather.h tests.c)