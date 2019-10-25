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

            /// Check for file existence first
            if ( check_file(filename) == EXIT_FAILURE ) {
                collect_garbage(indices, seqs_amount, args);
                return EXIT_FAILURE;
            }

            /// Create a thread and give it a job
            if ( pthread_create(&threads[jjj], NULL, &my_strstr, (void *)&args[jjj]) != 0 ) {
                fprintf(stderr, "Failed to create thread # %lu\n", iii + jjj);
                collect_garbage(indices, seqs_amount, args);
                return EXIT_FAILURE;
            }
        }
        /// Wait for threads to finish
        for ( size_t jjj = 0; jjj < MAX_THREADS && jjj + iii < seqs_amount; jjj++ ) {
            if ( pthread_join(threads[jjj], NULL) != 0 ) {
                fprintf(stderr, "Cannot join thread # %lu\n", iii + jjj);
                collect_garbage(indices, seqs_amount, args);
                return EXIT_FAILURE;
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
    collect_garbage(indices, seqs_amount, args);

    return result;
}

void* my_strstr(void* args) {
    thread_args *arguments = args;

    /// Open file
    FILE* gibberish;
    gibberish = fopen(arguments->filename, "r");

    /// Start searching
    char ch;
    int jjj = 0;
    while ( !feof(gibberish) ) {
        if ( (ch = fgetc(gibberish)) == arguments->sequence[0] ) {
            fpos_t position;
            fgetpos(gibberish, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (arguments->sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ( (ch = fgetc(gibberish)) != arguments->sequence[kkk] )
                    full_match = 0;
            }

            if (full_match == 1) {
                add(arguments->array, jjj);
            }
            fsetpos(gibberish, &position);         // Set the pointer in file back
        }
        jjj++;
    }

    /// Close file
    fclose(gibberish);

    return NULL;
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

void collect_garbage(DynArray **array_2d, size_t len_2d, thread_args *args) {
    for ( size_t iii = 0; iii < len_2d; iii++ ) {
        delete_DynArray(array_2d[iii]);
    }
    free(array_2d);
    free(args);
}
