#include <cstring>

#include "gtest/gtest.h"

extern "C" {
    #include "DynArray.h"
    #include "linear.h"
}

TEST(linear_small, find_indices) {
    char filename[MAX_CHARS] = "../test_files/small.txt";
    size_t seqs_amount = 2;
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        sequences[iii] = (char *) calloc(MAX_CHARS, sizeof(char));
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);

    int answer = find_indices(filename, seqs_amount, sequences);

    EXPECT_EQ(answer, 4) << " Wrong answer";

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        free(sequences[iii]);
    }
    free(sequences);
}

TEST(linear_medium, find_indices) {
    char filename[MAX_CHARS] = "../test_files/medium.txt";
    size_t seqs_amount = 2;
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        sequences[iii] = (char *) calloc(MAX_CHARS, sizeof(char));
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);

    int answer = find_indices(filename, seqs_amount, sequences);

    EXPECT_EQ(answer, 41633) << " Wrong answer";

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        free(sequences[iii]);
    }
    free(sequences);
}

TEST(linear_big, find_indices) {
    char filename[MAX_CHARS] = "../test_files/big.txt";
    size_t seqs_amount = 2;
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        sequences[iii] = (char *) calloc(MAX_CHARS, sizeof(char));
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);

    int answer = find_indices(filename, seqs_amount, sequences);

    EXPECT_EQ(answer, 4183196) << " Wrong answer";

    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        free(sequences[iii]);
    }
    free(sequences);
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
