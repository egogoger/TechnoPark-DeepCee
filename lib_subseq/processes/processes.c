#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processes.h"

int find_indices_proc(char *const filename, const size_t seqs_amount, char **sequences) {
    if (!filename || !sequences)
        return EXIT_FAILURE;
    /// Dynamic array of starting indices of sequences
    dyn_array **indices = (dyn_array **) calloc(seqs_amount, sizeof(dyn_array *));
    if (indices == NULL) {
        fprintf(stderr, "Failed to allocate memory for dyn_array\n");
        collect_garbage_proc(indices, NULL, seqs_amount, NULL);
        exit(EXIT_FAILURE);
    }
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        indices[iii] = new_dyn_array();
    }

    /////////////////////////////////////////////////////////////
    ////////////////////// MAIN AREA start //////////////////////
    /////////////////////////////////////////////////////////////

    int status;

    pid_t *pid = (pid_t *) calloc(MAX_PROCESSES, sizeof(pid_t));
    int **fds = (int **) calloc(MAX_PROCESSES, sizeof(int *));

    if (pid == NULL || fds == NULL) {
        fprintf(stderr, "Failed to allocate memory for pid or fds\n");
        collect_garbage_proc(indices, fds, seqs_amount, pid);
        exit(EXIT_FAILURE);
    }

    for (size_t iii = 0; iii < seqs_amount; iii += MAX_PROCESSES) {
        for (size_t jjj = 0; (jjj < MAX_PROCESSES) && (jjj + iii < seqs_amount); ++jjj) {
            /// Create and open pipe for each fd
            fds[jjj] = (int *) calloc(2, sizeof(int));
            if (fds[iii] == NULL) {
                fprintf(stderr, "Failed to allocate memory for fds[%zu]\n", jjj);
                collect_garbage_proc(indices, fds, seqs_amount, pid);
                exit(EXIT_FAILURE);
            }
            pipe(fds[jjj]);

            /// Create child process for that pipe
            pid[jjj] = fork();
            if (pid[jjj] == -1) {
                close(fds[jjj][0]);
                close(fds[jjj][1]);
                fprintf(stderr, "Failed to fork\n");
                collect_garbage_proc(indices, fds, seqs_amount, pid);
                exit(EXIT_FAILURE);
            } else if (pid[jjj] == 0) {
                /// Child process works
                close(fds[jjj][0]);  // close pipe for reading

                /// Check for file existence first
                if (access(filename, R_OK) == -1) {
                    fprintf(stderr, "Failed to open a file\n");
                    close(fds[jjj][1]);
                    collect_garbage_proc(indices, fds, seqs_amount, pid);
                    return EXIT_FAILURE;
                }

                proc_strstr(filename, sequences[iii + jjj], indices[iii + jjj]);

                write(fds[jjj][1], &(indices[iii + jjj]->real_size),
                      sizeof(indices[iii + jjj]->real_size));

                collect_garbage_proc(indices, fds, seqs_amount, pid);
                exit(EXIT_SUCCESS);
            }
        }

        for (size_t jjj = 0; (jjj < MAX_PROCESSES) && (jjj + iii < seqs_amount); ++jjj) {
            /// Wait till all processes are over
            waitpid(pid[jjj], &status, 0);
            if (WIFEXITED(status) == 0) {
                fprintf(stderr, "Failed to finish a process\n");
                close(fds[jjj][1]);
                collect_garbage_proc(indices, fds, seqs_amount, pid);
                exit(EXIT_FAILURE);
            }

            /// Read parts of dyn_array
            close(fds[jjj][1]);
            read(fds[jjj][0], &(indices[iii + jjj]->real_size), sizeof(indices[iii + jjj]->real_size));
        }
    }


    ////////////////////////////////////////////////////////////
    ////////////////////// MAIN AREA over //////////////////////
    ////////////////////////////////////////////////////////////

    /// Obtain result
    int result = 0;
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        result += indices[iii]->real_size;
    }

    /// Free space
    collect_garbage_proc(indices, fds, seqs_amount, pid);

    return result;
}

void proc_strstr(const char *const filename, const char *sequence, dyn_array *array) {
    if (!filename || !sequence || !array)
        return;
    /// Open file
    FILE *gibberish = fopen(filename, "r");

    /// Start searching
    char ch;
    int jjj = 0;
    while (!feof(gibberish)) {
        if ((ch = fgetc(gibberish)) == sequence[0]) {
            fpos_t position;
            fgetpos(gibberish, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ((ch = fgetc(gibberish)) != sequence[kkk])
                    full_match = 0;
            }

            if (full_match == 1) {
                add(array, jjj);
            }
            fsetpos(gibberish, &position);         // Set file pointer back
        }
        jjj++;
    }

    /// Close file
    fclose(gibberish);
}

void collect_garbage_proc(dyn_array **array_2d, int **fds, size_t len_2d, pid_t *pid) {
    for (size_t iii = 0; iii < len_2d; iii++) {
        delete_dyn_array(array_2d[iii]);
        free(fds[iii]);
    }
    free(array_2d);
    free(fds);
    free(pid);
}
