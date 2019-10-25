#ifndef SUBSEQUENCES_THREADS_H
#define SUBSEQUENCES_THREADS_H

#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"

#define MAX_CHARS 50
#define MAX_THREADS 4

typedef struct{
    char* filename;
    char* sequence;
    DynArray* array;
} thread_args;

char* input_filename();
int input_amount();
char** input_seqs(const size_t amount);
void* my_strstr(void* args);

int find_indices(char* const filename, const size_t seqs_amount, char **sequences);

#endif //SUBSEQUENCES_THREADS_H
