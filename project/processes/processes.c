#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "DynArray.h"
#include "processes.h"

/*int main() {

    /// Input filename
    char* filename;
    if ( (filename = input_filename()) == NULL) {
        return EXIT_FAILURE;
    }

    /// Input amount of sequences
    size_t seqs_amount = input_amount();

    /// Input array of words
    char **sequences = input_seqs(seqs_amount);

    /// Do the work
    find_indices(filename, seqs_amount, sequences);

    return EXIT_SUCCESS;
}*/

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

    pid_t *pid = (pid_t *)calloc(seqs_amount, sizeof(pid_t));       // Array of pids
    int **fds = (int **)calloc(seqs_amount, sizeof(int*));          // Array of fds

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {

        /// Create and open pipe for each fd
        fds[iii] = (int *)calloc(2, sizeof(int));             // fd for every child process
        pipe(fds[iii]);             // deleted check

        /// Create child process for that pipe
        pid[iii] = fork();
        if ( pid[iii] == -1 ) {
            fprintf(stderr, "Failed to fork\n");
            exit(EXIT_FAILURE);
        } else if ( pid[iii] == 0 ) {

            /// Child process works
//            printf("##### child #%lu works #####\n", iii);
            close(fds[iii][0]);
            proc_strstr(filename, sequences[iii], indices[iii]);

            write(fds[iii][1], &(indices[iii]->real_size), sizeof(indices[iii]->real_size));
            write(fds[iii][1], &(indices[iii]->buffer_size), sizeof(indices[iii]->buffer_size));
            write(fds[iii][1], indices[iii]->buffer, indices[iii]->real_size * sizeof(int));

//            printf("##### child #%lu stops #####\n", iii);
            exit(EXIT_SUCCESS);
        }
    }

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {

        /// Wait till all processes are over
//        printf("waitpid returned %d\n", waitpid(pid[iii], &status, 0));
        waitpid(pid[iii], &status, 0);
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

        int tempo = 0;
        close(fds[iii][1]);
        read(fds[iii][0], &(indices[iii]->real_size), sizeof(indices[iii]->real_size));
        read(fds[iii][0], &(indices[iii]->buffer_size), sizeof(indices[iii]->buffer_size));
        tempo = read(fds[iii][0], indices[iii]->buffer, indices[iii]->real_size * sizeof(int));
//        printf("read - %d\n", tempo);

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
    if ( (gibber = fopen(filename, "r") ) == NULL) {
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


char* input_filename() {
    /// Input filename and check for his existence
    FILE *gibber;
    char* filename = (char *)calloc(MAX_CHARS, sizeof(char));
    printf("File name: ");
    scanf("%s", filename);
    if ( (gibber = fopen(filename, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        return NULL;
    } else {
        return filename;
    }
}

int input_amount() {
    printf("Amount of sequences: ");
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

char** input_seqs(const size_t seqs_amount) {
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ )
        sequences[iii] = (char *)calloc(MAX_CHARS, sizeof(char));

    scanf(" %s", sequences[0]);
    for ( size_t iii = 1; iii < seqs_amount; iii++ ) {
        scanf("%s", sequences[iii]);
    }
    return sequences;
}
