#include "winsock2.h"
#include <iostream>
#include "Windows.h"
#include <unistd.h>
#include "WeatherForecast.h"

void forecast_controller(WeatherForecast& weather, Config& conf){
    int pointer = -1;
    int daypointer = 0;
    long long timer = 0;
    bool start = true;
    while(true) {
        bool city_next = GetKeyState('N') & 0x8000;
        bool city_prev = GetKeyState('P') & 0x8000;
        bool escape = GetKeyState(VK_ESCAPE) & 0x8000;
        bool plus = GetKeyState(VK_OEM_PLUS) & 0x8000;
        bool minus = GetKeyState(VK_OEM_MINUS) & 0x8000;
        if(timer == conf.frequency * 200000) {
            system("cls");
            weather.update_data(pointer % weather.cities_forecast.size());
            weather.print_weather(pointer % weather.cities_forecast.size(), daypointer);
            timer = 0;
        }
        if(city_next || start){
            system("cls");
            daypointer = 0;
            pointer++;
            daypointer = weather.cities_forecast[pointer%weather.cities_forecast.size()].num_of_days-1;
            weather.print_weather(pointer%weather.cities_forecast.size(),daypointer);
            sleep(1);
            start = false;
        }
        if(city_prev){
            system("cls");
            daypointer = 0;
            pointer--;
            daypointer = weather.cities_forecast[pointer%weather.cities_forecast.size()].num_of_days-1;
            weather.print_weather(pointer%weather.cities_forecast.size(),daypointer);
            sleep(1);
        }

        if(plus){
            daypointer++;
            weather.add_day_city(pointer%weather.cities_forecast.size());
            weather.print_table(pointer%weather.cities_forecast.size(),daypointer,weather.cities_forecast[pointer].num_of_days);
            sleep(1);
        }

        if(minus){
            system("cls");
            daypointer--;
            weather.cities_forecast[pointer%weather.cities_forecast.size()].num_of_days--;
            weather.print_weather(pointer%weather.cities_forecast.size(),daypointer);
            sleep(1);
        }

        if(escape){
            system("cls");
            break;
        }

        timer++;
    }
}