#ifndef PROJECT_LINEAR_LINEAR_H_
#define PROJECT_LINEAR_LINEAR_H_

#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"

#define MAX_CHARS 50

int find_indices_linear(const char *const filename, const size_t seqs_amount, char **sequences);

#endif  // PROJECT_LINEAR_LINEAR_H_
