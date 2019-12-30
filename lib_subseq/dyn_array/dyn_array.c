#include "dyn_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dyn_array *new_dyn_array() {
    dyn_array *dyn_array = calloc(1, sizeof(dyn_array));
    if (!dyn_array)
        return NULL;

    dyn_array->buffer_size = 2;

    dyn_array->buffer = (int *) calloc(dyn_array->buffer_size, sizeof(int));
    if (!dyn_array->buffer) {
        delete_dyn_array(dyn_array);
        return NULL;
    }
    dyn_array->real_size = 0;

    return dyn_array;
}

int delete_dyn_array(dyn_array *dyn_array) {
    if (!dyn_array)
        return EXIT_FAILURE;
    free(dyn_array->buffer);
    free(dyn_array);
    return EXIT_SUCCESS;
}

int is_empty(dyn_array *dyn_array) {
    return (dyn_array->real_size == 0) ? 1 : 0;
}

void grow(dyn_array *dyn_array) {
    size_t bigger_buffer_size = dyn_array->buffer_size * 2;

    int *bigger_buffer;
    bigger_buffer = (int *) malloc(bigger_buffer_size * sizeof(int));
    if (!bigger_buffer)
        delete_dyn_array(dyn_array);

    memcpy(bigger_buffer, dyn_array->buffer,
           dyn_array->buffer_size * sizeof(int));

    free(dyn_array->buffer);

    dyn_array->buffer = bigger_buffer;
    dyn_array->buffer_size = bigger_buffer_size;
}

void add(dyn_array *dyn_array, int number) {
    if (dyn_array->real_size == dyn_array->buffer_size)
        grow(dyn_array);
    //    assert(dyn_array->real_size < dyn_array->buffer_size &&
    //    dyn_array->buffer != 0);
    dyn_array->buffer[dyn_array->real_size++] = number;
}

// works provided user checked emptiness
int pop(dyn_array *dyn_array) {
    //    assert(dyn_array->buffer != 0);
    return dyn_array->buffer[--dyn_array->real_size];
}
