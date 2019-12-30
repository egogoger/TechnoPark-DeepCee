#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dyn_array.h"
#include "threads.h"

int find_indices_threads(const char *const filename, const size_t seqs_amount, char **sequences) {
    /// Dynamic array of starting indices of sequences
    dyn_array **indices = (dyn_array **) calloc(seqs_amount, sizeof(dyn_array *));
    if (indices == NULL) {
        fprintf(stderr, "Failed to allocate memory for dyn_array\n");
        collect_garbage_threads(indices, seqs_amount, NULL, NULL);
        exit(EXIT_FAILURE);
    }
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        indices[iii] = new_dyn_array();
    }

    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

    thread_args *args = (thread_args *) calloc(MAX_THREADS, sizeof(thread_args));
    pthread_t *threads = (pthread_t *) calloc(MAX_THREADS, sizeof(pthread_t));
    if (threads == NULL || args == NULL) {
        fprintf(stderr, "Failed to allocate memory for threads\n");
        collect_garbage_threads(indices, seqs_amount, args, threads);
        exit(EXIT_FAILURE);
    }

    for (size_t iii = 0; iii < seqs_amount; iii += MAX_THREADS) {
        /// This for-loop merely limits the amount of simultaneously running threads
        for (size_t jjj = 0; jjj < MAX_THREADS && jjj + iii < seqs_amount; jjj++) {
            /// Prepare arguments
            args[jjj].filename = filename;
            args[jjj].sequence = sequences[iii + jjj];
            args[jjj].array = indices[iii + jjj];

            /// Check for file existence first
            if (access(filename, R_OK) == -1) {
                fprintf(stderr, "Failed to open a file\n");
                collect_garbage_threads(indices, seqs_amount, args, threads);
                return EXIT_FAILURE;
            }

            /// Create a thread and give it a job
            if (pthread_create(&threads[jjj], NULL, &threads_strstr, (void *) &args[jjj]) != 0) {
                fprintf(stderr, "Failed to create thread # %lu\n", iii + jjj);
                collect_garbage_threads(indices, seqs_amount, args, threads);
                return EXIT_FAILURE;
            }
        }
        /// Wait for threads to finish
        for (size_t jjj = 0; jjj < MAX_THREADS && jjj + iii < seqs_amount; jjj++) {
            if (pthread_join(threads[jjj], NULL) != 0) {
                fprintf(stderr, "Cannot join thread # %lu\n", iii + jjj);
                collect_garbage_threads(indices, seqs_amount, args, threads);
                return EXIT_FAILURE;
            }
        }
    }


    /////////////////////////////////////////////////////////////
    ///////////////////////// MAIN AREA /////////////////////////
    /////////////////////////////////////////////////////////////

    int result = 0;
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        result += indices[iii]->real_size;
    }

    /// Freeing space
    collect_garbage_threads(indices, seqs_amount, args, threads);

    return result;
}

void *threads_strstr(void *args) {
    thread_args *arguments = args;

    /// Open file
    FILE *gibberish;
    gibberish = fopen(arguments->filename, "r");

    /// Start searching
    char ch;
    int jjj = 0;
    while (!feof(gibberish)) {
        if ((ch = fgetc(gibberish)) == arguments->sequence[0]) {
            fpos_t position;
            fgetpos(gibberish, &position);         // Remember the position to return to

            int8_t full_match = 1;

            /// Go through the sequence
            for (size_t kkk = 1; (arguments->sequence[kkk] != '\0') && (full_match != 0); kkk++) {
                if ((ch = fgetc(gibberish)) != arguments->sequence[kkk])
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

void collect_garbage_threads(dyn_array **array_2d, size_t len_2d, thread_args *args, pthread_t *threads) {
    for (size_t iii = 0; iii < len_2d; iii++) {
        delete_dyn_array(array_2d[iii]);
    }
    free(array_2d);
    free(args);
    free(threads);
}
