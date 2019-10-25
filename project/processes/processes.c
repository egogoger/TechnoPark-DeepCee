#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "DynArray.h"
#include "processes.h"

int find_indices(char* const filename, const size_t seqs_amount, char **sequences) {
    /// Dynamic array of starting indices of sequences
    DynArray **indices = (DynArray **)calloc(seqs_amount, sizeof(DynArray*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        indices[iii] = new_DynArray();
    }

    /////////////////////////////////////////////////////////////
    ////////////////////// MAIN AREA start //////////////////////
    /////////////////////////////////////////////////////////////

    int status;

    pid_t *pid = (pid_t *)calloc(MAX_PROCESSES, sizeof(pid_t));       // Array of pids
    int **fds = (int **)calloc(MAX_PROCESSES, sizeof(int*));          // Array of fds

    for ( size_t iii = 0; iii < seqs_amount; iii += MAX_PROCESSES ) {
        for ( size_t jjj = 0; (jjj < MAX_PROCESSES) && (jjj + iii < seqs_amount); jjj++ ) {
            /// Create and open pipe for each fd
            fds[jjj] = (int *)calloc(2, sizeof(int));
            pipe(fds[jjj]);

            /// Create child process for that pipe
            pid[jjj] = fork();
            if ( pid[jjj] == -1 ) {
                fprintf(stderr, "Failed to fork\n");
                collect_garbage(indices, fds, seqs_amount, pid);
                exit(EXIT_FAILURE);
            } else if ( pid[jjj] == 0 ) {
                /// Child process works
                close(fds[jjj][0]);  // close pipe for reading
                
                /// Check for file existence first
                if ( check_file(filename) == EXIT_FAILURE ) {
                    collect_garbage(indices, fds, seqs_amount, pid);
                    return EXIT_FAILURE;
                }
                proc_strstr(filename, sequences[iii + jjj], indices[iii + jjj]);

                write(fds[jjj][1], &(indices[iii + jjj]->real_size),
                        sizeof(indices[iii + jjj]->real_size));
                write(fds[jjj][1], &(indices[iii + jjj]->buffer_size),
                        sizeof(indices[iii + jjj]->buffer_size));

                int size = indices[iii + jjj]->real_size;
                int pack = 0;
                while ( size > SIZE_T_RANGE ) {
                    write(fds[jjj][1],
                            (indices[iii + jjj]->buffer) + (pack * sizeof(int)), SIZE_T_RANGE);
                    size -= SIZE_T_RANGE;
                    pack++;
                }
                write(fds[jjj][1], (indices[iii + jjj]->buffer) + (pack * sizeof(int)), size);

                collect_garbage(indices, fds, seqs_amount, pid);  // not sure here
                exit(EXIT_SUCCESS);
            }
        }

        for ( size_t jjj = 0; jjj < MAX_PROCESSES; jjj++ ) {
            /// Wait till all processes are over
            waitpid(pid[jjj], &status, 0);
            if ( WIFEXITED(status) == 0 ) {
                fprintf(stderr, "Failed to finish a process\n");
                collect_garbage(indices, fds, seqs_amount, pid);
                exit(EXIT_FAILURE);
            }

            /// Read parts of DynArray
            close(fds[jjj][1]);
            read(fds[jjj][0], &(indices[iii + jjj]->real_size), sizeof(indices[iii + jjj]->real_size));
            read(fds[jjj][0], &(indices[iii + jjj]->buffer_size), sizeof(indices[iii + jjj]->buffer_size));

            /// allocate more memory for indices[iii]->buffer
            indices[iii + jjj]->buffer = (int *)realloc(indices[iii + jjj]->buffer,
                    indices[iii + jjj]->buffer_size * sizeof(int));

            read(fds[jjj][0], indices[iii + jjj]->buffer, indices[iii + jjj]->real_size * sizeof(int));
        }
    }


    ////////////////////////////////////////////////////////////
    ////////////////////// MAIN AREA over //////////////////////
    ////////////////////////////////////////////////////////////

    int result = 0;
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        result += indices[iii]->real_size;
    }

    /// Freeing space
    collect_garbage(indices, fds, seqs_amount, pid);

    return result;
}

void proc_strstr(const char* const filename, const char* sequence, DynArray *array) {
    /// Open file
    FILE* gibberish = fopen(filename, "r");

    /// Start searching
    char ch;
    int jjj = 0;
    while ( !feof(gibberish) ) {
        if ( (ch = fgetc(gibberish)) == sequence[0] ) {
            fpos_t position;
            fgetpos(gibberish, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ( (ch = fgetc(gibberish)) != sequence[kkk] )
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

int check_file(const char* filename) {
    FILE* f;
    if ( (f = fopen(filename, "r")) == NULL ) {
        fprintf(stderr, "Failed to open a file\n");
        return(EXIT_FAILURE);
    }
    if ( fclose(f) ) {
        fprintf(stderr, "Failed to close a file\n");
        return(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

void collect_garbage(DynArray **array_2d, int **fds, size_t len_2d, pid_t *pid) {
    for ( size_t iii = 0; iii < len_2d; iii++ ) {
        delete_DynArray(array_2d[iii]);
        free(fds[iii]);
    }
    free(array_2d);
    free(fds);
    free(pid);
}
