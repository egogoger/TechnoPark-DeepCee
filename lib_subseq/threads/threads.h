#ifndef PROJECT_THREADS_THREADS_H_
#define PROJECT_THREADS_THREADS_H_

#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"

#define MAX_CHARS 50
#define MAX_THREADS sysconf(_SC_NPROCESSORS_ONLN)

typedef struct {
    char *filename;
    char *sequence;
    dyn_array *array;
} thread_args;

void collect_garbage_threads(dyn_array **array_2d, size_t len_2d, thread_args *args, pthread_t *threads);

void *threads_strstr(void *args);

int find_indices_threads(const char *const filename, const size_t seqs_amount, char **sequences);

#endif  //  PROJECT_THREADS_THREADS_H_
