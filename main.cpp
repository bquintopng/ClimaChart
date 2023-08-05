#include <matplot/matplot.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include "ClimaChart.h"
#include <chrono>
double convertCoord(std::string c){

    if(!c.empty()){
        double coord = stod(c);
        char cardinal = c.back();

        if(cardinal == 'S' || cardinal == 'W')
            coord = -coord;

        return coord;
    }else return 0.0;
}
std::vector<std::pair<std::pair<double, double>, double>> calculateAverageTempChange(const std::vector<DataPoint>& data) {
    std::map<std::pair<double, double>, std::pair<double, int>> tempSumCountMap;

    for (const auto& dp : data) {
        auto coords = dp.coordinate;
        double temperature = dp.avgTemp;

        // Check if the location exists in the map
        auto it = tempSumCountMap.find(coords);
        if (it != tempSumCountMap.end()) {
            // Location found, update the sum and count
            it->second.first += temperature;
            it->second.second++;
        } else {
            // Location not found, add it to the map
            tempSumCountMap[coords] = {temperature, 1};
        }
    }

    // Calculate the average temperature change for each location and store in a vector
    std::vector<std::pair<std::pair<double, double>, double>> avgTempChangeVector;
    for (const auto& entry : tempSumCountMap) {
        double avgTempChange = entry.second.first / static_cast<double>(entry.second.second);
        avgTempChangeVector.push_back({entry.first, avgTempChange});
    }
    return avgTempChangeVector;
}

void readFile(std::string filename, std::vector<DataPoint>& data){
    std::ifstream inputFile(filename);

    // Line Format:
    // Date, Avg Temp, Uncertainty, City, Country, latitude(NS), longitude(EW)
    std::string line = "";
    getline(inputFile, line);
    while(getline(inputFile, line)){
        std::string date;
        double temperature = 0.0;
        double uncertainty = 0.0;
        std::string country;
        std::string city;
        double lat;
        double lon;
        std::string tempString;
        std::stringstream ss(line);


        // Date
        getline(ss, date, ',');
        // Temperature
        getline(ss, tempString, ',');
        if(!tempString.empty()) temperature = stod(tempString);
        // Uncertainty
        getline(ss, tempString, ',');
        if(!tempString.empty()) uncertainty = stod(tempString);

        // Country and City:
        getline(ss, tempString, ',');
        getline(ss, tempString, ',');

        // Latitude & Longitude
        getline(ss, tempString, ',');
        lat = convertCoord(tempString);
        getline(ss, tempString, '\r');
        lon = convertCoord(tempString);
        std::pair<double, double> coordinate = {lat, lon}; // latitude always precedes longitude


        DataPoint dp(coordinate, date, temperature, uncertainty);

        data.push_back(dp);
    }
}

void mergeCities(std::vector<DataPoint>& data, int left, int middle, int right) {

    int leftArray = middle - left + 1;
    int rightArray = right - middle;
    std::vector<DataPoint> X;
    std::vector<DataPoint> Y;

    for(int i = 0; i < leftArray; i++) {
        X.push_back(data[left+i]);
    }
    for(int j = 0; j < rightArray; j++) {
        Y.push_back(data[middle+1+j]);
    }
    int leftP, rightP, mergedP;
    leftP = 0;
    rightP = 0;
    mergedP = left;

    while(leftP < leftArray && rightP < rightArray) {
        if(X[leftP].avgTemp <= Y[rightP].avgTemp) {
            data[mergedP] = X[leftP];
            leftP++;
        } else {
            data[mergedP] = Y[rightP];
            rightP++;
        }
        mergedP++;
    }
    while(leftP < leftArray) {
        data[mergedP] = X[leftP];
        leftP++;
        mergedP++;
    }
    while(rightP < rightArray) {
        data[mergedP] = Y[rightP];
        rightP++;
        mergedP++;
    }
}

void mergeSortCities(std::vector<DataPoint>& data, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        mergeSortCities(data, left, middle);
        mergeSortCities(data, middle+1, right);

        mergeCities(data, left, middle, right);

    }
}

int partition(std::vector<DataPoint>& data, int low, int high) {
    double pivot = data[high].avgTemp;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (data[j].avgTemp <= pivot) {
            i++;
            std::swap(data[i], data[j]);
        }
    }
    std::swap(data[i + 1], data[high]);
    return i + 1;
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

int main(){
    using namespace matplot;

    std::vector<double> lat;
    std::vector<double> lon;
    std::vector<double> weight;
    std::vector<DataPoint> data;

    /*
     * THIS WILL EVENTUALLY BECOME A CONSOLE DRIVER USER FILE SELECT
     * (they will pick major or all cities and it also prints runtime for file
     */

    std::cout<<"Choose File: \n1. Major Cities\n2. All Cities\n";
    int choice = 1; //tried using cin but broke the program for some reason
    //std::cin>>choice;
    auto start_time = std::chrono::high_resolution_clock::now();
    if(choice == 1)
        readFile("archive/GlobalLandTemperaturesByMajorCity.csv", data);
    else if (choice == 2)
        readFile("archive/GlobalLandTemperaturesByCity.csv", data);
    else{
        std::cout<<"Error: Invalid";
        return 0;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout<<"Time to read "<<data.size()<<" entries: "<< runtime <<" ms" <<std::endl;

    mergeSortCities(data,0,data.size()-1);
    for(int i = 0; i < 100; i++) {
        data[i].Display();
    }

    std::vector<std::pair<std::pair<double, double>,double>> avg = calculateAverageTempChange(data);
    for(auto& dp : avg){
        lat.push_back(dp.first.first);
        lon.push_back(dp.first.second);
        weight.push_back(dp.second);
    }

    HeatMap(lat, lon, weight);
    quickSort(data, 0, data.size() - 1);

    return 0;
}

