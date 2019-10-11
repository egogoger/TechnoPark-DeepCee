#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DynArray.h"
#include "DayWeather.h"

DayWeather* manual_input();
int printStats(size_t days, DynArray *dyn_array, FILE* output);

void run() {
    printf("How many days you want to enter? - ");
    size_t num = 0;
    char buffer[50];
    scanf("%s", buffer);
    num = atoi(buffer);
    while (!num) {
        puts("Invalid string!");
        scanf("%s", buffer);
        num = atoi(buffer);
    }

    DynArray *week_1 = NULL;
    new_DynArray(&week_1);


    puts("Precipitations: CLOUDY=1, FOG=2, RAIN=3, SHOWER=4, THUNDERSTORM=5, SNOW=6");
    DayWeather *new_day = NULL;
    for (size_t iii = 0; iii < num; iii++) {
        new_day = manual_input();
        Add(week_1, new_day);
    }
    free(new_day);

    puts("Statistics for how many days? (1=today)");
    size_t stat = 0;
    scanf("%s", buffer);
    stat = atoi(buffer);
    while (!stat || stat > week_1->real_size) {
        puts("Invalid string!");
        scanf("%s", buffer);
        stat = atoi(buffer);
    }

    delete_DynArray(week_1);
}

DayWeather* manual_input() {
    char buffer[50];
    DayWeather *new_day = calloc(1, sizeof(DayWeather));

    // Input temperature
    printf("\nTemperature: ");
    scanf("%s", buffer);
    float temper = 0;
    char *endptr;
    temper = strtof(buffer, &endptr);
    while ( endptr == buffer ) {
        puts("Invalid string!");
        scanf("%s", buffer);
        temper = strtof(buffer, &endptr);
    }

    // Input precipitations
    printf("Precipitations: ");
    precipitations prec = NONE;
    while ( !prec ) {
        size_t temp = 0;
        scanf("%s", buffer);
        temp = atoi(buffer);
        switch (temp) {
            case 1: prec = CLOUDY;          break;
            case 2: prec = FOG;             break;
            case 3: prec = RAIN;            break;
            case 4: prec = SHOWER;          break;
            case 5: prec = THUNDERSTORM;    break;
            case 6: prec = SNOW;            break;
            default:puts("Invalid string!");break;
        }
    }

    // Input wind speed
    printf("Wind speed: ");
    float ws = 0;
    scanf("%s", buffer);
    ws = strtof(buffer, &endptr);
    while ( endptr == buffer ) {
        puts("Invalid string!");
        scanf("%s", buffer);
        ws = strtof(buffer, &endptr);
    }

    // Commit changes :)
    new_day->temperature = temper;
    new_day->precipitation = prec;
    new_day->wind_speed = ws;
}

int printStats(size_t days, DynArray *dyn_array, FILE* output) {
    double stat = 0;
    for (size_t iii = 0; iii < days; iii++) {
        stat += dyn_array->buffer[iii].temperature;
    }
    stat /= days;
    fprintf(output, "%f", stat);
    return EXIT_SUCCESS;
}