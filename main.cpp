#include "MVC.h"

int main(){
    Config conf;
    conf.Get_Conf("1.txt");
    WeatherForecast w;
    w.Set_Forecast(conf);
    forecast_controller(w,conf);

    return 0;
}
