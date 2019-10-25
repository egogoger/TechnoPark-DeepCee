#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "DynArray.h"
#include "threads.h"

int find_indices(char* const filename, const size_t seqs_amount, char **sequences) {
    /// Dynamic array of starting indices of sequences
    DynArray **indices = (DynArray **)calloc(seqs_amount, sizeof(DynArray*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        indices[iii] = new_DynArray();
    }

    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

    thread_args *args = (thread_args*)calloc(MAX_THREADS, sizeof(thread_args));
    pthread_t threads[MAX_THREADS];                 // Array of threads
    for ( size_t iii = 0; iii < seqs_amount; iii += MAX_THREADS ) {
        /// This for-loop merely limits the amount of simultaneously running threads
        for ( size_t jjj = 0; jjj < MAX_THREADS && jjj + iii < seqs_amount; jjj++ ) {
            /// Prepare arguments
            args[jjj].filename = filename;
            args[jjj].sequence = sequences[iii + jjj];
            args[jjj].array = indices[iii + jjj];

            /// Create a thread and give it a job
            if ( pthread_create(&threads[jjj], NULL, &my_strstr, (void *)&args[jjj]) != 0 ) {
                fprintf(stderr, "Failed to create thread # %lu\n", iii + jjj);
                return EXIT_FAILURE;
            }
        }
        /// Wait for threads to finish
        for ( size_t jjj = 0; jjj < MAX_THREADS && jjj + iii < seqs_amount; jjj++ ) {
            if ( pthread_join(threads[jjj], NULL) != 0 ) {
                fprintf(stderr, "Cannot join thread # %lu\n", iii + jjj);
            }
        }
    }


    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

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
    free(args);

    return result;
}

void* my_strstr(void* args) {
    thread_args *arguments = args;

    /// Open file
    FILE* gibber;
    if ( (gibber = fopen(arguments->filename, "r")) == NULL ) {
        fprintf(stderr, "Failed to open a file\n");
        exit(EXIT_FAILURE);
    }

    /// Start searching
    char ch;
    int jjj = 0;
    while ( !feof(gibber) ) {
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

    return NULL;
}
