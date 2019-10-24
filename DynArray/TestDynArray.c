#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"

int test_creation() {
    DynArray *temp_da = new_DynArray();
    if ( !temp_da ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }
    delete_DynArray(temp_da);
    return EXIT_SUCCESS;
}

int test_is_empty() {
    DynArray *temp_da = new_DynArray();

    if( !is_empty(temp_da) ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }

    add(temp_da, 10);

    if ( is_empty(temp_da) ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }

    delete_DynArray(temp_da);
    return EXIT_SUCCESS;
}

int test_grow_and_add() {

    DynArray *temp_da = new_DynArray();

    for ( int iii = 0; iii < 10; iii++ ) {
        add(temp_da, iii);
    }
    if ( temp_da->real_size != 10 ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }

    int previous_buffer_size = temp_da->buffer_size;
    grow(temp_da);
    if ( temp_da->buffer_size <= previous_buffer_size ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }

    delete_DynArray(temp_da);
    return EXIT_SUCCESS;
}

int test_pop() {
    DynArray *temp_da = new_DynArray();

    int test_value = 10;

    add(temp_da, test_value);
    int previous_real_size = temp_da->real_size;

    int tmp = pop(temp_da);

    if ( tmp != test_value && previous_real_size != temp_da->real_size ) {
        delete_DynArray(temp_da);
        return EXIT_FAILURE;
    }

    delete_DynArray(temp_da);
    return EXIT_SUCCESS;
}


int main() {

    int flag = EXIT_SUCCESS;

    if ( test_creation() == EXIT_FAILURE ) {
        fprintf(stderr, "new_DynArray() failed\n");
        flag = EXIT_FAILURE;
    }

    if ( test_is_empty() == EXIT_FAILURE ) {
        fprintf(stderr, "is_empty() failed\n");
        flag = EXIT_FAILURE;
    }

    if ( test_grow_and_add() == EXIT_FAILURE ) {
        fprintf(stderr, "grow() and add() failed\n");
        flag = EXIT_FAILURE;
    }

    if ( test_pop() == EXIT_FAILURE ) {
        fprintf(stderr, "pop() failed\n");
        flag = EXIT_FAILURE;
    }

    return flag;
}