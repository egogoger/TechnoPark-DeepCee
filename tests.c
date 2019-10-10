#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "DayWeather.h"
#include "DynArray.h"

void test_DynArray() {

    // new_DynArray() + delete_DynArray()
    {
        DynArray *temp_da = new_DynArray();

        delete_DynArray(temp_da);
    }
    // Expand()
    {
        DynArray *temp_da = new_DynArray();

        assert( temp_da->buffer_size == DAYS_IN_WEEK );
        Expand(temp_da);
        assert( temp_da->buffer_size == DAYS_IN_WEEK * 2 );

        delete_DynArray(temp_da);
    }

    // IsEmpty()
    {
        DynArray *temp_da = new_DynArray();

        assert( IsEmpty(temp_da) == 0 );

        delete_DynArray(temp_da);
    }

    // Add() + IsEmpty()
    {
        DynArray *temp_da = new_DynArray();

        DayWeather *temp_day = (DayWeather *)calloc(1, sizeof(DayWeather));
        temp_day->temperature = 20;
        temp_day->precipitation = THUNDERSTORM;
        temp_day->wind_speed = 12.5;

        Add( temp_da, temp_day );
        free( temp_day );

        assert( temp_da->buffer[0].temperature == 20
                && temp_da->buffer[0].precipitation == THUNDERSTORM
                && temp_da->buffer[0].wind_speed == 12.5);

        assert( IsEmpty( temp_da ) != 0 );

        delete_DynArray(temp_da);
    }

    // Pop()
    {
        DynArray *temp_da = new_DynArray();

        DayWeather *temp_day = (DayWeather *)calloc(1, sizeof(DayWeather));
        temp_day->temperature = 20;
        temp_day->precipitation = THUNDERSTORM;
        temp_day->wind_speed = 12.5;

        Add( temp_da, temp_day );
        free( temp_day );

        DayWeather test_day = Pop(temp_da);
        assert( test_day.temperature == 20
                && test_day.precipitation == THUNDERSTORM
                && test_day.wind_speed == 12.5);

        assert( IsEmpty( temp_da ) == 0 );

        delete_DynArray(temp_da);
    }

    // Multiple Add and Pop
    {
        DynArray *temp_da = new_DynArray();

        size_t count = 100000;
        DayWeather *temp_day = (DayWeather *)calloc(1, sizeof(DayWeather));
        for (size_t iii = 0; iii < count; iii++) {
            temp_day->temperature = count % 10000 / 52.5 ;
            temp_day->precipitation = THUNDERSTORM;
            temp_day->wind_speed = count  % 1000 / 73.9;
            Add( temp_da, temp_day );
            Pop( temp_da );
        }
        free( temp_day );

        assert( IsEmpty( temp_da ) == 0 );

        delete_DynArray(temp_da);
    }
    puts("OK");
}

int main() {
    test_DynArray();

    return 0;
}