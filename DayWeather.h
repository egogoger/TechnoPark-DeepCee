#ifndef IS1_DAYWEATHER_H
#define IS1_DAYWEATHER_H

typedef enum precipitations {
    NONE,
    CLOUDY,
    FOG,
    RAIN,
    SHOWER,
    THUNDERSTORM,
    SNOW,
    TOTAL_AMOUNT,
} precipitations;

typedef struct DayWeather {
    double temperature;
    precipitations precipitation;
    double wind_speed;
} DayWeather;

char* print_weather_forecast(DayWeather today);

#endif //IS1_DAYWEATHER_H

#define MAX_LEN 100
