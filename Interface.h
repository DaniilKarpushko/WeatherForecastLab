#include "vector"
#include "fstream"
#include "nlohmann/json.hpp"
#include "cpr/cpr.h"



class City{
public:
    std::string name;
    std::string country;
    double latitude{0};
    double longitude{0};
    void Get_Points(){
        cpr::Response geo_point = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                           cpr::Header{{"X-Api-Key", "GZVHIqee8vH6Gy7tiwlYhQt2ScD5f0dLbcjWN4fI"}},
                                           cpr::Parameters{{"name",name}});
        auto json_parse = nlohmann::json::parse(geo_point.text);
        latitude = json_parse[0]["latitude"];
        longitude = json_parse[0]["longitude"];
        country = json_parse[0]["country"];
    }
};

class Config{
public:
    std::vector<City> places;
    int frequency;
    int number_of_days;

    void Get_Conf(const std::string& filename){
        std::fstream file(filename);
        nlohmann::basic_json json_parse;
        std::vector<std::string> city_names;

        file >> json_parse;

        frequency = json_parse[0]["frequency"];
        number_of_days = json_parse[0]["number_of_days"];
        city_names = json_parse[0]["names"];

        for(int i = 0; i < 2; ++i){
            City city;
            city.name = city_names[i];
            places.emplace_back(city);
            places[i].Get_Points();
        }
    };
};
