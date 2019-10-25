#include <cstring>

#include "gtest/gtest.h"

extern "C" {
    #include "DynArray.h"
    #include "threads.h"
}

TEST(threads_small, find_indices) {

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
}

TEST(threads_medium, find_indices) {

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
}
/// Had to comment out due to GitHub restrictions
/*TEST(threads_big, find_indices) {

    char filename[MAX_CHARS] = "../test_files/big.txt";
    size_t seqs_amount = 2;
    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
        sequences[iii] = (char *) calloc(MAX_CHARS, sizeof(char));
    }
    strncpy(sequences[0], "wi", MAX_CHARS);
    strncpy(sequences[1], "o", MAX_CHARS);

    int answer = find_indices(filename, seqs_amount, sequences);

    EXPECT_EQ(answer, 4191001) << " Wrong answer";
}*/

//TEST(threads_big_and_many, find_indices) {
//
//    char filename[MAX_CHARS] = "../test_files/big.txt";
//    size_t seqs_amount = 10;
//    char **sequences = (char **)calloc(seqs_amount, sizeof(char*));
//    for ( size_t iii = 0; iii < seqs_amount; iii++ ) {
//        sequences[iii] = (char *) calloc(MAX_CHARS, sizeof(char));
//    }
//    strncpy(sequences[0], "wi", MAX_CHARS);
//    strncpy(sequences[1], "o", MAX_CHARS);
//    strncpy(sequences[2], "lol", MAX_CHARS);
//    strncpy(sequences[3], "owo", MAX_CHARS);
//    strncpy(sequences[4], "oop", MAX_CHARS);
//    strncpy(sequences[5], "cpp", MAX_CHARS);
//    strncpy(sequences[6], "orm", MAX_CHARS);
//    strncpy(sequences[7], "posix", MAX_CHARS);
//    strncpy(sequences[8], "std", MAX_CHARS);
//    strncpy(sequences[9], "boost", MAX_CHARS);
//
//    int answer = find_indices(filename, seqs_amount, sequences);
//
//    EXPECT_EQ(answer, 4226436) << " Wrong answer";
//}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
