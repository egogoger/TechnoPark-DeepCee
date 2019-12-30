#ifndef PROJECT_DYNARRAY_DYNARRAY_H_
#define PROJECT_DYNARRAY_DYNARRAY_H_

typedef struct {
    int *buffer;      // Содержимое
    int buffer_size;  // размер буфера
    int real_size;    // Он же хэд
} dyn_array;

dyn_array *new_dyn_array();

int is_empty(dyn_array *dyn_array);

void grow(dyn_array *dyn_array);

void add(dyn_array *dyn_array, int day);

int pop(dyn_array *dyn_array);

int delete_dyn_array(dyn_array *dyn_array);

#endif  // PROJECT_DYNARRAY_DYNARRAY_H_
