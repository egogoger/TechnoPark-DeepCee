#include <cstring>
#include <dlfcn.h>
#include "gtest/gtest.h"

#define SUBSEQ_LIB "./lib_subseq.so"

#define TEST_SMALL "test_files/small.txt"
#define TEST_MEDIUM "test_files/medium.txt"
#define TEST_BIG "test_files/big.txt"

#define SMALL_CORRECT_ANSWER 4
#define MEDIUM_CORRECT_ANSWER 41633
#define BIG_CORRECT_ANSWER 4183196

#define MAX_CHARS 50

static void delete_sequences(char **sequences, const size_t seqs_amount) {
    for (size_t iii = 0; iii < seqs_amount; iii++)
        delete (sequences[iii]);
    delete[](sequences);
}

TEST(processes, find_indices_proc) {
    void *dl_handle = dlopen(SUBSEQ_LIB, RTLD_LAZY);
    if (!dl_handle) GTEST_FAIL();

    int (*func)(const char *const, const size_t, char **) = NULL;
    *(void **) (&func) = dlsym(dl_handle, "find_indices_proc");
    if (!func) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    if (access(TEST_SMALL, R_OK) == -1 &&
        access(TEST_MEDIUM, R_OK) == -1 &&
        access(TEST_BIG, R_OK) == -1) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    size_t seqs_amount = 2;
    auto sequences = new char *[seqs_amount];
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        sequences[iii] = new char[MAX_CHARS];
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);
    int answer = 0;

    answer = (*func)(TEST_SMALL, seqs_amount, sequences);
    EXPECT_EQ(answer, SMALL_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_MEDIUM, seqs_amount, sequences);
    EXPECT_EQ(answer, MEDIUM_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_BIG, seqs_amount, sequences);
    EXPECT_EQ(answer, BIG_CORRECT_ANSWER) << " Wrong answer";

    delete_sequences(sequences, seqs_amount);
}

TEST(threads, find_indices_threads) {
    void *dl_handle = dlopen(SUBSEQ_LIB, RTLD_LAZY);
    if (!dl_handle) GTEST_FAIL();

    int (*func)(const char *const, const size_t, char **) = NULL;
    *(void **) (&func) = dlsym(dl_handle, "find_indices_threads");
    if (!func) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    if (access(TEST_SMALL, R_OK) == -1 &&
        access(TEST_MEDIUM, R_OK) == -1 &&
        access(TEST_BIG, R_OK) == -1) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    size_t seqs_amount = 2;
    auto sequences = new char *[seqs_amount];
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        sequences[iii] = new char[MAX_CHARS];
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);
    int answer = 0;

    answer = (*func)(TEST_SMALL, seqs_amount, sequences);
    EXPECT_EQ(answer, SMALL_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_MEDIUM, seqs_amount, sequences);
    EXPECT_EQ(answer, MEDIUM_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_BIG, seqs_amount, sequences);
    EXPECT_EQ(answer, BIG_CORRECT_ANSWER) << " Wrong answer";

    delete_sequences(sequences, seqs_amount);
}

TEST(linear, find_indices_linear) {
    void *dl_handle = dlopen(SUBSEQ_LIB, RTLD_LAZY);
    if (!dl_handle) GTEST_FAIL();

    int (*func)(const char *const, const size_t, char **) = NULL;
    *(void **) (&func) = dlsym(dl_handle, "find_indices_linear");
    if (!func) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    if (access(TEST_SMALL, R_OK) == -1 &&
        access(TEST_MEDIUM, R_OK) == -1 &&
        access(TEST_BIG, R_OK) == -1) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    size_t seqs_amount = 2;
    auto sequences = new char *[seqs_amount];
    for (size_t iii = 0; iii < seqs_amount; iii++) {
        sequences[iii] = new char[MAX_CHARS];
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);
    int answer = 0;

    answer = (*func)(TEST_SMALL, seqs_amount, sequences);
    EXPECT_EQ(answer, SMALL_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_MEDIUM, seqs_amount, sequences);
    EXPECT_EQ(answer, MEDIUM_CORRECT_ANSWER) << " Wrong answer";

    answer = (*func)(TEST_BIG, seqs_amount, sequences);
    EXPECT_EQ(answer, BIG_CORRECT_ANSWER) << " Wrong answer";

    delete_sequences(sequences, seqs_amount);
}

TEST(main, compare_approaches) {
    void *dl_handle = dlopen(SUBSEQ_LIB, RTLD_LAZY);
    if (!dl_handle) GTEST_FAIL();

    int (*linear)(const char *const, const size_t, char **) = NULL;
    int (*proc)(const char *const, const size_t, char **) = NULL;
    int (*threads)(const char *const, const size_t, char **) = NULL;
    *(void **) (&linear) = dlsym(dl_handle, "find_indices_linear");
    *(void **) (&proc) = dlsym(dl_handle, "find_indices_proc");
    *(void **) (&threads) = dlsym(dl_handle, "find_indices_threads");
    if (!linear) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    if (access(TEST_SMALL, R_OK) == -1 &&
        access(TEST_MEDIUM, R_OK) == -1 &&
        access(TEST_BIG, R_OK) == -1) {
        dlclose(dl_handle);
        GTEST_FAIL();
    }

    size_t seqs_amount = 2;
    auto sequences = new char *[seqs_amount];
    for (size_t iii = 0; iii < seqs_amount; iii++)
        sequences[iii] = new char[MAX_CHARS];
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);
    int linear_answer = 0;
    int proc_answer = 0;
    int threads_answer = 0;

    linear_answer = (*linear)(TEST_SMALL, seqs_amount, sequences);
    proc_answer = (*proc)(TEST_SMALL, seqs_amount, sequences);
    threads_answer = (*threads)(TEST_SMALL, seqs_amount, sequences);
    EXPECT_EQ((linear_answer & proc_answer & threads_answer), SMALL_CORRECT_ANSWER);

    linear_answer = (*linear)(TEST_MEDIUM, seqs_amount, sequences);
    proc_answer = (*proc)(TEST_MEDIUM, seqs_amount, sequences);
    threads_answer = (*threads)(TEST_MEDIUM, seqs_amount, sequences);
    EXPECT_EQ((linear_answer & proc_answer & threads_answer), MEDIUM_CORRECT_ANSWER);

    linear_answer = (*linear)(TEST_BIG, seqs_amount, sequences);
    proc_answer = (*proc)(TEST_BIG, seqs_amount, sequences);
    threads_answer = (*threads)(TEST_BIG, seqs_amount, sequences);
    EXPECT_EQ((linear_answer & proc_answer & threads_answer), BIG_CORRECT_ANSWER);

    delete_sequences(sequences, seqs_amount);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
