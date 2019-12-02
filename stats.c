#include <stdio.h>

#include "DayWeather.h"
#include "DynArray.h"
#include "stats.h"

float min_temperature(const DynArray *dyn_array, int duration) {
    if (duration > dyn_array->real_size) {
        printf("Too many days\n");
        return 0;
    }
    float result = dyn_array->buffer[0].temperature;
    for (int iii = 1; iii < duration; ++iii) {
        float tmp = dyn_array->buffer[iii].temperature;
        result = (result > tmp ? tmp : result);
    }
    return result;
}

float average_temperature(const DynArray *dyn_array, int duration) {
    if (duration > dyn_array->real_size) {
        printf("Too many days\n");
        return 0;
    }
    float result = 0;
    for (int iii = 0; iii < duration; ++iii) {
        result += dyn_array->buffer[iii].temperature;
    }
    return result / duration;
}

float max_temperature(const DynArray *dyn_array, int duration) {
    if (duration > dyn_array->real_size) {
        printf("Too many days\n");
        return 0;
    }
    float result = dyn_array->buffer[0].temperature;
    for (int iii = 1; iii < duration; ++iii) {
        float tmp = dyn_array->buffer[iii].temperature;
        result = (result < tmp ? tmp : result);
    }
    return result;
}

int amount_of_precipitations(const DynArray *dyn_array, int duration) {
    if (duration > dyn_array->real_size) {
        printf("Too many days\n");
        return 0;
    }
    int result = 0;
    for (int iii = 0; iii < duration; ++iii) {
        switch (dyn_array->buffer[iii].precipitation) {
            case NONE:
                break;
            default:
                ++result;
                break;
        }
    }
    return result;
}