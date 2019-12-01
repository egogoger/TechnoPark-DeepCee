#ifndef PROJECT_DYNARRAY_DYNARRAY_H_
#define PROJECT_DYNARRAY_DYNARRAY_H_

typedef struct {
    int *buffer;      // Содержимое
    int buffer_size;  // размер буфера
    int real_size;    // Он же хэд
} DynArray;

DynArray *new_DynArray();

int is_empty(DynArray *dyn_array);

void grow(DynArray *dyn_array);

void add(DynArray *dyn_array, int day);

int pop(DynArray *dyn_array);

int delete_DynArray(DynArray *dyn_array);

#endif  // PROJECT_DYNARRAY_DYNARRAY_H_
