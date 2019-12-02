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

typedef struct {
    float temperature;
    precipitations precipitation;
    float wind_speed;
} DayWeather;

#endif // IS1_DAYWEATHER_H

#define MAX_LEN 100
