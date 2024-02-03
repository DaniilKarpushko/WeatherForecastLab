#include "WeatherForecast.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>
#include <iostream>

using namespace ftxui;

void CityWeather::Get_Info(const int& num_of_days_) {
    cpr::Response resp = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                             cpr::Parameters{{"latitude" , std::to_string(city.latitude)},
                                             {"longitude", std::to_string(city.longitude)},
                                             {"forecast_days", std::to_string(num_of_days_)},
                                             {"hourly", "temperature_2m"},
                                             {"hourly", "weathercode"}});
    auto json_parse = nlohmann::json::parse(resp.text);

    std::vector<double> temperature = json_parse["hourly"]["temperature_2m"];
    std::vector<int> weathercode = json_parse["hourly"]["weathercode"];
    num_of_days = num_of_days_;
//
    for(int i = 0; i < num_of_days_; ++i){
        DayslineWeather weather;
        weather.date = json_parse["hourly"]["time"][i*24];
        weather.forecast[0].first = temperature[7+24*i];
        weather.forecast[0].second = weathercode[7+24*i];
        weather.forecast[1].first = temperature[13+24*i];
        weather.forecast[1].second = weathercode[13+24*i];
        weather.forecast[2].first = temperature[18+24*i];
        weather.forecast[2].second = weathercode[18+24*i];
        weather.forecast[3].first = temperature[22+24*i];
        weather.forecast[3].second = weathercode[22+24*i];
        if(days.size() < num_of_days_) {
            days.emplace_back(weather);
        }else{
            days[i] = weather;
        }
    }
}

void CityWeather::add_day() {
    //if(num_of_days < 16 && num_of_days == days.size()) {
        cpr::Response resp = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                      cpr::Parameters{{"latitude",      std::to_string(city.latitude)},
                                                      {"longitude",     std::to_string(city.longitude)},
                                                      {"forecast_days", std::to_string(days.size() + 1)},
                                                      {"hourly",        "temperature_2m"},
                                                      {"hourly",        "weathercode"}});
        auto json_parse = nlohmann::json::parse(resp.text);
        std::vector<double> temperature = json_parse["hourly"]["temperature_2m"];
        std::vector<int> weathercode = json_parse["hourly"]["weathercode"];

        DayslineWeather weather;
        weather.date = json_parse["hourly"]["time"][(num_of_days - 1) * 24];
        weather.forecast[0].first = temperature[7 + 24 * (num_of_days - 1)];
        weather.forecast[0].second = weathercode[7 + 24 * (num_of_days - 1)];
        weather.forecast[1].first = temperature[13 + 24 * (num_of_days - 1)];
        weather.forecast[1].second = weathercode[13 + 24 * (num_of_days - 1)];
        weather.forecast[2].first = temperature[18 + 24 * (num_of_days - 1)];
        weather.forecast[2].second = weathercode[18 + 24 * (num_of_days - 1)];
        weather.forecast[3].first = temperature[22 + 24 * (num_of_days - 1)];
        weather.forecast[3].second = weathercode[22 + 24 * (num_of_days - 1)];
        days.emplace_back(weather);
    //}

}

void WeatherForecast::Set_Forecast(const Config& conf) {
    for(int i = 0; i < conf.places.size(); ++i) {
        CityWeather c_weather;
        c_weather.city = conf.places[i];
        c_weather.Get_Info(conf.number_of_days);
        cities_forecast.emplace_back(c_weather);
    }
}

void WeatherForecast::print_weather(int pointer,int daypointer) {
    auto cell = [](std::string t) { return ftxui::text(t) | ftxui::border; };
    auto doc = ftxui::gridbox(
            {{cell("latitude: " + std::to_string(cities_forecast[pointer].city.latitude)
                   + " longitude: " + std::to_string(cities_forecast[pointer].city.longitude)),
              cell(cities_forecast[pointer].city.name) | color(Color::Green),
              cell("country: " + cities_forecast[pointer].city.country)}});
    Screen screen = ftxui::Screen::Create(
            ftxui::Dimension::Full(),
            ftxui::Dimension::Fit(doc));
    Render(screen, doc);
    screen.Print();

    for (int i = daypointer; i >= 0 ; i--) {
        print_table(pointer, daypointer-i, cities_forecast[pointer].num_of_days);
    }
    std::cout<<'\n';
}

void WeatherForecast::print_table(const int& pointer,const int& daypointer, const int& num_of_days) {
    Element table = window(text(cities_forecast[pointer].days[daypointer%num_of_days].date.substr(0,10)),
    {hbox(window(text("Morning"),hbox(vbox(text("Temperature: "),
    text(std::to_string(cities_forecast[pointer].days[daypointer%num_of_days].forecast[0].first).substr(0,4)),
    text("Weather: "),text(weathercodes[cities_forecast[pointer].days[daypointer%num_of_days].forecast[0].second])),
    separator(),
    text("picture"))) | flex, window(text("Afternoon"),hbox(vbox(text("Temperature: "),
    text(std::to_string(cities_forecast[pointer].days[daypointer%num_of_days].forecast[1].first).substr(0,4)),
    text("Weather: "),text(weathercodes[cities_forecast[pointer].days[daypointer%num_of_days].forecast[1].second])),
    separator(),
    text("picture"))) | flex ,window(text("Evening"),hbox(vbox(text("Temperature: "),
    text(std::to_string(cities_forecast[pointer].days[daypointer%num_of_days].forecast[2].first).substr(0,4)),
    text("Weather: "),text(weathercodes[cities_forecast[pointer].days[daypointer%num_of_days].forecast[2].second])),
    separator(),
    text("picture"))) | flex, window(text("Night"),hbox(vbox(text("Temperature: "),
    text(std::to_string(cities_forecast[pointer].days[daypointer%num_of_days].forecast[3].first).substr(0,4)),
    text("Weather: "),text(weathercodes[cities_forecast[pointer].days[daypointer%num_of_days].forecast[3].second])),
    separator(),
    text("picture"))) | flex)});
    Screen screen = ftxui::Screen::Create(
            Dimension::Full(),
            Dimension::Fit(table));
    Render(screen, table);
    screen.Print();
    std::cout<<'\n';
}

void WeatherForecast::add_day_city(int pointer) {
    if(cities_forecast[pointer].num_of_days<16) {
        cities_forecast[pointer].num_of_days++;
        if(cities_forecast[pointer].num_of_days == cities_forecast[pointer].days.size() + 1) {
            cities_forecast[pointer].add_day();
        }
    }
    return;
}

void WeatherForecast::update_data(int pointer) {
    cities_forecast[pointer].Get_Info(cities_forecast[pointer].num_of_days);
}


