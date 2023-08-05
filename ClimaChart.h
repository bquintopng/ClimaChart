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
    std::vector<std::pair<std::pair<double, double>, double>> tempRateOfChange;

    /// Merge Sort
    void merge(std::vector<DataPoint>& data, int left, int middle, int right);
    void mergeYear(std::vector<DataPoint>& data, int left, int middle, int right);

    void mergeSortCities(std::vector<DataPoint>& data, int left, int right);
    void mergeSortYear(std::vector<DataPoint>& data, int left, int right);

    /// Quick Sort
    int partition(std::vector<DataPoint>& data, int low, int high);
    int partitionByTime(std::vector<DataPoint>& data, int low, int high);

    void quickSort(std::vector<DataPoint> &data, int low, int high);
    void quickSortByTime(std::vector<DataPoint>& data, int low, int high);

    double convertCoord(std::string c);

public:
    std::vector<double> latitude;
    std::vector<double> longitude;
    std::vector<double> weight;

    void calculateTempChange(int sortchoice);
    void calculateLinearRegression(int sortchoice);
    void readFile(std::string filename);

    void reset() {
        data.clear();
        latitude.clear();
        longitude.clear();
        weight.clear();
    }
};

