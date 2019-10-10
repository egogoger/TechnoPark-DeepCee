#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "DynArray.h"
#include "DayWeather.h"

DynArray* new_DynArray()
{
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
        return -1;
    free(dyn_array->buffer);
    free(dyn_array);
    return 0;
}

size_t IsEmpty(DynArray *dyn_array) {
    return (dyn_array->real_size != 0) ? 1 : 0;
}

// consider using realloc
void Expand(DynArray *dyn_array) {
    size_t bigger_buffer_size = dyn_array->buffer_size * 2;
    DayWeather *bigger_buffer;
    bigger_buffer = (DayWeather*)malloc(bigger_buffer_size * sizeof(DayWeather));
    if (!bigger_buffer)
        delete_DynArray(dyn_array);

    // memcpy(bigger_buffer, dyn_array->buffer, bigger_buffer_size * sizeof(DayWeather));
    for (size_t iii = 0; iii < dyn_array->buffer_size; iii++)
        bigger_buffer[iii] = dyn_array->buffer[iii];

    free(dyn_array->buffer);
    dyn_array->buffer = bigger_buffer;
    dyn_array->buffer_size = bigger_buffer_size;
}

void Add(DynArray *dyn_array, DayWeather* day) {
    if (dyn_array->real_size == dyn_array->buffer_size)
        Expand(dyn_array);
    assert(dyn_array->real_size < dyn_array->buffer_size && dyn_array->buffer != 0);
    dyn_array->buffer[dyn_array->real_size].temperature = day->temperature;
    dyn_array->buffer[dyn_array->real_size].precipitation = day->precipitation;
    dyn_array->buffer[dyn_array->real_size++].wind_speed = day->wind_speed;
}

// works provided user checked emptiness
DayWeather Pop(DynArray *dyn_array) {
    assert(dyn_array->buffer != 0);
    return dyn_array->buffer[--dyn_array->real_size];
}