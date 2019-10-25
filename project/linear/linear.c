#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"

int find_indices(const char* const filename, const size_t seqs_amount, char **sequences) {
    /// Input file name and open (or else)
    FILE *gibberish;
    if ( (gibberish = fopen(filename, "r")) == NULL ) {
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
