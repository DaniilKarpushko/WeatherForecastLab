#include "Interface.h"

class DayslineWeather{
public:
    std::string date;
    std::pair<double ,int> forecast[4];//temperature&weathercode
};

class CityWeather{
public:
    City city;
    std::vector<DayslineWeather> days;
    int num_of_days{0};
    void Get_Info(const int& num_of_days_);

    void add_day();
};

class WeatherForecast{
public:
    std::vector<CityWeather> cities_forecast;
    std::map<int,std::string> weathercodes = {{0,"Clear Sky"},{1,"Cloudy"},{2,"Cloudy"},{3,"Cloudy"},
                                              {45,"Fog"},{48,"Fog"},{51,"Drizzle"},{53,"Drizzle"},
                                              {55,"Drizzle"},{56,"Freezing Drizzle"},{57,"Freezing Drizzle"},
                                              {61,"Slight Rain"},{63,"Moderate Rain"},{65,"Heavy rain"},
                                              {66,"Freezing Rain"},{67,"Freezing Rain"},{71,"Snowfall"},
                                              {73,"Snowfall"},{75,"Snowfall"},{77,"Snow grains"},
                                              {80,"Shower"},{81,"Shower"},{82,"Shower"},{95,"Thunderstorm"},
                                              {96,"Thunderstorm"},{99, "Thunderstorm"},{85,"Snow Shower"},
                                              {86,"Snow shower"}};
    void Set_Forecast(const Config& conf);

    void print_weather(int pointer,int daypointer);

    void add_day_city(int pointer);

    void print_table(const int& pointer,const int& daypointer, const int& num_of_days);

    void update_data(int pointer);
};