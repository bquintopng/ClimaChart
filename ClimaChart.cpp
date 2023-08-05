//
// Created by Benjamin Quinto on 8/4/23.
//

#include "ClimaChart.h"
double ClimaChart::convertCoord(std::string c){

    if(!c.empty()){
        double coord = stod(c);
        char cardinal = c.back();

        if(cardinal == 'S' || cardinal == 'W')
            coord = -coord;

        return coord;
    }else return 0.0;
}
std::vector<std::pair<std::pair<double,double>, double>> ClimaChart::calculateTempChange(){
    std::map<std::pair<double,double>, std::vector<DataPoint>> record;
    for(auto& dp : data){
        record[dp.coordinate].push_back(dp);
    }

    std::vector<std::pair<std::pair<double,double>, double>> avgTempChange;
    for (const auto& entry : record) {
        const std::vector<DataPoint>& locationData = entry.second;

        // Sort the location data by year in ascending order
        std::vector<DataPoint> sortedData = locationData;
        std::sort(sortedData.begin(), sortedData.end(), [](const DataPoint& a, const DataPoint& b) {
            int year1 = stoi(a.date.substr(0,4));
            int month1 = stoi(a.date.substr(5,7));
            int year2 = stoi(b.date.substr(0,4));
            int month2 = stoi(b.date.substr(5,7));
            return year1 <= year2 && month1<month2;
        });

        // Calculate temperature change and rate of change for the location
        if (sortedData.size() > 1) {
            double tempChange = sortedData.back().avgTemp - sortedData.front().avgTemp;
            int yearChange = stoi(sortedData.back().date.substr(0,4)) - stoi(sortedData.front().date.substr(0,4));

            // Calculate the temperature rate of change per year
            double tempRateOfChange = tempChange / static_cast<double>(yearChange);

            // Store the result as a pair of location and temperature rate of change
            avgTempChange.push_back({entry.first, tempRateOfChange});
        }
    }
    std::cout<<avgTempChange.size();
    return avgTempChange;

}

bool ClimaChart::readFile(){
    std::string filename;

    std::cout<<"Choose File: \n1. Major Cities\n2. All Cities\n";
    int choice=2; //tried using cin but broke the program for some reason
    //std::cin>>choice;
    if(choice == 1)
        filename = "archive/GlobalLandTemperaturesByMajorCity.csv";
    else if (choice == 2)
        filename = "archive/GlobalLandTemperaturesByCity.csv";
    else{
        std::cout<<"Error: Invalid";
        return false;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream inputFile(filename);

    // Line Format:
    // Date, Avg Temp, Uncertainty, City, Country, latitude(NS), longitude(EW)
    std::string line = "";
    getline(inputFile, line);
    while(getline(inputFile, line)){
        std::string date;
        double temperature = 0.0;
        double uncertainty = 0.0;
        std::string cityname;
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
        getline(ss, cityname, ',');
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

    auto end_time = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout<<"Time to read "<<data.size()<<" entries: "<< runtime <<" ms" <<std::endl;

    return true;
}

void ClimaChart::merge(std::vector<DataPoint>& data, int left, int middle, int right) {

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

int ClimaChart::partition(std::vector<DataPoint>& data, int low, int high) {
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