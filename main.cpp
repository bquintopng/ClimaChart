#include <matplot/matplot.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "DataPoint.h"

double convertCoord(std::string c){

    if(!c.empty()){
        double coord = stod(c);
        char cardinal = c.back();

        if(cardinal == 'S' || cardinal == 'W')
            coord = -coord;

        return coord;
    }else return 0.0;
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
    std::vector<DataPoint> L;
    std::vector<DataPoint> R;

    for(int i = 0; i < leftArray; i++) {
        L.push_back(data[left+i]);
    }
    for(int j = 0; j < rightArray; j++) {
        R.push_back(data[middle+1+j]);
    }
    int leftP, rightP, mergedP;
    leftP = 0;
    rightP = 0;
    mergedP = left;

    while(leftP < leftArray && rightP < rightArray) {
        if(L[leftP]->avgTemp <= R[rightP]->avgTemp) {
            data[mergedP++] = L[leftP++];
        } else {
            data[mergedP++] = R[rightP++];
        }
    }
    while(leftP < leftArray) {
        data[mergedP++] = L[leftP++];
    }
    while(rightP < rightArray) {
        data[mergedP++] = R[rightP++];

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

int main(){
    using namespace matplot;

    std::vector<double> lat;
    std::vector<double> lon;
    std::vector<DataPoint> data;
    std::vector<DataPoint> mergeSorted;
    //readFile("archive/GlobalLandTemperaturesByMajorCity.csv", data);
    readFile("archive/GlobalLandTemperaturesByCity.csv", data);
    readFile("archive/GlobalLandTemperaturesByCity.csv", mergeSorted);
    mergeSortCities(mergeSorted,0,mergeSorted.size()-1);
    for(int i = 0; i < 100; i++) {
        cout << mergeSorted[i]->coordinate << " " << mergeSorted[i]->avgTemp<< endl;
    }

    for(auto& dp : data){
        lat.push_back(dp.coordinate.first);
        lon.push_back(dp.coordinate.second);
    }
    //std::vector<double> lon = iota(-170, 1, 170);
    //std::vector<double> lat = transform(lon, [](double x) { return x*rand(-0.5,0.5); });
    geodensityplot(lat, lon);
    //->marker_style(line_spec::marker_style::circle);

    show();
    return 0;
}
