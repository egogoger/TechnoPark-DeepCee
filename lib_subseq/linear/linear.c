#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "DynArray.h"
#include "linear.h"

int find_indices_linear(const char *const filename, const size_t seqs_amount, char **sequences) {
    /// Check for file existence first
    if (access(filename, R_OK) == -1) return EXIT_FAILURE;

    /// Open file
    FILE *gibberish = fopen(filename, "r");

    /// Dynamic array of starting indices of sequences
    DynArray **indices = (DynArray **) calloc(seqs_amount, sizeof(DynArray *));
    if ( indices == NULL ) {
        for (size_t iii = 0; iii < seqs_amount; iii++) {
            delete_DynArray(indices[iii]);
        }
        free(indices);
        fclose(gibberish);
        return EXIT_FAILURE;
    }
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        indices[iii] = new_DynArray();
    }

    /// Counting here
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        char ch;
        int jjj = 0;
        while (!feof(gibberish)) {
            if ((ch = fgetc(gibberish)) == sequences[iii][0]) {
                fpos_t position;
                fgetpos(gibberish, &position);         // Remember the position to return to

                int8_t full_match = 1;

                /// Go through the sequence
                for (size_t kkk = 1; (sequences[iii][kkk] != '\0') && (full_match != 0); kkk++) {
                    if ((ch = fgetc(gibberish)) != sequences[iii][kkk])
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

    /// Obtain result
    int result = 0;
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        result += indices[iii]->real_size;
    }

    /// Free space
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        delete_DynArray(indices[iii]);
    }
    free(indices);

    /// Close file
    fclose(gibberish);
    return result;
}
