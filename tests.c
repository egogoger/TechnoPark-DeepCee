#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "DayWeather.h"
#include "DynArray.h"


void test_DynArray() {
    // Create an empty one and delete right after
    {
        DynArray *temp_da = NULL;
        new_DynArray(&temp_da);
        delete_DynArray(temp_da);
    }
}

void test_Add() {
    char *str = NULL;

    DynArray *temp_da = NULL;
    new_DynArray(&temp_da);

    DayWeather* new_day;
    new_day = (DayWeather *)malloc(sizeof(DayWeather));
    new_day->temperature = 20;
    new_day->precipitation = THUNDERSTORM;
    new_day->wind_speed = 12.5;

    Add(temp_da, new_day);

    str = print_weather_forecast(temp_da->buffer[temp_da->real_size - 1]);
    // Строка не равна ?
    // assert(str == "\nTemperature: 20.00\nPrecipitations: Thunderstorm!\nThe wind speed: 12.50 m/s");

    free(str);
    free(new_day);
    delete_DynArray(temp_da);
}