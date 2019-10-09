#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "DynArray.h"
#include "DayWeather.h"

int main() {

    DynArray *week_1 = new_DynArray();

    for (size_t iii = 0; iii < DAYS_IN_WEEK; iii++) {
        DayWeather* new_day;
        new_day = (DayWeather *)malloc(sizeof(DayWeather));

        new_day->temperature = iii * 3;
        new_day->precipitation = SNOW;
        new_day->wind_speed = iii * 13 % 10; // Random stuff

        Add(week_1, *new_day);

        char *str;
        str = (char *)malloc(50 * sizeof(char));
        strcpy(str,print_weather_forecast(week_1->buffer[iii]));
        printf("%s", str);

        free(new_day);
    }

    delete_DynArray(week_1);

    return 0;
}