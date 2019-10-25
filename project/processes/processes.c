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
                exit(EXIT_FAILURE);
            } else if ( pid[jjj] == 0 ) {
                /// Child process works
                close(fds[jjj][0]);  // close pipe for reading
                proc_strstr(filename, sequences[iii + jjj], indices[iii + jjj]);

                write(fds[jjj][1], &(indices[iii + jjj]->real_size),
                        sizeof(indices[iii + jjj]->real_size));
                write(fds[jjj][1], &(indices[iii + jjj]->buffer_size),
                        sizeof(indices[iii + jjj]->buffer_size));

                int size = indices[iii + jjj]->real_size;
                int pack = 0;
                int bytes = 0;
                while ( size > SIZE_T_RANGE ) {
                    bytes = write(fds[jjj][1],
                            (indices[iii + jjj]->buffer) + (pack * sizeof(int)), SIZE_T_RANGE);
                    size -= SIZE_T_RANGE;
                    if ( bytes != indices[iii + jjj]->real_size * sizeof(int) ) {
                        fprintf(stderr, "WRITE ERROR for indices[%lu]", iii + jjj);
                    }
                    pack++;
                }
                bytes = write(fds[jjj][1], (indices[iii + jjj]->buffer) + (pack * sizeof(int)), size);
                if ( bytes != size ) {
                    fprintf(stderr, "WRITE ERROR for indices[%lu]\n", iii);
                }

                exit(EXIT_SUCCESS);
            }
        }

        for ( size_t jjj = 0; jjj < MAX_PROCESSES; jjj++ ) {
            /// Wait till all processes are over
            waitpid(pid[jjj], &status, 0);
            if ( WIFEXITED(status) == 0 ) {
                fprintf(stderr, "Failed to finish a process\n");
                for ( size_t kkk = 0; kkk < seqs_amount; kkk++ ) {
                    free(sequences[kkk]);
                    delete_DynArray(indices[kkk]);
                    free(fds[kkk]);
                }
                free(sequences);
                free(indices);
                free(fds);
                free(pid);
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
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        free(sequences[iii]);
        delete_DynArray(indices[iii]);
        free(fds[iii]);
    }
    free(sequences);
    free(indices);
    free(fds);
    free(pid);

    return result;
}

void proc_strstr(const char* const filename, const char* sequence, DynArray *array) {
    /// Open file
    FILE* gibber;
    if ( (gibber = fopen(filename, "r") ) == NULL ) {
        fprintf(stderr, "Failed to open a file\n");
        exit(EXIT_FAILURE);
    }

    /// Start searching
    char ch;
    int jjj = 0;
    while ( !feof(gibber) ) {
        if ( (ch = fgetc(gibber)) == sequence[0] ) {
            fpos_t position;
            fgetpos(gibber, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ( (ch = fgetc(gibber)) != sequence[kkk] )
                    full_match = 0;
            }

            if (full_match == 1) {
                add(array, jjj);
            }
            fsetpos(gibber, &position);         // Set file pointer back
        }
        jjj++;
    }

    if (fclose(gibber)) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}
