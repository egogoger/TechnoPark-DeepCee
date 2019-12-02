#include <cstdlib>
#include "gtest/gtest.h"

extern "C" {
#include "DayWeather.h"
#include "DynArray.h"
#include "stats.h"
}

#define DAYS 20

bool compare_floats(float l, float r) {
    if (l > r) {
        if (l - r < 0.001) {
            return true;
        }
    } else {
        if (r - l < 0.001) {
            return true;
        }
    }
    return false;
}

TEST(DynArray, is_empty) {
    DynArray *temp_da = new_DynArray();

    EXPECT_EQ(is_empty(temp_da), size_t(0));

    delete_DynArray(temp_da);
}

TEST(DynArray, expand) {
    DynArray *temp_da = new_DynArray();

    EXPECT_EQ(temp_da->buffer_size, size_t(DAYS_IN_WEEK));
    expand(temp_da);
    EXPECT_EQ(temp_da->buffer_size, size_t(DAYS_IN_WEEK * 2));

    delete_DynArray(temp_da);
}

TEST(DynArray, add) {
    DynArray *temp_da = new_DynArray();

    auto temp_day = (DayWeather *) calloc(1, sizeof(DayWeather));
    temp_day->temperature = 20;
    temp_day->precipitation = THUNDERSTORM;
    temp_day->wind_speed = 12.5;

    add(temp_da, temp_day);

    EXPECT_EQ(temp_da->buffer[0].temperature, 20);
    EXPECT_EQ(temp_da->buffer[0].precipitation, THUNDERSTORM);
    EXPECT_EQ(temp_da->buffer[0].wind_speed, 12.5);

    free(temp_day);
    delete_DynArray(temp_da);
}

TEST(DynArray, pop) {
    DynArray *temp_da = new_DynArray();

    auto temp_day = (DayWeather *) calloc(1, sizeof(DayWeather));
    temp_day->temperature = 20;
    temp_day->precipitation = THUNDERSTORM;
    temp_day->wind_speed = 12.5;

    add(temp_da, temp_day);

    DayWeather test_day = pop(temp_da);

    EXPECT_EQ(is_empty(temp_da), size_t(0));

    EXPECT_EQ(test_day.temperature, 20);
    EXPECT_EQ(test_day.precipitation, THUNDERSTORM);
    EXPECT_EQ(test_day.wind_speed, 12.5);

    free(temp_day);
    delete_DynArray(temp_da);
}

TEST(DynArray, Multiple_add_pop) {
    DynArray *temp_da = new_DynArray();

    size_t count = 100000;
    auto temp_day = (DayWeather *) calloc(1, sizeof(DayWeather));
    for (size_t iii = 0; iii < count; iii++) {
        temp_day->temperature = count % 10000 / 52.5;
        temp_day->precipitation = THUNDERSTORM;
        temp_day->wind_speed = count % 1000 / 73.9;
        add(temp_da, temp_day);
        pop(temp_da);
    }
    free(temp_day);

    EXPECT_EQ(is_empty(temp_da), size_t(0));

    delete_DynArray(temp_da);
}

TEST(DayWeather, LogicTest) {
    DynArray *week = new_DynArray();

    auto temp_day = (DayWeather *) calloc(1, sizeof(DayWeather));
    for (size_t iii = 0; iii < DAYS; iii++) {
        temp_day->temperature = iii % DAYS / 1.5;
        temp_day->precipitation = THUNDERSTORM;
        temp_day->wind_speed = iii % DAYS / 1.9;
        add(week, temp_day);
    }
    free(temp_day);

    EXPECT_TRUE(compare_floats(min_temperature(week, float(DAYS)), 0));
    EXPECT_TRUE(compare_floats(max_temperature(week, float(DAYS)), 12.666));
    EXPECT_TRUE(compare_floats(average_temperature(week, float(DAYS)), 6.333));
    EXPECT_EQ(amount_of_precipitations(week, DAYS), int(DAYS));

    delete_DynArray(week);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}