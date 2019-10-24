/*
 * Перед вами поставлена задача поиска заданных последовательностей символов
 * в загруженном в оперативную память файле размером 100 Мб.
 *
 * Составьте наивный алгоритм поиска, который сначала ищет вхождения первой последовательности,
 * затем — второй и т.д., а затем распараллельте его на несколько процессов
 * с учетом оптимизации работы с кэш-памятью.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "DynArray/DynArray.h"

#define MAX_CHARS 50

int input_amount();
void input_seqs(const size_t amount, char **array);
void proc_strstr(const char* filename, const char* sequence, DynArray *array);


int main() {

    /// Check for file existence
    FILE *gibber;
    char filename[50];
    printf("File name: ");
    scanf("%s", filename);
    if ( (gibber = fopen(filename, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        return EXIT_FAILURE;
    } else {
        fclose(gibber);
    }

    /// Creating array of sequences
    printf("Amount of sequences: ");
    int seqs_amount = input_amount();
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        sequences[iii] = (char *)calloc(MAX_CHARS, sizeof(char));
    }
    input_seqs(seqs_amount, sequences);

    /// Dynamic array of starting indices of sequences
    DynArray **indices = (DynArray **)calloc(seqs_amount, sizeof(DynArray*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        indices[iii] = new_DynArray();
    }

    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

    int status;

    pid_t *pid = (pid_t *)calloc(seqs_amount, sizeof(pid_t));       // Array of pids
    int **fds = (int **)calloc(seqs_amount, sizeof(int*));          // Array of fds

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {

        /// Open pipe for each fd
        fds[iii] = (int *)calloc(2, sizeof(int));             // fd for every child process
        pipe(fds[iii]);             // deleted check

        /// Create child process for that pipe
        pid[iii] = fork();
        if ( pid[iii] == -1 ) {
            fprintf(stderr, "Failed to fork\n");
            exit(EXIT_FAILURE);
        } else if ( pid[iii] == 0 ) {

            /// Child process works
            close(fds[iii][0]);
            proc_strstr(filename, sequences[iii], indices[iii]);

            write(fds[iii][1], &(indices[iii]->real_size), sizeof(indices[iii]->real_size));
            write(fds[iii][1], &(indices[iii]->buffer_size), sizeof(indices[iii]->buffer_size));
            write(fds[iii][1], indices[iii]->buffer, indices[iii]->real_size * sizeof(int));

            // TODO: fix the free
            for ( size_t kkk = 0; kkk < seqs_amount; kkk++ ) {
                free(sequences[kkk]);
                delete_DynArray(indices[kkk]);
                free(fds[kkk]);
            }
            free(sequences);
            free(indices);
            free(fds);
            free(pid);

            exit(EXIT_SUCCESS);
        }
    }

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {

        /// Wait till all processes are over
        waitpid(pid[iii], &status, 0);
        if ( WEXITSTATUS(status) ) {
            fprintf(stderr, "Failed to finish a process\n");
            exit(EXIT_FAILURE);
        }
        close(fds[iii][1]);

        /// Read parts of DynArray
        read(fds[iii][0], &(indices[iii]->real_size), sizeof(indices[iii]->real_size));
        read(fds[iii][0], &(indices[iii]->buffer_size), sizeof(indices[iii]->buffer_size));
        read(fds[iii][0], indices[iii]->buffer, indices[iii]->real_size * sizeof(int));

        close(fds[iii][0]);
    }


    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

    /// Printing out indices
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        printf("%s: ", sequences[iii]);
        for ( size_t jjj = 0; jjj < indices[iii]->real_size; jjj++ ) {
            printf("%d, ", indices[iii]->buffer[jjj]);
        }
        puts("");
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

    return EXIT_SUCCESS;
}

int input_amount() {
    int seqs_amount = 0;
    char buffer[50];
    scanf("%s", buffer);
    seqs_amount = atoi(buffer);
    while ( !seqs_amount ) {
        puts("Invalid string!");
        scanf("%s", buffer);
        seqs_amount = atoi(buffer);
    }

    return seqs_amount;
}

void input_seqs(const size_t amount, char **array) {
    scanf(" %s", array[0]);
    for ( int iii = 1; iii < amount; iii++ ) {
        scanf("%s", array[iii]);
    }
}


void proc_strstr(const char* filename, const char* sequence, DynArray *array) {

    /// Open file
    FILE* gibber;
    if ( (gibber = fopen(filename, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        exit(EXIT_FAILURE);
    }

    /// Start searching
    char ch;
    int jjj = 0;
    while ( !feof(gibber) ) {            // TODO: remove 50k
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