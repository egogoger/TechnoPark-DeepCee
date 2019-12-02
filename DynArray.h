#ifndef IS1_DYNARRAY_H
#define IS1_DYNARRAY_H

#include "DayWeather.h"

typedef struct {
    DayWeather *buffer;    // Содержимое
    size_t buffer_size;    // размер буфера
    size_t real_size;      // Он же хэд
} DynArray;

DynArray *new_DynArray();

size_t is_empty(const DynArray *dyn_array);

void expand(DynArray *dyn_array);

void add(DynArray *dyn_array, const DayWeather *day);

DayWeather pop(DynArray *dyn_array);

int delete_DynArray(DynArray *dyn_array);

#endif // IS1_DYNARRAY_H

#define DAYS_IN_WEEK 7