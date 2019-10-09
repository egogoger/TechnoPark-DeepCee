#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "DayWeather.h"
/*
DayWeather* new_DayWeather() {
    DayWeather* new_day;
    new_day = (DayWeather *)malloc(sizeof(DayWeather));

    new_day->temperature = 0;
    new_day->precipitation = TOTAL_AMOUNT;
    new_day->wind_speed = 0;

    return new_day;
}
*/

char* print_weather_forecast(DayWeather today) {
    char *temp;
    temp = (char *)malloc(sizeof(char) * MAX_LEN);
    size_t n = 0;
    n = snprintf(temp, MAX_LEN, "\nTemperature: %.2f\nPrecipitations: ", today.temperature);
    assert(n > 0);
    // printf("\nTemperature: %.2f\nPrecipitations: ", today.temperature);
    switch(today.precipitation)
    {
        case NONE:          strcat(temp, "Sunny");            break;
        case CLOUDY:        strcat(temp, "Cloudy");           break;
        case FOG:           strcat(temp, "Foggy");            break;
        case RAIN:          strcat(temp, "Rainy");            break;
        case SHOWER:        strcat(temp, "Heavy rain");       break;
        case THUNDERSTORM:  strcat(temp, "Thunderstorm!");    break;
        case SNOW:          strcat(temp, "Snowy");            break;
        default:            strcat(temp, "Unknown weather");  break;
    }
    n = snprintf(temp + strlen(temp), MAX_LEN - strlen(temp), "\nThe wind speed: %.2f m/s", today.wind_speed);
    assert(n > 0);
    return temp;
}