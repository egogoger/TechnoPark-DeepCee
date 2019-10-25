#ifndef SUBSEQUENCES_PROCESSES_H
#define SUBSEQUENCES_PROCESSES_H

#define MAX_CHARS 50
#define MAX_PROCESSES 4

char* input_filename();
int input_amount();
char** input_seqs(const size_t amount);
void proc_strstr(const char* const filename, const char* sequence, DynArray *array);

int find_indices(char* const filename, const size_t seqs_amount, char **sequences);

#endif //SUBSEQUENCES_PROCESSES_H
