#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"
#include "linear.h"

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

int find_indices(const char* const filename, const size_t seqs_amount, char **sequences) {

    /// Input file name and open (or else)
    FILE *gibberish;
    if ( (gibberish = fopen(filename, "r") ) == NULL) {
        fprintf(stderr, "Failed to open a file\n");
        fprintf(stderr, "%s\n", filename);
        return -1;
    }

    /// Dynamic array of starting indices of sequences
    DynArray **indices = (DynArray **)calloc(seqs_amount, sizeof(DynArray*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        indices[iii] = new_DynArray();
    }

    /// Counting here
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        char ch;
        int jjj = 0;
        while ( !feof(gibberish) ) {
            if ( (ch = fgetc(gibberish)) == sequences[iii][0] ) {
                fpos_t position;
                fgetpos(gibberish, &position);         // Remember the position to return to

                int8_t full_match = 1;

                /// Go through the sequence
                for (size_t kkk = 1; (sequences[iii][kkk] != '\0') && (full_match != 0); kkk++) {
                    if ( (ch = fgetc(gibberish)) != sequences[iii][kkk] )
                        full_match = 0;
                }

                if (full_match == 1) {
                    add(indices[iii], jjj);
                }
                fsetpos(gibberish, &position);         // Set the pointer in file back
            }
            jjj++;
        }
        rewind(gibberish);     // go back in the beginning of the file
    }

    int result = 0;
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        result += indices[iii]->real_size;
    }
//    /// Printing out indices
//    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
//        printf("%s: ", sequences[iii]);
//        for ( int jjj = 0; jjj < indices[iii]->real_size; jjj++ ) {
//            printf("%d, ", indices[iii]->buffer[jjj]);
//        }
//        puts("");
//    }

    /// Freeing space
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        free(sequences[iii]);
        delete_DynArray(indices[iii]);
    }
    free(sequences);
    free(indices);

    if (fclose(gibberish)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }

    return result;
}

/*
int main() {

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
}
*/
