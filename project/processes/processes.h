#ifndef PROJECT_PROCESSES_PROCESSES_H_
#define PROJECT_PROCESSES_PROCESSES_H_

#define MAX_CHARS 50
#define MAX_PROCESSES 2
#define SIZE_T_RANGE 65535

void proc_strstr(const char* const filename, const char* sequence, DynArray *array);

int find_indices(char* const filename, const size_t seqs_amount, char **sequences);

#endif  //  PROJECT_PROCESSES_PROCESSES_H_
