/*
 * Перед вами поставлена задача поиска заданных последовательностей символов
 * в загруженном в оперативную память файле размером 100 Мб.
 *
 * Составьте наивный алгоритм поиска, который сначала ищет вхождения первой последовательности,
 * затем — второй и т.д., а затем распараллельте его на несколько процессов
 * с учетом оптимизации работы с кэш-памятью.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "DynArray/DynArray.h"

#define MAX_CHARS 50

typedef struct{
    char* filename;
    char* sequence;
    DynArray* array;
} thread_args;

int input_amount();
void input_seqs(const size_t amount, char **array);
void* my_strstr(void* args);


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

    thread_args *args = (thread_args*)calloc(seqs_amount, sizeof(thread_args)); // Array of arguments for threads
    pthread_t threads[seqs_amount];                 // Array of threads
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        /// Prepare arguments
        args[iii].filename = filename;
        args[iii].sequence = sequences[iii];
        args[iii].array = indices[iii];

        /// Create a thread
        if ( pthread_create(&threads[iii], NULL, &my_strstr, (void *)&args[iii]) != 0 ) {
            fprintf(stderr, "Failed to create thread # %lu\n", iii);
            return EXIT_FAILURE;
        }
    }

    /// Wait for threads to finish
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        if ( pthread_join(threads[iii], NULL) != 0 ) {
            fprintf(stderr, "Cannot join thread # %lu\n", iii);
        }
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
    }
    free(sequences);
    free(indices);
    free(args);

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


void* my_strstr(void* args) {

    thread_args *arguments = args;

    FILE* gibber;
    if ( (gibber = fopen(arguments->filename, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        exit(EXIT_FAILURE);
    }
    char ch;
    int jjj = 0;
    while ( !feof(gibber) ) {        // TODO: remove the 50000
        if ( (ch = fgetc(gibber)) == arguments->sequence[0] ) {
            fpos_t position;
            fgetpos(gibber, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (arguments->sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ( (ch = fgetc(gibber)) != arguments->sequence[kkk] )
                    full_match = 0;
            }

            if (full_match == 1) {
                add(arguments->array, jjj);
            }
            fsetpos(gibber, &position);         // Set the pointer in file back
        }
        jjj++;
    }
    if (fclose(gibber)) {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}