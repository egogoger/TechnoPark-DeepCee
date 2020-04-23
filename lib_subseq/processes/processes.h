#ifndef PROJECT_PROCESSES_PROCESSES_H_
#define PROJECT_PROCESSES_PROCESSES_H_

#define MAX_CHARS 50
#define MAX_PROCESSES sysconf(_SC_NPROCESSORS_ONLN)
#define SIZE_T_RANGE 65535

void collect_garbage_proc(dyn_array **array_2d, int **fds, size_t len_2d, pid_t *pid);

void proc_strstr(const char *const filename, const char *sequence, dyn_array *array);

int find_indices_proc(char *const filename, const size_t seqs_amount, char **sequences);

#endif  //  PROJECT_PROCESSES_PROCESSES_H_
