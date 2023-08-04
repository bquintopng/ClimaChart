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
        geodensityplot(latitude, longitude, weight);
        color_array blue_water = {0.f, 0.4f, 0.61f, 0.76f};
        gca()->color(blue_water);
        show();
    }
};



#endif //CLIMACHART_HEATMAP_H
