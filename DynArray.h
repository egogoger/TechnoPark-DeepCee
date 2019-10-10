#ifndef IS1_DYNARRAY_H
#define IS1_DYNARRAY_H

#include "DayWeather.h"

typedef struct {
    DayWeather *buffer;    // Содержимое
    size_t buffer_size;    // размер буфера
    size_t real_size;      // Он же хэд
} DynArray;

DynArray*   new_DynArray    ();
size_t      IsEmpty         (DynArray *dyn_array);
void        Expand          (DynArray *dyn_array);
void        Add             (DynArray *dyn_array, DayWeather* day);
DayWeather  Pop             (DynArray *dyn_array);
void        delete_DynArray (DynArray *dyn_array);
int         malloc_fail     (DynArray *dyn_array);


#endif //IS1_DYNARRAY_H

#define DAYS_IN_WEEK 7