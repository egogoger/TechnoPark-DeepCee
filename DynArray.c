#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DynArray.h"
#include "DayWeather.h"

DynArray* new_DynArray() {
    DynArray* dyn_array = calloc(1, sizeof(DynArray));
    if ( !dyn_array )
        return NULL;

    dyn_array->buffer_size = DAYS_IN_WEEK;

    dyn_array->buffer = (DayWeather*)calloc( dyn_array->buffer_size, sizeof(DayWeather) );
    if ( !dyn_array->buffer ) {
        delete_DynArray( dyn_array );
        return NULL;
    }
    dyn_array->real_size = 0;

    return dyn_array;
}

int delete_DynArray(DynArray *dyn_array) {  // malloc_fail
    if ( !dyn_array )
        return EXIT_FAILURE;
    free(dyn_array->buffer);
    free(dyn_array);
    return EXIT_SUCCESS;
}

size_t is_empty(const DynArray *dyn_array) {
    return (dyn_array->real_size != 0) ? 1 : 0;
}

void expand(DynArray *dyn_array) {
    size_t bigger_buffer_size = dyn_array->buffer_size * 2;

    DayWeather *bigger_buffer;
    bigger_buffer = (DayWeather*)malloc(bigger_buffer_size * sizeof(DayWeather));
    if ( !bigger_buffer )
        delete_DynArray( dyn_array );

    memcpy(bigger_buffer, dyn_array->buffer, dyn_array->buffer_size * sizeof(DayWeather));
    // for (size_t iii = 0; iii < dyn_array->buffer_size; iii++)
    //     bigger_buffer[iii] = dyn_array->buffer[iii];

    free(dyn_array->buffer);

    dyn_array->buffer = bigger_buffer;
    dyn_array->buffer_size = bigger_buffer_size;
}

void add(DynArray *dyn_array, const DayWeather* day) {
    if (dyn_array->real_size == dyn_array->buffer_size)
        expand(dyn_array);
    if ( !(dyn_array->real_size < dyn_array->buffer_size && dyn_array->buffer != 0) ) {
        printf("Dynamic array failed\n");
        return;
    }
    dyn_array->buffer[dyn_array->real_size].temperature = day->temperature;
    dyn_array->buffer[dyn_array->real_size].precipitation = day->precipitation;
    dyn_array->buffer[dyn_array->real_size++].wind_speed = day->wind_speed;
}

// works provided user checked emptiness
DayWeather pop(DynArray *dyn_array) {
    return dyn_array->buffer[--dyn_array->real_size];
}