//
// Created by Benjamin Quinto on 8/4/23.
//
#include <iostream>
#include "HeatMap.h"
#include "DataPoint.h"
#include <chrono>

class ClimaChart {
private:
    std::vector<DataPoint> data;
    std::vector<double> latitude;
    std::vector<double> longitude;
    std::vector<double> weight;
    std::vector<std::pair<std::pair<double, double>, double>> tempRateOfChange;

    std::vector<std::pair<std::pair<double,double>, double>> calculateTempChange();

    void merge(std::vector<DataPoint>& data, int left, int middle, int right);
    double convertCoord(std::string c);

    int partition(std::vector<DataPoint>& data, int low, int high);

public:

    ClimaChart(){
        readFile();

        tempRateOfChange = calculateTempChange();
        for(auto& dp : tempRateOfChange){
            latitude.push_back(dp.first.first);
            longitude.push_back(dp.first.second);
            weight.push_back(dp.second);
        }

        HeatMap hm(latitude, longitude, weight);
        hm.Display();
    }

    void mergeSortCities(std::vector<DataPoint>& data, int left, int right) {
        if (left < right) {
            int middle = (left + right) / 2;
            mergeSortCities(data, left, middle);
            mergeSortCities(data, middle+1, right);

            merge(data, left, middle, right);

        }
    }
    void quickSort(std::vector<DataPoint> &data, int low, int high)
    {
        if (low < high)
        {
            int pi = partition(data, low, high);
            quickSort(data, low, pi - 1);
            quickSort(data, pi + 1, high);
        }
    }

    bool readFile();
};

