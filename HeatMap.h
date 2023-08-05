//
// Created by Benjamin Quinto on 8/3/23.
//

#ifndef CLIMACHART_HEATMAP_H
#define CLIMACHART_HEATMAP_H

#include <matplot/matplot.h>
#include <random>

using namespace matplot;


struct HeatMap {
    std::vector<double> latitude;
    std::vector<double> longitude;
    std::vector<double> weight;

    HeatMap(std::vector<double> lat, std::vector<double> lon, std::vector<double> w){
        latitude = lat;
        longitude = lon;
        weight = w;
    }

    void Display(){
        geodensityplot(latitude, longitude, weight);
        color_array blue_water = {0.f, 0.4f, 0.61f, 0.76f};
        gca()->color(blue_water);
        color_array terrain = {0.f, 0.71f, 0.65f, 0.59f};
        geoplot()->color(terrain);
        gca()->colormap(palette::hot());
        show();
        show();
    }

};



#endif //CLIMACHART_HEATMAP_H
