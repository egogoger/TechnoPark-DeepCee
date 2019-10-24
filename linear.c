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

#include "DynArray/DynArray.h"

#define MAX_CHARS 50

int input_amount();
void input_seqs(const size_t amount, char **array);


int main() {

    /// Input file name and open (or else)
    FILE *gibber;
    char buffer[50];
    printf("File name: ");
    scanf("%s", buffer);
    if ( (gibber = fopen(buffer, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        return EXIT_FAILURE;
    }

    /// Creating array of words
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

    //////////////////////////////////////////////////////////////////
    ///////////////////////// TEST AREA //////////////////////////////
    //////////////////////////////////////////////////////////////////

    /// Counting here
    for ( int iii = 0; iii < seqs_amount; iii++ ) {
        fpos_t start;
        fgetpos(gibber, &start);
        int index = 0;
        char ch;
        int jjj = 0;
        while ( !feof(gibber) ) {
            if ( (ch = fgetc(gibber)) == sequences[iii][0] ) {
                fpos_t position;
                fgetpos(gibber, &position);         // Remember the position to return to

                int8_t full_match = 1;

                /// Go through the sequence
                for (size_t kkk = 1; (sequences[iii][kkk] != '\0') && (full_match != 0); kkk++) {
                    if ( (ch = fgetc(gibber)) != sequences[iii][kkk] )
                        full_match = 0;
                }

                if (full_match == 1) {
                    add(indices[iii], jjj);
                }
                fsetpos(gibber, &position);         // Set the pointer in file back
            }
            jjj++;
        }
        fsetpos(gibber, &start);        // Set the pointer in file back
    }

    //////////////////////////////////////////////////////////////////
    ///////////////////////// TEST AREA //////////////////////////////
    //////////////////////////////////////////////////////////////////

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

    if (fclose(gibber)) {
        fprintf(stderr, "Failed to close file\n");
        return EXIT_FAILURE;
    }

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