#ifndef SUBSEQUENCES_LINEAR_H
#define SUBSEQUENCES_LINEAR_H

#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"

#define MAX_CHARS 50

char* input_filename();
int input_amount();
char** input_seqs(const size_t amount);

int find_indices(const char* const filename, const size_t seqs_amount, char **sequences);

#endif //SUBSEQUENCES_LINEAR_H
