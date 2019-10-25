#include "gtest/gtest.h"

extern "C" {
    #include "DynArray.h"
}

TEST(DynArray, new_DynArray) {
    DynArray* da = new_DynArray();
    if ( !da ) {
        GTEST_FAIL() << " DynArray should be created";
    }
    EXPECT_EQ(delete_DynArray(da), EXIT_SUCCESS) << " Failed to clean structure";
}

TEST(DynArray, is_empty) {
    DynArray* da = new_DynArray();
    if( !is_empty(da) ) {
        GTEST_FAIL() << " DynArray should be empty";
    }
    add(da, 10);
    if( is_empty(da) ) {
        GTEST_FAIL() << " DynArray should not be empty";
    }
    delete_DynArray(da);
}

/// Also grow()
TEST(DynArray, add) {

    DynArray *temp_da = new_DynArray();

    int num = 10;

    for ( int iii = 0; iii < num; iii++ ) {
        add(temp_da, iii);
    }
    EXPECT_EQ(temp_da->real_size, num) << " Failed to add 10 elements";

    int previous_buffer_size = temp_da->buffer_size;
    grow(temp_da);
    if ( temp_da->buffer_size <= previous_buffer_size ) {
        GTEST_FAIL() << " DynArray should have grew in size";
    }

    delete_DynArray(temp_da);
}

TEST(DynArray, pop) {
    DynArray *temp_da = new_DynArray();

    int test_value = 10;

    add(temp_da, test_value);
    int previous_real_size = temp_da->real_size;

    int tmp = pop(temp_da);

    if ( tmp != test_value && previous_real_size != temp_da->real_size ) {
        GTEST_FAIL() << " DynArray should have popped an element";
    }

    delete_DynArray(temp_da);
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}